/**
* @file life.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitLifeTex(int type);
void UninitLifeTex(void);
void UpdateLifeTex(void);
void DrawLifeTex(void);

void ChangeLife(int value, int type);
