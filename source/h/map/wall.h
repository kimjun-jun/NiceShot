/**
* @file wall.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"


/**
*�@@class WALL
*�@@brief GAMEOBJECT�h���N���X
*/
class WALL : public GAME_OBJECT
{
public:
	WALL();		//!< �f�[�^�ǂݍ���
	~WALL();	//!< �폜
	void		Init(void);		//!< ������
	void		Update(void);	//!< �X�V
	void		Draw(void);		//!< �`��

private:
} ;
