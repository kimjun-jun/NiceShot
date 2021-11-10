/**
* @file sockTCP.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#define BUFFER_SIZE 500
#define BUFFER_SIZE_STRING 500					//!< ���M�f�[�^�o�b�t�@�T�C�Y ���M��M���ɃT�C�Y�����킹�Ȃ��ƁA�������ǂݎ��G���[�ɂȂ�@�������X�g���[���ŏ��X�ɂ���Ă���

enum
{
	SetEnumPos = 0,
	SetEnumHoudaiRot = 1,
	SetEnumHoutouRot = 2,
	SetEnumHousinRot = 3,
};


/**
*�@@class MySOCKET
*�@@brief GAMEOBJECT�h���N���X
*/
class MySOCKET : public GAME_OBJECT
{
public:
	MySOCKET();	//!< �\�P�b�g�ݒ�
	~MySOCKET();	//!< �\�P�b�g�폜

	void Init(void);	//!< ������

	//�l�b�g�ΐ�J�n����
	void NetMatch(void);
	void NetMyNumberGet(void);
	void NetItemGet(ITEM *item);
	void NetCountdown(void);

	//---------�}���`�X���b�h���Ŏ��s�@�A�v���N�����Ƀ}���`�X���b�h�ŉi�v���[�v
	void Packet(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet,SHADOW *Shadow);
	//�l�b�g�ΐ���s������
	void SendPacket(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet);	//���M����
	void ReceivePacket(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet, SHADOW *Shadow);		//��M����
	void MsgAnalys(char* argRMsg, PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet, SHADOW *Shadow);	//��͏���

	//��������
	void SetBuff(char* RMsgBlock, int Type, int PlayerNum);	//RMsgBlock�̓f�[�^�̃u���b�N�@Type��Pos��Rot�Ȃǂ̎��

	//�A�C�e�����f
	void NetSetItem(ITEM *Item, D3DXVECTOR3 buff, int index, int type);

	//�n�`���f
	void NetSetTikeiSeed(FIELD *Field, ITEM *Item, int Seed, int PlayerNum);
	
	//�v���C���[���f�Q
	void NetSetPos(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoudaiRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoutouRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHousinRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetVital(PLAYER *Player, int buff, int PlayerNum);
	void NetSetMorphing(PLAYER *Player, int PlayerNum);
	void NetSetBulletType1(PLAYER *Player, BULLET *Bullet, SHADOW *Shadow, D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum);
	void NetSetBulletType3(PLAYER *Player, BULLET *Bullet, SHADOW *Shadow, D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum);

	void NetSetGameEnd(void);
	//---------�}���`�X���b�h���Ŏ��s�@�A�v���N�����Ƀ}���`�X���b�h�ŉi�v���[�v


private:
	/* IP �A�h���X�A�|�[�g�ԍ��A�\�P�b�g */
	char* Destination;
	unsigned short Port;
	int DstSocket;

	/* sockaddr_in �\���� */
	sockaddr_in DstAddr;

	//conect����p
	bool ConnectFlag = false;

	//countdown����p
	bool CountDownFlag = false;

	//�����o�֐��@�Q�b�^�[�Z�b�^�[
	void DestinationFunc(char* InIP) { Destination = InIP; }
	char* DestinationFunc(void) { return Destination; }
	void PortFunc(unsigned short InPort) { Port = InPort; }
	unsigned short PortFunc(void) { return Port; }
	void DstSocketFunc(int InDstSocket) { DstSocket = InDstSocket; }
	int DstSocketFunc(void) { return DstSocket; }
	void DstAddrFunc(sockaddr_in InDstAddr) { DstAddr = InDstAddr; }
	sockaddr_in DstAddrFunc(void) { return DstAddr; }
	sockaddr_in* pDstAddrFunc(void) { return &DstAddr; }

};




