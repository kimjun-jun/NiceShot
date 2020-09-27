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
enum FADE_TYPE
{
	FADE_NONE = 0,		//!< 何もない状態
	FADE_IN,			//!< フェードイン処理
	FADE_MUSIC_STOP,	//!< 音楽ストップ
	FADE_OUT,			//!< フェードアウト処理
	FADE_MAX			//!< マックス
};

#include "../../h/object/objectclass.h"


/**
*　@struct FADE
*　@brief 2Dポリゴンを定義する構造体
*/
class FADE : public OBJECT_2D
{
public:
	FADE() { color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); eFade = FADE_NONE; eScene = 0; sno = 0; }
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(GAME_OBJECT*obj);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	void								SetFade(FADE_TYPE fade, E_STAGE next, int sno);
	void								SetColor(D3DCOLOR col);

	D3DXCOLOR				color;								//!< カラー情報
	FADE_TYPE				eFade = FADE_IN;						//!< フェード番号
	int						eScene = SCENE_TITLE;					//!< 次に飛ぶ予定のScene
	int						sno = -1;								//!< サウンドナンバー

private:
	void MakeVertexFade(void);
};



