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
class BULLETGAUGE : public OBJECT_2D
{
public:
	BULLETGAUGE() { AmmoPower = 0; };
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						ReinitNet(int MyNumber);			//!< �ď������l�b�g�ΐ�O	
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(PLAYER_HONTAI *player);				//!< �X�V
	virtual void						Draw(bool Netflag, int NetMyNumber, int CntPlayer);					//!< �`��
	HRESULT						MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							AmmoPower;
	TEXTURE_2D					TexEmpty[PLAYER_AMMOPOWER_NORMAL];
	TEXTURE_2D					Tex[PLAYER_AMMOPOWER_NORMAL];
};
