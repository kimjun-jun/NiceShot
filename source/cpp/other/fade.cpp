/**
* @file fade.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/sound.h"
#include "../../h/other/fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FADE_RATE		(0.02f)									//!< フェード係数

//=============================================================================
// 初期化処理
//=============================================================================
void FADE::Init(void)
{
	// 頂点情報の設定
	MakeVertexFade();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void FADE::Reinit(void)
{
}

//=============================================================================
// 終了処理
//=============================================================================
void FADE::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void FADE::Update(GAME_OBJECT*obj)
{
	if (eFade != FADE_NONE)
	{// フェード処理中
		if (eFade == FADE_OUT)
		{// フェードアウト処理
			color.a += FADE_RATE;		// α値を加算して画面を消していく
			if (color.a >= 1.0f)
			{
				// 状態を切り替え
				obj->SetScene(eScene);

				// フェードイン処理に切り替え
				color.a = 1.0f;
				eFade = FADE_MUSIC_STOP;

				// BGM停止
				StopSound();
			}

			// 色を設定
			SetColor(color);
		}
		else if (eFade == FADE_MUSIC_STOP)
		{
			// BGM再生
			if (sno > -1)
			{
				PlaySound(sno);
			}

			eFade = FADE_IN;
		}
		else if (eFade == FADE_IN)
		{// フェードイン処理
			color.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
			if (color.a <= 0.0f)
			{
				// フェード処理終了
				color.a = 0.0f;
				eFade = FADE_NONE;

			}

			// 色を設定
			SetColor(color);
		}
	}
}

//=============================================================================
// フェード画面
//=============================================================================
void FADE::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[0].tex2D.textureVTX, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
void FADE::MakeVertexFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャのパースペクティブコレクト用
	this[0].tex2D.textureVTX[0].rhw =
		this[0].tex2D.textureVTX[1].rhw =
		this[0].tex2D.textureVTX[2].rhw =
		this[0].tex2D.textureVTX[3].rhw = 1.0f;

	// 反射光の設定
	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	this[0].tex2D.textureVTX[0].diffuse = color;
	this[0].tex2D.textureVTX[1].diffuse = color;
	this[0].tex2D.textureVTX[2].diffuse = color;
	this[0].tex2D.textureVTX[3].diffuse = color;

	// 頂点座標の設定
	this[0].tex2D.textureVTX[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this[0].tex2D.textureVTX[1].vtx = D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f);
	this[0].tex2D.textureVTX[2].vtx = D3DXVECTOR3(0.0f, SCREEN_H, 0.0f);
	this[0].tex2D.textureVTX[3].vtx = D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f);

	// テクスチャ座標の設定
	this[0].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	this[0].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	this[0].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	this[0].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 色を設定
//=============================================================================
void FADE::SetColor(D3DCOLOR col)
{
	// 反射光の設定
	this[0].tex2D.textureVTX[0].diffuse = col;
	this[0].tex2D.textureVTX[1].diffuse = col;
	this[0].tex2D.textureVTX[2].diffuse = col;
	this[0].tex2D.textureVTX[3].diffuse = col;

}

//=============================================================================
// フェードの状態設定
//=============================================================================
void FADE::SetFade(FADE_TYPE fade, E_STAGE next, int sno)
{
	this[0].eFade = fade;
	this[0].eScene = next;
	this[0].sno = sno;

}

