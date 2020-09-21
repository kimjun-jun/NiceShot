/**
* @file tutorial.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/scene/tutorial.h"
#include "../../h/other/input.h"
#include "../../h/scene/fade.h"
#include "../../h/other/sound.h"
#include "../../h/object/objectclass.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief 頂点生成関数 MakeVertexTutorial
* @return HRESULT
*/
HRESULT MakeVertexTutorial(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
TUTO tuto[TUTORIAL_GOUKEI];			//!<player画面1～4まで対応	構造体変数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (type == 0)
		{
			D3DXCreateTextureFromFile(pDevice,
				TEXTURE_TUTORIAL,
				&tuto[CntPlayer].pD3DTexture);
		}
		tuto[CntPlayer].cnt = 0;
	}
	//描画位置設定
	tuto[0].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_SIZE_X, TUTORIAL_POS_Y - TUTORIAL_SIZE_Y, 0.0f);
	tuto[1].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y - TUTORIAL_SIZE_Y, 0.0f);
	tuto[2].pos = D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f);
	tuto[3].pos = D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexTutorial();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (tuto[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			tuto[CntPlayer].pD3DTexture->Release();
			tuto[CntPlayer].pD3DTexture = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(1, BUTTON_START) || IsButtonTriggered(2, BUTTON_START) || IsButtonTriggered(3, BUTTON_START))
	{// Enter押したら、ステージを切り替える
		PlaySound(SOUND_LABEL_SE_enter02);
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
		InitGame();
	}
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(CntPlayer, BUTTON_R3))
		{
			tuto[CntPlayer].cnt++;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (tuto[CntPlayer].cnt % 2 == 0)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, tuto[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, tuto[CntPlayer].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// 頂点座標の設定
		tuto[CntPlayer].vertexWk[0].vtx = D3DXVECTOR3(tuto[CntPlayer].pos.x - TUTORIAL_SIZE_X, tuto[CntPlayer].pos.y - TUTORIAL_SIZE_Y, 0.0f);
		tuto[CntPlayer].vertexWk[1].vtx = D3DXVECTOR3(tuto[CntPlayer].pos.x + TUTORIAL_SIZE_X, tuto[CntPlayer].pos.y - TUTORIAL_SIZE_Y, 0.0f);
		tuto[CntPlayer].vertexWk[2].vtx = D3DXVECTOR3(tuto[CntPlayer].pos.x - TUTORIAL_SIZE_X, tuto[CntPlayer].pos.y + TUTORIAL_SIZE_Y, 0.0f);
		tuto[CntPlayer].vertexWk[3].vtx = D3DXVECTOR3(tuto[CntPlayer].pos.x + TUTORIAL_SIZE_X, tuto[CntPlayer].pos.y + TUTORIAL_SIZE_Y, 0.0f);
		// テクスチャのパースペクティブコレクト用
		tuto[CntPlayer].vertexWk[0].rhw =
			tuto[CntPlayer].vertexWk[1].rhw =
			tuto[CntPlayer].vertexWk[2].rhw =
			tuto[CntPlayer].vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		tuto[CntPlayer].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		tuto[CntPlayer].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		tuto[CntPlayer].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		tuto[CntPlayer].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		tuto[CntPlayer].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		tuto[CntPlayer].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		tuto[CntPlayer].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		tuto[CntPlayer].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

