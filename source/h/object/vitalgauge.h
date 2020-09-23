/**
* @file vitalgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class VITALGAUGE : public OBJECT_2D_VERTEXBUFFER
{
public:
	VITALGAUGE() { VitalPower = 0; };
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	HRESULT						MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							VitalPower;
};


