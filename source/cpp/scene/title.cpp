/**
* @file title.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/scene/title.h"

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


//=============================================================================
// 初期化処理
//=============================================================================
void TITLE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexTitle();

	// テクスチャの読み込み
	LPDIRECT3DTEXTURE9 pD3DTexture[OBJECT_TITLE_MAX];
	D3DXCreateTextureFromFile(pDevice,TEXTURE_TITLE_BG,&pD3DTexture[0]);	

	D3DXCreateTextureFromFile(pDevice,TEXTURE_LOGO_START,&pD3DTexture[1]);

	D3DXCreateTextureFromFile(pDevice,TEXTURE_TITLE_SELECT_TUTORIAL,&pD3DTexture[2]);

	D3DXCreateTextureFromFile(pDevice,TEXTURE_TITLE_SELECT_GAME,&pD3DTexture[3]);

	this[0].tex2DVB.SetpD3DTexture(pD3DTexture[0]);
	this[1].tex2DVB.SetpD3DTexture(pD3DTexture[1]);
	this[2].tex2DVB.SetpD3DTexture(pD3DTexture[2]);
	this[3].tex2DVB.SetpD3DTexture(pD3DTexture[3]);

	pD3DTexture[0]->Release();
	pD3DTexture[1]->Release();
	pD3DTexture[2]->Release();
	pD3DTexture[3]->Release();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void TITLE::Reinit(void)
{
	this[0].TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
	this[0].fAlpha = 1.0f;
	this[0].TitleSelectTime = false;

	this[1].nCountAppearStart = 0;
	this[1].fAlpha = 0.0f;
	this[1].nCountDisp = 0;
	this[1].bDisp = false;
	this[1].nConutDemo = 0;

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		this[CntTitleSelectScene].bDisp = false;
		this[CntTitleSelectScene].fAlpha = 0.0f;
	}
	this[0].SetColorTitle(0, this[0].fAlpha);
	this[0].SetColorTitle(1, this[0].fAlpha);
}

//=============================================================================
// 終了処理
//=============================================================================
void TITLE::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void TITLE::Update(void)
{
	//タイトル画面(セレクト中)
	if (this[0].TitleSelectTime == true)
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
			this[0].fAlpha = 1.0f;
			SetColorTitle(0, this[0].fAlpha);
			this[1].fAlpha = 0.0f;
			SetColorTitle(1, this[1].fAlpha);
			this[0].TitleSelectTime = false;
			this[2].bDisp = false;
			this[3].bDisp = false;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DIGITAL_DOWN))
		{
			////選択肢が二つだとこっち
			//{
			//	TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
			//}
			//3つ以上選択肢があるとこっち使う
			{
				if (this[0].TitleSelectSceneNum != TITLE_SELECT_SCENE_GAME) PlaySound(SOUND_LABEL_SE_enter02);
				this[0].TitleSelectSceneNum++;
				if (this[0].TitleSelectSceneNum > TITLE_SELECT_SCENE_GAME) this[0].TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
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
				if (this[0].TitleSelectSceneNum != TITLE_SELECT_SCENE_TUTORIAL) PlaySound(SOUND_LABEL_SE_enter02);
				this[0].TitleSelectSceneNum--;
				if (this[0].TitleSelectSceneNum <= TITLE_SELECT_SCENE_TUTORIAL) this[0].TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
		}
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			if (this[0].TitleSelectSceneNum == CntTitleSelectScene)
			{
				this[2].fAlpha = 1.0f;
			}
			else
			{
				this[3].fAlpha = 0.2f;
			}
			SetColorTitle(2, this[2].fAlpha);
			SetColorTitle(3, this[3].fAlpha);
		}
	}

	//タイトル画面(ロゴ点滅)
	else if (this[0].TitleSelectTime == false)
	{
		//セレクト切り替え
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			this[0].fAlpha = 0.4f;
			SetColorTitle(0, this[0].fAlpha);
			this[1].fAlpha = 0.0f;
			SetColorTitle(1,this[1].fAlpha);
			this[0].TitleSelectTime = true;
			this[2].bDisp = true;
			this[3].bDisp = true;
		}
		//初めは透明テクスチャのアルファ値を上げていき見えるようにする
		if (this[1].fAlpha < 1.0f)
		{
			this[1].fAlpha += 0.05f;
			if (this[1].fAlpha >= 1.0f)
			{
				this[1].fAlpha = 1.0f;
			}
			SetColorTitle(1, this[1].fAlpha);
		}
		//アルファ値が最大まで来たらカウントをとり、点滅を繰り返す
		else
		{
			this[1].nCountAppearStart++;
			if (this[1].nCountAppearStart > COUNT_APPERA_START)
			{
				this[1].nCountDisp = (this[1].nCountDisp + 1) % 80;
				if (this[1].nCountDisp > INTERVAL_DISP_START)
				{
					this[1].bDisp = false;
				}
				else
				{
					this[1].bDisp = true;
				}
			}
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void TITLE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this[0].tex2DVB.GetpD3DVtxBuff(), 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, this[0].tex2DVB.GetpD3DTexture());

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);

	if (this[1].bDisp == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, this[1].tex2DVB.GetpD3DVtxBuff(), 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, this[1].tex2DVB.GetpD3DTexture());

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}


	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (this[CntTitleSelectScene].bDisp == true)
		{

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this[CntTitleSelectScene].tex2DVB.GetpD3DVtxBuff(), 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, this[CntTitleSelectScene].tex2DVB.GetpD3DTexture());

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT TITLE::MakeVertexTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//タイトルBG生成
	{
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;

		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&pD3DVtxBuff,		// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
			pD3DVtxBuff->Unlock();

			this[0].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
		}
	}

	//タイトルスタートロゴ生成
	{
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;

		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロックする
			pD3DVtxBuff->Unlock();
			this[1].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
		}
	}

	//タイトルセレクトロゴ生成
	{
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;

			// オブジェクトの頂点バッファを生成
			if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
				D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
				FVF_VERTEX_2D,				// 使用する頂点フォーマット
				D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
				&pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
				NULL)))						// NULLに設定
			{
				return E_FAIL;
			}

			{//頂点バッファの中身を埋める
				VERTEX_2D *pVtx;

				// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
				pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				// 頂点データをアンロックする
				pD3DVtxBuff->Unlock();
				this[CntTitleSelectScene].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
			}
		}
	}


	return S_OK;
}

//=============================================================================
// 頂点色変更
//=============================================================================
void TITLE::SetColorTitle(int CntTitle, float alpha)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntTitle].tex2DVB.GetpD3DVtxBuff();
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// 頂点データをアンロックする
		pD3DVtxBuff->Unlock();

		this[CntTitle].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
	}

}

