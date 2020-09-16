/**
* @file bulletgauge.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/object/player.h"
#include "../../../h/object/bullet/bulletgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETGAUGE			"../data/TEXTURE/UI_BULLETGAUGE.png"		// 読み込むテクスチャファイル名
#define	BULLETGAUGE_SIZE_X			(15.0f)										// アタックゲージの数字の幅
#define	BULLETGAUGE_SIZE_Y			(20.0f)										// アタックゲージの数字の高さ
#define	BULLETGAUGE_POS_X			(18.0f)										// アタックゲージの表示基準位置Ｘ座標
#define	BULLETGAUGE_POS_Y			(30.0f)										// アタックゲージの表示基準位置Ｙ座標

#define BULLETGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletGauge = {NULL};						// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletGauge[PLAYER_MAX] = { NULL };		// 頂点バッファインターフェースへのポインタ
D3DXVECTOR3				g_posBulletGauge[PLAYER_MAX];							// 位置
int						g_nBulletGauge[PLAYER_MAX];								// 弾薬

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posBulletGauge[0] = D3DXVECTOR3(BULLETGAUGE_P1_POS_X, BULLETGAUGE_P1_POS_Y, 0.0f);
	g_posBulletGauge[1] = D3DXVECTOR3(BULLETGAUGE_P2_POS_X, BULLETGAUGE_P2_POS_Y, 0.0f);
	g_posBulletGauge[2] = D3DXVECTOR3(BULLETGAUGE_P3_POS_X, BULLETGAUGE_P3_POS_Y, 0.0f);
	g_posBulletGauge[3] = D3DXVECTOR3(BULLETGAUGE_P4_POS_X, BULLETGAUGE_P4_POS_Y, 0.0f);

	// 弾薬の初期化
	g_nBulletGauge[0] = PLAYER_AMMOPOWER_NORMAL;
	g_nBulletGauge[1] = PLAYER_AMMOPOWER_NORMAL;
	g_nBulletGauge[2] = PLAYER_AMMOPOWER_NORMAL;
	g_nBulletGauge[3] = PLAYER_AMMOPOWER_NORMAL;

	// 頂点情報の作成
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		MakeVertexBulletGauge(pDevice,CntPlayer);
	}
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_BULLETGAUGE,			// ファイルの名前
								&g_pD3DTextureBulletGauge);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBulletGauge(void)
{
	if (g_pD3DTextureBulletGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureBulletGauge->Release();
		g_pD3DTextureBulletGauge = NULL;
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DVtxBuffBulletGauge[CntPlayer] != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffBulletGauge[CntPlayer]->Release();
			g_pD3DVtxBuffBulletGauge[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBulletGauge(void)
{
	PLAYER_HONTAI *p =GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
			g_nBulletGauge[CntPlayer] = p[CntPlayer].AmmoNum;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletGauge[CntPlayer], 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureBulletGauge);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < g_nBulletGauge[CntPlayer]; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_AMMOPOWER_STRONG + 4),		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_2D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffBulletGauge[CntPlayer],							// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < PLAYER_AMMOPOWER_STRONG; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			if (CntPlayer == 0)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X *2, -BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X *2, BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
			}
			else if (CntPlayer == 1)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
			}
			else if (CntPlayer == 2)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
			}
			else if (CntPlayer == 3)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
			}

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffBulletGauge[CntPlayer]->Unlock();
	}

	return S_OK;
}

