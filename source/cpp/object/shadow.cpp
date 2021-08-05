/**
* @file shadow.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW		"../data/TEXTURE/other/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	SHADOW_SIZE_X		(25.0f)							// 影の幅
#define	SHADOW_SIZE_Z		(25.0f)							// 影の高さ

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
SHADOW::SHADOW(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex3DBill(OBJECT_SHADOW_MAX, FVF_VERTEX_3D);

	//カウントループ
	for (int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		//描画位置反映
		D3DXVECTOR3 pos[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2),
		D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2),
		D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2),
		D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2),
		};
		this->vtx.Vertex3D(CntShadow, pos);

		//NOR設定
		this->vtx.Nor3D(CntShadow);

		//UVの設定
		this->vtx.UV3D(CntShadow);

		//カラー設定
		this->vtx.Color3D(CntShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//使用設定
		this->iUseType[CntShadow].ChangeUse(NoUse);
	}

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_SHADOW);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
SHADOW::~SHADOW(void)
{
	//テクスチャ解放
	this->tex.~TEXTURE();
	//頂点解放
	this->vtx.~VTXBuffer();
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void SHADOW::Init(void)
{
	for (int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		this->Transform[CntShadow].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[CntShadow].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[CntShadow].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this->iUseType[CntShadow].Use(false);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void SHADOW::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for(int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		bool use = this->iUseType[CntShadow].Use();
		if(use)
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			//-----------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this->Transform[CntShadow].Pos();
			D3DXVECTOR3 rot = this->Transform[CntShadow].Rot();
			D3DXVECTOR3 scl = this->Transform[CntShadow].Scl();
			D3DXMATRIX mtxWorld;
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 頂点バッファをレンダリングパイプラインに設定
			pDevice->SetStreamSource(0,this->vtx.VtxBuff(), 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, this->tex.Texture());

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 影の作成
//=============================================================================
int SHADOW::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	int IdxShadow = -1;

	for(int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		bool use = this->iUseType[CntShadow].Use();
		if(use != true)
		{
			this->Transform[CntShadow].Pos(pos);
			this->Transform[CntShadow].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[CntShadow].Scl(scl);
			this->iUseType[CntShadow].Use(YesUse);

			IdxShadow = CntShadow;
			break;
		}
	}

	return IdxShadow;
}

//=============================================================================
// 影の破棄
//=============================================================================
void SHADOW::ReleaseInstance(int Idx)
{
	if(Idx >= 0 && Idx < OBJECT_SHADOW_MAX)
	{
		this->iUseType[Idx].Use(NoUse);
	}

}

//=============================================================================
// 位置の設定
//=============================================================================
void SHADOW::UpdateInstance(int Idx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	if (Idx < 0) return;	// 不正ならリターンする

	this->Transform[Idx].Pos(pos);
	this->Transform[Idx].Rot(rot);
	this->Transform[Idx].Scl(scl);
}

