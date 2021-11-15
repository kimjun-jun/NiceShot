/**
* @file sock.cpp
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
#include "../../h/Object/ObjectClass/objectclass.h"
#include "../../h/Object/Player/player.h"
#include "../../h/Object/Item/item.h"
#include "../../h/Object/Bullet/bullet.h"
#include "../../h/Map/field.h"
#include "../../h/Net/sock.h"

#pragma comment (lib, "Ws2_32.lib")


//STEP1	���[���ʐM	OK
//STEP2	LAN�ʐM		OK
//STEP3	WAN�ʐM		OK
//�ʐM�f�[�^�̃o�b�t�@�@���̒l�ƃN���C�A���g���݂̒l���r���ĕύX������΍ŐV�f�[�^�𑗐M����
//����܂ł���1�t���[���ŉ���������f�[�^�𑗐M���Ă���̂ŁA�ύX���������Ƃ��Ɉ�x�������M����
//WAN�v���g�R���̃|�[�g6432
//�|�[�g�J��
//�ő��300�o�C�g���炢
//sleep�����Ă���
//�đ�����,�ƍ����䂪����
//


struct SENDMSG_BUFF
{
	SENDMSG_BUFF() {
		PlayerPos = VEC3_ALL0;
		HoudaiRot = VEC3_ALL0;
		HoutouRot = VEC3_ALL0;
		HousinRot = VEC3_ALL0;
		BPos = VEC3_ALL0;
		ItemSyncUse = false;
	}

	//�v���C���[�f�[�^
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 HoudaiRot;
	D3DXVECTOR3 HoutouRot;
	D3DXVECTOR3 HousinRot;
	int Vital;
	bool Morphing;
	D3DXVECTOR3 BPos;

	//�A�C�e���f�[�^
	bool ItemSyncUse;

};

//SENDMSG_BUFF g_SendMsgBuff;

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


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
MainMsgData g_MainMsgData;

MySOCKET::MySOCKET(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//syokika ro-karu 
	char l_Destination[80] = { NULL };
	//unsigned short l_Port = 6432;//WAN�̐l�̓v���g�R���̃|�[�g�@LAN�⓯�[���͓����[���̃|�[�g
	unsigned short l_Port = 27015;//WAN�̐l�̓v���g�R���̃|�[�g�@LAN�⓯�[���͓����[���̃|�[�g
	sockaddr_in l_DstAddr;

	sprintf_s(l_Destination, "192.168.11.2"); //ro-karu
	//sprintf_s(l_Destination, "106.73.19.98"); //guro-baru

	/* Windows �Ǝ��̐ݒ� */
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	/* sockaddr_in �\���̂̃Z�b�g */
	inet_pton(AF_INET, l_Destination, &l_DstAddr.sin_addr);
	l_DstAddr.sin_port = htons(l_Port);
	l_DstAddr.sin_family = AF_INET;

	//de^ta hannein
	this->DestinationFunc(l_Destination);
	this->PortFunc(l_Port);
	this->DstAddrFunc(l_DstAddr);

	/* �\�P�b�g���� */
	//�X�g���[���^�C�v�[����̂ł������o�b�t�@�B�f�[�^�̋�؂������������K�v���o�Ă���@TCP�@�A�N�Z�v�g�Ȃǂ̏������K�v
	//�f�[�^�O�����^�C�v�[���f�[�^���Ƃŋ�؂��Ă���B���܂��܂Ƃ����f�[�^�𑽐������ď�������B�@UDP�@�A�N�Z�v�g�Ȃǂ̏������K�v�Ȃ�
	this->DstSocketFunc(socket(AF_INET, SOCK_STREAM, 0));

	// �����ŁA�m���u���b�L���O�ɐݒ肵�Ă��܂��B
	 // val = 0�Ńu���b�L���O���[�h�ɐݒ�ł��܂��B
	 // �\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�ł��B
	u_long val = 1;
	ioctlsocket(this->DstSocketFunc(), FIONBIO, &val);
}

