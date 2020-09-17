/**
* @file bulletgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletGauge(void);
HRESULT ReInitBulletGauge(void);
void UninitBulletGauge(void);
void UpdateBulletGauge(void);
void DrawBulletGauge(void);

