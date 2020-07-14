/**
* @file explosion.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//**************************************
// ���
//**************************************
enum
{
	EXPLOSIONTYPE_BULLET_PLAYER = 0,	// �v���C���[�̒e�̔���
	EXPLOSIONTYPE_BULLET_ENEMY,			// �G�̒e�̔���
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitExplosion(int type);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(int CntPlayer);

int SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType, D3DXCOLOR col);

