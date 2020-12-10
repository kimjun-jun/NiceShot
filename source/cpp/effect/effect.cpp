/**
* @file effect.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/effect/effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT			"../data/TEXTURE/effect/effect000.jpg"	// 読み込むテクスチャファイル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;		// 頂点バッファインターフェースへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
void EFFECT::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexEffect(pDevice);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_EFFECT,			// ファイルの名前
			&g_pD3DTextureEffect);	// 読み込むメモリー

}

//=============================================================================
// 再初期化処理
//=============================================================================
void EFFECT::Reinit(void)
{
	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		this[nCntEffect].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntEffect].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntEffect].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[nCntEffect].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntEffect].SetCol(D3DXCOLOR(DWORD(0)));
		this[nCntEffect].nTimer = 0;
		this[nCntEffect].nDecAlpha = 0.0f;
		this[nCntEffect].SetUse(false);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void  EFFECT::Uninit(void)
{
	if (g_pD3DTextureEffect != NULL)
	{// テクスチャの開放
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if (g_pD3DVtxBuffEffect != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  EFFECT::Update(void)
{
	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this[nCntEffect].GetUse();
		if (use)
		{
			//--------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[nCntEffect].GetPos();
			D3DXVECTOR3 move = this[nCntEffect].GetMove();
			D3DXCOLOR col = this[nCntEffect].GetCol();

			pos.x += move.x;
			pos.z += move.z;

			col.a -= this[nCntEffect].nDecAlpha;
			if (col.a <= 0.0f)
			{
				col.a = 0.0f;
			}

			//カラーセット
			SetColorEffect(nCntEffect, col);

			this[nCntEffect].nTimer--;
			if (this[nCntEffect].nTimer <= 0)
			{
				this[nCntEffect].SetUse(false);
			}

			//--------------------------------------------オブジェクト値書き込み
			this[nCntEffect].SetPos(pos);
			this[nCntEffect].SetCol(col);

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  EFFECT::Draw(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this[nCntEffect].GetUse();
		if (use)
		{
			//--------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[nCntEffect].GetPos();
			D3DXVECTOR3 scl = this[nCntEffect].GetScl();
			D3DXCOLOR col = this[nCntEffect].GetCol();
			D3DXMATRIX mtxWorldEffect = this[nCntEffect].GetMatrix();

			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldEffect);

			// ビューマトリックスを取得
			CAMERA *cam = GetCamera();

			mtxWorldEffect._11 = cam[CntPlayer].mtxView._11;
			mtxWorldEffect._12 = cam[CntPlayer].mtxView._21;
			mtxWorldEffect._13 = cam[CntPlayer].mtxView._31;
			mtxWorldEffect._21 = cam[CntPlayer].mtxView._12;
			mtxWorldEffect._22 = cam[CntPlayer].mtxView._22;
			mtxWorldEffect._23 = cam[CntPlayer].mtxView._32;
			mtxWorldEffect._31 = cam[CntPlayer].mtxView._13;
			mtxWorldEffect._32 = cam[CntPlayer].mtxView._23;
			mtxWorldEffect._33 = cam[CntPlayer].mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldEffect);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureEffect);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEffect * 4), POLYGON_2D_NUM);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT EFFECT::MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * OBJECT_EFFECT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffEffect,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_NORMALSET_SIZE_X / 2, -EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EFFECT_NORMALSET_SIZE_X / 2, -EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void  EFFECT::SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void  EFFECT::SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// エフェクトの設定
//=============================================================================
int EFFECT::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this[nCntEffect].GetUse();
		if (use != true)
		{
			this[nCntEffect].SetPos(pos);
			this[nCntEffect].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this[nCntEffect].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this[nCntEffect].SetMove(move);
			this[nCntEffect].SetCol(col);
			this[nCntEffect].nTimer = nTimer;
			this[nCntEffect].nDecAlpha = col.a / nTimer;
			this[nCntEffect].SetUse(true);

			// 頂点座標の設定
			SetVertexEffect(nCntEffect, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorEffect(nCntEffect,col);

			nIdxEffect = nCntEffect;

			break;
		}
	}
	return nIdxEffect;
}
