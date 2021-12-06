/**
* @file sock.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

class SOCK_UPDATE;
class SOCK_SUBTHREAD_RECEIVE;
class SOCK_SUBTHREAD_SEND;

#define BUFFER_SIZE 500
#define BUFFER_SIZE_STRING 500					//!< ���M�f�[�^�o�b�t�@�T�C�Y ���M��M���ɃT�C�Y�����킹�Ȃ��ƁA�������ǂݎ��G���[�ɂȂ�@�������X�g���[���ŏ��X�ɂ���Ă���

/**
*�@@class MySOCKET
*�@@brief GAMEOBJECT�h���N���X
*/
class MySOCKET : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	MySOCKET();		//!< �\�P�b�g�ݒ�
	~MySOCKET();	//!< �\�P�b�g�폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override {};			//!< �`��

	//-----------�d�v�x������
	void Packet(void);	//�}���`�X���b�h���Ŏ��s�@�A�v���N�����Ƀ}���`�X���b�h�ŉi�v���[�v
	int GetNetMyNumber(void) { return NetMyNumber; }	//�}�C�i���o�[�擾
	void SetNetMyNumber(int num) { NetMyNumber = num; }
	//--------------�����o�@�Q�b�^�[�Z�b�^�[
	//�}���`�X���b�h�t���O�@�A�v�����[�v�t���O
	void MultThreadFlagFunc(bool flag) { this->MultThreadFlag = flag; }
	bool MultThreadFlagFunc(void) { return this->MultThreadFlag; }
	void GameSceneFlagFunc(bool flag) { this->GameSceneFlag = flag; }
	bool GameSceneFlagFunc(void) { return this->GameSceneFlag; }
	//�l�b�g�ΐ�@���L�f�[�^�A�N�Z�X���t���O
	void SetNetShareDateFlag(bool flag) { NetShareDateFlag = flag; }
	bool GetNetShareDateFlag(void) { return NetShareDateFlag; }

	//-----------�d�v�x����
	//--------------�����o�@�Q�b�^�[�Z�b�^�[
	//�\�P�b�g���
	void DestinationFunc(char* InIP) { Destination = InIP; }
	char* DestinationFunc(void) { return Destination; }
	void PortFunc(unsigned short InPort) { Port = InPort; }
	unsigned short PortFunc(void) { return Port; }
	void DstSocketFunc(int InDstSocket) { DstSocket = InDstSocket; }
	int DstSocketFunc(void) { return DstSocket; }
	void DstAddrFunc(sockaddr_in InDstAddr) { DstAddr = InDstAddr; }
	sockaddr_in DstAddrFunc(void) { return DstAddr; }
	sockaddr_in* pDstAddrFunc(void) { return &DstAddr; }					
	//�R�l�N�g����p�t���O�@tcp coccect�������K�v
	void SetNetConnectFlag(bool flag) { NetConnectFlag = flag; }
	bool GetNetConnectFlag(void) { return NetConnectFlag; }
	//�}�b�`�t���O
	void SetNetMatchFlag(bool flag) { NetMatchFlag = flag; }
	bool GetNetMatchFlag(void) { return NetMatchFlag; }
	//�}�C�i���o�[
	void SetNetMyNumberFlag(bool flag) { NetMyNumberFlag = flag; }
	bool GetNetMyNumberFlag(void) { return NetMyNumberFlag; }
	//�A�C�e���t���O�@true�ŃA�C�e�����擾�����A�J�E���g�_�E���M�����󂯎��n�߂�
	void SetNetItemFlag(bool flag) { NetItemFlag = flag; }
	bool GetNetItemFlag(void) { return NetItemFlag; }
	//�J�E���g�_�E���t���O�@true�Ńl�b�g�ΐ�J�E���g�_�E���n�߂�
	void SetNetCountDownFlag(bool flag) { NetCountDownFlag = flag; }
	bool GetNetCountDownFlag(void) { return NetCountDownFlag; }
	//�ΐ�J�n�t���O�@true�Ńl�b�g�ΐ�J�n
	void SetNetGameStartFlag(bool flag) { NetGameStartFlag = flag; }
	bool GetNetGameStartFlag(void) { return NetGameStartFlag; }


	//--------------�����o�@�����I�u�W�F�N�g�̃Z�b�^�[	
	void NetSetItem(D3DXVECTOR3 buff, int index, int type);		//�A�C�e�����f
	void NetSetTikeiSeed(int Seed, int PlayerNum);				//�n�`���f	
	void NetSetPos(D3DXVECTOR3 buff, int PlayerNum);			//�v���C���[���f�Q
	void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetVital(int buff, int PlayerNum);
	void NetSetMorphing(int PlayerNum);
	void NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum);
	void NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum);
	void NetSetGameEnd(void);	//�l�b�g�ΐ�I���t���O���f

	//-----------�d�v�x�������������������������@�D���ɂȂ�Ȃ�
	//------�}�l�[�W���ւ̃|�C���^�[
	SOCK_UPDATE	*pSockUpdate = NULL;
	SOCK_SUBTHREAD_SEND *pSockSubThreadSend = NULL;
	SOCK_SUBTHREAD_RECEIVE *pSockSubThreadReceive = NULL;
	//------���N���X�̃A�h���X
	PLAYER *pplayer;
	ITEM *pitem;
	BULLET *pbullet;
	FIELD *pfield;
	SCENE *pscene;
	GAME_OBJECT_INSTANCE *pGameObjInstance; //�e�N���X�̃A�h���X�Ȃ̂ł���܂�ǂ��Ȃ�

private:
	/* IP �A�h���X�A�|�[�g�ԍ��A�\�P�b�g */
	char* Destination;
	unsigned short Port;
	int DstSocket;
	/* sockaddr_in �\���� */
	sockaddr_in DstAddr;

	//-----------�d�v�x������
	bool MultThreadFlag = false;	//�}���`�X���b�h����p�@�I���Ńl�b�g�ΐ� �I�t�Ńl�b�g�ΐ킵�Ă��Ȃ�
	bool GameSceneFlag = false;		//�l�b�g�ΐ풆���[�v�@�I���ŃA�v�����s���@�I�t�ŃA�v���I��
	bool NetShareDateFlag = false;	//�l�b�g�ΐ�@���L�f�[�^�A�N�Z�X���t���O
	int NetMyNumber = -1;			//�l�b�g�ΐ펞�̃}�C�v���C���[�i���o�[	

	//-----------�d�v�x��
	bool NetConnectFlag = false;		//conect����p
	bool NetMatchFlag = false;		//�}�b�`�t���O
	bool NetMyNumberFlag = false;	//�}�C�i���o�[
	bool NetItemFlag = false;		//�A�C�e���t���O�@true�ŃA�C�e�����擾�����A�J�E���g�_�E���M�����󂯎��n�߂�
	bool NetCountDownFlag = false;	//countdown����p true�Ńl�b�g�J�E���g�_�E���J�n
	bool NetGameStartFlag = false;	//�l�b�g�t���O�@true�Ńl�b�g�ΐ�J�n

};




