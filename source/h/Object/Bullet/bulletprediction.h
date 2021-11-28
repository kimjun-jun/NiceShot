/**
* @file bulletprediction.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class BULLETPREDICTION
*　@brief GAMEOBJECT派生クラス
*/
class BULLETPREDICTION : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	BULLETPREDICTION();		//!< データ読み込み　初期化
	~BULLETPREDICTION();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

private:
	TEXTURE	tex;				//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER vtx[OBJECT_PLAYER_MAX];				//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM Transform[OBJECT_PLAYER_MAX][OBJECT_BULLETPREDICTION_MAX/ OBJECT_PLAYER_MAX];		//!< トランスフォーム情報

	//------他クラスのアドレス
	PLAYER *pplayer;
	//------描画ループカウントのアドレス
	DRAW_MANAGER *pDrawManager;

	void UpdateInstance(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY, int CntBulletPrediction);

};


