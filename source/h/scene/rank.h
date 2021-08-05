/**
* @file rank.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


#include "../../h/object/objectclass.h"
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
	RANK_PARAMETER_ALL() { RankNum = 0; }
	~RANK_PARAMETER_ALL() {}
	int			RankNum;			//!< 自分の順位	
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
class RANK : public GAME_OBJECT
{
public:
	RANK();		//!< データ読み込み　初期化
	~RANK();	//!< 削除

	void		Init(void);				//!< 初期化
	//void		Update(void);			//!< 更新
	void		Draw(bool Netflag);		//!< 描画

	void		SetRank(int PlayerNum);
	void		SetRankNet(int PlayerNum, int NetMyNumber);

private:
	TEXTURE		tex[OBJECT_RANK_MAX-1];			//!< テクスチャ情報　複数使用するならここを配列化　ちょっとケチって-1　一位の分はない
	VTXBuffer	vtx;							//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_RANK_MAX];		//!< トランスフォーム情報
	iUseCheak	iUseType[OBJECT_RANK_MAX];		//!< 使用情報

	RANK_PARAMETER_ALL	RankParaAll[OBJECT_RANK_MAX];	//!< インスタンスに必要なデータ群
	RANK_PARAMETER_ONE	RankParaOne;					//!< マネージャーに必要なデータ群

	//ちょっとケチって-1　一位の分はない
	const char *c_aFileNameTex[OBJECT_RANK_MAX - 1] =
	{
		"../data/TEXTURE/rank/4i.png",	//!< 四位
		"../data/TEXTURE/rank/3i.png",	//!< 三位
		"../data/TEXTURE/rank/2i.png",	//!< 二位
	};
};
