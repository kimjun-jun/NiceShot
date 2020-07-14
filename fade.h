/**
* @file fade.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
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
* @param[in] FADE fade フェードの種類 enum FADE参照
* @param[in] E_STAGE next 次のゲームシーン　enum E_STAGE参照
* @param[in] int sno 流したい音楽 enum SOUND_TRACK参照
*/
void SetFade(FADE fade, E_STAGE next, int sno);

/**
* @brief フェード取得関数 GetFade
* @return FADE
* @details 現在のフェード状態を取得できる
*/
FADE GetFade(void);

