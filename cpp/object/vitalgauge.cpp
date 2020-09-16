/**
* @file vitalgauge.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/object/player.h"
#include "../../h/scene/rank.h"
#include "../../../h/object/vitalgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_VITALGAUGE		"../data/TEXTURE/UI_VITALGAUGE.png"		// 読み込むテクスチャファイル名
#define	VITALGAUGE_SIZE_X			(1.0f)							// アタックゲージの数字の幅
#define	VITALGAUGE_SIZE_Y			(30.0f)							// アタックゲージの数字の高さ
#define	VITALGAUGE_POS_X			(25.0f)							// アタックゲージの表示基準位置Ｘ座標
#define	VITALGAUGE_POS_Y			(70.0f)							// アタックゲージの表示基準位置Ｙ座標

#define VITALGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureVitalGauge = {NULL};					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffVitalGauge[PLAYER_MAX] = { NULL };		// 頂点バッファインターフェースへのポインタ
D3DXVECTOR3				g_posVitalGauge[PLAYER_MAX];						// 位置
int						g_nVitalGauge[PLAYER_MAX];							// ライフ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitVitalGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posVitalGauge[0] = D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f);
	g_posVitalGauge[1] = D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f);
	g_posVitalGauge[2] = D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f);
	g_posVitalGauge[3] = D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f);

	// ライフの初期化
	g_nVitalGauge[0] = PLAYER_VITAL;
	g_nVitalGauge[1] = PLAYER_VITAL;
	g_nVitalGauge[2] = PLAYER_VITAL;
	g_nVitalGauge[3] = PLAYER_VITAL;

	// 頂点情報の作成
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		MakeVertexVitalGauge(pDevice,CntPlayer);
	}
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_VITALGAUGE,			// ファイルの名前
								&g_pD3DTextureVitalGauge);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitVitalGauge(void)
{
	if (g_pD3DTextureVitalGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureVitalGauge->Release();
		g_pD3DTextureVitalGauge = NULL;
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DVtxBuffVitalGauge[CntPlayer] != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffVitalGauge[CntPlayer]->Release();
			g_pD3DVtxBuffVitalGauge[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateVitalGauge(void)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (p[CntPlayer].use == true)
		{
			if (g_nVitalGauge[CntPlayer] <= 0)
			{
				p[CntPlayer].use = false;
				SetRank(CntPlayer);
			}
		}
		g_nVitalGauge[CntPlayer] = p[CntPlayer].vital;
		//体力一定以上で緑色
		if (g_nVitalGauge[CntPlayer] >= PLAYER_VITAL * (0.2f))
		{
			{//頂点バッファの中身を埋める
				VERTEX_2D *pVtx;
				// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
				g_pD3DVtxBuffVitalGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// 反射光の設定
					pVtx[0].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
				}
			}
			// 頂点データをアンロックする
			g_pD3DVtxBuffVitalGauge[CntPlayer]->Unlock();
		}
		//体力一定以下で赤色
		else
		{
			{//頂点バッファの中身を埋める
				VERTEX_2D *pVtx;
				// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
				g_pD3DVtxBuffVitalGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// 反射光の設定
					pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
				}
			}
			// 頂点データをアンロックする
			g_pD3DVtxBuffVitalGauge[CntPlayer]->Unlock();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawVitalGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffVitalGauge[CntPlayer], 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureVitalGauge);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < g_nVitalGauge[CntPlayer]; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_VITAL + 4),		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_2D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffVitalGauge[CntPlayer],							// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffVitalGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			if (CntPlayer == 0)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, -VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
			}
			else if (CntPlayer == 1)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
			}
			else if (CntPlayer == 2)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
			}
			else if (CntPlayer == 3)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
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
		g_pD3DVtxBuffVitalGauge[CntPlayer]->Unlock();
	}

	return S_OK;
}

