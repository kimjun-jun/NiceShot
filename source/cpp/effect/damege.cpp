/**
* @file damege.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/effect/damege.h"

//=============================================================================
// 初期化処理
//=============================================================================
void DAMEGE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_DAMEGE,
			&this[CntDamege].tex2D.pD3DTexture);
	}
	D3DXVECTOR3 pos[4];
	pos[0] = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	pos[1] = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	pos[2] = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);
	pos[3] = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);

	this[0].SetPos(pos[0]);
	this[1].SetPos(pos[1]);
	this[2].SetPos(pos[2]);
	this[3].SetPos(pos[3]);



	// 頂点情報の作成
	MakeVertexDamege();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void DAMEGE::Reinit(void)
{
	D3DXVECTOR3 pos[4];
	pos[0] = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	pos[1] = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	pos[2] = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);
	pos[3] = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);
	this[0].SetPos(pos[0]);
	this[1].SetPos(pos[1]);
	this[2].SetPos(pos[2]);
	this[3].SetPos(pos[3]);
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		D3DXVECTOR3 Getpos = this[CntDamege].GetPos();
		// 頂点座標の設定
		this[CntDamege].tex2D.textureVTX[0].vtx = D3DXVECTOR3(Getpos.x - DAMEGE_SIZE_X, Getpos.y - DAMEGE_SIZE_Y, 0.0f);
		this[CntDamege].tex2D.textureVTX[1].vtx = D3DXVECTOR3(Getpos.x + DAMEGE_SIZE_X, Getpos.y - DAMEGE_SIZE_Y, 0.0f);
		this[CntDamege].tex2D.textureVTX[2].vtx = D3DXVECTOR3(Getpos.x - DAMEGE_SIZE_X, Getpos.y + DAMEGE_SIZE_Y, 0.0f);
		this[CntDamege].tex2D.textureVTX[3].vtx = D3DXVECTOR3(Getpos.x + DAMEGE_SIZE_X, Getpos.y + DAMEGE_SIZE_Y, 0.0f);
	}
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		this[CntDamege].alpha = 0;
		this[CntDamege].time = 0.0f;
		this[CntDamege].SetUse(false);
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void DAMEGE::ReinitNet(void)
{
	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(DAMEGE_POS_X, DAMEGE_POS_Y, 0.0f);
	this[0].SetPos(pos);
	D3DXVECTOR3 Getpos = this[0].GetPos();
	// 頂点座標の設定
	this[0].tex2D.textureVTX[0].vtx = D3DXVECTOR3(Getpos.x - DAMEGE_POS_X, Getpos.y - DAMEGE_POS_Y, 0.0f);
	this[0].tex2D.textureVTX[1].vtx = D3DXVECTOR3(Getpos.x + DAMEGE_POS_X, Getpos.y - DAMEGE_POS_Y, 0.0f);
	this[0].tex2D.textureVTX[2].vtx = D3DXVECTOR3(Getpos.x - DAMEGE_POS_X, Getpos.y + DAMEGE_POS_Y, 0.0f);
	this[0].tex2D.textureVTX[3].vtx = D3DXVECTOR3(Getpos.x + DAMEGE_POS_X, Getpos.y + DAMEGE_POS_Y, 0.0f);
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		this[CntDamege].alpha = 0;
		this[CntDamege].time = 0.0f;
		this[CntDamege].SetUse(false);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void DAMEGE::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void DAMEGE::Update(void)
{
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		bool use = this[CntDamege].GetUse();
		if (use==true)
		{
			this[CntDamege].time += 1.0f;
			this[CntDamege].alpha = int((this[CntDamege].time/ SCREENDAMEGE_TIME)*255.0f);

			// 反射光の設定
			this[CntDamege].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);
			this[CntDamege].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);
			this[CntDamege].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);
			this[CntDamege].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);

			if (this[CntDamege].time >= SCREENDAMEGE_TIME)
			{
				this[CntDamege].time = 0.0f;
				this[CntDamege].alpha = 0;
				this[CntDamege].SetUse(false);
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
			bool use = this[CntPlayer].GetUse();
			if (use == true)
			{
				pDevice->SetFVF(FVF_VERTEX_2D);
				pDevice->SetTexture(0, this[CntPlayer].tex2D.pD3DTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].tex2D.textureVTX, sizeof(VERTEX_2D));
			}
	}

	else
	{
		bool use = this[NetMyNumber].GetUse();//サーバーから受け取った自分番号
		if (use == true)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, this[0].tex2D.pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[0].tex2D.textureVTX, sizeof(VERTEX_2D));//ここは座標だから0
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT DAMEGE::MakeVertexDamege(void)
{
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		D3DXVECTOR3 pos = this[CntDamege].GetPos();

		// 頂点座標の設定
		this[CntDamege].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x - DAMEGE_SIZE_X, pos.y - DAMEGE_SIZE_Y, 0.0f);
		this[CntDamege].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + DAMEGE_SIZE_X, pos.y - DAMEGE_SIZE_Y, 0.0f);
		this[CntDamege].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x - DAMEGE_SIZE_X, pos.y + DAMEGE_SIZE_Y, 0.0f);
		this[CntDamege].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + DAMEGE_SIZE_X, pos.y + DAMEGE_SIZE_Y, 0.0f);
		// テクスチャのパースペクティブコレクト用
		this[CntDamege].tex2D.textureVTX[0].rhw =
			this[CntDamege].tex2D.textureVTX[1].rhw =
			this[CntDamege].tex2D.textureVTX[2].rhw =
			this[CntDamege].tex2D.textureVTX[3].rhw = 1.0f;

		// 反射光の設定
		this[CntDamege].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		this[CntDamege].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		this[CntDamege].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		this[CntDamege].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

		// テクスチャ座標の設定
		this[CntDamege].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[CntDamege].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[CntDamege].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[CntDamege].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}
	return S_OK;
}

