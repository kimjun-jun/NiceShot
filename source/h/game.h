/**
* @file game.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../h/object/objectclass.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ReinitGame(GAME_OBJECT *GameObj);
void UpdateGame(GAME_OBJECT *GameObj);
void DrawGame(GAME_OBJECT *GameObj);
void SetScene(int Scene);
int GetScene(void);

