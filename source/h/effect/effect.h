/**
* @file effect.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class EFFECT : public OBJECT_3D
{
public:
	EFFECT() { nTimer = 0, nDecAlpha = 0.0f; };
	virtual void				Init(void);					//!< ������
	virtual void				Reinit(void);				//!< �ď�����
	virtual void				Uninit(void);				//!< �I��
	virtual void				Update(void);				//!< �X�V
	virtual void				Draw(int CntPlayer);					//!< �`��
	HRESULT						MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
	void						SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
	void						SetColorEffect(int nIdxEffect, D3DXCOLOR col);
	int							SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);
	int							nTimer;				//!< �^�C�}�[
	float						nDecAlpha;			//!< �����l
} ;

