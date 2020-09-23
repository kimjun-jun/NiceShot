/**
* @file rank.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/scene/rank.h"
#include "../../h/object/objectclass.h"

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
	LPDIRECT3DTEXTURE9 pD3DTexture[OBJECT_RANK_MAX];
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_RANK4,
		&pD3DTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_RANK3,
		&pD3DTexture[1]);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_RANK2,
		&pD3DTexture[2]);

	this[0].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[1].tex2D.SetpD3DTexture(pD3DTexture[1]);
	this[2].tex2D.SetpD3DTexture(pD3DTexture[2]);
	// 頂点情報の作成
	MakeVertexRank();
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
			pDevice->SetTexture(0, this[CntRank].tex2D.GetpD3DTexture());
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntRank].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
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
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
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
		this[CntRank].tex2D.SettextureVTX(vtx2d);
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
			VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
			switch(PlayerNum)
			{
			case 0:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 1:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 2:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 3:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			default:
				break;
			}
			this[CntRank].tex2D.SettextureVTX(vtx2d);
			this[CntRank].SetUse(true);
			break;
		}
	}
}
