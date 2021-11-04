/**
* @file camera.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../h/other/debugproc.h"
#include "../../h/other/input.h"
#include "../../h/object/player.h"
#include "../../h/object/camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float	VIEW_ANGLE{ D3DXToRadian(45.0f) };							// ビュー平面の視野角
constexpr float	VIEW_NEAR_Z{ 10.0f };										// ビュー平面のNearZ値
constexpr float	VIEW_FAR_Z{ 10000.0f };										// ビュー平面のFarZ値
constexpr float	VALUE_MOVE_CAMERA{ 2.0f };									// カメラの移動量
constexpr float	VALUE_ROTATE_CAMERA{ D3DX_PI * 0.005f };					// カメラの回転量
constexpr float	VIEW_ASPECT{ (float)SCREEN_W / (float)SCREEN_H };			// ビュー平面のアスペクト比	

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA			g_Camera[OBJECT_CAMERA_MAX];				// カメラデータ

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	for (int CntCam = 0; CntCam < OBJECT_CAMERA_MAX; CntCam++)
	{
		g_Camera[CntCam].pos = D3DXVECTOR3(0.0f, POS_H_CAM, -POS_W_CAM);
		g_Camera[CntCam].at = VEC3_ALL0;
		g_Camera[CntCam].up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera[CntCam].rot = VEC3_ALL0;
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
