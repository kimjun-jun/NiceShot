/** 
* @file effect.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../h/Other/input.h"
#include "../../h/Object/Camera/camera.h"
#include "../../h/Effect/effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT			"../data/TEXTURE/effect/effect000.jpg"	// 読み込むテクスチャファイル名

//エフェクト関連定数
constexpr float	EFFECT_NORMALSET_SIZE_X{ 16.0f };			//!< エフェクト標準の幅
constexpr float	EFFECT_NORMALSET_SIZE_Y{ 16.0f };			//!< エフェクト標準の高さ


//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
EFFECT::EFFECT(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex3DBill(OBJECT_EFFECT_MAX, FVF_VERTEX_3D);

	//カウントループ
	for (int CntEffect = 0; CntEffect < OBJECT_EFFECT_MAX; CntEffect++)
	{
		//描画位置反映
		D3DXVECTOR3 pos[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(-EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2, 0.0f),
		D3DXVECTOR3(EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2, 0.0f),
		D3DXVECTOR3(-EFFECT_NORMALSET_SIZE_X / 2, -EFFECT_NORMALSET_SIZE_Y / 2, 0.0f),
		D3DXVECTOR3(EFFECT_NORMALSET_SIZE_X / 2, -EFFECT_NORMALSET_SIZE_Y / 2, 0.0f),
		};
		//描画位置反映
		//this->vtx.Vertex3D(CntEffect, EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2);
		this->vtx.Vertex3D(CntEffect, pos);

		//RHW設定
		this->vtx.Nor3D(CntEffect);

		//UVの設定
		this->vtx.UV3D(CntEffect);

		//カラー設定
		this->vtx.Color3D(CntEffect, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//使用設定
		this->iUseType[CntEffect].Use(NoUse);

		//移動量設定
		this->move[CntEffect].Move(VEC3_ALL0);
	}

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_EFFECT);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
EFFECT::~EFFECT(void)
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
void EFFECT::Init(void)
{
	//カウントループ
	for (int CntEffect = 0; CntEffect < OBJECT_EFFECT_MAX; CntEffect++)
	{
		//カラー設定
		this->vtx.Color3D(CntEffect, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//使用設定
		this->iUseType[CntEffect].Use(NoUse);

		//移動量設定
		this->move[CntEffect].Move(VEC3_ALL0);

		//トランスフォーム設定
		this->Transform[CntEffect].Pos(VEC3_ALL0);
		this->Transform[CntEffect].Scl(VEC3_ALL1);

		//パラメータ設定
		this->EffectPara[CntEffect].nTimer = 0;
		this->EffectPara[CntEffect].nDecAlpha = 0.0f;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  EFFECT::Update(void)
{
	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this->iUseType[nCntEffect].Use();
		if (use)
		{
			//--------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this->Transform[nCntEffect].Pos();
			D3DXVECTOR3 move = this->move[nCntEffect].Move();
			D3DXCOLOR col = this->vtx.GetColor3D(nCntEffect);

			pos.x += move.x;
			pos.z += move.z;

			col.a -= this->EffectPara[nCntEffect].nDecAlpha;
			if (col.a <= 0.0f)
			{
				col.a = 0.0f;
			}

			this->EffectPara[nCntEffect].nTimer--;
			if (this->EffectPara[nCntEffect].nTimer <= 0)
			{
				this->iUseType[nCntEffect].Use(NoUse);
			}

			//--------------------------------------------オブジェクト値書き込み
			this->Transform[nCntEffect].Pos(pos);
			this->vtx.Color3D(nCntEffect,col);

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  EFFECT::Draw(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//処理負荷軽減のためループ外で設定
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_3D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	pDevice->SetTexture(0, this->tex.Texture());
	// カメラビューマトリックスを取得
	CAMERA *cam = GetCamera();
	D3DXMATRIX mtxWorldEffect, mtxScale, mtxTranslate;

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		if (this->iUseType[nCntEffect].Use() == YesUseType1)
		{
			//--------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this->Transform[nCntEffect].Pos();
			D3DXVECTOR3 scl = this->Transform[nCntEffect].Scl();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldEffect);

			mtxWorldEffect._11 = cam[CntPlayer].mtxView._11;
			mtxWorldEffect._12 = cam[CntPlayer].mtxView._21;
			mtxWorldEffect._13 = cam[CntPlayer].mtxView._31;
			mtxWorldEffect._21 = cam[CntPlayer].mtxView._12;
			mtxWorldEffect._22 = cam[CntPlayer].mtxView._22;
			mtxWorldEffect._23 = cam[CntPlayer].mtxView._32;
			mtxWorldEffect._31 = cam[CntPlayer].mtxView._13;
			mtxWorldEffect._32 = cam[CntPlayer].mtxView._23;
			mtxWorldEffect._33 = cam[CntPlayer].mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldEffect);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEffect * 4), POLYGON_2D_NUM);

		}
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// インスタンスセット
//=============================================================================
int EFFECT::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		 
		if (this->iUseType[nCntEffect].Use() == NoUse)
		{
			this->Transform[nCntEffect].Pos(pos);
			this->Transform[nCntEffect].Rot(VEC3_ALL0);
			this->Transform[nCntEffect].Scl(VEC3_ALL1);
			this->move[nCntEffect].Move(move);
			this->EffectPara[nCntEffect].nTimer = nTimer;
			this->EffectPara[nCntEffect].nDecAlpha = float(col.a / nTimer);
			this->iUseType[nCntEffect].Use(YesUseType1);

			//描画位置反映
			D3DXVECTOR3 pos[POLYGON_2D_VERTEX] =
			{
			D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f),
			D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f),
			D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f),
			D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f),
			};
			// 頂点座標の設定
			this->vtx.Vertex3D(nCntEffect, pos);

			// 頂点カラーの設定
			this->vtx.Color3D(nCntEffect, col);

			nIdxEffect = nCntEffect;

			break;
		}
	}
	return nIdxEffect;
}
