/**
* @file sock.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../../h/main.h"
#include "../../h/other/sound.h"
#include "../../h/other/debugproc.h"
#include "../../h/other/input.h"
#include "../../h/object/objectclass.h"
#include "../../h/object/player.h"
#include "../../h/object/item.h"
#include "../../h/object/bullet/bullet.h"
#include "../../h/map/field.h"
#include "../../h/net/sock.h"


#pragma comment (lib, "Ws2_32.lib")
#define BUFFER_SIZE 500

//Send()用オブジェクトのClone
GAME_OBJECT *SendObjectP = NULL;

//STEP1	同端末通信	OK
//STEP2	LAN通信		OK
//STEP3	WAN通信		OK

/* IP アドレス、ポート番号、ソケット */
char destination[80] = { "192.168.11.3" };
unsigned short port = 27015;
int dstSocket=0;

/* sockaddr_in 構造体 */
struct sockaddr_in dstAddr;

/* 各種パラメータ */
int numrcv;
//char Packetbuffer[BUFFER_SIZE]; //送られてくるデータ内容
char toSendText[BUFFER_SIZE] = "Entry";

//マルチスレッド判定用
bool MultThreadFlag = false;
void SetMultThreadFlag(bool flag)
{
	MultThreadFlag = flag;
}
bool GetMultThreadFlag(void)
{
	return MultThreadFlag;
}

//ネット対戦中ループ　trueでネット対戦中　falseで対戦していない  これで同期どうきするか判定している
bool GameSceneFlag = false;
void SetGameSceneFlag(bool flag)
{
	GameSceneFlag = flag;
}
bool GetGameSceneFlag(void)
{
	return GameSceneFlag;
}


//通信データのバッファ　この値とクライアント現在の値を比較して変更があれば最新データを送信する
//これまでだと1フレームで何回も同じデータを送信しているので、変更があったときに一度だけ送信する
struct SENDMSG_BUFF
{
	SENDMSG_BUFF() {
		PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		HoudaiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		HoutouRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		HousinRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
SENDMSG_BUFF g_SendMsgBuff;

int NetClientSocketCreate(void)
{
	SendObjectP = NULL;

	sprintf_s(destination, "192.168.11.3");
	port = 27015;
	dstSocket = 0;
	toSendText[0] = NULL;

	dstAddr = { NULL };
	MultThreadFlag = false;

	g_SendMsgBuff.PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SendMsgBuff.HoudaiRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SendMsgBuff.HoutouRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SendMsgBuff.HousinRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SendMsgBuff.BPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SendMsgBuff.ItemSyncUse = false;

	/* Windows 独自の設定 */
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	/* 相手先アドレスの入力 */
	//LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	//DirectInput
	/*
	// バックバッファ＆Ｚバッファのクリア
	char* InputNum = "Connect to ? : (name or IP address) ";
	SetTextSo(InputNum);
	bool chk = false;
	while (chk != true)
	{
		UpdateInput();
		if (GetKeyboardTrigger(DIK_0))
		{
			for (int i = 0; i < 80; i++)
			{
				if (destination[i] == '\0')
				{
					destination[i] = '0';
					break;
				}
			}
		}
		else if (GetKeyboardTrigger(DIK_1))
		{
			for (int i = 0; i < 80; i++)
			{
				if (destination[i] == '\0')
				{
					destination[i] = '1';
					break;
				}
			}
		}
		else if (GetKeyboardTrigger(DIK_BACK))
		{
			for (int i = 0; i < 80; i++)
			{
				if (destination[i] == '\0')
				{
					if (i == 0) break;
					destination[i-1] = '\0';
					break;
				}
			}
		}
		if (destination[0] != NULL)
		{
			strcat_s(InputNum, 256, destination);
		}
		SetTextSo(InputNum);
		pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		if (SUCCEEDED(pD3DDevice->BeginScene()))
		{
			DrawTextTypeSo();
			// Direct3Dによる描画の終了
			pD3DDevice->EndScene();
			// バックバッファとフロントバッファの入れ替え
			pD3DDevice->Present(NULL, NULL, NULL, NULL);
		}
	}
	*/

	//bool chk = false;
	//while (chk != false)
	//{
	//	LPTSTR CALLBACK IP_DIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//}

	//gets_s(destination);

	/* sockaddr_in 構造体のセット */
	//memset(&dstAddr, 0, sizeof(dstAddr));
	inet_pton(AF_INET, destination, &dstAddr.sin_addr);
	dstAddr.sin_port = htons(port);
	dstAddr.sin_family = AF_INET;

	/* ソケット生成 */
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	// ここで、ノンブロッキングに設定しています。
	 // val = 0でブロッキングモードに設定できます。
	 // ソケットの初期設定はブロッキングモードです。
	u_long val = 1;
	ioctlsocket(dstSocket, FIONBIO, &val);
	return 0;
}

int NetClientSocketDelete(void)
{
	/* Windows 独自の設定 */
	closesocket(dstSocket);
	WSACleanup();
	return 0;
}

void NetMatch(void)
{
	/* 接続 */
	printf("Trying to connect to %s: \n", destination);
	connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr));

