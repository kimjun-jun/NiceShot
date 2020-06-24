//=============================================================================
//
// �J�E���g�_�E������ [countdown.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#pragma once


// �}�N����`
#define TEXTURE_GAME_COUNTDOWNGO	_T("data/TEXTURE/sozai/go.png")	
#define TEXTURE_COUNTDOWN_SIZE_X	(50) // �e�N�X�`���T�C�Y 
#define TEXTURE_COUNTDOWN_SIZE_Y	(50) // ����  
#define TEXTURE_COUNTDOWNGO_SIZE_X	(200) // �e�N�X�`���T�C�Y 
#define TEXTURE_COUNTDOWNGO_SIZE_Y	(200) // ����  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X	(10)	// �A�j���p�^�[���̃e�N�X�`�����������iX)  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)  
#define COUNTDOWN_ANIM_PATTERN_NUM			(COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X*COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����  
#define COUNTDOWN_TIME_ANIMATION			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g  
#define TEXTURE_COUNTDOWN_POS_X	(SCREEN_CENTER_X) // �e�N�X�`���T�C�Y
#define TEXTURE_COUNTDOWN_POS_Y	(SCREEN_CENTER_Y) // ����  
#define COUNTDOWN_GOUKEI				(2) //�X�R�A�̍��v
#define FPS_TIME_COUNTDOWN				(240) //���t���[���J�E���g���ă^�C���v��
#define COUNTDOWN_A						(10) //�J�E���g�_�E����0�̎�GO��`�悵��GO�������Ȃ�X�s�[�h
#define COUNTDOWN_CHANGESIZE			(5) //�J�E���g�_�E����0�̎�GO��`�悵��GO�������Ȃ�X�s�[�h

//*****************************************************************************
//* �\���̒�`
//******************************************************************************* /
typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;
	VERTEX_2D				texture[POLYGON_2D_VERTEX];
	D3DXVECTOR3				pos;
	int						nCountAnim;
	int						nPatternAnim;
	int						changeval;
	int						color;
	bool					use;
	bool					signal;
} COUNTDOWN;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCountdown(int type);
void ReInitCountdown(void);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
COUNTDOWN *GetCountdown(void);
void AddCountdown(int val);

