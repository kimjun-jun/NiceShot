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
#define	TEXTURE_TITLE_BG				"../data/TEXTURE/title.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START				"../data/TEXTURE/press_enter.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_TUTORIAL	"../data/TEXTURE/rensyurogo.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_GAME		"../data/TEXTURE/taisenrogo.jpg"	// 読み込むテクスチャファイル名


#define	TITLE_BG_WIDTH		(SCREEN_W/2)						// タイトルBGの幅
#define	TITLE_BG_HEIGHT		(SCREEN_H/2)						// タイトルBGの高さ
#define	TITLE_BG_POS_X		(SCREEN_CENTER_X)					// タイトルBGの位置(X座標)
#define	TITLE_BG_POS_Y		(SCREEN_CENTER_Y)					// タイトルBGの位置(Y座標)

#define	TITLE_START_LOGO_POS_X				(TITLE_BG_POS_X)			// スタートロゴの位置(X座標)
#define	TITLE_START_LOGO_POS_Y				(TITLE_BG_POS_Y+200)		// スタートロゴの位置(Y座標)
#define	TITLE_START_LOGO_WIDTH				(100)						// スタートロゴの幅
#define	TITLE_START_LOGO_HEIGHT				(100)						// スタートロゴの高さ

#define	TITLE_SELECT_POS_X				(TITLE_BG_POS_X+200.0f)		// セレクトテクスチャの位置(X座標)
#define	TITLE_SELECT_POS_Y				(TITLE_BG_POS_Y-100.0f)		// セレクトテクスチャの位置(Y座標)
#define	TITLE_SELECT_WIDTH				(150)						// セレクトテクスチャの幅
#define	TITLE_SELECT_HEIGHT				(150)						// セレクトテクスチャの高さ

#define	COUNT_APPERA_START		(10)						// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(30)						// スタートボタン点滅の時間
#define	COUNT_WAIT_DEMO			(60 * 5)					// デモまでの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
void SetColorTitle(TITLECLASS *SetTitle);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
TITLECLASS g_TitleBG;
TITLECLASS g_TitleStartLogo;
TITLECLASS g_TitleSelectScene[TITLE_SELECT_SCENE_MAX];

int			TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;	//!<
bool		TitleSelectTime = false;								//!< true=タイトルシーンでセレクト画面になっている　false=タイトルシーンで点滅画面になっている
bool		TitleSelectTimeCHK = false;								//!< true=次のシーンへ行って良い　false=タイトルシーンで点滅画面になっている　これをgamescneでシーンチェックの時に判定している
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
	TitleSelectTime = false;
	TitleSelectTimeCHK = false;

	g_TitleStartLogo.nCountAppearStart = 0;
	g_TitleStartLogo.fAlpha = 0.0f;
	g_TitleStartLogo.nCountDisp = 0;
	g_TitleStartLogo.bDisp = false;
	g_TitleStartLogo.nConutDemo = 0;

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		g_TitleSelectScene[CntTitleSelectScene].bDisp = false;
		g_TitleSelectScene[CntTitleSelectScene].fAlpha = 0.0f;
	}
	// 頂点情報の作成
	MakeVertexTitle(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE_BG,			// ファイルの名前
		&g_TitleBG.pD3DTexture);	// 読み込むメモリー


	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_LOGO_START,			// ファイルの名前
		&g_TitleStartLogo.pD3DTexture);		// 読み込むメモリー


	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE_SELECT_TUTORIAL,			// ファイルの名前
		&g_TitleSelectScene[TITLE_SELECT_SCENE_TUTORIAL].pD3DTexture);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_TITLE_SELECT_GAME,			// ファイルの名前
		&g_TitleSelectScene[TITLE_SELECT_SCENE_GAME].pD3DTexture);		// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 再初期化処理
