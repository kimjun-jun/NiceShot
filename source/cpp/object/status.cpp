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
			TEXTURE_STATUS_SPEED_ITEM,
			&this[CntPlayer].obj[STATUSTYPE_SPEED].tex2D.pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_SENSYA_ITEM,
			&this[CntPlayer].obj[STATUSTYPE_SENSYA].tex2D.pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_CAMERA_ITEM,
			&this[CntPlayer].obj[STATUSTYPE_CAMERA].tex2D.pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_KIRI_ITEM,
			&this[CntPlayer].obj[STATUSTYPE_KIRI].tex2D.pD3DTexture);
	}
	this[0].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[0].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[0].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[0].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));

	this[1].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));

	this[2].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	this[3].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	// 頂点情報の作成
	MakeVertexStatus();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::Reinit(void)
{
	this[0].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[0].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[0].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[0].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));

	this[1].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[1].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));

	this[2].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[2].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	this[3].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[3].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	for (int CntPlayer = 0; CntPlayer < OBJECT_STATUS_MAX; CntPlayer++)
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			//-----------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[CntPlayer].obj[CntStatus].GetPos();

			// 頂点座標の設定
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);

			// 反射光の設定
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::ReinitNet(int MyNumber)
{

	float buffsize = 48.0f;
	this[MyNumber].obj[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));
	this[MyNumber].obj[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));
	this[MyNumber].obj[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));
	this[MyNumber].obj[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET * 2, 0.0f));

	for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
	{
		//-----------------------------------オブジェクト値読み込み
		D3DXVECTOR3 pos = this[MyNumber].obj[CntStatus].GetPos();

		// 頂点座標の設定
		this[MyNumber].obj[CntStatus].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X*2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
		this[MyNumber].obj[CntStatus].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
		this[MyNumber].obj[CntStatus].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
		this[MyNumber].obj[CntStatus].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*STATUS_SIZE_X * 2 * 2.0f), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
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
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		//スピード
		if (p->speedbuffsignal == true)
		{
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_SPEED].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//戦車
		if (p->ModelType == PLAYER_MODEL_ATTACK)
		{
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_SENSYA].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//カメラ
		if (p->BackCameraItemSignal == true)
		{
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_CAMERA].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//霧
		if (p->KiriSignal == true)
		{
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntStatus].obj[STATUSTYPE_KIRI].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void STATUS::Draw(bool Netflag, int NetMyNumber)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (Netflag==false)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_STATUS_MAX; CntPlayer++)
		{
			for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
			{
				pDevice->SetFVF(FVF_VERTEX_2D);
				pDevice->SetTexture(0, this[CntPlayer].obj[CntStatus].tex2D.pD3DTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].obj[CntStatus].tex2D.textureVTX, sizeof(VERTEX_2D));
			}
		}
	}
	else
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, this[0].obj[CntStatus].tex2D.pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[NetMyNumber].obj[CntStatus].tex2D.textureVTX, sizeof(VERTEX_2D));
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
			//-----------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[CntPlayer].obj[CntStatus].GetPos();

			// 頂点座標の設定
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
			// テクスチャのパースペクティブコレクト用
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[0].rhw =
				this[CntPlayer].obj[CntStatus].tex2D.textureVTX[1].rhw =
				this[CntPlayer].obj[CntStatus].tex2D.textureVTX[2].rhw =
				this[CntPlayer].obj[CntStatus].tex2D.textureVTX[3].rhw = 1.0f;

			// 反射光の設定
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

			// テクスチャ座標の設定
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			this[CntPlayer].obj[CntStatus].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	return S_OK;
}
