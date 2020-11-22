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
#include "../../h/object/player.h"
#include "../../h/net/sock.h"


#pragma comment (lib, "Ws2_32.lib")
#define BUFFER_SIZE 256

//Send()�p�I�u�W�F�N�g��Clone
GAME_OBJECT *SendObjectP = NULL;

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
				//�}���`�X���b�h�J�n�M��ON
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
			//�Q�[����
			//�p�P�b�g�𐧌�
			SendPacket();
			ReceivePacket();
		}
	}
}

void SendPacket(void)
{
	//�M����ON(�ύX����������)��[�ω�����C���X�^���X1][�ω���̃p�����[�^2]�𑗂�
	/*
	�o���b�g->use,move,type
	effect
	shadow
	explosion
	damege
	�A�C�e��->Update���ׂăT�[�o�[.use,pos,rot,scl,FieldNorUpNorCross,Qrot
	�t�B�[���h->�n�`�A�C�e���擾�M������T�[�o�[��Ŏ��̒n�`�𐶐��B
	���̃f�[�^���N���C�A���g�ɑ���A��Ԃ����Ȃ���n�`�ό`�ApD3DVtxBuffFieldDraw
	*/



	//���ۂ̃Q�[���ň����Ă���A�h���X���擾
	SendObjectP = GetSendObjectP();
	int MyNum = GetNetMyNumber();
	char SMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e

	//�ύX�������m�F�@�ύX�ӏ��������send()����
	//�`�F�b�N���� pos,rot(�C��C���C�g),Morphing,vital
	SendObjectP->player[MyNum].NetChkPos = false;
	SendObjectP->player[MyNum].NetChkHoudaiRot = false;
	SendObjectP->player[MyNum].NetChkHoutouRot = false;
	SendObjectP->player[MyNum].NetChkHousinRot = false;
	SendObjectP->player[MyNum].NetChkvital[4] = { false };
	SendObjectP->player[MyNum].NetChkMorphing = false;
	//���W
	D3DXVECTOR3 Pos = SendObjectP->player[MyNum].GetPos();
	D3DXVECTOR3 OldPos = SendObjectP->player[MyNum].GetOldPos();
	if (Pos != OldPos)
	{
		if (Pos.y >= OldPos.y + 0.01f || Pos.y <= OldPos.y - 0.01f)
		{
			//SendObjectP->player[MyNum].NetChkPos = true;
			//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
			char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(NewSMsg, "@P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, Pos.x, Pos.y, Pos.z);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
	}
	//��]
	D3DXVECTOR3 HoudaiRot = SendObjectP->player[MyNum].GetRot();
	D3DXVECTOR3 HoudaiOldRot = SendObjectP->player[MyNum].GetOldRot();
	if (HoudaiRot != HoudaiOldRot)
	{
		SendObjectP->player[MyNum].NetChkHoudaiRot = true;
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HoudaiRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoudaiRot.x, HoudaiRot.y, HoudaiRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HoutouRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HoutouOldRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUTOU].GetOldRot();
	if (HoutouRot != HoutouOldRot)
	{
		SendObjectP->player[MyNum].NetChkHoutouRot = true;
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg,"@P%d,HoutouRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoutouRot.x, HoutouRot.y, HoutouRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HousinRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUSIN].GetRot();
	D3DXVECTOR3 HousinOldRot = SendObjectP->player[MyNum].parts[PARTSTYPE_HOUSIN].GetOldRot();
	if (HousinRot != HousinOldRot)
	{
		SendObjectP->player[MyNum].NetChkHousinRot = true;
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HousinRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HousinRot.x, HousinRot.y, HousinRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	//�o�C�^��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (SendObjectP->player[CntPlayer].vital != SendObjectP->player[CntPlayer].oldvital)
		{
			int Vital = SendObjectP->player[CntPlayer].vital;
			SendObjectP->player[MyNum].NetChkvital[CntPlayer] = true;
			//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
			char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(NewSMsg, "@P%d,Vital,%d&", CntPlayer, Vital);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
	}
	//���[�t�B���O ���[�t�B���O�M��ON(�A�^�b�N���f���̎��A�펞ON)�̂Ƃ����b�Z�[�W���M�Breceive����1���󂯑�����B
	if (SendObjectP->player[MyNum].GetMorphing == true)
	{
		SendObjectP->player[MyNum].NetChkMorphing = true;
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,Morphing,%d&", MyNum, 1);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}

	//�ύX���������ꍇsend()����
	if(SMsg[0]!=NULL)
	{
		send(dstSocket, SMsg, strlen(SMsg) + 1, 0);
	}
}

void ReceivePacket(void)
{
	//���ۂ̃Q�[���ň����Ă���A�h���X���擾
	SendObjectP = GetSendObjectP();
	int MyNum = GetNetMyNumber();
	char RMsg[BUFFER_SIZE] = { NULL }; //�����Ă���f�[�^���e

	//�펞�󂯎�������B�f�[�^�̕ύX�����������̂݃A�N�Z�X�������A�Ώۃf�[�^�ɏ������݂��s���B
	numrcv = recv(dstSocket, RMsg, BUFFER_SIZE, 0);
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
		//�v���C���[�f�[�^�𕪐̓��b�Z�[�W�u���b�N�̐擪��@ �u���b�N�G���h��&
		//�T���v���@"@P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&"
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
	/* �X�y�[�X.����؂�ɕ�����𒊏o */
	RMsgBlock = strtok_s(argRMsg, ",", &next);

	//�v���C���[0�̎��͂���
	if (strcmp(RMsgBlock, "@P0") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Pos������m�F
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHousinRot(buff, 0);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetVital(buff, 0);
		}
		else if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(0);
		}

	}
	//�v���C���[1�̎��͂���
	else if (strcmp(RMsgBlock, "@P1") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Pos������m�F
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHousinRot(buff, 1);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetVital(buff, 1);
		}
		else if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(1);
		}
	}
	//�v���C���[2�̎��͂���
	else if (strcmp(RMsgBlock, "@P2") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Pos������m�F
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetPos(buff, 2);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHoudaiRot(buff, 2);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHoutouRot(buff, 2);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHousinRot(buff, 2);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetVital(buff, 2);
		}
		else if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(2);
		}
	}
	//�v���C���[3�̎��͂���
	else if (strcmp(RMsgBlock, "@P3") == 0)
	{
		RMsgBlock = strtok_s(NULL, ",", &next);
		//Pos������m�F
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetPos(buff, 3);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHoudaiRot(buff, 3);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHoutouRot(buff, 3);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
					//�f�[�^���i�[
					NetSetHousinRot(buff, 3);
					break;
				}
			}
		}
		else if (strcmp(RMsgBlock, "Vital") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetVital(buff, 3);
		}
		else if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(3);
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
		//XYZ�̓��������擾
		GetRMsgBlock = strtok_s(RMsgBlockBuff, ",", &next1);
		//XYZ�̐��l�������擾����
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
			//�f�[�^���i�[
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

void NetSetPos(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].SetPos(buff);
	}
}

void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].SetRot(buff);
	}
}

void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].parts[PARTSTYPE_HOUTOU].SetRot(buff);
	}
}

void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].parts[PARTSTYPE_HOUSIN].SetRot(buff);
	}
}

void NetSetVital(int buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		SendObjectP = GetSendObjectP();
		SendObjectP->player[PlayerNum].vital = buff;
	}
}

void NetSetMorphing(int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
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

















