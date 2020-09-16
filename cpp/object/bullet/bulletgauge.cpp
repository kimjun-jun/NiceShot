//=============================================================================
//
// アタックゲージ処理 [bulletgauge.cpp]
// Author : 
//
//=============================================================================
#include "../../../h/main.h"
#include "../../../h/object/player.h"
#include "../../../h/object/bullet/bulletgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_ATTACKGAUGE		"../data/TEXTURE/UI_ATTACKGAUGE.png"		// 読み込むテクスチャファイル名
#define	ATTACKGAUGE_SIZE_X			(0.2f)							// アタックゲージの数字の幅
#define	ATTACKGAUGE_SIZE_Y			(20.0f)							// アタックゲージの数字の高さ
#define	ATTACKGAUGE_POS_X			(20.0f)							// アタックゲージの表示基準位置Ｘ座標
#define	ATTACKGAUGE_POS_Y			(225.0f)							// アタックゲージの表示基準位置Ｙ座標

#define ATTACKGAUGE_P1_POS_X		(SCREEN_W * 1 / 2 - (SCREEN_SEPARATE_BUFF)) - ATTACKGAUGE_POS_X
#define ATTACKGAUGE_P1_POS_Y		(SCREEN_H * 1 / 2) + ATTACKGAUGE_POS_Y
#define ATTACKGAUGE_P2_POS_X		(SCREEN_W * 4 / 4 - (SCREEN_SEPARATE_BUFF)) - ATTACKGAUGE_POS_X
#define ATTACKGAUGE_P2_POS_Y		(SCREEN_H * 1 / 2) + ATTACKGAUGE_POS_Y


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexAttackGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureAttackGauge = {NULL};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffAttackGauge[PLAYER_MAX] = { NULL };		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posAttackGauge[PLAYER_MAX];						// 位置

int						g_nAttackGauge[PLAYER_MAX];							// ライフ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAttackGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posAttackGauge[0] = D3DXVECTOR3(ATTACKGAUGE_P1_POS_X, ATTACKGAUGE_P1_POS_Y, 0.0f);
	g_posAttackGauge[1] = D3DXVECTOR3(ATTACKGAUGE_P2_POS_X, ATTACKGAUGE_P2_POS_Y, 0.0f);

	// ライフの初期化
	//g_nAttackGauge[0] = PLAYER_VALUE_POWER;
	//g_nAttackGauge[1] = PLAYER_VALUE_POWER;

	// 頂点情報の作成
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		MakeVertexAttackGauge(pDevice,CntPlayer);
	}
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_ATTACKGAUGE,			// ファイルの名前
								&g_pD3DTextureAttackGauge);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAttackGauge(void)
{
	if (g_pD3DTextureAttackGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureAttackGauge->Release();
		g_pD3DTextureAttackGauge = NULL;
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DVtxBuffAttackGauge[CntPlayer] != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffAttackGauge[CntPlayer]->Release();
			g_pD3DVtxBuffAttackGauge[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAttackGauge(void)
{
	//PLAYER *p =GetPlayer();
	//for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	//{
	//	if (p[CntPlayer].TeamFlag == TEAMTYPE_HIT)
	//	{
	//		g_nAttackGauge[CntPlayer] = p[CntPlayer].power;
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAttackGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//PLAYER *p = GetPlayer();
		//if (p[CntPlayer].TeamFlag == TEAMTYPE_HIT)
		//{
		//	// 頂点バッファをデバイスのデータストリームにバインド
		//	pDevice->SetStreamSource(0, g_pD3DVtxBuffAttackGauge[CntPlayer], 0, sizeof(VERTEX_2D));

		//	// 頂点フォーマットの設定
		//	pDevice->SetFVF(FVF_VERTEX_2D);

		//	// テクスチャの設定
		//	pDevice->SetTexture(0, g_pD3DTextureAttackGauge);

		//	// ポリゴンの描画
		//	for (int nCntPlace = 0; nCntPlace < g_nAttackGauge[CntPlayer]; nCntPlace++)
		//	{
		//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		//	}
		//}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexAttackGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// オブジェクトの頂点バッファを生成
 //   if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_VALUE_POWER + 4),		// 頂点データ用に確保するバッファサイズ(バイト単位)
	//											D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
	//											FVF_VERTEX_2D,								// 使用する頂点フォーマット
	//											D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
	//											&g_pD3DVtxBuffAttackGauge[CntPlayer],							// 頂点バッファインターフェースへのポインタ
	//											NULL)))										// NULLに設定
	//{
 //       return E_FAIL;
	//}

	//{//頂点バッファの中身を埋める
	//	VERTEX_2D *pVtx;

	//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	//	g_pD3DVtxBuffAttackGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

	//	for(int nCntPlace = 0; nCntPlace < PLAYER_VALUE_POWER; nCntPlace++, pVtx += 4)
	//	{
	//		// 頂点座標の設定
	//		if (CntPlayer == 0)
	//		{
	//			pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, -ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//			pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X*2, -ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//			pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//			pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X*2, ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//		}
	//		else if (CntPlayer == 1)
	//		{
	//			pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, -ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1];
	//			pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X * 2, -ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1];
	//			pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1];
	//			pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X * 2, ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1
	//			];
	//		}
	//		// rhwの設定
	//		pVtx[0].rhw =
	//		pVtx[1].rhw =
	//		pVtx[2].rhw =
	//		pVtx[3].rhw = 1.0f;

	//		// 反射光の設定
	//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//		// テクスチャ座標の設定
	//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//	}


	//	// 頂点データをアンロックする
	//	g_pD3DVtxBuffAttackGauge[CntPlayer]->Unlock();
	//}

	return S_OK;
}

