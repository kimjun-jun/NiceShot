//=============================================================================
//
// 地面の処理 [field.cpp]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "field.h"
#include "item.h"
#include "bullet.h"
#include "sound.h"
#include "explosion.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME				"data/TEXTURE/white.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_NOISE					"data/TEXTURE/noise.bmp"		// 読み込むテクスチャファイル名
#define FIELD_PARTITION_FIRST_NUM		(4)
#define FIELD_PARTITION_SECOND_NUM		(16)
#define FIELD_PARTITION_THIRD_NUM		(64)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ClearField(void);				//初期化
void SetFieldType01(void);			//オリジナル1 
void SetFieldType02(void);			//オリジナル2 
bool InterPolationField(void);		//地形を徐々に変化させる

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//フィールド生成用
LPDIRECT3DTEXTURE9 g_pD3DTextureField;				// テクスチャ読み込み場所
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFieldDraw;		// 頂点バッファインターフェースへのポインタ描画用
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFieldEnd;		// 頂点バッファインターフェースへのポインタ補間データエンドバッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFieldStart;	// 頂点バッファインターフェースへのポインタ補間データスタートバッファ
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffFieldDraw;		// インデックスバッファインターフェースへのポインタ
D3DXMATRIX g_mtxWorldField;							// ワールドマトリックス
D3DXVECTOR3 g_posField;								// ポリゴン表示位置の中心座標
D3DXVECTOR3 g_rotField;								// ポリゴンの回転角
int g_nNumBlockXField, g_nNumBlockZField;			// ブロック数
int g_nNumVertexField;								// 総頂点数	
int g_nNumVertexIndexField;							// 総インデックス数
int g_nNumPolygonField;								// 総ポリゴン数
float g_fBlockSizeXField, g_fBlockSizeZField;		// ブロックサイズ
float g_fSideSizeXMax, g_fSideSizeZMax;				// 辺サイズ最大 1920
float g_fSideSizeXHalf, g_fSideSizeZHalf;			// 辺サイズ1/2   960
float g_fSideSizeXQuarter, g_fSideSizeZQuarter;		// 辺サイズ1/4   480
float g_fSideSizeXEighth, g_fSideSizeZEighth;		// 辺サイズ1/8   240


//フィールド変形用
static float g_time = 0.0f;						// 地形の変形時間
static float Gt = 60.0f;						// 地形の終了時間
bool InterPolationFieldSignal;					// 地形変形信号
static int InterPolationFieldType;				// 地形変形タイプ


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
};

