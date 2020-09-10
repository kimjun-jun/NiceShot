/**
* @file title.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief タイトル初期化関数 InitTitle
* @return HRESULT
*/
HRESULT InitTitle(void);

/**
* @brief タイトル開放関数 UninitTitle
*/
void UninitTitle(void);

/**
* @brief タイトル更新関数 UpdateTitle
*/
void UpdateTitle(void);

/**
* @brief タイトル描画関数 DrawTitle
*/
void DrawTitle(void);

