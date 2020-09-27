/**
* @file vitalgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class VITALGAUGE : public OBJECT_2D_VERTEXBUFFER
{
public:
	VITALGAUGE() { VitalPower = 0; };
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(PLAYER_HONTAI *p, RANK *rank);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							VitalPower;
};


