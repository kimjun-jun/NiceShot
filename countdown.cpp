/**
* @file countdown.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "countdown.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief 頂点生成関数 MakeVertexCountdown
* @return HRESULT
*/
HRESULT MakeVertexCountdown(void);
/**
* @brief テクスチャ設定関数 SetTextureCountdown
*/
void SetTextureCountdown(void);
/**
* @brief 頂点設定関数 SetVertexCountdown
*/
void SetVertexCountdown(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
COUNTDOWN g_countdown[COUNTDOWN_GOUKEI];					//!< 0カウントダウン値,1スタートLOGO	構造体変数
static int g_countdown_maneger;								//!< カウントダウン数値	→毎フレーム減算される
static int g_fps_maneger;									//!< カウントダウン減算値	→[3,2,1]GO

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCountdown(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)	// 初回のみ読み込む
	{
		// テクスチャの読み込み  
		D3DXCreateTextureFromFile(pDevice,				// デバイスのポインタ
			TEXTURE_GAME_SCORE,							// ファイルの名前
			&g_countdown[0].pD3DTexture);				// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,	
			TEXTURE_GAME_COUNTDOWNGO,		
			&g_countdown[1].pD3DTexture);	
	}


	for (int i = 0; i < COUNTDOWN_GOUKEI; i++)
	{
		COUNTDOWN *countdown = &g_countdown[i];
		countdown->pos = D3DXVECTOR3(TEXTURE_COUNTDOWN_POS_X, TEXTURE_COUNTDOWN_POS_Y, 0.0f);
		countdown->nCountAnim = 0;
		countdown->nPatternAnim = 0;
		countdown->changeval = 0;
	}
	g_countdown[0].use = true;
	g_countdown[1].use = false;
	g_countdown[0].signal = false;
	g_countdown[1].color = 255;
	g_countdown_maneger = FPS_TIME_COUNTDOWN;
	g_fps_maneger = 4;
	MakeVertexCountdown();
	return S_OK;
}

