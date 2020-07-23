/**
* @file status.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "main.h"
#include "player.h"
#include "status.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexStatus(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
STATUS status[PLAYER_MAX][STATUSTYPE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStatus(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_SPEED_ITEM,
			&status[0][STATUSTYPE_SPEED].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_SENSYA_ITEM,
			&status[0][STATUSTYPE_SENSYA].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_CAMERA_ITEM,
			&status[0][STATUSTYPE_CAMERA].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_STATUS_KIRI_ITEM,
			&status[0][STATUSTYPE_KIRI].pD3DTexture);
	}

	for (int CntStatus = 0; CntStatus < PLAYER_MAX; CntStatus++)
	{

		status[0][CntStatus].pos = D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f);
		status[1][CntStatus].pos = D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f);
		status[2][CntStatus].pos = D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f);
		status[3][CntStatus].pos = D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f);
	}

	// 頂点情報の作成
	MakeVertexStatus();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStatus(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			if (status[CntPlayer][CntStatus].pD3DTexture != NULL)
			{// テクスチャの開放
				status[CntPlayer][CntStatus].pD3DTexture->Release();
				status[CntPlayer][CntStatus].pD3DTexture = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStatus(void)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//スピード
		if (p[CntPlayer].speedbuffsignal == true)
		{
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_SPEED].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//戦車
		if (p[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
		{
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_SENSYA].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//カメラ
		if (p[CntPlayer].BackCameraItemSignal == true)
		{
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_CAMERA].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		//霧
		if (p[CntPlayer].KiriSignal == true)
		{
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][STATUSTYPE_KIRI].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStatus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
				pDevice->SetFVF(FVF_VERTEX_2D);
				pDevice->SetTexture(0, status[0][CntStatus].pD3DTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, status[CntPlayer][CntStatus].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexStatus(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			// 頂点座標の設定
			status[CntPlayer][CntStatus].vertexWk[0].vtx = D3DXVECTOR3(status[CntPlayer][CntStatus].pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X*2), status[CntPlayer][CntStatus].pos.y - STATUS_SIZE_Y, 0.0f);
			status[CntPlayer][CntStatus].vertexWk[1].vtx = D3DXVECTOR3(status[CntPlayer][CntStatus].pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X*2), status[CntPlayer][CntStatus].pos.y - STATUS_SIZE_Y, 0.0f);
			status[CntPlayer][CntStatus].vertexWk[2].vtx = D3DXVECTOR3(status[CntPlayer][CntStatus].pos.x - STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X*2), status[CntPlayer][CntStatus].pos.y + STATUS_SIZE_Y, 0.0f);
			status[CntPlayer][CntStatus].vertexWk[3].vtx = D3DXVECTOR3(status[CntPlayer][CntStatus].pos.x + STATUS_SIZE_X + (CntStatus*STATUS_SIZE_X*2), status[CntPlayer][CntStatus].pos.y + STATUS_SIZE_Y, 0.0f);
			// テクスチャのパースペクティブコレクト用
			status[CntPlayer][CntStatus].vertexWk[0].rhw =
				status[CntPlayer][CntStatus].vertexWk[1].rhw =
				status[CntPlayer][CntStatus].vertexWk[2].rhw =
				status[CntPlayer][CntStatus].vertexWk[3].rhw = 1.0f;

			// 反射光の設定
			status[CntPlayer][CntStatus].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][CntStatus].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][CntStatus].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);
			status[CntPlayer][CntStatus].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 100);

			// テクスチャ座標の設定
			status[CntPlayer][CntStatus].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			status[CntPlayer][CntStatus].vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			status[CntPlayer][CntStatus].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			status[CntPlayer][CntStatus].vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	return S_OK;
}
