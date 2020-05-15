//=============================================================================
//
// �o���b�g���n�_�J�[�\������ [bulletpoint.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 scl;		// �X�P�[��
	float fWidth;			// ��
	float fHeight;			// ����
	int nIdxShadow;			// �eID
	bool bUse;				// �g�p���Ă��邩�ǂ���
} BULLETPOINT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletPoint(void);
void UninitBulletPoint(void);
void UpdateBulletPoint(void);
void DrawBulletPoint(int type);
void SetBulletPoint(D3DXVECTOR3 pos, float fWidth, float fHeight, int type);

BULLETPOINT *GetBulletPoint(void);

