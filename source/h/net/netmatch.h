/**
* @file netmatch.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*　@class NETMATCH
*　@brief 2Dポリゴンを定義する構造体
*/
class NETMATCH : public OBJECT_2D
{
public:
	NETMATCH() { cnt = 0; }
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(GAME_OBJECT* obj, FADE *fade);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexTutorial(void);
	int							cnt;						//!< 描画判定用　2で割った余り判定　0表示　1非表示　今後のテクスチャ情報が増えることを加味してint定義
};

