/**
* @file vitalgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class VITALGAUGE : public OBJECT_2D_VERTEXBUFFER
{
public:
	VITALGAUGE() { VitalPower = 0; };
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(PLAYER_HONTAI *p, RANK *rank);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							VitalPower;
};