MySOCKET::~MySOCKET(void)
{
	/* Windows �Ǝ��̐ݒ� */
	closesocket(this->DstSocketFunc());
	WSACleanup();
}

void MySOCKET::Init(void)
{
	//�t���O���I�t�ɂ���
	ConnectFlag = false;
	CountDownFlag = false;
}

void MySOCKET::NetMatch(void)
{
	/* �ڑ� */
	//printf("Trying to connect to %s: \n", destination);

	//��x�R�l�N�g����ƃt���O������
	if (ConnectFlag == false)
	{
		//�ڑ��v��
		connect(this->DstSocketFunc(), (struct sockaddr *) &this->DstAddrFunc(), sizeof(this->DstAddrFunc()));
		ConnectFlag = true;
	}
	//�}�b�`���O
	bool ChkMatch = false;
	bool ChkSend = false;//�G���g���[�͈�x��������Ȃ�
	//while (ChkMatch != true)
	//{
		//�Q�[���I������
		//if (GetGameLoop() == true)
		//{
		//	SetEndGame(true);
		//	this->MultThreadFlagFunc(false);
		//	this->GameSceneFlagFunc(false);
		//	break;
		//}

	char ConnectRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	ConnectRMsg[0] = NULL;
	char toSendText[500] = { NULL };
	sprintf_s(toSendText, "Entry");
	if (ChkSend == false)//WAN���ŗv���񐔂��ς��
	{
		send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
		ChkSend = true;
	}
	printf("�}�b�`���O��\n");
	int numrcv = recv(this->DstSocketFunc(), ConnectRMsg, BUFFER_SIZE_STRING, 0);
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
			//send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			ChkMatch = true;
			SetNetMatchFlag(true);

		}
	}
	Sleep(10);
	//}
}

void MySOCKET::NetMyNumberGet(void)
{
	//�l�ԍ��擾
	//bool ChkMyNumber = false;
	//while (ChkMyNumber != true)
	//{
	char MyNumChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	MyNumChkRMsg[0] = NULL;
	printf("�l�ԍ��擾��\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(this->DstSocketFunc(), MyNumChkRMsg, BUFFER_SIZE_STRING, 0);
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
			send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			//printf("received: �l�ԍ��擾%s\n", Packetbuffer);
			//ChkMyNumber = true;
			int num = packet;
			SetNetMyNumber(num);
			SetNetMyNumberFlag(true);
		}
	}
	Sleep(10);
	//}
}

void MySOCKET::NetItemGet(ITEM* item)
{
	//�A�C�e�����擾
	int ItemChk = 0;
	char ItemChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	ItemChkRMsg[0] = NULL;
	//printf("�A�C�e�����擾��\n");
	char toSendText[500] = { NULL };
	int numrcv = recv(this->DstSocketFunc(), ItemChkRMsg, BUFFER_SIZE_STRING, 0);
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
						NetSetItem(item, buff, iIndex, iType);
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
			if (item->ItemParaAll[ItemChkCnt].NetUse == true)
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
			send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
			SetNetItemFlag(true);
		}
	}
}

void MySOCKET::NetCountdown(void)
{
	char CountChkRMsg[BUFFER_SIZE]; //�����Ă���f�[�^���e
	CountChkRMsg[0] = NULL;
	//�Q�[���J�E���g�_�E���J�n
	//printf("�Q�[���J�E���g�_�E���J�n\n");
	if (CountDownFlag == false)
	{
		char toSendText[500] = { NULL };
		int numrcv = recv(this->DstSocketFunc(), CountChkRMsg, BUFFER_SIZE_STRING, 0);
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
				send(this->DstSocketFunc(), toSendText, BUFFER_SIZE_STRING, 0);
				CountDownFlag = true;
				SetNetGameStartFlag(true);
				//�}���`�X���b�h�J�n�M��ON
				this->MultThreadFlagFunc(true);
				this->GameSceneFlagFunc(true);
			}
		}
		Sleep(1);
	}
}

