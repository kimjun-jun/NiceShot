/**
* @file countdown.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


#include "../../h/object/objectclass.h"

enum COUNTDOWN_TEX_TYPE
{
	COUNTDOWN_TEX_NUMBER,
	COUNTDOWN_TEX_LOGO,
	COUNTDOWN_TEX_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@struct COUNTDOWN_PARAMETER
*　@brief 
*/
class COUNTDOWN_PARAMETER
{
public:
	COUNTDOWN_PARAMETER() { color = 0.0f; CountAnim = 0; PatternAnim = 0; changeval = 0;  time_maneger = 0; fps_maneger = 0; CountEnd = false; }
	~COUNTDOWN_PARAMETER() {}
	float		color;				//!< カラー値(今回はアルファ値として使用)
	int			CountAnim;			//!< アニメカウント
	int			PatternAnim;		//!< アニメパターン
	int			changeval;			//!< 拡大する変化量
	int			time_maneger;		//!< カウントダウン数値	→毎フレーム減算される
	int			fps_maneger;		//!< カウントダウン減算値	→[3,2,1]GO
	bool		CountEnd;			//!< カウントダウン終了判定　これがtrueならゲーム開始シーン
};

/**
*　@class COUNTDOWN
*　@brief GAMEOBJECT派生クラス
*/
class COUNTDOWN : public GAME_OBJECT
{
public:
	COUNTDOWN();	//!< データ読み込み　初期化
	~COUNTDOWN();	//!< 削除

	void		Init(void);				//!< 初期化
	void		Update(GAME_OBJECT*obj, bool Netflag);	//!< 更新
	void		Draw(void);				//!< 描画

	void		AddCountdown(int val);		//!< タイマーカウントを反映させる

private:

	void		NumberCountUpdate();	//!< カウントダウン数値の更新
	void		NumberSizeUpdate();		//!< カウントダウンのスケール更新

	TEXTURE		tex[OBJECT_COUNTDOWN_MAX];		//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;							//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_COUNTDOWN_MAX];//!< トランスフォーム情報
	iUseCheak	iUseType[OBJECT_COUNTDOWN_MAX];	//!< 使用情報

	COUNTDOWN_PARAMETER CountdownPara;			//!< インスタンスに必要なデータ群
} ;

