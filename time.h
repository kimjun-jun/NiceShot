//=============================================================================
//
// タイムの処理 [time.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once


// マクロ定義
#define TEXTURE_GAME_DOT	"data/TEXTURE/BG/dot.png"	
#define TEXTURE_GAME_SCORE	"data/TEXTURE/BG/0-9.png"
#define TEXTURE_GAME_TIMELOGO	"data/TEXTURE/sozai/TIME.png" 
#define TEXTURE_TIME_SIZE_X	(30) // テクスチャサイズ 
#define TEXTURE_TIME_SIZE_Y	(30) // 同上  
#define SCORE_TEXTURE_PATTERN_DIVIDE_X	(10)	// アニメパターンのテクスチャ内分割数（X)  
#define SCORE_TEXTURE_PATTERN_DIVIDE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)  
#define SCORE_ANIM_PATTERN_NUM			(SCORE_TEXTURE_PATTERN_DIVIDE_X*SCORE_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数  
#define SCORE_TIME_ANIMATION			(4)	// アニメーションの切り替わるカウント  
#define TEXTURE_DOT_SIZE_X	(4) // テクスチャサイズ 
#define TEXTURE_DOT_SIZE_Y	(4) // 同上  


#define DOT_GOUKEI					(2)
#define TIME_DIGIT					(3)
#define FPS_TIME_COUNT				(4200) //毎フレームカウントしてタイム計測


#define TEXTURE_TIMELOGO_SIZE_X	(35) // テクスチャサイズ 
#define TEXTURE_TIMELOGO_SIZE_Y	(30) // 同上  

#define TEXTURE_TIME_POS_X	(SCREEN_W-30) // テクスチャサイズ 
#define TEXTURE_TIME_POS_Y	(25) // 同上  


#define TEXTURE_TIMELOGO_POS_X	(TEXTURE_TIME_POS_X-110) // テクスチャサイズ 
#define TEXTURE_TIMELOGO_POS_Y	(TEXTURE_TIME_POS_Y) // 同上  

#define NUM_INTERVAL_X	(float(TEXTURE_TIME_SIZE_X)) // 同上  

//*****************************************************************************
//* 構造体定義
//******************************************************************************* /
typedef struct // 構造体
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;

	VERTEX_2D				texture[POLYGON_2D_VERTEX];

	D3DXVECTOR3				pos;
	D3DXVECTOR3				rot;
	int						nCountAnim;
	int						nPatternAnim;
	bool					use;
} TIME;

typedef struct // 構造体
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;

	VERTEX_2D				texture[POLYGON_2D_VERTEX];

	D3DXVECTOR3				pos;
	int						nCountAnim;
	int						nPatternAnim;
} DOT;

typedef struct // 構造体
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;
	VERTEX_2D				texture[POLYGON_2D_VERTEX];
	D3DXVECTOR3				pos;
} TIMELOGO;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTime(int type);
void ReInitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
int *GetTimemaneger(void);
void AddTime(int val);

