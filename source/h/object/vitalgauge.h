/**
* @file vitalgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

enum VITALTEX_TYPE
{
	VITALTEX_BASE,
	VITALTEX_GREEN,
	VITALTEX_ORANGE,
	VITALTEX_RED,
	VITALTEX_MAX,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class VITALGAUGE : public OBJECT_2D
{
public:
	VITALGAUGE() { VitalPower = 0; };
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						ReinitNet(int MyNumber);			//!< �ď������l�b�g�ΐ�O	
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(PLAYER_HONTAI *p, RANK *rank, bool Netflag, int NetMyNumber);				//!< �X�V
	virtual void						Draw(bool Netflag, int NetMyNumber,int CntPlayer);					//!< �`��
	HRESULT						MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							VitalPower;
	TEXTURE_2D					Tex[VITALTEX_MAX];
};


