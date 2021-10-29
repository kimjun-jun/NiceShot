/**
* @file result.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../h/object/objectclass.h"

enum eRESULT_TEX_TYPE
{
	RESULT_TEX_BG,
	RESULT_TEX_LOGO,
	RESULT_TEX_MAX,
};

/**
*　@class RESULT
*　@brief GAMEOBJECT派生クラス
*/
class RESULT : public GAME_OBJECT
{
public:
	RESULT();		//!< データ読み込み　初期化
	~RESULT();		//!< 削除
	void		Init(void);					//!< 初期化
	void		Update(GAME_OBJECT* obj, FADE *fade);	//!< 更新
	void		Draw(void);					//!< 描画


private:
	TEXTURE		tex[RESULT_TEX_MAX];		//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;						//!< 頂点情報　複数使用するならここを配列化
	//TransForm	Transform[OBJECT_RESULT_MAX];		//!< トランスフォーム情報

	const char *c_aFileNameTex[OBJECT_RESULT_MAX] =
	{
		"../data/TEXTURE/other/GameClear_Background.png",	//!< リザルト背景
		"../data/TEXTURE/other/GameClear.png"				//!< リザルトロゴ
	};

};
