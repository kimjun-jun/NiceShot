/**
* @file rank.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/scene/rank.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief 頂点生成関数 MakeVertexRank
* @return HRESULT
*/
HRESULT MakeVertexRank(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
RANK rank[RANK_GOUKEI];			//!< ランクのクラス変数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRank(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntRank = 0; CntRank < RANK_GOUKEI; CntRank++)
	{
		rank[CntRank].use = false;
	}

	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_RANK4,
			&rank[0].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_RANK3,
			&rank[1].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_RANK2,
			&rank[2].pD3DTexture);
	}

	// 頂点情報の作成
	MakeVertexRank();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRank(void)
{
	for (int CntRank = 0; CntRank < RANK_GOUKEI; CntRank++)
	{
		if (rank[CntRank].pD3DTexture != NULL)
		{// テクスチャの開放
			rank[CntRank].pD3DTexture->Release();
			rank[CntRank].pD3DTexture = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRank(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntRank = 0; CntRank < RANK_GOUKEI; CntRank++)
	{
		if (rank[CntRank].use == true)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, rank[CntRank].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, rank[CntRank].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexRank(void)
{
	for (int CntRank = 0; CntRank < RANK_GOUKEI; CntRank++)
	{
		// テクスチャのパースペクティブコレクト用
		rank[CntRank].vertexWk[0].rhw =
			rank[CntRank].vertexWk[1].rhw =
			rank[CntRank].vertexWk[2].rhw =
			rank[CntRank].vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		rank[CntRank].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		rank[CntRank].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		rank[CntRank].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		rank[CntRank].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		rank[CntRank].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		rank[CntRank].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		rank[CntRank].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		rank[CntRank].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// ランク情報を取得
//=============================================================================
RANK *GetRank(void)
{
	return &rank[0];
}

//=============================================================================
// ランクをセット
//=============================================================================
void SetRank(int PlayerNum)
{
	for (int CntRank = 0; CntRank < RANK_GOUKEI; CntRank++)
	{
		if (rank[CntRank].use == false)
		{
			switch(PlayerNum)
			{
			case 0:
				rank[CntRank].vertexWk[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 1:
				rank[CntRank].vertexWk[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 2:
				rank[CntRank].vertexWk[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 3:
				rank[CntRank].vertexWk[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				rank[CntRank].vertexWk[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			default:
				break;
			}
			rank[CntRank].use = true;
			break;
		}
	}
}
