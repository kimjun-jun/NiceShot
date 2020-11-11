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


#pragma comment (lib, "Ws2_32.lib")
#define BUFFER_SIZE 256

//STEP1	同端末通信	OK
//STEP2	LAN通信		OK
//STEP3	WAN通信		OK

int client(void)
{
	/* IP アドレス、ポート番号、ソケット */
	char destination[80] = { "192.168.11.3" };
	unsigned short port = 27015;
	int dstSocket;

	/* sockaddr_in 構造体 */
	struct sockaddr_in dstAddr;

	/* 各種パラメータ */
	int numrcv;
	char buffer[BUFFER_SIZE]; //送られてくるデータ内容
	char toSendText[1024] = "Entry";
	int status;

	/************************************************************/

	/* Windows 独自の設定 */
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	/* 相手先アドレスの入力 */
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

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
	memset(&dstAddr, 0, sizeof(dstAddr));
	inet_pton(AF_INET, destination, &dstAddr.sin_addr);
	dstAddr.sin_port = htons(port);
	dstAddr.sin_family = AF_INET;

	/* ソケット生成 */
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	/* 接続 */
	//printf("Trying to connect to %s: \n", destination);
	connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr));

	// ここで、ノンブロッキングに設定しています。
	 // val = 0でブロッキングモードに設定できます。
	 // ソケットの初期設定はブロッキングモードです。
	u_long val = 1;
	ioctlsocket(dstSocket, FIONBIO, &val);

	/* パケット送出 */
	//マッチング
	bool ChkMatch = false;
	while (ChkMatch != true)
	{
		send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
		printf("マッチング中\n");
		numrcv = recv(dstSocket, buffer, BUFFER_SIZE, 0);
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
			printf("received: ただ今の接続人数%s\n", buffer);
			if (buffer[0] == '4') ChkMatch = true;
			if (numrcv == 0 || numrcv == -1)
			{
				status = closesocket(dstSocket);
			}
		}
		Sleep(1000);
	}

	//ゲーム開始
	printf("ゲーム開始\n");
	gets_s(toSendText);
	while (1)
	{
		//strcpy_s(toSendText,"No1");
		send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
		//numrcv = recv(dstSocket, buffer, BUFFER_SIZE, 0);
		Sleep(1000);
	}

	/* Windows 独自の設定 */
	closesocket(dstSocket);
	WSACleanup();

	return 0;
}


void CreateWinDlg(HWND hWnd,HINSTANCE hInstance)
{
	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
		"AppClass",
		"サーバーIP入力画面",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_W + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		SCREEN_H + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		hInstance,
		NULL);

}