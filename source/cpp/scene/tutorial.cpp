/**
* @file tutorial.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/game.h"
#include "../../h/scene/tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("../data/TEXTURE/tutorialbg.png")						//!< 読み込むテクスチャファイル名
#define	TUTORIAL_SIZE_X			(SCREEN_W/4-50)										//!< チュートリアルの幅
#define	TUTORIAL_SIZE_Y			(SCREEN_H/4-50)										//!< チュートリアルの高さ
#define	TUTORIAL_POS_X			(SCREEN_CENTER_X)									//!< チュートリアルの表示位置
#define	TUTORIAL_POS_Y			(SCREEN_CENTER_Y)									//!< チュートリアルの表示位置

//=============================================================================
// 初期化処理
//=============================================================================
void TUTO::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPDIRECT3DTEXTURE9 pD3DTexture;
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TUTORIAL, &pD3DTexture);
	this[0].tex2D.SetpD3DTexture(pD3DTexture);
	pD3DTexture->Release();

	//描画位置設定
	this[0].SetPos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_SIZE_X, TUTORIAL_POS_Y - TUTORIAL_SIZE_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y - TUTORIAL_SIZE_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f));

	// 頂点情報の作成
	MakeVertexTutorial();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void TUTO::Reinit(void)
{
	this[0].cnt = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void TUTO::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void TUTO::Update(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(1, BUTTON_START) || IsButtonTriggered(2, BUTTON_START) || IsButtonTriggered(3, BUTTON_START))
	{
		PlaySound(SOUND_LABEL_SE_enter02);
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
		InitGame();
	}
	for (int CntPlayer = 0; CntPlayer < OBJECT_TUTORIAL_MAX; CntPlayer++)
	{
		if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(CntPlayer, BUTTON_R3))
		{
			this[CntPlayer].cnt++;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void TUTO::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < OBJECT_TUTORIAL_MAX; CntPlayer++)
	{
		if (this[CntPlayer].cnt % 2 == 0)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, this[CntPlayer].tex2D.GetpD3DTexture());
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT TUTO::MakeVertexTutorial(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_TUTORIAL_MAX; CntPlayer++)
	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		// 頂点座標の設定
		vtx2d[0].vtx = D3DXVECTOR3(pos.x - TUTORIAL_SIZE_X, pos.y - TUTORIAL_SIZE_Y, 0.0f);
		vtx2d[1].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SIZE_X, pos.y - TUTORIAL_SIZE_Y, 0.0f);
		vtx2d[2].vtx = D3DXVECTOR3(pos.x - TUTORIAL_SIZE_X, pos.y + TUTORIAL_SIZE_Y, 0.0f);
		vtx2d[3].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SIZE_X, pos.y + TUTORIAL_SIZE_Y, 0.0f);
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
		this[CntPlayer].tex2D.SettextureVTX(vtx2d);
	}
	return S_OK;
}

