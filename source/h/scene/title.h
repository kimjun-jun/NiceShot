/**
* @file title.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

enum eTITLE_HIERARCHY_NUM
{
	TITLE_HIERARCHY_NUM_ANIM,				//!< 0アニメ中
	TITLE_HIERARCHY_NUM_SELECT,				//!< 1シーン選択中
	TITLE_HIERARCHY_NUM_MAX
};

/**
*　@class TITLE_PARAMETER
*　@brief 
*/
class TITLE_PARAMETER
{
public:
	TITLE_PARAMETER() { HierarchyNum = TITLE_HIERARCHY_NUM_ANIM; SceneNumX = OldSceneNumX = SceneNumY = OldSceneNumY = 0; }
	~TITLE_PARAMETER() {}
	int		SceneNumX;		//!< カーソルデータX値
	int		OldSceneNumX;	//!< カーソルデータX値
	int		SceneNumY;		//!< カーソルデータY値
	int		OldSceneNumY;	//!< カーソルデータY値
	eTITLE_HIERARCHY_NUM	HierarchyNum;	//!< 階層データ　タイトルアニメ階層(起動初期状態)、シーンセレクト階層(シーン選択状態)
};

/**
*　@class TITLE
*　@brief GAMEOBJECT派生クラス
*/
class TITLE :public GAME_OBJECT
{
public:
	TITLE();	//!< データ読み込み　初期化
	~TITLE();	//!< 削除

private:
	void	Init(void);								//!< 初期化
	void	Update(GAME_OBJECT* obj, FADE *fade);	//!< 更新
	void	Draw(void);								//!< 描画

	void	ChangeHierarchy(int NextHierarchyType);	//!< 階層切り替え
	void	CheckScene(FADE *fade);						//!< シーンチェック

	TEXTURE		tex[OBJECT_TITLE_MAX];				//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;								//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_TITLE_MAX];		//!< トランスフォーム情報
	iUseCheak	iUseType[OBJECT_TITLE_MAX];			//!< 使用情報

	TITLE_PARAMETER TitlePara;						//!< インスタンスに必要なデータ群

	enum TITLE_NAME
	{
		NAME_BG,		//!< 0背景
		NAME_LOGO,		//!< 1ロゴ
		NAME_PRESS,		//!< 2PRESS
		NAME_TUTO,		//!< 3チュートリアル
		NAME_LOCAL,		//!< 4ローカル
		NAME_NET,		//!< 5ネット
		NAME_QUIT,		//!< 6終了
		NAME_MAX,		//!< 最大
	};

	const char *c_FileNameTex[OBJECT_TITLE_MAX] =
	{
		"../data/TEXTURE/other/Title.png"					// 読み込むテクスチャファイル名
		"../data/TEXTURE/other/TItle_Background.png"		// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Press.png"					// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Tutorial.png"				// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_LocalBattle.png"				// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_NetorkBattle.png"			// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Quit.png"					// 読み込むテクスチャファイル名
	};

};
