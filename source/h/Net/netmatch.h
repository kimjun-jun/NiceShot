/**
* @file netmatch.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
*　@class NETMATCH
*　@brief GAME_OBJECT_INTERFACE_SUMMRY派生クラス
*/
class NETMATCH : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	NETMATCH();		//!< データ読み込み　初期化
	~NETMATCH();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画
private:

	TEXTURE	tex;			//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER vtx;			//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM Transform;	//!< トランスフォーム情報　各プレイヤーの数 * チュートリアル画像分作成
};

