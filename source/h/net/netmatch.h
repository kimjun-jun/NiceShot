/**
* @file netmatch.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*　@class TUTO
*　@brief GAMEOBJECT派生クラス
*/
class NETMATCH : public GAME_OBJECT
{
public:
	NETMATCH();		//!< データ読み込み　初期化
	~NETMATCH();	//!< 削除

	void	Init(void)override;								//!< 初期化
	void	Update(GAME_OBJECT* obj, FADE *fade);			//!< 更新
	void	Draw(void)override;								//!< 描画

private:

	TEXTURE		tex;			//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;			//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform;		//!< トランスフォーム情報　各プレイヤーの数 * チュートリアル画像分作成
};

