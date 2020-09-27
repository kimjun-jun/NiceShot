/**
* @file fade.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


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

#include "../../h/object/objectclass.h"


/**
*�@@struct FADE
*�@@brief 2D�|���S�����`����\����
*/
class FADE : public OBJECT_2D
{
public:
	FADE() { color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); eFade = FADE_NONE; eScene = 0; sno = 0; }
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(GAME_OBJECT*obj);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	void								SetFade(FADE_TYPE fade, E_STAGE next, int sno);
	void								SetColor(D3DCOLOR col);

	D3DXCOLOR				color;								//!< �J���[���
	FADE_TYPE				eFade = FADE_IN;						//!< �t�F�[�h�ԍ�
	int						eScene = SCENE_TITLE;					//!< ���ɔ�ԗ\���Scene
	int						sno = -1;								//!< �T�E���h�i���o�[

private:
	void MakeVertexFade(void);
};



