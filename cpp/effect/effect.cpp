/**
* @file effect.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/effect/effect.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT			"../data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名

#define	MAX_EFFECT				(4096)							// エフェクト最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nTimer;				// タイマー
	float nDecAlpha;		// 減衰値
	bool bUse;				// 使用しているかどうか
} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
void SetColorEffect(int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldEffect;				// ワールドマトリックス

EFFECT					g_aEffect[MAX_EFFECT];			// 弾ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexEffect(pDevice);
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_EFFECT,			// ファイルの名前
			&g_pD3DTextureEffect);	// 読み込むメモリー
	}
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].nDecAlpha = 0.0f;
		g_aEffect[nCntEffect].fSizeX = EFFECT_NORMALSET_SIZE_X;
		g_aEffect[nCntEffect].fSizeY = EFFECT_NORMALSET_SIZE_Y;
		g_aEffect[nCntEffect].nTimer = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
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
void UpdateEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].nDecAlpha;
			if (g_aEffect[nCntEffect].col.a <= 0.0f)
			{
				g_aEffect[nCntEffect].col.a = 0.0f;
			}

			//カラーセット
			SetColorEffect(nCntEffect, D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a));

			g_aEffect[nCntEffect].nTimer--;
			if (g_aEffect[nCntEffect].nTimer <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// Z比較なし
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWorldEffect);

				// ビューマトリックスを取得
				CAMERA *cam = GetCamera();

				g_mtxWorldEffect._11 = cam[CntPlayer].mtxView._11;
				g_mtxWorldEffect._12 = cam[CntPlayer].mtxView._21;
				g_mtxWorldEffect._13 = cam[CntPlayer].mtxView._31;
				g_mtxWorldEffect._21 = cam[CntPlayer].mtxView._12;
				g_mtxWorldEffect._22 = cam[CntPlayer].mtxView._22;
				g_mtxWorldEffect._23 = cam[CntPlayer].mtxView._32;
				g_mtxWorldEffect._31 = cam[CntPlayer].mtxView._13;
				g_mtxWorldEffect._32 = cam[CntPlayer].mtxView._23;
				g_mtxWorldEffect._33 = cam[CntPlayer].mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, g_aEffect[nCntEffect].scale.x, g_aEffect[nCntEffect].scale.y, g_aEffect[nCntEffect].scale.z);
				D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

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

	// Z比較あり
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * MAX_EFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
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

		for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
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
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
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
void SetColorEffect(int nIdxEffect, D3DXCOLOR col)
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
int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffect[nCntEffect].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fSizeX = fSizeX;
			g_aEffect[nCntEffect].fSizeY = fSizeY;
			g_aEffect[nCntEffect].nTimer = nTimer;
			g_aEffect[nCntEffect].nDecAlpha = col.a / nTimer;
			g_aEffect[nCntEffect].bUse = true;

			// 頂点座標の設定
			SetVertexEffect(nCntEffect, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorEffect(nCntEffect,
				D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g,
					g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a));

			nIdxEffect = nCntEffect;

			break;
		}
	}
	return nIdxEffect;
}
