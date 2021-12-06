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
#include "../../h/Net/SockUpdate.h"

#pragma comment (lib, "Ws2_32.lib")

//=============================================================================
// sockの更新MANAGER
//=============================================================================
void SOCK_UPDATE::UpdateFunc(MySOCKET *MySockObj)
{
	//マッチング中
	if (MySockObj->GetNetMatchFlag() == false)
	{
		this->NetMatch(MySockObj);
	}
	//マイナンバー取得中
	if (MySockObj->GetNetMyNumberFlag() == false)
	{
		if (MySockObj->GetNetMyNumber() >= -1 && MySockObj->GetNetMyNumber() <= 3)
		{
			this->NetMyNumberGet(MySockObj);
		}
	}
	//アイテム情報取得中
	else if (MySockObj->GetNetItemFlag() == false)
	{
		this->NetItemGet(MySockObj);
	}
	//カウントダウン信号待ち中
	if (MySockObj->GetNetMatchFlag() == true && MySockObj->GetNetMyNumberFlag() == true && MySockObj->GetNetItemFlag() == true && MySockObj->GetNetGameStartFlag() == false)
	{
		MySockObj->pGameObjInstance->InitNet();
		this->NetCountdown(MySockObj);
	}
	//スタートフラグが送られてきて信号がONになったらカウントダウン開始
	if (MySockObj->GetNetGameStartFlag() == true)
	{
		MySockObj->pscene->NextScene(FADE_OUT, SCENE_NETGAMECOUNTDOWN, SOUND_LABEL_BGM_boss01);
		SourceVolumeChange(0, SOUND_LABEL_BGM_boss01);
	}
}

//=============================================================================
// 接続要求(確立)とプレイヤーマッチ処理
//=============================================================================
void SOCK_UPDATE::NetMatch(MySOCKET *MySockObj)
{
	/* 接続 */
	//一度コネクトするとフラグを消す
	if (MySockObj->GetNetConnectFlag() == false)
	{
		//接続要求
		connect(MySockObj->DstSocketFunc(), (struct sockaddr *) &MySockObj->DstAddrFunc(), sizeof(MySockObj->DstAddrFunc()));
		MySockObj->SetNetConnectFlag(true);
	}
	//マッチング
	bool ChkSend = false;//エントリーは一度しか送らない
	char toSendText[500] = { NULL };
	sprintf_s(toSendText, "Entry");
	if (ChkSend == false)//WAN環境で要求回数が変わる
	{
		send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
		ChkSend = true;
	}
	//printf("マッチング中\n");
	char ConnectRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	ConnectRMsg[0] = NULL;
	int numrcv = recv(MySockObj->DstSocketFunc(), ConnectRMsg, BUFFER_SIZE_STRING, 0);
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
			//send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			MySockObj->SetNetMatchFlag(true);

		}
	}
	Sleep(10);
	//}
}

//=============================================================================
// マッチ確立後　ゲーム中マイナンバー取得処理
//=============================================================================
void SOCK_UPDATE::NetMyNumberGet(MySOCKET *MySockObj)
{
	//個人番号取得
	//bool ChkMyNumber = false;
	//while (ChkMyNumber != true)
	//{
	char MyNumChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	MyNumChkRMsg[0] = NULL;
	printf("個人番号取得中\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(MySockObj->DstSocketFunc(), MyNumChkRMsg, BUFFER_SIZE_STRING, 0);
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
			send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			//printf("received: 個人番号取得%s\n", Packetbuffer);
			//ChkMyNumber = true;
			int num = packet;
			MySockObj->SetNetMyNumber(num);
			MySockObj->SetNetMyNumberFlag(true);
		}
	}
	Sleep(10);
	//}
}

//=============================================================================
// マイナンバー取得後　ゲーム開始前アイテム同期処理
//=============================================================================
void SOCK_UPDATE::NetItemGet(MySOCKET *MySockObj)
{
	//アイテム情報取得
	int ItemChk = 0;
	char ItemChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	ItemChkRMsg[0] = NULL;
	//printf("アイテム情報取得中\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(MySockObj->DstSocketFunc(), ItemChkRMsg, BUFFER_SIZE_STRING, 0);
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
						MySockObj->NetSetItem(buff, iIndex, iType);
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
			if (MySockObj->pitem->ItemParaAll[ItemChkCnt].NetUse == true)
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
			send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			MySockObj->SetNetItemFlag(true);
		}
	}
}

//=============================================================================
// アイテム同期処理後　ネット対戦始めるためのカウントダウン信号取得処理
//=============================================================================
void SOCK_UPDATE::NetCountdown(MySOCKET *MySockObj)
{
	char CountChkRMsg[BUFFER_SIZE]; //送られてくるデータ内容
	CountChkRMsg[0] = NULL;
	//ゲームカウントダウン開始
	//printf("ゲームカウントダウン開始\n");
	if (MySockObj->GetNetCountDownFlag() == false)
	{
		char toSendText[500] = { NULL };
		int numrcv = recv(MySockObj->DstSocketFunc(), CountChkRMsg, BUFFER_SIZE_STRING, 0);
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
				send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
				MySockObj->SetNetCountDownFlag(true);
				MySockObj->SetNetGameStartFlag(true);
				//マルチスレッド開始信号ON
				MySockObj->MultThreadFlagFunc(true);
				MySockObj->GameSceneFlagFunc(true);
			}
		}
		Sleep(1);
	}
}





