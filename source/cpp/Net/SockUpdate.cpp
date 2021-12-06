/**
* @file SockUpdate.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
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
// sock�̍X�VMANAGER
//=============================================================================
void SOCK_UPDATE::UpdateFunc(MySOCKET *MySockObj)
{
	//�}�b�`���O��
	if (MySockObj->GetNetMatchFlag() == false)
	{
		this->NetMatch(MySockObj);
	}
	//�}�C�i���o�[�擾��
	if (MySockObj->GetNetMyNumberFlag() == false)
	{
		if (MySockObj->GetNetMyNumber() >= -1 && MySockObj->GetNetMyNumber() <= 3)
		{
			this->NetMyNumberGet(MySockObj);
		}
	}
	//�A�C�e�����擾��
	else if (MySockObj->GetNetItemFlag() == false)
	{
		this->NetItemGet(MySockObj);
	}
	//�J�E���g�_�E���M���҂���
	if (MySockObj->GetNetMatchFlag() == true && MySockObj->GetNetMyNumberFlag() == true && MySockObj->GetNetItemFlag() == true && MySockObj->GetNetGameStartFlag() == false)
	{
		MySockObj->pGameObjInstance->InitNet();
		this->NetCountdown(MySockObj);
	}
	//�X�^�[�g�t���O�������Ă��ĐM����ON�ɂȂ�����J�E���g�_�E���J�n
	if (MySockObj->GetNetGameStartFlag() == true)
	{
		MySockObj->pscene->NextScene(FADE_OUT, SCENE_NETGAMECOUNTDOWN, SOUND_LABEL_BGM_boss01);
		SourceVolumeChange(0, SOUND_LABEL_BGM_boss01);
	}
}

//=============================================================================
// �ڑ��v��(�m��)�ƃv���C���[�}�b�`����
//=============================================================================
void SOCK_UPDATE::NetMatch(MySOCKET *MySockObj)
{
	/* �ڑ� */
	//��x�R�l�N�g����ƃt���O������
	if (MySockObj->GetNetConnectFlag() == false)
	{
		//�ڑ��v��
		connect(MySockObj->DstSocketFunc(), (struct sockaddr *) &MySockObj->DstAddrFunc(), sizeof(MySockObj->DstAddrFunc()));
		MySockObj->SetNetConnectFlag(true);
	}
	//�}�b�`���O
	bool ChkSend = false;//�G���g���[�͈�x��������Ȃ�
	char toSendText[500] = { NULL };
	sprintf_s(toSendText, "Entry");
	if (ChkSend == false)//WAN���ŗv���񐔂��ς��
	{
		send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
		ChkSend = true;
	}
	//printf("�}�b�`���O��\n");
	char ConnectRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	ConnectRMsg[0] = NULL;
	int numrcv = recv(MySockObj->DstSocketFunc(), ConnectRMsg, BUFFER_SIZE_STRING, 0);
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
			//send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			MySockObj->SetNetMatchFlag(true);

		}
	}
	Sleep(10);
	//}
}

//=============================================================================
// �}�b�`�m����@�Q�[�����}�C�i���o�[�擾����
//=============================================================================
void SOCK_UPDATE::NetMyNumberGet(MySOCKET *MySockObj)
{
	//�l�ԍ��擾
	//bool ChkMyNumber = false;
	//while (ChkMyNumber != true)
	//{
	char MyNumChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	MyNumChkRMsg[0] = NULL;
	printf("�l�ԍ��擾��\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(MySockObj->DstSocketFunc(), MyNumChkRMsg, BUFFER_SIZE_STRING, 0);
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
			sprintf_s(toSendText, "MyNumOK");
			send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			//printf("received: �l�ԍ��擾%s\n", Packetbuffer);
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
// �}�C�i���o�[�擾��@�Q�[���J�n�O�A�C�e����������
//=============================================================================
void SOCK_UPDATE::NetItemGet(MySOCKET *MySockObj)
{
	//�A�C�e�����擾
	int ItemChk = 0;
	char ItemChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	ItemChkRMsg[0] = NULL;
	//printf("�A�C�e�����擾��\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(MySockObj->DstSocketFunc(), ItemChkRMsg, BUFFER_SIZE_STRING, 0);
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
		char *RMsgBlock;
		char *next = NULL;
		char *next2 = NULL;

		/* �X�y�[�X.����؂�ɕ�����𒊏o */
		RMsgBlock = strtok_s(ItemChkRMsg, ",", &next);

		if (strcmp(RMsgBlock, "@I") == 0)
		{
			next2 = next;
			//�A�C�e�����b�Z�[�W��� "@Item,%d,%d,X%d,Z%d&"
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
					//XZ�̐��l�������擾����
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
						//�f�[�^���i�[
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

		//�S�A�C�e�������擾�������m�F
		for (int ItemChkCnt = 0; ItemChkCnt < OBJECT_ITEM_MAX; ItemChkCnt++)
		{
			if (MySockObj->pitem->ItemParaAll[ItemChkCnt].NetUse == true)
			{
				ItemChk++;
			}
		}
		//�A�C�e�������m�F�ł���
		if (ItemChk == 20)
		{
			//��M�������b�Z�[�W���M
			toSendText[0] = NULL;
			sprintf_s(toSendText, "ItemOK");
			send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			MySockObj->SetNetItemFlag(true);
		}
	}
}

//=============================================================================
// �A�C�e������������@�l�b�g�ΐ�n�߂邽�߂̃J�E���g�_�E���M���擾����
//=============================================================================
void SOCK_UPDATE::NetCountdown(MySOCKET *MySockObj)
{
	char CountChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	CountChkRMsg[0] = NULL;
	//�Q�[���J�E���g�_�E���J�n
	//printf("�Q�[���J�E���g�_�E���J�n\n");
	if (MySockObj->GetNetCountDownFlag() == false)
	{
		char toSendText[500] = { NULL };
		int numrcv = recv(MySockObj->DstSocketFunc(), CountChkRMsg, BUFFER_SIZE_STRING, 0);
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
				sprintf_s(toSendText, "StartOK");
				send(MySockObj->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
				MySockObj->SetNetCountDownFlag(true);
				MySockObj->SetNetGameStartFlag(true);
				//�}���`�X���b�h�J�n�M��ON
				MySockObj->MultThreadFlagFunc(true);
				MySockObj->GameSceneFlagFunc(true);
			}
		}
		Sleep(1);
	}
}





