/**
* @file damege.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_DAMEGE			("data/TEXTURE/screendamage.png")			
#define	DAMEGE_SIZE_X			(SCREEN_W/4)										// チュートリアルの幅
#define	DAMEGE_SIZE_Y			(SCREEN_H/4)										// チュートリアルの高さ
#define	DAMEGE_POS_X			(SCREEN_CENTER_X)									// チュートリアルの表示位置
#define	DAMEGE_POS_Y			(SCREEN_CENTER_Y)									// チュートリアルの表示位置

#define DAMEGE_GOUKEI			(4)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;					// テクス?ャへの?リゴン 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];		// 頂?情報格?ワ?ク
	D3DXVECTOR3				pos;								// ?リゴンの移動量
	int						alpha;
	float					time;
	bool					use;
}DAMEGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDamege(int type);
void UninitDamege(void);
void UpdateDamege(void);
void DrawDamege(void);
DAMEGE *GetDamege(void);

