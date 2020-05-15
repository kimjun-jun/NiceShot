//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultok = NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureResultno = NULL;	// テクスチャへのポインタ

VERTEX_2D				g_vertexWkResultok[POLYGON_2D_VERTEX];			// 頂点情報格納ワーク
VERTEX_2D				g_vertexWkResultno[POLYGON_2D_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT_OK,				// ファイルの名前
			&g_pD3DTextureResultok);		// 読み込むメモリー

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_RESULT_NO,		// ファイルの名前
			&g_pD3DTextureResultno);	// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if (g_pD3DTextureResultok != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultok->Release();
		g_pD3DTextureResultok = NULL;
	}

	if (g_pD3DTextureResultno != NULL)
	{// テクスチャの開放
		g_pD3DTextureResultno->Release();
		g_pD3DTextureResultno = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
	{// Enter押したら、ステージを切り替える
		InitGame();				// ゲームの再初期化処理
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureResultok);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, g_vertexWkResultok, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(void)
{
	// 頂点座標の設定
	g_vertexWkResultok[0].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y, 0.0f);
	g_vertexWkResultok[1].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y, 0.0f);
	g_vertexWkResultok[2].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);
	g_vertexWkResultok[3].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultok[0].rhw =
		g_vertexWkResultok[1].rhw =
		g_vertexWkResultok[2].rhw =
		g_vertexWkResultok[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultok[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultok[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultok[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultok[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultok[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultok[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultok[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultok[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の設定
	g_vertexWkResultno[0].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y, 0.0f);
	g_vertexWkResultno[1].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y, 0.0f);
	g_vertexWkResultno[2].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);
	g_vertexWkResultno[3].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkResultno[0].rhw =
		g_vertexWkResultno[1].rhw =
		g_vertexWkResultno[2].rhw =
		g_vertexWkResultno[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkResultno[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultno[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultno[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultno[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkResultno[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultno[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultno[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultno[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

