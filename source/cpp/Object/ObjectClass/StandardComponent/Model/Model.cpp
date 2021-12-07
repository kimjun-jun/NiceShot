/**
* @file Model.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../../../../h/main.h"
#include "../../../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"

//テクスチャ読み込み処理
void TEXTURE::LoadTexture(const char *FileName)
{
	D3DXCreateTextureFromFile(GetDevice(), FileName, &this->pD3DTexture);
}

//頂点[作成]時設定(3D)
//作成のみ (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定
}

//座標のみ入力　他自動 (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//＋テクスチャ入力　他自動 (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//全項目入力 (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点[作成]時設定(3Dビルボード)
//作成のみ (3Dビルボード)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定
}

//座標のみ入力　他自動 (3Dビルボード)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//＋テクスチャ入力　他自動 (3Dビルボード)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//全項目入力 (3Dビルボード)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点[作成]時設定(2D)
//作成のみ (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

}

//頂点のみ入力　他自動 (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//＋テクスチャ入力　他自動 (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}

		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//全項目入力 (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
	const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点[作成]時設定(3D)
void VTXBUFFER::MakeIdxVertex(const int VTXIdxNum)
{
	// オブジェクトのインデックスバッファを生成
	GetDevice()->CreateIndexBuffer(VTXIdxNum,						// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		D3DFMT_INDEX16,														// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DIdxBuff,														// インデックスバッファインターフェースへのポインタ
		NULL);
}

//頂点サイズの設定　左右上下対称 (3D) effect で使用
void VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　左右上下非対称　比率固定 (3D)
void VTXBUFFER::Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標直接指定　(pos無しver) (3D)
void VTXBUFFER::Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標間接指定　(pos有りver) (3D)
void VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　左右上下対称 (2D)
void VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　左右上下非対称　比率固定 (2D)
void VTXBUFFER::Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標直接指定　(pos無しver) (2D)
void VTXBUFFER::Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標間接指定　(pos有りver) (2D) damege で使用
void VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		//pVtx += 4;
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点法線の設定
//法線自動
void VTXBUFFER::Nor3D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 法線の設定
		pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//法線入力
void VTXBUFFER::Nor3D(const int Indx, const D3DXVECTOR3 Nor[4])
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 法線の設定
		pVtx[0].nor = Nor[0];
		pVtx[1].nor = Nor[1];
		pVtx[2].nor = Nor[2];
		pVtx[3].nor = Nor[3];
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

// テクスチャのパースペクティブコレクト用
void VTXBUFFER::RHW2D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//=============================================================================
// メッシュカラーをセット
//=============================================================================
void VTXBUFFER::MeshColor(DWORD nNumPolygon, int CntPlayer)
{
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	this->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// 反射光の設定
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[CntPlayer];
	}
	// 頂点データをアンロックする
	this->pD3DVtxBuff->Unlock();
	this->pD3DIdxBuff->Unlock();
}

//頂点カラーの設定
//カラー自動 (3D)
void VTXBUFFER::Color3D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//カラー入力 (3D)
void VTXBUFFER::Color3D(const int Indx, const D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//カラー自動 (2D)
void VTXBUFFER::Color2D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 反射光の設定
		pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//カラー入力 (2D)
void VTXBUFFER::Color2D(const int Indx, const D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV設定(3D)
//UV自動 (3D)
void VTXBUFFER::UV3D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　調整用 (3D)
void VTXBUFFER::UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　直接入力 (3D)
void VTXBUFFER::UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV自動 (2D)
void VTXBUFFER::UV2D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　スプライトテクスチャ用 (2D)
void VTXBUFFER::UV2D(const int Indx, const float PatternU, const float PatternV)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定　+1は次のスプライトでの調整用
		pVtx[0].tex = D3DXVECTOR2(PatternU, PatternV);
		pVtx[1].tex = D3DXVECTOR2((PatternU + 1), PatternV);
		pVtx[2].tex = D3DXVECTOR2(PatternU, (PatternV + 1));
		pVtx[3].tex = D3DXVECTOR2((PatternU + 1), (PatternV + 1));
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　調整用 (2D)
void VTXBUFFER::UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　直接入力 (2D)
void VTXBUFFER::UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//Color取得(3D)
D3DXCOLOR VTXBUFFER::GetColor3D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}

//Color取得(2D)
D3DXCOLOR VTXBUFFER::GetColor2D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}






//頂点[作成]時設定(3D)
//作成のみ (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定
}

//座標のみ入力　他自動 (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//＋テクスチャ入力　他自動 (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//全項目入力 (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点[作成]時設定(3Dビルボード)
//作成のみ (3Dビルボード)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定
}

//座標のみ入力　他自動 (3Dビルボード)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//＋テクスチャ入力　他自動 (3Dビルボード)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//全項目入力 (3Dビルボード)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D頂点処理　基本的にビルボード処理で使用
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// 法線の設定
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点[作成]時設定(2D)
//作成のみ (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

}

//頂点のみ入力　他自動 (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//＋テクスチャ入力　他自動 (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}

		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//全項目入力 (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
	const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// オブジェクトの頂点バッファを生成
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF,										// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
		NULL);										// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// 反射光の設定
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点[作成]時設定(3D)
void My3D_VTXBUFFER::MakeIdxVertex3D(const int VTXIdxNum)
{
	// オブジェクトのインデックスバッファを生成
	GetDevice()->CreateIndexBuffer(VTXIdxNum,						// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		D3DFMT_INDEX16,														// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DIdxBuff,														// インデックスバッファインターフェースへのポインタ
		NULL);
}

//頂点[作成]時設定(2D)
void My2D_VTXBUFFER::MakeIdxVertex2D(const int VTXIdxNum)
{
	// オブジェクトのインデックスバッファを生成
	GetDevice()->CreateIndexBuffer(VTXIdxNum,						// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		D3DFMT_INDEX16,														// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		&this->pD3DIdxBuff,														// インデックスバッファインターフェースへのポインタ
		NULL);
}

//頂点サイズの設定　左右上下対称 (3D) effect で使用
void My3D_VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　左右上下非対称　比率固定 (3D)
void My3D_VTXBUFFER::Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標直接指定　(pos無しver) (3D)
void My3D_VTXBUFFER::Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標間接指定　(pos有りver) (3D)
void My3D_VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　左右上下対称 (2D)
void My2D_VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　左右上下非対称　比率固定 (2D)
void My2D_VTXBUFFER::Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標直接指定　(pos無しver) (2D)
void My2D_VTXBUFFER::Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点サイズの設定　全座標間接指定　(pos有りver) (2D) damege で使用
void My2D_VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		//pVtx += 4;
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点法線の設定
//法線自動
void My3D_VTXBUFFER::Nor3D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 法線の設定
		pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//法線入力
void My3D_VTXBUFFER::Nor3D(const int Indx, const D3DXVECTOR3 Nor[4])
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 法線の設定
		pVtx[0].nor = Nor[0];
		pVtx[1].nor = Nor[1];
		pVtx[2].nor = Nor[2];
		pVtx[3].nor = Nor[3];
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

// テクスチャのパースペクティブコレクト用
void My2D_VTXBUFFER::RHW2D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//頂点カラーの設定
//カラー自動 (3D)
void My3D_VTXBUFFER::Color3D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//カラー入力 (3D)
void My3D_VTXBUFFER::Color3D(const int Indx, const D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//カラー自動 (2D)
void My2D_VTXBUFFER::Color2D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 反射光の設定
		pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//カラー入力 (2D)
void My2D_VTXBUFFER::Color2D(const int Indx, const D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV設定(3D)
//UV自動 (3D)
void My3D_VTXBUFFER::UV3D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　調整用 (3D)
void My3D_VTXBUFFER::UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　直接入力 (3D)
void My3D_VTXBUFFER::UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV自動 (2D)
void My2D_VTXBUFFER::UV2D(const int Indx)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　スプライトテクスチャ用 (2D)
void My2D_VTXBUFFER::UV2D(const int Indx, const float PatternU, const float PatternV)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定　+1は次のスプライトでの調整用
		pVtx[0].tex = D3DXVECTOR2(PatternU, PatternV);
		pVtx[1].tex = D3DXVECTOR2((PatternU + 1), PatternV);
		pVtx[2].tex = D3DXVECTOR2(PatternU, (PatternV + 1));
		pVtx[3].tex = D3DXVECTOR2((PatternU + 1), (PatternV + 1));
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　調整用 (2D)
void My2D_VTXBUFFER::UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//UV入力　直接入力 (2D)
void My2D_VTXBUFFER::UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
}

//Color取得(3D)
D3DXCOLOR My3D_VTXBUFFER::GetColor3D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}

//Color取得(2D)
D3DXCOLOR My2D_VTXBUFFER::GetColor2D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_2D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// 頂点データをアンロックする
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}

void My2D_VTXBUFFER::VtxBuffDraw2D(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, *vtxbuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
	pDevice->SetTexture(0, *texture);
	// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (StartVertex * 4), POLYGON_2D_NUM);
}

