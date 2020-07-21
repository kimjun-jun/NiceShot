///**
//* @file bullet.cpp
//* @brief NiceShot(3D)戦車ゲーム
//* @author キムラジュン
//* @date 2020/01/15
//*/
//#include "main.h"
//#include "bulletprediction.h"
//#include "input.h"
//#include "camera.h"
//#include "shadow.h"
//#include "effect.h"
//#include "explosion.h"
//#include "player.h"
//#include "life.h"
//#include "collision.h"
//#include "sound.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define	TEXTURE_BULLET_PREDICTION			"data/TEXTURE/bullet000.png"	// 読み込むテクスチャファイル名
//
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//HRESULT MakeVertexBulletPrediction(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexBulletPrediction(int nIdxBulletPrediction, float fSizeX, float fSizeY);
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//LPDIRECT3DTEXTURE9		g_pD3DTextureBulletPrediction = NULL;		// テクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletPrediction = NULL;		// 頂点バッファインターフェースへのポインタ
//
//D3DXMATRIX				g_mtxWorldBulletPrediction;				// ワールドマトリックス
//
//BULLET_PREDICTION					g_aBulletPrediction[BULLET_PREDICTION_MAX];			// 弾ワーク
//
//static D3DXCOLOR PLAYER_COLOR[] = {
//	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
//	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
//	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
//	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
//};
//
////=============================================================================
//// 初期化処理
////=============================================================================
//HRESULT InitBulletPrediction(int type)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	// 頂点情報の作成
//	MakeVertexBulletPrediction(pDevice);
//
//	if (type == 0)
//	{
//		// テクスチャの読み込み
//		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
//			TEXTURE_BULLET_PREDICTION,				// ファイルの名前
//			&g_pD3DTextureBulletPrediction);	// 読み込むメモリー
//	}
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		g_aBulletPrediction[nCntBulletPrediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//		g_aBulletPrediction[nCntBulletPrediction].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].Hormingmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].fSizeX = BULLET_PREDICTION_SIZE_X;
//		g_aBulletPrediction[nCntBulletPrediction].fSizeY = BULLET_PREDICTION_SIZE_Y;
//		g_aBulletPrediction[nCntBulletPrediction].nTimer = 0;
//		g_aBulletPrediction[nCntBulletPrediction].UsePlayerType = -1;
//		g_aBulletPrediction[nCntBulletPrediction].HormingPlayerType = -1;
//		g_aBulletPrediction[nCntBulletPrediction].use = false;
//		g_aBulletPrediction[nCntBulletPrediction].HormingSignal = false;
//		g_aBulletPrediction[nCntBulletPrediction].fRadius = 10.0f;
//		g_aBulletPrediction[nCntBulletPrediction].GravityAdd = 1.1f;
//		g_aBulletPrediction[nCntBulletPrediction].Gravity = 0.01f;
//		g_aBulletPrediction[nCntBulletPrediction].FieldPosY = 0.1f;
//		g_aBulletPrediction[nCntBulletPrediction].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_PlayerHoudai[CntPlayer].pD3DTexture = NULL;
//		g_PlayerHoudai[CntPlayer].pD3DXMesh = NULL;
//		g_PlayerHoudai[CntPlayer].pD3DXBuffMat = NULL;
//		g_PlayerHoudai[CntPlayer].nNumMat = 0;
//		g_PlayerHoudai[CntPlayer].pD3DVtxBuff = NULL;
//		g_PlayerHoudai[CntPlayer].pD3DIdxBuff = NULL;
//		g_PlayerHoudai[CntPlayer].nNumVertex = 0;
//		g_PlayerHoudai[CntPlayer].nNumPolygon = 0;
//		g_PlayerHoudai[CntPlayer].nNumVertexIndex = 0;
//
//
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// 終了処理
////=============================================================================
//void UninitBulletPrediction(void)
//{
//	if (g_pD3DTextureBulletPrediction != NULL)
//	{// テクスチャの開放
//		g_pD3DTextureBulletPrediction->Release();
//		g_pD3DTextureBulletPrediction = NULL;
//	}
//
//	if (g_pD3DVtxBuffBulletPrediction != NULL)
//	{// 頂点バッファの開放
//		g_pD3DVtxBuffBulletPrediction->Release();
//		g_pD3DVtxBuffBulletPrediction = NULL;
//	}
//}
//
////=============================================================================
//// 更新処理
////=============================================================================
//void UpdateBulletPrediction(void)
//{
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		if (g_aBulletPrediction[nCntBulletPrediction].use)
//		{
//		}
//	}
//}
//
////=============================================================================
//// 描画処理
////=============================================================================
//void DrawBulletPrediction(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
//
//	// ライティングを無効に
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// αテストを有効に
//	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
//	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		if (g_aBulletPrediction[nCntBulletPrediction].use)
//		{
//			// ワールドマトリックスの初期化
//			D3DXMatrixIdentity(&g_mtxWorldBulletPrediction);
//
//			// ビューマトリックスを取得
//			mtxView = GetCamera()->mtxView;
//
//			g_mtxWorldBulletPrediction._11 = mtxView._11;
//			g_mtxWorldBulletPrediction._12 = mtxView._21;
//			g_mtxWorldBulletPrediction._13 = mtxView._31;
//			g_mtxWorldBulletPrediction._21 = mtxView._12;
//			g_mtxWorldBulletPrediction._22 = mtxView._22;
//			g_mtxWorldBulletPrediction._23 = mtxView._32;
//			g_mtxWorldBulletPrediction._31 = mtxView._13;
//			g_mtxWorldBulletPrediction._32 = mtxView._23;
//			g_mtxWorldBulletPrediction._33 = mtxView._33;
//
//			// スケールを反映
//			D3DXMatrixScaling(&mtxScale, g_aBulletPrediction[nCntBulletPrediction].scale.x, g_aBulletPrediction[nCntBulletPrediction].scale.y, g_aBulletPrediction[nCntBulletPrediction].scale.z);
//			D3DXMatrixMultiply(&g_mtxWorldBulletPrediction, &g_mtxWorldBulletPrediction, &mtxScale);
//
//			// 移動を反映
//			D3DXMatrixTranslation(&mtxTranslate, g_aBulletPrediction[nCntBulletPrediction].pos.x, g_aBulletPrediction[nCntBulletPrediction].pos.y, g_aBulletPrediction[nCntBulletPrediction].pos.z);
//			D3DXMatrixMultiply(&g_mtxWorldBulletPrediction, &g_mtxWorldBulletPrediction, &mtxTranslate);
//
//			// ワールドマトリックスの設定
//			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBulletPrediction);
//
//			// 頂点バッファをデバイスのデータストリームにバインド
//			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletPrediction, 0, sizeof(VERTEX_3D));
//
//			// 頂点フォーマットの設定
//			pDevice->SetFVF(FVF_VERTEX_3D);
//
//			// テクスチャの設定
//			pDevice->SetTexture(0, g_pD3DTextureBulletPrediction);
//
//			// ポリゴンの描画
//			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBulletPrediction * 4), POLYGON_2D_NUM);
//		}
//	}
//
//	// ライティングを有効に
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//	// αテストを無効に
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//}
//
////=============================================================================
//// 頂点情報の作成
////=============================================================================
//HRESULT MakeVertexBulletPrediction(LPDIRECT3DDEVICE9 pDevice)
//{
//	// オブジェクトの頂点バッファを生成
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * BULLET_PREDICTION_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
//		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
//		FVF_VERTEX_3D,								// 使用する頂点フォーマット
//		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
//		&g_pD3DVtxBuffBulletPrediction,						// 頂点バッファインターフェースへのポインタ
//		NULL)))										// NULLに設定
//	{
//		return E_FAIL;
//	}
//
//	{//頂点バッファの中身を埋める
//		VERTEX_3D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffBulletPrediction->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++, pVtx += 4)
//		{
//			// 頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(-BULLET_PREDICTION_SIZE_X / 2, -BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//			pVtx[1].vtx = D3DXVECTOR3(BULLET_PREDICTION_SIZE_X / 2, -BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//			pVtx[2].vtx = D3DXVECTOR3(-BULLET_PREDICTION_SIZE_X / 2, BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//			pVtx[3].vtx = D3DXVECTOR3(BULLET_PREDICTION_SIZE_X / 2, BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//
//			// 法線の設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//
//			// 反射光の設定
//			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffBulletPrediction->Unlock();
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// 頂点座標の設定
////=============================================================================
//void SetVertexBulletPrediction(int nIdxBulletPrediction, float fSizeX, float fSizeY)
//{
//	{//頂点バッファの中身を埋める
//		VERTEX_3D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffBulletPrediction->Lock(0, 0, (void**)&pVtx, 0);
//
//		pVtx += (nIdxBulletPrediction * 4);
//
//		// 頂点座標の設定
//		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffBulletPrediction->Unlock();
//	}
//}
//
////=============================================================================
//// 弾の設定
////=============================================================================
//int SetBulletPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, int type)
//{
//	int nIdxBulletPrediction = -1;
//
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		if (!g_aBulletPrediction[nCntBulletPrediction].use)
//		{
//			g_aBulletPrediction[nCntBulletPrediction].pos = pos;
//			g_aBulletPrediction[nCntBulletPrediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			g_aBulletPrediction[nCntBulletPrediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//			g_aBulletPrediction[nCntBulletPrediction].Gravity = 0.01f;
//			g_aBulletPrediction[nCntBulletPrediction].move = move;
//			g_aBulletPrediction[nCntBulletPrediction].Hormingmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			g_aBulletPrediction[nCntBulletPrediction].fSizeX = fSizeX;
//			g_aBulletPrediction[nCntBulletPrediction].fSizeY = fSizeY;
//			g_aBulletPrediction[nCntBulletPrediction].nTimer = nTimer;
//			g_aBulletPrediction[nCntBulletPrediction].use = true;
//			g_aBulletPrediction[nCntBulletPrediction].UsePlayerType = type;
//			g_aBulletPrediction[nCntBulletPrediction].HormingSignal = false;
//			g_aBulletPrediction[nCntBulletPrediction].HormingPlayerType = -1;
//
//
//			// 影の設定
//			g_aBulletPrediction[nCntBulletPrediction].nIdxShadow = CreateShadow(pos, g_aBulletPrediction[nCntBulletPrediction].scale);		// 影の設定
//
//			// 頂点座標の設定
//			SetVertexBulletPrediction(nCntBulletPrediction, fSizeX, fSizeY);
//
//			nIdxBulletPrediction = nCntBulletPrediction;
//
//			break;
//		}
//	}
//
//	return nIdxBulletPrediction;
//}
//
////=============================================================================
//// 弾の削除
////=============================================================================
//void ReleaseBulletPrediction(int nIdxBulletPrediction)
//{
//	if (nIdxBulletPrediction >= 0 && nIdxBulletPrediction < BULLET_PREDICTION_MAX)
//	{
//		ReleaseShadow(g_aBulletPrediction[nIdxBulletPrediction].nIdxShadow);
//		g_aBulletPrediction[nIdxBulletPrediction].use = false;
//	}
//}
//
////=============================================================================
//// 弾のデータ取得
////=============================================================================
//BULLET_PREDICTION *GetBulletPrediction(void)
//{
//	return &g_aBulletPrediction[0];
//}
//
