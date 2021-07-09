/**
* @file vitalgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

enum VITAL_TEX_TYPE
{
	VITAL_TEX_BASE,
	VITAL_TEX_GREEN,
	VITAL_TEX_ORANGE,
	VITAL_TEX_RED,
	VITAL_TEX_MAX,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class VITALGAUGE
*�@@brief GAMEOBJECT�h���N���X
*/
class VITALGAUGE_PARAMETER
{
public:
	VITALGAUGE_PARAMETER() { VitalPower = PLAYER_VITAL_MAX; }
	~VITALGAUGE_PARAMETER() {}

	int			VitalPower;
};

/**
*�@@class VITALGAUGE
*�@@brief GAMEOBJECT�h���N���X
*/
class VITALGAUGE : public GAME_OBJECT
{
public:
	VITALGAUGE();	//!< �f�[�^�ǂݍ��݁@������
	~VITALGAUGE();	//!< �폜

private:
	void		Init(void);								//!< ������
	void		Update(PLAYER_HONTAI *p, RANK *rank, bool Netflag, int NetMyNumber);	//!< �X�V
	void		Draw(bool Netflag, int NetMyNumber, int CntPlayer);						//!< �`��

	void		ReinitNet(int MyNumber);		//!< �ď������l�b�g�ΐ�O	

	TEXTURE			tex[VITAL_TEX_MAX];				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@0:�g�@1,2,3:���g(�J���[�o���G�[�V����)
	VTXBuffer		vtx[OBJECT_VITAL_MAX];			//!< ���_���@�����g�p����Ȃ炱����z�� 
	TransForm		Transform[OBJECT_VITAL_MAX];	//!< �g�����X�t�H�[�����
	VITALGAUGE_PARAMETER	VitalGaugePara[OBJECT_VITAL_MAX];//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	const char *c_aFileNameTex[VITAL_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_HP_Base.png",	
		"../data/TEXTURE/UI/UI_HP_Green.png",	
		"../data/TEXTURE/UI/UI_HP_Orange.png",	
		"../data/TEXTURE/UI/UI_HP_Red.png",		
	};

};

