//=============================================================================
//
// モデル読み込み処理 [load.cpp]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#include "main.h"
#include "load.h"

//=============================================================================
// メッシュロード関数
//=============================================================================
HRESULT LoadMesh(char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff, 
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXMESH pMesh;
	DWORD Vertex, Polygon, VertexIndex,NumMat;
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(FileName,			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
		pDevice,									// IDirect3DDevice9インターフェイスへのポインタ
		NULL,										// 隣接性データを含むバッファへのポインタ
		pD3DXBuffMat,								// マテリアルデータを含むバッファへのポインタ
		NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
		nNumMat,									// D3DXMATERIAL構造体の数
		pD3DXMesh)))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}
	pMesh = *pD3DXMesh;
	Vertex = pMesh->GetNumVertices();
	*nNumVertex = Vertex;
	Polygon = pMesh->GetNumFaces();
	*nNumPolygon = Polygon;
	VertexIndex = Polygon * 3;
	*nNumVertexIndex = VertexIndex;
	NumMat = *nNumMat;
	//pD3DTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9) * NumMat);

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		FVF_VERTEX_3D,														// 使用する頂点フォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		pD3DVtxBuff,														// 頂点バッファインターフェースへのポインタ
		NULL)))																// NULLに設定
	{
		return E_FAIL;
	}
	pMesh->GetVertexBuffer(pD3DVtxBuff);


	// オブジェクトのインデックスバッファを生成
	if (FAILED(pDevice->CreateIndexBuffer(VertexIndex,						// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		D3DFMT_INDEX16,														// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		pD3DIdxBuff,														// インデックスバッファインターフェースへのポインタ
		NULL)))																// NULLに設定
	{
		return E_FAIL;
	}
	pMesh->GetIndexBuffer(pD3DIdxBuff);

	if (pMesh != NULL)
	{// メッシュの開放
		pMesh->Release();
		pMesh = NULL;
	}

	return S_OK;

}
