/**
* @file tutorial.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("data/TEXTURE/tutorialbg.png")						//!< 読み込むテクスチャファイル名
#define	TUTORIAL_SIZE_X			(SCREEN_W/4-50)										//!< チュートリアルの幅
#define	TUTORIAL_SIZE_Y			(SCREEN_H/4-50)										//!< チュートリアルの高さ
#define	TUTORIAL_POS_X			(SCREEN_CENTER_X)									//!< チュートリアルの表示位置
#define	TUTORIAL_POS_Y			(SCREEN_CENTER_Y)									//!< チュートリアルの表示位置
#define TUTORIAL_GOUKEI			(4)													//!< チュートリアル構造体の合計

/**
*　@struct TUTO
*　@brief 2Dポリゴンを定義する構造体
*/
typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;										//!< テクスチャへのポインタ
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];							//!< 頂点情報格納ワーク
	D3DXVECTOR3				pos;													//!< ポリゴンの座標
	int						cnt;													//!< 描画判定用　2で割った余り判定　0表示　1非表示　今後のテクスチャ情報が増えることを加味してint定義
}TUTO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief Tutorial初期化関数 InitTutorial
* @param[in] type 初期化タイプ　INIT_TIME利用。0:初回　0以外: 2回目以降
* @return HRESULT
*/
HRESULT InitTutorial(int type);

/**
* @brief Tutorial開放関数 UninitTutorial
*/
void UninitTutorial(void);

/**
* @brief Tutorial更新関数 UpdateTutorial
*/
void UpdateTutorial(void);

/**
* @brief Tutorial描画関数 DrawTutorial
*/
void DrawTutorial(void);

