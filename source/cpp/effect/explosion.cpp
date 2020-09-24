/**
* @file explosion.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/effect/explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EXPLOSION		"../data/TEXTURE/bomb.png"		// 読み込むテクスチャファイル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureExplosion = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExplosion = NULL;	// 頂点バッファインターフェースへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
void EXPLOSION::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexExplosion(pDevice);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_EXPLOSION,			// ファイルの名前
			&g_pD3DTextureExplosion);	// 読み込むメモリー
}

//=============================================================================
// 再初期化処理
//=============================================================================
void EXPLOSION::Reinit(void)
{
	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		this[nCntExplosion].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntExplosion].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntExplosion].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[nCntExplosion].SetCol(D3DXCOLOR(DWORD(0)));
		this[nCntExplosion].nCounter = 0;
		this[nCntExplosion].nPatternX = 0;
		this[nCntExplosion].nPatternY = 0;
		this[nCntExplosion].fSizeX = 0;
		this[nCntExplosion].fSizeY = 0;
		this[nCntExplosion].nType = nType;
		this[nCntExplosion].SetUse(false);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void EXPLOSION::Uninit(void)
{
	if(g_pD3DTextureExplosion != NULL)
	{// テクスチャの開放
		g_pD3DTextureExplosion->Release();
		g_pD3DTextureExplosion = NULL;
	}

	if(g_pD3DVtxBuffExplosion != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffExplosion->Release();
		g_pD3DVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void EXPLOSION::Update(void)
{
	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this[nCntExplosion].GetUse();
		if(use)
		{
			this[nCntExplosion].nCounter--;
			if((this[nCntExplosion].nCounter % 4) == 0)
			{
				this[nCntExplosion].nPatternX++;
				if(this[nCntExplosion].nPatternY >= 4)
				{
					this[nCntExplosion].SetUse(false);
				}
				else if (this[nCntExplosion].nPatternX >= 4)
				{
					this[nCntExplosion].nPatternX = 0;
					this[nCntExplosion].nPatternY++;
				}
				else
				{
					// テクスチャ座標の設定
					SetTextureExplosion(nCntExplosion, this[nCntExplosion].nPatternX, this[nCntExplosion].nPatternY);
				}
			}

			// 頂点座標の設定
			this[nCntExplosion].fSizeX += 0.50f;
			this[nCntExplosion].fSizeY += 0.50f;
			SetVertexExplosion(nCntExplosion, this[nCntExplosion].fSizeX, this[nCntExplosion].fSizeY);

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void EXPLOSION::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this[nCntExplosion].GetUse();
		if (use)
		{
			//--------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[nCntExplosion].GetPos();
			D3DXVECTOR3 scl = this[nCntExplosion].GetScl();
			D3DXCOLOR col = this[nCntExplosion].GetCol();
			D3DXMATRIX mtxWorldExplosion = this[nCntExplosion].GetMatrix();

			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				D3DXMATRIX mtxView, mtxScale, mtxTranslate;

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorldExplosion);

				// ビューマトリックスを取得
				CAMERA *cam = GetCamera();

				mtxWorldExplosion._11 = cam[CntPlayer].mtxView._11;
				mtxWorldExplosion._12 = cam[CntPlayer].mtxView._21;
				mtxWorldExplosion._13 = cam[CntPlayer].mtxView._31;
				mtxWorldExplosion._21 = cam[CntPlayer].mtxView._12;
				mtxWorldExplosion._22 = cam[CntPlayer].mtxView._22;
				mtxWorldExplosion._23 = cam[CntPlayer].mtxView._32;
				mtxWorldExplosion._31 = cam[CntPlayer].mtxView._13;
				mtxWorldExplosion._32 = cam[CntPlayer].mtxView._23;
				mtxWorldExplosion._33 = cam[CntPlayer].mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldExplosion);

				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, g_pD3DVtxBuffExplosion, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureExplosion);

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntExplosion * 4), POLYGON_2D_NUM);

				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			}
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT EXPLOSION::MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * OBJECT_EXPLOSION_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_3D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffExplosion,					// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
		g_pD3DVtxBuffExplosion->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void EXPLOSION::SetVertexExplosion(int nIdxExplosion, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxExplosion * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void EXPLOSION::SetColorExplosion(int nIdxExplosion, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxExplosion * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void EXPLOSION::SetTextureExplosion(int nIdxExplosion, int nPatternX, int nPatternY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxExplosion * 4);

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(nPatternX * 0.25f, nPatternY * 0.25f);
		pVtx[1].tex = D3DXVECTOR2((nPatternX + 1) * 0.25f, nPatternY * 0.25f);
		pVtx[2].tex = D3DXVECTOR2(nPatternX * 0.25f, (nPatternY + 1) * 0.25f);
		pVtx[3].tex = D3DXVECTOR2((nPatternX + 1) * 0.25f, (nPatternY + 1) * 0.25f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int EXPLOSION::SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType, D3DXCOLOR col)
{
	int nIdxExplosion = -1;

	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this[nCntExplosion].GetUse();
		if (use!=true)
		{
			this[nCntExplosion].SetPos(pos);
			this[nCntExplosion].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this[nCntExplosion].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this[nCntExplosion].SetCol(col);
			this[nCntExplosion].SetUse(true);
			this[nCntExplosion].fSizeX = fSizeX;
			this[nCntExplosion].fSizeY = fSizeY;
			this[nCntExplosion].nCounter = 0;
			this[nCntExplosion].nPatternX = 0;
			this[nCntExplosion].nPatternY = 0;
			this[nCntExplosion].nType = nType;

			// 頂点座標の設定
			SetVertexExplosion(nCntExplosion, fSizeX, fSizeY);

			SetColorExplosion(nCntExplosion, col);

			// テクスチャ座標の設定
			SetTextureExplosion(nCntExplosion, 0,0);

			nIdxExplosion = nCntExplosion;

			break;
		}
	}

	return nIdxExplosion;
}