	//マッチング
	bool ChkMatch = false;
	bool ChkSend = false;//エントリーは一度しか送らない
	while (ChkMatch != true)
	{
		char ConnectRMsg[BUFFER_SIZE]; //送られてくるデータ内容
		ConnectRMsg[0] = NULL;
		toSendText[0] = NULL;
		sprintf_s(toSendText, "Entry");
		if (ChkSend == false)
		{
			send(dstSocket, toSendText, BUFFER_SIZE, 0);
			ChkSend = true;
		}
		printf("マッチング中\n");
		numrcv = recv(dstSocket, ConnectRMsg, BUFFER_SIZE, 0);
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
				//send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
				ChkMatch = true;
				SetNetMatchFlag(true);

			}
		}
		//Sleep(10);
	}
}

void NetMyNumberGet(void)
{
	//個人番号取得
	//bool ChkMyNumber = false;
	//while (ChkMyNumber != true)
	//{
	char MyNumChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	MyNumChkRMsg[0] = NULL;
	printf("個人番号取得中\n");
	numrcv = recv(dstSocket, MyNumChkRMsg, BUFFER_SIZE, 0);
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
			sprintf_s(toSendText, "OK");
			send(dstSocket, toSendText, BUFFER_SIZE, 0);
			//printf("received: 個人番号取得%s\n", Packetbuffer);
			//ChkMyNumber = true;
			int num = packet;
			SetNetMyNumber(num);
			SetNetMyNumberFlagFlag(true);
		}
	}
	//Sleep(10);
//}
}

void NetItemGet(void)
{
	//実際のゲームで扱っているアドレスを取得
	SendObjectP = GetSendObjectP();

	//アイテム情報取得
	int ItemChk = 0;
	char ItemChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	ItemChkRMsg[0] = NULL;
	//printf("アイテム情報取得中\n");
	numrcv = recv(dstSocket, ItemChkRMsg, BUFFER_SIZE, 0);
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
			if (SendObjectP->item[ItemChkCnt].NetUse == true)
			{
				ItemChk++;
			}
		}
		//アイテム情報を確認できた
		if (ItemChk == 20)
		{
			//受信完了メッセージ送信
			toSendText[0] = NULL;
			sprintf_s(toSendText, "OK");
			send(dstSocket, toSendText, BUFFER_SIZE, 0);
			SetNetItemFlag(true);
		}
	}
}

void NetCountdown(void)
{
	char CountChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	CountChkRMsg[0] = NULL;
	//ゲームカウントダウン開始
	printf("ゲームカウントダウン開始\n");
	//bool ChkStart = false;
	//while (ChkStart != true)
	//{
		numrcv = recv(dstSocket, CountChkRMsg, BUFFER_SIZE, 0);
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
				sprintf_s(toSendText, "OK");
				send(dstSocket, toSendText, BUFFER_SIZE, 0);
				//ChkStart = true;
				SetNetGameStartFlag(true);
				//マルチスレッド開始信号ON
				MultThreadFlag = true;
				GameSceneFlag = true;
			}
		}
		Sleep(10);
	//}
}

