/**
* @file bullet.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../../h/main.h"
#include "../../../h/other/input.h"
#include "../../../h/Object/Camera/camera.h"
//#include "../../../h/Collision/"
#include "../../../h/Other/sound.h"
#include "../../../h/Object/Shadow/shadow.h"
#include "../../../h/Effect/effect.h"
#include "../../../h/Object/Bullet/bullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float	EFFECT_BULLET_SIZE_X{ 16.0f };		//!< エフェクトバレットの幅
constexpr float	EFFECT_BULLET_SIZE_Y{ 16.0f };		//!< エフェクトバレットの高さ
constexpr int	EFFECT_BULLET_TIME{ 12 };			//!< エフェクトバレットの生存時間

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
BULLET::BULLET(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//カウントループ
	for (int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		//使用設定
		this->iUseType[CntBullet].Use(NoUse);
	}

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
BULLET::~BULLET(void)
{
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void BULLET::Init(void)
{
	//カウントループ
	for(int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		//使用設定
		this->iUseType[CntBullet].Use(NoUse);
		this->Transform[CntBullet].Pos(VEC3_ALL0);
		this->move[CntBullet].Move(VEC3_ALL0);
		this->BulletPara[CntBullet].Gravity = 0.0f;
		this->BulletPara[CntBullet].Timer = 0;
		this->BulletPara[CntBullet].UsePlayerType = PLAYER_NONE;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void BULLET::Update(SHADOW *shadow, EFFECT *effect)
{
	for(int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		bool use = this->iUseType[CntBullet].Use();
		//使用していたら弾道制御、影、エフェクトを設定する
		if (use)
		{
			//---------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this->Transform[CntBullet].Pos();
			D3DXVECTOR3 rot = this->Transform[CntBullet].Rot();
			D3DXVECTOR3 scl = this->Transform[CntBullet].Scl();
			D3DXVECTOR3 move = this->move[CntBullet].Move();

			//バレットがプレイヤーの一定範囲内なら若干のホーミング　未使用
			/*
			PLAYER_HONTAI *p = GetPlayerHoudai();

			for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
			{
				if (this[CntBullet].UsePlayerType == CntPlayer) continue;
				if (p[CntPlayer].use == false) continue;

				if (CollisionBC(p[CntPlayer].pos, PLAYER_MODEL_SIZE*5.0f, pos, BULLET_MODEL_SIZE))
				{
					if (this[CntBullet].HormingSignal == false)
					{
						this[CntBullet].HormingSignal = true;
						this[CntBullet].HormingPlayerType = CntPlayer;
					}
				}
				else
				{
					this[CntBullet].HormingSignal = false;
				}
			}
			if (this[CntBullet].HormingSignal == true)
			{
				this[CntBullet].Hormingmove = p[this[CntBullet].HormingPlayerType].pos - pos;
			}
			*/

			//ホーミングは未使用
			//pos.x += move.x + (this[CntBullet].Hormingmove.x / 30.0f);
			//pos.y -= this[CntBullet].Gravity + move.y - (this[CntBullet].Hormingmove.y / 10.0f);
			//pos.z += move.z + (this[CntBullet].Hormingmove.z / 30.0f);

			//弾道制御
			pos.x += move.x;
			pos.y -= this->BulletPara[CntBullet].Gravity + move.y;
			pos.z += move.z;

			this->BulletPara[CntBullet].Gravity += VALUE_GRAVITYADD_BULLET;
			if (this->BulletPara[CntBullet].Gravity > VALUE_GRAVITYMAX_BULLET) this->BulletPara[CntBullet].Gravity = VALUE_GRAVITYMAX_BULLET;

			//タイマー(生存時間)を減らす
			this->BulletPara[CntBullet].Timer--;

			//一定時間経過で未使用にする
			if (this->BulletPara[CntBullet].Timer <= 0)
			{
				shadow->ReleaseInstance(this->BulletPara[CntBullet].IdxShadow);
				this->ReleaseInstance(CntBullet, &shadow[0]);
			}
			else
			{
				// 影の位置設定
				shadow->UpdateInstance(this->BulletPara[CntBullet].IdxShadow,
					D3DXVECTOR3(pos.x, this->BulletPara[CntBullet].FieldPosY, pos.z), 
					rot,scl);

				// エフェクトの設定
				effect->SetInstance(pos, VEC3_ALL0,
					PLAYER_COLOR[this->BulletPara[CntBullet].UsePlayerType], 
					EFFECT_BULLET_SIZE_X, EFFECT_BULLET_SIZE_Y, EFFECT_BULLET_TIME);
			}
			this->Transform[CntBullet].Pos(pos);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void BULLET::Draw(void)
{

}

//=============================================================================
// 弾の設定
//=============================================================================
int BULLET::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, ePLAYER_TYPE type,SHADOW *s)
{
	int nIdxBullet = -1;

	for (int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		bool use = this->iUseType[CntBullet].Use();
		if (use != true)
		{
			//引数で初期値を設定 rotやsclは使用しない
   			this->Transform[CntBullet].Pos(pos);
			this->Transform[CntBullet].Rot(VEC3_ALL0);
			this->Transform[CntBullet].Scl(VEC3_ALL1);
			this->move[CntBullet].Move(move);
			this->iUseType[CntBullet].Use(YesUseType1);
			this->BulletPara[CntBullet].Timer = nTimer;
			this->BulletPara[CntBullet].UsePlayerType = type;
			this->BulletPara[CntBullet].Gravity = 0.0f;

			// 影の設定
			this->BulletPara[CntBullet].IdxShadow = s->SetInstance(pos, VEC3_ALL1);

			//リターンするインデックスを更新
			nIdxBullet = CntBullet;

			break;
		}
	}

	return nIdxBullet;
}

//=============================================================================
// 弾の削除
//=============================================================================
void BULLET::ReleaseInstance(int nIdxBullet, SHADOW *s)
{
	//インデックスが正常なら対応するバレットを未使用にする
	if(nIdxBullet >= 0 && nIdxBullet < OBJECT_BULLET_MAX)
	{
		s->ReleaseInstance(this->BulletPara[nIdxBullet].IdxShadow);
		this->iUseType[nIdxBullet].Use(NoUse);
	}
}
