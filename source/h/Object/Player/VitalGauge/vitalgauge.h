/**
* @file vitalgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

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
class VITALGAUGE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	VITALGAUGE();	//!< �f�[�^�ǂݍ��݁@������
	~VITALGAUGE();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override;			//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

private:
	TEXTURE	tex[VITAL_TEX_MAX];				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@0:�g�@1,2,3:���g(�J���[�o���G�[�V����)
	VTXBUFFER vtx[OBJECT_VITAL_MAX];		//!< ���_���@�����g�p����Ȃ炱����z�� 
	TRANSFORM Transform[OBJECT_VITAL_MAX];	//!< �g�����X�t�H�[�����
	VITALGAUGE_PARAMETER VitalGaugePara[OBJECT_VITAL_MAX];//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	//------���N���X�̃A�h���X
	PLAYER *pplayer;
	MySOCKET *pmysocket;
	RANK *prank;
	//------�`�惋�[�v�J�E���g�̃A�h���X
	DRAW_MANAGER *pDrawManager;

	const char *c_aFileNameTex[VITAL_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_HP_Base.png",	
		"../data/TEXTURE/UI/UI_HP_Green.png",	
		"../data/TEXTURE/UI/UI_HP_Orange.png",	
		"../data/TEXTURE/UI/UI_HP_Red.png",		
	};

};

