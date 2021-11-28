/**
* @file title.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

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
class TITLE :private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	TITLE();	//!< データ読み込み　初期化
	~TITLE();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

private:
	TEXTURE	tex[OBJECT_TITLE_MAX];				//!< テクスチャ情報　複数使用するならここを配列化
	VTXBUFFER vtx;								//!< 頂点情報　複数使用するならここを配列化
	TRANSFORM Transform[OBJECT_TITLE_MAX];		//!< トランスフォーム情報
	iUseCheck iUseType[OBJECT_TITLE_MAX];		//!< 使用情報

	TITLE_PARAMETER TitlePara;					//!< インスタンスに必要なデータ群

	//------他クラスのアドレス
	SCENE *pscene;

	void ChangeHierarchy(int NextHierarchyType);	//!< 階層切り替え
	void CheckScene(void);							//!< シーンチェック

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
		"../data/TEXTURE/other/TItle_Background.png",		// 読み込むテクスチャファイル名
		"../data/TEXTURE/other/Title.png",					// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Press.png",					// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Tutorial.png",				// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_LocalBattle.png",			// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_NetorkBattle.png",			// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Quit.png",					// 読み込むテクスチャファイル名
	};

};
