/**
* @file bullettex.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletTex(int type);
void UninitBulletTex(void);
void UpdateBulletTex(void);
void DrawBulletTex(void);

void ChangeBulletTex(int value, int type);

