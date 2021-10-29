/**
* @file damege.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../h/effect/damege.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_DAMEGE			("../data/TEXTURE/effect/screendamage.png")			
#define	DAMEGE_SIZE_X			(SCREEN_W/4)										// チュートリアルの幅
#define	DAMEGE_SIZE_Y			(SCREEN_H/4)										// チュートリアルの高さ
#define	DAMEGE_POS_X			(SCREEN_CENTER_X)									// チュートリアルの表示位置
#define	DAMEGE_POS_Y			(SCREEN_CENTER_Y)									// チュートリアルの表示位置

#define SCREENDAMEGE_TIME		(20.0f)									//!< 被ダメージ時の画面フェード時間

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
DAMEGE::DAMEGE(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_DAMEGE_MAX, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f));

	//カウントループ
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		//描画位置反映
		D3DXVECTOR3 pos = this->Transform[CntDamege].Pos();
		this->vtx.Vertex2D(CntDamege, DAMEGE_SIZE_X, DAMEGE_SIZE_Y, pos);

		//RHW設定
		this->vtx.RHW2D(CntDamege);

		//UVの設定
		this->vtx.UV2D(CntDamege);

		//カラー設定
		this->vtx.Color2D(CntDamege, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//使用設定
		this->iUseType[CntDamege].Use(NoUse);
	}

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_DAMEGE);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
DAMEGE::~DAMEGE(void)
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
void DAMEGE::Init(void)
{
	//カウントループ
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		//カラー設定
		this->vtx.Color2D(CntDamege, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//使用設定
		this->iUseType[CntDamege].Use(NoUse);

		//パラメータ設定
		this->DamegePara[CntDamege].time = 0.0f;
		this->DamegePara[CntDamege].alpha = 0.0f;

	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void DAMEGE::InitNet(int MyNumber)
{
	/*
	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(DAMEGE_POS_X, DAMEGE_POS_Y, 0.0f);
	this[0].Pos(pos);
	D3DXVECTOR3 Getpos = this[0].Pos();
	// 頂点座標の設定
	this[0].tex2D.textureVTX[0].vtx = D3DXVECTOR3(Getpos.x - DAMEGE_POS_X, Getpos.y - DAMEGE_POS_Y, 0.0f);
	this[0].tex2D.textureVTX[1].vtx = D3DXVECTOR3(Getpos.x + DAMEGE_POS_X, Getpos.y - DAMEGE_POS_Y, 0.0f);
	this[0].tex2D.textureVTX[2].vtx = D3DXVECTOR3(Getpos.x - DAMEGE_POS_X, Getpos.y + DAMEGE_POS_Y, 0.0f);
	this[0].tex2D.textureVTX[3].vtx = D3DXVECTOR3(Getpos.x + DAMEGE_POS_X, Getpos.y + DAMEGE_POS_Y, 0.0f);
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		this[CntDamege].alpha = 0;
		this[CntDamege].time = 0.0f;
		this[CntDamege].Use(false);
	}
	*/
}

//=============================================================================
// 更新処理
//=============================================================================
void DAMEGE::Update(void)
{
	//カウントループ
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		//使用中なら徐々にαを高くする
		if (this->iUseType[CntDamege].Use() ==YesUseType1)
		{
			this->DamegePara[CntDamege].time += 1.0f;
			this->DamegePara[CntDamege].alpha = this->DamegePara[CntDamege].time / SCREENDAMEGE_TIME;

			// 反射光の設定
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, this->DamegePara[CntDamege].alpha);
			this->vtx.Color2D(CntDamege, col);

			//一定時間経過で終了
			if (this->DamegePara[CntDamege].time >= SCREENDAMEGE_TIME)
			{
				this->DamegePara[CntDamege].time = 0;
				this->DamegePara[CntDamege].alpha = 0.0f;
				this->iUseType[CntDamege].Use(NoUse);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DAMEGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

	if (Netflag==false)
	{
		//描画判定　
		if (this->iUseType[CntPlayer].Use() == YesUseType1)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex.Texture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntPlayer * 4), POLYGON_2D_NUM);
		}
	}

	else
	{
		//描画判定　
		if (this->iUseType[NetMyNumber].Use() == YesUseType1)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex.Texture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NetMyNumber * 4), POLYGON_2D_NUM);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり
}

