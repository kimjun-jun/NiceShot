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
#include "../../h/Net/SockSubThreadReceive.h"

#pragma comment (lib, "Ws2_32.lib")

//��MMANAGER
void SOCK_SUBTHREAD_RECEIVE::ReceiveFunc(MySOCKET *MySockObj)
{
	this->ReceivePacket(MySockObj);
}

void SOCK_SUBTHREAD_RECEIVE::ReceivePacket(MySOCKET *MySockObj)
{
	//���ۂ̃Q�[���ň����Ă���A�h���X���擾

	int MyNum = MySockObj->GetNetMyNumber();
	char RMsg[BUFFER_SIZE] = { NULL }; //�����Ă���f�[�^���e

	//�펞�󂯎�������B�f�[�^�̕ύX�����������̂݃A�N�Z�X�������A�Ώۃf�[�^�ɏ������݂��s���B
	int numrcv = recv(MySockObj->DstSocketFunc(), RMsg, BUFFER_SIZE_STRING, 0);
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
			ObjectAnalys(MySockObj,RMsg);
		}
	}
}

//��͏����@��ʏ����@�I�u�W�F�N�g����
void SOCK_SUBTHREAD_RECEIVE::ObjectAnalys(MySOCKET *MySockObj, char* argRMsg)
{
	char *RMsgBlock;
	char *next = NULL;
	/* �X�y�[�X.����؂�ɕ�����𒊏o */
	RMsgBlock = strtok_s(argRMsg, ",", &next);

	//�v���C���[0�̎��͂���
	if (strcmp(RMsgBlock, "@P0") == 0)
	{
		VariableAnalys(MySockObj, next, 0, "@P0,");
	}
	//�v���C���[1�̎��͂���
	else if (strcmp(RMsgBlock, "@P1") == 0)
	{
		VariableAnalys(MySockObj, next, 1, "@P1,");
	}
	//�v���C���[2�̎��͂���
	else if (strcmp(RMsgBlock, "@P2") == 0)
	{
		VariableAnalys(MySockObj, next, 2, "@P2,");
	}
	//�v���C���[3�̎��͂���
	else if (strcmp(RMsgBlock, "@P3") == 0)
	{
		VariableAnalys(MySockObj, next, 3, "@P3,");
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

		MySockObj->NetSetTikeiSeed(iSeed, iPlayerNum);
	}

	//�Q�[���I�����͂���
	if (strcmp(RMsgBlock, "@EndGame") == 0)
	{
		MySockObj->NetSetGameEnd();
	}
}

//��͏����@���ʏ����@�Ή��ϐ�����
void SOCK_SUBTHREAD_RECEIVE::VariableAnalys(MySOCKET *MySockObj, char* argVariableMsg, int PlayerNum, char* PlayerType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argVariableMsg, ",", &next);

	//XYZ�^�C�v1 switch����ˁH
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

	//XYZ�^�C�v2
	else if (strcmp(RMsgBlock, "BulletA") == 0)
	{
		XYZValue2Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_BulletA);
	}

	//XYZ�^�C�v4
	else if (strcmp(RMsgBlock, "BulletB") == 0)
	{
		XYZValue4Analys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_BulletB);
	}

	//INT�^�C�v
	else if (strcmp(RMsgBlock, "Vital") == 0)
	{
		INTValueAnalys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_Vital);
	}
	else if (strcmp(RMsgBlock, "Morphing") == 0)
	{
		INTValueAnalys(MySockObj, next, PlayerNum, PlayerType, VariableAnalysType_Morphing);
	}

}

//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3 XYZ�^�C�v1
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
		//XYZ�̐��l�������擾����
		GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
			//�f�[�^���i�[
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
			//��͎c�肪����Ȃ�ċA����
			if (next[0] != NULL)
			{
				//argValueMsg = strtok_s(next, PlayerType, &next);
				//VariableAnalys(, PlayerType, );
				ObjectAnalys(MySockObj, next);
			}
			//��͎c��Ȃ��@�o�b�t�@����Ȃ��͏I��
			else
			{
				break;
			}
		}
	}
}

//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3 *2 XYZ�^�C�v2
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
		//XYZ�̐��l�������擾����
		GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
				MySockObj->NetSetBulletType1(buffpos, buffmove, PlayerNum);
				if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
				break;
			}
		}
	}
}

//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3 *4 XYZ�^�C�v4
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
		//XYZ�̐��l�������擾����
		GetVal = strtok_s(XYZnext, ",", &XYZnext);//GetVal=X000.000,Y000.000,Z000.000  next=����Rot
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
						MySockObj->NetSetBulletType3(buffpos, &buffmove[0], PlayerNum);
						if (next[0] != NULL) argValueMsg = strtok_s(next, PlayerType, &next);
					}
				}
			}
			break;
		}
	}
}

//��͏����@���ʏ����@���l�f�[�^���ʁ@int *1 INT�^�C�v
void SOCK_SUBTHREAD_RECEIVE::INTValueAnalys(MySOCKET *MySockObj, char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType)
{
	char *RMsgBlock;
	char *next = NULL;
	RMsgBlock = strtok_s(argValueMsg, "&", &next);

	int buff;
	buff = atoi(RMsgBlock);
	//�f�[�^���i�[
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





