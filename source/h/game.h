/**
* @file game.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../h/object/objectclass.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ReinitGame(GAME_OBJECT *GameObj);
void UpdateGame(GAME_OBJECT *GameObj);
void DrawGame(GAME_OBJECT *GameObj);
void SetScene(int Scene);
int GetScene(void);

