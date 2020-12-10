/**
* @file title.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

enum TITLE_HIERARCHY_NUM
{
	TITLE_HIERARCHY_NUM_ANIM,						//!< 0アニメ中
	TITLE_HIERARCHY_NUM_SELECT,				//!< 1シーン選択中
	TITLE_HIERARCHY_NUM_MAX
};

enum TITLE_OBJECT_NAME
{
	TITLE_OBJECT_NAME_BG,					//!< 0背景
	TITLE_OBJECT_NAME_LOGO,					//!< 1ロゴ
	TITLE_OBJECT_NAME_PRESS,				//!< 2PRESS
	TITLE_OBJECT_NAME_TUTO,					//!< 3チュートリアル
	TITLE_OBJECT_NAME_LOCAL,				//!< 4ローカル
	TITLE_OBJECT_NAME_NET,					//!< 5ネット
	TITLE_OBJECT_NAME_QUIT,					//!< 6終了
	TITLE_OBJECT_NAME_MAX,					//!< 最大
};

/**
*　@class TITLECLASS
*　@brief 2Dポリゴンを定義する構造体
*/
class TITLE :public OBJECT_2D_VERTEXBUFFER
{
public:
	TITLE() {
		fAlpha = 0.0f;
		HierarchyNum = 0;
		SceneNumX = 0;
		SceneNumY = 0;
		bDisp = false;
	}
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(GAME_OBJECT* obj, FADE *fade);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexTitle(void);
	void						SetColorTitle(int CntTitle, float alpha);

	float			fAlpha;							//!<
	int				HierarchyNum;					//!<
	int				SceneNumX;						//!<
	int				SceneNumY;						//!<
	bool			bDisp;							//!<
};
