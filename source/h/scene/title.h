/**
* @file title.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
 * @enum E_STAGE
 * 画面遷移定数
 */
enum TITLE_SELECT_SCENE//列挙型。defineの番号を自動で割り当ててくれる。
{
	TITLE_SELECT_SCENE_TUTORIAL,					//!< 0チュートリアル
	TITLE_SELECT_SCENE_GAME,						//!< 1ゲーム
	TITLE_SELECT_SCENE_MAX
};


/**
*　@class TITLECLASS 0背景 1スタートロゴ 2練習 3試合
*　@brief 2Dポリゴンを定義する構造体
*/
class TITLE :public OBJECT_2D_VERTEXBUFFER
{
public:
	TITLE() { fAlpha = 0.0f, nCountAppearStart = 0, nCountDisp = 0, nConutDemo = 0,
		TitleSelectSceneNum = 0, bDisp = false, TitleSelectTime = false; };
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(void);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	HRESULT						MakeVertexTitle(void);
	void						SetColorTitle(int CntTitle, float alpha);

	float			fAlpha;							//!<
	int				nCountAppearStart;				//!<
	int				nCountDisp;						//!<
	int				nConutDemo;						//!<
	int				TitleSelectSceneNum;			//!<
	bool			bDisp;							//!<
	bool			TitleSelectTime;				//!<
};
