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
*　@brief GAMEOBJECT派生クラス
*/
class NETMATCH : public GAME_OBJECT
{
public:
	NETMATCH();		//!< データ読み込み
	~NETMATCH();	//!< 削除
	void		Init(void);								//!< 初期化
	void		Update(GAME_OBJECT* obj, FADE *fade);	//!< 更新
	void		Draw(void);						//!< 描画

private:
	void		MakeVertex(void);
};

