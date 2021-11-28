/**
* @file sock.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../../h/main.h"
#include "../../h/Other/sound.h"
#include "../../h/Debug/debugproc.h"
#include "../../h/Other/input.h"
#include "../../h/Object/ObjectClass/Interface/interface.h"
#include "../../h/Object/Player/player.h"
#include "../../h/Object/Item/item.h"
#include "../../h/Object/Bullet/bullet.h"
#include "../../h/Object/Scene/Scene.h"
#include "../../h/Map/field.h"
#include "../../h/Net/sock.h"

#pragma comment (lib, "Ws2_32.lib")


//STEP1	同端末通信	OK
//STEP2	LAN通信		OK
//STEP3	WAN通信		OK
//通信データのバッファ　この値とクライアント現在の値を比較して変更があれば最新データを送信する
//これまでだと1フレームで何回も同じデータを送信しているので、変更があったときに一度だけ送信する
//WANプロトコルのポート6432
//ポート開放
//最大で300バイトくらい
//sleep無くていい
//再送制御,照合制御がいる
//

struct SENDMSG_BUFF
{
	SENDMSG_BUFF() {
		PlayerPos = VEC3_ALL0;
		HoudaiRot = VEC3_ALL0;
		HoutouRot = VEC3_ALL0;
		HousinRot = VEC3_ALL0;
		BPos = VEC3_ALL0;
		ItemSyncUse = false;
	}

	//プレイヤーデータ
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 HoudaiRot;
	D3DXVECTOR3 HoutouRot;
	D3DXVECTOR3 HousinRot;
	int Vital;
	bool Morphing;
	D3DXVECTOR3 BPos;

	//アイテムデータ
	bool ItemSyncUse;

};

//player1-4,item1-20
class SubMsgName
{
public:
	void Name(string InName) { TargetName = InName; }
	string Name() { return TargetName; }

private:
	string TargetName;
	string TargetNum;
};

class SubMsgObjectNum
{
public:
	void ObjectNum(string InObjectNum) { TargetObjectNum = InObjectNum; }
	string ObjectNum() { return TargetObjectNum; }

private:
	string TargetObjectNum;
};

class SubMsgParameter
{
public:
	void Parameter(string InParameter) { TargetParameter = InParameter; }
	string Parameter() { return TargetParameter; }

private:
	string TargetParameter;
};

class SubMsgValue
{
public:
	void Value(string InValue) { TargetValue = InValue; }
	string Value() { return TargetValue; }

private:
	string TargetValue;
};

class SubMsgPacketNum
{
public:
	void PacketNum(string InPacketNum) { TargetPacketNum = InPacketNum; }
	string PacketNum() { return TargetPacketNum; }

private:
	string TargetPacketNum;
};

class MainMsgData
{
public:
	SubMsgName MsgName;
	SubMsgObjectNum MsgObjectNum;
	SubMsgParameter MsgParameter;
	SubMsgValue MsgValue;
	SubMsgPacketNum MsgPacketNum;
private:

};


MySOCKET::MySOCKET(void)
{
	//初期化処理
	this->GameSceneFlagFunc(true);
	this->MultThreadFlagFunc(false);

	char l_Destination[80] = { NULL };
	//unsigned short l_Port = 6432;//WANの人はプロトコルのポート　LANや同端末は同じ端末のポート
	unsigned short l_Port = 27015;//WANの人はプロトコルのポート　LANや同端末は同じ端末のポート
	sockaddr_in l_DstAddr;

	sprintf_s(l_Destination, "192.168.11.2"); //ローカルネットワークの人はこれを使う
	//sprintf_s(l_Destination, "106.73.19.98"); //グローバルネットワークの人はこれを使う

	/* Windows 独自の設定 */
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	/* sockaddr_in 構造体のセット */
	inet_pton(AF_INET, l_Destination, &l_DstAddr.sin_addr);
	l_DstAddr.sin_port = htons(l_Port);
	l_DstAddr.sin_family = AF_INET;

	//de^ta hannein
	this->DestinationFunc(l_Destination);
	this->PortFunc(l_Port);
	this->DstAddrFunc(l_DstAddr);

	/* ソケット生成 */
	//ストリームタイプー＞一つのでっかいバッファ。データの区切りをしっかり作る必要が出てくる　TCP　アクセプトなどの処理が必要
	//データグラムタイプー＞データごとで区切られている。こまごまとしたデータを多数送って処理する。　UDP　アクセプトなどの処理が必要ない
	this->DstSocketFunc(socket(AF_INET, SOCK_STREAM, 0));

	// ここで、ノンブロッキングに設定しています。
	 // val = 0でブロッキングモードに設定できます。
	 // ソケットの初期設定はブロッキングモードです。
	u_long val = 1;
	ioctlsocket(this->DstSocketFunc(), FIONBIO, &val);
}

