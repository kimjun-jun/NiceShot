/**
* @file bulletprediction.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "main.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "bulletprediction.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETPREDICTION		"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	BULLETPREDICTION_SIZE_X			(5.0f)							// ビルボードの幅
#define	BULLETPREDICTION_SIZE_Y			(5.0f)							// ビルボードの高さ

#define	MAX_BULLETPREDICTION			(1000)							// エフェクト最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;									//!< ワールドマトリックス
	D3DXVECTOR3 pos;										//!< 位置
	D3DXVECTOR3 rot;										//!< 回転
	D3DXVECTOR3 scale;										//!< スケール
	D3DXCOLOR	col;										//!< 色
	float		fSizeX;										//!< 幅
	float		fSizeY;										//!< 高さ
	bool		bUse;										//!< 使用しているかどうか
} BULLETPREDICTION;

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.2f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.2f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.2f),//p4カラー
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY);
void SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletprediction[PLAYER_MAX] = { NULL };		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletprediction[PLAYER_MAX] = { NULL };		// 頂点バッファインターフェースへのポインタ

BULLETPREDICTION		g_aBulletprediction[PLAYER_MAX][MAX_BULLETPREDICTION];	// 弾ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBulletprediction(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexBulletprediction(pDevice);
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_BULLETPREDICTION,			// ファイルの名前
			&g_pD3DTextureBulletprediction[CntPlayer]);	// 読み込むメモリー
	}
		for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
		{

			g_aBulletprediction[CntPlayer][nCntBulletprediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].fSizeX = BULLETPREDICTION_SIZE_X;
			g_aBulletprediction[CntPlayer][nCntBulletprediction].fSizeY = BULLETPREDICTION_SIZE_Y;
			g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse = false;
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBulletprediction(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DTextureBulletprediction[CntPlayer] != NULL)
		{// テクスチャの開放
			g_pD3DTextureBulletprediction[CntPlayer]->Release();
			g_pD3DTextureBulletprediction[CntPlayer] = NULL;
		}

		if (g_pD3DVtxBuffBulletprediction[CntPlayer] != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffBulletprediction[CntPlayer]->Release();
			g_pD3DVtxBuffBulletprediction[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBulletprediction(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
		{
			if (g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse)
			{
				g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse = false;
			}
		}
	}
	PLAYER_HONTAI *player = GetPlayerHoudai();

	//プレイヤーの情報から発射位置角度移動量を利用してバレットの着弾点を算出する
	for (int CntPlayer=0;CntPlayer<PLAYER_MAX;CntPlayer++)
	{
		D3DXVECTOR3	BulletPredictionPos = player[CntPlayer].BposStart;
		float Gravity = 0.0f;
		float time = 1.0f;
		float maxtime = 10.0f;
		do
		{
			//重力と移動量を反映
			//BulletPredictionPos.x += player[CntPlayer].bulletmove.x*(time / maxtime);
			//BulletPredictionPos.y -= player[CntPlayer].bulletmove.y*(time / maxtime) + Gravity;
			//BulletPredictionPos.z += player[CntPlayer].bulletmove.z*(time / maxtime);

			BulletPredictionPos.x += player[CntPlayer].bulletmove.x;
			BulletPredictionPos.y -= player[CntPlayer].bulletmove.y + Gravity;
			BulletPredictionPos.z += player[CntPlayer].bulletmove.z;

			//重力を加速させる
			Gravity += VALUE_GRAVITYADD_BULLET;
			//時間割合を進める。飛距離が増えるほど弾道予測間隔が広くなる
			time++;
			//重力最大値制限
			if (Gravity > VALUE_GRAVITYMAX_BULLET) Gravity = VALUE_GRAVITYMAX_BULLET;

			SetBulletprediction(BulletPredictionPos, CntPlayer, PLAYER_COLOR[CntPlayer], BULLETPREDICTION_SIZE_X, BULLETPREDICTION_SIZE_Y);


		} while (BulletPredictionPos.y >= 0.0f);
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBulletprediction(int CntPlayer)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();
	if (p[CntPlayer].use)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX mtxView, mtxScale, mtxTranslate;

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// Z比較なし
		//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
		{
			if (g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld);

				// ビューマトリックスを取得
				CAMERA *cam = GetCamera();

				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._11 = cam[CntPlayer].mtxView._11;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._12 = cam[CntPlayer].mtxView._21;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._13 = cam[CntPlayer].mtxView._31;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._21 = cam[CntPlayer].mtxView._12;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._22 = cam[CntPlayer].mtxView._22;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._23 = cam[CntPlayer].mtxView._32;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._31 = cam[CntPlayer].mtxView._13;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._32 = cam[CntPlayer].mtxView._23;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._33 = cam[CntPlayer].mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, g_aBulletprediction[CntPlayer][nCntBulletprediction].scale.x, g_aBulletprediction[CntPlayer][nCntBulletprediction].scale.y, g_aBulletprediction[CntPlayer][nCntBulletprediction].scale.z);
				D3DXMatrixMultiply(&g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, g_aBulletprediction[CntPlayer][nCntBulletprediction].pos.x, g_aBulletprediction[CntPlayer][nCntBulletprediction].pos.y, g_aBulletprediction[CntPlayer][nCntBulletprediction].pos.z);
				D3DXMatrixMultiply(&g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld);

				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletprediction[CntPlayer], 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureBulletprediction[CntPlayer]);

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
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * MAX_BULLETPREDICTION,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_3D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&g_pD3DVtxBuffBulletprediction[CntPlayer],						// 頂点バッファインターフェースへのポインタ
			NULL)))										// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pD3DVtxBuffBulletprediction[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

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
			g_pD3DVtxBuffBulletprediction[CntPlayer]->Unlock();
		}
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletprediction[PlayerType]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletprediction[PlayerType]->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletprediction[PlayerType]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletprediction[PlayerType]->Unlock();
	}
}

//=============================================================================
// エフェクトの設定
//=============================================================================
int SetBulletprediction(D3DXVECTOR3 pos, int PlayerType,D3DXCOLOR col, float fSizeX, float fSizeY)
{
	int nIdxBulletprediction = -1;

	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		if (!g_aBulletprediction[PlayerType][nCntBulletprediction].bUse)
		{
			g_aBulletprediction[PlayerType][nCntBulletprediction].pos = pos;
			g_aBulletprediction[PlayerType][nCntBulletprediction].col = col;
			g_aBulletprediction[PlayerType][nCntBulletprediction].fSizeX = fSizeX;
			g_aBulletprediction[PlayerType][nCntBulletprediction].fSizeY = fSizeY;
			g_aBulletprediction[PlayerType][nCntBulletprediction].bUse = true;

			// 頂点座標の設定
			SetVertexBulletprediction(PlayerType,nCntBulletprediction, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorBulletprediction(PlayerType,nCntBulletprediction,
				D3DXCOLOR(g_aBulletprediction[PlayerType][nCntBulletprediction].col.r,g_aBulletprediction[PlayerType][nCntBulletprediction].col.g,
					g_aBulletprediction[PlayerType][nCntBulletprediction].col.b, g_aBulletprediction[PlayerType][nCntBulletprediction].col.a));

			nIdxBulletprediction = nCntBulletprediction;

			break;
		}
	}
	return nIdxBulletprediction;
}
