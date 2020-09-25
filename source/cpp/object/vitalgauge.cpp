/**
* @file vitalgauge.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../../h/scene/rank.h"
#include "../../h/object/vitalgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_VITALGAUGE		"../data/TEXTURE/UI_VITALGAUGE.jpg"		// 読み込むテクスチャファイル名
#define	VITALGAUGE_SIZE_X			(1.0f)							// アタックゲージの数字の幅
#define	VITALGAUGE_SIZE_Y			(30.0f)							// アタックゲージの数字の高さ
#define	VITALGAUGE_POS_X			(25.0f)							// アタックゲージの表示基準位置Ｘ座標
#define	VITALGAUGE_POS_Y			(70.0f)							// アタックゲージの表示基準位置Ｙ座標

#define VITALGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y

//=============================================================================
// 初期化処理
//=============================================================================
void VITALGAUGE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this[0].SetPos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));

	// 頂点情報の作成
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		MakeVertexVitalGauge(pDevice,CntPlayer);

		LPDIRECT3DTEXTURE9 pD3DTexture;
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_VITALGAUGE,			// ファイルの名前
			&pD3DTexture);	// 読み込むメモリー
		this[CntPlayer].tex2DVB.SetpD3DTexture(pD3DTexture);
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void VITALGAUGE::Reinit(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		// ライフの初期化
		this[CntPlayer].VitalPower = PLAYER_VITAL;

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			pD3DVtxBuff[CntPlayer].Lock(0, 0, (void**)&pVtx, 0);
			for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
			{
				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			}
			// 頂点データをアンロックする
			pD3DVtxBuff[CntPlayer].Unlock();

			this[CntPlayer].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void VITALGAUGE::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void VITALGAUGE::Update(void)
{
	//-----------------------------------オブジェクト先頭アドレスを読み込み
	GAME_OBJECT *playerobj = this->GetPointerPlayer();
	PLAYER_HONTAI *p = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		bool puse = p[CntPlayer].GetUse();
		if (puse == true)
		{
			if (this[CntPlayer].VitalPower <= 0)
			{
				p[CntPlayer].SetUse(false);
				//-----------------------------------オブジェクト先頭アドレスを読み込み
				GAME_OBJECT *rankobj = this->GetPointerRank();
				RANK *rank = dynamic_cast<RANK*>(&rankobj[0]);
				rank->SetRank(CntPlayer);
			}
		}
		this[CntPlayer].VitalPower = p[CntPlayer].vital;
		//体力一定以上で緑色
		if (this[CntPlayer].VitalPower >= PLAYER_VITAL * (0.2f))
		{
			{//頂点バッファの中身を埋める
				VERTEX_2D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
				// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
				pD3DVtxBuff[CntPlayer].Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// 反射光の設定
					pVtx[0].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				}
				// 頂点データをアンロックする
				pD3DVtxBuff[CntPlayer].Unlock();
			}
		}
		//体力一定以下で赤色
		else
		{
			{//頂点バッファの中身を埋める
				VERTEX_2D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
				// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
				pD3DVtxBuff[CntPlayer].Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// 反射光の設定
					pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
				}
				// 頂点データをアンロックする
				pD3DVtxBuff[CntPlayer].Unlock();
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void VITALGAUGE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
		pDevice->SetStreamSource(0, pD3DVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		LPDIRECT3DTEXTURE9 pD3DTexture = this[CntPlayer].tex2DVB.GetpD3DTexture;
		pDevice->SetTexture(0, pD3DTexture);

		// ポリゴンの描画
		for (int nCntPlace = 0; nCntPlace < this[CntPlayer].VitalPower; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT VITALGAUGE::MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// オブジェクトの頂点バッファを生成
	LPDIRECT3DVERTEXBUFFER9 VtxBuffBulletGauge = NULL;
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_VITAL + 4),		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
												FVF_VERTEX_2D,								// 使用する頂点フォーマット
												D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
												&VtxBuffBulletGauge,							// 頂点バッファインターフェースへのポインタ
												NULL)))										// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxBuffBulletGauge->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
		{
			//--------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[CntPlayer].GetPos();
			// 頂点座標の設定
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, -VITALGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, VITALGAUGE_SIZE_Y/2, 0.0f) + pos;

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		VtxBuffBulletGauge->Unlock();

		//---------------------------オブジェクト値書き込み
		this[CntPlayer].tex2DVB.SetpD3DVtxBuff(VtxBuffBulletGauge);
	}

	return S_OK;
}

