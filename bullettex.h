//=============================================================================
//
// バレット残弾テクスチャ処理 [bullettex.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletTex(int type);
void UninitBulletTex(void);
void UpdateBulletTex(void);
void DrawBulletTex(void);

void ChangeBulletTex(int value, int type);

