/**
* @file result.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*　@class RESULT
*　@brief 2Dポリゴンを定義するクラス
*/
class RESULT : public OBJECT_2D
{
public:
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(GAME_OBJECT* obj, FADE *fade);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexResult(void);
};
