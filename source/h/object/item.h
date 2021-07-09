/**
* @file item.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"
class PLAYER_HONTAI;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ITEM_BIG_SCL				2.0f			//�A�C�e���T�C�Y�����l
#define ITEM_SMALL_SCL				0.03f			//�A�C�e���擾�オ�A�C�e�������񂾂񏬂����Ȃ�l
#define ITEM_DELETE_SCL				0.5f			//�A�C�e����������l

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class ITEM_PARAMETER_ALL
*�@@brief 
*/
class ITEM_PARAMETER_ALL
{
public:
	ITEM_PARAMETER_ALL() {
		nIdxShadow = -1; GetPlayerType = 0; GettingSignal = false; GettingSignalEnd = false;
		CollisionFieldEnd = false; NetUse = false; NetGetItemFlag = false; NetGetItemFlagOld = false;
		eType = ITEMTYPE_NONE;
	}
	~ITEM_PARAMETER_ALL() {}

	int		nIdxShadow;				//!< �eID
	int		GetPlayerType;			//!< �ǂ̃v���C���[���擾������
	bool	GettingSignal;			//!< �v���C���[���A�C�e�����擾�������ǂ����@�^:�擾���@�U:�擾���Ă��Ȃ�
	bool	GettingSignalEnd;		//!< �v���C���[���A�C�e�����擾�I���������ǂ����@�^:�I���@�U:�I�����Ă��Ȃ��A�������͎擾���Ă��Ȃ�
	bool	CollisionFieldEnd;		//!< �n�`�Ƃ̓����蔻�肪�I��������ǂ���

	bool	NetUse;
	bool	NetGetItemFlag;			//!< �A�C�e�����擾����1�t���[��������true�ɂ���
	bool	NetGetItemFlagOld;		//!< �A�C�e�����擾����1�t���[��������true�ɂ��� ��r�l

	eITEMTYPE		eType;			//!< ���

};

/**
*�@@class ITEM_PARAMETER_ONE
*�@@brief GAMEOBJECT�h���N���X
*/
class ITEM_PARAMETER_ONE
{
public:
	ITEM_PARAMETER_ONE() { GoukeiDrop = 0, Droptime = -1; 
	MaxSize = D3DXVECTOR3(ITEM_BIG_SCL, ITEM_BIG_SCL, ITEM_BIG_SCL); MinSize = D3DXVECTOR3(ITEM_DELETE_SCL, ITEM_DELETE_SCL, ITEM_DELETE_SCL);}
	~ITEM_PARAMETER_ONE() {}

	int	GoukeiDrop;			//!< �h���b�v���v
	int	Droptime;			//!< �o������܂ł̎���
	D3DXVECTOR3	MaxSize;	//!< �ő�T�C�Y
	D3DXVECTOR3	MinSize;	//!< �ŏ��T�C�Y
};

/**
*�@@class ITEM
*�@@brief GAMEOBJECT�h���N���X
*/
class ITEM : public GAME_OBJECT
{
public:
	ITEM();		//!< �f�[�^�ǂݍ��݁@������
	~ITEM();	//!< �폜

private:
	void	Init(void);										//!< ������
	void	Update(PLAYER_HONTAI *p,bool NetGameStartFlag);	//!< �X�V
	void	Draw(void);										//!< �`��

	//void	ReinitNet(void);								//!< �������@����

	void	SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType);
	void	ReleaseInstance(int nIdxItem);
	void	GettingItem(int nIdxItem, PLAYER_HONTAI *p);

	ModelAttribute			model;								//!< ���f�����@�}�e���A���Ⓒ�_���Ȃǁ@�����g�p����Ȃ炱����z��
	TEXTURE					tex[ITEMTYPE_MAX];					//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	VTXBuffer				vtx[OBJECT_ITEM_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm				Transform[OBJECT_ITEM_MAX];			//!< �g�����X�t�H�[�����
	iUseCheak				iUseType[OBJECT_ITEM_MAX];			//!< �g�p���
	FieldNor				PostureVec[OBJECT_ITEM_MAX];		//!< �p���x�N�g��
	ITEM_PARAMETER_ALL		ItemParaAll[OBJECT_ITEM_MAX];		//!< �e�C���X�^���X�ɕK�v�ȃf�[�^�Q
	ITEM_PARAMETER_ONE		ItemParaOne;						//!< �}�l�[�W���[�ɕK�v�ȃf�[�^�Q

	const char *c_aFileNameModel[ITEMTYPE_MAX] =
	{
		"../data/MODEL/tikeiItem.x",		// �n�`�ό`�A�C�e��
		"../data/MODEL/lifeItem.x",			// ���C�t��
		"../data/MODEL/sensyaItem.x",		// ��ԕό`�A�C�e��
		"../data/MODEL/bulletItem.x",		// �o���b�g�A�C�e��
		"../data/MODEL/speedItem.x",		// �X�s�[�h�A�b�v�A�C�e��
		"../data/MODEL/cameraItem.x",		// �����o�b�N�J�����A�C�e��
		"../data/MODEL/kiriItem.x",			// ���A�C�e��
	};

	const char *c_aFileNameTex[ITEMTYPE_MAX] =
	{
		"../data/MODEL/landmark_aogashima.png",			// �n�`�ό`�A�C�e��
		"../data/MODEL/life000.png",					// ���C�t��
		"../data/MODEL/war_sensya_noman.png",			// ��ԕό`�A�C�e��
		"../data/MODEL/bullettex.png",					// �o���b�g�A�C�e��
		"../data/MODEL/1213810.png",					// �X�s�[�h�A�b�v�A�C�e��
		"../data/MODEL/mark_camera_satsuei_ok.png",		// �����o�b�N�J�����A�C�e��
		"../data/MODEL/yama_kiri.png",					// ���A�C�e��
	};

} ;

