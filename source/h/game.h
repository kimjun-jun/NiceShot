/**
* @file game.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
class GAME_OBJECT{};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void ReinitGame(void);
void UninitGame(void);
void UpdateGame(GAME_OBJECT* GameObj);
void DrawGame(GAME_OBJECT* GameObj);
void SetScene(int Scene);
int GetScene(void);

