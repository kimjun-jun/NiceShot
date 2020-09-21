/**
* @file game.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void ReinitGame(void);
void UninitGame(void);
void UpdateGame(GAME_OBJECT* GameObj);
void DrawGame(GAME_OBJECT* GameObj);


/**
* @brief ゲームシーンをセットする関数 SetScene
* @param[in] Scene 移動先シーンを入力
* @details ゲームシーンはE_STAGEを参照
*/
void SetScene(int Scene);

/**
* @brief 現在のゲームシーンを入手する関数 GetScene
* @return int 現在のゲームシーンを返す
* @details ゲームシーンはE_STAGEを参照
*/
int GetScene(void);

