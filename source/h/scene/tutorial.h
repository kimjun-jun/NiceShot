/**
* @file tutorial.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
*　@class TUTO
*　@brief GAMEOBJECT派生クラス
*/
class TUTO : public GAME_OBJECT
{
public:
	TUTO();		//!< データ読み込み　初期化
	~TUTO();	//!< 削除

	void	Init(void);								//!< 初期化
	void	Update(GAME_OBJECT* obj, FADE *fade);	//!< 更新
	void	Draw(void);								//!< 描画

private:

	TEXTURE		tex;								//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;								//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX];		//!< トランスフォーム情報　各プレイヤーの数 * チュートリアル画像分作成
	iUseCheak	iUseType[OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX];		//!< 使用情報　各プレイヤーの数分作成 * チュートリアル画像分作成
};

