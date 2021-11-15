/**
* @file sound.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum SOUND_TRACK//�����̏��Ԃ�.CPP��SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =���Ԃ���v�����邱�ƁB�����̖��O���g���ĉ���炷
{
	SOUND_LABEL_BGM_title01,			// �^�C�g��BGM
	SOUND_LABEL_BGM_tutorial01,			// �`���[�g���A��BGM
	SOUND_LABEL_BGM_select01,			// �Z���N�gBGM
	SOUND_LABEL_BGM_normal01,			// �Q�[���ʏ펞BGM
	SOUND_LABEL_BGM_boss01,				// �Q�[���{�X��BGM
	SOUND_LABEL_BGM_gameclear01,		// �Q�[���N���ABGM
	SOUND_LABEL_BGM_gameover01,			// �Q�[���I�[�o�[BGM
	SOUND_LABEL_SE_attack01,			// �{�[����ł�SE �J�[��
	SOUND_LABEL_SE_attack02,			// �{�[����ł�SE �|�[��
	SOUND_LABEL_SE_attack03,			// �{�[����ł�SE �|�b
	SOUND_LABEL_SE_damage,				// �ŋ���������SE
	SOUND_LABEL_SE_enter01,				// �G���^�[������SE ������[��
	SOUND_LABEL_SE_enter02,				// �G���^�[������SE ������
	SOUND_LABEL_SE_enter03,				// �G���^�[������SE �����
	SOUND_LABEL_SE_strat,				// �J�nSE
	SOUND_LABEL_SE_swing,				// �X�C���OSE
	SOUND_LABEL_SE_nyu,					// �ɂ�
	SOUND_LABEL_SE_rap1,				// ���b�p1
	SOUND_LABEL_SE_rap2,				// ���b�p2
	SOUND_LABEL_SE_speed,				// �X�s�[�h�A�b�v
	SOUND_LABEL_SE_kiri,				// ��
	SOUND_LABEL_SE_quake,				// �n�k
	SOUND_LABEL_SE_kurukuru,			// ���邭��
	SOUND_LABEL_SE_parin,				// �p���[��
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);
void MasterVolumeChange(float num);
void SourceVolumeChange(float num,int sno);
