/**
* @file damege.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*　@class DAMEGE_PARAMETER
*　@brief 
*/
class DAMEGE_PARAMETER 
{
public:
	DAMEGE_PARAMETER() { alpha = 0; time = 0; }
	~DAMEGE_PARAMETER() {}

	int		time;		//!< 描画時間
	float	alpha;		//!< アルファ値
};

/**
*　@class DAMEGE
*　@brief GAMEOBJECT派生クラス
*/
class DAMEGE : public GAME_OBJECT
{
public:
	DAMEGE();	//!< データ読み込み　初期化
	~DAMEGE();	//!< 削除

	void	Init(void);			//!< 初期化
	void	Update(void);		//!< 更新
	void	Draw(bool Netflag, int NetMyNumber, int CntPlayer);			//!< 描画
	iUseCheak	iUseType[OBJECT_DAMEGE_MAX];			//!< 使用情報
	DAMEGE_PARAMETER	DamegePara[OBJECT_DAMEGE_MAX];	//!< インスタンスに必要なデータ群

private:

	void	ReinitNet(void);	//!< 再初期化

	TEXTURE		tex;	//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;	//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_DAMEGE_MAX];			//!< トランスフォーム情報

};

