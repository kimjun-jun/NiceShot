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
#include "../../h/net/sock.h"


#pragma comment (lib, "Ws2_32.lib")
#define BUFFER_SIZE 256

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
char toSendText[1024] = "Entry";

bool MultThreadFlag = false;

int NetClientSocketCreate(void)
{
	SendObjectP = NULL;

	sprintf_s(destination, "192.168.11.3");
	port = 27015;
	dstSocket = 0;
	toSendText[0] = NULL;

	dstAddr = { NULL };
	MultThreadFlag = false;

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
			send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
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
		//MyNum0〜3の数値が送られてくる。
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
			send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
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
				send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
				//ChkStart = true;
				SetNetGameStartFlag(true);
				//マルチスレッド開始信号ON
				MultThreadFlag = true;
			}
		}
		Sleep(10);
	//}
}

void Packet(void)
{
	while (GetEndGame() == false)
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

void SendPacket(void)
{
	//信号がON(変更があった時)に[変化するインスタンス1][変化後のパラメータ2]を送る
	/*
	バレット->use,move,type
	effect
	shadow
	explosion
	damege
	アイテム->Updateすべてサーバー.use,pos,rot,scl,FieldNorUpNorCross,Qrot
	フィールド->地形アイテム取得信号からサーバー上で次の地形を生成。
	そのデータをクライアントに送り、補間させながら地形変形、pD3DVtxBuffFieldDraw
	*/



	//実際のゲームで扱っているアドレスを取得
	SendObjectP = GetSendObjectP();
	int MyNum = GetNetMyNumber();
	char SMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容

	//変更したか確認　変更箇所があればsend()する
	//チェック項目 pos,rot(砲台砲塔砲身),Morphing,vital
	SendObjectP->player[MyNum].NetChkPos = false;
	SendObjectP->player[MyNum].NetChkHoudaiRot = false;
	SendObjectP->player[MyNum].NetChkHoutouRot = false;
	SendObjectP->player[MyNum].NetChkHousinRot = false;
	SendObjectP->player[MyNum].NetChkvital[4] = { false };
	SendObjectP->player[MyNum].NetChkMorphing = false;
	//座標
	D3DXVECTOR3 Pos = SendObjectP->player[MyNum].GetPos();
	D3DXVECTOR3 OldPos = SendObjectP->player[MyNum].GetOldPos();
	if (Pos != OldPos)
	{
		if (Pos.y >= OldPos.y + 0.01f || Pos.y <= OldPos.y - 0.01f)
		{
			//SendObjectP->player[MyNum].NetChkPos = true;
			//変更があるので送信用メッセージに書き込む
			char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
			sprintf_s(NewSMsg, "P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, Pos.x, Pos.y, Pos.z);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
	}
	//回転
	D3DXVECTOR3 HoudaiRot = SendObjectP->player[MyNum].GetRot();
	D3DXVECTOR3 HoudaiOldRot = SendObjectP->player[MyNum].GetOldRot();
	if (HoudaiRot != HoudaiOldRot)
	{
		SendObjectP->player[MyNum].NetChkHoudaiRot = true;
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "P%d,HoudaiRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoudaiRot.x, HoudaiRot.y, HoudaiRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HoutouRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HoutouOldRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUTOU].GetOldRot();
	if (HoutouRot != HoutouOldRot)
	{
		SendObjectP->player[MyNum].NetChkHoutouRot = true;
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg,"P%d,HoutouRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoutouRot.x, HoutouRot.y, HoutouRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HousinRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUSIN].GetRot();
	D3DXVECTOR3 HousinOldRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUSIN].GetOldRot();
	if (HousinRot != HousinOldRot)
	{
		SendObjectP->player[MyNum].NetChkHousinRot = true;
		//変更があるので送信用メッセージに書き込む
		char NewSMsg[BUFFER_SIZE] = { NULL }; //送るデータ内容
		sprintf_s(NewSMsg, "P%d,HousinRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HousinRot.x, HousinRot.y, HousinRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	//バイタル
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (SendObjectP->player[CntPlayer].vital != SendObjectP->player[CntPlayer].oldvital)
		{
			SendObjectP->player[MyNum].NetChkvital[CntPlayer] = true;
		}
	}
	//モーフィング
	if (SendObjectP->player[MyNum].ModelType != SendObjectP->player[MyNum].OldModelType)
	{
		SendObjectP->player[MyNum].NetChkMorphing = true;
	}

	//変更があった場合send()する
	if(SMsg[0]!=NULL)
	{
		send(dstSocket, SMsg, strlen(SMsg) + 1, 0);
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
		//ロックされてなければ分析して書き込み
		if (GetNetShareDateFlag() == false)
		{
			//プレイヤーデータを分析
			if (RMsg[0] == 'P')
			{
				MsgAnalys(RMsg);
			}
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
	if (strcmp(RMsgBlock, "P0") == 0)
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
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoudaiRot") == 0)
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
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoutouRot") == 0)
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
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HousinRot") == 0)
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
					break;
				}
			}
		}
	}
	//プレイヤー1の時はここ
	else if (strcmp(RMsgBlock, "P1") == 0)
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
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoudaiRot") == 0)
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
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoutouRot") == 0)
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
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HousinRot") == 0)
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
					break;
				}
			}
		}
	}
	//プレイヤー2の時はここ
	else if (strcmp(RMsgBlock, "P2") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Posがある確認
		if (strcmp(RMsgBlock, "Pos") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 2);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetPos(buff, 2);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoudaiRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumHoudaiRot, 2);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetHoudaiRot(buff, 2);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoutouRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumHoutouRot, 2);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetHoutouRot(buff, 2);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HousinRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumHousinRot, 2);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetHousinRot(buff, 2);
					break;
				}
			}
		}
	}
	//プレイヤー3の時はここ
	else if (strcmp(RMsgBlock, "P3") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Posがある確認
		if (strcmp(RMsgBlock, "Pos") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumPos, 3);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetPos(buff, 3);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoudaiRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumHoudaiRot, 3);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetHoudaiRot(buff, 3);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HoutouRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumHoutouRot, 3);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetHoutouRot(buff, 3);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "HousinRot") == 0)
		{
			RMsgBlock = strtok_s(NULL, ",", &next);
			//SetBuff(RMsgBlock, SetEnumHousinRot, 3);

			D3DXVECTOR3 buff;
			for (int CntXYZ = 0; CntXYZ < 3; CntXYZ++)
			{
				//XYZの頭部分を取得
				RMsgBlock = strtok_s(NULL, ",", &next);
				//XYZの数値部分を取得する
				RMsgBlock = strtok_s(NULL, ",", &next);
				switch (CntXYZ)
				{
				case 0:
					buff.x = strtof(RMsgBlock, &next);
					break;
				case 1:
					buff.y = strtof(RMsgBlock, &next);
					break;
				case 2:
					buff.z = strtof(RMsgBlock, &next);
					//データを格納
					NetSetHousinRot(buff, 3);
					break;
				}
			}
		}
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

void NetSetPos(D3DXVECTOR3 buff,int PlayerNum)
{
	SendObjectP = GetSendObjectP();
	SendObjectP->player[PlayerNum].SetPos(buff);
}

void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum)
{
	SendObjectP = GetSendObjectP();
	SendObjectP->player[PlayerNum].SetRot(buff);
}
void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum)
{
	SendObjectP = GetSendObjectP();
	SendObjectP->player[PlayerNum].parts[PARTSTYPE_HOUTOU].SetRot(buff);
}
void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum)
{
	SendObjectP = GetSendObjectP();
	SendObjectP->player[PlayerNum].parts[PARTSTYPE_HOUSIN].SetRot(buff);
}