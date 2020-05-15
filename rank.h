//=============================================================================
//
// �����N���� [rank.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RANK2		("data/TEXTURE/2i.png")			
#define	TEXTURE_RANK3		("data/TEXTURE/3i.png")			
#define	TEXTURE_RANK4		("data/TEXTURE/4i.png")		
#define	RANK_SIZE_X			(SCREEN_W/4)										// �`���[�g���A���̕�
#define	RANK_SIZE_Y			(SCREEN_H/4)										// �`���[�g���A���̍���
#define	RANK_POS_X			(SCREEN_W)										// �`���[�g���A���̕\���ʒu
#define	RANK_POS_Y			(SCREEN_H)										// �`���[�g���A���̕\���ʒu

#define RANK_GOUKEI			(3)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;					// �e�N�X?���ւ�?���S�� 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];		// ��?���i?��?�N
	D3DXVECTOR3				pos;								// ?���S���̈ړ���
	bool					use;
}RANK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRank(int type);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);
RANK *GetRank(void);
void SetRank(int PlayerNum);

