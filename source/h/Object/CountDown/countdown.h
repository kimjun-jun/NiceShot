/**
* @file countdown.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

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
class COUNTDOWN : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	COUNTDOWN();	//!< データ読み込み　初期化
	~COUNTDOWN();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	void AddCountdown(int val);		//!< タイマーカウントを反映させる

private:
	TEXTURE	tex[OBJECT_COUNTDOWN_MAX];			//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER vtx;								//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM Transform[OBJECT_COUNTDOWN_MAX];	//!< トランスフォーム情報
	iUseCheck iUseType[OBJECT_COUNTDOWN_MAX];	//!< 使用情報

	COUNTDOWN_PARAMETER CountdownPara;			//!< インスタンスに必要なデータ群

	//------他クラスのアドレス
	MySOCKET *pmysocket;
	SCENE *pscene;

	void NumberCountUpdate();	//!< カウントダウン数値の更新
	void NumberSizeUpdate();	//!< カウントダウンのスケール更新

} ;

