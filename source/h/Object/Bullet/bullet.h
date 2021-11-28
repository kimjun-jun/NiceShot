/**
* @file bullet.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

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
class BULLET : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	BULLET();		//!< データ読み込み　初期化
	~BULLET();		//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	TRANSFORM Transform[OBJECT_BULLET_MAX];		//!< トランスフォーム情報
	iUseCheck iUseType[OBJECT_BULLET_MAX];		//!< 使用情報
	BULLET_PARAMETER BulletPara[OBJECT_BULLET_MAX];	//!< インスタンスに必要なデータ群

	int SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, ePLAYER_TYPE type);	//!< インスタンスセット
	void ReleaseInstance(int nIdxBullet);	//!< インスタンス解放

private:

	MOVEMENT	move[OBJECT_BULLET_MAX];			//!< 移動量

	//------他クラスのアドレス
	SHADOW *pshadow;
	EFFECT *peffect;
} ;


