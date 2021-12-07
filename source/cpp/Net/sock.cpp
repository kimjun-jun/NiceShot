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
#include "../../h/Net/SockUpdate.h"
#include "../../h/Net/SockSubThreadSend.h"
#include "../../h/Net/SockSubThreadReceive.h"
#include "../../h/Net/sock.h"

#pragma comment (lib, "Ws2_32.lib")

//WANプロトコルのポート6432
//ポート開放
//最大で300バイトくらい
//sleep無くていい
//再送制御,照合制御

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
	pscene = obj->GetScene();
	pGameObjInstance = obj;
}

//=============================================================================
// 初期化処理
//=============================================================================
void MySOCKET::Init(void)
{
	//フラグをオフにする
	NetMatchFlag = false;
	NetMyNumberFlag = false;
	NetMyNumber = -1;
	NetItemFlag = false;
	NetGameStartFlag = false;
	NetShareDateFlag = false;
	this->MultThreadFlagFunc(false);
	NetConnectFlag = false;
	NetCountDownFlag = false;
}

//=============================================================================
// 更新処理
//=============================================================================
void MySOCKET::Update(void)
{
	pSockUpdate->UpdateFunc(this);
}

//=============================================================================
// マルチスレッド処理
//=============================================================================
void MySOCKET::Packet(void)
{
	while (this->GameSceneFlagFunc() == true)
	{
		//ネット対戦時処理する
		if (this->MultThreadFlagFunc() == true)
		{
			//送受信制御
			this->pSockSubThreadSend->SendFunc(this);
			this->pSockSubThreadReceive->ReceiveFunc(this);
			//調整用スリープ　これがないとランタイムエラーになる receiveとsendの回数に関係がありそう。すこし待ってからreceiveしないといけいない
			Sleep(1);
			//ゲーム終了判定　アプリ右上Xを押されたら終了する
			if (GetEndGame() == true) break;
		}
	}
}

//=============================================================================
// オブジェクト同期反映処理
//=============================================================================
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
		if (pplayer->PlayerPara[PlayerNum].BulletPara.GetNetBulletShotFlagOneFrame() == 0)
		{
			pbullet->SetInstance(buffpos, buffmove,
				BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
				static_cast<ePLAYER_TYPE>(PlayerNum));
			pplayer->PlayerPara[PlayerNum].BulletPara.SetNetBulletShotFlagOneFrame(1);
		}
	}
}

void MySOCKET::NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum)
{
	//ロックされてなければ分析して書き込み
	if (GetNetShareDateFlag() == false)
	{
		//フラグが0なら発射してあげる。1フレームで1回発射できる。ここでフラグを1にして更新処理で0にするしている
		if (pplayer->PlayerPara[PlayerNum].BulletPara.GetNetBulletShotFlagOneFrame() == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				pbullet->SetInstance(buffpos, buffmove[i],
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					static_cast<ePLAYER_TYPE>(PlayerNum));
			}
			pplayer->PlayerPara[PlayerNum].BulletPara.SetNetBulletShotFlagOneFrame(1);
		}
	}
}

void MySOCKET::NetSetGameEnd(void)
{
	//対戦フラグを終了させる
	this->MultThreadFlagFunc(false);
}

