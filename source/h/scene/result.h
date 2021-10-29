/**
* @file result.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../h/object/objectclass.h"

enum eRESULT_TEX_TYPE
{
	RESULT_TEX_BG,
	RESULT_TEX_LOGO,
	RESULT_TEX_MAX,
};

/**
*�@@class RESULT
*�@@brief GAMEOBJECT�h���N���X
*/
class RESULT : public GAME_OBJECT
{
public:
	RESULT();		//!< �f�[�^�ǂݍ��݁@������
	~RESULT();		//!< �폜
	void		Init(void);					//!< ������
	void		Update(GAME_OBJECT* obj, FADE *fade);	//!< �X�V
	void		Draw(void);					//!< �`��


private:
	TEXTURE		tex[RESULT_TEX_MAX];		//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;						//!< ���_���@�����g�p����Ȃ炱����z��
	//TransForm	Transform[OBJECT_RESULT_MAX];		//!< �g�����X�t�H�[�����

	const char *c_aFileNameTex[OBJECT_RESULT_MAX] =
	{
		"../data/TEXTURE/other/GameClear_Background.png",	//!< ���U���g�w�i
		"../data/TEXTURE/other/GameClear.png"				//!< ���U���g���S
	};

};
