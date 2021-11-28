/**
* @file title.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

enum eTITLE_HIERARCHY_NUM
{
	TITLE_HIERARCHY_NUM_ANIM,				//!< 0�A�j����
	TITLE_HIERARCHY_NUM_SELECT,				//!< 1�V�[���I��
	TITLE_HIERARCHY_NUM_MAX
};

/**
*�@@class TITLE_PARAMETER
*�@@brief 
*/
class TITLE_PARAMETER
{
public:
	TITLE_PARAMETER() { HierarchyNum = TITLE_HIERARCHY_NUM_ANIM; SceneNumX = OldSceneNumX = SceneNumY = OldSceneNumY = 0; }
	~TITLE_PARAMETER() {}
	int		SceneNumX;		//!< �J�[�\���f�[�^X�l
	int		OldSceneNumX;	//!< �J�[�\���f�[�^X�l
	int		SceneNumY;		//!< �J�[�\���f�[�^Y�l
	int		OldSceneNumY;	//!< �J�[�\���f�[�^Y�l
	eTITLE_HIERARCHY_NUM	HierarchyNum;	//!< �K�w�f�[�^�@�^�C�g���A�j���K�w(�N���������)�A�V�[���Z���N�g�K�w(�V�[���I�����)
};

/**
*�@@class TITLE
*�@@brief GAMEOBJECT�h���N���X
*/
class TITLE :private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	TITLE();	//!< �f�[�^�ǂݍ��݁@������
	~TITLE();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

private:
	TEXTURE	tex[OBJECT_TITLE_MAX];				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER vtx;								//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform[OBJECT_TITLE_MAX];		//!< �g�����X�t�H�[�����
	iUseCheck iUseType[OBJECT_TITLE_MAX];		//!< �g�p���

	TITLE_PARAMETER TitlePara;					//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	//------���N���X�̃A�h���X
	SCENE *pscene;

	void ChangeHierarchy(int NextHierarchyType);	//!< �K�w�؂�ւ�
	void CheckScene(void);							//!< �V�[���`�F�b�N

	enum TITLE_NAME
	{
		NAME_BG,		//!< 0�w�i
		NAME_LOGO,		//!< 1���S
		NAME_PRESS,		//!< 2PRESS
		NAME_TUTO,		//!< 3�`���[�g���A��
		NAME_LOCAL,		//!< 4���[�J��
		NAME_NET,		//!< 5�l�b�g
		NAME_QUIT,		//!< 6�I��
		NAME_MAX,		//!< �ő�
	};

	const char *c_FileNameTex[OBJECT_TITLE_MAX] =
	{
		"../data/TEXTURE/other/TItle_Background.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/other/Title.png",					// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Press.png",					// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Tutorial.png",				// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_LocalBattle.png",			// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_NetorkBattle.png",			// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Quit.png",					// �ǂݍ��ރe�N�X�`���t�@�C����
	};

};
