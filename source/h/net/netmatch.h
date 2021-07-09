/**
* @file netmatch.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*�@@class NETMATCH
*�@@brief GAMEOBJECT�h���N���X
*/
class NETMATCH : public GAME_OBJECT
{
public:
	NETMATCH();		//!< �f�[�^�ǂݍ���
	~NETMATCH();	//!< �폜
	void		Init(void);								//!< ������
	void		Update(GAME_OBJECT* obj, FADE *fade);	//!< �X�V
	void		Draw(void);						//!< �`��

private:
	void		MakeVertex(void);
};

