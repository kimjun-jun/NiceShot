/**
* @file explosion.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/effect/explosion.h"

//*****************************************************************************
// マクロ定義 nPatternX
//*****************************************************************************
#define	TEXTURE_EXPLOSION	"../data/TEXTURE/effect/bomb.png"		//!< 読み込むテクスチャファイル名
#define	TEXTURE_PATTERN_X_UV_EXPLOSION		0.25f					//!< テクスチャスプライトX
#define	TEXTURE_PATTERN_Y_UV_EXPLOSION		0.25f					//!< テクスチャスプライトY
#define	TEXTURE_PATTERN_X_COUNT_EXPLOSION	4						//!< テクスチャスプライトX
#define	TEXTURE_PATTERN_Y_COUNT_EXPLOSION	4						//!< テクスチャスプライトY
#define	TEXTURE_PATTERN_COUNTER_EXPLOSION	4						//!< テクスチャスプライトのカウントタイマー
#define	EXPLOSION_VERTEX_SIZE				(5.0f)					//!< EXPLOSION頂点サイズ
#define	EXPLOSION_VERTEX_ADD_SIZE			(0.5f)					//!< EXPLOSION頂点サイズ


//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
EXPLOSION::EXPLOSION(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_EXPLOSION_MAX, FVF_VERTEX_2D);

	//カウントループ
	for (int CntExplosion = 0; CntExplosion < OBJECT_EXPLOSION_MAX; CntExplosion++)
	{
		//描画位置反映
		this->vtx.Vertex2D(CntExplosion, EXPLOSION_VERTEX_SIZE / 2, EXPLOSION_VERTEX_SIZE / 2);

		//RHW設定
		this->vtx.RHW2D(CntExplosion);

		//UVの設定
		this->vtx.UV2D(CntExplosion);

		//カラー設定
		this->vtx.Color2D(CntExplosion, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//使用設定
		this->iUseType[CntExplosion].ChangeUse(NoUse);
	}

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_EXPLOSION);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
EXPLOSION::~EXPLOSION(void)
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
void EXPLOSION::Init(void)
{
	//カウントループ
	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		//初期化設定
		this->Transform[nCntExplosion].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[nCntExplosion].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[nCntExplosion].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this->vtx.Color2D(nCntExplosion);
		this->vtx.UV2D(nCntExplosion);
		this->ExploPara[nCntExplosion].nCounter = 0;
		this->ExploPara[nCntExplosion].nPatternX = 0;
		this->ExploPara[nCntExplosion].nPatternY = 0;
		this->ExploPara[nCntExplosion].eType = EXPLOSIONTYPE_BULLET_NONE;;
		this->ExploPara[nCntExplosion].fSizeX = 0.0f;
		this->ExploPara[nCntExplosion].fSizeY = 0.0f;
		this->iUseType[nCntExplosion].Use(NoUse);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void EXPLOSION::Update(void)
{
	//使用中の場合　スプライト再生後　未使用状態
	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this->iUseType[nCntExplosion].Use();
		if(use)
		{
			//スプライト再生　カウントしてUVテクスチャを切り替える
			this->ExploPara[nCntExplosion].nCounter--;
			if((this->ExploPara[nCntExplosion].nCounter % TEXTURE_PATTERN_COUNTER_EXPLOSION) == 0)
			{
				this->ExploPara[nCntExplosion].nPatternX++;
				if(this->ExploPara[nCntExplosion].nPatternY >= TEXTURE_PATTERN_Y_COUNT_EXPLOSION)
				{
					this->iUseType[nCntExplosion].Use(false);
				}
				else if (this->ExploPara[nCntExplosion].nPatternX >= TEXTURE_PATTERN_X_COUNT_EXPLOSION)
				{
					this->ExploPara[nCntExplosion].nPatternX = 0;
					this->ExploPara[nCntExplosion].nPatternY++;
				}
				else
				{
					// テクスチャ座標の設定
					this->vtx.UV2D(nCntExplosion,
						float(this->ExploPara[nCntExplosion].nPatternX*TEXTURE_PATTERN_X_UV_EXPLOSION),
						float(this->ExploPara[nCntExplosion].nPatternY*TEXTURE_PATTERN_Y_UV_EXPLOSION));
				}
			}

			// 頂点サイズの設定
			this->ExploPara[nCntExplosion].fSizeX += EXPLOSION_VERTEX_ADD_SIZE;
			this->ExploPara[nCntExplosion].fSizeY += EXPLOSION_VERTEX_ADD_SIZE;
			this->vtx.Vertex2D(nCntExplosion,
				this->ExploPara[nCntExplosion].fSizeX, 
				this->ExploPara[nCntExplosion].fSizeY);

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void EXPLOSION::Draw(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this->iUseType[nCntExplosion].Use();
		if (use)
		{
			//--------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this->Transform[nCntExplosion].Pos();
			D3DXVECTOR3 scl = this->Transform[nCntExplosion].Scl();
			D3DXCOLOR col = this->vtx.GetColor2D(nCntExplosion);
			D3DXMATRIX mtxWorldExplosion;

				D3DXMATRIX mtxView, mtxScale, mtxTranslate;

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorldExplosion);

				// ビューマトリックスを取得
				CAMERA *cam = GetCamera();

				mtxWorldExplosion._11 = cam[CntPlayer].mtxView._11;
				mtxWorldExplosion._12 = cam[CntPlayer].mtxView._21;
				mtxWorldExplosion._13 = cam[CntPlayer].mtxView._31;
				mtxWorldExplosion._21 = cam[CntPlayer].mtxView._12;
				mtxWorldExplosion._22 = cam[CntPlayer].mtxView._22;
				mtxWorldExplosion._23 = cam[CntPlayer].mtxView._32;
				mtxWorldExplosion._31 = cam[CntPlayer].mtxView._13;
				mtxWorldExplosion._32 = cam[CntPlayer].mtxView._23;
				mtxWorldExplosion._33 = cam[CntPlayer].mtxView._33;

				// スケールを反映
				D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxScale);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldExplosion);

				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// テクスチャの設定
				pDevice->SetTexture(0, this->tex.Texture());

				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntExplosion * 4), POLYGON_2D_NUM);

				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// 
//=============================================================================
int EXPLOSION::SetInstance(D3DXVECTOR3 pos, float fSizeX, float fSizeY, eEXPLOSION_TYPE eType, D3DXCOLOR col)
{
	int nIdxExplosion = -1;

	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this->iUseType[nCntExplosion].Use();
		if (use!=true)
		{
			this->Transform[nCntExplosion].Pos(pos);
			this->Transform[nCntExplosion].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[nCntExplosion].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this->vtx.Color2D(nCntExplosion,col);
			this->iUseType[nCntExplosion].Use(YesUse);
			this->ExploPara[nCntExplosion].fSizeX = fSizeX;
			this->ExploPara[nCntExplosion].fSizeY = fSizeY;
			this->ExploPara[nCntExplosion].nCounter = 0;
			this->ExploPara[nCntExplosion].nPatternX = 0;
			this->ExploPara[nCntExplosion].nPatternY = 0;
			this->ExploPara[nCntExplosion].eType = eType;

			// 頂点座標の設定
			this->vtx.Vertex2D(nCntExplosion, fSizeX, fSizeY);

			// テクスチャ座標の設定
			this->vtx.UV2D(nCntExplosion, 0,0);

			nIdxExplosion = nCntExplosion;

			break;
		}
	}

	return nIdxExplosion;
}
