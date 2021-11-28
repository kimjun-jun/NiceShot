/**
* @file bulletgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

enum eBULLETGAUGE_TEX_TYPE
{
	BULLETGAUGE_TEX_BASE,
	BULLETGAUGE_TEX_SHELL,
	BULLETGAUGE_TEX_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class BULLETGAUGE_PARAMETER
*　@brief 
*/
class BULLETGAUGE_PARAMETER
{
public:
	BULLETGAUGE_PARAMETER() { AmmoStock = 0; }
	~BULLETGAUGE_PARAMETER() { }
	int	AmmoStock;
};

/**
*　@class BULLETGAUGE
*　@brief GAMEOBJECT派生クラス
*/
class BULLETGAUGE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	BULLETGAUGE();	//!< データ読み込み　初期化
	~BULLETGAUGE();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override;			//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

private:

	TEXTURE	tex[BULLETGAUGE_TEX_MAX];					//!< テクスチャ情報　複数使用するならここを配列化　0:枠　1:中身
	VTXBUFFER vtx[OBJECT_BULLETGAUGE_MAX];				//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM Transform[OBJECT_BULLETGAUGE_MAX];		//!< トランスフォーム情報
	BULLETGAUGE_PARAMETER BulletGaugePara[OBJECT_BULLETGAUGE_MAX];//!< インスタンスに必要なデータ群

	//------他クラスのアドレス
	MySOCKET *pmysocket;
	PLAYER *pplayer;
	//------描画ループカウントのアドレス
	DRAW_MANAGER *pDrawManager;


	const char *c_aFileNameTex[BULLETGAUGE_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_Shell.png" ,
		"../data/TEXTURE/UI/UI_Shell_Empty.png",
	};
};