void Packet(void)
{
	while (GetEndGame() == false)
	{
		while(GameSceneFlag==true)
		{
			if (MultThreadFlag == true)
			{
				//ゲーム中
				//パケットを制御
				SendPacket();
				ReceivePacket();
			}
		}
	}
}

void SendPacket(void)
{
	//実際のゲームで扱っているアドレスを取得
	SendObjectP = GetSendObjectP();
	int MyNum = GetNetMyNumber();
	char SMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
	char ItemSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
	char ItemUseSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
	char EndGameMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容

	//---------------------------プレイヤー
	//変更したか確認　変更箇所があればsend()する
	//チェック項目 pos,rot(砲台砲塔砲身),Morphing,vital
	SendObjectP->player[MyNum].NetChkPos = false;
	SendObjectP->player[MyNum].NetChkHoudaiRot = false;
	SendObjectP->player[MyNum].NetChkHoutouRot = false;
	SendObjectP->player[MyNum].NetChkHousinRot = false;
	SendObjectP->player[MyNum].NetChkvital[4] = { false };
	SendObjectP->player[MyNum].NetChkMorphing = false;

	if (SendObjectP != NULL)
	{
		//座標
		D3DXVECTOR3 Pos = SendObjectP->player[MyNum].GetPos();
		D3DXVECTOR3 OldPos = SendObjectP->player[MyNum].GetOldPos();
		if (Pos != OldPos)
		{
			g_SendMsgBuff.PlayerPos = Pos;
			//SendObjectP->player[MyNum].NetChkPos = true;
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, Pos.x, Pos.y, Pos.z);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
		//回転
		D3DXVECTOR3 HoudaiRot = SendObjectP->player[MyNum].GetRot();
		D3DXVECTOR3 HoudaiOldRot = SendObjectP->player[MyNum].GetOldRot();
		if (HoudaiRot != HoudaiOldRot)
		{
			g_SendMsgBuff.HoudaiRot = HoudaiRot;
			SendObjectP->player[MyNum].NetChkHoudaiRot = true;
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@P%d,HoudaiRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoudaiRot.x, HoudaiRot.y, HoudaiRot.z);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
		D3DXVECTOR3 HoutouRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HoutouOldRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUTOU].GetOldRot();
		if (HoutouRot != HoutouOldRot)
		{
			g_SendMsgBuff.HoutouRot = HoutouRot;
			SendObjectP->player[MyNum].NetChkHoutouRot = true;
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@P%d,HoutouRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoutouRot.x, HoutouRot.y, HoutouRot.z);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
		D3DXVECTOR3 HousinRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUSIN].GetRot();
		D3DXVECTOR3 HousinOldRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUSIN].GetOldRot();
		if (HousinRot != HousinOldRot)
		{
			g_SendMsgBuff.HousinRot = HousinRot;
			SendObjectP->player[MyNum].NetChkHousinRot = true;
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@P%d,HousinRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HousinRot.x, HousinRot.y, HousinRot.z);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
		//バイタル
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (SendObjectP->player[CntPlayer].vital != SendObjectP->player[CntPlayer].oldvital)
			{
				g_SendMsgBuff.Vital = SendObjectP->player[CntPlayer].vital;
				int Vital = SendObjectP->player[CntPlayer].vital;
				SendObjectP->player[MyNum].NetChkvital[CntPlayer] = true;
				//変更があるので送信用メッセージに書き込む
				char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
				sprintf_s(NewSMsg, "@P%d,Vital,%d&", CntPlayer, Vital);
				sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
			}
		}
		//モーフィング モーフィング信号ON(アタックモデルの時、常時ON)のときメッセージ送信。receive側は1を受け続ける。
		if (SendObjectP->player[MyNum].GetMorphing == true)
		{
			g_SendMsgBuff.Morphing = SendObjectP->player[MyNum].GetMorphing;
			SendObjectP->player[MyNum].NetChkMorphing = true;
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@P%d,Morphing,%d&", MyNum, 1);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}


		//---------------------------バレット
		//バレットを発射していなと0,通常モデルの発射1,アタックモデルの発射3
		switch (SendObjectP->player[MyNum].BFlag)
		{
		case 0:
			break;
		case 1:
		{
			g_SendMsgBuff.BPos = SendObjectP->player[MyNum].Bpos;
			D3DXVECTOR3 BPos = SendObjectP->player[MyNum].Bpos;
			D3DXVECTOR3 BMove = SendObjectP->player[MyNum].Bmove[0];
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "@P%d,BulletA,PX%4.3f,PY%4.3f,PZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f&"
				, MyNum, BPos.x, BPos.y, BPos.z, BMove.x, BMove.y, BMove.z);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
			break;
		}
		case 3:
		{
			g_SendMsgBuff.BPos = SendObjectP->player[MyNum].Bpos;
			D3DXVECTOR3 BPos = SendObjectP->player[MyNum].Bpos;
			D3DXVECTOR3 BMove[3];
			BMove[0] = SendObjectP->player[MyNum].Bmove[0];
			BMove[1] = SendObjectP->player[MyNum].Bmove[1];
			BMove[2] = SendObjectP->player[MyNum].Bmove[2];
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


		//---------------------------アイテム取得信号

		for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
		{
			if (SendObjectP->item[CntItem].NetGetItemFlag == true &&
				SendObjectP->item[CntItem].GetPlayerType == MyNum)
			{
				bool use = SendObjectP->item[CntItem].GetUse();
				//if (g_SendMsgBuff.ItemSyncUse != use)
				//{
				g_SendMsgBuff.ItemSyncUse = use;
				//int ItemN = SendObjectP->item[CntItem].GetPlayerType;
				int ItemT = SendObjectP->item[CntItem].nType;
				D3DXVECTOR3 ItemPos = SendObjectP->item[CntItem].GetPos();
				//変更があるので送信用メッセージに書き込む
				char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
				sprintf_s(NewSMsg, "@I,N%d,T%d,X%4.3f,Y%4.3f,Z%4.3f&"
					, CntItem, ItemT, ItemPos.x, ItemPos.y, ItemPos.z);
				sprintf_s(ItemSMsg, "%s%s", ItemSMsg, NewSMsg);
				//}
			}
		}

		//---------------------------アイテム使用信号
		/*
		for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
		{
			//情報取得
			bool ItemUse = SendObjectP->item[CntItem].GetUse();
			int ItemT = SendObjectP->item[CntItem].nType;
			//送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL };
			sprintf_s(NewSMsg, "@UI,N%d,T%d,U%d&"
				, CntItem, ItemT, ItemUse);
			sprintf_s(ItemUseSMsg, "%s%s", ItemUseSMsg, NewSMsg);
		}

		*/



		//ゲームシーン終了メッセージ
		int Puse = 0;
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (SendObjectP->player[CntPlayer].GetUse() == false) Puse++;
		}
		if (Puse >= 3)
		{
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "EndGame");
			sprintf_s(EndGameMsg, "%s%s", EndGameMsg, NewSMsg);
		}





		//変更があった場合send()する　基本情報
		if (SMsg[0] != NULL)
		{
			send(dstSocket, SMsg, BUFFER_SIZE, 0);
		}

		//変更があった場合send()する　アイテム取得情報
		if (ItemSMsg[0] != NULL)
		{
			send(dstSocket, ItemSMsg, BUFFER_SIZE, 0);

			//地形アイテムが取得された時だけ追加のメッセージを送信する
			if (SendObjectP->field->TikeiSeed != SendObjectP->field->OldTikeiSeed)
			{
				char SpecialSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
				sprintf_s(SpecialSMsg, "@T,S%d,N%d&", SendObjectP->field->TikeiSeed, SendObjectP->field->GetPlayerNum);
				send(dstSocket, SpecialSMsg, BUFFER_SIZE, 0);
			}
		}

		//変更があった場合send()する　基本情報
		if (EndGameMsg[0] != NULL)
		{
			send(dstSocket, EndGameMsg, BUFFER_SIZE, 0);
		}

		//常時send()する　アイテム使用情報
		//send(dstSocket, ItemUseSMsg, strlen(ItemUseSMsg) + 1, 0);
	}
}