//当たり判定高速化用
FIELD_COLLISION g_FirstField_Collision[FIELD_PARTITION_FIRST_NUM];
FIELD_COLLISION g_SecondField_Collision[FIELD_PARTITION_SECOND_NUM];
FIELD_COLLISION g_ThirdField_Collision[FIELD_PARTITION_THIRD_NUM];


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	InterPolationFieldSignal = true;
	InterPolationFieldType = -1;
	// ポリゴン表示位置の中心座標を設定
	g_posField = pos;

	g_rotField = rot;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,								// ファイルの名前
		&g_pD3DTextureField);							// 読み込むメモリー

	// ブロック数の設定
	g_nNumBlockXField = nNumBlockX;
	g_nNumBlockZField = nNumBlockZ;

	// 頂点数の設定
	g_nNumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	g_nNumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	g_nNumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;

	//　辺サイズの設定
	g_fSideSizeXMax = nNumBlockX * fBlockSizeX;
	g_fSideSizeZMax = nNumBlockZ * fBlockSizeZ;
	g_fSideSizeXHalf = g_fSideSizeXMax / 2.0f;
	g_fSideSizeZHalf = g_fSideSizeZMax / 2.0f;
	g_fSideSizeXQuarter = g_fSideSizeXHalf / 2.0f;
	g_fSideSizeZQuarter = g_fSideSizeZHalf / 2.0f;
	g_fSideSizeXEighth = g_fSideSizeXQuarter / 2.0f;
	g_fSideSizeZEighth = g_fSideSizeZQuarter / 2.0f;

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffFieldDraw,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffFieldEnd,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffFieldStart,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DIdxBuffFieldDraw,				// インデックスバッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	//描画用
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		g_pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffFieldDraw->Unlock();
		g_pD3DVtxBuffFieldStart->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (g_nNumBlockZField - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + g_nNumBlockXField;
				nCntIdx++;
			}
		}

		// インデックスデータをアンロックする
		g_pD3DIdxBuffFieldDraw->Unlock();
	}

	//描画用
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		g_pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		//上限
		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1)/2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1)/2; nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;
				pVtx[(rand() % g_nNumVertexField)].vtx.y = float((rand() % 200));
			}
		}
		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 50.0f;
				//中側
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = y;
				}

				// 反射光の設定
				if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
				}
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
				}
				else
				{
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
				}
			}
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffFieldDraw->Unlock(); 
		g_pD3DVtxBuffFieldStart->Unlock();
	}



	//自動生成のバッファ。地形変更するときにこの形に変形	F
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffFieldEnd->Unlock();
	}


	//当たり判定高速化用
	//親判定
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_FIRST_NUM; CntPartition++)
	{
		g_FirstField_Collision[CntPartition].Parent = NULL;
		g_FirstField_Collision[CntPartition].Num = CntPartition;
		g_FirstField_Collision[CntPartition].Cheak = false;
	}
	//子判定
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_SECOND_NUM; CntPartition++)
	{
		switch (CntPartition)
		{
		case 0:case 1:case 2:case 3:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[0];
			break;
		case 4:case 5:case 6:case 7:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[1];
			break;
		case 8:case 9:case 10:case 11:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[2];
			break;
		case 12:case 13:case 14:case 15:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[3];
			break;
		default:
			break;
		}
		g_SecondField_Collision[CntPartition].Num = CntPartition;
		g_SecondField_Collision[CntPartition].Cheak = false;
	}
	//孫判定
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_THIRD_NUM; CntPartition++)
	{
		switch (CntPartition)
		{
		case 0:case 1:case 2:case 3:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[0];
			break;
		case 4:case 5:case 6:case 7:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[1];
			break;
		case 8:case 9:case 10:case 11:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[2];
			break;
		case 12:case 13:case 14:case 15:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[3];
			break;

		case 16:case 17:case 18:case 19:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[4];
			break;
		case 20:case 21:case 22:case 23:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[5];
			break;
		case 24:case 25:case 26:case 27:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[6];
			break;
		case 28:case 29:case 30:case 31:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[7];
			break;

		case 32:case 33:case 34:case 35:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[8];
			break;
		case 36:case 37:case 38:case 39:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[9];
			break;
		case 40:case 41:case 42:case 43:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[10];
			break;
		case 44:case 45:case 46:case 47:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[11];
			break;

		case 48:case 49:case 50:case 51:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[12];
			break;
		case 52:case 53:case 54:case 55:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[13];
			break;
		case 56:case 57:case 58:case 59:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[14];
			break;
		case 60:case 61:case 62:case 63:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[15];
			break;
		}
		g_ThirdField_Collision[CntPartition].Num = CntPartition;
		g_ThirdField_Collision[CntPartition].Cheak = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	if (g_pD3DVtxBuffFieldDraw)
	{// 頂点バッファの開放
		g_pD3DVtxBuffFieldDraw->Release();
		g_pD3DVtxBuffFieldDraw = NULL;
	}

	if (g_pD3DIdxBuffFieldDraw)
	{// インデックスバッファの開放
		g_pD3DIdxBuffFieldDraw->Release();
		g_pD3DIdxBuffFieldDraw = NULL;
	}

	if (g_pD3DTextureField)
	{// テクスチャの開放
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{
	if (InterPolationFieldType == 0)
	{
		ClearField();
		SetFieldType01();
	}
	else if (InterPolationFieldType == 1)
	{
		ClearField();
		SetFieldType02();
	}
	if (InterPolationFieldSignal == false)
	{
		InterPolationFieldSignal = InterPolationField();
	}

	PLAYER_HONTAI *player = GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHit(D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y + 1000.0f, player[CntPlayer].pos.z),
			D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y - 1000.0f, player[CntPlayer].pos.z),
			&player[CntPlayer].RotVecAxis, &player[CntPlayer].pos.y);
	}

	ITEM *item = GetItem();
	for (int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		if (item[CntItem].bUse == true)
		{
			if (item[CntItem].fCollisionEnd == false)
			{
				FieldHit(D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y + 1000.0f, item[CntItem].pos.z),
					D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y - 1000.0f, item[CntItem].pos.z),
					&item[CntItem].rotVecAxis, &item[CntItem].pos.y);
				FieldHit(D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y - 1000.0f, item[CntItem].pos.z),
					D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y + 1000.0f, item[CntItem].pos.z),
					&item[CntItem].rotVecAxis, &item[CntItem].pos.y);
				if (InterPolationFieldSignal == true)
				{
					item[CntItem].fCollisionEnd = true;
				}
			}
		}
	}

	BULLET *bullet = GetBullet();
	for (int Cntbullet = 0; Cntbullet < BULLET_MAX; Cntbullet++)
	{
		if (bullet[Cntbullet].use == true)
		{
			D3DXVECTOR3 kari;
			FieldHit(D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].pos.y + 1000.0f, bullet[Cntbullet].pos.z),
				D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].pos.y - 1000.0f, bullet[Cntbullet].pos.z),
				&kari, &bullet[Cntbullet].FieldPosY);
			if (bullet[Cntbullet].FieldPosY - 3.0f > bullet[Cntbullet].pos.y)//地形との当たり判定を緩く(調整)するためのbullet[Cntbullet].FieldPosY-.0f
			{
				// 爆発の生成
				SetExplosion(bullet[Cntbullet].pos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[bullet[Cntbullet].UsePlayerType]);
				// バレット破棄
				ReleaseBullet(Cntbullet);
				// SE再生
				PlaySound(SOUND_LABEL_SE_damage);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, g_pD3DVtxBuffFieldDraw, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(g_pD3DIdxBuffFieldDraw);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureField);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexField, 0, g_nNumPolygonField);
}

