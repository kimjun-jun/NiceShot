/**
* @file title.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

enum TITLE_SELECT_SCENE//列挙型。defineの番号を自動で割り当ててくれる。
{
	TITLE_SELECT_SCENE_TUTORIAL=2,					//!< 0チュートリアル
	TITLE_SELECT_SCENE_BATTLE,						//!< 1対決
	TITLE_SELECT_SCENE_LOCAL,						//!< 2ローカルゲーム
	TITLE_SELECT_SCENE_NET,							//!< 3ネットゲーム
	TITLE_SELECT_SCENE_MAX
};

enum TITLE_HIERARCHY_NUM
{
	TITLE_HIERARCHY_NUM_TITLE,						//!< 0タイトルロゴ
	TITLE_HIERARCHY_NUM_TUTO_BATTLE,				//!< 1チュートリアル、対決
	TITLE_HIERARCHY_NUM_LOCAL_NET,					//!< 2ローカル、ネット選択
	TITLE_HIERARCHY_NUM_MAX
};

enum TITLE_OBJECT_NAME
{
	TITLE_OBJECT_NAME_TITLELOGO,					//!< 0タイトルロゴ
	TITLE_OBJECT_NAME_START,						//!< 1スタートロゴ
	TITLE_OBJECT_NAME_TUTO,							//!< 2チュートリアル
	TITLE_OBJECT_NAME_BATTLE,						//!< 3対決
	TITLE_OBJECT_NAME_LOCAL,						//!< 4ローカル
	TITLE_OBJECT_NAME_NET,							//!< 5ネット
	TITLE_OBJECT_NAME_MAX,							//!< 5ネット
};

/**
*　@class TITLECLASS 0背景 1スタートロゴ 2練習 3試合
*　@brief 2Dポリゴンを定義する構造体
*/
class TITLE :public OBJECT_2D_VERTEXBUFFER
{
public:
	TITLE() {
		fAlpha = 0.0f; HierarchyNum = 0; SceneNum = 0; bDisp = false;
	}
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(FADE *fade);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexTitle(void);
	void						SetColorTitle(int CntTitle, float alpha);

	float			fAlpha;							//!<
	int				HierarchyNum;					//!<
	int				SceneNum;						//!<
	bool			bDisp;							//!<
};
