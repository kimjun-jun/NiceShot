/**
* @file fade.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/sound.h"
#include "../../h/other/fade.h"
#include "../../h/object/objectclass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FADE_RATE		(0.02f)									//!< フェード係数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

/**
* @brief 頂点生成関数 MakeVertexFade
* @return HRESULT
*/
HRESULT MakeVertexFade(void);

/**
* @brief フェードカラー設定関数 SetColor
* @param[in] D3DCOLOR col カラー値　画面がフェードする色を設定
*/
void SetColor(D3DCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_p3DTextureFade = NULL;				//!< テクスチャへのポインタ
static VERTEX_2D				g_vertexWkFade[POLYGON_2D_VERTEX];		//!< 頂点情報格納ワーク
static D3DXCOLOR				g_color;								//!< カラー情報
static FADE						g_eFade = FADE_IN;						//!< フェード番号
static int						g_eScene = SCENE_TITLE;					//!< 次に飛ぶ予定のScene
static int						g_sno = -1;								//!< サウンドナンバー

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の設定
	MakeVertexFade();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	if (g_p3DTextureFade != NULL)
	{// テクスチャの開放
		g_p3DTextureFade->Release();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	if (g_eFade != FADE_NONE)
	{// フェード処理中
		if (g_eFade == FADE_OUT)
		{// フェードアウト処理
			g_color.a += FADE_RATE;		// α値を加算して画面を消していく
			if (g_color.a >= 1.0f)
			{
				// 状態を切り替え
				SetScene(g_eScene);

				// フェードイン処理に切り替え
				g_color.a = 1.0f;
				g_eFade = FADE_MUSIC_STOP;

				// BGM停止
				StopSound();
			}

			// 色を設定
			SetColor(g_color);
		}
		else if (g_eFade == FADE_MUSIC_STOP)
		{
			// BGM再生
			if (g_sno > -1)
			{
				PlaySound(g_sno);
			}

			g_eFade = FADE_IN;
		}
		else if (g_eFade == FADE_IN)
		{// フェードイン処理
			g_color.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
			if (g_color.a <= 0.0f)
			{
				// フェード処理終了
				g_color.a = 0.0f;
				g_eFade = FADE_NONE;

			}

			// 色を設定
			SetColor(g_color);
		}
	}
}

//=============================================================================
// フェード画面
//=============================================================================
void DrawFade()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, g_vertexWkFade, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャのパースペクティブコレクト用
	g_vertexWkFade[0].rhw =
		g_vertexWkFade[1].rhw =
		g_vertexWkFade[2].rhw =
		g_vertexWkFade[3].rhw = 1.0f;

	// 反射光の設定
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_vertexWkFade[0].diffuse = g_color;
	g_vertexWkFade[1].diffuse = g_color;
	g_vertexWkFade[2].diffuse = g_color;
	g_vertexWkFade[3].diffuse = g_color;

	// 頂点座標の設定
	g_vertexWkFade[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vertexWkFade[1].vtx = D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f);
	g_vertexWkFade[2].vtx = D3DXVECTOR3(0.0f, SCREEN_H, 0.0f);
	g_vertexWkFade[3].vtx = D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f);

	// テクスチャ座標の設定
	g_vertexWkFade[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkFade[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkFade[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkFade[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 色を設定
//=============================================================================
void SetColor(D3DCOLOR col)
{
	// 反射光の設定
	g_vertexWkFade[0].diffuse = col;
	g_vertexWkFade[1].diffuse = col;
	g_vertexWkFade[2].diffuse = col;
	g_vertexWkFade[3].diffuse = col;
}

//=============================================================================
// フェードの状態設定
//=============================================================================
void SetFade(FADE fade, E_STAGE next, int sno)
{
	g_eFade = fade;
	g_eScene = next;
	g_sno = sno;

}

//=============================================================================
// フェードの状態取得
//=============================================================================
FADE GetFade(void)
{
	return g_eFade;
}
