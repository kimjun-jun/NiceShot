/**
* @file fade.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#include "../../../h/Object/ObjectClass/objectclass.h"

/**
 * @enum FADE
 * �t�F�[�h�̏��
 */
enum FADE_TYPE
{
	FADE_NONE = 0,		//!< �����Ȃ����
	FADE_IN,			//!< �t�F�[�h�C������
	FADE_MUSIC_STOP,	//!< ���y�X�g�b�v
	FADE_OUT,			//!< �t�F�[�h�A�E�g����
	FADE_MAX			//!< �}�b�N�X
};

/**
*�@@class FADE_PARAMETER
*�@@brief 
*/
class FADE_PARAMETER
{
public:
	FADE_PARAMETER() { color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); eFade = FADE_IN; eScene = SCENE_TITLE; sno = 0; }
	~FADE_PARAMETER() { }

	D3DXCOLOR	color;							//!< �J���[���
	FADE_TYPE	eFade;							//!< �t�F�[�h�ԍ�
	int			sno;							//!< �T�E���h�i���o�[
	E_STAGE		eScene;							//!< ���ɔ�ԗ\���Scene
};

/**
*�@@class FADE
*�@@brief GAMEOBJECT�h���N���X�@�V�[���؂�ւ����̃t�F�[�h���ʁ@�������^�C����ɃV�[����؂�ւ���
*/
class FADE : public GAME_OBJECT
{
public:
	FADE();		//!< �f�[�^�ǂݍ��݁@������
	~FADE();	//!< �폜

	void		Init(void);						//!< ������
	void		Update(GAME_OBJECT*obj);		//!< �X�V
	void		Draw(void);						//!< �`��

	void		SetFade(FADE_TYPE fade, E_STAGE next, int sno);		//�t�F�[�h���ʃZ�b�g

	FADE_PARAMETER	FadePara;					//!< �}�l�[�W���[�ɕK�v�ȃf�[�^�Q

private:

	VTXBuffer	vtx;							//!< ���_���@�����g�p����Ȃ炱����z��
};
