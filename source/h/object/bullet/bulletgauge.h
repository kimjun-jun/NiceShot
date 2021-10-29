/**
* @file bulletgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#include "../../../h/object/objectclass.h"

enum eBULLETGAUGE_TEX_TYPE
{
	BULLETGAUGE_TEX_BASE,
	BULLETGAUGE_TEX_SHELL,
	BULLETGAUGE_TEX_MAX,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class BULLETGAUGE_PARAMETER
*�@@brief 
*/
class BULLETGAUGE_PARAMETER
{
public:
	BULLETGAUGE_PARAMETER() { AmmoStock = 0; }
	~BULLETGAUGE_PARAMETER() { }
	int	AmmoStock;
};

/**
*�@@class BULLETGAUGE
*�@@brief GAMEOBJECT�h���N���X
*/
class BULLETGAUGE : public GAME_OBJECT
{
public:
	BULLETGAUGE();	//!< �f�[�^�ǂݍ��݁@������
	~BULLETGAUGE();	//!< �폜

	void		Init(void);						//!< ������
	void		InitNet(int MyNumber);	//!< ������
	void		Update(PLAYER *player);	//!< �X�V
	void		Draw(bool Netflag, int NetMyNumber, int CntPlayer);		//!< �`��

private:

	TEXTURE			tex[BULLETGAUGE_TEX_MAX];					//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@0:�g�@1:���g
	VTXBuffer		vtx[OBJECT_BULLETGAUGE_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm		Transform[OBJECT_BULLETGAUGE_MAX];			//!< �g�����X�t�H�[�����
	BULLETGAUGE_PARAMETER	BulletGaugePara[OBJECT_BULLETGAUGE_MAX];//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	const char *c_aFileNameTex[BULLETGAUGE_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_Shell.png" ,
		"../data/TEXTURE/UI/UI_Shell_Empty.png",
	};
};
