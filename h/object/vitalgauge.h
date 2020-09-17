/**
* @file vitalgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitVitalGauge(void);
HRESULT ReInitVitalGauge(void);
void UninitVitalGauge(void);
void UpdateVitalGauge(void);
void DrawVitalGauge(void);

