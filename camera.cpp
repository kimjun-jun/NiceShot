//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_W / (float)SCREEN_H)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.005f)							// カメラの回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA			g_Camera[PLAYER_MAX];				// カメラデータ

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	for (int CntCam = 0; CntCam < PLAYER_MAX; CntCam++)
	{
		g_Camera[CntCam].pos = D3DXVECTOR3(0.0f, POS_H_CAM, -POS_W_CAM);
		g_Camera[CntCam].at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera[CntCam].up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera[CntCam].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 視点と注視点の距離を計算
		float vx, vz;
		vx = g_Camera[CntCam].pos.x - g_Camera[CntCam].at.x;
		vz = g_Camera[CntCam].pos.z - g_Camera[CntCam].at.z;
		g_Camera[CntCam].len = sqrtf(vx * vx + vz * vz);

	}
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{

}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(int CamNum) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ビューマトリックスの初期化
		D3DXMatrixIdentity(&g_Camera[CamNum].mtxView);

		// ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_Camera[CamNum].mtxView,
			&g_Camera[CamNum].pos,		// カメラの視点(位置)
			&g_Camera[CamNum].at,		// カメラの注視点
			&g_Camera[CamNum].up);		// カメラの上方向ベクトル

		// ビューマトリックスの設定
		pDevice->SetTransform(D3DTS_VIEW, &g_Camera[CamNum].mtxView);

		// プロジェクションマトリックスの初期化
		D3DXMatrixIdentity(&g_Camera[CamNum].mtxProjection);

		// プロジェクションマトリックスの作成
		D3DXMatrixPerspectiveFovLH(&g_Camera[CamNum].mtxProjection,
			VIEW_ANGLE,			// ビュー平面の視野角
			VIEW_ASPECT,		// ビュー平面のアスペクト比
			VIEW_NEAR_Z,		// ビュー平面のNearZ値
			VIEW_FAR_Z);		// ビュー平面のFarZ値

		// プロジェクションマトリックスの設定(透視変換の設定)
		pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera[CamNum].mtxProjection);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera[0];
}
