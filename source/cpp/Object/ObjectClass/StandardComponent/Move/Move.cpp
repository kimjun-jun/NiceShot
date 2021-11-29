/**
* @file Move.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../../../../h/main.h"
#include "../../../../../h/Other/input.h"
#include "../../../../../h/Effect/effect.h"
#include "../../../../../h/Object/Player/player.h"
#include "../../../../../h/Object/ObjectClass/StandardComponent/Move/Move.h"

constexpr float	EFFECT_SPEEDUP_SIZE_X{ 25.0f };			//!< エフェクトスピードアップの幅
constexpr float	EFFECT_SPEEDUP_SIZE_Y{ 10.0f };			//!< エフェクトスピードアップの高さ
constexpr int	EFFECT_SPEEDUP_TIME{ 10 };				//!< エフェクトスピードアップの生存時間
constexpr float	PLAYER_VALUE_DASHRATE{ 2.0f };			//!< 歩きに比べてのダッシュの速度倍率
constexpr float	PLAYER_MOVE_RATE_X{ 1.0f / 40000.0f };	//!< 左右歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
constexpr float	PLAYER_MOVE_RATE_Y{ 1.0f / 200.0f };	//!< 前後歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
constexpr float	PLAYER_MOVE_RATE_LR2{ 1.0f / 20000.0f };//!< キャタピラの移動量倍率(0～65000が入力範囲なのでそれに乗算する)
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU{ 0.03f };	//!< プレイヤー砲塔回転量
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU_MAX{ 1.57f };//!< プレイヤー砲塔回転量最大値
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN{ 0.01f };	//!< プレイヤー砲身回転量
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN_MAX{ 0.2f };	//!< プレイヤー砲塔回転量最大値
constexpr float	MOVE_INERTIA_MOMENT{ 0.8f };			//!< 移動量の慣性モーメント、減速係数として使用
constexpr float	VALUE_SPEEDBUFF_SUB{ 1.0f };			//!< スピートアップ時間のデクリメントする値
constexpr float	PLAYER_VALUE_MOVE{ 0.1f };				//!< 移動加速度
constexpr float	PLAYER_VALUE_MOVE_MAX{ 4.0f };			//!< 移動加速度の最大値
constexpr int	FRONT_VEC{ 1 };							//!< 前ベクトル
constexpr int	BACK_VEC{ -1 };							//!< 後ろベクトル

//=============================================================================
// 移動制御(ki-bo-doで移動制御)
//=============================================================================
void MOVE::MoveKeybord(int CntPlayer, PLAYER *Player)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	int dir = FRONT_VEC;
	//移動処理
	if (GetKeyboardPress(DIK_W))
	{
		dir = FRONT_VEC;
		Player->PlayerPara[CntPlayer].StandardPara.Speed += PLAYER_VALUE_MOVE * 10 * 4;
	}
	else if (GetKeyboardPress(DIK_S))
	{
		dir = BACK_VEC;
		Player->PlayerPara[CntPlayer].StandardPara.Speed -= PLAYER_VALUE_MOVE * 10 * 4;
	}

	// 無移動時は移動量に慣性をかける
	else
	{
		Player->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//旋回
	if (GetKeyboardPress(DIK_D))
	{
		HoudaiRot.y += 0.02f * dir*Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff * 2;
	}
	else if (GetKeyboardPress(DIK_A))
	{
		HoudaiRot.y -= 0.02f * dir*Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff * 2;
	}

	//角度の制限値
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// 移動速度の制限
	if (Player->PlayerPara[CntPlayer].StandardPara.Speed >= PLAYER_VALUE_MOVE_MAX) Player->PlayerPara[CntPlayer].StandardPara.Speed = PLAYER_VALUE_MOVE_MAX;
	else if (Player->PlayerPara[CntPlayer].StandardPara.Speed <= -PLAYER_VALUE_MOVE_MAX) Player->PlayerPara[CntPlayer].StandardPara.Speed = -PLAYER_VALUE_MOVE_MAX;

	// プレイヤーの座標を更新
	pos.x -= sinf(HoudaiRot.y) * (Player->PlayerPara[CntPlayer].StandardPara.Speed * Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (Player->PlayerPara[CntPlayer].StandardPara.Speed * Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//スピードバフ時間減少
	if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		Player->peffect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
	}


	//砲塔操作　バレット着弾点(左右エイム)
	if (GetKeyboardPress(DIK_RIGHT))
	{
		HoutouRot.y += VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y >= VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		HoutouRot.y -= VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y <= -VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = -VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}

	//砲身操作　バレット着弾点(前後エイム)
	if (GetKeyboardPress(DIK_UP))
	{
		HousinRot.x += VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x >= VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		HousinRot.x -= VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x <= -VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = -VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}

	//---------------------------------------------------------オブジェクト値セット
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);
}

//=============================================================================
// 移動制御(Lスティックで移動制御)
//=============================================================================
void MOVE::MoveL(int CntPlayer, PLAYER *Player, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = Player->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old保存
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//移動変化はLスティックアナログ値を使用
	float LAnalogX = 0.0f;		//縦入力
	float LAnalogY = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//スピードバフ時間減少
		Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		Player->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = pos;
		Player->peffect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			Player->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (Player->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//移動処理
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//旋回入力は後退中に限りリバースする
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		Player->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//移動量を反映
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	//---------------------------------------------------------オブジェクト値セット
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

}







//未使用
/*

//=============================================================================
// 移動制御(Lスティックで移動制御)
//=============================================================================
void MOVE::MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//移動変化はLスティックアナログ値を使用
	float LAnalogX = 0.0f;		//縦入力
	float LAnalogY = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//スピードバフ時間減少
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = pos;
		effect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (this->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//移動処理
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//旋回入力は後退中に限りリバースする
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//移動量を反映
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
}

//=============================================================================
// 移動制御(ABボタンLスティックで移動制御)
//=============================================================================
void MOVE::MoveABL(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);

	int dir = FRONT_VEC;
	//移動処理
	if (IsButtonPressed(CntPlayer, BUTTON_A))
	{
		dir = FRONT_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed += PLAYER_VALUE_MOVE;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_B))
	{
		dir = BACK_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed -= PLAYER_VALUE_MOVE;
	}

	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//視点変化のアナログ値を旋回に代入してアナログ操作で旋回
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//旋回
	if (!IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT) && !IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		dir = FRONT_VEC;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
	{
		HoudaiRot.y += LAnalogX * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		HoudaiRot.y += LAnalogX * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff;
	}

	//角度の制限値
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// 移動速度の制限
	if (this->PlayerPara[CntPlayer].StandardPara.Speed >= PLAYER_VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = PLAYER_VALUE_MOVE_MAX;
	else if (this->PlayerPara[CntPlayer].StandardPara.Speed <= -PLAYER_VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = -PLAYER_VALUE_MOVE_MAX;

	// プレイヤーの座標を更新
	pos.x -= sinf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//スピードバフ時間減少
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		effect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//砲塔操作　バレット着弾点(左右エイム)
	if (IsButtonPressed(CntPlayer, BUTTON_R1))
	{
		HoutouRot.y += VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y >= VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_L1))
	{
		HoutouRot.y -= VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y <= -VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = -VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}

	//砲身操作　バレット着弾点(前後エイム)
	if (IsButtonPressed(CntPlayer, BUTTON_R2))
	{
		HousinRot.x += VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x >= VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(CntPlayer, BUTTON_L2))
	{
		HousinRot.x -= VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x <= -VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = -VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}

	//---------------------------------------------------------オブジェクト値セット
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

}

//=============================================================================
// 移動制御(LRスティックでキャタピラ移動制御)
//=============================================================================
void MOVE::MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	//移動変化はLスティックアナログ値を使用
	float L2 = 0.0f;		//縦入力
	float R2 = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//スピードバフ時間減少
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = pos;
		effect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (this->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//左右のキャタピラを前進交代どちらかのフラグ
	//両キャタピラ前進方向=0、左キャタピラのみ前進=3.925f、右キャタピラのみ前進=-3.925f、左キャタピラのみ後退=-0.785f、右キャタピラのみ後退=0.785f、
	float Lrot = 3.925f;
	float Rrot = -3.925f;
	int Ldir = 1;
	int Rdir = 1;
	//移動処理
	if (IsButtonPressed(PadNum, BUTTON_L1))
	{
		Lrot = -0.785f;
		Ldir *= -1;
	}
	if (IsButtonPressed(PadNum, BUTTON_R1))
	{
		Rrot = 0.785f;
		Rdir *= -1;
	}

	//両キャタピラ使用
	if (IsButtonPressed(PadNum, BUTTON_L2) && IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		//入力中央値32767　R2最小0　L2最大64000
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		R2 = L2;

	}
	//左キャタピラのみ使用
	else if (IsButtonPressed(PadNum, BUTTON_L2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		HoudaiRot.y += 0.1f*Ldir;
	}
	//右キャタピラのみ使用
	else if (IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		float IZbuf = Button->lZ * PLAYER_MOVE_RATE_LR2;
		R2 = IZbuf;
		R2 = 32767 * PLAYER_MOVE_RATE_LR2 + (-R2) + 32767 * PLAYER_MOVE_RATE_LR2;
		HoudaiRot.y -= 0.1f*Rdir;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}

	//移動量を反映
	move.x = R2 * sinf(HoudaiRot.y + Rrot) + L2 * sinf(HoudaiRot.y + Lrot);
	move.z = R2 * cosf(HoudaiRot.y + Rrot) + L2 * cosf(HoudaiRot.y + Lrot);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}
*/
