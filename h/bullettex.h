/**
* @file bullettex.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletTex(int type);
void UninitBulletTex(void);
void UpdateBulletTex(void);
void DrawBulletTex(void);

void ChangeBulletTex(int value, int type);