void ReceivePacket(void)
{
	//実際のゲームで扱っているアドレスを取得
	SendObjectP = GetSendObjectP();
	int MyNum = GetNetMyNumber();
	char RMsg[BUFFER_SIZE] = { NULL }; //送られてくるデータ内容

	//常時受け取りをする。データの変更があった時のみアクセス許可を取り、対象データに書き込みを行う。
	numrcv = recv(dstSocket, RMsg, BUFFER_SIZE, 0);
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
			MsgAnalys(RMsg);
		}
	}
}

void MsgAnalys(char* argRMsg)
{
	char *RMsgBlock;
	char *next = NULL;
	/* スペース.を区切りに文字列を抽出 */
	RMsgBlock = strtok_s(argRMsg, ",", &next);

	//プレイヤー0の時はここ
	if (strcmp(RMsgBlock, "@P0") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Posがある確認
		if (strcmp(RMsgBlock, "Pos") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);  X709.000,

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
				//RMsgBlock = XYZnext;
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
					NetSetPos(buff, 0);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoudaiRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);


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
				//XYZnext = YZnext;
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
					NetSetHoudaiRot(buff, 0);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoutouRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHoutouRot(buff, 0);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HousinRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHousinRot(buff, 0);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetVital(buff, 0);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetMorphing(0);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
		}
		if (strcmp(RMsgBlock, "BulletA") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
							//XYZnext = YZnext;
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
						NetSetBulletType1(buffpos, buffmove, 0);
						if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
						break;
					}
				}
			}
		}
		if (strcmp(RMsgBlock, "BulletB") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
								//XYZnext = YZnext;
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
								NetSetBulletType3(buffpos, &buffmove[0], 0);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
							}
						}
					}
					break;
				}
			}
		}
	}
	//プレイヤー1の時はここ
	else if (strcmp(RMsgBlock, "@P1") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Posがある確認
		if (strcmp(RMsgBlock, "Pos") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);
			char *XYZnext = RMsgBlock;

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				char *GetVal = NULL;
				char *SetVal = NULL;
				char *YZnext = NULL;
				char *nullp = NULL;
				//XYZの数値部分を取得する
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
				//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
				//XYZnext = YZnext;
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
					NetSetPos(buff, 1);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoudaiRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);


			char *XYZnext = RMsgBlock;
			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				char *GetVal = NULL;
				char *SetVal = NULL;
				char *YZnext = NULL;
				char *nullp = NULL;
				//XYZの数値部分を取得する
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
				//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
				//XYZnext = YZnext;
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
					NetSetHoudaiRot(buff, 1);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoutouRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHoutouRot(buff, 1);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HousinRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHousinRot(buff, 1);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetVital(buff, 1);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetMorphing(1);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
		}
		if (strcmp(RMsgBlock, "BulletA") == 0)
		{
		RMsgBlock = strtok_s(NULL, "&", &next);
		//SetBuff(RMsgBlock, SetEnumPos, 0);

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
			//XYZnext = YZnext;
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
						//XYZnext = YZnext;
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
					NetSetBulletType1(buffpos, buffmove, 1);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
					break;
				}
			}
		}
		}
		if (strcmp(RMsgBlock, "BulletB") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
								//XYZnext = YZnext;
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
								NetSetBulletType3(buffpos, &buffmove[0], 1);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
							}
						}
					}
					break;
				}
			}
		}
	}
	//プレイヤー2の時はここ
	else if (strcmp(RMsgBlock, "@P2") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Posがある確認
		if (strcmp(RMsgBlock, "Pos") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);
			char *XYZnext = RMsgBlock;

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				char *GetVal = NULL;
				char *SetVal = NULL;
				char *YZnext = NULL;
				char *nullp = NULL;
				//XYZの数値部分を取得する
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
				//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
				//XYZnext = YZnext;
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
					NetSetPos(buff, 2);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoudaiRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);


			char *XYZnext = RMsgBlock;
			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				char *GetVal = NULL;
				char *SetVal = NULL;
				char *YZnext = NULL;
				char *nullp = NULL;
				//XYZの数値部分を取得する
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
				//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
				//XYZnext = YZnext;
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
					NetSetHoudaiRot(buff, 2);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoutouRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHoutouRot(buff, 2);
					break;
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
				}
			}
		}
		if (strcmp(RMsgBlock, "HousinRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHousinRot(buff, 2);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetVital(buff, 2);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetMorphing(2);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
		}
		if (strcmp(RMsgBlock, "BulletA") == 0)
		{
		RMsgBlock = strtok_s(NULL, "&", &next);
		//SetBuff(RMsgBlock, SetEnumPos, 0);

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
			//XYZnext = YZnext;
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
						//XYZnext = YZnext;
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
					NetSetBulletType1(buffpos, buffmove, 2);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
					break;
				}
			}
		}
		}
		if (strcmp(RMsgBlock, "BulletB") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
								//XYZnext = YZnext;
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
								NetSetBulletType3(buffpos, &buffmove[0], 2);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
							}
						}
					}
					break;
				}
			}
		}
	}
	//プレイヤー3の時はここ
	else if (strcmp(RMsgBlock, "@P3") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Posがある確認
		if (strcmp(RMsgBlock, "Pos") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);
			char *XYZnext = RMsgBlock;

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				char *GetVal = NULL;
				char *SetVal = NULL;
				char *YZnext = NULL;
				char *nullp = NULL;
				//XYZの数値部分を取得する
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
				//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
				//XYZnext = YZnext;
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
					NetSetPos(buff, 3);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoudaiRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);


			char *XYZnext = RMsgBlock;
			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				char *GetVal = NULL;
				char *SetVal = NULL;
				char *YZnext = NULL;
				char *nullp = NULL;
				//XYZの数値部分を取得する
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=次のRot
				//GetVal = strtok_s(NULL, ",", &YZnext);//GetVal=X000.000 XYZnext=Y000.000,Z000.000
				//XYZnext = YZnext;
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
					NetSetHoudaiRot(buff, 3);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HoutouRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHoutouRot(buff, 3);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "HousinRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
					NetSetHousinRot(buff, 3);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
					break;
				}
			}
		}
		if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetVital(buff, 3);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//データを格納
			NetSetMorphing(3);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
		}
		if (strcmp(RMsgBlock, "BulletA") == 0)
		{
		RMsgBlock = strtok_s(NULL, "&", &next);
		//SetBuff(RMsgBlock, SetEnumPos, 0);

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
			//XYZnext = YZnext;
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
						//XYZnext = YZnext;
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
					NetSetBulletType1(buffpos, buffmove, 3);
					if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
					break;
				}
			}
		}
		}
		if (strcmp(RMsgBlock, "BulletB") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 0);

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
				//XYZnext = YZnext;
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
								//XYZnext = YZnext;
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
								NetSetBulletType3(buffpos, &buffmove[0], 3);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
							}
						}
					}
					break;
				}
			}
		}
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
		//アイテムメッセージ解析 "@Item,%d,%d,X%d,Z%d&"
		char *Seed;
		Seed = strtok_s(next2, "S,", &next);
		char *PlayerNum;
		PlayerNum = strtok_s(NULL, "N", &next);
		int iSeed = atoi(Seed);
		int iPlayerNum = atoi(PlayerNum);

		NetSetTikeiSeed(iSeed, iPlayerNum);
	}
}

