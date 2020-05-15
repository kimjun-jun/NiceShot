//=============================================================================
//
// ランク処理 [rank.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RANK2		("data/TEXTURE/2i.png")			
#define	TEXTURE_RANK3		("data/TEXTURE/3i.png")			
#define	TEXTURE_RANK4		("data/TEXTURE/4i.png")		
#define	RANK_SIZE_X			(SCREEN_W/4)										// チュートリアルの幅
#define	RANK_SIZE_Y			(SCREEN_H/4)										// チュートリアルの高さ
#define	RANK_POS_X			(SCREEN_W)										// チュートリアルの表示位置
#define	RANK_POS_Y			(SCREEN_H)										// チュートリアルの表示位置

#define RANK_GOUKEI			(3)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;					// テクス?ャへの?リゴン 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];		// 頂?情報格?ワ?ク
	D3DXVECTOR3				pos;								// ?リゴンの移動量
	bool					use;
}RANK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRank(int type);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);
RANK *GetRank(void);
void SetRank(int PlayerNum);

