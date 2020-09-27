/**
* @file tutorial.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*�@@class TUTO
*�@@brief 2D�|���S�����`����\����
*/
class TUTO : public OBJECT_2D
{
public:
	TUTO() { cnt = 0; }
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(GAME_OBJECT* obj, FADE *fade);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexTutorial(void);
	int							cnt;						//!< �`�攻��p�@2�Ŋ������]�蔻��@0�\���@1��\���@����̃e�N�X�`����񂪑����邱�Ƃ���������int��`
};