//�ȉ��}���`�X���b�h���Ŏ��s
void MySOCKET::Packet(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet, SHADOW *Shadow)
{
	while (this->GameSceneFlagFunc() == true)
	{
		if (this->MultThreadFlagFunc() == true)
		{
			//�Q�[����
			//�p�P�b�g�𐧌�
			SendPacket(Player, Item, Field, Bullet);
			ReceivePacket(Player, Item, Field, Bullet, Shadow);
			//�����p�X���[�v�@���ꂪ�Ȃ��ƃ����^�C���G���[�ɂȂ�
			//receive��send�̉񐔂Ɋ֌W�����肻���B�������҂��Ă���receive���Ȃ��Ƃ������Ȃ�
			Sleep(1);

			//�Q�[���I������@�A�v���E��X�������ꂽ��I������
			if (GetEndGame() == true) break;
		}
	}
}

void MySOCKET::SendPacket(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet)
{
	int MyNum = GetNetMyNumber();
	char SMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
	char ItemSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
	char ItemUseSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
	char EndGameMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e

	//--------�ύX�������m�F�@�ύX�ӏ��������send()����
	//---------------�v���C���[
	//���W
	D3DXVECTOR3 Pos = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 OldPos = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldPos();
	//if (Pos != OldPos)
	{
		//g_SendMsgBuff.PlayerPos = Pos;
		//Player[MyNum].NetChkPos = true;
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,Pos,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, Pos.x, Pos.y, Pos.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	//��]
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoudaiOldRot = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldRot();
	if (HoudaiRot != HoudaiOldRot)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HoudaiRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoudaiRot.x, HoudaiRot.y, HoudaiRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HoutouRot = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HoutouOldRot = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].OldRot();
	if (HoutouRot != HoutouOldRot)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HoutouRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HoutouRot.x, HoutouRot.y, HoutouRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	D3DXVECTOR3 HousinRot = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();
	D3DXVECTOR3 HousinOldRot = Player->modelDraw[MyNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].OldRot();
	if (HousinRot != HousinOldRot)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,HousinRot,X%4.3f,Y%4.3f,Z%4.3f&", MyNum, HousinRot.x, HousinRot.y, HousinRot.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}
	//�o�C�^��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (Player->PlayerPara[CntPlayer].StandardPara.Vital != Player->PlayerPara[CntPlayer].StandardPara.OldVital)
		{
			int Vital = Player->PlayerPara[CntPlayer].StandardPara.Vital;
			//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
			char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(NewSMsg, "@P%d,Vital,%d&", CntPlayer, Vital);
			sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		}
	}
	//���[�t�B���O ���[�t�B���O�M��ON(�A�^�b�N���f���̎��A�펞ON)���b�Z�[�W���M�Breceive����1���󂯑�����B
	if (Player->PlayerPara[MyNum].MorphingPara.NetGetMorphingOneFrame == true)//NetGetMorphing
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,Morphing,%d&", MyNum, 1);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
	}

	//---------------------------�o���b�g
	//�o���b�g�𔭎˂��Ă��Ȃ�0,�ʏ탂�f���̔���1,�A�^�b�N���f���̔���3
	switch (Player->PlayerPara[MyNum].BulletPara.NetBulletShotFlagOneFrame)
	{
	case 0:
		break;
	case 1:
	{
		D3DXVECTOR3 BPos = Player->PlayerPara[MyNum].BulletPara.BulletStartPos;
		D3DXVECTOR3 BMove = Player->PlayerPara[MyNum].BulletPara.BulletMove[0];
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@P%d,BulletA,PX%4.3f,PY%4.3f,PZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f&"
			, MyNum, BPos.x, BPos.y, BPos.z, BMove.x, BMove.y, BMove.z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		break;
	}
	case 3:
	{
		D3DXVECTOR3 BPos = Player->PlayerPara[MyNum].BulletPara.BulletStartPos;
		D3DXVECTOR3 BMove[3];
		BMove[0] = Player->PlayerPara[MyNum].BulletPara.BulletMove[0];
		BMove[1] = Player->PlayerPara[MyNum].BulletPara.BulletMove[1];
		BMove[2] = Player->PlayerPara[MyNum].BulletPara.BulletMove[2];
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg,
			"@P%d,BulletB,PX%4.3f,PY%4.3f,PZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f,MX%4.3f,MY%4.3f,MZ%4.3f&"
			, MyNum, BPos.x, BPos.y, BPos.z, BMove[0].x, BMove[0].y, BMove[0].z
			, BMove[1].x, BMove[1].y, BMove[1].z, BMove[2].x, BMove[2].y, BMove[2].z);
		sprintf_s(SMsg, "%s%s", SMsg, NewSMsg);
		break;
	}
	}

	//---------------------------�A�C�e���擾�M��
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		if (Item->ItemParaAll[CntItem].NetGetItemFlag == true &&
			Item->ItemParaAll[CntItem].GetPlayerType == MyNum)
		{
			int ItemT = Item->ItemParaAll[CntItem].eType;
			D3DXVECTOR3 ItemPos = Item->Transform[CntItem].Pos();
			//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
			char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(NewSMsg, "@I,N%d,T%d,X%4.3f,Y%4.3f,Z%4.3f&"
				, CntItem, ItemT, ItemPos.x, ItemPos.y, ItemPos.z);
			sprintf_s(ItemSMsg, "%s%s", ItemSMsg, NewSMsg);
		}
	}

	//---------------------------�A�C�e���g�p�M��
	/*
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		//���擾
		bool ItemUse = Item[CntItem].Use();
		int ItemT = Item[CntItem].nType;
		//���M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL };
		sprintf_s(NewSMsg, "@UI,N%d,T%d,U%d&"
			, CntItem, ItemT, ItemUse);
		sprintf_s(ItemUseSMsg, "%s%s", ItemUseSMsg, NewSMsg);
	}

	*/

	//---------------------------�Q�[���V�[���I�����b�Z�[�W
	int Puse = 0;
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (Player->iUseType[CntPlayer].Use() == NoUse) Puse++;
	}
	if (Puse >= 3)
	{
		//�ύX������̂ő��M�p���b�Z�[�W�ɏ�������
		char NewSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
		sprintf_s(NewSMsg, "@EndGame&");
		sprintf_s(EndGameMsg, "%s%s", EndGameMsg, NewSMsg);
	}

	//----------------------------�ύX���������ꍇsend()����@��{���
	if (SMsg[0] != NULL)
	{
		send(this->DstSocketFunc(), SMsg, BUFFER_SIZE_STRING, 0);
	}

	//----------------------------�ύX���������ꍇsend()����@�A�C�e���擾���
	if (ItemSMsg[0] != NULL)
	{
		send(this->DstSocketFunc(), ItemSMsg, BUFFER_SIZE_STRING, 0);

		//�n�`�A�C�e�����擾���ꂽ�������ǉ��̃��b�Z�[�W�𑗐M����
		if (Field->FieldPara.TikeiSeed != Field->FieldPara.OldTikeiSeed)
		{
			char SpecialSMsg[BUFFER_SIZE] = { NULL }; //����f�[�^���e
			sprintf_s(SpecialSMsg, "@T,S%d,N%d&", Field->FieldPara.TikeiSeed, Field->FieldPara.ItemGetPlayerNum);
			send(this->DstSocketFunc(), SpecialSMsg, BUFFER_SIZE_STRING, 0);
		}
	}

	//�ύX���������ꍇsend()����@��{���
	if (EndGameMsg[0] != NULL)
	{
		send(this->DstSocketFunc(), EndGameMsg, BUFFER_SIZE_STRING, 0);
	}

	//�펞send()����@�A�C�e���g�p���
	//send(this->DstSocketFunc(), ItemUseSMsg, BUFFER_SIZE_STRING, 0);

}