void SetBuff(char* RMsgBlock, int SetEnumType,int PlayerNum)
{
	D3DXVECTOR3 buff;
	char* RMsgBlockBuff = RMsgBlock;
	char* GetRMsgBlock;
	char* next1 = NULL;
	//char* next2;
	for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
	{
		//XYZの頭部分を取得
		GetRMsgBlock = strtok_s(RMsgBlockBuff, ",", &next1);
		//XYZの数値部分を取得する
		GetRMsgBlock = strtok_s(NULL, ",", &next1);
		switch (CntXYZ)
		{
		case 0:
			buff.x = strtof(GetRMsgBlock, &next1);
			break;
		case 1:
			buff.y = strtof(GetRMsgBlock, &next1);
			break;
		case 2:
			buff.z = strtof(GetRMsgBlock, &next1);
			//データを格納
			switch (SetEnumType)
			{
			case SetEnumPos:
				NetSetPos(buff, PlayerNum);
				break;
			case SetEnumHoudaiRot:
				NetSetHoudaiRot(buff, PlayerNum);
				break;
			case SetEnumHoutouRot:
				NetSetHoutouRot(buff, PlayerNum);
				break;
			case SetEnumHousinRot:
				NetSetHousinRot(buff, PlayerNum);
				break;
			}
			break;
		}
	}
}

