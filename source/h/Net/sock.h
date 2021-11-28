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

#define BUFFER_SIZE 500
#define BUFFER_SIZE_STRING 500					//!< ���M�f�[�^�o�b�t�@�T�C�Y ���M��M���ɃT�C�Y�����킹�Ȃ��ƁA�������ǂݎ��G���[�ɂȂ�@�������X�g���[���ŏ��X�ɂ���Ă���

/**
 * @enum e_VariableAnalysType
 * ��͗p�萔�@�Ή�������͏����𔻕ʂ���
 */
enum e_VariableAnalysType
{
	VariableAnalysType_Pos,					//!< 0�@���W
	VariableAnalysType_HoudaiRot,			//!< 1�@�C��rot
	VariableAnalysType_HoutouRot,			//!< 2�@�C��rot
	VariableAnalysType_HousinRot,			//!< 3�@�C�grot
	VariableAnalysType_Vital,				//!< 4�@�̗�
	VariableAnalysType_Morphing,			//!< 5�@�ό`
	VariableAnalysType_BulletA,				//!< 6�@�ʏ픭��
	VariableAnalysType_BulletB,				//!< 7�@�g�U����
	VariableAnalysType_MAX
};

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

	void MultThreadFlagFunc(bool flag) { this->MultThreadFlag = flag; }
	bool MultThreadFlagFunc(void) { return this->MultThreadFlag; }
	void GameSceneFlagFunc(bool flag) { this->GameSceneFlag = flag; }
	bool GameSceneFlagFunc(void) { return this->GameSceneFlag; }
					
	//�l�b�g�ΐ�J�n����
	void NetMatch(void);
	void NetMyNumberGet(void);
	void NetItemGet(void);
	void NetCountdown(void);

	//---------�}���`�X���b�h���Ŏ��s�@�A�v���N�����Ƀ}���`�X���b�h�ŉi�v���[�v
	void Packet(void);

	//�l�b�g�ΐ풆���s����
	void SendPacket(void);						//���M����
	void ReceivePacket(void);					//��M����



	void ObjectAnalys(char* argRMsg);			//��͏����@��ʏ����@�I�u�W�F�N�g����
	void VariableAnalys(char* argVariableMsg, int PlayerNum, char* PlayerType);		//��͏����@���ʏ����@�Ή��ϐ�����
	void XYZValue1Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3
	void XYZValue2Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3 *2
	void XYZValue4Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3 *4
	void INTValueAnalys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);			//��͏����@���ʏ����@���l�f�[�^���ʁ@int *1

	//�A�C�e�����f
	void NetSetItem(D3DXVECTOR3 buff, int index, int type);

	//�n�`���f
	void NetSetTikeiSeed(int Seed, int PlayerNum);
	
	//�v���C���[���f�Q
	void NetSetPos(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetVital(int buff, int PlayerNum);
	void NetSetMorphing(int PlayerNum);
	void NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum);
	void NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum);
	void NetSetGameEnd(void);



	//�}�b�`�t���O
	void SetNetMatchFlag(bool flag) { NetMatchFlag = flag; }
	//�}�C�i���o�[
	void SetNetMyNumberFlag(bool flag) { NetMyNumberFlag = flag; }
	void SetNetMyNumber(int num) { NetMyNumber = num; }
	int GetNetMyNumber(void) { return NetMyNumber; }
	//�A�C�e���t���O�@true�ŃA�C�e�����擾�����A�J�E���g�_�E���M�����󂯎��n�߂�
	void SetNetItemFlag(bool flag) { NetItemFlag = flag; }
	bool GetNetItemFlag(void) { return NetItemFlag; }
	//�l�b�g�t���O�@true�Ńl�b�g�ΐ�n�߂�
	void SetNetGameStartFlag(bool flag) { NetGameStartFlag = flag; }
	bool GetNetGameStartFlag(void) { return NetGameStartFlag; }
	//�l�b�g�ΐ�@���L�f�[�^�A�N�Z�X���t���O
	void SetNetShareDateFlag(bool flag) { NetShareDateFlag = flag; }
	bool GetNetShareDateFlag(void) { return NetShareDateFlag; }

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
	//�}���`�X���b�h����p
	bool MultThreadFlag = false;
	//�l�b�g�ΐ풆���[�v�@true�Ńl�b�g�ΐ풆�@false�őΐ킵�Ă��Ȃ�  ����œ������邩���肵�Ă���
	bool GameSceneFlag = false;

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


	//�}�b�`�t���O
	bool NetMatchFlag = false;
	//�}�C�i���o�[
	bool NetMyNumberFlag = false;
	int NetMyNumber = -1;
	//�A�C�e���t���O�@true�ŃA�C�e�����擾�����A�J�E���g�_�E���M�����󂯎��n�߂�
	bool NetItemFlag = false;
	//�l�b�g�t���O�@true�Ńl�b�g�ΐ�n�߂�
	bool NetGameStartFlag = false;
	//�l�b�g�ΐ�@���L�f�[�^�A�N�Z�X���t���O
	bool NetShareDateFlag = false;

	//------���N���X�̃A�h���X
	PLAYER *pplayer;
	ITEM *pitem;
	BULLET *pbullet;
	FIELD *pfield;
	SCENE *pscene;
	GAME_OBJECT_INSTANCE *pGameObjInstance; //�e�N���X�̃A�h���X�Ȃ̂ł���܂�ǂ��Ȃ�

};




