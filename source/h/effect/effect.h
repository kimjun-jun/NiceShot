/**
* @file effect.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class EFFECT : public OBJECT_3D
{
public:
	EFFECT() { nTimer = 0, nDecAlpha = 0.0f; };
	virtual void				Init(void);					//!< 初期化
	virtual void				Reinit(void);				//!< 再初期化
	virtual void				Uninit(void);				//!< 終了
	virtual void				Update(void);				//!< 更新
	virtual void				Draw(int CntPlayer);					//!< 描画
	HRESULT						MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
	void						SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
	void						SetColorEffect(int nIdxEffect, D3DXCOLOR col);
	int							SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);
	int							nTimer;				//!< タイマー
	float						nDecAlpha;			//!< 減衰値
} ;

