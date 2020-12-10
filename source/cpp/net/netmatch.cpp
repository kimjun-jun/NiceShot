/**
* @file netmatch.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/net/netmatch.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NETMATCHRIAL		("../data/TEXTURE/old/MatchLogo.png")						//!< 読み込むテクスチャファイル名
#define	NETMATCHRIAL_SIZE_X			(SCREEN_W/4-50)										//!< チュートリアルの幅
#define	NETMATCHRIAL_SIZE_Y			(SCREEN_H/4-50)										//!< チュートリアルの高さ
#define	NETMATCHRIAL_POS_X			(SCREEN_CENTER_X)									//!< チュートリアルの表示位置
#define	NETMATCHRIAL_POS_Y			(SCREEN_CENTER_Y)									//!< チュートリアルの表示位置

//=============================================================================
// 初期化処理
//=============================================================================
void NETMATCH::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntTuto = 0; CntTuto < OBJECT_NETMATCH_MAX; CntTuto++)
	{
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NETMATCHRIAL, &this[CntTuto].tex2D.pD3DTexture);
	}

	//描画位置設定
	this[0].SetPos(D3DXVECTOR3(NETMATCHRIAL_POS_X - NETMATCHRIAL_SIZE_X, NETMATCHRIAL_POS_Y - NETMATCHRIAL_SIZE_Y, 0.0f));
	// 頂点情報の作成
	MakeVertexTutorial();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void NETMATCH::Reinit(void)
{
}

//=============================================================================
// 終了処理
//=============================================================================
void NETMATCH::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void NETMATCH::Update(GAME_OBJECT* obj, FADE *fade)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void NETMATCH::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntTuto = 0; CntTuto < OBJECT_NETMATCH_MAX; CntTuto++)
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0, this[CntTuto].tex2D.pD3DTexture);

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntTuto].tex2D.textureVTX, sizeof(VERTEX_2D));

	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT NETMATCH::MakeVertexTutorial(void)
{
	for (int CntTuto = 0; CntTuto < OBJECT_NETMATCH_MAX; CntTuto++)
	{
		D3DXVECTOR3 pos = this[CntTuto].GetPos();
		// 頂点座標の設定
		this[CntTuto].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x - NETMATCHRIAL_SIZE_X, pos.y - NETMATCHRIAL_SIZE_Y, 0.0f);
		this[CntTuto].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + NETMATCHRIAL_SIZE_X, pos.y - NETMATCHRIAL_SIZE_Y, 0.0f);
		this[CntTuto].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x - NETMATCHRIAL_SIZE_X, pos.y + NETMATCHRIAL_SIZE_Y, 0.0f);
		this[CntTuto].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + NETMATCHRIAL_SIZE_X, pos.y + NETMATCHRIAL_SIZE_Y, 0.0f);
		// テクスチャのパースペクティブコレクト用
		this[CntTuto].tex2D.textureVTX[0].rhw =
			this[CntTuto].tex2D.textureVTX[1].rhw =
			this[CntTuto].tex2D.textureVTX[2].rhw =
			this[CntTuto].tex2D.textureVTX[3].rhw = 1.0f;

		// 反射光の設定
		this[CntTuto].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntTuto].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntTuto].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntTuto].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		this[CntTuto].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[CntTuto].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[CntTuto].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[CntTuto].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

