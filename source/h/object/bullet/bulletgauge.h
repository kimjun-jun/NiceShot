/**
* @file bulletgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// クラス定義
//*****************************************************************************
class BULLETGAUGE : public OBJECT_2D_VERTEXBUFFER 
{
public:
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletGauge(void);
HRESULT ReInitBulletGauge(void);
void UninitBulletGauge(void);
void UpdateBulletGauge(void);
void DrawBulletGauge(void);

