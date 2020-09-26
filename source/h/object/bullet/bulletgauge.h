/**
* @file bulletgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../../h/object/objectclass.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class BULLETGAUGE : public OBJECT_2D_VERTEXBUFFER 
{
public:
	BULLETGAUGE() { AmmoPower = 0; };
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(void);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							AmmoPower;
};
