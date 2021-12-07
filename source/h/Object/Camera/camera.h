/**
* @file camera.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

class PLAYER;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float	POS_H_CAM		{ 80.0f };		// カメラの位置高さ(プレイヤーからみて)
constexpr float	POS_W_CAM		{ -150.0f };	// カメラの位置横幅(プレイヤーからみて)
constexpr float	AT_H_CAM		{ 0.0f };		// カメラの注視点高さ(プレイヤーからみて)
constexpr float	AT_W_CAM		{ 300.0f };		// カメラの注視点横幅(プレイヤーからみて)

struct CAMERA
{
	D3DXVECTOR3			pos;			// カメラの視点(位置)
	D3DXVECTOR3			at;				// カメラの注視点
	D3DXVECTOR3			up;				// カメラの上方向ベクトル
	D3DXVECTOR3			rot;			// カメラの回転
	float				len;			// カメラの視点と注視点の距離
	D3DXMATRIX			mtxView;		// ビューマトリックス
	D3DXMATRIX			mtxProjection;	// プロジェクションマトリックス

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int CamNum);
CAMERA *GetCamera();

//------カメラ制御
void CameraRevers(PLAYER *Player, int CntPlayer, bool Netflag);		//!< カメラ制御
void CameraRotControl(PLAYER *Player, int CntPlayer, bool Netflag);	//!< カメラ制御(LRスティックで移動制御)