void MySOCKET::ReceivePacket(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet, SHADOW *Shadow)
{
	//���ۂ̃Q�[���ň����Ă���A�h���X���擾

	int MyNum = GetNetMyNumber();
	char RMsg[BUFFER_SIZE] = { NULL }; //�����Ă���f�[�^���e

	//�펞�󂯎�������B�f�[�^�̕ύX�����������̂݃A�N�Z�X�������A�Ώۃf�[�^�ɏ������݂��s���B
	int numrcv = recv(this->DstSocketFunc(), RMsg, BUFFER_SIZE_STRING, 0);
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
			MsgAnalys(RMsg, Player, Item, Field, Bullet, Shadow);
		}
	}
}

void MySOCKET::MsgAnalys(char* argRMsg, PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet,SHADOW *Shadow)
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
					NetSetPos(Player,buff, 0);
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
					NetSetHoudaiRot(Player,buff, 0);
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
					NetSetHoutouRot(Player,buff, 0);
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
					NetSetHousinRot(Player,buff, 0);
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
			//�f�[�^���i�[
			NetSetVital(Player,buff, 0);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(Player,0);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//Move�擾
					{
						D3DXVECTOR3 buffmove;
						char *XYZnextmove = XYZnext;
						for (int CntXYZmove = 0; CntXYZmove < 3; CntXYZmove++)
						{
							char *GetValmove = NULL;
							char *SetValmove = NULL;
							char *YZnextmove = NULL;
							char *nullpmove = NULL;
							//XYZ�̐��l�������擾����
							GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

						//�f�[�^���i�[
						NetSetBulletType1(Player,Bullet,Shadow,buffpos, buffmove, 0);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//Move�擾
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
								//XYZ�̐��l�������擾����
								GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

							//�f�[�^���i�[
							if (MoveCnt == 2)
							{
								NetSetBulletType3(Player,Bullet,Shadow,buffpos, &buffmove[0], 0);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P0,", &next);
							}
						}
					}
					break;
				}
			}
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
					NetSetPos(Player,buff, 1);
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
					NetSetHoudaiRot(Player,buff, 1);
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
					NetSetHoutouRot(Player,buff, 1);
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
					NetSetHousinRot(Player,buff, 1);
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
			//�f�[�^���i�[
			NetSetVital(Player,buff, 1);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(Player,1);
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
			//XYZ�̐��l�������擾����
			GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

				//Move�擾
				{
					D3DXVECTOR3 buffmove;
					char *XYZnextmove = XYZnext;
					for (int CntXYZmove = 0; CntXYZmove < 3; CntXYZmove++)
					{
						char *GetValmove = NULL;
						char *SetValmove = NULL;
						char *YZnextmove = NULL;
						char *nullpmove = NULL;
						//XYZ�̐��l�������擾����
						GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//�f�[�^���i�[
					NetSetBulletType1(Player,Bullet,Shadow,buffpos, buffmove, 1);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//Move�擾
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
								//XYZ�̐��l�������擾����
								GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

							//�f�[�^���i�[
							if (MoveCnt == 2)
							{
								NetSetBulletType3(Player,Bullet,Shadow,buffpos, &buffmove[0], 1);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P1,", &next);
							}
						}
					}
					break;
				}
			}
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
					NetSetPos(Player,buff, 2);
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
					NetSetHoudaiRot(Player,buff, 2);
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
					NetSetHoutouRot(Player,buff, 2);
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
					NetSetHousinRot(Player,buff, 2);
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
			//�f�[�^���i�[
			NetSetVital(Player,buff, 2);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(Player, 2);
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
			//XYZ�̐��l�������擾����
			GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

				//Move�擾
				{
					D3DXVECTOR3 buffmove;
					char *XYZnextmove = XYZnext;
					for (int CntXYZmove = 0; CntXYZmove < 3; CntXYZmove++)
					{
						char *GetValmove = NULL;
						char *SetValmove = NULL;
						char *YZnextmove = NULL;
						char *nullpmove = NULL;
						//XYZ�̐��l�������擾����
						GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//�f�[�^���i�[
					NetSetBulletType1(Player,Bullet,Shadow,buffpos, buffmove, 2);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//Move�擾
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
								//XYZ�̐��l�������擾����
								GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

							//�f�[�^���i�[
							if (MoveCnt == 2)
							{
								NetSetBulletType3(Player,Bullet,Shadow,buffpos, &buffmove[0], 2);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P2,", &next);
							}
						}
					}
					break;
				}
			}
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
					NetSetPos(Player,buff, 3);
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
					NetSetHoudaiRot(Player,buff, 3);
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
					NetSetHoutouRot(Player,buff, 3);
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
					NetSetHousinRot(Player,buff, 3);
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
			//�f�[�^���i�[
			NetSetVital(Player,buff, 3);
			if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
		}
		if (strcmp(RMsgBlock, "Morphing") == 0)
		{
			RMsgBlock = strtok_s(NULL, "&", &next);
			int buff;
			buff = atoi(RMsgBlock);
			//�f�[�^���i�[
			NetSetMorphing(Player,3);
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
			//XYZ�̐��l�������擾����
			GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

				//Move�擾
				{
					D3DXVECTOR3 buffmove;
					char *XYZnextmove = XYZnext;
					for (int CntXYZmove = 0; CntXYZmove < 3; CntXYZmove++)
					{
						char *GetValmove = NULL;
						char *SetValmove = NULL;
						char *YZnextmove = NULL;
						char *nullpmove = NULL;
						//XYZ�̐��l�������擾����
						GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//�f�[�^���i�[
					NetSetBulletType1(Player,Bullet,Shadow,buffpos, buffmove, 3);
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
				//XYZ�̐��l�������擾����
				GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

					//Move�擾
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
								//XYZ�̐��l�������擾����
								GetValmove = strtok_s(XYZnextmove, ",", &XYZnextmove);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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

							//�f�[�^���i�[
							if (MoveCnt == 2)
							{
								NetSetBulletType3(Player,Bullet,Shadow,buffpos, &buffmove[0], 3);
								if (next[0] != NULL) RMsgBlock = strtok_s(next, "@P3,", &next);
							}
						}
					}
					break;
				}
			}
		}
	}

	//�A�C�e�����͂���
	if (strcmp(RMsgBlock, "@I") == 0)
	{
		char *next2 = NULL;
		next2 = next;
		//�A�C�e�����b�Z�[�W��� "@Item,%d,%d,X%d,Z%d&"
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
				NetSetItem(Item,buff, iIndex, iType);
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

	//�n�`���͂���
	if (strcmp(RMsgBlock, "@T") == 0)
	{
		char *next2 = NULL;
		next2 = next;
		//�n�`�A�C�e���������ǉ��̃��b�Z�[�W
		char *Seed;
		Seed = strtok_s(next2, "S,", &next);
		char *PlayerNum;
		PlayerNum = strtok_s(NULL, "N", &next);
		int iSeed = atoi(Seed);
		int iPlayerNum = atoi(PlayerNum);

		NetSetTikeiSeed(Field,Item,iSeed, iPlayerNum);
	}

	//�Q�[���I�����͂���
	if (strcmp(RMsgBlock, "@EndGame") == 0)
	{
		//char *next2 = NULL;
		//next2 = next;
		////�Q�[���I���M���@@EndGame
		//char *EndMsg;
		//EndMsg = strtok_s(next2, "&", &next);

		NetSetGameEnd();
	}
}

