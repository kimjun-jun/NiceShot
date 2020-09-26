/**
* @file bulletgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../../h/object/objectclass.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class BULLETGAUGE : public OBJECT_2D_VERTEXBUFFER 
{
public:
	BULLETGAUGE() { AmmoPower = 0; };
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(void);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							AmmoPower;
};
