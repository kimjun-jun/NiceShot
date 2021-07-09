/**
* @file sky.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*　@class SKY_PARAMETER
*　@brief SKY_PARAMETER
*/
class SKY_PARAMETER
{
	SKY_PARAMETER();
	~SKY_PARAMETER();

	float time;					// 空の時間　時間によって空の色が変わる
	float Addtime;				// 空の時間　時間によって空の色が変わる
	float fRotY;				// 回転量
	int nNumBlockH;
	int nNumBlockV;
};

/**
*　@class SKY
*　@brief GAMEOBJECT派生クラス
*/
class SKY : public GAME_OBJECT
{
public:
	SKY();		//!< データ読み込み
	~SKY();		//!< 削除
	void		Init(void);		//!< 初期化
	void		Update(void);	//!< 更新
	void		Draw(void);		//!< 描画

private:
	SKY_PARAMETER SkyPara;
};
