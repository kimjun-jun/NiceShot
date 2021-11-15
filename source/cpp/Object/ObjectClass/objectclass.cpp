/**
* @file objectclass.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
using namespace std;

#include "../../../h/main.h"
#include "../../../h/Debug/debugproc.h"
#include "../../../h/Other/input.h"
#include "../../../h/Object/Fade/fade.h"
#include "../../../h/Object/Camera/camera.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Collision/collision.h"
#include "../../../h/Map/field.h"
#include "../../../h/Map/wall.h"
#include "../../../h/Map/sky.h"
#include "../../../h/Object/Player/player.h"
#include "../../../h/Object/Shadow/shadow.h"
#include "../../../h/Object/Title/title.h"
#include "../../../h/Object/Result/result.h"
#include "../../../h/Object/Tutorial/tutorial.h"
#include "../../../h/Effect/effect.h"
#include "../../../h/Object/Bullet/bullet.h"
#include "../../../h/Effect/explosion.h"
#include "../../../h/Object/Rank/rank.h"
#include "../../../h/Object/CountDown/countdown.h"
#include "../../../h/Object/Item/item.h"
#include "../../../h/Effect/damege.h"
#include "../../../h/Object/Player/Status/status.h"
#include "../../../h/Object/Bullet/bulletprediction.h"
#include "../../../h/Object/Bullet/bulletgauge.h"
#include "../../../h/Object/Player/VitalGauge/vitalgauge.h"
#include "../../../h/net/netmatch.h"
#include "../../../h/net/sock.h"
#include "../../../h/Object/ObjectClass/objectclass.h"

int GAME_OBJECT::AllOBJCnt = 0;


#define MAX_SPEEDBUFF						(1.5f)																		//!< スピードバフ移動量最大値(積)
#define MAX_SPEEDBUFFTIME					(120.0f*3)																	//!< スピードバフ有効時間
#define	EXPLOSION_COLLISIONPOS_BUFFSIZE		(5.0f)																		//!< 2D爆破を壁に当たった時の描画する座標を調整
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define ITEM_MODEL_SIZE						(15.0f)																		//!< モデルサイズ



void SetOjama(int type, int UsePlayer, PLAYER *p);

//マッチフラグ
bool NetMatchFlag = false;
void SetNetMatchFlag(bool flag) { NetMatchFlag = flag; }

//マイナンバー
bool NetMyNumberFlag = false;
int NetMyNumber = -1;
void SetNetMyNumberFlag(bool flag) { NetMyNumberFlag = flag; }
void SetNetMyNumber(int num) { NetMyNumber = num; }
int GetNetMyNumber(void) { return NetMyNumber; }

//アイテムフラグ　trueでアイテム情報取得完了、カウントダウン信号を受け取り始める
bool NetItemFlag = false;
void SetNetItemFlag(bool flag) { NetItemFlag = flag; }
bool GetNetItemFlag(void) { return NetItemFlag; }

//ネットフラグ　trueでネット対戦始める
bool NetGameStartFlag = false;
void SetNetGameStartFlag(bool flag) { NetGameStartFlag = flag; }

//ネット対戦　共有データアクセス許可フラグ
bool NetShareDateFlag = false;
void SetNetShareDateFlag(bool flag) { NetShareDateFlag = flag; }
bool GetNetShareDateFlag(void) { return NetShareDateFlag; }

//使用するタイプを変更する
//void iUseCheak::ChangeUse(eUse_Type UseType)
//{
//	this->Use(UseType);
//}

//テクスチャ読み込み処理
void TEXTURE::LoadTexture(const char *FileName)
{
	D3DXCreateTextureFromFile(GetDevice(), FileName, &this->pD3DTexture);
}

//頂点[作成]時設定(3D)
//作成のみ (3D)
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF)
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
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF)
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF)
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
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
void VTXBuffer::MakeIdxVertex(const int VTXIdxNum)
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
void VTXBuffer::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
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
void VTXBuffer::Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4])
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
void VTXBuffer::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
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
void VTXBuffer::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
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
void VTXBuffer::Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4])
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
void VTXBuffer::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
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
void VTXBuffer::Nor3D(const int Indx)
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
void VTXBuffer::Nor3D(const int Indx, const D3DXVECTOR3 Nor[4])
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
void VTXBuffer::RHW2D(const int Indx)
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
void VTXBuffer::Color3D(const int Indx)
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
void VTXBuffer::Color3D(const int Indx, const D3DXCOLOR col)
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
void VTXBuffer::Color2D(const int Indx)
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
void VTXBuffer::Color2D(const int Indx, const D3DXCOLOR col)
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
void VTXBuffer::UV3D(const int Indx)
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
void VTXBuffer::UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
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
void VTXBuffer::UV2D(const int Indx)
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
void VTXBuffer::UV2D(const int Indx, const float PatternU, const float PatternV)
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
void VTXBuffer::UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
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
D3DXCOLOR VTXBuffer::GetColor3D(const int Indx)
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
D3DXCOLOR VTXBuffer::GetColor2D(const int Indx)
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

void GAME_OBJECT::Generate()
{
	player = new PLAYER;								//ok 
	effect = new EFFECT;								//ok
	bullet = new BULLET;								//ok
	shadow = new SHADOW;								//ok
	countdown = new COUNTDOWN;							//ok
	tuto = new TUTO;									//ok
	netmatch = new NETMATCH;							//ok 改良中
	status = new STATUS;								//ok
	bulletprediction = new BULLETPREDICTION;			//ok
	vitalgauge = new VITALGAUGE;						//ok
	bulletgauge = new BULLETGAUGE;						//ok
	damege = new DAMEGE;								//ok
	explosion = new EXPLOSION;							//ok
	item = new ITEM;									//ok
	rank = new RANK;									//ok
	result = new RESULT;								//ok
	title = new TITLE;									//ok
	field = new FIELD;									//ok
	sky = new SKY;										//ok
	wall = new WALL;									//ok
	fade = new FADE;									//ok

	mysocket = new MySOCKET;
	this->GameSceneFlagFunc(true);
	this->MultThreadFlagFunc(false);
}

void GAME_OBJECT::Init()
{
	//ネット対戦用
	NetMatchFlag = false;
	NetMyNumberFlag = false;
	NetMyNumber = -1;
	NetItemFlag = false;
	NetGameStartFlag = false;
	NetShareDateFlag = false;


	//こっちの書き方のほうが短くなる　引数が必要な場合は例外処理をするか　引数を必要としない構造にする
	//for (int CntOBJ = 0; CntOBJ < AllOBJCnt; CntOBJ++) { this[CntOBJ].Init(); }

	field->Init();
	player->Init(&field[0]);
	effect->Init();
	bullet->Init();
	shadow->Init();
	countdown->Init();
	tuto->Init();
	status->Init();
	bulletprediction->Init();
	vitalgauge->Init();
	bulletgauge->Init();
	damege->Init();
	explosion->Init();
	item->Init();
	rank->Init();
	result->Init();
	title->Init();
	sky->Init();
	wall->Init();
	mysocket->Init();

	this->MultThreadFlagFunc(false);
}

void GAME_OBJECT::InitNet()
{
	//スクリーンのUIを再設定してるだけ
	status->InitNet(NetMyNumber);
	vitalgauge->InitNet(NetMyNumber);
	bulletgauge->InitNet(NetMyNumber);
	damege->InitNet(NetMyNumber);
	rank->InitNet(NetMyNumber);
}

void GAME_OBJECT::Delete()
{
	delete player;
	delete effect;
	delete bullet;
	delete shadow;
	delete countdown;
	delete tuto;
	delete netmatch;
	delete status;
	delete bulletprediction;
	delete vitalgauge;
	delete bulletgauge;
	delete damege;
	delete explosion;
	delete item;
	delete rank;
	delete result;
	delete title;
	delete field;
	delete sky;
	delete wall;
	delete fade;
	delete mysocket;
}

void GAME_OBJECT::Update()
{
	//ロックする
	SetNetShareDateFlag(true);

	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	// 入力更新
	UpdateInput();
#ifdef _DEBUG

	//一時停止処理
	if (GetKeyboardTrigger(DIK_F12) || IsButtonTriggered(0, BUTTON_SELECT)) stop++;
	if (GetKeyboardTrigger(DIK_F11) || IsButtonTriggered(0, BUTTON_L3))
	{
		MasterVolumeChange(1);
		this->Init();
		fade->SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_title01);
	}
#endif

	if (stop == false)
	{
		// カメラの更新処理
		UpdateCamera();

		switch (this->nScene)
		{
		case SCENE_TITLE:
			//タイトル更新
			title->Update(&this[0], &fade[0]);
			break;
		case SCENE_TUTORIAL:
			//チュートリアル更新
			tuto->Update(&this[0], &fade[0]);
			// map更新
			field->Update(&player[0], &item[0], &bullet[0], &explosion[0], &shadow[0]);
			sky->Update();

			//3D空間
			player->Update(&effect[0], &bullet[0], &shadow[0], &fade[0], NetGameStartFlag, NetMyNumber);
			bullet->Update(&shadow[0], &effect[0]);
			bulletprediction->Update(&player[0]);
			effect->Update();
			explosion->Update();
			item->Update(&player[0], &shadow[0], NetGameStartFlag);
			shadow->Update();
			//
			CheckHit(0);
			//
			//2D空間
			bulletgauge->Update(&player[0]);
			damege->Update();
			status->Update(&player[0]);
			break;

		case SCENE_GAMECOUNTDOWN:
			//カウントダウンの更新
			countdown->Update(&this[0], NetGameStartFlag);
			countdown->AddCountdown(-1);
			break;

		case SCENE_GAME:
			// map更新
			field->Update(&player[0], &item[0], &bullet[0], &explosion[0], &shadow[0]);
			sky->Update();

			//オブジェクトの更新
			player->Update(&effect[0], &bullet[0], &shadow[0], &fade[0], NetGameStartFlag, NetMyNumber);
			bullet->Update(&shadow[0], &effect[0]);
			bulletprediction->Update(&player[0]);
			effect->Update();
			explosion->Update();
			item->Update(&player[0], &shadow[0], NetGameStartFlag);
			shadow->Update();

			//当たり判定
			CheckHit(1);

			//2D空間
			bulletgauge->Update(&player[0]);
			damege->Update();
			status->Update(&player[0]);
			vitalgauge->Update(&player[0], &rank[0], NetGameStartFlag, NetMyNumber);
			break;

		case SCENE_NETMATCH:
			netmatch->Update(&this[0], &fade[0]);
			//マッチング中
			if (NetMatchFlag == false)
			{
				this->mysocket->NetMatch();
			}
			//マイナンバー取得中
			if (NetMyNumberFlag == false)
			{
				if (NetMyNumber >= -1 && NetMyNumber <= 3) this->mysocket->NetMyNumberGet();
			}
			//アイテム情報取得中
			else if (NetItemFlag==false)
			{
				this->mysocket->NetItemGet(item);
			}
			//カウントダウン信号待ち中
			if(NetMatchFlag == true && NetMyNumberFlag == true && NetItemFlag == true&& NetGameStartFlag == false)
			{
				this->InitNet();
				this->mysocket->NetCountdown();
			}
			//スタートフラグが送られてきて信号がONになったらカウントダウン開始
			if (NetGameStartFlag == true)
			{
				fade->SetFade(FADE_OUT, SCENE_NETGAMECOUNTDOWN, SOUND_LABEL_BGM_boss01);
				SourceVolumeChange(0, SOUND_LABEL_BGM_boss01);
			}

			//ゲーム終了処理
			//if (GetGameLoop() == true)
			//{
			//	SetEndGame(true);
			//	this->MultThreadFlagFunc(false);
			//	this->GameSceneFlagFunc(false);
			//}
			break;
		case SCENE_NETGAMECOUNTDOWN:
			//カウントダウンの更新
			countdown->Update(&this[0], NetGameStartFlag);
			countdown->AddCountdown(-1);
			break;
		case SCENE_NETGAME:
			//パケットはマルチスレッドでロックされていないとき(Draw中はロックされている)に常に反映させる

			// map更新
			field->Update(&player[0], &item[0], &bullet[0], &explosion[0], &shadow[0]);//パケット有り
			sky->Update();

			//オブジェクトの更新
			player->Update(&effect[0], &bullet[0], &shadow[0], &fade[0], NetGameStartFlag, NetMyNumber);//パケット有り
			bullet->Update(&shadow[0], &effect[0]);//パケット有り
			bulletprediction->Update(&player[0]);
			effect->Update();//パケット有り
			explosion->Update();//パケット有り
			item->Update(&player[0], &shadow[0], NetGameStartFlag);//パケット有り
			shadow->Update();//パケット有り

			//当たり判定
			CheckHit(1);

			//2D空間
			bulletgauge->Update(&player[0]);
			damege->Update();//パケット有り
			status->Update(&player[0]);
			vitalgauge->Update(&player[0], &rank[0], NetGameStartFlag, NetMyNumber);
			break;
		case SCENE_RESULT:
			//リザルトの更新
			result->Update(&this[0], &fade[0]);
			break;
		}
		// フェード処理
		fade->Update(&this[0]);
	}
}

void GAME_OBJECT::Draw()
{
	//四人分の画面分割設定
	D3DVIEWPORT9 vp[]
	{
		{DWORD(0),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(0),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
	};
	D3DVIEWPORT9 VpMaster{ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// バックバッファ＆Ｚバッファのクリア
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(pD3DDevice->BeginScene()))
	{
		// 画面遷移
		switch (this->nScene)
		{
		case SCENE_TITLE:
			//タイトル描画
			title->Draw();
			break;
		case SCENE_TUTORIAL:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

				if (player->PlayerPara[CntPlayer].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

				// カメラの設定
				SetCamera(CntPlayer);

				// map描画
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D空間
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], CntPlayer);
				explosion->Draw(CntPlayer);
				effect->Draw(CntPlayer);
				shadow->Draw();

				//2d画面上
				damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				tuto->Draw();
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;
		case SCENE_GAMECOUNTDOWN:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);
				// カメラの設定
				SetCamera(CntPlayer);

				// map描画
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D空間
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], CntPlayer);
				explosion->Draw(CntPlayer);
				effect->Draw(CntPlayer);
				shadow->Draw();

				//2d画面上
				damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
			}
			pD3DDevice->SetViewport(&VpMaster);
			countdown->Draw();
			break;
		case SCENE_GAME:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

				bool puse = player->iUseType[CntPlayer].Use();
				if (puse == true)
				{
					if (player->PlayerPara[CntPlayer].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(CntPlayer);

					// map描画
					field->Draw();
					sky->Draw();
					wall->Draw();

					//3D空間
					player->Draw();
					item->Draw();
					bulletprediction->Draw(&player[0], CntPlayer);
					explosion->Draw(CntPlayer);
					effect->Draw(CntPlayer);
					shadow->Draw();

					//2d画面上
					damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(CntPlayer);

					// map描画
					field->Draw();
					sky->Draw();
					wall->Draw();

					//3D空間
					player->Draw();
					item->Draw();
					bulletprediction->Draw(&player[0], CntPlayer);
					explosion->Draw(CntPlayer);
					effect->Draw(CntPlayer);
					shadow->Draw();

					//2d画面上
					damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					rank->Draw(NetGameStartFlag, NetMyNumber);
				}
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;


		case SCENE_NETMATCH:
			netmatch->Draw();

			break;
		case SCENE_NETGAMECOUNTDOWN:
		{
			// カメラの設定
			SetCamera(NetMyNumber);

			// map描画
			field->Draw();
			sky->Draw();
			wall->Draw();

			//3D空間
			player->Draw();
			item->Draw();
			bulletprediction->Draw(&player[0], NetMyNumber);
			explosion->Draw(0);
			effect->Draw(0);
			shadow->Draw();

			//2d画面上
			damege->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			status->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			vitalgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			bulletgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			countdown->Draw();
			break;
		}
		case SCENE_NETGAME:
		{
			bool puseNet = player->iUseType[NetMyNumber].Use();
			if (puseNet == true)
			{
				if (player->PlayerPara[NetMyNumber].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

				// カメラの設定
				SetCamera(NetMyNumber);

				// map描画
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D空間
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], NetMyNumber);
				explosion->Draw(NetMyNumber);
				effect->Draw(NetMyNumber);
				shadow->Draw();

				//2d画面上
				damege->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				status->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			}
			else
			{
				pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

				// カメラの設定
				SetCamera(NetMyNumber);

				// map描画
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D空間
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], NetMyNumber);
				explosion->Draw(0);
				effect->Draw(0);
				shadow->Draw();

				//2d画面上
				damege->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				status->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				rank->Draw(NetGameStartFlag, NetMyNumber);
			}

			break;
		}
		case SCENE_RESULT:
			//リザルト描画
			result->Draw();
			break;
		}

		// フェード描画
		fade->Draw();

#ifdef _DEBUG
		// デバッグ表示
		DrawTextType();
		DrawDebugProc();
#endif
	}
	// Direct3Dによる描画の終了
	pD3DDevice->EndScene();
	// バックバッファとフロントバッファの入れ替え
	pD3DDevice->Present(NULL, NULL, NULL, NULL);


	//スレッド間の共有変数を解除
	//m.unlock();

	//解除する
	SetNetShareDateFlag(false);
}

void GAME_OBJECT::CheckHit(int scene)
{
	//プレイヤーに対する当たり判定
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (player->iUseType[CntPlayer].Use() == YesUseType1)
		{
			//オブジェクト値読み込み
			D3DXVECTOR3 ppos = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
			D3DXVECTOR3 poldpos = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldPos();

			//保存
			player->PlayerPara[CntPlayer].StandardPara.OldVital = player->PlayerPara[CntPlayer].StandardPara.Vital;

			//プレイヤー対壁
			for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
			{

				D3DXVECTOR3 wpos = wall->Transform[CntWall].Pos();
				switch (CntWall)
				{
				case 0:
					if (ppos.z + PLAYER_MODEL_SIZE >= wpos.z) ppos.z = poldpos.z;
					break;
				case 1:
					if (ppos.x - PLAYER_MODEL_SIZE <= wpos.x) ppos.x = poldpos.x;
					break;
				case 2:
					if (ppos.x + PLAYER_MODEL_SIZE >= wpos.x) ppos.x = poldpos.x;
					break;
				case 3:
					if (ppos.z - PLAYER_MODEL_SIZE <= wpos.z) ppos.z = poldpos.z;
					break;
				default:
					break;
				}
				//オブジェクト値書き込み
				player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(ppos);
			}

			//プレイヤーバレット対プレイヤー、壁、床
			for (int CntPlayerBullet = 0; CntPlayerBullet < OBJECT_BULLET_MAX; CntPlayerBullet++)
			{
				//オブジェクト値読み込み
				D3DXVECTOR3 bpos = bullet->Transform[CntPlayerBullet].Pos();
				eUse_Type buse = bullet->iUseType[CntPlayerBullet].Use();

				if (buse == YesUseType1)
				{
					//対プレイヤー
					if (bullet->BulletPara[CntPlayerBullet].UsePlayerType != CntPlayer)
					{
						if (CollisionBC(ppos, PLAYER_MODEL_SIZE, bpos, BULLET_MODEL_SIZE))
						{
							// エフェクト爆発の生成
							effect->SetInstance(bpos, VEC3_ALL0,
								PLAYER_COLOR[bullet->BulletPara[CntPlayerBullet].UsePlayerType], 150.0f, 150.0f, 40);
							if (scene == 1)
							{
								player->PlayerPara[CntPlayer].StandardPara.Vital -= PLAYER_ATTACK_NORMAL;
							}
							//画面ダメージエフェクト
							damege->iUseType[CntPlayer].Use(YesUseType1);
							damege->DamegePara[CntPlayer].time = 0;
							damege->DamegePara[CntPlayer].alpha = 0;

							// バレット破棄
							bullet->ReleaseInstance(CntPlayerBullet,shadow);

							// SE再生
							PlaySound(SOUND_LABEL_SE_attack02);
							break;
						}
					}
					//対壁
					for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
					{
						D3DXVECTOR3 wpos = wall->Transform[CntWall].Pos();
						switch (CntWall)
						{
							//フラグ立てて、関数呼び出し書く数減らす
						case 0:
							if (bpos.z >= wpos.z)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z- EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 1:
							if (bpos.x <= wpos.x)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x + EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 2:
							if (bpos.x >= wpos.x)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x - EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 3:
							if (bpos.z <= wpos.z)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z + EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						default:
							break;
						}
						if (buse == NoUse) break;
					}
				}
			}

			//プレイヤー対アイテム
			for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
			{
				//オブジェクト値読み込み
				D3DXVECTOR3 ipos = item->Transform[CntItem].Pos();
				bool iuse = item->iUseType[CntItem].Use();
				if (iuse == false || item->ItemParaAll[CntItem].GettingSignal == true || item->ItemParaAll[CntItem].GettingSignalEnd == true) continue;
				if (CollisionBC(ppos, PLAYER_MODEL_SIZE, ipos, ITEM_MODEL_SIZE))
				{
					switch (item->ItemParaAll[CntItem].eType)
					{
					case ITEM_TYPE_TIKEI:
						//地形変形信号ON
						if (NetGameStartFlag==true&& CntPlayer== NetMyNumber)
						{
							this->field->FieldPara.ItemGetPlayerNum = CntPlayer;
							//ランダム関数のseed値を決める
							this->field->FieldPara.TikeiSeed = (rand() % 1000000);
							//通信しているときはこのシード値を同期させて、これをもとに同じ地形を生成する
							//srand(this[0].field->TikeiSeed);
							//NetSetTikeiSeed(this[0].field->TikeiSeed, this[0].field->GetPlayerNum);
						}
						else if(NetGameStartFlag == false)
						{
							this->field->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, CntPlayer, &item[0]);
							//SetFieldInterPolationFieldType(0);
							PlaySound(SOUND_LABEL_SE_enter03);
							PlaySound(SOUND_LABEL_SE_quake);

						}
						break;
					case ITEM_TYPE_LIFE:
						//体力回復
						player->PlayerPara[CntPlayer].StandardPara.Vital += PLAYER_ATTACK_NORMAL;
						if (player->PlayerPara[CntPlayer].StandardPara.Vital >= PLAYER_VITAL_MAX) player->PlayerPara[CntPlayer].StandardPara.Vital = PLAYER_VITAL_MAX;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_SENSYA:
						//通常モデルならモーフィング信号ON
						if (player->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_NORMAL)
						{
							//モーフィング開始信号、モデルタイプ、モーフィング中信号、モーフィング時間、モーフィング終了カウントのセット
							player->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							player->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//モーフィング終了変化中にアイテムを拾った時はもう一度モーフィングさせる
						else if (player->PlayerPara[CntPlayer].MorphingPara.MorphingStart == false && player->PlayerPara[CntPlayer].MorphingPara.MorphingTime <= 0.0f)
						{
							player->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							player->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//アタックモデル中にアイテム拾ったら時間延長
						else
						{
							player->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						player->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = true;
						PlaySound(SOUND_LABEL_SE_rap1);
						break;
					case ITEM_TYPE_BULLET:
						player->PlayerPara[CntPlayer].BulletPara.BulletStock = BULLET_MAX_STOCK;
						player->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_SPEEDUP:
						player->PlayerPara[CntPlayer].ItemPara.SpeedBuff = MAX_SPEEDBUFF;
						player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime = MAX_SPEEDBUFFTIME;
						player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = true;
						PlaySound(SOUND_LABEL_SE_speed);
						break;
					case ITEM_TYPE_CAMERA:
						SetOjama(ITEM_TYPE_CAMERA, CntPlayer, &player[0]);
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_KIRI:
						SetOjama(ITEM_TYPE_KIRI, CntPlayer, &player[0]);
						PlaySound(SOUND_LABEL_SE_kiri);
						break;
					default:

						break;
					}
					PlaySound(SOUND_LABEL_SE_kurukuru);
					item->ItemParaAll[CntItem].GettingSignal = true;
					item->ItemParaAll[CntItem].GetPlayerType = CntPlayer;
					item->ItemParaAll[CntItem].NetGetItemFlag = true;
				}
			}
		}
	}
}

void GAME_OBJECT::PackectAll(void)
{
	while (GetEndGame() == false)
	{
		mysocket->Packet(player, item, field, bullet,shadow);
	}
}

void SetOjama(int type, int UsePlayer, PLAYER *p)
{
	//カメラ反転
	if (type == ITEM_TYPE_CAMERA)
	{
		//対象プレイヤーのみカメラ反転
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraTime = 0.0f;
		}
	}
	//フォグ
	else if (type == ITEM_TYPE_KIRI)
	{
		//対象プレイヤーのみフォグ発動
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.KiriSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.KiriTime = 0.0f;
		}
	}
}





