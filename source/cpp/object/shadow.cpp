/**
* @file shadow.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW		"../data/TEXTURE/other/shadow000.jpg"	// 読み込むテクスチャファイル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureShadow = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuffShadow = NULL;		// 頂点バッファインターフェースへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
void SHADOW::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexShadow(pDevice);

	// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_SHADOW,				// ファイルの名前
			&g_pD3DTextureShadow);		// 読み込むメモリー
}

//=============================================================================
// 再初期化処理
//=============================================================================
void SHADOW::Reinit(void)
{
	for (int nCntShadow = 0; nCntShadow < OBJECT_SHADOW_MAX; nCntShadow++)
	{
		this[nCntShadow].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntShadow].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntShadow].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[nCntShadow].SetUse(false);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void SHADOW::Uninit(void)
{
	if(g_pD3DTextureShadow != NULL)
	{// テクスチャの開放
		g_pD3DTextureShadow->Release();
		g_pD3DTextureShadow = NULL;
	}

	if(g_pD3DVtxBuffShadow != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffShadow->Release();
		g_pD3DVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void SHADOW::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void SHADOW::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for(int nCntShadow = 0; nCntShadow < OBJECT_SHADOW_MAX; nCntShadow++)
	{
		bool use = this[nCntShadow].GetUse();
		if(use)
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			//-----------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[nCntShadow].GetPos();
			D3DXVECTOR3 rot = this[nCntShadow].GetRot();
			D3DXVECTOR3 scl = this[nCntShadow].GetScl();
			D3DXMATRIX mtxWorld = this[nCntShadow].GetMatrix();
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 頂点バッファをレンダリングパイプラインに設定
			pDevice->SetStreamSource(0, g_pD3DVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT SHADOW::MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	int max_shadow = OBJECT_SHADOW_MAX;		// OBJECT_SHADOW_MAX

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * max_shadow,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
												FVF_VERTEX_3D,							// 使用する頂点フォーマット
												D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffShadow,					// 頂点バッファインターフェースへのポインタ
												NULL)))									// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntShadow = 0; nCntShadow < max_shadow; nCntShadow++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			pVtx[1].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2);
			pVtx[2].vtx = D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);
			pVtx[3].vtx = D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[1].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[2].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
			pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffShadow->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SHADOW::SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ)
{
	//if (nIdxShadow < 0) return;	// 不正ならリターンする
	//{//頂点バッファの中身を埋める
	//	VERTEX_3D *pVtx;
	//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	//	g_pD3DVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	//	pVtx += (nIdxShadow * 4);
	//	// 頂点座標の設定
	//	pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, 0.0f, fSizeZ / 2);
	//	pVtx[1].vtx = D3DXVECTOR3(fSizeX / 2, 0.0f, fSizeZ / 2);
	//	pVtx[2].vtx = D3DXVECTOR3(-fSizeX / 2, 0.0f, -fSizeZ / 2);
	//	pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, 0.0f, -fSizeZ / 2);
	//	// 頂点データをアンロックする
	//	g_pD3DVtxBuffShadow->Unlock();
	//}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SHADOW::SetColorShadow(int nIdxShadow, D3DXCOLOR col)
{
	//if (nIdxShadow < 0) return;	// 不正ならリターンする
	//							{//頂点バッファの中身を埋める
	//	VERTEX_3D *pVtx;
	//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	//	g_pD3DVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	//	pVtx += (nIdxShadow * 4);
	//	// 頂点座標の設定
	//	pVtx[0].diffuse =
	//	pVtx[1].diffuse =
	//	pVtx[2].diffuse =
	//	pVtx[3].diffuse = col;
	//	// 頂点データをアンロックする
	//	g_pD3DVtxBuffShadow->Unlock();
	//}
}

//=============================================================================
// 影の作成
//=============================================================================
int SHADOW::CreateShadow(D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	int nIdxShadow = -1;

	for(int nCntShadow = 0; nCntShadow < OBJECT_SHADOW_MAX; nCntShadow++)
	{
		bool use = this[nCntShadow].GetUse();
		if(use != true)
		{
			this[nCntShadow].SetPos(pos);
			this[nCntShadow].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this[nCntShadow].SetScl(scl);
			this[nCntShadow].SetUse(true);

			nIdxShadow = nCntShadow;
			break;
		}
	}

	return nIdxShadow;
}

//=============================================================================
// 影の破棄
//=============================================================================
void SHADOW::ReleaseShadow(int nIdxShadow)
{
	if(nIdxShadow >= 0 && nIdxShadow < OBJECT_SHADOW_MAX)
	{
		this[nIdxShadow].SetUse(false);
	}

}

//=============================================================================
// 位置の設定
//=============================================================================
void SHADOW::SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	if (nIdxShadow < 0) return;	// 不正ならリターンする

	this[nIdxShadow].SetPos(pos);
	this[nIdxShadow].SetScl(scl);
}

//=============================================================================
// 影情報を取得
//=============================================================================
SHADOW* SHADOW::GetShadow(void)
{
	return &this[0];	// 不正ならリターンする
}

