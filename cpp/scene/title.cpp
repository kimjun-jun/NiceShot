/**
* @file title.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/scene/title.h"
#include "../../h/other/input.h"
#include "../../h/scene/fade.h"
#include "../../h/other/sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE		"../data/TEXTURE/bg000.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO	"../data/TEXTURE/title_logo.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START	"../data/TEXTURE/press_enter.png"	// 読み込むテクスチャファイル名

#define	TITLE_LOGO_WIDTH		(150)						// タイトルロゴの幅
#define	TITLE_LOGO_HEIGHT		(100)						// タイトルロゴの高さ

#define	TITLE_LOGO_POS_X		(SCREEN_CENTER_X)			// タイトルロゴの位置(X座標)
#define	TITLE_LOGO_POS_Y		(SCREEN_CENTER_Y-TITLE_LOGO_HEIGHT)			// タイトルロゴの位置(Y座標)

#define	START_POS_X				(TITLE_LOGO_POS_X)			// スタートボタンの位置(X座標)
#define	START_POS_Y				(TITLE_LOGO_POS_Y+200)		// スタートボタンの位置(Y座標)
#define	START_WIDTH				(100)						// スタートボタンの幅
#define	START_HEIGHT			(100)						// スタートボタンの高さ

#define	COUNT_APPERA_START		(10)						// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(30)						// スタートボタン点滅の時間

#define	COUNT_WAIT_DEMO			(60 * 5)					// デモまでの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief 頂点生成関数 MakeVertexTitle
* @return HRESULT
*/
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);

/**
* @brief 頂点カラー設定関数 SetColorTitleLogo
*/
void SetColorTitleLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		//!< テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		//!< 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	//!< テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	//!< 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		//!< テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		//!< 頂点バッファインターフェースへのポインタ
int						g_nCountAppearStart = 0;		//!< スタートロゴ点滅用カウントアッパー
float					g_fAlphaLogo = 0.0f;			//!< スタートロゴのアルファ値
int						g_nCountDisp = 0;				//!< スタートロゴ点滅させる時間
bool					g_bDispStart = false;			//!< スタートロゴ描画判定
int						g_nConutDemo = 0;				//!< タイトル時一定時間無操作だとデモリール再生。今回は未使用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearStart = 0;
	g_fAlphaLogo = 0.0f;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_nConutDemo = 0;

	// 頂点情報の作成
	MakeVertexTitle(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE,				// ファイルの名前
		&g_pD3DTextureTitle);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE_LOGO,			// ファイルの名前
		&g_pD3DTextureTitleLogo);	// 読み込むメモリー


	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_LOGO_START,			// ファイルの名前
		&g_pD3DTextureStart);		// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_pD3DTextureTitle != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if (g_pD3DVtxBuffTitle != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTitle->Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	if (g_pD3DTextureTitleLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitleLogo->Release();
		g_pD3DTextureTitleLogo = NULL;
	}

	if (g_pD3DVtxBuffTitleLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTitleLogo->Release();
		g_pD3DVtxBuffTitleLogo = NULL;
	}

	if (g_pD3DTextureStart != NULL)
	{// テクスチャの開放
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if (g_pD3DVtxBuffStart != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
#if 0
	if (g_nCountAppearStart >= COUNT_APPERA_START)
	{
		g_nConutDemo++;
		if (g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
#endif

	if (g_fAlphaLogo < 1.0f)
	{
		g_fAlphaLogo += 0.05f;
		if (g_fAlphaLogo >= 1.0f)
		{
			g_fAlphaLogo = 1.0f;
		}
		SetColorTitleLogo();
	}
	else
	{
		g_nCountAppearStart++;
		if (g_nCountAppearStart > COUNT_APPERA_START)
		{
			g_nCountDisp = (g_nCountDisp + 1) % 80;
			if (g_nCountDisp > INTERVAL_DISP_START)
			{
				g_bDispStart = false;
			}
			else
			{
				g_bDispStart = true;
			}
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A) || IsButtonTriggered(1, BUTTON_A) || IsButtonTriggered(2, BUTTON_A) || IsButtonTriggered(3, BUTTON_A))
	{
		if (g_nCountAppearStart == 0)
		{// タイトル登場スキップ
			g_fAlphaLogo = 1.0f;
			SetColorTitleLogo();

			g_nCountAppearStart = COUNT_APPERA_START;
		}
		else
		{// ゲームへ
			PlaySound(SOUND_LABEL_SE_enter02);
			SetFade(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitle);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);


	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);

	if (g_bDispStart == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureStart);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffTitle,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_H, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTitle->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffTitleLogo,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X - TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y - TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y - TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X - TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTitleLogo->Unlock();
	}


	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffStart,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(START_POS_X - START_WIDTH, START_POS_Y - START_HEIGHT, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y - START_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(START_POS_X - START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffStart->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorTitleLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTitleLogo->Unlock();
	}

}