MySOCKET::~MySOCKET(void)
{
	/* Windows 独自の設定 */
	closesocket(this->DstSocketFunc());
	WSACleanup();
}

//=============================================================================
// 他クラスのアドレス取得
//=============================================================================
void MySOCKET::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pplayer = obj->GetPlayer();
	pitem = obj->GetItem();
	pbullet = obj->GetBullet();
	pfield = obj->GetField();
	pGameObjInstance = obj;
}

//=============================================================================
// 初期化処理
//=============================================================================
void MySOCKET::Init(void)
{
	NetMatchFlag = false;
	NetMyNumberFlag = false;
	NetMyNumber = -1;
	NetItemFlag = false;
	NetGameStartFlag = false;
	NetShareDateFlag = false;
	this->MultThreadFlagFunc(false);

	//フラグをオフにする
	ConnectFlag = false;
	CountDownFlag = false;
}

//=============================================================================
// 更新処理
//=============================================================================
void MySOCKET::Update(void)
{
	//マッチング中
	if (NetMatchFlag == false)
	{
		this->NetMatch();
	}
	//マイナンバー取得中
	if (NetMyNumberFlag == false)
	{
		if (NetMyNumber >= -1 && NetMyNumber <= 3) this->NetMyNumberGet();
	}
	//アイテム情報取得中
	else if (NetItemFlag == false)
	{
		this->NetItemGet();
	}
	//カウントダウン信号待ち中
	if (NetMatchFlag == true && NetMyNumberFlag == true && NetItemFlag == true && NetGameStartFlag == false)
	{
		pGameObjInstance->InitNet();
		this->NetCountdown();
	}
	//スタートフラグが送られてきて信号がONになったらカウントダウン開始
	if (NetGameStartFlag == true)
	{
		pscene->NextScene(FADE_OUT, SCENE_NETGAMECOUNTDOWN, SOUND_LABEL_BGM_boss01);
		SourceVolumeChange(0, SOUND_LABEL_BGM_boss01);
	}

	//ゲーム終了処理
	//if (GetGameLoop() == true)
	//{
	//	SetEndGame(true);
	//	this->MultThreadFlagFunc(false);
	//	this->GameSceneFlagFunc(false);
	//}
}








void MySOCKET::NetMatch(void)
{
	/* 接続 */
	//printf("Trying to connect to %s: \n", destination);

	//一度コネクトするとフラグを消す
	if (ConnectFlag == false)
	{
		//接続要求
		connect(this->DstSocketFunc(), (struct sockaddr *) &this->DstAddrFunc(), sizeof(this->DstAddrFunc()));
		ConnectFlag = true;
	}
	//マッチング
	bool ChkMatch = false;
	bool ChkSend = false;//エントリーは一度しか送らない
	//while (ChkMatch != true)
	//{
		//ゲーム終了処理
		//if (GetGameLoop() == true)
		//{
		//	SetEndGame(true);
		//	this->MultThreadFlagFunc(false);
		//	this->GameSceneFlagFunc(false);
		//	break;
		//}

	char ConnectRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	ConnectRMsg[0] = NULL;
	char toSendText[500] = { NULL };
	sprintf_s(toSendText, "Entry");
	if (ChkSend == false)//WAN環境で要求回数が変わる
	{
		send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
		ChkSend = true;
	}
	printf("マッチング中\n");
	int numrcv = recv(this->DstSocketFunc(), ConnectRMsg, BUFFER_SIZE_STRING, 0);
	if (numrcv < 1)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// まだ来ない。
			//printf("ただ今の接続人数%s\n", buffer);
		}
		else
		{
			printf("error : 0x%x\n", WSAGetLastError());
		}
	}
	else
	{
		printf("received: ただ今の接続人数%s\n", ConnectRMsg);
		if (ConnectRMsg[0] == '5')
		{
			//受信完了メッセージ送信
			//strcpy_s(toSendText, "OK");
			//send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			ChkMatch = true;
			SetNetMatchFlag(true);

		}
	}
	Sleep(10);
	//}
}

