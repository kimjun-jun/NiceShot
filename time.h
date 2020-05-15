//=============================================================================
//
// �^�C���̏��� [time.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#pragma once


// �}�N����`
#define TEXTURE_GAME_DOT	"data/TEXTURE/BG/dot.png"	
#define TEXTURE_GAME_SCORE	"data/TEXTURE/BG/0-9.png"
#define TEXTURE_GAME_TIMELOGO	"data/TEXTURE/sozai/TIME.png" 
#define TEXTURE_TIME_SIZE_X	(30) // �e�N�X�`���T�C�Y 
#define TEXTURE_TIME_SIZE_Y	(30) // ����  
#define SCORE_TEXTURE_PATTERN_DIVIDE_X	(10)	// �A�j���p�^�[���̃e�N�X�`�����������iX)  
#define SCORE_TEXTURE_PATTERN_DIVIDE_Y	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)  
#define SCORE_ANIM_PATTERN_NUM			(SCORE_TEXTURE_PATTERN_DIVIDE_X*SCORE_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����  
#define SCORE_TIME_ANIMATION			(4)	// �A�j���[�V�����̐؂�ւ��J�E���g  
#define TEXTURE_DOT_SIZE_X	(4) // �e�N�X�`���T�C�Y 
#define TEXTURE_DOT_SIZE_Y	(4) // ����  


#define DOT_GOUKEI					(2)
#define TIME_DIGIT					(3)
#define FPS_TIME_COUNT				(4200) //���t���[���J�E���g���ă^�C���v��


#define TEXTURE_TIMELOGO_SIZE_X	(35) // �e�N�X�`���T�C�Y 
#define TEXTURE_TIMELOGO_SIZE_Y	(30) // ����  

#define TEXTURE_TIME_POS_X	(SCREEN_W-30) // �e�N�X�`���T�C�Y 
#define TEXTURE_TIME_POS_Y	(25) // ����  


#define TEXTURE_TIMELOGO_POS_X	(TEXTURE_TIME_POS_X-110) // �e�N�X�`���T�C�Y 
#define TEXTURE_TIMELOGO_POS_Y	(TEXTURE_TIME_POS_Y) // ����  

#define NUM_INTERVAL_X	(float(TEXTURE_TIME_SIZE_X)) // ����  

//*****************************************************************************
//* �\���̒�`
//******************************************************************************* /
typedef struct // �\����
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;

	VERTEX_2D				texture[POLYGON_2D_VERTEX];

	D3DXVECTOR3				pos;
	D3DXVECTOR3				rot;
	int						nCountAnim;
	int						nPatternAnim;
	bool					use;
} TIME;

typedef struct // �\����
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;

	VERTEX_2D				texture[POLYGON_2D_VERTEX];

	D3DXVECTOR3				pos;
	int						nCountAnim;
	int						nPatternAnim;
} DOT;

typedef struct // �\����
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;
	VERTEX_2D				texture[POLYGON_2D_VERTEX];
	D3DXVECTOR3				pos;
} TIMELOGO;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTime(int type);
void ReInitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
int *GetTimemaneger(void);
void AddTime(int val);

