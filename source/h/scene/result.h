/**
* @file result.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*�@@class RESULT
*�@@brief 2D�|���S�����`����N���X
*/
class RESULT : public OBJECT_2D
{
public:
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(GAME_OBJECT* obj, FADE *fade);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexResult(void);
};
