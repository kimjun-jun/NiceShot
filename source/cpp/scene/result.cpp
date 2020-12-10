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
#define	TEXTURE_RESULT_LOGO		_T("../data/TEXTURE/other/GameClear.png")					// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_BG		_T("../data/TEXTURE/other/GameClear_Background.png")		// 読み込むテクスチャファイル名
#define	RESULT_LOGO_POS_X		(SCREEN_CENTER_X)											// ロゴの座標
#define	RESULT_LOGO_POS_Y		(SCREEN_CENTER_Y-200.0f)									// ロゴの座標
#define	RESULT_LOGO_SIZE_X		(300.0f)													// ロゴの幅
#define	RESULT_LOGO_SIZE_Y		(200.0f)													// ロゴの高さ
#define	RESULT_BG_SIZE_X		(SCREEN_W)					
#define	RESULT_BG_SIZE_Y		(SCREEN_H)					

//=============================================================================
// 初期化処理
//=============================================================================
void RESULT::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_LOGO, &this[0].tex2D.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_BG, &this[1].tex2D.pD3DTexture);


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
void RESULT::Update(GAME_OBJECT* obj, FADE *fade)
{
	if (IsButtonTriggered(0, BUTTON_A)|| GetKeyboardTrigger(DIK_RETURN))
	{
		obj->Reinit();				// ゲームの再初期化処理
		fade->SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void RESULT::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//BG
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, this[1].tex2D.pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[1].tex2D.textureVTX, sizeof(VERTEX_2D));


	//LOGO
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, this[0].tex2D.pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[0].tex2D.textureVTX, sizeof(VERTEX_2D));

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT RESULT::MakeVertexResult(void)
{
	{
		// 頂点座標の設定
		this[0].tex2D.textureVTX[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f);
		this[0].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f);
		this[0].tex2D.textureVTX[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f);
		this[0].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		this[0].tex2D.textureVTX[0].rhw =
			this[0].tex2D.textureVTX[1].rhw =
			this[0].tex2D.textureVTX[2].rhw =
			this[0].tex2D.textureVTX[3].rhw = 1.0f;

		// 反射光の設定
		this[0].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[0].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[0].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[0].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		this[0].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[0].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[0].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[0].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	{
		// 頂点座標の設定
		this[1].tex2D.textureVTX[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this[1].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RESULT_BG_SIZE_X, 0.0f, 0.0f);
		this[1].tex2D.textureVTX[2].vtx = D3DXVECTOR3(0.0f, RESULT_BG_SIZE_Y, 0.0f);
		this[1].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RESULT_BG_SIZE_X, RESULT_BG_SIZE_Y, 0.0f);

		// テクスチャのパースペクティブコレクト用
		this[1].tex2D.textureVTX[0].rhw =
			this[1].tex2D.textureVTX[1].rhw =
			this[1].tex2D.textureVTX[2].rhw =
			this[1].tex2D.textureVTX[3].rhw = 1.0f;

		// 反射光の設定
		this[1].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[1].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[1].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[1].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		this[1].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[1].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[1].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[1].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

