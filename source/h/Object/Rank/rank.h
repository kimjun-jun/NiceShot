/**
* @file rank.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

constexpr int	RANK_COUNTDOWN_NUM{ 2 };					//!< 順位の数　4人なので2,1,0,を使う テクスチャ配列が0-2なので最大値が2

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@struct RANK_PARAMETER_ALL
*　@brief
*/
class RANK_PARAMETER_ALL
{
public:
	RANK_PARAMETER_ALL() { RankNum = RANK_COUNTDOWN_NUM; }
	~RANK_PARAMETER_ALL() {}
	int			RankNum;			//!< 自分の順位	4人だから　2,1,0とカウントダウンさせていく
};

/**
*　@struct RANK_PARAMETER_ONE
*　@brief
*/
class RANK_PARAMETER_ONE
{
public:
	RANK_PARAMETER_ONE() { NetUse = false; }
	~RANK_PARAMETER_ONE() {}
	bool		NetUse;				//!< ネット対戦時に自分の順位が確定したか判定
};

/**
*　@class RANK
*　@brief GAMEOBJECT派生クラス
*/
class RANK : public GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	RANK();		//!< データ読み込み　初期化
	~RANK();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override;		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	void SetRank(int PlayerNum);
	void SetRankNet(int PlayerNum, int NetMyNumber);

private:
	TEXTURE	tex[OBJECT_RANK_MAX-1];			//!< テクスチャ情報　複数使用するならここを配列化　ちょっとケチって-1　一位の分はない
	VTXBUFFER vtx;							//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM Transform[OBJECT_RANK_MAX];	//!< トランスフォーム情報
	iUseCheck iUseType[OBJECT_RANK_MAX];	//!< 使用情報

	RANK_PARAMETER_ALL RankParaAll[OBJECT_RANK_MAX];	//!< インスタンスに必要なデータ群
	RANK_PARAMETER_ONE RankParaOne;						//!< マネージャーに必要なデータ群

	//------他クラスのアドレス
	MySOCKET *pmysocket;

	//ちょっとケチって-1　一位の分はない
	const char *c_aFileNameTex[OBJECT_RANK_MAX - 1] =
	{
		"../data/TEXTURE/rank/2i.png",	//!< 二位
		"../data/TEXTURE/rank/3i.png",	//!< 三位
		"../data/TEXTURE/rank/4i.png",	//!< 四位
	};
};
