/**
* @file wall.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"


/**
*　@class WALL
*　@brief GAMEOBJECT派生クラス
*/
class WALL : public GAME_OBJECT
{
public:
	WALL();		//!< データ読み込み
	~WALL();	//!< 削除
	void		Init(void);		//!< 初期化
	void		Update(void);	//!< 更新
	void		Draw(void);		//!< 描画

private:
} ;
