/**
* @file bulletgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

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
class BULLETGAUGE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	BULLETGAUGE();	//!< �f�[�^�ǂݍ��݁@������
	~BULLETGAUGE();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override;			//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

private:

	TEXTURE	tex[BULLETGAUGE_TEX_MAX];					//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@0:�g�@1:���g
	VTXBUFFER vtx[OBJECT_BULLETGAUGE_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform[OBJECT_BULLETGAUGE_MAX];		//!< �g�����X�t�H�[�����
	BULLETGAUGE_PARAMETER BulletGaugePara[OBJECT_BULLETGAUGE_MAX];//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	//------���N���X�̃A�h���X
	MySOCKET *pmysocket;
	PLAYER *pplayer;
	//------�`�惋�[�v�J�E���g�̃A�h���X
	DRAW_MANAGER *pDrawManager;


	const char *c_aFileNameTex[BULLETGAUGE_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_Shell.png" ,
		"../data/TEXTURE/UI/UI_Shell_Empty.png",
	};
};
