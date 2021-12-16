/**
* @file camera.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../../h/main.h"
#include "../../../h/Debug/debugproc.h"
#include "../../../h/Other/input.h"
#include "../../../h/Object/Player/player.h"
#include "../../../h/Object/Camera/camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float	VIEW_ANGLE{ D3DXToRadian(45.0f) };							//!< ビュー平面の視野角
constexpr float	VIEW_NEAR_Z{ 10.0f };										//!< ビュー平面のNearZ値
constexpr float	VIEW_FAR_Z{ 10000.0f };										//!< ビュー平面のFarZ値
constexpr float	VALUE_MOVE_CAMERA{ 2.0f };									//!< カメラの移動量
constexpr float	VALUE_ROTATE_CAMERA{ D3DX_PI * 0.005f };					//!< カメラの回転量
constexpr float	VIEW_ASPECT{ (float)SCREEN_W / (float)SCREEN_H };			//!< ビュー平面のアスペクト比	
constexpr float BACKCAMERA_TIME{ 150.0f };									//!< バックカメラアイテム有効時間

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

//=============================================================================
// カメラ反転制御(手動とアイテムの両方で制御)
//=============================================================================
void CameraRevers(PLAYER *Player, int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	CAMERA *cam = GetCamera();

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//バックカメラ処理　バックカメラオン　カメラ視点、注視点、Yボタンを押しているもしくは、バックカメラアイテムがONになっているときはカメラ反転
	if (GetKeyboardPress(DIK_B) || IsButtonPressed(PadNum, BUTTON_Y) || Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraItemSignal() == true)
	{
		cam[CntPlayer].at.x = pos.x + (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z + (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x - sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z - cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}

	//バックカメラオフ　カメラ視点、注視点
	//それ以外は通常カメラ
	else
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//もし、バックカメラアイテムがONの時にYを押すと通常カメラになる
	if (Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraItemSignal() == true && IsButtonPressed(PadNum, BUTTON_Y))
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//バックカメラの時間処理
	if (Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraItemSignal() == true)
	{
		Player->PlayerPara[CntPlayer].ItemPara.SetBackCameraTime(Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraTime() + 1.0f);
		if (Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraTime() >= BACKCAMERA_TIME)
		{
			Player->PlayerPara[CntPlayer].ItemPara.SetBackCameraTime(0.0f);
			Player->PlayerPara[CntPlayer].ItemPara.SetBackCameraItemSignal(false);
		}
	}
}

//=============================================================================
// カメラ制御(RスティックでRot制御)
//=============================================================================
void CameraRotControl(PLAYER *Player, int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old保存
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//視野角変化はRスティックアナログ値を使用
	float RAnalogX = 0.0f;		//縦入力
	float RAnalogY = 0.0f;		//横入力

	//視野角処理
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_R_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_R_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		//入力中央値32000　最小0　最大64000
		//なので-32000することで　中央値0　最小-32000　最大32000にしている
		//rotに32000とかバカ高い数値を入れるとぶっ飛ぶので、さらに入力値を小さくする
		//最大0.03くらいになるよう調整　/1000000する(操作しやすい値でいい)
		RAnalogX = float(Button->lRx) - 32000.0f;
		RAnalogY = float(Button->lRy) - 32800.0f;

		RAnalogX = RAnalogX / 1000000.0f;
		RAnalogY = RAnalogY / 1000000.0f;

	}

	//回転量を反映
	D3DXVECTOR3 moverot = VEC3_ALL0;
	moverot.y = RAnalogX;
	moverot.x = -RAnalogY;

	HoutouRot.y += moverot.y;
	HousinRot.x += moverot.x;

	//角度の制限値
	{
		if (HousinRot.x >= 0.3f)
		{
			HousinRot.x = 0.3f;
		}
		else if (HousinRot.x <= -0.3f)
		{
			HousinRot.x = -0.3f;
		}
	}

	//---------------------------------------------------------オブジェクト値セット
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);
}

