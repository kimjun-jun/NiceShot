/**
* @file fade.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once
#include "../../../h/Object/ObjectClass/objectclass.h"

/**
 * @enum FADE
 * フェードの状態
 */
enum FADE_TYPE
{
	FADE_NONE = 0,		//!< 何もない状態
	FADE_IN,			//!< フェードイン処理
	FADE_MUSIC_STOP,	//!< 音楽ストップ
	FADE_OUT,			//!< フェードアウト処理
	FADE_MAX			//!< マックス
};

/**
*　@class FADE_PARAMETER
*　@brief 
*/
class FADE_PARAMETER
{
public:
	FADE_PARAMETER() { color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); eFade = FADE_IN; eScene = SCENE_TITLE; sno = 0; }
	~FADE_PARAMETER() { }

	D3DXCOLOR	color;							//!< カラー情報
	FADE_TYPE	eFade;							//!< フェード番号
	int			sno;							//!< サウンドナンバー
	E_STAGE		eScene;							//!< 次に飛ぶ予定のScene
};

/**
*　@class FADE
*　@brief GAMEOBJECT派生クラス　シーン切り替え時のフェード効果　一定周期タイム後にシーンを切り替える
*/
class FADE : public GAME_OBJECT
{
public:
	FADE();		//!< データ読み込み　初期化
	~FADE();	//!< 削除

	void		Init(void);						//!< 初期化
	void		Update(GAME_OBJECT*obj);		//!< 更新
	void		Draw(void);						//!< 描画

	void		SetFade(FADE_TYPE fade, E_STAGE next, int sno);		//フェード効果セット

	FADE_PARAMETER	FadePara;					//!< マネージャーに必要なデータ群

private:

	VTXBuffer	vtx;							//!< 頂点情報　複数使用するならここを配列化
};
