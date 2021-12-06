/**
* @file SockUpdate.h
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
#include "../../h/Net/SockSubThreadReceive.h"

#pragma comment (lib, "Ws2_32.lib")

//受信MANAGER
void SOCK_SUBTHREAD_RECEIVE::ReceiveFunc(MySOCKET *MySockObj)
{
	this->ReceivePacket(MySockObj);
}

void SOCK_SUBTHREAD_RECEIVE::ReceivePacket(MySOCKET *MySockObj)
{
	//実際のゲームで扱っているアドレスを取得

	int MyNum = MySockObj->GetNetMyNumber();
	char RMsg[BUFFER_SIZE] = { NULL }; //送られてくるデータ内容

	//常時受け取りをする。データの変更があった時のみアクセス許可を取り、対象データに書き込みを行う。
	int numrcv = recv(MySockObj->DstSocketFunc(), RMsg, BUFFER_SIZE_STRING, 0);
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
			ObjectAnalys(MySockObj,RMsg);
		}
	}
}

//解析処理　上位処理　オブジェクト判別
void SOCK_SUBTHREAD_RECEIVE::ObjectAnalys(MySOCKET *MySockObj, char* argRMsg)
{
	char *RMsgBlock;
	char *next = NULL;
	/* スペース.を区切りに文字列を抽出 */
	RMsgBlock = strtok_s(argRMsg, ",", &next);

	//プレイヤー0の時はここ
	if (strcmp(RMsgBlock, "@P0") == 0)
	{
		VariableAnalys(MySockObj, next, 0, "@P0,");
	}
	//プレイヤー1の時はここ
	else if (strcmp(RMsgBlock, "@P1") == 0)
	{
		VariableAnalys(MySockObj, next, 1, "@P1,");
	}
	//プレイヤー2の時はここ
	else if (strcmp(RMsgBlock, "@P2") == 0)
	{
		VariableAnalys(MySockObj, next, 2, "@P2,");
	}
	//プレイヤー3の時はここ
	else if (strcmp(RMsgBlock, "@P3") == 0)
	{
		VariableAnalys(MySockObj, next, 3, "@P3,");
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
				MySockObj->NetSetItem(buff, iIndex, iType);
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

		MySockObj->NetSetTikeiSeed(iSeed, iPlayerNum);
	}

	//ゲーム終了情報はここ
	if (strcmp(RMsgBlock, "@EndGame") == 0)
	{
		MySockObj->NetSetGameEnd();
	}
}

//解析処理　中位処理　対応変数判別
void SOCK_SUBTHREAD_RECEIVE::VariableAnalys(MySOCKET *MySockObj, char* argVariableMsg, int PlayerNum, char* PlayerType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argVariableMsg, ",", &next);

	//XYZタイプ1 switchじゃね？
	if (strcmp(RMsgBlock, "Pos") == 0)
	{
		XYZValue1Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_Pos);
	}
	else if (strcmp(RMsgBlock, "HoudaiRot") == 0)
	{
		XYZValue1Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_HoudaiRot);
	}
	else if (strcmp(RMsgBlock, "HoutouRot") == 0)
	{
		XYZValue1Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_HoutouRot);
	}
	else if (strcmp(RMsgBlock, "HousinRot") == 0)
	{
		XYZValue1Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_HousinRot);
	}

	//XYZタイプ2
	else if (strcmp(RMsgBlock, "BulletA") == 0)
	{
		XYZValue2Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_BulletA);
	}

	//XYZタイプ4
	else if (strcmp(RMsgBlock, "BulletB") == 0)
	{
		XYZValue4Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_BulletB);
	}

	//INTタイプ
	else if (strcmp(RMsgBlock, "Vital") == 0)
	{
		INTValueAnalys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_Vital);
	}
	else if (strcmp(RMsgBlock, "Morphing") == 0)
	{
		INTValueAnalys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_Morphing);
	}

}

//解析処理　下位処理　数値データ判別　float *3 XYZタイプ1
void SOCK_SUBTHREAD_RECEIVE::XYZValue1Analys(MySOCKET *MySockObj, char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
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
				MySockObj->NetSetPos(buff, PlayerNum);
				break;
			case VariableAnalysType_HoudaiRot:
				MySockObj->NetSetHoudaiRot(buff, PlayerNum);
				break;
			case VariableAnalysType_HoutouRot:
				MySockObj->NetSetHoutouRot(buff, PlayerNum);
				break;
			case VariableAnalysType_HousinRot:
				MySockObj->NetSetHousinRot(buff, PlayerNum);
				break;
			}
			//解析残りがあるなら再帰処理
			if (next[0] != NULL)
			{
				//argValueMsg = strtok_s(next, PlayerType, &next);
				//VariableAnalys(, PlayerType, );
				ObjectAnalys(MySockObj, next);
			}
			//解析残りなし　バッファが空なら解析終了
			else
			{
				break;
			}
		}
	}
}

//解析処理　下位処理　数値データ判別　float *3 *2 XYZタイプ2
void SOCK_SUBTHREAD_RECEIVE::XYZValue2Analys(MySOCKET *MySockObj, char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
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
				MySockObj->NetSetBulletType1(buffpos, buffmove, PlayerNum);
				if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
				break;
			}
		}
	}
}

//解析処理　下位処理　数値データ判別　float *3 *4 XYZタイプ4
void SOCK_SUBTHREAD_RECEIVE::XYZValue4Analys(MySOCKET *MySockObj, char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
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
						MySockObj->NetSetBulletType3(buffpos, &buffmove[0], PlayerNum);
						if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
					}
				}
			}
			break;
		}
	}
}

//解析処理　下位処理　数値データ判別　int *1 INTタイプ
void SOCK_SUBTHREAD_RECEIVE::INTValueAnalys(MySOCKET *MySockObj, char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
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
		MySockObj->NetSetVital(buff, PlayerNum);
		break;
	case VariableAnalysType_Morphing:
		MySockObj->NetSetMorphing(PlayerNum);
		break;
	}
	if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
}





