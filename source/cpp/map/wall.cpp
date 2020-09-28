/**
* @file wall.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME	"../data/TEXTURE/interior_fence01_01.png"		// 読み込むテクスチャファイル名
#define	VALUE_MOVE_WALL		(5.0f)							// 移動速度
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.002f)				// 回転速度

int WALL::cnt = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void WALL::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//座標と角度を設定
	D3DXVECTOR3 pos[OBJECT_WALL_MAX];
	D3DXVECTOR3 rot[OBJECT_WALL_MAX];
	pos[0] = D3DXVECTOR3(0.0f, 0.0f, WALL_INIT_POSZ);
	pos[1] = D3DXVECTOR3(-WALL_INIT_POSX, 0.0f, 0.0f);
	pos[2] = D3DXVECTOR3(WALL_INIT_POSX, 0.0f, 0.0f);
	pos[3] = D3DXVECTOR3(0.0f, 0.0f, -WALL_INIT_POSZ);

	rot[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot[1] = D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f);
	rot[2] = D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f);
	rot[3] = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	this[0].SetPos(pos[0]);
	this[1].SetPos(pos[1]);
	this[2].SetPos(pos[2]);
	this[3].SetPos(pos[3]);
	this[0].SetRot(rot[0]);
	this[1].SetRot(rot[1]);
	this[2].SetRot(rot[2]);
	this[3].SetRot(rot[3]);

	for (int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &this[nCntMeshField].model.pD3DTexture);

		// オブジェクトの頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
			FVF_VERTEX_3D,						// 使用する頂点フォーマット
			D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
			&this[nCntMeshField].model.pD3DVtxBuff,				// 頂点バッファインターフェースへのポインタ
			NULL);								// NULLに設定

		{//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			this[nCntMeshField].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// 頂点座標の設定
					pVtx[0].vtx = D3DXVECTOR3(-WALL_SIZE_X / 2, WALL_SIZE_Y, 0.0f);
					pVtx[1].vtx = D3DXVECTOR3(WALL_SIZE_X / 2, WALL_SIZE_Y, 0.0f);
					pVtx[2].vtx = D3DXVECTOR3(-WALL_SIZE_X / 2, 0.0f, 0.0f);
					pVtx[3].vtx = D3DXVECTOR3(WALL_SIZE_X / 2, 0.0f, 0.0f);

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

			// 頂点データをアンロックする
			this[nCntMeshField].model.pD3DVtxBuff->Unlock();
		}
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void WALL::Reinit(void)
{
}

//=============================================================================
// 終了処理
//=============================================================================
void WALL::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void WALL::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void WALL::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for(int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
		//-----------------------------オブジェクト読み込み
		mtxWorld = this[nCntMeshField].GetMatrix();
		D3DXVECTOR3 rot = this[nCntMeshField].GetRot();
		D3DXVECTOR3 pos = this[nCntMeshField].GetPos();

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
		pDevice->SetStreamSource(0, this[nCntMeshField].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, this[nCntMeshField].model.pD3DTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}
}

