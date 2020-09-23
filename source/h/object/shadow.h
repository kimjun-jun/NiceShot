/**
* @file shadow.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// 構造体定義
//*****************************************************************************
class SHADOW : public OBJECT_3D
{
public:
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	HRESULT						MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
	int							CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						ReleaseShadow(int nIdxShadow);
	void						SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scl);
	void						SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);
	void						SetColorShadow(int nIdxShadow, D3DXCOLOR col);
	SHADOW						*GetShadow(void);
} ;

