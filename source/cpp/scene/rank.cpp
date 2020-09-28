/**
* @file rank.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/scene/rank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RANK2		("../data/TEXTURE/2i.png")							//!< 読み込むテクスチャファイル名
#define	TEXTURE_RANK3		("../data/TEXTURE/3i.png")							//!< 読み込むテクスチャファイル名
#define	TEXTURE_RANK4		("../data/TEXTURE/4i.png")							//!< 読み込むテクスチャファイル名
#define	RANK_SIZE_X			(SCREEN_W/4)									//!< やられた順位テクスチャの幅
#define	RANK_SIZE_Y			(SCREEN_H/4)									//!< やられた順位テクスチャの高さ
#define	RANK_POS_X			(SCREEN_W)										//!< やられた順位テクスチャの表示位置
#define	RANK_POS_Y			(SCREEN_H)										//!< やられた順位テクスチャの表示位置

//=============================================================================
// 初期化処理
//=============================================================================
void RANK::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_RANK4,
			&this[0].tex2D.pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_RANK3,
			&this[1].tex2D.pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_RANK2,
			&this[2].tex2D.pD3DTexture);

	// 頂点情報の作成
	MakeVertexRank();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void RANK::Reinit(void)
{
	this[0].SetUse(false);
	this[1].SetUse(false);
	this[2].SetUse(false);
}

//=============================================================================
// 終了処理
//=============================================================================
void RANK::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void RANK::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void RANK::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		bool use = this[CntRank].GetUse();
		if (use == true)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, this[CntRank].tex2D.pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntRank].tex2D.textureVTX, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT RANK::MakeVertexRank(void)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		// テクスチャのパースペクティブコレクト用
		this[CntRank].tex2D.textureVTX[0].rhw =
			this[CntRank].tex2D.textureVTX[1].rhw =
			this[CntRank].tex2D.textureVTX[2].rhw =
			this[CntRank].tex2D.textureVTX[3].rhw = 1.0f;

		// 反射光の設定
		this[CntRank].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntRank].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntRank].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntRank].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		this[CntRank].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[CntRank].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[CntRank].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[CntRank].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

//=============================================================================
// ランクをセット
//=============================================================================
void RANK::SetRank(int PlayerNum)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		bool use = this[CntRank].GetUse();
		if (use != true)
		{
			switch(PlayerNum)
			{
			case 0:
				this[CntRank].tex2D.textureVTX[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 1:
				this[CntRank].tex2D.textureVTX[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 2:
				this[CntRank].tex2D.textureVTX[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 3:
				this[CntRank].tex2D.textureVTX[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				this[CntRank].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			default:
				break;
			}
			this[CntRank].SetUse(true);
			break;
		}
	}
}
