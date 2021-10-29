///**
//* @file life.cpp
//* @brief NiceShot(3D)戦車ゲーム
//* @author キムラジュン
//*/
//#include "../../h/main.h"
//#include "../../h/object/player.h"
//#include "../../h/other/sound.h"
//#include "../../h/scene/rank.h"
//#include "../../h/object/life.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define	TEXTURE_LIFETEX		"../data/TEXTURE/life000.png"		// 読み込むテクスチャファイル名
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//HRESULT MakeVertexLifeTex(LPDIRECT3DDEVICE9 pDevice, int type);
//
////*****************************************************************************
//// グローバル変数宣言
////*****************************************************************************
//LPDIRECT3DTEXTURE9		g_pD3DTextureLifeTex;		// テクスチャへのポインタ
//
//LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLifeTex[PLAYER_MAX];	// 頂点バッファインターフェースへのポインタ
//
//D3DXVECTOR3				g_posLifeTex;						// 位置
//D3DXVECTOR3				g_rotLifeTex;						// 向き
//
//int						g_nLifeTex[PLAYER_MAX];			// バレット残弾
//int						g_nNumVertexLifeTex;			// 頂点の総数
//int						g_nNumPolygonLifeTex;			// ポリゴンの総数
//int						g_nNumIndexLifeTex;				// インデックスの総数
//
//
////=============================================================================
//// 初期化処理
////=============================================================================
//HRESULT InitLifeTex(int type)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	if (type == 0)
//	{
//		// テクスチャの読み込み
//		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
//			TEXTURE_LIFETEX,			// ファイルの名前
//			&g_pD3DTextureLifeTex);	// 読み込むメモリー
//	}
//
//	g_nNumVertexLifeTex = (POLYGON_2D_VERTEX * MAX_LIFE) - 2;
//	g_nNumPolygonLifeTex = MAX_LIFE * 2;
//	g_nNumIndexLifeTex = g_nNumPolygonLifeTex;
//	g_posLifeTex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	g_rotLifeTex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		// バレット残弾の初期化
//		g_nLifeTex[CntPlayer] = MAX_LIFE;
//		g_pD3DVtxBuffLifeTex[CntPlayer] = NULL;
//		// 頂点情報の作成
//		MakeVertexLifeTex(pDevice, CntPlayer);
//
//	}
//
//
//	return S_OK;
//}
//
////=============================================================================
//// 終了処理
////=============================================================================
//void UninitLifeTex(void)
//{
//	if (g_pD3DTextureLifeTex != NULL)
//	{// テクスチャの開放
//		g_pD3DTextureLifeTex->Release();
//		g_pD3DTextureLifeTex = NULL;
//	}
//
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		if (g_pD3DVtxBuffLifeTex[CntPlayer] != NULL)
//		{// 頂点バッファの開放
//			g_pD3DVtxBuffLifeTex[CntPlayer]->Release();
//			g_pD3DVtxBuffLifeTex[CntPlayer] = NULL;
//		}
//	}
//}
//
////=============================================================================
//// 更新処理
////=============================================================================
//void UpdateLifeTex(void)
//{
//	PLAYER_HONTAI *p = GetPlayerHoudai();
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		if (p[CntPlayer].use == true)
//		{
//			if (g_nLifeTex[CntPlayer] <= 0)
//			{
//				p[CntPlayer].use = false;
//				SetRank(CntPlayer);
//			}
//		}
//	}
//}
//
////=============================================================================
//// 描画処理
////=============================================================================
//void DrawLifeTex(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		// 頂点バッファをデバイスのデータストリームにバインド
//		pDevice->SetStreamSource(0, g_pD3DVtxBuffLifeTex[CntPlayer], 0, sizeof(VERTEX_2D));
//
//		// 頂点フォーマットの設定
//		pDevice->SetFVF(FVF_VERTEX_2D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, g_pD3DTextureLifeTex);
//
//		// ポリゴンの描画
//		for (int nCntPlace = 0; nCntPlace < g_nLifeTex[CntPlayer]; nCntPlace++)
//		{
//			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
//		}
//	}
//}
//
////=============================================================================
//// 頂点の作成
////=============================================================================
//HRESULT MakeVertexLifeTex(LPDIRECT3DDEVICE9 pDevice, int type)
//{
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * MAX_LIFE + 4),		// 頂点データ用に確保するバッファサイズ(バイト単位)
//		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
//		FVF_VERTEX_2D,								// 使用する頂点フォーマット
//		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
//		&g_pD3DVtxBuffLifeTex[type],							// 頂点バッファインターフェースへのポインタ
//		NULL)))										// NULLに設定
//	{
//		return E_FAIL;
//	}
//
//	{//頂点バッファの中身を埋める
//		VERTEX_2D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffLifeTex[type]->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int CntPlace = 0; CntPlace < MAX_LIFE; CntPlace++, pVtx += 4)
//		{
//			//1プレイヤーの時は画面左上
//			if (type == PLAYER01)
//			{
//				// 頂点座標の設定
//				pVtx[0].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//			}
//			//2プレイヤーの時は画面右上
//			else if (type == PLAYER02)
//			{
//				// 頂点座標の設定
//				pVtx[0].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//			}
//			else if (type == PLAYER03)
//			{
//				// 頂点座標の設定
//				pVtx[0].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//			}
//			else if (type == PLAYER04)
//			{
//				// 頂点座標の設定
//				pVtx[0].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//			}
//			// rhwの設定
//			pVtx[0].rhw =
//				pVtx[1].rhw =
//				pVtx[2].rhw =
//				pVtx[3].rhw = 1.0f;
//
//			// 反射光の設定
//			pVtx[0].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);//ピンク色
//			pVtx[1].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffLifeTex[type]->Unlock();
//	}
//	return S_OK;
//}
//
////=============================================================================
//// バレット残弾の変更
////=============================================================================
//void ChangeLife(int nValue, int type)
//{
//	g_nLifeTex[type] += nValue;
//	if (g_nLifeTex[type] < 0)
//	{
//		g_nLifeTex[type] = 0;
//	}
//	else if (g_nLifeTex[type] >= MAX_LIFE)
//	{
//		g_nLifeTex[type] = MAX_LIFE;
//	}
//
//}
