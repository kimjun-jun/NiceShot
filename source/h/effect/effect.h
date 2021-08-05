/**
* @file effect.h
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
*　@class EFFECT_PARAMETER
*　@brief 
*/
class EFFECT_PARAMETER
{
public:
	EFFECT_PARAMETER() { nTimer = 0; nDecAlpha = 1.0f; }
	~EFFECT_PARAMETER() {}
	int			nTimer;				//!< タイマー
	float		nDecAlpha;			//!< 減衰値
};

/**
*　@class EFFECT
*　@brief GAMEOBJECT派生クラス
*/
class EFFECT : public GAME_OBJECT
{
public:
	EFFECT();	//!< データ読み込み　初期化
	~EFFECT();	//!< 削除

	void		Init(void);					//!< 初期化
	void		Update(void);				//!< 更新
	void		Draw(int CntPlayer);		//!< 描画

	//インスタンス作成と解放　＊解放に関してはタイマーで自動未使用になるので用意しない
	int		SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);	//!< インスタンスセット

private:


	TEXTURE		tex;				//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;				//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_EFFECT_MAX];		//!< トランスフォーム情報
	iUseCheak	iUseType[OBJECT_EFFECT_MAX];		//!< 使用情報
	Movement	move[OBJECT_EFFECT_MAX];			//!< 移動量

	EFFECT_PARAMETER EffectPara[OBJECT_EFFECT_MAX];	//!< インスタンスに必要なデータ群

};