void MySOCKET::NetMyNumberGet(void)
{
	//個人番号取得
	//bool ChkMyNumber = false;
	//while (ChkMyNumber != true)
	//{
	char MyNumChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	MyNumChkRMsg[0] = NULL;
	printf("個人番号取得中\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(this->DstSocketFunc(), MyNumChkRMsg, BUFFER_SIZE_STRING, 0);
	if (numrcv < 1)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// まだ来ない。
			//printf("ただ今の接続人数%s\n", buffer);
		}
		else
		{
			printf("error : 0x%x\n", WSAGetLastError());
		}
	}
	else
	{
		int packet = -1;
		//MyNum0～3の数値が送られてくる。
		if (strcmp(MyNumChkRMsg, "MyNum0") == 0) packet = 0;
		else if (strcmp(MyNumChkRMsg, "MyNum1") == 0) packet = 1;
		else if (strcmp(MyNumChkRMsg, "MyNum2") == 0) packet = 2;
		else if (strcmp(MyNumChkRMsg, "MyNum3") == 0) packet = 3;

		//MuNumから0以上の時に特定データを受信したと認識
		if (packet >= 0)
		{
			//受信完了メッセージ送信
			toSendText[0] = NULL;
			sprintf_s(toSendText, "MyNumOK");
			send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			//printf("received: 個人番号取得%s\n", Packetbuffer);
			//ChkMyNumber = true;
			int num = packet;
			SetNetMyNumber(num);
			SetNetMyNumberFlag(true);
		}
	}
	Sleep(10);
	//}
}

void MySOCKET::NetItemGet(void)
{
	//アイテム情報取得
	int ItemChk = 0;
	char ItemChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	ItemChkRMsg[0] = NULL;
	//printf("アイテム情報取得中\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(this->DstSocketFunc(), ItemChkRMsg, BUFFER_SIZE_STRING, 0);
	if (numrcv < 1)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// まだ来ない。
			//printf("ただ今の接続人数%s\n", buffer);
		}
		else
		{
			printf("error : 0x%x\n", WSAGetLastError());
		}
	}
	else
	{
		char *RMsgBlock;
		char *next = NULL;
		char *next2 = NULL;

		/* スペース.を区切りに文字列を抽出 */
		RMsgBlock = strtok_s(ItemChkRMsg, ",", &next);

		if (strcmp(RMsgBlock, "@I") == 0)
		{
			next2 = next;
			//アイテムメッセージ解析 "@Item,%d,%d,X%d,Z%d&"
			for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
			{
				char *Index;
				Index = strtok_s(next2, "N.", &next);
				char *Type;
				Type = strtok_s(NULL, "T,", &next);

				RMsgBlock = strtok_s(NULL, "&", &next);
				D3DXVECTOR3 buff;
				char *XYZnext = RMsgBlock;
				for (int CntXZ = 0; CntXZ < 2; CntXZ++)
				{
					char *GetVal = NULL;
					char *SetVal = NULL;
					char *nullp = NULL;
					//XZの数値部分を取得する
					GetVal = strtok_s(XYZnext, ",", &XYZnext);
					if (CntXZ == 0) SetVal = strtok_s(GetVal, "X", &nullp);//SetVal=000.000 nullp=NULL
					else if (CntXZ == 1) SetVal = strtok_s(GetVal, "Z", &nullp);//SetVal=000.000 nullp=NULL
					switch (CntXZ)
					{
					case 0:
						buff.x = strtof(SetVal, NULL);
						break;
					case 1:
						buff.y = ITEM_INIT_POSY;
						buff.z = strtof(SetVal, NULL);
						//データを格納
						int iIndex = atoi(Index);
						int iType = atoi(Type);
						NetSetItem(buff, iIndex, iType);
						if (next[0] != NULL)
						{
							next2 = strtok_s(next, "#", &next);
							strtok_s(next2, ",", &next);
							next2 = next;
						}
						break;
					}
				}
			}
		}

		//全アイテム情報を取得したか確認
		for (int ItemChkCnt = 0; ItemChkCnt < OBJECT_ITEM_MAX; ItemChkCnt++)
		{
			if (pitem->ItemParaAll[ItemChkCnt].NetUse == true)
			{
				ItemChk++;
			}
		}
		//アイテム情報を確認できた
		if (ItemChk == 20)
		{
			//受信完了メッセージ送信
			toSendText[0] = NULL;
			sprintf_s(toSendText, "ItemOK");
			send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			SetNetItemFlag(true);
		}
	}
}

void MySOCKET::NetCountdown(void)
{
	char CountChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	CountChkRMsg[0] = NULL;
	//ゲームカウントダウン開始
	//printf("ゲームカウントダウン開始\n");
	if (CountDownFlag == false)
	{
		char toSendText[500] = { NULL };
		int numrcv = recv(this->DstSocketFunc(), CountChkRMsg, BUFFER_SIZE_STRING, 0);
		if (numrcv < 1)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// まだ来ない。
				//printf("ただ今の接続人数%s\n", buffer);
			}
			else
			{
				//printf("error : 0x%x\n", WSAGetLastError());
			}
		}
		else
		{
			//特定のデータを受け取ってカウントダウンネットゲームシーンに遷移
			if (strcmp(CountChkRMsg, "Start") == 0)
			{
				//受信完了メッセージ送信
				toSendText[0] = NULL;
				sprintf_s(toSendText, "StartOK");
				send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
				CountDownFlag = true;
				SetNetGameStartFlag(true);
				//マルチスレッド開始信号ON
				this->MultThreadFlagFunc(true);
				this->GameSceneFlagFunc(true);
			}
		}
		Sleep(1);
	}
}







