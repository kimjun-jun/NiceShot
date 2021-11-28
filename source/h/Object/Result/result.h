/**
* @file result.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

enum eRESULT_TEX_TYPE
{
	RESULT_TEX_BG,
	RESULT_TEX_LOGO,
	RESULT_TEX_MAX,
};

/**
*　@class RESULT
*　@brief GAMEOBJECT派生クラス
*/
class RESULT : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	RESULT();		//!< データ読み込み　初期化
	~RESULT();		//!< 削除
	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

private:
	TEXTURE tex[RESULT_TEX_MAX];		//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER vtx;						//!< 頂点情報　複数使用するならここを配列化

	//------他クラスのアドレス
	SCENE *pscene;
	GAME_OBJECT_INSTANCE *pGameObjInstance; //親クラスのアドレスなのであんまり良くない

	const char *c_aFileNameTex[OBJECT_RESULT_MAX] =
	{
		"../data/TEXTURE/other/GameClear_Background.png",	//!< リザルト背景
		"../data/TEXTURE/other/GameClear.png"				//!< リザルトロゴ
	};

};
