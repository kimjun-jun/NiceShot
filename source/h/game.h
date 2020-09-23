/**
* @file game.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
class GAME_OBJECT{};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGame(void);
void ReinitGame(void);
void UninitGame(void);
void UpdateGame(GAME_OBJECT* GameObj);
void DrawGame(GAME_OBJECT* GameObj);
void SetScene(int Scene);
int GetScene(void);

