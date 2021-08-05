/**
* @file sky.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

enum eSKY_MODEL_TYPE
{
	SKY_MODEL_TYPE_SPHERE = 0,
	SKY_MODEL_TYPE_TOP,
	SKY_MODEL_TYPE_MAX,
};

//*****************************************************************************
// class定義
//*****************************************************************************
/**
*　@class SKY_PARAMETER
*　@brief 
*/
class SKY_PARAMETER
{
public:
	SKY_PARAMETER() { time = Addtime = fRotY = 0.0f; nNumBlockH = nNumBlockV = 0; }
	~SKY_PARAMETER() {}

	float time;					// 空の時間　時間によって空の色が変わる　未使用
	float Addtime;				// 空の時間　時間によって空の色が変わる　未使用
	float fRotY;				// 回転量
	int nNumBlockH;
	int nNumBlockV;
};

/**
*　@class SKY
*　@brief GAMEOBJECT派生クラス
*/
class SKY : public GAME_OBJECT
{
public:
	SKY();		//!< データ読み込み　初期化
	~SKY();		//!< 削除
	void		Init(void)override;		//!< 初期化
	void		Update(void)override;	//!< 更新
	void		Draw(void)override;		//!< 描画

private:
	ModelAttribute			Attribute[OBJECT_SKY_MAX];			//!< モデル情報　マテリアルや頂点数など　複数使用するならここを配列化
	TEXTURE					tex;								//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX
	VTXBuffer				vtx[OBJECT_SKY_MAX];				//!< 頂点情報　複数使用するならここを配列化
	TransForm				Transform[OBJECT_SKY_MAX];			//!< トランスフォーム情報
	SKY_PARAMETER			SkyPara[OBJECT_SKY_MAX];

	void SetUpMeshSphere(void);			// 球体SKYセットアップ処理
	void SetUpMeshTop(void);			// 上部SKYセットアップ処理
	void ResetColor(void);				//カラー初期化　球体モデルと上部モデル

};
