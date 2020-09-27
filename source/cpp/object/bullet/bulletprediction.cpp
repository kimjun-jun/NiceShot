/**
* @file bulletprediction.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/other/input.h"
#include "../../../h/object/camera.h"
#include "../../../h/object/player.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/bullet/bulletprediction.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETPREDICTION		"../data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.01f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.01f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.01f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.01f),//p4カラー 
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletprediction[OBJECT_PLAYER_MAX] = { NULL };		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletprediction[OBJECT_PLAYER_MAX] = { NULL };		// 頂点バッファインターフェースへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
void BULLETPREDICTION::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexBulletprediction(pDevice);
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_BULLETPREDICTION,			// ファイルの名前
			&g_pD3DTextureBulletprediction[CntPlayer]);	// 読み込むメモリー
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void BULLETPREDICTION::Reinit(void)
{
	for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++)
	{
		this[nCntBulletprediction].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntBulletprediction].SetCol(D3DXCOLOR(DWORD(0)));
		this[nCntBulletprediction].SetUse(false);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void BULLETPREDICTION::Uninit(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
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
void BULLETPREDICTION::Update(PLAYER_HONTAI *player)
{
	for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++)
	{
		bool use = this[nCntBulletprediction].GetUse();
		if (use)
		{
			this[nCntBulletprediction].SetUse(false);
		}
	}

	//プレイヤーの情報から発射位置角度移動量を利用してバレットの着弾点を算出する
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//---------------------------------オブジェクト値読み込み
		D3DXVECTOR3	BulletPredictionPos = player[CntPlayer].GetPos();
		D3DXVECTOR3 HoudaiRot = player[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = player[CntPlayer].parts[PARTSTYPE_HOUTOU]->GetRot();
		D3DXVECTOR3 HousinRot = player[CntPlayer].parts[PARTSTYPE_HOUSIN]->GetRot();

		//発射角度、発射座用計算
		D3DXVECTOR3 BmoveRot;
		BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
		BmoveRot.y = sinf(player[CntPlayer].Brot - HousinRot.x);
		BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);
		D3DXVECTOR3 bulletmove;
		bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
		bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
		bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

		D3DXCOLOR	col = PLAYER_COLOR[CntPlayer];
		float Gravity = 0.0f;
		//float time = 1.0f;
		//float maxtime = 10.0f;
		for (int nCntBulletprediction = 0; nCntBulletprediction < BULLETPREDICTION_MAX; nCntBulletprediction++)
		{
			//重力と移動量を反映
			//BulletPredictionPos.x += player[CntPlayer].bulletmove.x*(time / maxtime);
			//BulletPredictionPos.y -= player[CntPlayer].bulletmove.y*(time / maxtime) + Gravity;
			//BulletPredictionPos.z += player[CntPlayer].bulletmove.z*(time / maxtime);

			BulletPredictionPos.x += bulletmove.x;
			BulletPredictionPos.y -= bulletmove.y + Gravity;
			BulletPredictionPos.z += bulletmove.z;

			//重力を加速させる
			Gravity += VALUE_GRAVITYADD_BULLET;
			//時間割合を進める。飛距離が増えるほど弾道予測間隔が広くなる
			//time++;
			//重力最大値制限
			if (Gravity > VALUE_GRAVITYMAX_BULLET) Gravity = VALUE_GRAVITYMAX_BULLET;
			//徐々にアルファ値を強くして遠距離地点を見やすくする
			col.a += 0.01f;
			SetBulletprediction(BulletPredictionPos, CntPlayer, col, BULLETPREDICTION_SIZE_X, BULLETPREDICTION_SIZE_Y);


		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void BULLETPREDICTION::Draw(PLAYER_HONTAI *player)
{
	//-----------------------------------オブジェクト先頭アドレスを読み込み
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool puse = player[CntPlayer].GetUse();
		if (puse)
		{
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			for (int nCntBulletprediction = 0; nCntBulletprediction < BULLETPREDICTION_MAX; nCntBulletprediction++)
			{
				D3DXMATRIX mtxView, mtxScale, mtxTranslate;

				bool bpuse = this[nCntBulletprediction].GetUse();
				if (bpuse)
				{
					//---------------------------------オブジェクト値読み込み
					D3DXVECTOR3	pos = this[nCntBulletprediction].GetPos();
					D3DXVECTOR3	scl = this[nCntBulletprediction].GetScl();
					D3DXMATRIX mtxWorld = this[nCntBulletprediction].GetMatrix();

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&mtxWorld);

					// ビューマトリックスを取得
					CAMERA *cam = GetCamera();

					mtxWorld._11 = cam[CntPlayer].mtxView._11;
					mtxWorld._12 = cam[CntPlayer].mtxView._21;
					mtxWorld._13 = cam[CntPlayer].mtxView._31;
					mtxWorld._21 = cam[CntPlayer].mtxView._12;
					mtxWorld._22 = cam[CntPlayer].mtxView._22;
					mtxWorld._23 = cam[CntPlayer].mtxView._32;
					mtxWorld._31 = cam[CntPlayer].mtxView._13;
					mtxWorld._32 = cam[CntPlayer].mtxView._23;
					mtxWorld._33 = cam[CntPlayer].mtxView._33;

					// スケールを反映
					D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

					// 移動を反映
					D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

					// ワールドマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

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
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT BULLETPREDICTION::MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * OBJECT_BULLETPREDICTION_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
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

			for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++, pVtx += 4)
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
void BULLETPREDICTION::SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY)
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
void BULLETPREDICTION::SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col)
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
int BULLETPREDICTION::SetBulletprediction(D3DXVECTOR3 pos, int PlayerType,D3DXCOLOR col, float fSizeX, float fSizeY)
{
	int nIdxBulletprediction = -1;

	for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++)
	{
		bool use = this[nCntBulletprediction].GetUse();
		if (use != true)
		{
			this[nCntBulletprediction].SetPos(pos);
			this[nCntBulletprediction].SetCol(col);
			this[nCntBulletprediction].SetUse(true);

			// 頂点座標の設定
			SetVertexBulletprediction(PlayerType,nCntBulletprediction, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorBulletprediction(PlayerType,nCntBulletprediction,col);
			nIdxBulletprediction = nCntBulletprediction;
			break;
		}
	}
	return nIdxBulletprediction;
}
