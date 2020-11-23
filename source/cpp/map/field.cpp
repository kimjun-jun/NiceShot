/**
* @file field.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/field.h"
#include "../../h/object/item.h"
#include "../../h/object/bullet/bullet.h"
#include "../../h/other/sound.h"
#include "../../h/effect/explosion.h"
#include "../../h/object/player.h"
#include "../../h/library.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME				"../data/TEXTURE/white.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_NOISE					"../data/TEXTURE/noise.bmp"		// 読み込むテクスチャファイル名
#define SPHERELINEAR_LEN				(8.0f)							//!< 球面線形補間で使う、プレイヤーの座標と3角ポリゴン辺までの距離(プレイヤーの座標がこの値より辺に近ければ球面線形補間開始する)

#define FIELD_Y_HIGH					(120.0f)						//!< フィールドの高さで色を変える。高い
#define FIELD_Y_LOW						(20.0f)							//!< フィールドの高さで色を変える。低い

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4カラー
};

int FIELD::cnt = 0;


//=============================================================================
// 初期化処理
//=============================================================================
void FIELD::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this[0].InterPolationFieldSignal = true;
	this[0].InterPolationFieldSignalEnd = false;

	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldPlayerNum = -1;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,								// ファイルの名前
		&this[0].model.pD3DTexture);							// 読み込むメモリー

	// ブロック数の設定
	this[0].nNumBlockXField = 32;
	this[0].nNumBlockZField = 32;

	// 頂点数の設定
	this[0].model.nNumVertex = (nNumBlockXField + 1) * (nNumBlockZField + 1);


	// インデックス数の設定
	this[0].model.nNumVertexIndex = (nNumBlockXField + 1) * 2 * nNumBlockZField + (nNumBlockZField - 1) * 2;

	// ポリゴン数の設定
	this[0].model.nNumPolygon = nNumBlockXField * nNumBlockZField * 2 + (nNumBlockZField - 1) * 4;

	// ブロックサイズの設定
	this[0].fBlockSizeXField = 250.0f;
	this[0].fBlockSizeZField = 250.0f;

	//　辺サイズの設定
	fSideSizeXMax = nNumBlockXField * fBlockSizeXField;
	fSideSizeZMax = nNumBlockZField * fBlockSizeZField;
	fSideSizeXHalf = fSideSizeXMax / 2.0f;
	fSideSizeZHalf = fSideSizeZMax / 2.0f;
	fSideSizeXQuarter = fSideSizeXHalf / 2.0f;
	fSideSizeZQuarter = fSideSizeZHalf / 2.0f;
	fSideSizeXEighth = fSideSizeXQuarter / 2.0f;
	fSideSizeZEighth = fSideSizeZQuarter / 2.0f;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffFieldDraw,				// 頂点バッファインターフェースへのポインタ
		NULL);								// NULLに設定
	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffFieldEnd,				// 頂点バッファインターフェースへのポインタ
		NULL);								// NULLに設定

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffFieldStart,				// 頂点バッファインターフェースへのポインタ
		NULL);							// NULLに設定

	// オブジェクトのインデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * this[0].model.nNumVertexIndex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&this[0].model.pD3DIdxBuff,				// インデックスバッファインターフェースへのポインタ
		NULL);								// NULLに設定

	//描画用
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldStart->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < this[0].nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this[0].nNumBlockZField + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (this[0].nNumBlockZField - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (this[0].nNumBlockZField + 1) + this[0].nNumBlockZField;
				nCntIdx++;
			}
		}

		// インデックスデータをアンロックする
		this[0].model.pD3DIdxBuff->Unlock();
	}

	//描画用
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		//上限
		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1)/2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1)/2; nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = float((rand() % 200));
				//pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = 0.0f;
			}
		}

		//縮退ポリゴンの座標を調整
		SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
					//pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				}
				//中側
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
				}

				// 反射光の設定
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
				}
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
				}
				else
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
				}
			}
		}

		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldStart->Unlock();
	}



	//自動生成のバッファ。地形変更するときにこの形に変形	F
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}
		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}


	//当たり判定高速化用。一応親子関係も登録
	InitCntPartition();

}

//=============================================================================
// 再初期化処理
//=============================================================================
void FIELD::Reinit(void)
{
	this[0].InterPolationFieldSignal = true;
	this[0].InterPolationFieldSignalEnd = false;
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldPlayerNum = -1;

	//--------------------------------DRAWとSTARTのセット
	//描画用
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		//高さクリア
		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx .y= 0.0f;
			}
		}

		//上限
		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1) / 2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1) / 2; nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				//高さを仮決定
				pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = float((rand() % 200));
			}
		}

		//縮退ポリゴンの座標を調整
		//SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
				//中側
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
				}

				// 反射光の設定
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
				}
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
				}
				else
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
				}
			}
		}

		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldStart->Unlock();
	}
	//----------------------------ENDのセット
	//自動生成のバッファ。地形変更するときにこの形に変形	F
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}
		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void FIELD::Uninit(void)
{
	this[0].pD3DVtxBuffFieldStart->Release();
	this[0].pD3DVtxBuffFieldEnd->Release();
	this[0].pD3DVtxBuffFieldDraw->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void FIELD::Update(PLAYER_HONTAI *player,ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow)
{
	//アイテムを拾うとInterPolationFieldTypeが変わる。このスイッチ文SetFieldType関数最後に-1を入れる
	switch (this[0].InterPolationFieldType)
	{
		this[0].ClearField();
	case FIELD_TYPE_BOKOBOKO:
		this[0].SetFieldType01();
		break;
	case FIELD_TYPE_NADARAKA:
		this[0].SetFieldType02();
		break;
	case FIELD_TYPE_PLAYERADVANTAGE:
		this[0].SetFieldType03(&player[0]);
		break;
	default:
		break;
	}
	//SetFieldTypeでInterPolationFieldSignalをfalseにしている。InterPolationField()の返り値で地形の補間が終わったか判定
	if (this[0].InterPolationFieldSignal == false)
	{
		this[0].InterPolationFieldSignal = this[0].InterPolationField();
	}

	//プレイヤーと地面の当たり判定
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//-------------------オブジェクト値読み込み
		D3DXVECTOR3 rayS = player[CntPlayer].GetPos();
		rayS.y += 1000.0f;
		D3DXVECTOR3 rayG = player[CntPlayer].GetPos();
		rayG.y -= 1000.0f;
		D3DXVECTOR3 FieldNor = player[CntPlayer].GetFieldNorVec();
		D3DXVECTOR3 ReturnPos = player[CntPlayer].GetPos();

		//判定
		this[0].FieldHitGetSphereVec(rayS, rayG, &FieldNor, &ReturnPos.y);

		//-------------------オブジェクト値書き込み
		player[CntPlayer].SetPos(ReturnPos);
		player[CntPlayer].SetFieldNorVec(FieldNor);
	}

	//アイテムと地面の当たり判定
	//-----------------------------------オブジェクト先頭アドレスを読み込み
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		bool use = item[CntItem].GetUse();
		if (use == true)
		{
			if (item[CntItem].CollisionFieldEnd != true || InterPolationFieldSignal == false)
			{
				//-------------------オブジェクト値読み込み
				D3DXVECTOR3 rayS = item[CntItem].GetPos();
				rayS.y += 5.0f;
				D3DXVECTOR3 rayG = item[CntItem].GetPos();
				rayG.y -= 5.0f;
				D3DXVECTOR3 FieldNor = item[CntItem].GetFieldNorVec();
				D3DXVECTOR3 ReturnPos = item[CntItem].GetPos();

				//判定
				item[CntItem].CollisionFieldEnd = this[0].FieldHitItem(rayS, rayG, &FieldNor, &ReturnPos.y);

				//-------------------オブジェクト値書き込み
				item[CntItem].SetPos(ReturnPos);
				item[CntItem].SetFieldNorVec(FieldNor);

			}
			else if (InterPolationFieldSignal == true && item[CntItem].CollisionFieldEnd == true)
			{
				//-------------------オブジェクト値読み込み
				D3DXVECTOR3 rayS = item[CntItem].GetPos();
				rayS.y -= 1000.0f;
				D3DXVECTOR3 rayG = item[CntItem].GetPos();
				rayG.y += 1000.0f;
				D3DXVECTOR3 FieldNor = item[CntItem].GetFieldNorVec();
				D3DXVECTOR3 ReturnPos = item[CntItem].GetPos();

				//判定
				this[0].FieldHit(rayS, rayG, &FieldNor, &ReturnPos.y);

				//-------------------オブジェクト値書き込み
				item[CntItem].SetPos(ReturnPos);
				item[CntItem].SetFieldNorVec(FieldNor);
			}
		}
	}

	//バレットと地面の当たり判定
	for (int Cntbullet = 0; Cntbullet < OBJECT_BULLET_MAX; Cntbullet++)
	{
		bool buse = bullet[Cntbullet].GetUse();
		if (buse == true)
		{
			D3DXVECTOR3 kari = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//-------------------オブジェクト値読み込み
			D3DXVECTOR3 rayS = bullet[Cntbullet].GetPos();
			rayS.y += 1000.0f;
			D3DXVECTOR3 rayG = bullet[Cntbullet].GetPos();
			rayG.y -= 1000.0f;

			//判定
			this[0].FieldHit(rayS, rayG, &kari, &bullet[Cntbullet].FieldPosY);

			//地形との当たり判定を緩く(調整)するためのbullet[Cntbullet].FieldPosY-3.0f
			D3DXVECTOR3 bpos = bullet[Cntbullet].GetPos();
			if (bullet[Cntbullet].FieldPosY - 3.0f > bpos.y)
			{
				// 爆発の生成
				D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bullet[Cntbullet].FieldPosY, bpos.z);
				explosion->SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[bullet[Cntbullet].UsePlayerType]);
				// バレット破棄
				bullet[0].ReleaseBullet(Cntbullet, shadow);
				// SE再生
				PlaySound(SOUND_LABEL_SE_damage);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void FIELD::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate,mtxWorld;
	mtxWorld = this[0].GetMatrix();
	D3DXVECTOR3 pos = this[0].GetPos();
	D3DXVECTOR3 rot = this[0].GetRot();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, pD3DVtxBuffFieldDraw, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(this[0].model.pD3DIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, this[0].model.pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this[0].model.nNumVertex, 0, this[0].model.nNumPolygon);
}

//=============================================================================
// 地形変形タイプ
//=============================================================================
void FIELD::SetFieldInterPolationFieldType(int type,int CntPlayer,ITEM *item)
{
	//フラグセット
	this[0].InterPolationFieldType = type;
	this[0].InterPolationFieldPlayerNum = CntPlayer;
	//-----------------------------------オブジェクト先頭アドレスを読み込み
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		item[CntItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// 地形のクリア
//=============================================================================
void FIELD::ClearField(void)
{
	VERTEX_3D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
		{
			// 頂点座標の設定
			pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
		}
	}
	// 頂点データをアンロックする
	this[0].pD3DVtxBuffFieldEnd->Unlock();
}

//=============================================================================
// 地形の自動生成01 ぼこぼこぎみ地形　ブロック数32*32　ブロックサイズ60*60～100*100
//=============================================================================
void FIELD::SetFieldType01(void)
{
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//上限　高さを設定
		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertex/4); nCntVtx++)
		{
				//高さを決める頂点を決定
				int YTXrandNum(rand() % this[0].model.nNumVertex);

				//高さを決め代入 縮退ポリゴンも同時に動かす
				int VTXrandY(rand() % 200);
				if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
				{
					pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float((VTXrandY / 1.0f));
					continue;
				}
				else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
				{
					pVtx[YTXrandNum+1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float((VTXrandY / 1.0f));
					continue;
				}
				pVtx[YTXrandNum].vtx.y = float((VTXrandY / 1.0f));
		}

		//縮退ポリゴンの座標を調整
		//SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//頂点最端の高さは固定。壁際の頂点のこと。
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
				//中側
				//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
				}
			}
		}
		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldSignal = false;
}

//=============================================================================
// 地形の自動生成02　見通しのいい地形　ブロック数32*32　ブロックサイズ60*60～100*100
//=============================================================================
void FIELD::SetFieldType02(void)
{
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//上限　高さを設定
		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertex / 4); nCntVtx++)
		{
			//高さを決める頂点を決定
			int YTXrandNum(rand() % this[0].model.nNumVertex);

			//高さを決め代入 縮退ポリゴンも同時に動かす
			int VTXrandY(rand() % 200);
			if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
			{
				pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float((VTXrandY / 1.0f));
				continue;
			}
			else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
			{
				pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float((VTXrandY / 1.0f));
				continue;
			}
			pVtx[YTXrandNum].vtx.y = float((VTXrandY / 1.0f));
		}

		//縮退ポリゴンの座標を調整
		//SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//頂点最端の高さは固定。壁際の頂点のこと。
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 100.0f;
				//中側　上下左右の平均値を算出
				//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = float(fabs(y));
				}
			}
		}
		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldSignal = false;
}

//=============================================================================
// 地形の自動生成03　取得プレイヤーが有利になる地形(相手プレイヤー付近を盆地)　　ブロック数32*32　ブロックサイズ250*250
//=============================================================================
void FIELD::SetFieldType03(PLAYER_HONTAI *player)
{
	//高さを決める
	{
		VERTEX_3D *pVtx;
		WORD *pIdx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		//上限　高さを設定
		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertex); nCntVtx++)
		{
			//高さを決める頂点を決定
			int YTXrandNum(rand() % this[0].model.nNumVertex);

			//高さを決め代入
			int VTXrandY(rand() % 200);
			VTXrandY += 20;//オフセット
			if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
			{
				pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float(VTXrandY);
				continue;
			}
			else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
			{
				pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float(VTXrandY);
				continue;
			}
			pVtx[YTXrandNum].vtx.y = float(VTXrandY);
		}

		//縮退ポリゴンの座標を調整
		//SetDegenerationPoly();

		//隣接頂点の高さ平均値を(ダイアモンドスクエア、フラクタルを参考)

		//for (int nCntVtx = 0; nCntVtx < this[0].model.nNumVertexIndex/2; nCntVtx++)
		//{
		//	//高さを決める頂点を決定
		//	int YTXrandNum(rand() % this[0].model.nNumVertex);
		//	//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		//	if (YTXrandNum == this[0].model.nNumVertexIndex - 2) break;
		//	else if (pIdx[YTXrandNum] == pIdx[YTXrandNum + 1]) continue;
		//	else if (pIdx[YTXrandNum + 1] == pIdx[YTXrandNum + 2]) continue;
		//	// 頂点座標の設定
		//	//頂点最端の高さは固定。壁際の頂点のこと。
		//	//上側
		//	//if (nCntVtx == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
		//	//	pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 200.0f;
		//	//中側　上下左右の平均値を算出
		//	//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
		//	else
		//	{
		//		float y = (pVtx[YTXrandNum - 1].vtx.y + pVtx[YTXrandNum + 1].vtx.y +
		//			pVtx[(this[0].nNumBlockZField + 1) + YTXrandNum].vtx.y + pVtx[(this[0].nNumBlockZField - 1) + YTXrandNum].vtx.y) / 4.0f;
		//		pVtx[YTXrandNum].vtx.y = fabsf(y);
		//	}
		//}


		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				// 頂点座標の設定
				//頂点最端の高さは固定。壁際の頂点のこと。
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 200.0f;
				//中側　上下左右の平均値を算出
				//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4.0f;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = fabsf(y);
				}
			}
		}
		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldEnd->Unlock();
		this[0].model.pD3DIdxBuff->Unlock();

	}


	//相手プレイヤー付近の地形を盆地化
	{
		VERTEX_3D *pVtx;
		WORD *pIdx;
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);
		//頂点インデックスバッファは共通で1つしかないのでDrawのやつを使っている
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		//-----------------------------------オブジェクト先頭アドレスを読み込み
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (CntPlayer != InterPolationFieldPlayerNum)
			{
				float HitPosUp;
				float HitPosDown;
				float HitPosLeft;
				float HitPosRight;
				HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

				//プレイヤーの乗っているエリアを特定。4分木で範囲を絞る。
				//-------------------オブジェクト値読み込み
				D3DXVECTOR3 pos = player[CntPlayer].GetPos();
				SpeedUpFieldHitPoly(pos, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
					fSideSizeXEighth, fSideSizeZEighth, fSideSizeXEighth/2, fSideSizeZEighth/2);

				for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
				{
					//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
					if (nCntVtx == this[0].model.nNumVertexIndex - 2)
					{
						break;
					}
					else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])
					{
						continue;
					}
					else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])
					{
						continue;
					}
					//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
					if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
					{
						if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
						{
							// 頂点座標の設定
							pVtx[pIdx[nCntVtx]].vtx.y = 20.0f;
						}
					}
				}
			}
		}

		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldEnd->Unlock();
		// インデックスデータをアンロックする
		this[0].model.pD3DIdxBuff->Unlock();
	}
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldSignal = false;
}

//=============================================================================
// 地形の変形処理　生成されたフィールドに変形する
//=============================================================================
bool FIELD::InterPolationField(void)
{
	{
		VERTEX_3D *pVtxStart;
		VERTEX_3D *pVtxEnd;
		VERTEX_3D *pVtxDraw;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxStart, 0);
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtxEnd, 0);
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

		float dt = 1.0f / 60.0f;	// 1フレームで進める時間
		time += dt;

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx) continue;
				else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				//補間開始
				pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx = MyInterpolation(
					pVtxStart[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx, 
					pVtxEnd[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx,
					0.0, 1.0f, dt, time, LerpEaseInEaseOut);

				// 反射光の設定
				if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y >= FIELD_Y_HIGH)
				{
					pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
				}
				else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y < FIELD_Y_HIGH && pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y > FIELD_Y_LOW)
				{
					pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
				}
				else
				{
					pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
				}
			}
		}
		if (time >= 1.0f)
		{
			//変形が終了したら現在描画している地形をpD3DVtxBuffFieldStartへ保存。次の変形開始時の地形にする
			for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
			{
				for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
				{
					pVtxStart[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx;
				}
			}
			time = 0.0f;
			InterPolationFieldSignalEnd = true;
		}
		// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldStart->Unlock();
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
	if (this[0].InterPolationFieldSignalEnd == true)
	{
		this[0].InterPolationFieldSignalEnd = false;
		StopSound(SOUND_LABEL_SE_quake);
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// 縮退ポリゴンの座標を再計算する関数　地形変形後、縮退ポリゴンの座標を設定する
//=============================================================================
void FIELD::SetDegenerationPoly(void)
{
	VERTEX_3D *pVtxDraw;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
		{
			//縮退ポリゴンの
			if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)
			{
				pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx;
			}
			else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx)
			{
				pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx = pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx;
			}
		}
	}

	this[0].pD3DVtxBuffFieldDraw->Unlock();
}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値void バレットの当たり判定で使用
//=============================================================================
void FIELD::FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//当たり判定高速化用の座標四隅
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//高速化処理。4分木で当たり判定をする範囲を絞る。
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight, 
		fSideSizeXHalf, fSideSizeZHalf, fSideSizeXEighth, fSideSizeZEighth);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
	{
		//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		if (nCntVtx == this[0].model.nNumVertexIndex - 2)
		{
			break;
		}
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])
		{
			continue;
		}
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])
		{
			continue;
		}
		//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
		else if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
		{
			if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
			{
				// 頂点座標の設定
				D3DXVECTOR3 vtx[3];
				D3DXVECTOR3 vtxHen[2];
				D3DXVECTOR3 NaibunPos;

				//-----------------------オブジェクト読み込み
				D3DXVECTOR3 pos = this[0].GetPos();
				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

				//レイキャストで調べる。trueで座標yを変更
				if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
				{
					vtxHen[0] = vtx[1] - vtx[0];
					vtxHen[1] = vtx[2] - vtx[0];

					//プレイやの傾く軸(地形の法線)
					D3DXVECTOR3 vtxHOUSEN;
					D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
					if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
					*vtxNor = vtxHOUSEN;
					*posY = NaibunPos.y + 1.0f;
					break;
				}
			}
		}
	}

	// 頂点データをアンロックする
	this[0].pD3DVtxBuffFieldDraw->Unlock();
	// インデックスデータをアンロックする
	this[0].model.pD3DIdxBuff->Unlock();

}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値void　プレイヤーの当たり判定で使用。球面補間で使用するため。
//=============================================================================
void FIELD::FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//当たり判定高速化用の座標四隅
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//高速化処理。4分木で当たり判定をする範囲を絞る。
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
		fSideSizeXQuarter, fSideSizeZQuarter, fSideSizeXEighth, fSideSizeZEighth);


	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
		{
			//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
			if (nCntVtx == this[0].model.nNumVertexIndex - 2)
			{
				break;
			}
			else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])
			{
				continue;
			}
			else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])
			{
				continue;
			}
			//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
			else if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
			{
				if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
				{
					// 頂点座標の設定
					D3DXVECTOR3 vtx[3];
					D3DXVECTOR3 vtxHen[2];
					D3DXVECTOR3 NaibunPos;

					//-----------------------オブジェクト読み込み
					D3DXVECTOR3 pos = this[0].GetPos();
					vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
					vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
					vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

					//レイキャストで調べる。trueで座標yを変更
					if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
					{
						//プレイヤーの傾く軸(プレイヤーが乗っている地形の法線)
						vtxHen[0] = vtx[1] - vtx[0];
						vtxHen[1] = vtx[2] - vtx[0];
						D3DXVECTOR3 vtxHOUSEN;
						D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
						if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
						*vtxNor = vtxHOUSEN;
						*posY = NaibunPos.y + 1.0f;

						//pVtx[pIdx[nCntVtx]]=今の頂点
						//乗ってるポリゴンがわかったらそこから上下左右のポリゴン法線を計算する
						//ポリゴンは三角形なので隣接辺は3つ(隣接ポリゴンは3つ)　
						//ポリゴンの最端値(ブロックx,zの切り返し番号)だったら、最端値側のポリゴン法線はプレイヤーが乗ってるポリゴン法線と同じ
						//上　-1
						//下　+1
						//左右　同じ座標にある頂点

						//今の頂点z座標と比べ、一個次の頂点z座標が大きければ左下直角三角形に乗っている
						if (pVtx[pIdx[nCntVtx]].vtx.z < pVtx[pIdx[nCntVtx + 1]].vtx.z)
						{
							//特別な頂点番号
							if (nCntVtx == this[0].model.nNumVertexIndex - this[0].nNumBlockZField * 2)
							{
								//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
								float MinDistance = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								if (SPHERELINEAR_LEN > MinDistance)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistance) / 10);
									*vtxNor = NewPolyNorVec;
								}
							}
							//　左　左下直角三角形で今の頂点番号がthis[0].nNumBlockZField以下ならば、地面の最左端にいる
							//else if (nCntVtx >= this[0].nNumBlockZField)
							//{
							//	int a = 0;

							//}
							//　上　左下直角三角形で今の頂点座標と一個前の頂点座標が同じもしくは、一個前の頂点座標と二個前の頂点座標が同じならば、地面の最上端にいる
							else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx - 1]].vtx || pVtx[pIdx[nCntVtx - 1]].vtx == pVtx[pIdx[nCntVtx - 2]].vtx)
							{
								int a = 0;

							}
							//　下　左下直角三角形で今の頂点座標と一個次の頂点座標が同じもしくは、一個次の頂点座標と二個次の頂点座標が同じならば、地面の最下端にいる(縮退ポリゴンなのでtrueにならないはず)
							else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx + 1]].vtx || pVtx[pIdx[nCntVtx + 1]].vtx == pVtx[pIdx[nCntVtx + 2]].vtx)
							{
								//縮退ポリゴン
								int a = 0;
							}
							else
							{
								//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
								//右上
								float MinDistanceRightUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								float MinDistanceDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
								float MinDistanceLeft = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								//プレイヤー座標から2辺までの距離がSPHERELINEAR_LENより短いと、その先に続く二つの地形の法線平均値をとり、その値で補間させる
								if (SPHERELINEAR_LEN > MinDistanceRightUp && SPHERELINEAR_LEN > MinDistanceDown)
								{
									// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRightUp
									D3DXVECTOR3 NextPolyNorVec1;
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceDown
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

									//二つの法線平均値を求める
									D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

									//競合している二つの距離の平均を求める
									float MinRtionDistance = (MinDistanceRightUp+ MinDistanceDown)/2.0f;

									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceRightUp && SPHERELINEAR_LEN > MinDistanceLeft)
								{
									// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRightUp
									D3DXVECTOR3 NextPolyNorVec1;
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeft
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

									//二つの法線平均値を求める
									D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

									//競合している二つの距離の平均を求める
									float MinRtionDistance = (MinDistanceRightUp + MinDistanceLeft) / 2.0f;

									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
									*vtxNor = NewPolyNorVec;

								}
								else if (SPHERELINEAR_LEN > MinDistanceDown && SPHERELINEAR_LEN > MinDistanceLeft)
								{
									// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceDown
									D3DXVECTOR3 NextPolyNorVec1;
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeft
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

									//二つの法線平均値を求める
									D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

									//競合している二つの距離の平均を求める
									float MinRtionDistance = (MinDistanceDown + MinDistanceLeft) / 2.0f;

									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
									*vtxNor = NewPolyNorVec;

								}
								//1辺しか干渉していない
								else if (SPHERELINEAR_LEN > MinDistanceRightUp)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceRightUp) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceDown)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceDown) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceLeft)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceLeft) / 10);
									*vtxNor = NewPolyNorVec;
								}
							}
						}
						//それ以外は右上直角三角形に乗っている
						else
						{
							//特別な頂点番号
							if (nCntVtx == this[0].nNumBlockZField * 2 - 3)
							{
								//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
								float MinDistance = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
								if (SPHERELINEAR_LEN > MinDistance)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx]].vtx - pVtx[pIdx[nCntVtx - 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx - 1]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistance) / 10);
									*vtxNor = NewPolyNorVec;
								}
							}
							//　上　右上直角三角形で今の頂点座標と一個前の頂点座標が同じもしくは、一個前の頂点座標と二個前の頂点座標が同じならば、地面の最上端にいる
							else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx - 1]].vtx || pVtx[pIdx[nCntVtx - 1]].vtx == pVtx[pIdx[nCntVtx - 2]].vtx)
							{
								//縮退ポリゴン
							}
							//　下　右上直角三角形今の頂点座標と一個次の頂点座標が同じもしくは、一個次の頂点座標と二個次の頂点座標が同じならば、地面の最下端にいる(縮退ポリゴンなのでtrueにならないはず)
							else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx + 1]].vtx || pVtx[pIdx[nCntVtx + 1]].vtx == pVtx[pIdx[nCntVtx + 2]].vtx)
							{

							}
							//　右　右上直角三角形で今の頂点番号がthis[0].nNumBlockZField以上ならば、地面の最右端にいる
							//else if (nCntVtx >= this[0].nNumBlockZField * this[0].nNumBlockZField)
							//{

							//}
							else
							{
								//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
								float MinDistanceRight = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								float MinDistanceUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								float MinDistanceLeftDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
								//プレイヤー座標から2辺までの距離がSPHERELINEAR_LENより短いと、その先に続く二つの地形の法線平均値をとり、その値で補間させる
								if (SPHERELINEAR_LEN > MinDistanceRight && SPHERELINEAR_LEN > MinDistanceUp)
								{
									// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRight
									D3DXVECTOR3 NextPolyNorVec1;
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceUp
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

									//二つの法線平均値を求める
									D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

									//競合している二つの距離の平均を求める
									float MinRtionDistance = (MinDistanceRight + MinDistanceUp) / 2.0f;

									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceRight && SPHERELINEAR_LEN > MinDistanceLeftDown)
								{
									// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRight
									D3DXVECTOR3 NextPolyNorVec1;
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeftDown
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

									//二つの法線平均値を求める
									D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

									//競合している二つの距離の平均を求める
									float MinRtionDistance = (MinDistanceRight + MinDistanceLeftDown) / 2.0f;

									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
									*vtxNor = NewPolyNorVec;

								}
								else if (SPHERELINEAR_LEN > MinDistanceUp && SPHERELINEAR_LEN > MinDistanceLeftDown)
								{
									// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceUp
									D3DXVECTOR3 NextPolyNorVec1;
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeftDown
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

									//二つの法線平均値を求める
									D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

									//競合している二つの距離の平均を求める
									float MinRtionDistance = (MinDistanceUp + MinDistanceLeftDown) / 2.0f;

									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
									*vtxNor = NewPolyNorVec;

								}

								//1辺しか干渉していない
								else if (SPHERELINEAR_LEN > MinDistanceRight)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceRight) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceUp)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceUp) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceLeftDown)
								{
									//辺先にある地面ポリゴンの法線を求める
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceLeftDown) / 10);
									*vtxNor = NewPolyNorVec;
								}
							}
						}

						//プレイヤーの座標がポリゴンのつなぎ目付近にくると、そのポリゴン法線同士で球面線形補間
						//補間で求めた中間ベクトルがプレイヤーの姿勢ベクトルになる

						break;
					}
				}
			}
		}

	// 頂点データをアンロックする
		this[0].pD3DVtxBuffFieldDraw->Unlock();
	// インデックスデータをアンロックする
	this[0].model.pD3DIdxBuff->Unlock();

}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値bool アイテムの当たり判定で使用
//=============================================================================
bool FIELD::FieldHitItem(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	bool Cheack = false;
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//当たり判定高速化用の座標四隅
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//高速化処理。4分木で当たり判定をする範囲を絞る。
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
		fSideSizeXHalf, fSideSizeZHalf, fSideSizeXEighth, fSideSizeZEighth);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
	{
		//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		if (nCntVtx == this[0].model.nNumVertexIndex - 2)
		{
			break;
		}
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])
		{
			continue;
		}
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])
		{
			continue;
		}
		else if (InrayS.y <= 400.0f)
		{
			//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
			if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
			{
				if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
				{
					// 頂点座標の設定
					D3DXVECTOR3 vtx[3];
					D3DXVECTOR3 vtxHen[2];
					D3DXVECTOR3 NaibunPos;

					//-----------------------オブジェクト読み込み
					D3DXVECTOR3 pos = this[0].GetPos();
					vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
					vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
					vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

					//レイキャストで調べる。trueで座標yを変更
					if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
					{
						vtxHen[0] = vtx[1] - vtx[0];
						vtxHen[1] = vtx[2] - vtx[0];

						//プレイやの傾く軸(地形の法線)
						D3DXVECTOR3 vtxHOUSEN;
						D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
						if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
						*vtxNor = vtxHOUSEN;
						*posY = NaibunPos.y + 1.0f;
						Cheack = true;
					}
				}
			}
		}
	}

	// 頂点データをアンロックする
	this[0].pD3DVtxBuffFieldDraw->Unlock();
	// インデックスデータをアンロックする
	this[0].model.pD3DIdxBuff->Unlock();
	if (Cheack == true) return true;

	return false;
}