//-------------------以下マルチスレッド環境で実行
void MySOCKET::Packet(void)
{
	while (this->GameSceneFlagFunc() == true)
	{
		if (this->MultThreadFlagFunc() == true)
		{
			//ゲーム中
			//パケットを制御
			SendPacket();
			ReceivePacket();
			//調整用スリープ　これがないとランタイムエラーになる
			//receiveとsendの回数に関係がありそう。すこし待ってからreceiveしないといけいない
			Sleep(1);

			//ゲーム終了判定　アプリ右上Xを押されたら終了する
			if (GetEndGame() == true) break;
		}
	}
}

void MySOCKET::SendPacket(void)
{
	int MyNum = GetNetMyNumber();
	char SMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
	char ItemSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
	char ItemUseSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
	char EndGameMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容

	//--------変更したか確認　変更箇所があればsend()する　現在座標は常時同期している
	//座標
	D3DXVECTOR3 Pos = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 OldPos = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldPos();
	//if (Pos != OldPos)
	{
		//g_SendMsgBuff.playerPos = Pos;
		//player[MyNum].NetChkPos = true;
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "@P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, Pos.x, Pos.y, Pos.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	//回転
	D3DXVECTOR3 HoudaiRot = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoudaiOldRot = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldRot();
	if (HoudaiRot != HoudaiOldRot)
	{
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "@P%d,HoudaiRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoudaiRot.x, HoudaiRot.y, HoudaiRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HoutouRot = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HoutouOldRot = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].OldRot();
	if (HoutouRot != HoutouOldRot)
	{
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "@P%d,HoutouRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoutouRot.x, HoutouRot.y, HoutouRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HousinRot = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();
	D3DXVECTOR3 HousinOldRot = pplayer->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].OldRot();
	if (HousinRot != HousinOldRot)
	{
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "@P%d,HousinRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HousinRot.x, HousinRot.y, HousinRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}

	//バレット
	//バレットを発射していなと0,通常モデルの発射1,アタックモデルの発射3
	switch (pplayer->PlayerPara[MyNum].BulletPara.NetBulletShotFlagOneFrame)
	{
	case 0:
		break;
	case 1:
	{
		D3DXVECTOR3 BPos = pplayer->PlayerPara[MyNum].BulletPara.BulletStartPos;
		D3DXVECTOR3 BMove = pplayer->PlayerPara[MyNum].BulletPara.BulletMove[0];
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "@P%d,BulletA,PX%4.3f,PY%4.3f,PZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f&"
			, MyNum, BPos.x, BPos.y, BPos.z, BMove.x, BMove.y, BMove.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		break;
	}
	case 3:
	{
		D3DXVECTOR3 BPos = pplayer->PlayerPara[MyNum].BulletPara.BulletStartPos;
		D3DXVECTOR3 BMove[3];
		BMove[0] = pplayer->PlayerPara[MyNum].BulletPara.BulletMove[0];
		BMove[1] = pplayer->PlayerPara[MyNum].BulletPara.BulletMove[1];
		BMove[2] = pplayer->PlayerPara[MyNum].BulletPara.BulletMove[2];
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg,
			"@P%d,BulletB,PX%4.3f,PY%4.3f,PZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f&"
			, MyNum, BPos.x, BPos.y, BPos.z, BMove[0].x, BMove[0].y, BMove[0].z
			, BMove[1].x, BMove[1].y, BMove[1].z, BMove[2].x, BMove[2].y, BMove[2].z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		break;
	}
	}

	//バイタル
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (pplayer->PlayerPara[CntPlayer].StandardPara.Vital != pplayer->PlayerPara[CntPlayer].StandardPara.OldVital)
		{
			int Vital = pplayer->PlayerPara[CntPlayer].StandardPara.Vital;
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@P%d,Vital,%d&", CntPlayer, Vital);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
	}
	//モーフィング モーフィング信号ON(アタックモデルの時、常時ON)メッセージ送信。receive側は1を受け続ける。
	if (pplayer->PlayerPara[MyNum].MorphingPara.NetGetMorphingOneFrame == true)//NetGetMorphing
	{
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "@P%d,Morphing,%d&", MyNum, 1);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}

	//---------------------------アイテム取得信号
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		if (pitem->ItemParaAll[CntItem].NetGetItemFlag == true &&
			pitem->ItemParaAll[CntItem].GetPlayerType == MyNum)
		{
			int ItemT = pitem->ItemParaAll[CntItem].eType;
			D3DXVECTOR3 ItemPos = pitem->Transform[CntItem].Pos();
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@I,N%d,T%d,X%4.3f,Y%4.3f,Z%4.3f&"
				, CntItem, ItemT, ItemPos.x, ItemPos.y, ItemPos.z);
			sprintf_s(ItemSMsg, "%s%s", ItemSMsg, NewSMsg);
		}
	}

	//---------------------------アイテム使用信号
	/*
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		//情報取得
		bool ItemUse = Item[CntItem].Use();
		int ItemT = Item[CntItem].nType;
		//送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL };
		sprintf_s(NewSMsg, "@UI,N%d,T%d,U%d&"
			, CntItem, ItemT, ItemUse);
		sprintf_s(ItemUseSMsg, "%s%s", ItemUseSMsg, NewSMsg);
	}

	*/

	//---------------------------ゲームシーン終了メッセージ
	int Puse = 0;
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (pplayer->iUseType[CntPlayer].Use() == NoUse) Puse++;
	}
	if (Puse >= 3)
	{
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "@EndGame&");
		sprintf_s(EndGameMsg, "%s%s", EndGameMsg, NewSMsg);
	}

	//----------------------------変更があった場合send()する　基本情報
	if (SMsg[0] != NULL)
	{
		send(this->DstSocketFunc(), SMsg, BUFFER_SIZE_STRING, 0);
	}

	//----------------------------変更があった場合send()する　アイテム取得情報
	if (ItemSMsg[0] != NULL)
	{
		send(this->DstSocketFunc(), ItemSMsg, BUFFER_SIZE_STRING, 0);

		//地形アイテムが取得された時だけ追加のメッセージを送信する
		if (pfield->FieldPara.TikeiSeed != pfield->FieldPara.OldTikeiSeed)
		{
			char SpecialSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(SpecialSMsg, "@T,S%d,N%d&", pfield->FieldPara.TikeiSeed, pfield->FieldPara.ItemGetPlayerNum);
			send(this->DstSocketFunc(), SpecialSMsg, BUFFER_SIZE_STRING, 0);
		}
	}

	//変更があった場合send()する　基本情報
	if (EndGameMsg[0] != NULL)
	{
		send(this->DstSocketFunc(), EndGameMsg, BUFFER_SIZE_STRING, 0);
	}

	//常時send()する　アイテム使用情報
	//send(this->DstSocketFunc(), ItemUseSMsg, BUFFER_SIZE_STRING, 0);

}

