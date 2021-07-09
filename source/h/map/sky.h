/**
* @file sky.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*�@@class SKY_PARAMETER
*�@@brief SKY_PARAMETER
*/
class SKY_PARAMETER
{
	SKY_PARAMETER();
	~SKY_PARAMETER();

	float time;					// ��̎��ԁ@���Ԃɂ���ċ�̐F���ς��
	float Addtime;				// ��̎��ԁ@���Ԃɂ���ċ�̐F���ς��
	float fRotY;				// ��]��
	int nNumBlockH;
	int nNumBlockV;
};

/**
*�@@class SKY
*�@@brief GAMEOBJECT�h���N���X
*/
class SKY : public GAME_OBJECT
{
public:
	SKY();		//!< �f�[�^�ǂݍ���
	~SKY();		//!< �폜
	void		Init(void);		//!< ������
	void		Update(void);	//!< �X�V
	void		Draw(void);		//!< �`��

private:
	SKY_PARAMETER SkyPara;
};