//=============================================================================
// 頂点データのアドレスを渡す
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 *GetFieldBuff(void)
{
	return &g_pD3DVtxBuffFieldDraw;
}

//=============================================================================
// 頂点データのアドレスを渡す
//=============================================================================
int GetFieldBlockXNum(void)
{
	return g_nNumBlockXField;
}

//=============================================================================
// 頂点データのアドレスを渡す
//=============================================================================
int GetFieldBlockZNum(void)
{
	return g_nNumBlockZField;
}

//=============================================================================
// 頂点データのアドレスを渡す
//=============================================================================
float GetFieldBlockXSize(void)
{
	return g_fBlockSizeXField;
}

//=============================================================================
// 頂点データのアドレスを渡す
//=============================================================================
float GetFieldBlockZSize(void)
{
	return g_fBlockSizeZField;
}

//=============================================================================
// 地形変形タイプ
//=============================================================================
void SetFieldInterPolationFieldType(int type)
{
	InterPolationFieldType = type;
	ITEM *item = GetItem();
	for (int CntItem = 0; CntItem < DROP_ITEM_MAX; CntItem++)
	{
		item[CntItem].fCollisionEnd = false;
	}
}

//=============================================================================
// 地形のクリア
//=============================================================================
void ClearField(void)
{
	VERTEX_3D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
		{
			// 頂点座標の設定
			pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
		}
	}
	// 頂点データをアンロックする
	g_pD3DVtxBuffFieldEnd->Unlock();
}

