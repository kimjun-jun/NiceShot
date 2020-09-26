/**
* @file field.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/sky.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	SKY_TEXTURE_FILENAME	"../data/TEXTURE/sky001.jpg"		// 読み込むテクスチャファイル名
#define	VALUE_MOVE_SKY		(5.0f)							// 移動速度
#define	VALUE_ROTATE_SKY	(D3DX_PI * 0.01f)				// 回転速度
#define	VALUE_TIME_SKY		(0.001f)							// 移動速度

#define	SKY_HEIGHT_RATE		(2.0f)		// 空ドームの高さ係数
#define	TEX_COUNT_LOOP		(1)			// テクスチャの繰り返し回数

int SKY::cnt = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void SKY::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 空の初期化
	//InitMeshSky(, ,, , );

	this[0].Addtime = VALUE_TIME_SKY;
	this[1].Addtime = VALUE_TIME_SKY;


	// ポリゴン表示位置の中心座標を設定
	this[0].SetPos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this[0].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	this[1].SetPos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this[1].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fRadius = 4000.0f;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		SKY_TEXTURE_FILENAME,	// ファイルの名前
		&this[0].model.pD3DTexture);	// 読み込むメモリー
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		SKY_TEXTURE_FILENAME,	// ファイルの名前
		&this[1].model.pD3DTexture);	// 読み込むメモリー


	// ブロック数の設定
	this[0].nNumBlockH = 16;
	this[0].nNumBlockV = 8;

	this[1].nNumBlockH = 16;
	this[1].nNumBlockV = 8;

	// 頂点数の設定
	this[0].model.nNumVertex = (nNumBlockH + 1) * (nNumBlockV + 1);
	// インデックス数の設定
	this[0].model.nNumVertexIndex = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);
	// ポリゴン数の設定
	this[0].model.nNumPolygon = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);
	// 回転量
	this[0].fRotY = 0.0001f;

	// 頂点数の設定
	this[1].model.nNumVertex = (nNumBlockH + 1) * (nNumBlockV + 1);
	// インデックス数の設定
	this[1].model.nNumVertexIndex = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);
	// ポリゴン数の設定
	this[1].model.nNumPolygon = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);
	// 回転量
	this[1].fRotY = 0.0001f;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&this[0].model.pD3DVtxBuff,					// 頂点バッファインターフェースへのポインタ
		NULL);								// NULLに設定

// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumBlockH + 1),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
		FVF_VERTEX_3D,							// 使用する頂点フォーマット
		D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
		&this[1].model.pD3DVtxBuff,					// 頂点バッファインターフェースへのポインタ
		NULL);								// NULLに設定

	// オブジェクトのインデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) *this[0].model.nNumVertexIndex,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&this[0].model.pD3DIdxBuff,					// インデックスバッファインターフェースへのポインタ
		NULL);								// NULLに設定

	// オブジェクトのインデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (nNumBlockH + 2),	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&this[1].model.pD3DIdxBuff,				// インデックスバッファインターフェースへのポインタ
		NULL);

	// NULLに設定
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		const float ANGLE_H = (D3DX_PI * 2.0f) / nNumBlockH;
		const float ANGLE_V = (D3DX_PI / 8.0f) / (nNumBlockV + 1);
		const float WIDTH = 1.0f / nNumBlockH;
		const float HEIGHT = 1.0f / nNumBlockV;
		float fLengthXZ;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < (nNumBlockV + 1); nCntV++)
		{
			fLengthXZ = cosf(ANGLE_V * nCntV) * fRadius;

			for (int nCntH = 0; nCntH < (nNumBlockH + 1); nCntH++, pVtx++)
			{
				// 頂点座標の設定
				pVtx->vtx.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
				pVtx->vtx.y = sinf(ANGLE_V * nCntV) * fRadius * SKY_HEIGHT_RATE;
				pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

				// 法線の設定
				pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 頂点カラーの設定
				pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

				if (nCntV == nNumBlockV)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * HEIGHT;
				}
			}
		}

		// 頂点データをアンロックする
		this[0].model.pD3DVtxBuff->Unlock();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[1].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx->vtx.x = 0.0f;
		pVtx->vtx.y = sinf(ANGLE_V * (nNumBlockV + 1)) * fRadius * SKY_HEIGHT_RATE;
		pVtx->vtx.z = 0.0f;

		// 法線の設定
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;

		pVtx++;

		fLengthXZ = cosf(ANGLE_V * nNumBlockV) * fRadius;
		for (int nCntH = 0; nCntH < nNumBlockH; nCntH++, pVtx++)
		{
			// 頂点座標の設定
			pVtx->vtx.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
			pVtx->vtx.y = sinf(ANGLE_V * nNumBlockV) * fRadius * SKY_HEIGHT_RATE;
			pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// 法線の設定
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx->tex.x = 0.01f;
			pVtx->tex.y = 0.01f;
		}

		// 頂点データをアンロックする
		this[1].model.pD3DVtxBuff->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntV = 0; nCntV < nNumBlockV; nCntV++)
		{
			if (nCntV != 0)
			{
				*pIdx = nCntV * (nNumBlockH + 1);
				pIdx++;
			}

			for (int nCntH = 0; nCntH < nNumBlockH + 1; nCntH++)
			{
				*pIdx = nCntV * (nNumBlockH + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (nNumBlockH + 1) + nCntH;
				pIdx++;
			}

			if (nCntV != (nNumBlockV - 1))
			{
				*pIdx = (nCntV + 1) * (nNumBlockH + 1);
				pIdx++;
			}
		}

		// インデックスデータをアンロックする
		this[0].model.pD3DIdxBuff->Unlock();

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		this[1].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntH = 0; nCntH < nNumBlockH + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// インデックスデータをアンロックする
		this[1].model.pD3DIdxBuff->Unlock();
	}

}

//=============================================================================
// 再初期化処理
//=============================================================================
void SKY::Reinit(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < (nNumBlockV + 1); nCntV++)
		{

			for (int nCntH = 0; nCntH < (nNumBlockH + 1); nCntH++, pVtx++)
			{
				// 頂点カラーの設定
				pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		// 頂点データをアンロックする
		this[0].model.pD3DVtxBuff->Unlock();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[1].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx++;

		for (int nCntH = 0; nCntH < nNumBlockH; nCntH++, pVtx++)
		{
			// 頂点カラーの設定
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		this[1].model.pD3DVtxBuff->Unlock();
	}

	this[0].fRotY = 0.0f;
	this[0].time = 0.0f;
	this[1].fRotY = 0.0f;
	this[1].time = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void SKY::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void SKY::Update(void)
{
	for (int nCntSky = 0; nCntSky < OBJECT_SKY_MAX; nCntSky++)
	{
		D3DXVECTOR3 rot=this[nCntSky].GetRot();
		rot.y += D3DX_PI * this[nCntSky].fRotY;

		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		this[nCntSky].SetRot(rot);
	}


		{//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			this[0].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntV = 0; nCntV < (this[0].nNumBlockV + 1); nCntV++)
			{
				for (int nCntH = 0; nCntH < (this[0].nNumBlockH + 1); nCntH++, pVtx++)
				{
					// 頂点カラーの設定
						pVtx->diffuse = D3DXCOLOR(1.0f*this[0].time, 1.0f*this[0].time, 1.0f*this[0].time, 1.0f);
				}
			}

			// 頂点データをアンロックする
			this[0].model.pD3DVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			this[1].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 法線の設定
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
				pVtx->diffuse = D3DXCOLOR(1.0f*this[1].time, 1.0f*this[1].time, 1.0f*this[1].time, 1.0f);

			pVtx++;

			for (int nCntH = 0; nCntH < this[0].nNumBlockH; nCntH++, pVtx++)
			{
				// 頂点カラーの設定
					pVtx->diffuse = D3DXCOLOR(1.0f*this[1].time, 1.0f*this[1].time, 1.0f*this[1].time, 1.0f);
			}

			// 頂点データをアンロックする
			this[1].model.pD3DVtxBuff->Unlock();
		}


		this[0].time += this[0].Addtime;
		this[1].time += this[1].Addtime;


	if (this[0].time >= 1.0 || this[0].time <= -1.0) this[0].Addtime *= -1.0f;
	if (this[1].time >= 1.0 || this[1].time <= -1.0) this[1].Addtime *= -1.0f;
}

//=============================================================================
// 描画処理
//=============================================================================
void SKY::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD dwSettingLighting, dwSettingCullmode;

	pDevice->GetRenderState(D3DRS_LIGHTING, &dwSettingLighting);
	pDevice->GetRenderState(D3DRS_CULLMODE, &dwSettingCullmode);

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	// ワールドマトリックスの初期化
	mtxWorld = this[0].GetMatrix();
	D3DXVECTOR3 pos = this[0].GetPos();
	D3DXVECTOR3 rot = this[0].GetRot();

	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y,
		rot.x,
		rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x,
		pos.y,
		pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, this[0].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(this[0].model.pD3DIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, this[0].model.pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this[0].model.nNumVertex, 0, this[0].model.nNumPolygon);


	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, this[1].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(this[1].model.pD3DIdxBuff);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (this[1].nNumBlockH + 2), 0, this[1].nNumBlockH);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング

	pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, dwSettingLighting);
	pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
}

