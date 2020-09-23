/**
* @file shadow.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
class SHADOW : public OBJECT_3D
{
public:
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	HRESULT						MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
	int							CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						ReleaseShadow(int nIdxShadow);
	void						SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);
	void						SetColorShadow(int nIdxShadow, D3DXCOLOR col);
	SHADOW						*GetShadow(void);
} ;

