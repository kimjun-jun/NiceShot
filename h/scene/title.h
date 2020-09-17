/**
* @file title.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


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
*　@class TITLECLASS
*　@brief 2Dポリゴンを定義する構造体
*/
class TITLECLASS :public TEXTURECLASS2D
{
public:
	int				nCountAppearStart = 0;				//!<
	float			fAlpha = 0.0f;						//!<
	int				nCountDisp = 0;						//!<
	bool			bDisp = false;						//!<
	int				nConutDemo = 0;						//!<
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
HRESULT ReInitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void SetTitleSelectTimeCHK(bool b);
bool GetTitleSelectTimeCHK(void);

/**
* @brief タイトル時の選択シーン取得関数 GetSelectTitleScene
* @return int　E_STAGEより	SCENE_TUTORIAL＝1チュートリアル　SCENE_GAME＝3ゲーム
* @details タイトルcppにある変数SelectTitleSceneをタイトルアップデート中に切り替え事で選択可能
*/
int GetSelectTitleScene(void);
