/**
* @file bulletgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class BULLETGAUGE : public OBJECT_2D_VERTEXBUFFER 
{
public:
	BULLETGAUGE() { AmmoPower = 0; };
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	HRESULT						MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							AmmoPower;
};
