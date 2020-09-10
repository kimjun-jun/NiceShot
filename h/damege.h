/**
* @file damege.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_DAMEGE			("data/TEXTURE/screendamage.png")			
#define	DAMEGE_SIZE_X			(SCREEN_W/4)										// �`���[�g���A���̕�
#define	DAMEGE_SIZE_Y			(SCREEN_H/4)										// �`���[�g���A���̍���
#define	DAMEGE_POS_X			(SCREEN_CENTER_X)									// �`���[�g���A���̕\���ʒu
#define	DAMEGE_POS_Y			(SCREEN_CENTER_Y)									// �`���[�g���A���̕\���ʒu

#define DAMEGE_GOUKEI			(4)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;					// �e�N�X?���ւ�?���S�� 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];		// ��?���i?��?�N
	D3DXVECTOR3				pos;								// ?���S���̈ړ���
	int						alpha;
	float					time;
	bool					use;
}DAMEGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDamege(int type);
void UninitDamege(void);
void UpdateDamege(void);
void DrawDamege(void);
DAMEGE *GetDamege(void);