void MySOCKET::ReceivePacket(void)
{
	//実際のゲームで扱っているアドレスを取得

	int MyNum = GetNetMyNumber();
	char RMsg[BUFFER_SIZE] = { NULL }; //送られてくるデータ内容

	//常時受け取りをする。データの変更があった時のみアクセス許可を取り、対象データに書き込みを行う。
	int numrcv = recv(this->DstSocketFunc(), RMsg, BUFFER_SIZE_STRING, 0);
	if (numrcv < 1)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// まだ来ない。
			//printf("ただ今の接続人数%s\n", buffer);
		}
		else
		{
			//printf("error : 0x%x\n", WSAGetLastError());
		}
	}
	else
	{
		//プレイヤーデータを分析メッセージブロックの先頭は@ ブロックエンドは&
		//サンプル　"@P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&"
		if (RMsg[0] == '@')
		{
			ObjectAnalys(RMsg);
		}
	}
}







//解析処理　上位処理　オブジェクト判別
void MySOCKET::ObjectAnalys(char* argRMsg)
{
	char *RMsgBlock;
	char *next = NULL;
	/* スペース.を区切りに文字列を抽出 */
	RMsgBlock = strtok_s(argRMsg, ",", &next);

	//プレイヤー0の時はここ
	if (strcmp(RMsgBlock, "@P0") == 0)
	{
		VariableAnalys(next,0, "@P0,");
	}
	//プレイヤー1の時はここ
	else if (strcmp(RMsgBlock, "@P1") == 0)
	{
		VariableAnalys(next,1, "@P1,");
	}
	//プレイヤー2の時はここ
	else if (strcmp(RMsgBlock, "@P2") == 0)
	{
		VariableAnalys(next, 2, "@P2,");
	}
	//プレイヤー3の時はここ
	else if (strcmp(RMsgBlock, "@P3") == 0)
	{
		VariableAnalys(next, 3, "@P3,");
	}

	//アイテム情報はここ
	if (strcmp(RMsgBlock, "@I") == 0)
	{
		char *next2 = NULL;
		next2 = next;
		//アイテムメッセージ解析 "@Item,%d,%d,X%d,Z%d&"
		char *Index;
		Index = strtok_s(next2, "N.", &next);
		char *Type;
		Type = strtok_s(NULL, "T,", &next);

		RMsgBlock = strtok_s(NULL, "&", &next);
		D3DXVECTOR3 buff;
		char *XYZnext = RMsgBlock;
		for (int CntXZ = 0; CntXZ < 2; CntXZ++)
		{
			char *GetVal = NULL;
			char *SetVal = NULL;
			char *nullp = NULL;
			//XZの数値部分を取得する
			GetVal = strtok_s(XYZnext, ",", &XYZnext);
			if (CntXZ == 0) SetVal = strtok_s(GetVal, "X", &nullp);//SetVal=000.000 nullp=NULL
			else if (CntXZ == 1) SetVal = strtok_s(GetVal, "Z", &nullp);//SetVal=000.000 nullp=NULL
			switch (CntXZ)
			{
			case 0:
				buff.x = strtof(SetVal, NULL);
				break;
			case 1:
				buff.y = ITEM_INIT_POSY;
				buff.z = strtof(SetVal, NULL);
				//データを格納
				int iIndex = atoi(Index);
				int iType = atoi(Type);
				NetSetItem(buff, iIndex, iType);
				PlaySound(SOUND_LABEL_SE_nyu);
				if (next[0] != NULL)
				{
					next2 = strtok_s(next, "#", &next);
					strtok_s(next2, ",", &next);
					next2 = next;
				}
				break;
			}
		}
	}

	//地形情報はここ
	if (strcmp(RMsgBlock, "@T") == 0)
	{
		char *next2 = NULL;
		next2 = next;
		//地形アイテム時だけ追加のメッセージ
		char *Seed;
		Seed = strtok_s(next2, "S,", &next);
		char *PlayerNum;
		PlayerNum = strtok_s(NULL, "N", &next);
		int iSeed = atoi(Seed);
		int iPlayerNum = atoi(PlayerNum);

		NetSetTikeiSeed(iSeed, iPlayerNum);
	}

	//ゲーム終了情報はここ
	if (strcmp(RMsgBlock, "@EndGame") == 0)
	{
		NetSetGameEnd();
	}
}