void NetSetItem(D3DXVECTOR3 buff, int index, int type)
{
	SendObjectP = GetSendObjectP();
	if (SendObjectP->item[index].GetUse() == false)
	{
		SendObjectP->item[index].SetItem(buff, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);
		SendObjectP->item[index].NetUse = true;
		g_SendMsgBuff.ItemSyncUse = false;
	}
}

void NetSetTikeiSeed(int Seed, int PlayerNum)
{
	SendObjectP = GetSendObjectP();
	if (SendObjectP->field->NetTikei == false)
	{
		srand(Seed);
		SendObjectP->field->TikeiSeed = Seed;
		SendObjectP->field->NetTikei = true;
		SendObjectP->field->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, PlayerNum, &SendObjectP->item[0]);
		//SetFieldInterPolationFieldType(0);
		PlaySound(SOUND_LABEL_SE_enter03);
		PlaySound(SOUND_LABEL_SE_quake);
	}
}

void NetSetPos(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].SetPos(buff);
	}
}

void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].SetRot(buff);
	}
}

void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].parts[PARTSTYPE_HOUTOU].SetRot(buff);
	}
}

void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].parts[PARTSTYPE_HOUSIN].SetRot(buff);
	}
}

void NetSetVital(int buff, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].vital = buff;
	}
}

