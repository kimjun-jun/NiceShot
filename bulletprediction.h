/**
* @file bulletprediction.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletprediction(int type);
void UninitBulletprediction(void);
void UpdateBulletprediction(void);
void DrawBulletprediction(void);

int SetBulletprediction(D3DXVECTOR3 pos, D3DXCOLOR col, float fSizeX, float fSizeY);