//=============================================================================
// 地形の自動生成01 ぼこぼこぎみ地形
//=============================================================================
void SetFieldType01(void)
{
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//上限
		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1) / 2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;
				pVtx[(rand() % g_nNumVertexField)].vtx.y = float((rand() % 200));
			}
		}

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 50.0f;
				//中側
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = y;
				}
			}
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffFieldEnd->Unlock();
	}
	InterPolationFieldSignal = false;
}

//=============================================================================
// 地形の自動生成02　見通しのいい地形
//=============================================================================
void SetFieldType02(void)
{
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//上限
		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1) / 2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;
				pVtx[(rand() % g_nNumVertexField)].vtx.y = float((rand() % 100));
			}
		}

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 100.0f;
				//中側　上下左右の平均値を算出
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = float(fabs(y));
				}
			}
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffFieldEnd->Unlock();
	}
	InterPolationFieldSignal = false;
}

//=============================================================================
// 地形の変形処理　生成されたフィールドに変形する
//=============================================================================
bool InterPolationField(void)
{
	{
		VERTEX_3D *pVtxStart;
		VERTEX_3D *pVtxEnd;
		VERTEX_3D *pVtxDraw;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxStart, 0);
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtxEnd, 0);
		g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

		float dt = 0.05f/1.0f;	// 1フレームで進める時間
		g_time += dt;		// アニメーションの合計時間に足す

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				//線形　座標を求める	X = StartX + (EndX - StartX) * 今の時間
				//線形　i(t)= s(1-v(t))+e*v(t)
					D3DXVECTOR3 vtxvec1 = pVtxEnd[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx - pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx + vtxvec1 * g_time;

					//三次　v(t)= -2t^3 = t^2(3-2t)
				//t(タイム(dt)),b(開始点),c(開始点と目的点の差分),d(合計時間(t))
				//t/=d/2.0f
				//c/2.0f*t*t+b
				//-c/2.0f*(t*(t-2)-1)+b
				//D3DXVECTOR3 c = pVtxF[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx - pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
				//D3DXVECTOR3 oldpos = pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
				//pVtxG[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx =
				//	-c / 2.0f*(t*(t-2)-1) + pVtxG[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;

				//pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = oldpos;

				// 反射光の設定
				if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
				}
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
				}
				else
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
				}
			}
			if (g_time >= 1.0)
			{
				for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
				{
					for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
					{
						pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
					}
				}
				g_time = 0.0f;
				InterPolationFieldType = -1;
				return true;
			}
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffFieldStart->Unlock();
		g_pD3DVtxBuffFieldDraw->Unlock();
		g_pD3DVtxBuffFieldEnd->Unlock();
	}
	return false;
}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値void
//=============================================================================
void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
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

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);

	//親判定　X,Zがプラスかマイナスか　基準はg_posFieldの中心(x0,z0)
	if (InrayS.x >= 0.0f && InrayS.z >= 0.0f)			//親++第一象限
	{
		g_FirstField_Collision[1].Cheak = true;
		//子判定　親の時と同じく、第一象限のなかでさらにプラスかマイナスか判定
		if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//子++第一象限
		{
			g_SecondField_Collision[5].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[21].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[20].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[22].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[23].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//子-+第二象限
		{
			g_SecondField_Collision[4].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[17].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[16].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[18].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[19].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//子--第三象限
		{
			g_SecondField_Collision[6].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[25].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[24].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[26].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >=  g_fSideSizeXEighth && InrayS.z <  g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[27].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//子+-第四象限
		{
			g_SecondField_Collision[7].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[29].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[28].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[30].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[31].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
	}
	else if (InrayS.x < 0.0f && InrayS.z >= 0.0f)			//親-+第二象限
	{
		g_FirstField_Collision[0].Cheak = true;
		//子判定　親の時と同じく、第一象限のなかでさらにプラスかマイナスか判定
		if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//子++第一象限
		{
			g_SecondField_Collision[1].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[5].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[4].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[6].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[7].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//子-+第二象限
		{
			g_SecondField_Collision[0].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[1].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[0].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[2].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[3].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				HitPosDown = g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//子--第三象限
		{
			g_SecondField_Collision[2].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[9].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[8].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[10].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z <  g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[11].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//子+-第四象限
		{
			g_SecondField_Collision[3].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[13].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[12].Cheak = true;
				HitPosUp = g_fSideSizeZQuarter;
				HitPosDown = g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[14].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[15].Cheak = true;
				HitPosUp = g_fSideSizeZEighth;
				HitPosDown = 0.0f;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
		}
	}
	else if (InrayS.x < 0.0f && InrayS.z < 0.0f)			//親--第三象限
	{
		g_FirstField_Collision[2].Cheak = true;
		if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//子++第一象限
		{
			g_SecondField_Collision[9].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[37].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[36].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[38].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[39].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//子-+第二象限
		{
			g_SecondField_Collision[8].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[33].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[32].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[34].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[35].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//子--第三象限
		{
			g_SecondField_Collision[10].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[41].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[40].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[42].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[43].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				HitPosRight = -g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//子+-第四象限
		{
			g_SecondField_Collision[11].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[45].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[44].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[46].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXQuarter;
				HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[47].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = -g_fSideSizeXEighth;
				HitPosRight = 0.0f;
			}
		}
	}
	else if (InrayS.x >= 0.0f && InrayS.z < 0.0f)			//親+-第四象限
	{
		g_FirstField_Collision[3].Cheak = true;
		if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//子++第一象限
		{
			g_SecondField_Collision[13].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[53].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[52].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[54].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[55].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//子-+第二象限
		{
			g_SecondField_Collision[12].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[49].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[48].Cheak = true;
				HitPosUp = 0.0f;
				HitPosDown = -g_fSideSizeZEighth;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[50].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[51].Cheak = true;
				HitPosUp = -g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//子--第三象限
		{
			g_SecondField_Collision[14].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[57].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[56].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[58].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = 0.0f;
				HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[59].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//子+-第四象限
		{
			g_SecondField_Collision[15].Cheak = true;
			//孫判定　同上
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫++第一象限
			{
				g_ThirdField_Collision[61].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫-+第二象限
			{
				g_ThirdField_Collision[60].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫--第三象限
			{
				g_ThirdField_Collision[62].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXQuarter;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//孫+-第四象限
			{
				g_ThirdField_Collision[63].Cheak = true;
				HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
	}

	//親判定
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_FIRST_NUM; CntPartition++)
	{
		g_FirstField_Collision[CntPartition].Cheak = false;
	}
	//子判定
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_SECOND_NUM; CntPartition++)
	{
		g_SecondField_Collision[CntPartition].Cheak = false;
	}
	//孫判定
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_THIRD_NUM; CntPartition++)
	{
		g_ThirdField_Collision[CntPartition].Cheak = false;
	}




	for (int nCntVtx = 0; nCntVtx < g_nNumVertexIndexField; nCntVtx++)
	{
		//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		if (nCntVtx == g_nNumVertexIndexField - 2) break;
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])	continue;
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2]) continue;
		//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
		else if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
		{
			if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
			{
				// 頂点座標の設定
				D3DXVECTOR3 vtx[3];
				D3DXVECTOR3 vtxHen[2];
				D3DXVECTOR3 NaibunPos;

				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + g_posField;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + g_posField;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + g_posField;

				//レイキャストで調べる。trueで座標yを変更
				if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
				{
					vtxHen[0] = vtx[1] - vtx[0];
					vtxHen[1] = vtx[2] - vtx[0];

					//プレイやの傾く軸
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
	g_pD3DVtxBuffFieldDraw->Unlock();
	// インデックスデータをアンロックする
	g_pD3DIdxBuffFieldDraw->Unlock();

}