//���f�����ꊇ�֐��@����g�p�\��
void MySOCKET::SetBuff(char* RMsgBlock, int SetEnumType,int PlayerNum)
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
				//NetSetPos(Player,buff, PlayerNum);
				break;
			case SetEnumHoudaiRot:
				//NetSetHoudaiRot(Player,buff, PlayerNum);
				break;
			case SetEnumHoutouRot:
				//NetSetHoutouRot(Player,buff, PlayerNum);
				break;
			case SetEnumHousinRot:
				//NetSetHousinRot(Player,buff, PlayerNum);
				break;
			}
			break;
		}
	}
}

void MySOCKET::NetSetItem(ITEM *Item, D3DXVECTOR3 buff, int index, int type)
{
	if (Item->iUseType[index].Use() == false)
	{
		Item->SetInstance(index, buff, D3DXVECTOR3(2.0f, 2.0f, 2.0f), VEC3_ALL0, static_cast<eITEM_TYPE>(type));
		Item->ItemParaAll[index].NetUse = true;
	}
}

void MySOCKET::NetSetTikeiSeed(FIELD *Field, ITEM *Item, int Seed, int PlayerNum)
{
	if (Field->FieldPara.NetTikei == false)
	{
		srand(Seed);
		Field->FieldPara.TikeiSeed = Seed;
		Field->FieldPara.NetTikei = true;
		Field->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, PlayerNum, Item);
		//SetFieldInterPolationFieldType(0);
		PlaySound(SOUND_LABEL_SE_enter03);
		PlaySound(SOUND_LABEL_SE_quake);
	}
}