//解析処理　中位処理　対応変数判別
void MySOCKET::VariableAnalys(char* argVariableMsg, int PlayerNum, char* PlayerType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argVariableMsg, ",", &next);

	//XYZタイプ1
	if (strcmp(RMsgBlock, "Pos") == 0)
	{
		XYZValue1Analys(next, 0, PlayerType, VariableAnalysType_Pos);
	}
	if (strcmp(RMsgBlock, "HoudaiRot") == 0)
	{
		XYZValue1Analys(next, 0, PlayerType, VariableAnalysType_HoudaiRot);
	}
	if (strcmp(RMsgBlock, "HoutouRot") == 0)
	{
		XYZValue1Analys(next, 0, PlayerType, VariableAnalysType_HoutouRot);
	}
	if (strcmp(RMsgBlock, "HousinRot") == 0)
	{
		XYZValue1Analys(next, 0, PlayerType, VariableAnalysType_HousinRot);
	}

	//XYZタイプ2
	if (strcmp(RMsgBlock, "BulletA") == 0)
	{
		XYZValue2Analys(next, 0, PlayerType, VariableAnalysType_BulletA);
	}

	//XYZタイプ4
	if (strcmp(RMsgBlock, "BulletB") == 0)
	{
		XYZValue4Analys(next, 0, PlayerType, VariableAnalysType_BulletB);
	}

	//INTタイプ
	if (strcmp(RMsgBlock, "Vital") == 0)
	{
		INTValueAnalys(next, 0, PlayerType, VariableAnalysType_Vital);
	}
	if (strcmp(RMsgBlock, "Morphing") == 0)
	{
		INTValueAnalys(next, 0, PlayerType, VariableAnalysType_Morphing);
	}

}

