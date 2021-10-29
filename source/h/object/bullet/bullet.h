/**
* @file bullet.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once
#include "../../../h/object/objectclass.h"


//*****************************************************************************
// class定義
//*****************************************************************************
/**
*　@class BULLET_PARAMETER
*　@brief 
*/
class BULLET_PARAMETER
{
public:
	BULLET_PARAMETER() { Timer = IdxShadow = 0; Gravity = FieldPosY = 0.0f; UsePlayerType = PLAYER_NONE; }
	~BULLET_PARAMETER() {}

	int		Timer;							// タイマー
	int		IdxShadow;						// 影ID
	float	Gravity;						// 重力
	float	FieldPosY;						// 影用の現在地の地形POSYを記憶
	ePLAYER_TYPE	UsePlayerType;			// 何プレイヤーが使用してるか識別する

};

/**
*　@class BULLET
*　@brief GAMEOBJECT派生クラス
*/
class BULLET : public GAME_OBJECT
{
public:
	BULLET();		//!< データ読み込み　初期化
	~BULLET();		//!< 削除

	void	Init(void);						//!< 初期化
	void	Update(SHADOW *s, EFFECT *e);	//!< 更新
	void	Draw(void);						//!< 描画

	TransForm	Transform[OBJECT_BULLET_MAX];		//!< トランスフォーム情報
	iUseCheak	iUseType[OBJECT_BULLET_MAX];		//!< 使用情報
	BULLET_PARAMETER	BulletPara[OBJECT_BULLET_MAX];	//!< インスタンスに必要なデータ群

	int		SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, ePLAYER_TYPE type, SHADOW *s);	//!< インスタンスセット
	void	ReleaseInstance(int nIdxBullet, SHADOW *s);	//!< インスタンス解放

private:

	Movement	move[OBJECT_BULLET_MAX];			//!< 移動量

} ;


