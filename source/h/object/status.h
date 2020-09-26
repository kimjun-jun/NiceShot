/**
* @file status.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	STATUS_SIZE_X			(20.0f)							// ステータスの幅
#define	STATUS_SIZE_Y			(20.0f)							// ステータスの高さ
#define	STATUS_POS_X			(SCREEN_CENTER_X)				// ステータスの表示位置
#define	STATUS_POS_Y			(SCREEN_CENTER_Y)				// ステータスの表示位置
#define	STATUS_POS_X_OFFSET		(180.0f)						// ステータスの表示位置オフセット
#define	STATUS_POS_Y_OFFSET		(120.0f)						// ステータスの表示位置オフセット


class OBJECT_2D;


class STATUS : public OBJECT_2D
{
public:
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(void);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexStatus(void);
};
