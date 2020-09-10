/**
* @file result.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT_OK		_T("data/TEXTURE/result/result_gameclear.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_NO		_T("data/TEXTURE/result/result_gameover.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_OK_POS_X			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_OK_POS_Y			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_OK_SIZE_X		(SCREEN_W)						// �^�C�g���̕�
#define	RESULT_OK_SIZE_Y		(SCREEN_H)						// �^�C�g���̍���
#define	RESULT_NO_POS_X			(0)						
#define	RESULT_NO_POS_Y			(0)						
#define	RESULT_NO_SIZE_X		(SCREEN_W)					
#define	RESULT_NO_SIZE_Y		(SCREEN_H)					

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

