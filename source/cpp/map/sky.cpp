/**
* @file field.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/sky.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	SKY_TEXTURE_FILENAME	"../data/TEXTURE/map/sky001.jpg"		// 読み込むテクスチャファイル名

constexpr float	VALUE_MOVE_SKY		{ 5.0f };				// 移動速度
constexpr int	SKY_BLOCK_H			{ 16 };					// 水平のポリゴン数
constexpr int	SKY_BLOCK_V			{ 8 };					// 垂直のポリゴン数
constexpr float	SKY_RADIUS			{ 4000.0f };			// SKY半径
constexpr float	VALUE_ROTATE_SKY	{ D3DX_PI * 0.01f };	// 回転速度
constexpr float	VALUE_TIME_SKY		{ 0.01f };				// 移動速度
constexpr float	SKY_HEIGHT_RATE		{ 2.0f };				// 空ドームの高さ係数
constexpr int	TEX_COUNT_LOOP		{ 1 };					// テクスチャの繰り返し回数

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
SKY::SKY(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	// テクスチャの読み込み
	this->tex.LoadTexture(SKY_TEXTURE_FILENAME);

	//回転速度のセット
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].Addtime = VALUE_TIME_SKY;
	this->SkyPara[SKY_MODEL_TYPE_TOP].Addtime = VALUE_TIME_SKY;

	// ポリゴン表示位置の中心座標を設定
	this->Transform[SKY_MODEL_TYPE_SPHERE].Pos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this->Transform[SKY_MODEL_TYPE_SPHERE].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	this->Transform[SKY_MODEL_TYPE_TOP].Pos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this->Transform[SKY_MODEL_TYPE_TOP].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ブロック数の設定
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH = SKY_BLOCK_H;
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV = SKY_BLOCK_V;
	this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH = SKY_BLOCK_H;
	this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV = SKY_BLOCK_V;

	// 頂点数、インデックス数、ポリゴン数、回転量の設定
	this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertex((this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1) * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1));
	this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertexIndex(this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV * 2) + this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV * 2 + (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV - 1) * 2);
	this->Attribute[SKY_MODEL_TYPE_SPHERE].NumPolygon(this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV * 2) + (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV - 1) * 4);
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].fRotY = VALUE_ROTATE_SKY;

	// 頂点数、インデックス数、ポリゴン数、回転量の設定
	this->Attribute[SKY_MODEL_TYPE_TOP].NumVertex((this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH + 1) * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV + 1));
	this->Attribute[SKY_MODEL_TYPE_TOP].NumVertexIndex(this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV * 2) + this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV * 2 + (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV - 1) * 2);
	this->Attribute[SKY_MODEL_TYPE_TOP].NumPolygon(this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV * 2) + (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV - 1) * 4);
	this->SkyPara[SKY_MODEL_TYPE_TOP].fRotY = VALUE_ROTATE_SKY;

	//頂点作成
	this->vtx[SKY_MODEL_TYPE_SPHERE].MakeVertex3D(this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertex(), FVF_VERTEX_3D);
	this->vtx[SKY_MODEL_TYPE_SPHERE].MakeIdxVertex(this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertexIndex());
	this->vtx[SKY_MODEL_TYPE_TOP].MakeVertex3D(this->Attribute[SKY_MODEL_TYPE_TOP].NumVertex(), FVF_VERTEX_3D);
	this->vtx[SKY_MODEL_TYPE_TOP].MakeIdxVertex(this->Attribute[SKY_MODEL_TYPE_TOP].NumVertexIndex());

	//頂点設定
	this->SetUpMeshSphere();
	this->SetUpMeshTop();
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
SKY::~SKY(void)
{
	//テクスチャ解放
	this->tex.~TEXTURE();

	//カウントループ(アイテムのインスタンス数)　
	for (int CntSky = 0; CntSky < OBJECT_SKY_MAX; CntSky++)
	{
		//頂点解放
		this->vtx[CntSky].~VTXBuffer();
	}
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void SKY::Init()
{
	//色の初期化
	this->ResetColor();

	//タイム初期化
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].time = 1.0f;
	this->SkyPara[SKY_MODEL_TYPE_TOP].time = 1.0f;
}

//=============================================================================
// 更新処理
//=============================================================================
void SKY::Update(void)
{
	//空を回転させる処理
	for (int nCntSky = 0; nCntSky < OBJECT_SKY_MAX; nCntSky++)
	{
		D3DXVECTOR3 rot = this->Transform[nCntSky].Rot();
		rot.y += D3DX_PI * this->SkyPara[nCntSky].fRotY;

		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		this->Transform[nCntSky].Rot(rot);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void SKY::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//DWORD dwSettingLighting, dwSettingCullmode;
	//pDevice->GetRenderState(D3DRS_LIGHTING, &dwSettingLighting);
	//pDevice->GetRenderState(D3DRS_CULLMODE, &dwSettingCullmode);
	//// ライティングを無効に
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	D3DXVECTOR3 pos = this->Transform[SKY_MODEL_TYPE_SPHERE].Pos();
	D3DXVECTOR3 rot = this->Transform[SKY_MODEL_TYPE_SPHERE].Rot();

	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y,rot.x,rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x,pos.y,pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff(), 0, sizeof(VERTEX_3D));
	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(this->vtx[SKY_MODEL_TYPE_SPHERE].IdxBuff());
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(0, this->tex.Texture());
	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertex(), 0, this->Attribute[SKY_MODEL_TYPE_SPHERE].NumPolygon());

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff(), 0, sizeof(VERTEX_3D));
	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(this->vtx[SKY_MODEL_TYPE_TOP].IdxBuff());
	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH + 2), 0, this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH);

	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 裏面をカリング
	//pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
	//// ライティングを有効に
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//pDevice->SetRenderState(D3DRS_LIGHTING, dwSettingLighting);
	//pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
}

//=============================================================================
// 球体SKYセットアップ処理
//=============================================================================
void SKY::SetUpMeshSphere(void)
{
	//サイズ設定用処理
	VERTEX_3D *pVtx;
	const float ANGLE_H = (D3DX_PI * 2.0f) / this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH;
	const float ANGLE_V = (D3DX_PI / 8.0f) / (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1);
	const float WIDTH = 1.0f / this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH;
	const float HEIGHT = 1.0f / this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV;
	float fLengthXZ;
	float fRadius = SKY_RADIUS;

	//頂点バッファのセット
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	VtxBuff = this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntV = 0; nCntV < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1); nCntV++)
	{
		fLengthXZ = cosf(ANGLE_V * nCntV) * fRadius;

		for (int nCntH = 0; nCntH < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1); nCntH++, pVtx++)
		{
			// 頂点座標の設定
			pVtx->vtx.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
			pVtx->vtx.y = sinf(ANGLE_V * nCntV) * fRadius * SKY_HEIGHT_RATE;
			pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// 法線の設定
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

			if (nCntV == this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV)
			{
				pVtx->tex.y = 0.01f;
			}
			else
			{
				pVtx->tex.y = 1.0f - nCntV * HEIGHT;
			}
		}
	}
	// 頂点データをアンロックする
	VtxBuff->Unlock();

	//頂点インデックスのセット
	LPDIRECT3DINDEXBUFFER9	IdxBuff;
	IdxBuff = this->vtx[SKY_MODEL_TYPE_SPHERE].IdxBuff();
	WORD *pIdx;
	// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntV = 0; nCntV < this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV; nCntV++)
	{
		if (nCntV != 0)
		{
			*pIdx = nCntV * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1);
			pIdx++;
		}

		for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1; nCntH++)
		{
			*pIdx = nCntV * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1) + nCntH;
			pIdx++;

			*pIdx = (nCntV + 1) * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1) + nCntH;
			pIdx++;
		}

		if (nCntV != (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV - 1))
		{
			*pIdx = (nCntV + 1) * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1);
			pIdx++;
		}
	}

	// インデックスデータをアンロックする
	IdxBuff->Unlock();

	//セットアップしたデータを反映
	this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff(VtxBuff);
	this->vtx[SKY_MODEL_TYPE_SPHERE].IdxBuff(IdxBuff);
}

//=============================================================================
// 上部SKYセットアップ処理
//=============================================================================
void SKY::SetUpMeshTop(void)
{
	//サイズ設定用処理
	VERTEX_3D *pVtx;
	const float ANGLE_H = (D3DX_PI * 2.0f) / this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH;
	const float ANGLE_V = (D3DX_PI / 8.0f) / (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV + 1);
	const float WIDTH = 1.0f / this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH;
	const float HEIGHT = 1.0f / this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV;
	float fLengthXZ;
	float fRadius = SKY_RADIUS;

	//頂点バッファのセット
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	VtxBuff = this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx->vtx.x = 0.0f;
	pVtx->vtx.y = sinf(ANGLE_V * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV + 1)) * fRadius * SKY_HEIGHT_RATE;
	pVtx->vtx.z = 0.0f;

	// 法線の設定
	pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx->tex.x = 0.01f;
	pVtx->tex.y = 0.01f;

	pVtx++;

	fLengthXZ = cosf(ANGLE_V * this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV) * fRadius;
	for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV; nCntH++, pVtx++)
	{
		// 頂点座標の設定
		pVtx->vtx.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
		pVtx->vtx.y = sinf(ANGLE_V * this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV) * fRadius * SKY_HEIGHT_RATE;
		pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

		// 法線の設定
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;
	}
	// 頂点データをアンロックする
	VtxBuff->Unlock();


	//頂点インデックスのセット
	LPDIRECT3DINDEXBUFFER9	IdxBuff;
	IdxBuff = this->vtx[SKY_MODEL_TYPE_TOP].IdxBuff();
	WORD *pIdx;

	// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH + 1; nCntH++, pIdx++)
	{
		*pIdx = nCntH;
	}
	*pIdx = 1;

	// インデックスデータをアンロックする
	IdxBuff->Unlock();

	//セットアップしたデータを反映
	this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff(VtxBuff);
	this->vtx[SKY_MODEL_TYPE_TOP].IdxBuff(IdxBuff);
}

//=============================================================================
//カラー初期化　球体モデルと上部モデル
//=============================================================================
void SKY::ResetColor(void)
{
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 vtxSphere = this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff();
	LPDIRECT3DVERTEXBUFFER9 vtxTop = this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff();

	//球体モデル処理
	vtxSphere->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntV = 0; nCntV < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1); nCntV++)
	{
		for (int nCntH = 0; nCntH < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1); nCntH++, pVtx++)
		{
			// 頂点カラーの設定
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	vtxSphere->Unlock();

	//上部モデル処理
	vtxTop->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx++;
	for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH; nCntH++, pVtx++) pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vtxTop->Unlock();

	//データ反映
	this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff(vtxSphere);
	this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff(vtxTop);
}