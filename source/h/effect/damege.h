/**
* @file damege.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
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
	DAMEGE_PARAMETER() { alpha = 0.0f; time = 0.0f; }
	~DAMEGE_PARAMETER() {}

	float	time;		//!< 描画時間
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
	void	InitNet(int MyNumber);	//!< 初期化
	void	Update(void);		//!< 更新
	void	Draw(bool Netflag, int NetMyNumber, int CntPlayer);			//!< 描画
	iUseCheak	iUseType[OBJECT_DAMEGE_MAX];			//!< 使用情報
	DAMEGE_PARAMETER	DamegePara[OBJECT_DAMEGE_MAX];	//!< インスタンスに必要なデータ群

private:


	TEXTURE		tex;	//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;	//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_DAMEGE_MAX];			//!< トランスフォーム情報

};