//=============================================================================
// 再初期化処理
//=============================================================================
void ReInitCountdown(void)
{

	for (int i = 0; i < COUNTDOWN_GOUKEI; i++)
	{
		COUNTDOWN *countdown = &g_countdown[i];
		countdown->pos = D3DXVECTOR3(TEXTURE_COUNTDOWN_POS_X, TEXTURE_COUNTDOWN_POS_Y, 0.0f);
		countdown->nCountAnim = 0;
		countdown->nPatternAnim = 0;
		countdown->changeval = 0;
	}
	g_countdown[0].use = true;
	g_countdown[1].use = false;
	g_countdown[0].signal = false;
	g_countdown[1].color = 255;
	g_fps_maneger = 4;
	g_countdown_maneger = FPS_TIME_COUNTDOWN;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitCountdown(void)
{
	for (int i = 0; i < COUNTDOWN_GOUKEI; i++)
	{
		COUNTDOWN *countdown = &g_countdown[i];
		if (countdown->pD3DTexture != NULL)
		{	// テクスチャの開放
			countdown->pD3DTexture->Release();
			countdown->pD3DTexture = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCountdown(void)
{
	SetTextureCountdown();
	SetVertexCountdown();
	StopSound();
	MasterVolumeChange(1);
	if (g_countdown[0].signal == true)
	{
		SetScene(SCENE_GAME);
		PlaySound(SOUND_LABEL_BGM_normal01);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int i = 0; i < COUNTDOWN_GOUKEI; i++)
	{
		COUNTDOWN *countdown = &g_countdown[i];
		if (countdown->use == true )
		{
			// テクスチャの設定  
			pDevice->SetTexture(0, countdown->pD3DTexture);
			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, countdown->texture, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点座標の設定
	SetVertexCountdown();
	for (int i = 0; i < COUNTDOWN_GOUKEI; i++)
	{
		// rhwの設定
		g_countdown[i].texture[0].rhw =
			g_countdown[i].texture[1].rhw =
			g_countdown[i].texture[2].rhw =
			g_countdown[i].texture[3].rhw = 1.0f;
		// 反射設定  [i]
		g_countdown[i].texture[0].diffuse = D3DCOLOR_RGBA(255,255,255,255);
		g_countdown[i].texture[1].diffuse = D3DCOLOR_RGBA(255,255,255,255);
		g_countdown[i].texture[2].diffuse = D3DCOLOR_RGBA(255,255,255,255);
		g_countdown[i].texture[3].diffuse = D3DCOLOR_RGBA(255,255,255,255);
	}
	// テクャ座標[i]の設定  
	g_countdown[0].texture[0].tex = D3DXVECTOR2(0.3f, 0.0f);
	g_countdown[0].texture[1].tex = D3DXVECTOR2(0.4f, 0.0f);
	g_countdown[0].texture[2].tex = D3DXVECTOR2(0.3f, 1.0f);
	g_countdown[0].texture[3].tex = D3DXVECTOR2(0.4f, 1.0f);

	g_countdown[1].texture[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_countdown[1].texture[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_countdown[1].texture[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_countdown[1].texture[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureCountdown(void)
{
	int val = g_countdown_maneger;
	if (val <= 0)
	{
		g_countdown[0].use = false;
		g_countdown[1].use = true;
	}
	else g_countdown[1].use = false;
	if (val % (FPS_TIME_COUNTDOWN/3) == 0)
	{
		g_fps_maneger -= 1;
		// テクスチャ座標の設定
		g_countdown[0].texture[0].tex = D3DXVECTOR2(0.1f*g_fps_maneger, 0.0f);
		g_countdown[0].texture[1].tex = D3DXVECTOR2(0.1f*g_fps_maneger+0.1f, 0.0f);
		g_countdown[0].texture[2].tex = D3DXVECTOR2(0.1f*g_fps_maneger, 1.0f);
		g_countdown[0].texture[3].tex = D3DXVECTOR2(0.1f*g_fps_maneger+0.1f, 1.0f);
		g_countdown[0].changeval = 0;
	}
	if (g_countdown[1].use == true)
	{
		g_countdown[1].color -= COUNTDOWN_A;
		g_countdown[1].texture[0].diffuse = D3DCOLOR_RGBA(255, 255, 255,g_countdown[1].color);
		g_countdown[1].texture[1].diffuse = D3DCOLOR_RGBA(255, 255, 255,g_countdown[1].color);
		g_countdown[1].texture[2].diffuse = D3DCOLOR_RGBA(255, 255, 255,g_countdown[1].color);
		g_countdown[1].texture[3].diffuse = D3DCOLOR_RGBA(255, 255, 255,g_countdown[1].color);
		if (g_countdown[1].color <= 0) g_countdown[0].signal = true;
	}
	g_countdown[0].changeval += COUNTDOWN_CHANGESIZE;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexCountdown(void)
{
	// 頂点座標の設定 
	g_countdown[0].texture[0].vtx.x = g_countdown[0].pos.x - TEXTURE_COUNTDOWN_SIZE_X - g_countdown[0].changeval;
	g_countdown[0].texture[0].vtx.y = g_countdown[0].pos.y - TEXTURE_COUNTDOWN_SIZE_Y - g_countdown[0].changeval;
	g_countdown[0].texture[0].vtx.z = 0.0f;
	g_countdown[0].texture[1].vtx.x = g_countdown[0].pos.x + TEXTURE_COUNTDOWN_SIZE_X + g_countdown[0].changeval;
	g_countdown[0].texture[1].vtx.y = g_countdown[0].pos.y - TEXTURE_COUNTDOWN_SIZE_Y - g_countdown[0].changeval;
	g_countdown[0].texture[1].vtx.z = 0.0f;
	g_countdown[0].texture[2].vtx.x = g_countdown[0].pos.x - TEXTURE_COUNTDOWN_SIZE_X - g_countdown[0].changeval;
	g_countdown[0].texture[2].vtx.y = g_countdown[0].pos.y + TEXTURE_COUNTDOWN_SIZE_Y + g_countdown[0].changeval;
	g_countdown[0].texture[2].vtx.z = 0.0f;
	g_countdown[0].texture[3].vtx.x = g_countdown[0].pos.x + TEXTURE_COUNTDOWN_SIZE_X + g_countdown[0].changeval;
	g_countdown[0].texture[3].vtx.y = g_countdown[0].pos.y + TEXTURE_COUNTDOWN_SIZE_Y + g_countdown[0].changeval;
	g_countdown[0].texture[3].vtx.z = 0.0f;

	g_countdown[1].texture[0].vtx.x = g_countdown[1].pos.x - TEXTURE_COUNTDOWNGO_SIZE_X;
	g_countdown[1].texture[0].vtx.y = g_countdown[1].pos.y - TEXTURE_COUNTDOWNGO_SIZE_Y;
	g_countdown[1].texture[0].vtx.z = 0.0f;		  
	g_countdown[1].texture[1].vtx.x = g_countdown[1].pos.x + TEXTURE_COUNTDOWNGO_SIZE_X;
	g_countdown[1].texture[1].vtx.y = g_countdown[1].pos.y - TEXTURE_COUNTDOWNGO_SIZE_Y;
	g_countdown[1].texture[1].vtx.z = 0.0f;		  
	g_countdown[1].texture[2].vtx.x = g_countdown[1].pos.x - TEXTURE_COUNTDOWNGO_SIZE_X;
	g_countdown[1].texture[2].vtx.y = g_countdown[1].pos.y + TEXTURE_COUNTDOWNGO_SIZE_Y;
	g_countdown[1].texture[2].vtx.z = 0.0f;		  
	g_countdown[1].texture[3].vtx.x = g_countdown[1].pos.x + TEXTURE_COUNTDOWNGO_SIZE_X;
	g_countdown[1].texture[3].vtx.y = g_countdown[1].pos.y + TEXTURE_COUNTDOWNGO_SIZE_Y;
	g_countdown[1].texture[3].vtx.z = 0.0f;
}

//=============================================================================
// スコアのアドレスを返す
//=============================================================================
COUNTDOWN *GetCountdown(void)
{
	return &g_countdown[0];
}

//=============================================================================
// カウントダウン加算処理
//=============================================================================
void AddCountdown(int val)
{
	g_countdown_maneger += val;
}
