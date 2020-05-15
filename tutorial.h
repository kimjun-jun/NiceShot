//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("data/TEXTURE/tutorialbg.png")			
#define	TUTORIAL_SIZE_X			(SCREEN_W/4)										// �`���[�g���A���̕�
#define	TUTORIAL_SIZE_Y			(SCREEN_H/4)										// �`���[�g���A���̍���
#define	TUTORIAL_POS_X			(SCREEN_CENTER_X)									// �`���[�g���A���̕\���ʒu
#define	TUTORIAL_POS_Y			(SCREEN_CENTER_Y)									// �`���[�g���A���̕\���ʒu

#define TUTORIAL_GOUKEI			(4)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;					// �e�N�X?���ւ�?���S�� 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];		// ��?���i?��?�N
	D3DXVECTOR3				pos;								// ?���S���̈ړ���
	int						cnt;
}TUTO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

