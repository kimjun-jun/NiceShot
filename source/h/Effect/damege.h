/** 
* @file damege.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../h/Object/ObjectClass/Interface/interface.h"
#include "../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//前置宣言
class MySOCKET;

/**
*　@class DAMEGE_PARAMETER
*　@brief 
*/
class DAMEGE_PARAMETER 
{
public:
	DAMEGE_PARAMETER() { alpha = 0.0f; time = 0.0f; }
	~DAMEGE_PARAMETER() {}

	float	time;		//!< 描画時間
	float	alpha;		//!< アルファ値
};

/**
*　@class DAMEGE
*　@brief GAMEOBJECT派生クラス
*/
class DAMEGE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	DAMEGE();	//!< データ読み込み　初期化
	~DAMEGE();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override;			//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	iUseCheck	iUseType[OBJECT_DAMEGE_MAX];			//!< 使用情報
	DAMEGE_PARAMETER	DamegePara[OBJECT_DAMEGE_MAX];	//!< インスタンスに必要なデータ群

private:
	TEXTURE		tex;	//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER	vtx;	//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM	Transform[OBJECT_DAMEGE_MAX];			//!< トランスフォーム情報

	//------他クラスのアドレス
	MySOCKET *pmysocket;

	//------描画ループカウントのアドレス
	DRAW_MANAGER *pDrawManager;

};

