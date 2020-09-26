/**
* @file bulletprediction.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../../h/object/objectclass.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class BULLETPREDICTION : public OBJECT_3D
{
public:
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(void);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice);
	void						SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY);
	void						SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col);
	int							SetBulletprediction(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY);
};


