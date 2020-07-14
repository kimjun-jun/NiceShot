/**
* @file status.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	STATUS_SIZE_X			(20.0f)							// ステータスの幅
#define	STATUS_SIZE_Y			(20.0f)							// ステータスの高さ
#define	STATUS_POS_X			(SCREEN_CENTER_X)				// ステータスの表示位置
#define	STATUS_POS_Y			(SCREEN_CENTER_Y)				// ステータスの表示位置
#define	STATUS_POS_X_OFFSET		(180.0f)						// ステータスの表示位置オフセット
#define	STATUS_POS_Y_OFFSET		(120.0f)						// ステータスの表示位置オフセット

#define STATUS_MAX			(4)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;					// テクス?ャへの?リゴン 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];		// 頂?情報格?ワ?ク
	D3DXVECTOR3				pos;								// ?リゴンの移動量
}STATUS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStatus(int type);
void UninitStatus(void);
void UpdateStatus(void);
void DrawStatus(void);
