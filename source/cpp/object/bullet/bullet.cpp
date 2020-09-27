/**
* @file bullet.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/other/input.h"
#include "../../../h/object/camera.h"
#include "../../../h/collision.h"
#include "../../../h/other/sound.h"
#include "../../../h/object/shadow.h"
#include "../../../h/effect/effect.h"
#include "../../../h/object/bullet/bullet.h"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
};

//=============================================================================
// 初期化処理
//=============================================================================
void BULLET::Init(void)
{
	for(int nCntBullet = 0; nCntBullet < OBJECT_BULLET_MAX; nCntBullet++)
	{
		this[nCntBullet].UsePlayerType = -1;
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void BULLET::Reinit(void)
{
	for (int nCntBullet = 0; nCntBullet < OBJECT_BULLET_MAX; nCntBullet++)
	{
		this[nCntBullet].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntBullet].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntBullet].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[nCntBullet].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntBullet].SetUse(false);
		this[nCntBullet].Gravity = 0.0f;
		this[nCntBullet].nTimer = 0;
		this[nCntBullet].UsePlayerType = -1;
		this[nCntBullet].fRadius = 0.0f;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void BULLET::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void BULLET::Update(SHADOW *shadow, EFFECT *effect)
{
	for(int nCntBullet = 0; nCntBullet < OBJECT_BULLET_MAX; nCntBullet++)
	{
		bool use = this[nCntBullet].GetUse();
		if (use)
		{
			//---------------------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[nCntBullet].GetPos();
			D3DXVECTOR3 scl = this[nCntBullet].GetScl();
			D3DXVECTOR3 move = this[nCntBullet].GetMove();

			//バレットがプレイヤーの一定範囲内なら若干のホーミング　未使用
			/*
			PLAYER_HONTAI *p = GetPlayerHoudai();

			for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
			{
				if (this[nCntBullet].UsePlayerType == CntPlayer) continue;
				if (p[CntPlayer].use == false) continue;

				if (CollisionBC(p[CntPlayer].pos, PLAYER_MODEL_SIZE*5.0f, pos, BULLET_MODEL_SIZE))
				{
					if (this[nCntBullet].HormingSignal == false)
					{
						this[nCntBullet].HormingSignal = true;
						this[nCntBullet].HormingPlayerType = CntPlayer;
					}
				}
				else
				{
					this[nCntBullet].HormingSignal = false;
				}
			}
			if (this[nCntBullet].HormingSignal == true)
			{
				this[nCntBullet].Hormingmove = p[this[nCntBullet].HormingPlayerType].pos - pos;
			}
			*/

			//ホーミングは未使用
			//pos.x += move.x + (this[nCntBullet].Hormingmove.x / 30.0f);
			//pos.y -= this[nCntBullet].Gravity + move.y - (this[nCntBullet].Hormingmove.y / 10.0f);
			//pos.z += move.z + (this[nCntBullet].Hormingmove.z / 30.0f);

			pos.x += move.x;
			pos.y -= this[nCntBullet].Gravity + move.y;
			pos.z += move.z;

			this[nCntBullet].Gravity += VALUE_GRAVITYADD_BULLET;
			if (this[nCntBullet].Gravity > VALUE_GRAVITYMAX_BULLET) this[nCntBullet].Gravity = VALUE_GRAVITYMAX_BULLET;

			this[nCntBullet].nTimer--;

			if (this[nCntBullet].nTimer <= 0)
			{
				shadow->ReleaseShadow(this[nCntBullet].nIdxShadow);
				ReleaseBullet(nCntBullet, &shadow[0]);
			}
			else
			{
				// 影の位置設定
				shadow->SetPositionShadow(this[nCntBullet].nIdxShadow, D3DXVECTOR3(pos.x, this[nCntBullet].FieldPosY, pos.z), scl);

				// エフェクトの設定
				effect->SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					PLAYER_COLOR[this[nCntBullet].UsePlayerType], EFFECT_BULLET_SIZE_X, EFFECT_BULLET_SIZE_Y, EFFECT_BULLET_TIME);
			}

			//影の処理
			float fSizeX = 8.0f + (pos.y - 4.0f) * 0.05f;
			if (fSizeX < 8.0f) fSizeX = 8.0f;
			float fSizeY = 8.0f + (pos.y - 4.0f) * 0.05f;
			if (fSizeY < 8.0f) fSizeY = 8.0f;
			shadow->SetVertexShadow(this[nCntBullet].nIdxShadow, fSizeX, fSizeY);

			float colA = (200.0f - (pos.y - 4.0f)) / 400.0f;
			if (colA < 0.0f) colA = 0.0f;
			shadow->SetColorShadow(this[nCntBullet].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));

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
int BULLET::SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, int type,SHADOW *s)
{
	int nIdxBullet = -1;

	for (int nCntBullet = 0; nCntBullet < OBJECT_BULLET_MAX; nCntBullet++)
	{
		bool use = this[nCntBullet].GetUse();
		if (use != true)
		{
			this[nCntBullet].SetPos(pos);
			this[nCntBullet].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this[nCntBullet].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this[nCntBullet].SetMove(move);
			this[nCntBullet].SetUse(true);
			this[nCntBullet].nTimer = nTimer;
			this[nCntBullet].UsePlayerType = type;
			this[nCntBullet].Gravity = 0.0f;


			// 影の設定
			this[nCntBullet].nIdxShadow = s->CreateShadow(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));		// 影の設定

			nIdxBullet = nCntBullet;

			break;
		}
	}

	return nIdxBullet;
}

//=============================================================================
// 弾の削除
//=============================================================================
void BULLET::ReleaseBullet(int nIdxBullet, SHADOW *s)
{
	if(nIdxBullet >= 0 && nIdxBullet < OBJECT_BULLET_MAX)
	{
		s->ReleaseShadow(this[nIdxBullet].nIdxShadow);
		this[nIdxBullet].SetUse(false);
	}
}