//=============================================================================
HRESULT ReInitTitle(void)
{
	TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
	TitleSelectTime = false;
	TitleSelectTimeCHK = false;
	g_TitleBG.fAlpha = 1.0f;

	g_TitleStartLogo.nCountAppearStart = 0;
	g_TitleStartLogo.fAlpha = 0.0f;
	g_TitleStartLogo.nCountDisp = 0;
	g_TitleStartLogo.bDisp = false;
	g_TitleStartLogo.nConutDemo = 0;

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		g_TitleSelectScene[CntTitleSelectScene].bDisp = false;
		g_TitleSelectScene[CntTitleSelectScene].fAlpha = 0.0f;
	}
	SetColorTitle(&g_TitleBG);
	SetColorTitle(&g_TitleStartLogo);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if (g_TitleBG.pD3DTexture != NULL)
	{// テクスチャの開放
		g_TitleBG.pD3DTexture->Release();
		g_TitleBG.pD3DTexture = NULL;
	}

	if (g_TitleBG.pD3DVtxBuff != NULL)
	{// 頂点バッファの開放
		g_TitleBG.pD3DVtxBuff->Release();
		g_TitleBG.pD3DVtxBuff = NULL;
	}

	if (g_TitleStartLogo.pD3DTexture != NULL)
	{// テクスチャの開放
		g_TitleStartLogo.pD3DTexture->Release();
		g_TitleStartLogo.pD3DTexture = NULL;
	}

	if (g_TitleStartLogo.pD3DVtxBuff != NULL)
	{// 頂点バッファの開放
		g_TitleStartLogo.pD3DVtxBuff->Release();
		g_TitleStartLogo.pD3DVtxBuff = NULL;
	}

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (g_TitleSelectScene[CntTitleSelectScene].pD3DTexture != NULL)
		{// テクスチャの開放
			g_TitleSelectScene[CntTitleSelectScene].pD3DTexture->Release();
			g_TitleSelectScene[CntTitleSelectScene].pD3DTexture = NULL;
		}

		if (g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff != NULL)
		{// 頂点バッファの開放
			g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff->Release();
			g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	//タイトル画面(セレクト中)
	if (TitleSelectTime == true)
	{
		//決定処理
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			if(TitleSelectSceneNum == TITLE_SELECT_SCENE_TUTORIAL) SetFade(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
			else if (TitleSelectSceneNum == TITLE_SELECT_SCENE_GAME) SetFade(FADE_OUT, SCENE_GAMECOUNTDOWN, SOUND_LABEL_BGM_normal01);
		}
		//セレクト切り替え
		else if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			g_TitleBG.fAlpha = 1.0f;
			SetColorTitle(&g_TitleBG);
			g_TitleStartLogo.fAlpha = 0.0f;
			SetColorTitle(&g_TitleStartLogo);
			TitleSelectTime = false;
			g_TitleSelectScene[0].bDisp = false;
			g_TitleSelectScene[1].bDisp = false;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DIGITAL_DOWN))
		{
			////選択肢が二つだとこっち
			//{
			//	TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
			//}
			//3つ以上選択肢があるとこっち使う
			{
				if (TitleSelectSceneNum != TITLE_SELECT_SCENE_GAME) PlaySound(SOUND_LABEL_SE_enter02);
				TitleSelectSceneNum++;
				if (TitleSelectSceneNum > TITLE_SELECT_SCENE_GAME) TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
			}
		}
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_DIGITAL_UP))
		{
			////選択肢が二つだとこっち
			//{
			//	TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			//}
			//3つ以上選択肢があるとこっち使う
			{
				if (TitleSelectSceneNum != TITLE_SELECT_SCENE_TUTORIAL) PlaySound(SOUND_LABEL_SE_enter02);
				TitleSelectSceneNum--;
				if (TitleSelectSceneNum <= TITLE_SELECT_SCENE_TUTORIAL) TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
		}
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			if (TitleSelectSceneNum == CntTitleSelectScene)
			{
				g_TitleSelectScene[CntTitleSelectScene].fAlpha = 1.0f;
			}
			else
			{
				g_TitleSelectScene[CntTitleSelectScene].fAlpha = 0.2f;
			}
			SetColorTitle(&g_TitleSelectScene[CntTitleSelectScene]);
		}
	}

	//タイトル画面(ロゴ点滅)
	else if (TitleSelectTime == false)
	{
		//セレクト切り替え
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			g_TitleBG.fAlpha = 0.4f;
			SetColorTitle(&g_TitleBG);
			g_TitleStartLogo.fAlpha = 0.0f;
			SetColorTitle(&g_TitleStartLogo);
			TitleSelectTime = true;
			g_TitleSelectScene[0].bDisp = true;
			g_TitleSelectScene[1].bDisp = true;
		}
		//初めは透明テクスチャのアルファ値を上げていき見えるようにする
		if (g_TitleStartLogo.fAlpha < 1.0f)
		{
			g_TitleStartLogo.fAlpha += 0.05f;
			if (g_TitleStartLogo.fAlpha >= 1.0f)
			{
				g_TitleStartLogo.fAlpha = 1.0f;
			}
			SetColorTitle(&g_TitleStartLogo);
		}
		//アルファ値が最大まで来たらカウントをとり、点滅を繰り返す
		else
		{
			g_TitleStartLogo.nCountAppearStart++;
			if (g_TitleStartLogo.nCountAppearStart > COUNT_APPERA_START)
			{
				g_TitleStartLogo.nCountDisp = (g_TitleStartLogo.nCountDisp + 1) % 80;
				if (g_TitleStartLogo.nCountDisp > INTERVAL_DISP_START)
				{
					g_TitleStartLogo.bDisp = false;
				}
				else
				{
					g_TitleStartLogo.bDisp = true;
				}
			}
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
	pDevice->SetStreamSource(0, g_TitleBG.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_TitleBG.pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);

	if (g_TitleStartLogo.bDisp == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_TitleStartLogo.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_TitleStartLogo.pD3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}


	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (g_TitleSelectScene[CntTitleSelectScene].bDisp == true)
		{

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_TitleSelectScene[CntTitleSelectScene].pD3DTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	//タイトルBG生成
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&g_TitleBG.pD3DVtxBuff,		// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_TitleBG.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
			g_TitleBG.pD3DVtxBuff->Unlock();
		}
	}

	//タイトルスタートロゴ生成
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&g_TitleStartLogo.pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_TitleStartLogo.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X - TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y - TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X + TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y - TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X - TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y + TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X + TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y + TITLE_START_LOGO_HEIGHT, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロックする
			g_TitleStartLogo.pD3DVtxBuff->Unlock();
		}
	}

	//タイトルセレクトロゴ生成
	{
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			// オブジェクトの頂点バッファを生成
			if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
				D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
				FVF_VERTEX_2D,				// 使用する頂点フォーマット
				D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
				&g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
				NULL)))						// NULLに設定
			{
				return E_FAIL;
			}

			{//頂点バッファの中身を埋める
				VERTEX_2D *pVtx;

				// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
				g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);

				// テクスチャのパースペクティブコレクト用
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				// 頂点データをアンロックする
				g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff->Unlock();
			}
		}
	}


	return S_OK;
}

//=============================================================================
// 頂点色変更
//=============================================================================
void SetColorTitle(TITLECLASS *SetTitle)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		SetTitle->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);

		// 頂点データをアンロックする
		SetTitle->pD3DVtxBuff->Unlock();
	}

}


void SetTitleSelectTimeCHK(bool b)
{
	TitleSelectTimeCHK = b;
}

bool GetTitleSelectTimeCHK(void)
{
	return TitleSelectTimeCHK;
}

int GetSelectTitleScene(void)
{
	return TitleSelectSceneNum;
}
