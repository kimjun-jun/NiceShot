/**
* @file rank.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


#include "../../h/object/objectclass.h"

class RANK : public OBJECT_2D
{
public:
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	void						SetRank(int PlayerNum);
	HRESULT						MakeVertexRank(void);
};
