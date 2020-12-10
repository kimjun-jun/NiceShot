/**
* @file vitalgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
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
// クラス定義
//*****************************************************************************
class VITALGAUGE : public OBJECT_2D
{
public:
	VITALGAUGE() { VitalPower = 0; };
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						ReinitNet(int MyNumber);			//!< 再初期化ネット対戦前	
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(PLAYER_HONTAI *p, RANK *rank, bool Netflag, int NetMyNumber);				//!< 更新
	virtual void						Draw(bool Netflag, int NetMyNumber,int CntPlayer);					//!< 描画
	HRESULT						MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);
	int							VitalPower;
	TEXTURE_2D					Tex[VITALTEX_MAX];
};


