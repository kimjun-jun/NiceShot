/**
* @file game.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGame(void);
void ReinitGame(void);
void UninitGame(void);
void UpdateGame(GAME_OBJECT* GameObj);
void DrawGame(GAME_OBJECT* GameObj);


/**
* @brief �Q�[���V�[�����Z�b�g����֐� SetScene
* @param[in] Scene �ړ���V�[�������
* @details �Q�[���V�[����E_STAGE���Q��
*/
void SetScene(int Scene);

/**
* @brief ���݂̃Q�[���V�[������肷��֐� GetScene
* @return int ���݂̃Q�[���V�[����Ԃ�
* @details �Q�[���V�[����E_STAGE���Q��
*/
int GetScene(void);