void NetSetMorphing(int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		if (SendObjectP->player[PlayerNum].ModelType == PLAYER_MODEL_NORMAL)
		{
			SendObjectP->player[PlayerNum].Morphing = true;
			SendObjectP->player[PlayerNum].ModelType = PLAYER_MODEL_ATTACK;
			SendObjectP->player[PlayerNum].MorphingSignal = NowMorphing;
			SendObjectP->player[PlayerNum].time = 0.0f;
			SendObjectP->player[PlayerNum].MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else if (SendObjectP->player[PlayerNum].Morphing == false && SendObjectP->player[PlayerNum].MorphingTime <= 0.0f)
		{
			SendObjectP->player[PlayerNum].Morphing = true;
			SendObjectP->player[PlayerNum].ModelType = PLAYER_MODEL_ATTACK;
			SendObjectP->player[PlayerNum].MorphingSignal = NowMorphing;
			SendObjectP->player[PlayerNum].time = 0.0f;
			SendObjectP->player[PlayerNum].MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else
		{
			SendObjectP->player[PlayerNum].MorphingTime = MORPHING_TIME;
		}
	}
}

void NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		//フラグが0なら発射してあげる。1フレームで1回発射できる。ここでフラグを1にして更新処理で0にするしている
		if (SendObjectP->player[PlayerNum].BFlag == 0)
		{
			SendObjectP = GetSendObjectP();
				SendObjectP->bullet->SetBullet(buffpos, buffmove,
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					PlayerNum, SendObjectP->shadow);
				SendObjectP->player[PlayerNum].BFlag = 1;
		}
	}
}

void NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		//フラグが0なら発射してあげる。1フレームで1回発射できる。ここでフラグを1にして更新処理で0にするしている
		if (SendObjectP->player[PlayerNum].BFlag == 0)
		{
			SendObjectP = GetSendObjectP();
			for (int i = 0; i < 3; i++)
			{
				SendObjectP->bullet->SetBullet(buffpos, buffmove[i],
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					PlayerNum, SendObjectP->shadow);
			}
			SendObjectP->player[PlayerNum].BFlag = 1;
		}
	}
}











