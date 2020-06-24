/**
* @file fade.h
* @brief HitBallTarget(2D)的当てゲーム
* @author キムラジュン
* @date 2019/09/01
*/
#pragma once


/**
 * @enum FADE
 * フェードの状態
 */
enum FADE
{
	FADE_NONE = 0,		//!< 何もない状態
	FADE_IN,			//!< フェードイン処理
	FADE_MUSIC_STOP,	//!< 音楽ストップ
	FADE_OUT,			//!< フェードアウト処理
	FADE_MAX			//!< マックス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

/**
* @brief フェード初期化関数 InitFade
* @return HRESULT
*/
HRESULT InitFade(void);

/**
* @brief フェードメモリ開放関数 UninitFade
*/
void UninitFade(void);

/**
* @brief フェード更新関数 UpdateFade
* @details ゲームシーン切り替え時(SetFade()を使用するとき)に実行される。
*/
void UpdateFade(void);

/**
* @brief フェード描画関数 DrawFade
*/
void DrawFade(void);

/**
* @brief フェード設定関数 SetFade
* @param[in] FADE fade フェードの種類, E_STAGE next 次のゲームシーン, int sno 流したい音楽
*/
void SetFade(FADE fade, E_STAGE next, int sno);

/**
* @brief フェード取得関数 GetFade
* @return FADE
* @details 現在のフェード状態を取得できる
*/
FADE GetFade(void);

