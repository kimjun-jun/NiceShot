/**
* @file wall.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
*　@class WALL
*　@brief GAMEOBJECT派生クラス
*/
class WALL : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	WALL();		//!< データ読み込み　初期化
	~WALL();	//!< 削除
	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画
	TRANSFORM Transform[OBJECT_WALL_MAX];			//!< トランスフォーム情報

private:
	TEXTURE	tex;								//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX
	VTXBUFFER vtx[OBJECT_WALL_MAX];				//!< 頂点情報　複数使用するならここを配列化

	void SetUpMesh(void);			// WALLセットアップ処理

} ;
