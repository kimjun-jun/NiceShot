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

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ClearField(void);				//初期化
void SetFieldType01(void);			//オリジナル1 
void SetFieldType02(void);			//オリジナル2 
void SetFieldType03(void);			//オリジナル3 
bool InterPolationField(void);		//地形を徐々に変化させる
void SetDegenerationPoly(void);		//縮退ポリゴンの座標を再計算する関数　地形変形後、縮退ポリゴンの座標を設定する

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
bool InterPolationFieldSignal;					// 地形変形信号
bool InterPolationFieldSignalEnd;				// 地形変形信号終了したかどうか
bool InterPolationFieldSignalFirstTime;			// 地形変形初めての読み込み時の信号。一度変形開始して移動量を求める。
static int InterPolationFieldType;				// 地形変形タイプ
static int InterPolationFieldPlayerNum;			// 地形変形アイテムを取得したプレイヤーナンバー


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	InterPolationFieldSignal = true;
	InterPolationFieldSignalEnd = false;
	InterPolationFieldSignalFirstTime = false;

	InterPolationFieldType = -1;
	InterPolationFieldPlayerNum = -1;
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

		//縮退ポリゴンの座標を調整
		SetDegenerationPoly();

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


	//当たり判定高速化用。一応親子関係も登録
	InitCntPartition();

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
	switch (InterPolationFieldType)
	{
		ClearField();
	case FIELD_TYPE_BOKOBOKO:
		SetFieldType01();
		break;
	case FIELD_TYPE_NADARAKA:
		SetFieldType02();
		break;
	case FIELD_TYPE_PLAYERADVANTAGE:
		SetFieldType03();
		break;
	default:
		break;
	}

	if (InterPolationFieldSignal == false)
	{
		InterPolationFieldSignal = InterPolationField();
	}

	//プレイヤーと地面の当たり判定
	PLAYER_HONTAI *player = GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHitGetSphereVec(D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y + 1000.0f, player[CntPlayer].pos.z),
			D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y - 1000.0f, player[CntPlayer].pos.z),
			&player[CntPlayer].RotVecAxis, &player[CntPlayer].pos.y);
	}

	//アイテムと地面の当たり判定
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

	//バレットと地面の当たり判定
	BULLET *bullet = GetBullet();
	for (int Cntbullet = 0; Cntbullet < BULLET_MAX; Cntbullet++)
	{
		if (bullet[Cntbullet].use == true)
		{
			D3DXVECTOR3 kari;
			FieldHit(D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].pos.y + 1000.0f, bullet[Cntbullet].pos.z),
				D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].pos.y - 1000.0f, bullet[Cntbullet].pos.z),
				&kari, &bullet[Cntbullet].FieldPosY);
			if (bullet[Cntbullet].FieldPosY - 3.0f > bullet[Cntbullet].pos.y)//地形との当たり判定を緩く(調整)するためのbullet[Cntbullet].FieldPosY-3.0f
			{
				// 爆発の生成
				D3DXVECTOR3 ExploPos = D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].FieldPosY, bullet[Cntbullet].pos.z);
				SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[bullet[Cntbullet].UsePlayerType]);
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
void SetFieldInterPolationFieldType(int type,int CntPlayer)
{
	//フラグセット
	InterPolationFieldType = type;
	InterPolationFieldPlayerNum = CntPlayer;

	//地形が変わるとアイテムの当たり判定ももう一度やりなおす
	ITEM *item = GetItem();
	for (int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
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
// 地形の自動生成01 ぼこぼこぎみ地形　ブロック数32*32　ブロックサイズ60*60～100*100
//=============================================================================
void SetFieldType01(void)
{
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//上限　高さを設定
		for (int nCntVtx = 0; nCntVtx < g_nNumVertexField/4; nCntVtx++)
		{
				//高さを決める頂点を決定
				int YTXrandNum(rand() % g_nNumVertexField);

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

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//頂点最端の高さは固定。壁際の頂点のこと。
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 50.0f;
				//中側
				//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
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
// 地形の自動生成02　見通しのいい地形　ブロック数32*32　ブロックサイズ60*60～100*100
//=============================================================================
void SetFieldType02(void)
{
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//上限　高さを設定
		for (int nCntVtx = 0; nCntVtx < g_nNumVertexField / 4; nCntVtx++)
		{
			//高さを決める頂点を決定
			int YTXrandNum(rand() % g_nNumVertexField);

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

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//頂点最端の高さは固定。壁際の頂点のこと。
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 100.0f;
				//中側　上下左右の平均値を算出
				//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
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
// 地形の自動生成03　取得プレイヤーが有利になる地形(相手プレイヤー付近を盆地)　　ブロック数32*32　ブロックサイズ200*200
//=============================================================================
void SetFieldType03(void)
{
	{
		VERTEX_3D *pVtx;
		WORD *pIdx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);
		g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);

		//上限　高さを設定
		for (int nCntVtx = 0; nCntVtx < g_nNumVertexField / 4; nCntVtx++)
		{
			//高さを決める頂点を決定
			int YTXrandNum(rand() % g_nNumVertexField);

			//高さを決め代入
			int VTXrandY(rand() % 400);
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
		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//頂点最端の高さは固定。壁際の頂点のこと。
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 400.0f;
				//中側　上下左右の平均値を算出
				//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = fabsf(y);
				}
			}
		}

		//相手プレイヤー付近の地形を盆地化
		PLAYER_HONTAI *p = GetPlayerHoudai();
		for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
		{
			if (CntPlayer != InterPolationFieldPlayerNum)
			{
				float HitPosUp;
				float HitPosDown;
				float HitPosLeft;
				float HitPosRight;
				HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;
				//プレイヤーの乗っているエリアを特定。4分木で範囲を絞る。
				SpeedUpFieldHitPoly(p[CntPlayer].pos, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
					g_fSideSizeXEighth, g_fSideSizeZEighth, g_fSideSizeXEighth/2, g_fSideSizeZEighth/2);

				for (int nCntVtx = 0; nCntVtx < g_nNumVertexIndexField; nCntVtx++)
				{
					//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
					if (nCntVtx == g_nNumVertexIndexField - 2)
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
		g_pD3DVtxBuffFieldDraw->Unlock();
		// インデックスデータをアンロックする
		g_pD3DIdxBuffFieldDraw->Unlock();
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

		float dt = 1.0f / 60.0f;	// 1フレームで進める時間
		g_time += dt;		// アニメーションの合計時間に足す

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx) continue;
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				//補間開始
				pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = MyInterpolation(
					pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx, 
					pVtxEnd[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx,
					0.01f, 1.0f, dt, g_time, LerpEaseInEaseOut);

				// 反射光の設定
				if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y >= 150.0f)
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
				}
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y < 150.0f && pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
				}
				else
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
				}
			}
		}
		if (g_time >= 1.0f)
		{
			//変形が終了したら現在描画している地形をg_pD3DVtxBuffFieldStartへ保存。次の変形開始時の地形にする
			for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
			{
				for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
				{
					pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
				}
			}
			g_time = 0.0f;
			InterPolationFieldType = -1;
			InterPolationFieldSignalEnd = true;
			InterPolationFieldSignalFirstTime = false;
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffFieldStart->Unlock();
		g_pD3DVtxBuffFieldDraw->Unlock();
		g_pD3DVtxBuffFieldEnd->Unlock();
	}
	if (InterPolationFieldSignalEnd == true)
	{
		InterPolationFieldSignalEnd = false;
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
void SetDegenerationPoly(void)
{
	VERTEX_3D *pVtxDraw;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
		{
			//縮退ポリゴンの
			if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)
			{
				pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx;
			}
			else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx)
			{
				pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx = pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx;
			}
		}
	}
}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値void プレイヤー以外の当たり判定で使用
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

	//高速化処理。4分木で当たり判定をする範囲を絞る。
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight, 
		g_fSideSizeXHalf, g_fSideSizeZHalf, g_fSideSizeXEighth, g_fSideSizeZEighth);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < g_nNumVertexIndexField; nCntVtx++)
	{
		//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		if (nCntVtx == g_nNumVertexIndexField - 2)
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

				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + g_posField;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + g_posField;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + g_posField;

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
	g_pD3DVtxBuffFieldDraw->Unlock();
	// インデックスデータをアンロックする
	g_pD3DIdxBuffFieldDraw->Unlock();

}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値void　プレイヤーの当たり判定で使用。球面補間で使用するため。
//=============================================================================
void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
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
		g_fSideSizeXQuarter, g_fSideSizeZQuarter, g_fSideSizeXEighth, g_fSideSizeZEighth);


	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);


		for (int nCntVtx = 0; nCntVtx < g_nNumVertexIndexField; nCntVtx++)
		{
			//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
			if (nCntVtx == g_nNumVertexIndexField - 2)
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

					vtx[0] = pVtx[pIdx[nCntVtx]].vtx + g_posField;
					vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + g_posField;
					vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + g_posField;

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
							if (nCntVtx == g_nNumVertexIndexField - g_nNumBlockXField * 2)
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
							//　左　左下直角三角形で今の頂点番号がg_nNumBlockZField以下ならば、地面の最左端にいる
							//else if (nCntVtx >= g_nNumBlockZField)
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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeft
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
							if (nCntVtx == g_nNumBlockXField * 2 - 3)
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
							//　右　右上直角三角形で今の頂点番号がg_nNumBlockXField以上ならば、地面の最右端にいる
							//else if (nCntVtx >= g_nNumBlockXField * g_nNumBlockZField)
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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeftDown
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
	g_pD3DVtxBuffFieldDraw->Unlock();
	// インデックスデータをアンロックする
	g_pD3DIdxBuffFieldDraw->Unlock();

}




