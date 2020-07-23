/**
* @file bulletprediction.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "main.h"
#include "input.h"
#include "camera.h"
#include "bulletprediction.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETPREDICTION		"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	BULLETPREDICTION_SIZE_X			(50.0f)							// ビルボードの幅
#define	BULLETPREDICTION_SIZE_Y			(50.0f)							// ビルボードの高さ

#define	MAX_BULLETPREDICTION			(4096)							// エフェクト最大数

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
} BULLETPREDICTION;

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletprediction(int nIdxBulletprediction, float fSizeX, float fSizeY);
void SetColorBulletprediction(int nIdxBulletprediction, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletprediction = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletprediction = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldBulletprediction;				// ワールドマトリックス

BULLETPREDICTION		g_aBulletprediction[MAX_BULLETPREDICTION];			// 弾ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBulletprediction(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexBulletprediction(pDevice);
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_BULLETPREDICTION,			// ファイルの名前
			&g_pD3DTextureBulletprediction);	// 読み込むメモリー
	}
	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		g_aBulletprediction[nCntBulletprediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBulletprediction[nCntBulletprediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBulletprediction[nCntBulletprediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBulletprediction[nCntBulletprediction].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBulletprediction[nCntBulletprediction].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBulletprediction[nCntBulletprediction].nDecAlpha = 0.0f;
		g_aBulletprediction[nCntBulletprediction].fSizeX = BULLETPREDICTION_SIZE_X;
		g_aBulletprediction[nCntBulletprediction].fSizeY = BULLETPREDICTION_SIZE_Y;
		g_aBulletprediction[nCntBulletprediction].nTimer = 0;
		g_aBulletprediction[nCntBulletprediction].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBulletprediction(void)
{
	if (g_pD3DTextureBulletprediction != NULL)
	{// テクスチャの開放
		g_pD3DTextureBulletprediction->Release();
		g_pD3DTextureBulletprediction = NULL;
	}

	if (g_pD3DVtxBuffBulletprediction != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBulletprediction->Release();
		g_pD3DVtxBuffBulletprediction = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBulletprediction(void)
{
	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		if (g_aBulletprediction[nCntBulletprediction].bUse)
		{
			g_aBulletprediction[nCntBulletprediction].pos.x += g_aBulletprediction[nCntBulletprediction].move.x;
			g_aBulletprediction[nCntBulletprediction].pos.z += g_aBulletprediction[nCntBulletprediction].move.z;

			//g_aBulletprediction[nCntBulletprediction].col.a -= g_aBulletprediction[nCntBulletprediction].nDecAlpha;
			//if (g_aBulletprediction[nCntBulletprediction].col.a <= 0.0f)
			//{
			//	g_aBulletprediction[nCntBulletprediction].col.a = 0.0f;
			//}
			SetColorBulletprediction(nCntBulletprediction,
				D3DXCOLOR(g_aBulletprediction[nCntBulletprediction].col.r, g_aBulletprediction[nCntBulletprediction].col.b,
					g_aBulletprediction[nCntBulletprediction].col.b, g_aBulletprediction[nCntBulletprediction].col.a));

			g_aBulletprediction[nCntBulletprediction].nTimer--;
			if (g_aBulletprediction[nCntBulletprediction].nTimer <= 0)
			{
				g_aBulletprediction[nCntBulletprediction].bUse = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBulletprediction(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		if (g_aBulletprediction[nCntBulletprediction].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBulletprediction);

			// ビューマトリックスを取得
			CAMERA *cam = GetCamera();

			g_mtxWorldBulletprediction._11 = cam[CntPlayer].mtxView._11;
			g_mtxWorldBulletprediction._12 = cam[CntPlayer].mtxView._21;
			g_mtxWorldBulletprediction._13 = cam[CntPlayer].mtxView._31;
			g_mtxWorldBulletprediction._21 = cam[CntPlayer].mtxView._12;
			g_mtxWorldBulletprediction._22 = cam[CntPlayer].mtxView._22;
			g_mtxWorldBulletprediction._23 = cam[CntPlayer].mtxView._32;
			g_mtxWorldBulletprediction._31 = cam[CntPlayer].mtxView._13;
			g_mtxWorldBulletprediction._32 = cam[CntPlayer].mtxView._23;
			g_mtxWorldBulletprediction._33 = cam[CntPlayer].mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aBulletprediction[nCntBulletprediction].scale.x, g_aBulletprediction[nCntBulletprediction].scale.y, g_aBulletprediction[nCntBulletprediction].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletprediction, &g_mtxWorldBulletprediction, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aBulletprediction[nCntBulletprediction].pos.x, g_aBulletprediction[nCntBulletprediction].pos.y, g_aBulletprediction[nCntBulletprediction].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletprediction, &g_mtxWorldBulletprediction, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBulletprediction);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletprediction, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureBulletprediction);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBulletprediction * 4), POLYGON_2D_NUM);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// Z比較あり
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * MAX_BULLETPREDICTION,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffBulletprediction,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletprediction->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
		g_pD3DVtxBuffBulletprediction->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBulletprediction(int nIdxBulletprediction, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletprediction->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletprediction->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorBulletprediction(int nIdxBulletprediction, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletprediction->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletprediction->Unlock();
	}
}

//=============================================================================
// エフェクトの設定
//=============================================================================
int SetBulletprediction(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxBulletprediction = -1;

	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		if (!g_aBulletprediction[nCntBulletprediction].bUse)
		{
			g_aBulletprediction[nCntBulletprediction].pos = pos;
			g_aBulletprediction[nCntBulletprediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBulletprediction[nCntBulletprediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aBulletprediction[nCntBulletprediction].move = move;
			g_aBulletprediction[nCntBulletprediction].col = col;
			g_aBulletprediction[nCntBulletprediction].fSizeX = fSizeX;
			g_aBulletprediction[nCntBulletprediction].fSizeY = fSizeY;
			g_aBulletprediction[nCntBulletprediction].nTimer = nTimer;
			g_aBulletprediction[nCntBulletprediction].nDecAlpha = col.a / nTimer;
			g_aBulletprediction[nCntBulletprediction].bUse = true;

			// 頂点座標の設定
			SetVertexBulletprediction(nCntBulletprediction, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorBulletprediction(nCntBulletprediction,
				D3DXCOLOR(g_aBulletprediction[nCntBulletprediction].col.r, g_aBulletprediction[nCntBulletprediction].col.g,
					g_aBulletprediction[nCntBulletprediction].col.b, g_aBulletprediction[nCntBulletprediction].col.a));

			nIdxBulletprediction = nCntBulletprediction;

			break;
		}
	}
	return nIdxBulletprediction;
}