void MySOCKET::NetSetPos(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		Player->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(buff);
	}
}

void MySOCKET::NetSetHoudaiRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		Player->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(buff);
	}
}

void MySOCKET::NetSetHoutouRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		Player->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(buff);
	}
}

void MySOCKET::NetSetHousinRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{	
		Player->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(buff);
	}
}

void MySOCKET::NetSetVital(PLAYER *Player, int buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		Player->PlayerPara[PlayerNum].StandardPara.Vital = buff;
	}
}

void MySOCKET::NetSetMorphing(PLAYER *Player, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		//���[�J�������Ɠ����@CheackHit
		if (Player->PlayerPara[PlayerNum].StandardPara.eModelType == PLAYER_MODEL_TYPE_NORMAL)
		{
			//���[�t�B���O�J�n�M���A���f���^�C�v�A���[�t�B���O���M���A���[�t�B���O���ԁA���[�t�B���O�I���J�E���g�̃Z�b�g
			Player->PlayerPara[PlayerNum].MorphingPara.MorphingStart = true;
			Player->PlayerPara[PlayerNum].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
			Player->PlayerPara[PlayerNum].MorphingPara.MorphingSignal = NowMorphing;
			Player->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else if (Player->PlayerPara[PlayerNum].MorphingPara.MorphingStart == false && Player->PlayerPara[PlayerNum].MorphingPara.MorphingTime <= 0.0f)
		{
			Player->PlayerPara[PlayerNum].MorphingPara.MorphingStart = true;
			Player->PlayerPara[PlayerNum].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
			Player->PlayerPara[PlayerNum].MorphingPara.MorphingSignal = NowMorphing;
			Player->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else
		{
			Player->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
		}
	}
}

void MySOCKET::NetSetBulletType1(PLAYER *Player, BULLET *Bullet, SHADOW *Shadow, D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		//�t���O��0�Ȃ甭�˂��Ă�����B1�t���[����1�񔭎˂ł���B�����Ńt���O��1�ɂ��čX�V������0�ɂ��邵�Ă���
		if (Player->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame == 0)
		{
				Bullet->SetInstance(buffpos, buffmove,
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					static_cast<ePLAYER_TYPE>(PlayerNum), Shadow);
				Player->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame = 1;
		}
	}
}

void MySOCKET::NetSetBulletType3(PLAYER *Player, BULLET *Bullet, SHADOW *Shadow, D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		//�t���O��0�Ȃ甭�˂��Ă�����B1�t���[����1�񔭎˂ł���B�����Ńt���O��1�ɂ��čX�V������0�ɂ��邵�Ă���
		if (Player->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame == 0)
		{
		
			for (int i = 0; i < 3; i++)
			{
				Bullet->SetInstance(buffpos, buffmove[i],
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					static_cast<ePLAYER_TYPE>(PlayerNum), Shadow);
			}
			Player->PlayerPara[PlayerNum].BulletPara.NetBulletShotFlagOneFrame = 1;
		}
	}
}

void MySOCKET::NetSetGameEnd(void)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		//�ΐ�t���O���I��������
		this->MultThreadFlagFunc(false);
	}
}










