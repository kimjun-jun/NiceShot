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

	LPDIRECT3DTEXTURE9 pD3DTexture[4];
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_STATUS_SPEED_ITEM,
		&pD3DTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_STATUS_SENSYA_ITEM,
		&pD3DTexture[1]);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_STATUS_CAMERA_ITEM,
		&pD3DTexture[2]);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_STATUS_KIRI_ITEM,
		&pD3DTexture[3]);

	//---------------------------------------オブジェクト値書き込み
	this[STATUSTYPE_SPEED].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[STATUSTYPE_SENSYA].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[STATUSTYPE_CAMERA].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[STATUSTYPE_KIRI].tex2D.SetpD3DTexture(pD3DTexture[0]);

	this[STATUSTYPE_SPEED].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[STATUSTYPE_SENSYA].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this[STATUSTYPE_CAMERA].SetPos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this[STATUSTYPE_KIRI].SetPos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	// 頂点情報の作成
	MakeVertexStatus();
}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::Reinit(void)
{
	VERTEX_2D vtx[POLYGON_2D_VERTEX];
	vtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
	vtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
	vtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
	vtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
	this[STATUSTYPE_SPEED].tex2D.SettextureVTX(vtx);
	this[STATUSTYPE_SENSYA].tex2D.SettextureVTX(vtx);
	this[STATUSTYPE_CAMERA].tex2D.SettextureVTX(vtx);
	this[STATUSTYPE_KIRI].tex2D.SettextureVTX(vtx);
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
	for (int CntPlayer = 0; CntPlayer < OBJECT_STATUS_MAX; CntPlayer++)
	{
		//スピード
		if (p->speedbuffsignal == true)
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_SPEED].tex2D.GettextureVTX();

			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_SPEED].tex2D.GettextureVTX();

			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//戦車
		if (p->ModelType == PLAYER_MODEL_ATTACK)
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_SENSYA].tex2D.GettextureVTX();
			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_SENSYA].tex2D.GettextureVTX();
			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//カメラ
		if (p->BackCameraItemSignal == true)
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_CAMERA].tex2D.GettextureVTX();
			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_CAMERA].tex2D.GettextureVTX();
			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//霧
		if (p->KiriSignal == true)
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_KIRI].tex2D.GettextureVTX();
			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			VERTEX_2D *vtx2d;
			vtx2d = this[STATUSTYPE_KIRI].tex2D.GettextureVTX();
			vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void STATUS::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < OBJECT_STATUS_MAX; CntPlayer++)
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
				pDevice->SetFVF(FVF_VERTEX_2D);
				pDevice->SetTexture(0, LPDIRECT3DTEXTURE9(this[CntStatus].tex2D.GetpD3DTexture()));
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntStatus].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT STATUS::MakeVertexStatus(void)
{
	for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
	{
		//-----------------------------------オブジェクト値読み込み
		D3DXVECTOR3 pos = this[CntStatus].GetPos();

		// 頂点座標の設定
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		vtx2d[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
		vtx2d[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y - STATUS_SIZE_Y, 0.0f);
		vtx2d[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
		vtx2d[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X * 2), pos.y + STATUS_SIZE_Y, 0.0f);
		// テクスチャのパースペクティブコレクト用
		vtx2d[0].rhw =
			vtx2d[1].rhw =
			vtx2d[2].rhw =
			vtx2d[3].rhw = 1.0f;

		// 反射光の設定
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

		// テクスチャ座標の設定
		vtx2d[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//-----------------------------------オブジェクト値書き込み
		this[CntStatus].tex2D.SettextureVTX(vtx2d);
	}
	return S_OK;
}
