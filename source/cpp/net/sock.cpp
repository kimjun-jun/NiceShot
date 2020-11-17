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
#include "../../h/object/objectclass.h"
#include "../../h/net/sock.h"


#pragma comment (lib, "Ws2_32.lib")
#define BUFFER_SIZE 256

//STEP1	���[���ʐM	OK
//STEP2	LAN�ʐM		OK
//STEP3	WAN�ʐM		OK

	/* IP �A�h���X�A�|�[�g�ԍ��A�\�P�b�g */
char destination[80] = { "192.168.11.3" };
unsigned short port = 27015;
int dstSocket=0;

/* sockaddr_in �\���� */
struct sockaddr_in dstAddr;

/* �e��p�����[�^ */
int numrcv;
//char Packetbuffer[BUFFER_SIZE]; //�����Ă���f�[�^���e

char toSendText[1024] = "Entry";

int NetClientSocketCreate(void)
{
	sprintf_s(destination, "192.168.11.3");
	port = 27015;
	dstSocket = 0;
	toSendText[0] = NULL;

	dstAddr = { NULL };

	/* Windows �Ǝ��̐ݒ� */
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	/* �����A�h���X�̓��� */
	//LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

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
	//memset(&dstAddr, 0, sizeof(dstAddr));
	inet_pton(AF_INET, destination, &dstAddr.sin_addr);
	dstAddr.sin_port = htons(port);
	dstAddr.sin_family = AF_INET;

	/* �\�P�b�g���� */
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	// �����ŁA�m���u���b�L���O�ɐݒ肵�Ă��܂��B
	 // val = 0�Ńu���b�L���O���[�h�ɐݒ�ł��܂��B
	 // �\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�ł��B
	u_long val = 1;
	ioctlsocket(dstSocket, FIONBIO, &val);
	return 0;
}

int NetClientSocketDelete(void)
{
	/* Windows �Ǝ��̐ݒ� */
	closesocket(dstSocket);
	WSACleanup();
	return 0;
}

void NetMatch(void)
{
	/* �ڑ� */
	printf("Trying to connect to %s: \n", destination);
	connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr));

	//�}�b�`���O
	bool ChkMatch = false;
	bool ChkSend = false;//�G���g���[�͈�x��������Ȃ�
	while (ChkMatch != true)
	{
		char ConnectRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
		ConnectRMsg[0] = NULL;
		toSendText[0] = NULL;
		sprintf_s(toSendText, "Entry");
		if (ChkSend == false)
		{
			send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
			ChkSend = true;
		}
		printf("�}�b�`���O��\n");
		numrcv = recv(dstSocket, ConnectRMsg, BUFFER_SIZE, 0);
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
			printf("received: �������̐ڑ��l��%s\n", ConnectRMsg);
			if (ConnectRMsg[0] == '5')
			{
				//��M�������b�Z�[�W���M
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
	//�l�ԍ��擾
	//bool ChkMyNumber = false;
	//while (ChkMyNumber != true)
	//{
	char MyNumChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	MyNumChkRMsg[0] = NULL;
	printf("�l�ԍ��擾��\n");
	numrcv = recv(dstSocket, MyNumChkRMsg, BUFFER_SIZE, 0);
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
		int packet = -1;
		//MyNum0�`3�̐��l�������Ă���B
		if (strcmp(MyNumChkRMsg, "MyNum0") == 0) packet = 0;
		else if (strcmp(MyNumChkRMsg, "MyNum1") == 0) packet = 1;
		else if (strcmp(MyNumChkRMsg, "MyNum2") == 0) packet = 2;
		else if (strcmp(MyNumChkRMsg, "MyNum3") == 0) packet = 3;

		//MuNum����0�ȏ�̎��ɓ���f�[�^����M�����ƔF��
		if (packet >= 0)
		{
			//��M�������b�Z�[�W���M
			toSendText[0] = NULL;
			sprintf_s(toSendText, "OK");
			send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
			//printf("received: �l�ԍ��擾%s\n", Packetbuffer);
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
	char CountChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	CountChkRMsg[0] = NULL;
	//�Q�[���J�E���g�_�E���J�n
	printf("�Q�[���J�E���g�_�E���J�n\n");
	//bool ChkStart = false;
	//while (ChkStart != true)
	//{
		numrcv = recv(dstSocket, CountChkRMsg, BUFFER_SIZE, 0);
		if (numrcv < 1)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				// �܂����Ȃ��B
				//printf("�������̐ڑ��l��%s\n", buffer);
			}
			else
			{
				//printf("error : 0x%x\n", WSAGetLastError());
			}
		}
		else
		{
			//����̃f�[�^���󂯎���ăJ�E���g�_�E���l�b�g�Q�[���V�[���ɑJ��
			if (strcmp(CountChkRMsg, "Start") == 0)
			{
				//��M�������b�Z�[�W���M
				toSendText[0] = NULL;
				sprintf_s(toSendText, "OK");
				send(dstSocket, toSendText, strlen(toSendText) + 1, 0);
				//ChkStart = true;
				SetNetGameStartFlag(true);

				//�}���`�X���b�h�J�n
				std::thread t1(Packet);
				t1.join();
			}
		}
		Sleep(10);
	//}
}

void Packet(void)
{
	//�Q�[���J�n
	printf("�Q�[���J�n\n");
	bool ChkGameEnd = false;
	//while (ChkGameEnd != true)
	//{
	char GameRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	GameRMsg[0] = NULL;
	//�p�P�b�g�𐧌�
	SendPacket();
	ReceivePacket();

	//����̃f�[�^���󂯎���ă��U���g�ɑJ��
	if (strcmp(GameRMsg, "End") == 0)
	{
		ChkGameEnd = true;
	}
	//Sleep(10);
//}
}

void SendPacket(void)
{
	//���̂Ȃ��ŕϐ��̓���ւ����s��

}

void ReceivePacket(void)
{
	//���̂Ȃ��ŕϐ��̓���ւ����s��

}