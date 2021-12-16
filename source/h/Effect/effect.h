/** 
* @file effect.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../h/Object/ObjectClass/Interface/interface.h"
#include "../../h/Object/ObjectClass/Instance/instance.h"
#include "../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"
#include "../../h/Object/ObjectClass/StandardComponent/Move/Move.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class EFFECT_PARAMETER
*　@brief 
*/
class EFFECT_PARAMETER
{
public:
	EFFECT_PARAMETER() { nTimer = 0; nDecAlpha = 1.0f; }
	~EFFECT_PARAMETER() {}
	int			nTimer;				//!< タイマー
	float		nDecAlpha;			//!< 減衰値
};

/**
*　@class EFFECT
*　@brief GAMEOBJECT派生クラス
*/
class EFFECT : public GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	EFFECT();	//!< データ読み込み　初期化
	~EFFECT();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	//インスタンス作成と解放　＊解放に関してはタイマーで自動未使用になるので用意しない
	int		SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);	//!< インスタンスセット

private:
	TEXTURE		tex;				//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER	vtx;				//!< 頂点情報　複数使用するならここを配列化

	TRANSFORM	Transform[OBJECT_EFFECT_MAX];		//!< トランスフォーム情報
	iUseCheck	iUseType[OBJECT_EFFECT_MAX];		//!< 使用情報
	MOVE	move[OBJECT_EFFECT_MAX];			//!< 移動量

	EFFECT_PARAMETER EffectPara[OBJECT_EFFECT_MAX];	//!< インスタンスに必要なデータ群

	//------描画ループカウントのアドレス
	DRAW_MANAGER *pDrawManager;

};