//解析処理　下位処理　数値データ判別　float *3 XYZタイプ1
void MySOCKET::XYZValue1Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argValueMsg, "&", &next);

	D3DXVECTOR3 buff;
	char *XYZnext = RMsgBlock;
	for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
	{
		char *GetVal = NULL;
		char *SetVal = NULL;
		char *YZnext = NULL;
		char *nullp = NULL;
		//XYZの数値部分を取得する
		GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
		//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
		if (CntXYZ == 0) SetVal = strtok_s(GetVal, "X", &nullp);//SetVal=000.000 nullp=NULL
		else if (CntXYZ == 1) SetVal = strtok_s(GetVal, "Y", &nullp);//SetVal=000.000 nullp=NULL
		else if (CntXYZ == 2) SetVal = strtok_s(GetVal, "Z", &nullp);//SetVal=000.000 nullp=NULL
		switch (CntXYZ)
		{
		case 0:
			buff.x = strtof(SetVal, NULL);
			break;
		case 1:
			buff.y = strtof(SetVal, NULL);
			break;
		case 2:
			buff.z = strtof(SetVal, NULL);
			//データを格納
			switch (VariableType)
			{
			case VariableAnalysType_Pos:
				NetSetPos(buff, PlayerNum);
				break;
			case VariableAnalysType_HoudaiRot:
				NetSetHoudaiRot(buff, PlayerNum);
				break;
			case VariableAnalysType_HoutouRot:
				NetSetHoutouRot(buff, PlayerNum);
				break;
			case VariableAnalysType_HousinRot:
				NetSetHousinRot(buff, PlayerNum);
				break;
			}
			if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
			break;
		}
	}
}

//解析処理　下位処理　数値データ判別　float *3 *2 XYZタイプ2
void MySOCKET::XYZValue2Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argValueMsg, "&", &next);

	D3DXVECTOR3 buffpos;
	char *XYZnext = RMsgBlock;
	for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
	{
		char *GetVal = NULL;
		char *SetVal = NULL;
		char *YZnext = NULL;
		char *nullp = NULL;
		//XYZの数値部分を取得する
		GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
		//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
		if (CntXYZ == 0) SetVal = strtok_s(GetVal, "PX", &nullp);//SetVal=000.000 nullp=NULL
		else if (CntXYZ == 1) SetVal = strtok_s(GetVal, "PY", &nullp);//SetVal=000.000 nullp=NULL
		else if (CntXYZ == 2) SetVal = strtok_s(GetVal, "PZ", &nullp);//SetVal=000.000 nullp=NULL
		switch (CntXYZ)
		{
		case 0:
			buffpos.x = strtof(SetVal, NULL);
			break;
		case 1:
			buffpos.y = strtof(SetVal, NULL);
			break;
		case 2:
			buffpos.z = strtof(SetVal, NULL);

			//Move取得
			{
				D3DXVECTOR3 buffmove;
				char *XYZnextmove = XYZnext;
				for (int CntXYZmove = 0; CntXYZmove < 3; CntXYZmove++)
				{
					char *GetValmove = NULL;
					char *SetValmove = NULL;
					char *YZnextmove = NULL;
					char *nullpmove = NULL;
					//XYZの数値部分を取得する
					GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
					//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
					if (CntXYZmove == 0) SetValmove = strtok_s(GetValmove, "MX", &nullpmove);//SetVal=000.000 nullp=NULL
					else if (CntXYZmove == 1) SetValmove = strtok_s(GetValmove, "MY", &nullpmove);//SetVal=000.000 nullp=NULL
					else if (CntXYZmove == 2) SetValmove = strtok_s(GetValmove, "MZ", &nullpmove);//SetVal=000.000 nullp=NULL
					switch (CntXYZmove)
					{
					case 0:
						buffmove.x = strtof(SetValmove, NULL);
						break;
					case 1:
						buffmove.y = strtof(SetValmove, NULL);
						break;
					case 2:
						buffmove.z = strtof(SetValmove, NULL);
						break;
					}
				}

				//データを格納
				NetSetBulletType1(buffpos, buffmove, PlayerNum);
				if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
				break;
			}
		}
	}
}

//解析処理　下位処理　数値データ判別　float *3 *4 XYZタイプ4
void MySOCKET::XYZValue4Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argValueMsg, "&", &next);

	D3DXVECTOR3 buffpos;
	char *XYZnext = RMsgBlock;
	for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
	{
		char *GetVal = NULL;
		char *SetVal = NULL;
		char *YZnext = NULL;
		char *nullp = NULL;
		//XYZの数値部分を取得する
		GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
		//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
		if (CntXYZ == 0) SetVal = strtok_s(GetVal, "PX", &nullp);//SetVal=000.000 nullp=NULL
		else if (CntXYZ == 1) SetVal = strtok_s(GetVal, "PY", &nullp);//SetVal=000.000 nullp=NULL
		else if (CntXYZ == 2) SetVal = strtok_s(GetVal, "PZ", &nullp);//SetVal=000.000 nullp=NULL
		switch (CntXYZ)
		{
		case 0:
			buffpos.x = strtof(SetVal, NULL);
			break;
		case 1:
			buffpos.y = strtof(SetVal, NULL);
			break;
		case 2:
			buffpos.z = strtof(SetVal, NULL);

			//Move取得
			{
				D3DXVECTOR3 buffmove[3];
				char *XYZnextmove = XYZnext;
				for (int MoveCnt = 0; MoveCnt < 3; MoveCnt++)
				{
					for (int CntXYZmove = 0; CntXYZmove < 3; CntXYZmove++)
					{
						char *GetValmove = NULL;
						char *SetValmove = NULL;
						char *YZnextmove = NULL;
						char *nullpmove = NULL;
						//XYZの数値部分を取得する
						GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
						//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
						if (CntXYZmove == 0) SetValmove = strtok_s(GetValmove, "MX", &nullpmove);//SetVal=000.000 nullp=NULL
						else if (CntXYZmove == 1) SetValmove = strtok_s(GetValmove, "MY", &nullpmove);//SetVal=000.000 nullp=NULL
						else if (CntXYZmove == 2) SetValmove = strtok_s(GetValmove, "MZ", &nullpmove);//SetVal=000.000 nullp=NULL
						switch (CntXYZmove)
						{
						case 0:
							buffmove[MoveCnt].x = strtof(SetValmove, NULL);
							break;
						case 1:
							buffmove[MoveCnt].y = strtof(SetValmove, NULL);
							break;
						case 2:
							buffmove[MoveCnt].z = strtof(SetValmove, NULL);
							break;
						}
					}

					//データを格納
					if (MoveCnt == 2)
					{
						NetSetBulletType3(buffpos, &buffmove[0], PlayerNum);
						if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
					}
				}
			}
			break;
		}
	}
}

