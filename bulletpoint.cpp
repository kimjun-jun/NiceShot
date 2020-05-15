//=============================================================================
//
// 頭上アイコン処理 [bulletpoint.cpp]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "camera.h"
#include "bulletpoint.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETPOINT			"data/TEXTURE/rakatiten.png"		// 読み込むテクスチャファイル名
#define	BULLETPOINT_WIDTH			(50.0f)								// アイコンの半径幅
#define	BULLETPOINT_HEIGHT			(80.0f)								// アイコンの半径幅
#define	BULLETPOINT_LEN				(300.0f)							// プレイヤーとアイコンの距離
#define	VALUE_MOVE_BULLETPOINT		(3.00f)								// 移動速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletPoint(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletPoint(int nIdxBulletPoint, float fWidth, float fHeight);
void SetColorBulletPoint(int nIdxBulletPoint);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureBulletPoint = {};					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletPoint = NULL;				// 頂点バッファインターフェースへのポインタ

BULLETPOINT					g_aBulletPoint[BULLETPOINT_MAX];

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.8f, 0.6f, 0.2f, 1.0f),//p4カラー
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBulletPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MakeVertexBulletPoint(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
		TEXTURE_BULLETPOINT,									// ファイルの名前
		&g_apD3DTextureBulletPoint);							// 読み込むメモリー

	for (int nCntBulletPoint = 0; nCntBulletPoint < BULLETPOINT_MAX; nCntBulletPoint++)
	{
		g_aBulletPoint[nCntBulletPoint].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBulletPoint[nCntBulletPoint].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBulletPoint[nCntBulletPoint].fWidth = BULLETPOINT_WIDTH;
		g_aBulletPoint[nCntBulletPoint].fHeight = BULLETPOINT_HEIGHT;
		g_aBulletPoint[nCntBulletPoint].bUse = false;
	}

	// プレイキャラ分のアイコンをセット
	for (int Cnt = 0; Cnt < BULLETPOINT_MAX; Cnt++)
	{
		// 頂点情報の作成
		SetBulletPoint(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 60.0f, 90.0f, Cnt);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBulletPoint(void)
{
	if (g_apD3DTextureBulletPoint != NULL)
	{// テクスチャの開放
		g_apD3DTextureBulletPoint->Release();
		g_apD3DTextureBulletPoint = NULL;
	}

	if (g_pD3DVtxBuffBulletPoint != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBulletPoint->Release();
		g_pD3DVtxBuffBulletPoint = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBulletPoint(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
	{
		PLAYER_HONTAI *p = GetPlayerHoudai();
		if (p[nCntPlayer].OneParameter.use)
		{
			if (g_aBulletPoint[nCntPlayer].bUse)
			{
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBulletPoint(int type)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();
	if (p[type].OneParameter.use)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX  mtxScale, mtxTranslate;

		// αテストを有効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ラインティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		if (g_aBulletPoint[type].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBulletPoint[type].mtxWorld);

			// ビューマトリックスを取得
			CAMERA *cam = GetCamera();
			g_aBulletPoint[type].mtxWorld._11 = cam[type].mtxView._11;
			g_aBulletPoint[type].mtxWorld._12 = cam[type].mtxView._21;
			g_aBulletPoint[type].mtxWorld._13 = cam[type].mtxView._31;
			g_aBulletPoint[type].mtxWorld._21 = cam[type].mtxView._12;
			g_aBulletPoint[type].mtxWorld._22 = cam[type].mtxView._22;
			g_aBulletPoint[type].mtxWorld._23 = cam[type].mtxView._32;
			g_aBulletPoint[type].mtxWorld._31 = cam[type].mtxView._13;
			g_aBulletPoint[type].mtxWorld._32 = cam[type].mtxView._23;
			g_aBulletPoint[type].mtxWorld._33 = cam[type].mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aBulletPoint[type].scl.x, g_aBulletPoint[type].scl.y, g_aBulletPoint[type].scl.z);
			D3DXMatrixMultiply(&g_aBulletPoint[type].mtxWorld, &g_aBulletPoint[type].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aBulletPoint[type].pos.x, g_aBulletPoint[type].pos.y, g_aBulletPoint[type].pos.z);
			D3DXMatrixMultiply(&g_aBulletPoint[type].mtxWorld, &g_aBulletPoint[type].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBulletPoint[type].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletPoint, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureBulletPoint);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (type * 4), POLYGON_2D_NUM);
		}
		// ラインティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBulletPoint(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * BULLETPOINT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffBulletPoint,				// 頂点バッファインターフェースへのポインタ
		NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletPoint->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntBulletPoint = 0; nCntBulletPoint < BULLETPOINT_MAX; nCntBulletPoint++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-BULLETPOINT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-BULLETPOINT_WIDTH / 2.0f, BULLETPOINT_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(BULLETPOINT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLETPOINT_WIDTH / 2.0f, BULLETPOINT_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffBulletPoint->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBulletPoint(int nIdxBulletPoint, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletPoint->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletPoint * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, fHeight);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, fHeight);

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletPoint->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorBulletPoint(int nIdxBulletPoint)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletPoint->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletPoint * 4);

		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletPoint->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetBulletPoint(D3DXVECTOR3 pos, float fWidth, float fHeight, int type)
{
	int nIdxBulletPoint = -1;

	if (!g_aBulletPoint[type].bUse)
	{
		g_aBulletPoint[type].pos = pos;
		g_aBulletPoint[type].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBulletPoint[type].fWidth = fWidth;
		g_aBulletPoint[type].fHeight = fHeight;
		g_aBulletPoint[type].bUse = true;

		// 頂点座標の設定
		SetVertexBulletPoint(type, fWidth, fHeight);

		// 頂点カラーの設定
		SetColorBulletPoint(type);

	}
}

//=============================================================================
// ヘッドアイコンを取得
//=============================================================================
BULLETPOINT *GetBulletPoint(void)
{
	return &g_aBulletPoint[0];
}


