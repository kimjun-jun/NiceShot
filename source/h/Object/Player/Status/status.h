/**
* @file status.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
*　@class STATUS
*　@brief GAMEOBJECT派生クラス　
*/
class STATUS : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	STATUS();		//!< データ読み込み　初期化
	~STATUS();		//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override;			//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画


private:
	TEXTURE tex[OBJECT_STATUS_MAX];			//!< テクスチャ情報　複数使用するならここを配列化　
	VTXBUFFER vtx[OBJECT_PLAYER_MAX];		//!< 頂点情報　配列構造は[P1tex1,P1tex2,P1tex3,P1tex4,P2tex1,P2tex2,,,]構造
	TRANSFORM Transform[OBJECT_PLAYER_MAX];	//!< トランスフォーム情報　各プレイヤー用のスクリーンに描画
	iUseCheck iUseType[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< 使用情報

	//------他クラスのアドレス
	PLAYER *pplayer;
	MySOCKET *pmysocket;
	//------描画ループカウントのアドレス
	DRAW_MANAGER *pDrawManager;

	float PtternV[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< 取得直後は全部描画するが、時間が経過するにつれてテクスチャが徐々に上から消えていく　表示非表示もここで設定できる
	//eSTATUS_TYPE e;
	const char *c_FileNameTex[OBJECT_STATUS_MAX] =
	{
		"../data/TEXTURE/UI/UI_Skill_Power.png",		// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Skill_Speed.png",		// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Skill_Flash.png",		// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Skill_Blind.png",		// 読み込むテクスチャファイル名
	};

};
