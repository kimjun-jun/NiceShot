/**
* @file fade.h
* @brief HitBallTarget(2D)�I���ăQ�[��
* @author �L�����W����
* @date 2019/09/01
*/
#pragma once


/**
 * @enum FADE
 * �t�F�[�h�̏��
 */
enum FADE
{
	FADE_NONE = 0,		//!< �����Ȃ����
	FADE_IN,			//!< �t�F�[�h�C������
	FADE_MUSIC_STOP,	//!< ���y�X�g�b�v
	FADE_OUT,			//!< �t�F�[�h�A�E�g����
	FADE_MAX			//!< �}�b�N�X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

/**
* @brief �t�F�[�h�������֐� InitFade
* @return HRESULT
*/
HRESULT InitFade(void);

/**
* @brief �t�F�[�h�������J���֐� UninitFade
*/
void UninitFade(void);

/**
* @brief �t�F�[�h�X�V�֐� UpdateFade
* @details �Q�[���V�[���؂�ւ���(SetFade()���g�p����Ƃ�)�Ɏ��s�����B
*/
void UpdateFade(void);

/**
* @brief �t�F�[�h�`��֐� DrawFade
*/
void DrawFade(void);

/**
* @brief �t�F�[�h�ݒ�֐� SetFade
* @param[in] FADE fade �t�F�[�h�̎��, E_STAGE next ���̃Q�[���V�[��, int sno �����������y
*/
void SetFade(FADE fade, E_STAGE next, int sno);

/**
* @brief �t�F�[�h�擾�֐� GetFade
* @return FADE
* @details ���݂̃t�F�[�h��Ԃ��擾�ł���
*/
FADE GetFade(void);

