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
#define	WALL_TEXTURE_FILENAME	"../data/TEXTURE/map/interior_fence01_01.png"		// 読み込むテクスチャファイル名

enum eWALL_TYPE
{
	WALL_TYPE_FRONT = 0,
	WALL_TYPE_LEFT,
	WALL_TYPE_RIGHT,
	WALL_TYPE_BACK,
};

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
WALL::WALL(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	// テクスチャの読み込み
	this->tex.LoadTexture(WALL_TEXTURE_FILENAME);

	// ポリゴン表示位置の中心座標を設定
	this->Transform[WALL_TYPE_FRONT].Pos(D3DXVECTOR3(0.0f, 0.0f, WALL_INIT_POSZ));
	this->Transform[WALL_TYPE_LEFT].Pos(D3DXVECTOR3(-WALL_INIT_POSX, 0.0f, 0.0f));
	this->Transform[WALL_TYPE_RIGHT].Pos(D3DXVECTOR3(WALL_INIT_POSX, 0.0f, 0.0f));
	this->Transform[WALL_TYPE_BACK].Pos(D3DXVECTOR3(0.0f, 0.0f, -WALL_INIT_POSZ));

	//ポリゴンの回転角角度を設定
	this->Transform[WALL_TYPE_FRONT].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	this->Transform[WALL_TYPE_LEFT].Rot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f));
	this->Transform[WALL_TYPE_RIGHT].Rot(D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f));
	this->Transform[WALL_TYPE_BACK].Rot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//頂点作成
	this->vtx[WALL_TYPE_FRONT].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);
	this->vtx[WALL_TYPE_LEFT].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);
	this->vtx[WALL_TYPE_RIGHT].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);
	this->vtx[WALL_TYPE_BACK].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);

	//頂点設定
	this->SetUpMesh();
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
WALL::~WALL(void)
{
	//テクスチャ解放
	this->tex.~TEXTURE();

	//カウントループ(アイテムのインスタンス数)　
	for (int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		//頂点解放
		this->vtx[nCntMeshField].~VTXBuffer();
	}
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void WALL::Init(void)
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
		D3DXVECTOR3 pos = this->Transform[nCntMeshField].Pos();
		D3DXVECTOR3 rot = this->Transform[nCntMeshField].Rot();

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
		pDevice->SetStreamSource(0, this->vtx[nCntMeshField].VtxBuff(), 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, this->tex.Texture());

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}
}

//=============================================================================
// WALLセットアップ処理
//=============================================================================
void WALL::SetUpMesh(void)
{
	for (int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		//頂点バッファのセット
		VERTEX_3D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 VtxBuff;
		VtxBuff = this->vtx[nCntMeshField].VtxBuff();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		VtxBuff->Unlock();

		//セットアップしたデータを反映
		this->vtx[nCntMeshField].VtxBuff(VtxBuff);
	}
}