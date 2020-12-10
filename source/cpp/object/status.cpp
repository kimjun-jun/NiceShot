/**
* @file this.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../h/object/status.h"

//=============================================================================
// 初期化処理
//=============================================================================
void STATUS::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_EMPTY_ITEM,
			&this[CntPlayer].TexEmpty[0].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_SPEED_ITEM,
			&this[CntPlayer].Tex[STATUSTYPE_SPEED].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_SENSYA_ITEM,
			&this[CntPlayer].Tex[STATUSTYPE_SENSYA].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_CAMERA_ITEM,
			&this[CntPlayer].Tex[STATUSTYPE_CAMERA].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_KIRI_ITEM,
			&this[CntPlayer].Tex[STATUSTYPE_KIRI].pD3DTexture);

		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			this[CntPlayer].use[CntStatus] = false;
			this[CntPlayer].usebuff[CntStatus] = -1;
		}
	}

	this[0].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	// 頂点情報の作成
	MakeVertexStatus();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::Reinit(void)
{
	this[0].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	// 頂点情報の作成
	MakeVertexStatus();

	for (int CntPlayer = 0; CntPlayer < OBJECT_STATUS_MAX; CntPlayer++)
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			this[CntPlayer].use[CntStatus] = false;
			this[CntPlayer].usebuff[CntStatus] = -1;
		}
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::ReinitNet(int MyNumber)
{

	float buffsize = 48.0f;
	this[MyNumber].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));
	this[MyNumber].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));
	this[MyNumber].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));
	this[MyNumber].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));

	for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
	{
		//-----------------------------------オブジェクト値読み込み
		D3DXVECTOR3 pos = this[MyNumber].GetPos();

		// 頂点座標の設定
		this[MyNumber].Tex[CntStatus].textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X*2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
		this[MyNumber].Tex[CntStatus].textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
		this[MyNumber].Tex[CntStatus].textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
		this[MyNumber].Tex[CntStatus].textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void STATUS::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void STATUS::Update(PLAYER_HONTAI *p)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//スピード
		if (p[CntPlayer].speedbuffsignal == true) this[CntPlayer].use[STATUSTYPE_SPEED] = true;
		else this[CntPlayer].use[STATUSTYPE_SPEED] = false;
		//戦車
		if (p[CntPlayer].ModelType == PLAYER_MODEL_ATTACK) this[CntPlayer].use[STATUSTYPE_SENSYA] = true;
		else this[CntPlayer].use[STATUSTYPE_SENSYA] = false;
		//カメラ
		if (p[CntPlayer].BackCameraItemSignal == true) this[CntPlayer].use[STATUSTYPE_CAMERA] = true;
		else this[CntPlayer].use[STATUSTYPE_CAMERA] = false;
		//霧
		if (p[CntPlayer].KiriSignal == true) this[CntPlayer].use[STATUSTYPE_KIRI] = true;
		else this[CntPlayer].use[STATUSTYPE_KIRI] = false;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void STATUS::Draw(bool Netflag, int NetMyNumber,int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (Netflag == false)
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			//Base
			pDevice->SetTexture(0, this[CntPlayer].TexEmpty[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].TexEmpty[CntStatus].textureVTX, sizeof(VERTEX_2D));
			//中身
			if (this[CntPlayer].use[CntStatus] == true)
			{
				pDevice->SetTexture(0, this[CntPlayer].Tex[CntStatus].pD3DTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].Tex[CntStatus].textureVTX, sizeof(VERTEX_2D));
			}
		}
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			//Base
			pDevice->SetTexture(0, this[NetMyNumber].TexEmpty[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[NetMyNumber].TexEmpty[CntStatus].textureVTX, sizeof(VERTEX_2D));
			//中身
			if (this[NetMyNumber].use[CntStatus] == true)
			{
				pDevice->SetTexture(0, this[NetMyNumber].Tex[CntStatus].pD3DTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[NetMyNumber].Tex[CntStatus].textureVTX, sizeof(VERTEX_2D));
			}
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT STATUS::MakeVertexStatus(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_STATUS_MAX; CntPlayer++)
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			D3DXVECTOR3 pos = this[CntPlayer].GetPos();
			//Base
			// 頂点座標の設定
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
			// テクスチャのパースペクティブコレクト用
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[0].rhw =
				this[CntPlayer].TexEmpty[CntStatus].textureVTX[1].rhw =
				this[CntPlayer].TexEmpty[CntStatus].textureVTX[2].rhw =
				this[CntPlayer].TexEmpty[CntStatus].textureVTX[3].rhw = 1.0f;

			// 反射光の設定
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ座標の設定
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			this[CntPlayer].TexEmpty[CntStatus].textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//中身
			// 頂点座標の設定
			this[CntPlayer].Tex[CntStatus].textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].Tex[CntStatus].textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].Tex[CntStatus].textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].Tex[CntStatus].textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
			// テクスチャのパースペクティブコレクト用
			this[CntPlayer].Tex[CntStatus].textureVTX[0].rhw =
				this[CntPlayer].Tex[CntStatus].textureVTX[1].rhw =
				this[CntPlayer].Tex[CntStatus].textureVTX[2].rhw =
				this[CntPlayer].Tex[CntStatus].textureVTX[3].rhw = 1.0f;

			// 反射光の設定
			this[CntPlayer].Tex[CntStatus].textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntPlayer].Tex[CntStatus].textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntPlayer].Tex[CntStatus].textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntPlayer].Tex[CntStatus].textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ座標の設定
			this[CntPlayer].Tex[CntStatus].textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			this[CntPlayer].Tex[CntStatus].textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			this[CntPlayer].Tex[CntStatus].textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			this[CntPlayer].Tex[CntStatus].textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	return S_OK;
}
