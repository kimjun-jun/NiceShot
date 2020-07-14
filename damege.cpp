/**
* @file damege.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "main.h"
#include "damege.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexDamege(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
DAMEGE damege[DAMEGE_GOUKEI];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDamege(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (type == 0)
		{
			D3DXCreateTextureFromFile(pDevice,
				TEXTURE_DAMEGE,
				&damege[CntPlayer].pD3DTexture);
		}
		damege[CntPlayer].alpha = 0;
		damege[CntPlayer].time = 0.0f;
		damege[CntPlayer].use = false;
	}
	damege[0].pos = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	damege[1].pos = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	damege[2].pos = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);
	damege[3].pos = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexDamege();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDamege(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (damege[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			damege[CntPlayer].pD3DTexture->Release();
			damege[CntPlayer].pD3DTexture = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDamege(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (damege[CntPlayer].use==true)
		{
			damege[CntPlayer].time += 1.0f;
			damege[CntPlayer].alpha = int((damege[CntPlayer].time/ SCREENDAMEGE_TIME)*255.0f);
			// 反射光の設定
			damege[CntPlayer].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, damege[CntPlayer].alpha);
			damege[CntPlayer].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, damege[CntPlayer].alpha);
			damege[CntPlayer].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, damege[CntPlayer].alpha);
			damege[CntPlayer].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, damege[CntPlayer].alpha);

			if (damege[CntPlayer].time >= SCREENDAMEGE_TIME)
			{
				damege[CntPlayer].time = 0.0f;
				damege[CntPlayer].alpha = 0;
				damege[CntPlayer].use = false;
			}
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDamege(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (damege[CntPlayer].use == true)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, damege[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, damege[CntPlayer].vertexWk, sizeof(VERTEX_2D));
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexDamege(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// 頂点座標の設定
		damege[CntPlayer].vertexWk[0].vtx = D3DXVECTOR3(damege[CntPlayer].pos.x - DAMEGE_SIZE_X, damege[CntPlayer].pos.y - DAMEGE_SIZE_Y, 0.0f);
		damege[CntPlayer].vertexWk[1].vtx = D3DXVECTOR3(damege[CntPlayer].pos.x + DAMEGE_SIZE_X, damege[CntPlayer].pos.y - DAMEGE_SIZE_Y, 0.0f);
		damege[CntPlayer].vertexWk[2].vtx = D3DXVECTOR3(damege[CntPlayer].pos.x - DAMEGE_SIZE_X, damege[CntPlayer].pos.y + DAMEGE_SIZE_Y, 0.0f);
		damege[CntPlayer].vertexWk[3].vtx = D3DXVECTOR3(damege[CntPlayer].pos.x + DAMEGE_SIZE_X, damege[CntPlayer].pos.y + DAMEGE_SIZE_Y, 0.0f);
		// テクスチャのパースペクティブコレクト用
		damege[CntPlayer].vertexWk[0].rhw =
			damege[CntPlayer].vertexWk[1].rhw =
			damege[CntPlayer].vertexWk[2].rhw =
			damege[CntPlayer].vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		damege[CntPlayer].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		damege[CntPlayer].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		damege[CntPlayer].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		damege[CntPlayer].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

		// テクスチャ座標の設定
		damege[CntPlayer].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		damege[CntPlayer].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		damege[CntPlayer].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		damege[CntPlayer].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// スクリーンダメージ情報を取得
//=============================================================================
DAMEGE *GetDamege(void)
{
	 return &damege[0];
}
