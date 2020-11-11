/**
* @file sock.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
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

//STEP1	���[���ʐM	OK
//STEP2	LAN�ʐM		OK
//STEP3	WAN�ʐM		OK

int client(void)
{
	/* IP �A�h���X�A�|�[�g�ԍ��A�\�P�b�g */
	char destination[80] = { "192.168.11.3" };
	unsigned short port = 27015;
	int dstSocket;

	/* sockaddr_in �\���� */
	struct sockaddr_in dstAddr;

	/* �e��p�����[�^ */
	int numrcv;
	char buffer[BUFFER_SIZE]; //�����Ă���f�[�^���e
	char toSendText[1024] = "Entry";
	int status;

	/************************************************************/

	/* Windows �Ǝ��̐ݒ� */
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	/* �����A�h���X�̓��� */
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	//DirectInput
	/*
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
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
			// Direct3D�ɂ��`��̏I��
			pD3DDevice->EndScene();
			// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
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

	/* sockaddr_in �\���̂̃Z�b�g */
	memset(&dstAddr, 0, sizeof(dstAddr));
	inet_pton(AF_INET, destination, &dstAddr.sin_addr);
	dstAddr.sin_port = htons(port);
	dstAddr.sin_family = AF_INET;

	/* �\�P�b�g���� */
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	/* �ڑ� */
	//printf("Trying to connect to %s: \n", destination);
	connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr));

	// �����ŁA�m���u���b�L���O�ɐݒ肵�Ă��܂��B
	 // val = 0�Ńu���b�L���O���[�h�ɐݒ�ł��܂��B
	 // �\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�ł��B
	u_long val = 1;
	ioctlsocket(dstSocket, FIONBIO, &val);

	/* �p�P�b�g���o */
	//�}�b�`���O
	bool ChkMatch = false;
	while (ChkMatch != true)
	{
		send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
		printf("�}�b�`���O��\n");
		numrcv = recv(dstSocket, buffer, BUFFER_SIZE, 0);
		if (numrcv < 1)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// �܂����Ȃ��B
				//printf("�������̐ڑ��l��%s\n", buffer);
			}
			else
			{
				printf("error : 0x%x\n", WSAGetLastError());
			}
		}
		else
		{
			printf("received: �������̐ڑ��l��%s\n", buffer);
			if (buffer[0] == '4') ChkMatch = true;
			if (numrcv == 0 || numrcv == -1)
			{
				status = closesocket(dstSocket);
			}
		}
		Sleep(1000);
	}

	//�Q�[���J�n
	printf("�Q�[���J�n\n");
	gets_s(toSendText);
	while (1)
	{
		//strcpy_s(toSendText,"No1");
		send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
		//numrcv = recv(dstSocket, buffer, BUFFER_SIZE, 0);
		Sleep(1000);
	}

	/* Windows �Ǝ��̐ݒ� */
	closesocket(dstSocket);
	WSACleanup();

	return 0;
}


void CreateWinDlg(HWND hWnd,HINSTANCE hInstance)
{
	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
		"AppClass",
		"�T�[�o�[IP���͉��",
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