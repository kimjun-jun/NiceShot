/**
* @file vitalgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

enum VITAL_TEX_TYPE
{
	VITAL_TEX_BASE,
	VITAL_TEX_GREEN,
	VITAL_TEX_ORANGE,
	VITAL_TEX_RED,
	VITAL_TEX_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class VITALGAUGE
*　@brief GAMEOBJECT派生クラス
*/
class VITALGAUGE_PARAMETER
{
public:
	VITALGAUGE_PARAMETER() { VitalPower = PLAYER_VITAL_MAX; }
	~VITALGAUGE_PARAMETER() {}

	int			VitalPower;
};

/**
*　@class VITALGAUGE
*　@brief GAMEOBJECT派生クラス
*/
class VITALGAUGE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	VITALGAUGE();	//!< データ読み込み　初期化
	~VITALGAUGE();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override;			//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

private:
	TEXTURE	tex[VITAL_TEX_MAX];				//!< テクスチャ情報　複数使用するならここを配列化　0:枠　1,2,3:中身(カラーバリエーション)
	VTXBUFFER vtx[OBJECT_VITAL_MAX];		//!< 頂点情報　複数使用するならここを配列化 
	TRANSFORM Transform[OBJECT_VITAL_MAX];	//!< トランスフォーム情報
	VITALGAUGE_PARAMETER VitalGaugePara[OBJECT_VITAL_MAX];//!< インスタンスに必要なデータ群

	//------他クラスのアドレス
	PLAYER *pplayer;
	MySOCKET *pmysocket;
	RANK *prank;
	//------描画ループカウントのアドレス
	DRAW_MANAGER *pDrawManager;

	const char *c_aFileNameTex[VITAL_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_HP_Base.png",	
		"../data/TEXTURE/UI/UI_HP_Green.png",	
		"../data/TEXTURE/UI/UI_HP_Orange.png",	
		"../data/TEXTURE/UI/UI_HP_Red.png",		
	};

};

