/**
* @file result.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/scene/result.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT_OK		_T("../data/TEXTURE/result/result_gameclear.png")			// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_NO		_T("../data/TEXTURE/result/result_gameover.png")		// 読み込むテクスチャファイル名
#define	RESULT_OK_POS_X			(0)							// タイトルの表示位置
#define	RESULT_OK_POS_Y			(0)							// タイトルの表示位置
#define	RESULT_OK_SIZE_X		(SCREEN_W)						// タイトルの幅
#define	RESULT_OK_SIZE_Y		(SCREEN_H)						// タイトルの高さ
#define	RESULT_NO_POS_X			(0)						
#define	RESULT_NO_POS_Y			(0)						
#define	RESULT_NO_SIZE_X		(SCREEN_W)					
#define	RESULT_NO_SIZE_Y		(SCREEN_H)					

//=============================================================================
// 初期化処理
//=============================================================================
void RESULT::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPDIRECT3DTEXTURE9 pD3DTexture[OBJECT_RESULT_MAX];
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_OK, &pD3DTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_NO, &pD3DTexture[1]);

	this[0].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[1].tex2D.SetpD3DTexture(pD3DTexture[1]);

	pD3DTexture[0]->Release();
	pD3DTexture[1]->Release();

	// 頂点情報の作成
	MakeVertexResult();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void RESULT::Reinit(void)
{

}

//=============================================================================
// 終了処理
//=============================================================================
void RESULT::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void RESULT::Update(void)
{
	if (IsButtonTriggered(0, BUTTON_A))
	{
		ReinitGame(&this[0]);				// ゲームの再初期化処理
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void RESULT::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, this[0].tex2D.GetpD3DTexture());

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[0].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT RESULT::MakeVertexResult(void)
{
	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		// 頂点座標の設定
		vtx2d[0].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y, 0.0f);
		vtx2d[1].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y, 0.0f);
		vtx2d[2].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);
		vtx2d[3].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vtx2d[0].rhw =
			vtx2d[1].rhw =
			vtx2d[2].rhw =
			vtx2d[3].rhw = 1.0f;

		// 反射光の設定
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vtx2d[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		this[0].tex2D.SettextureVTX(vtx2d);
	}

	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		// 頂点座標の設定
		vtx2d[0].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y, 0.0f);
		vtx2d[1].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y, 0.0f);
		vtx2d[2].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);
		vtx2d[3].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		vtx2d[0].rhw =
			vtx2d[1].rhw =
			vtx2d[2].rhw =
			vtx2d[3].rhw = 1.0f;

		// 反射光の設定
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		vtx2d[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		this[1].tex2D.SettextureVTX(vtx2d);
	}

	return S_OK;
}

