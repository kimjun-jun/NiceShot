/**
* @file shadow.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
/**
*　@class SHADOW
*　@brief GAMEOBJECT派生クラス
*/
class SHADOW : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	SHADOW();	//!< データ読み込み　初期化
	~SHADOW();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	int	SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl);	//!< インスタンスセット
	void ReleaseInstance(int Idx);						//!< インスタンス解放
	void UpdateInstance(int Idx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);	//インスタンスアップデート　各インスタンス毎に任意呼び出し

private:

	TEXTURE	tex;	//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER vtx;	//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM Transform[OBJECT_SHADOW_MAX];		//!< トランスフォーム情報
	iUseCheck iUseType[OBJECT_SHADOW_MAX];		//!< 使用情報

} ;