//解析処理　下位処理　数値データ判別　int *1 INTタイプ
void MySOCKET::INTValueAnalys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argValueMsg, "&", &next);

	int buff;
	buff = atoi(RMsgBlock);
	//データを格納
	switch (VariableType)
	{
	case VariableAnalysType_Vital:
		NetSetVital(buff, PlayerNum);
		break;
	case VariableAnalysType_Morphing:
		NetSetMorphing(PlayerNum);
		break;
	}
	if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
}










void MySOCKET::NetSetItem(D3DXVECTOR3 buff, int index, int type)
{
	if (pitem->iUseType[index].Use() == false)
	{
		pitem->SetInstance(index, buff, D3DXVECTOR3(2.0f, 2.0f, 2.0f), VEC3_ALL0, static_cast<eITEM_TYPE>(type));
		pitem->ItemParaAll[index].NetUse = true;
	}
}

void MySOCKET::NetSetTikeiSeed(int Seed, int PlayerNum)
{
	if (pfield->FieldPara.NetTikei == false)
	{
		srand(Seed);
		pfield->FieldPara.TikeiSeed = Seed;
		pfield->FieldPara.NetTikei = true;
		pfield->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, PlayerNum);
		//SetFieldInterPolationFieldType(0);
		PlaySound(SOUND_LABEL_SE_enter03);
		PlaySound(SOUND_LABEL_SE_quake);
	}
}

void MySOCKET::NetSetPos(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(buff);
	}
}

void MySOCKET::NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(buff);
	}
}

void MySOCKET::NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(buff);
	}
}

void MySOCKET::NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{	
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(buff);
	}
}

void MySOCKET::NetSetVital(int buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		pplayer->PlayerPara[PlayerNum].StandardPara.Vital = buff;
	}
}

void MySOCKET::NetSetMorphing(int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		//ローカル処理と同じ　CheackHit
		if (pplayer->PlayerPara[PlayerNum].StandardPara.eModelType == PLAYER_MODEL_TYPE_NORMAL)
		{
			//モーフィング開始信号、モデルタイプ、モーフィング中信号、モーフィング時間、モーフィング終了カウントのセット
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingStart = true;
			pplayer->PlayerPara[PlayerNum].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingSignal = NowMorphing;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else if (pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingStart == false && pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime <= 0.0f)
		{
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingStart = true;
			pplayer->PlayerPara[PlayerNum].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingSignal = NowMorphing;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else
		{
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
		}
	}
}

void MySOCKET::NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		//フラグが0なら発射してあげる。1フレームで1回発射できる。ここでフラグを1にして更新処理で0にするしている
		if (pplayer->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame == 0)
		{
				pbullet->SetInstance(buffpos, buffmove,
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					static_cast<ePLAYER_TYPE>(PlayerNum));
				pplayer->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame = 1;
		}
	}
}

void MySOCKET::NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		//フラグが0なら発射してあげる。1フレームで1回発射できる。ここでフラグを1にして更新処理で0にするしている
		if (pplayer->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame == 0)
		{
		
			for (int i = 0; i < 3; i++)
			{
				pbullet->SetInstance( buffpos, buffmove[i],
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					static_cast<ePLAYER_TYPE>(PlayerNum));
			}
			pplayer->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame = 1;
		}
	}
}

void MySOCKET::NetSetGameEnd(void)
{
	//対戦フラグを終了させる
	this->MultThreadFlagFunc(false);
}










