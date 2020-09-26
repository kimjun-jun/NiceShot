/**
* @file shadow.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
class SHADOW : public OBJECT_3D
{
public:
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(void);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
	int							CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						ReleaseShadow(int nIdxShadow);
	void						SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);
	void						SetColorShadow(int nIdxShadow, D3DXCOLOR col);
	SHADOW						*GetShadow(void);
} ;

