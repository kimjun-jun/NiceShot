//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("data/TEXTURE/tutorialbg.png")			
#define	TUTORIAL_SIZE_X			(SCREEN_W/4)										// チュートリアルの幅
#define	TUTORIAL_SIZE_Y			(SCREEN_H/4)										// チュートリアルの高さ
#define	TUTORIAL_POS_X			(SCREEN_CENTER_X)									// チュートリアルの表示位置
#define	TUTORIAL_POS_Y			(SCREEN_CENTER_Y)									// チュートリアルの表示位置

#define TUTORIAL_GOUKEI			(4)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;					// テクス?ャへの?リゴン 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];		// 頂?情報格?ワ?ク
	D3DXVECTOR3				pos;								// ?リゴンの移動量
	int						cnt;
}TUTO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

