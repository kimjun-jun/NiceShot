//=============================================================================
//
// カウントダウン処理 [countdown.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once


// マクロ定義
#define TEXTURE_GAME_COUNTDOWNGO	_T("data/TEXTURE/sozai/go.png")	
#define TEXTURE_COUNTDOWN_SIZE_X	(50) // テクスチャサイズ 
#define TEXTURE_COUNTDOWN_SIZE_Y	(50) // 同上  
#define TEXTURE_COUNTDOWNGO_SIZE_X	(200) // テクスチャサイズ 
#define TEXTURE_COUNTDOWNGO_SIZE_Y	(200) // 同上  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X	(10)	// アニメパターンのテクスチャ内分割数（X)  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)  
#define COUNTDOWN_ANIM_PATTERN_NUM			(COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X*COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数  
#define COUNTDOWN_TIME_ANIMATION			(4)	// アニメーションの切り替わるカウント  
#define TEXTURE_COUNTDOWN_POS_X	(SCREEN_CENTER_X) // テクスチャサイズ
#define TEXTURE_COUNTDOWN_POS_Y	(SCREEN_CENTER_Y) // 同上  
#define COUNTDOWN_GOUKEI				(2) //スコアの合計
#define FPS_TIME_COUNTDOWN				(240) //毎フレームカウントしてタイム計測
#define COUNTDOWN_A						(10) //カウントダウンが0の時GOを描画してGOが薄くなるスピード
#define COUNTDOWN_CHANGESIZE			(5) //カウントダウンが0の時GOを描画してGOが薄くなるスピード

//*****************************************************************************
//* 構造体定義
//******************************************************************************* /
typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;
	VERTEX_2D				texture[POLYGON_2D_VERTEX];
	D3DXVECTOR3				pos;
	int						nCountAnim;
	int						nPatternAnim;
	int						changeval;
	int						color;
	bool					use;
	bool					signal;
} COUNTDOWN;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCountdown(int type);
void ReInitCountdown(void);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
COUNTDOWN *GetCountdown(void);
void AddCountdown(int val);

