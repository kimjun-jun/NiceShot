/**
* @file ATTACK.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../../../../h/main.h"
#include "../../../../../h/Other/input.h"
#include "../../../../../h/Other/sound.h"
#include "../../../../../h/Object/Bullet/bullet.h"
#include "../../../../../h/Object/Player/player.h"
#include "../../../../../h/Object/ObjectClass/StandardComponent/Attack/Attack.h"


constexpr float BULLET_BORN_TIME_ADD{ 1.0f };			//!< 弾薬復活させるための加算タイム
constexpr float BULLET_BORN_TIME_MAX{ 120.0f };			//!< 1弾薬復活するのに必要なタイム
constexpr float VALUE_LEN_BULLET{ 10.0f };				//!< プレイヤー中心としたバレットの発射位置までの距離


//=============================================================================
// バレット関連制御
//=============================================================================
void PLAYER_PARAMETER_BULLET::BulletALL(PLAYER *Player, int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------初期化(ネット対戦時使用)
	Player->PlayerPara[CntPlayer].BulletPara.BulletStartPos = VEC3_ALL0;
	Player->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
	for (int i = 0; i < 3; i++) Player->PlayerPara[CntPlayer].BulletPara.BulletMove[i] = VEC3_ALL0;

	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 POSTUREVec = Player->PlayerPara[CntPlayer].PostureVec.FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
	float Bkakezan = D3DXVec3Dot(&POSTUREVec, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(POSTUREVec.x*POSTUREVec.x +
				POSTUREVec.y *POSTUREVec.y +
				POSTUREVec.z * POSTUREVec.z)
			*
			sqrtf(Frontvec.x*Frontvec.x +
				Frontvec.y *Frontvec.y +
				Frontvec.z * Frontvec.z);
		Player->PlayerPara[CntPlayer].BulletPara.BulletRotY = acosf(cossita);
	}
	else
	{
		Player->PlayerPara[CntPlayer].BulletPara.BulletRotY = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	Player->PlayerPara[CntPlayer].BulletPara.BulletRotY -= 1.57f;

	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
	Player->PlayerPara[CntPlayer].BulletPara.BulletStartPos.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
	Player->PlayerPara[CntPlayer].BulletPara.BulletStartPos.y = pos.y + (sinf(Player->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
	Player->PlayerPara[CntPlayer].BulletPara.BulletStartPos.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);

	D3DXVECTOR3 BmoveRot;
	BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
	BmoveRot.y = sinf(Player->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x);
	BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

	Player->PlayerPara[CntPlayer].BulletPara.BulletMove[0].x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	Player->PlayerPara[CntPlayer].BulletPara.BulletMove[0].y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	Player->PlayerPara[CntPlayer].BulletPara.BulletMove[0].z = (BmoveRot.z) *VALUE_MOVE_BULLET;

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	// 弾発射
	if (Player->PlayerPara[CntPlayer].BulletPara.BulletStock > 0)
	{
		if (IsButtonTriggered(PadNum, BUTTON_R1) || GetKeyboardTrigger(DIK_SPACE))
		{
			Player->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 1;
			Player->pbullet->SetInstance(Player->PlayerPara[CntPlayer].BulletPara.BulletStartPos, Player->PlayerPara[CntPlayer].BulletPara.BulletMove[0], BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
			//拡散弾処理
			if (Player->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
			{
				Player->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 3;
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					Player->PlayerPara[CntPlayer].BulletPara.BulletMove[0].y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					Player->PlayerPara[CntPlayer].BulletPara.BulletMove[0].y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				Player->pbullet->SetInstance(Player->PlayerPara[CntPlayer].BulletPara.BulletStartPos, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
				Player->pbullet->SetInstance(Player->PlayerPara[CntPlayer].BulletPara.BulletStartPos, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));

			}
			//残弾を減らす
			Player->PlayerPara[CntPlayer].BulletPara.BulletStock -= 1;

			// SE再生
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}
	//残弾復活 一定時間経過で1個づつ自動回復
	if (Player->PlayerPara[CntPlayer].BulletPara.BulletStock < BULLET_MAX_STOCK) Player->PlayerPara[CntPlayer].BulletPara.BulletBornTime += BULLET_BORN_TIME_ADD;
	if (Player->PlayerPara[CntPlayer].BulletPara.BulletBornTime >= BULLET_BORN_TIME_MAX)
	{
		Player->PlayerPara[CntPlayer].BulletPara.BulletStock++;
		Player->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
	}
}



//未使用
/*
//=============================================================================
// バレット関連制御
//=============================================================================
void PLAYER_PARAMETER_BULLET::BulletALLMoveL2R2Ver(PLAYER *Player, int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 POSTUREVec = Player->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
	float Bkakezan = D3DXVec3Dot(&POSTUREVec, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(POSTUREVec.x*POSTUREVec.x +
				POSTUREVec.y *POSTUREVec.y +
				POSTUREVec.z * POSTUREVec.z)
			*
			sqrtf(Frontvec.x*Frontvec.x +
				Frontvec.y *Frontvec.y +
				Frontvec.z * Frontvec.z);
		Player->PlayerPara[CntPlayer].BulletPara.BulletRotY = acosf(cossita);
	}
	else
	{
		Player->PlayerPara[CntPlayer].BulletPara.BulletRotY = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	Player->PlayerPara[CntPlayer].BulletPara.BulletRotY -= 1.57f;


	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
	D3DXVECTOR3 BposStart;
	BposStart.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
	BposStart.y = pos.y + (sinf(Player->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
	BposStart.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);


	D3DXVECTOR3 BmoveRot;
	BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
	BmoveRot.y = sinf(Player->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x);
	BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

	D3DXVECTOR3 bulletmove;
	bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

	// 弾発射
	if (Player->PlayerPara[CntPlayer].BulletPara.BulletStock > 0)
	{
		if (IsButtonTriggered(CntPlayer, BUTTON_X))
		{
			Player->pbullet->SetInstance(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
			//拡散弾処理
			if (Player->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
			{
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				Player->pbullet->SetInstance(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
				Player->pbullet->SetInstance(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
			}
			//残弾を減らす
			Player->PlayerPara[CntPlayer].BulletPara.BulletStock -= 1;

			// SE再生
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}

	//残弾復活 一定時間経過で1個づつ自動回復
	if (Player->PlayerPara[CntPlayer].BulletPara.BulletStock < BULLET_MAX_STOCK) Player->PlayerPara[CntPlayer].BulletPara.BulletBornTime += BULLET_BORN_TIME_ADD;
	if (Player->PlayerPara[CntPlayer].BulletPara.BulletBornTime >= BULLET_BORN_TIME_MAX)
	{
		Player->PlayerPara[CntPlayer].BulletPara.BulletStock++;
		Player->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
	}
}
*/







