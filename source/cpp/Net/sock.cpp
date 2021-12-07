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
#include "../../h/Object/ObjectClass/Interface/interface.h"
#include "../../h/Object/Player/player.h"
#include "../../h/Object/Item/item.h"
#include "../../h/Object/Bullet/bullet.h"
#include "../../h/Object/Scene/Scene.h"
#include "../../h/Map/field.h"
#include "../../h/Net/SockUpdate.h"
#include "../../h/Net/SockSubThreadSend.h"
#include "../../h/Net/SockSubThreadReceive.h"
#include "../../h/Net/sock.h"

#pragma comment (lib, "Ws2_32.lib")

//WAN�v���g�R���̃|�[�g6432
//�|�[�g�J��
//�ő��300�o�C�g���炢
//sleep�����Ă���
//�đ�����,�ƍ�����

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

MySOCKET::MySOCKET(void)
{
	//����������
	this->GameSceneFlagFunc(true);
	this->MultThreadFlagFunc(false);

	char l_Destination[80] = { NULL };
	//unsigned short l_Port = 6432;//WAN�̐l�̓v���g�R���̃|�[�g�@LAN�⓯�[���͓����[���̃|�[�g
	unsigned short l_Port = 27015;//WAN�̐l�̓v���g�R���̃|�[�g�@LAN�⓯�[���͓����[���̃|�[�g
	sockaddr_in l_DstAddr;

	sprintf_s(l_Destination, "192.168.11.2"); //���[�J���l�b�g���[�N�̐l�͂�����g��
	//sprintf_s(l_Destination, "106.73.19.98"); //�O���[�o���l�b�g���[�N�̐l�͂�����g��

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

//=============================================================================
// ���N���X�̃A�h���X�擾
//=============================================================================
void MySOCKET::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pplayer = obj->GetPlayer();
	pitem = obj->GetItem();
	pbullet = obj->GetBullet();
	pfield = obj->GetField();
	pscene = obj->GetScene();
	pGameObjInstance = obj;
}

//=============================================================================
// ����������
//=============================================================================
void MySOCKET::Init(void)
{
	//�t���O���I�t�ɂ���
	NetMatchFlag = false;
	NetMyNumberFlag = false;
	NetMyNumber = -1;
	NetItemFlag = false;
	NetGameStartFlag = false;
	NetShareDateFlag = false;
	this->MultThreadFlagFunc(false);
	NetConnectFlag = false;
	NetCountDownFlag = false;
}

//=============================================================================
// �X�V����
//=============================================================================
void MySOCKET::Update(void)
{
	pSockUpdate->UpdateFunc(this);
}

//=============================================================================
// �}���`�X���b�h����
//=============================================================================
void MySOCKET::Packet(void)
{
	while (this->GameSceneFlagFunc() == true)
	{
		//�l�b�g�ΐ펞��������
		if (this->MultThreadFlagFunc() == true)
		{
			//����M����
			this->pSockSubThreadSend->SendFunc(this);
			this->pSockSubThreadReceive->ReceiveFunc(this);
			//�����p�X���[�v�@���ꂪ�Ȃ��ƃ����^�C���G���[�ɂȂ� receive��send�̉񐔂Ɋ֌W�����肻���B�������҂��Ă���receive���Ȃ��Ƃ������Ȃ�
			Sleep(1);
			//�Q�[���I������@�A�v���E��X�������ꂽ��I������
			if (GetEndGame() == true) break;
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�������f����
//=============================================================================
void MySOCKET::NetSetItem(D3DXVECTOR3 buff, int index, int type)
{
	if (pitem->iUseType[index].Use() == false)
	{
		pitem->SetInstance(index, buff, D3DXVECTOR3(2.0f, 2.0f, 2.0f), VEC3_ALL0, static_cast<eITEM_TYPE>(type));
		pitem->ItemParaAll[index].NetUse = true;
	}
}

void MySOCKET::NetSetTikeiSeed(int Seed, int PlayerNum)
{
	if (pfield->FieldPara.NetTikei == false)
	{
		srand(Seed);
		pfield->FieldPara.TikeiSeed = Seed;
		pfield->FieldPara.NetTikei = true;
		pfield->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, PlayerNum);
		//SetFieldInterPolationFieldType(0);
		PlaySound(SOUND_LABEL_SE_enter03);
		PlaySound(SOUND_LABEL_SE_quake);
	}
}

void MySOCKET::NetSetPos(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(buff);
	}
}

void MySOCKET::NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(buff);
	}
}

void MySOCKET::NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(buff);
	}
}

void MySOCKET::NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		pplayer->modelDraw[PlayerNum].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(buff);
	}
}

void MySOCKET::NetSetVital(int buff, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		pplayer->PlayerPara[PlayerNum].StandardPara.Vital = buff;
	}
}

void MySOCKET::NetSetMorphing(int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		//���[�J�������Ɠ����@CheackHit
		if (pplayer->PlayerPara[PlayerNum].StandardPara.eModelType == PLAYER_MODEL_TYPE_NORMAL)
		{
			//���[�t�B���O�J�n�M���A���f���^�C�v�A���[�t�B���O���M���A���[�t�B���O���ԁA���[�t�B���O�I���J�E���g�̃Z�b�g
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingStart = true;
			pplayer->PlayerPara[PlayerNum].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingSignal = NowMorphing;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else if (pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingStart == false && pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime <= 0.0f)
		{
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingStart = true;
			pplayer->PlayerPara[PlayerNum].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingSignal = NowMorphing;
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
			PlaySound(SOUND_LABEL_SE_rap1);
		}
		else
		{
			pplayer->PlayerPara[PlayerNum].MorphingPara.MorphingTime = MORPHING_TIME;
		}
	}
}

void MySOCKET::NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		//�t���O��0�Ȃ甭�˂��Ă�����B1�t���[����1�񔭎˂ł���B�����Ńt���O��1�ɂ��čX�V������0�ɂ��邵�Ă���
		if (pplayer->PlayerPara[PlayerNum].BulletPara.GetNetBulletShotFlagOneFrame() == 0)
		{
			pbullet->SetInstance(buffpos, buffmove,
				BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
				static_cast<ePLAYER_TYPE>(PlayerNum));
			pplayer->PlayerPara[PlayerNum].BulletPara.SetNetBulletShotFlagOneFrame(1);
		}
	}
}

void MySOCKET::NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum)
{
	//���b�N����ĂȂ���Ε��͂��ď�������
	if (GetNetShareDateFlag() == false)
	{
		//�t���O��0�Ȃ甭�˂��Ă�����B1�t���[����1�񔭎˂ł���B�����Ńt���O��1�ɂ��čX�V������0�ɂ��邵�Ă���
		if (pplayer->PlayerPara[PlayerNum].BulletPara.GetNetBulletShotFlagOneFrame() == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				pbullet->SetInstance(buffpos, buffmove[i],
					BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME,
					static_cast<ePLAYER_TYPE>(PlayerNum));
			}
			pplayer->PlayerPara[PlayerNum].BulletPara.SetNetBulletShotFlagOneFrame(1);
		}
	}
}

void MySOCKET::NetSetGameEnd(void)
{
	//�ΐ�t���O���I��������
	this->MultThreadFlagFunc(false);
}

