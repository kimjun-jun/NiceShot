/**
* @file result.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT_OK		_T("../data/TEXTURE/result/result_gameclear.png")			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_NO		_T("../data/TEXTURE/result/result_gameover.png")		// 読み込むテクスチャファイル名
#define	RESULT_OK_POS_X			(0)							// タイトルの表示位置
#define	RESULT_OK_POS_Y			(0)							// タイトルの表示位置
#define	RESULT_OK_SIZE_X		(SCREEN_W)						// タイトルの幅
#define	RESULT_OK_SIZE_Y		(SCREEN_H)						// タイトルの高さ
#define	RESULT_NO_POS_X			(0)						
#define	RESULT_NO_POS_Y			(0)						
#define	RESULT_NO_SIZE_X		(SCREEN_W)					
#define	RESULT_NO_SIZE_Y		(SCREEN_H)					

/**
*　@class RESULT
*　@brief 2Dポリゴンを定義するクラス
*/
class RESULT : public OBJECT_2D
{
public:
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

