/**
* @file damege.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_DAMEGE			("../data/TEXTURE/screendamage.png")			
#define	DAMEGE_SIZE_X			(SCREEN_W/4)										// チュートリアルの幅
#define	DAMEGE_SIZE_Y			(SCREEN_H/4)										// チュートリアルの高さ
#define	DAMEGE_POS_X			(SCREEN_CENTER_X)									// チュートリアルの表示位置
#define	DAMEGE_POS_Y			(SCREEN_CENTER_Y)									// チュートリアルの表示位置



class DAMEGE : public OBJECT_2D
{
public:
	DAMEGE() { alpha = 0, time = 0.0f; }
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	HRESULT						MakeVertexDamege(void);		//!< 頂点設定
	int							alpha;						//!< アルファ値
	float						time;						//!< 描画時間
};

