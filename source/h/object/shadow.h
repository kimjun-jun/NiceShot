/**
* @file shadow.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
class SHADOW : public OBJECT_3D
{
public:
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(void);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
	int							CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						ReleaseShadow(int nIdxShadow);
	void						SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);
	void						SetColorShadow(int nIdxShadow, D3DXCOLOR col);
	SHADOW						*GetShadow(void);
} ;

