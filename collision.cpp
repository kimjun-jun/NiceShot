//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "bullettex.h"
#include "wall.h"
#include "field.h"
#include "fade.h"
#include "effect.h"
#include "life.h"
#include "item.h"
#include "time.h"
#include "sound.h"
#include "morphing.h"
#include "explosion.h"
#include "damege.h"
#include "status.h"
#include "collision.h"


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
};

void SetOjama(int type, int UsePlayer);

//=============================================================================
// 当たり判定BB2D　xz軸
//=============================================================================
bool CollisionBB2D(D3DXVECTOR3 pos1, float h1, float w1, D3DXVECTOR3 pos2, float h2, float w2)
{
	if (pos1.x + w1 >= pos2.x - w2
		&& pos1.x - w1 <= pos2.x + w2
		&& pos1.z + h1 >= pos2.z - h2
		&& pos1.z - h1 <= pos2.z + h2)
	{
		//当たっている
		return true;
	}
	//当たっていない
	return false;
}

//=============================================================================
// 当たり判定BB3D
//=============================================================================
bool CollisionBB3D(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{
	if (pos1.x + size1.x >= pos2.x - size2.x && pos1.x - size1.x <= pos2.x + size2.x
		&& pos1.y + size1.y >= pos2.y - size2.y  && pos1.y - size1.y <= pos2.y + size2.y
		&& pos1.z + size1.z >= pos2.z - size2.z  && pos1.z - size1.z <= pos2.y + size2.z)
	{
		//当たっている
		return true;
	}
	//当たっていない
	return false;
}

//=============================================================================
// 当たり判定BC
//=============================================================================
bool CollisionBC(D3DXVECTOR3 pos1, float r1, D3DXVECTOR3 pos2, float r2)
{
	//式1 プレイヤーの中心からエネミーの中心までの距離をplayer_ballettに代入
	D3DXVECTOR3 distance_pos = pos1 - pos2;

	//式2 プレイヤーサークル(ボディ)の半径
	//playerのpos(D3DXVECTOR3なので3次元)のx座標から当たり判定用テクスチャサイズ(プレイヤー半径)を差し引いた値をplayer_r(プレイヤー半径)に代入
	float wr1 = r1;
	//式2 エネミーサークル(ボディ)の半径
	float wr2 = r2;

	//D3DXVec2LengthSq()関数でdistance_pos(3次元)を2乗している
	float dista = D3DXVec3LengthSq(&distance_pos);

	float wr3 = (wr1 + wr2)*(wr1 + wr2);

	//当たり判定
	if (dista <= wr3)
	{
		//当たっている
		return true;
	}
	return false;
}

//=============================================================================
//衝突判定
//=============================================================================
void CheakHit(int scene)
{
	PLAYER_HONTAI *p =GetPlayerHoudai();			//playerhoudai
	PLAYER_PRATS *ps = GetPlayerHousin();			//playerhousin
	BULLET *b =GetBullet();							//playerbullet
	ITEM *i = GetItem();							//item
	WALL *Wall = GetWall();							//wall
	int WallNum = GetWallNum();						//wall総数
	DAMEGE *damede =GetDamege();					//screendamege


	//プレイヤーに対する当たり判定
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (p[CntPlayer].use == true)
		{
			//プレイヤー対壁
			for (int CntWall = 0; CntWall < 4; CntWall++)
			{
				switch (CntWall)
				{
				case 0:
					if (p[CntPlayer].pos.z + PLAYER_MODEL_SIZE >= Wall[CntWall].pos.z) p[CntPlayer].pos.z = p[CntPlayer].oldpos.z;
					break;
				case 1:
					if (p[CntPlayer].pos.x - PLAYER_MODEL_SIZE <= Wall[CntWall].pos.x) p[CntPlayer].pos.x = p[CntPlayer].oldpos.x;
					break;
				case 2:
					if (p[CntPlayer].pos.x + PLAYER_MODEL_SIZE >= Wall[CntWall].pos.x) p[CntPlayer].pos.x = p[CntPlayer].oldpos.x;
					break;
				case 3:
					if (p[CntPlayer].pos.z - PLAYER_MODEL_SIZE <= Wall[CntWall].pos.z) p[CntPlayer].pos.z = p[CntPlayer].oldpos.z;
					break;
				default:
					break;
				}
			}

			//プレイヤーバレット対プレイヤー、壁、床
			for (int CntPlayerBullet = 0; CntPlayerBullet < BULLET_MAX; CntPlayerBullet++)
			{
				if (b[CntPlayerBullet].use == true)
				{
						//対プレイヤー
						if (b[CntPlayerBullet].UsePlayerType != CntPlayer)
						{
							if (CollisionBC(p[CntPlayer].pos, PLAYER_MODEL_SIZE, b[CntPlayerBullet].pos, BULLET_MODEL_SIZE))
							{
								// エフェクト爆発の生成
								SetEffect(b[CntPlayerBullet].pos, D3DXVECTOR3(0.0f, 200.0f, 0.0f),
									PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType], 24.0f, 240.0f, 30);
								if (scene == 1)
								{
									ChangeLife(-1, CntPlayer);
								}
								//画面ダメージエフェクト
								damede[CntPlayer].use = true;
								damede[CntPlayer].time = 0.0f;
								damede[CntPlayer].alpha = 0;

								// バレット破棄
								ReleaseBullet(CntPlayerBullet);

								// SE再生
								PlaySound(SOUND_LABEL_SE_damage);
								break;
							}
						}
					//対壁
					for (int CntWall = 0; CntWall < WallNum; CntWall++)
					{
						switch (CntWall)
						{
						case 0:
							if (b[CntPlayerBullet].pos.z >= Wall[CntWall].pos.z)
							{
								// 爆発の生成
								SetExplosion(b[CntPlayerBullet].pos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								ReleaseBullet(CntPlayerBullet);
								// SE再生
								PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 1:
							if (b[CntPlayerBullet].pos.x <= Wall[CntWall].pos.x)
							{
								// 爆発の生成
								SetExplosion(b[CntPlayerBullet].pos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								ReleaseBullet(CntPlayerBullet);
								// SE再生
								PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 2:
							if (b[CntPlayerBullet].pos.x >= Wall[CntWall].pos.x)
							{
								// 爆発の生成
								SetExplosion(b[CntPlayerBullet].pos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								ReleaseBullet(CntPlayerBullet);
								// SE再生
								PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 3:
							if (b[CntPlayerBullet].pos.z <= Wall[CntWall].pos.z)
							{
								// 爆発の生成
								SetExplosion(b[CntPlayerBullet].pos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								ReleaseBullet(CntPlayerBullet);
								// SE再生
								PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						default:
							break;
						}
						if (b[CntPlayerBullet].use == false) break;
					}
				}
			}

			//プレイヤー対アイテム
			for (int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
			{
				if (i[CntItem].bUse == false) continue;
				if (CollisionBC(p[CntPlayer].pos, PLAYER_MODEL_SIZE, i[CntItem].pos, ITEM_MODEL_SIZE))
				{
					switch (i[CntItem].nType)
					{
					case ITEMTYPE_TIKEI:
						SetFieldInterPolationFieldType(rand() % 2);
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_LIFE:
						ChangeLife(1, CntPlayer);
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_SENSYA:
						if (p[CntPlayer].ModelType == PLAYER_MODEL_NORMAL)
						{
							p[CntPlayer].Morphing = true;
							p[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
							ps[CntPlayer].MorphingSignal = NowMorphing;
							ps[CntPlayer].time = 0.0f;
							p[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						else if (p[CntPlayer].Morphing == false && p[CntPlayer].MorphingTime <= 0.0f)
						{
							p[CntPlayer].Morphing = true;
							p[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
							ps[CntPlayer].MorphingSignal = NowMorphing;
							ps[CntPlayer].time = 0.0f;
							p[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						else
						{
							p[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						PlaySound(SOUND_LABEL_SE_rap1);
						break;
					case ITEMTYPE_BULLET:
						p[CntPlayer].AmmoNum = MAX_AMMO;
						ChangeBulletTex(MAX_AMMO, CntPlayer);
						p[CntPlayer].AmmoBornCnt = 0.0f;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_SPEEDUP:
						p[CntPlayer].speedbuff = MAX_SPEEDBUFF;
						p[CntPlayer].speedbufftime = MAX_SPEEDBUFFTIME;
						p[CntPlayer].speedbuffsignal = true;
						PlaySound(SOUND_LABEL_SE_speed);
						break;
					case ITEMTYPE_CAMERA:
						SetOjama(ITEMTYPE_CAMERA,CntPlayer);
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_KIRI:
						SetOjama(ITEMTYPE_KIRI, CntPlayer);
						PlaySound(SOUND_LABEL_SE_kiri);
						break;
					default:

						break;
					}
					DeleteItem(CntItem);
					i[0].GoukeiDrop--;
				}
			}
		}
	}
}

void SetOjama(int type, int UsePlayer)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();			//playerhoudai
	if (type == ITEMTYPE_CAMERA)
	{
		switch (UsePlayer)
		{
		case 0:
			p[1].BackCameraItemSignal = true;
			p[1].BackCameraItemTime = 0.0f;
			p[2].BackCameraItemSignal = true;
			p[2].BackCameraItemTime = 0.0f;
			p[3].BackCameraItemSignal = true;
			p[3].BackCameraItemTime = 0.0f;
			break;
		case 1:
			p[0].BackCameraItemSignal = true;
			p[0].BackCameraItemTime = 0.0f;
			p[2].BackCameraItemSignal = true;
			p[2].BackCameraItemTime = 0.0f;
			p[3].BackCameraItemSignal = true;
			p[3].BackCameraItemTime = 0.0f;
			break;
		case 2:
			p[0].BackCameraItemSignal = true;
			p[0].BackCameraItemTime = 0.0f;
			p[1].BackCameraItemSignal = true;
			p[1].BackCameraItemTime = 0.0f;
			p[3].BackCameraItemSignal = true;
			p[3].BackCameraItemTime = 0.0f;
			break;
		case 3:
			p[0].BackCameraItemSignal = true;
			p[0].BackCameraItemTime = 0.0f;
			p[1].BackCameraItemSignal = true;
			p[1].BackCameraItemTime = 0.0f;
			p[2].BackCameraItemSignal = true;
			p[2].BackCameraItemTime = 0.0f;
			break;
		default:
			break;
		}
	}
	else if(type==ITEMTYPE_KIRI)
	{
		switch (UsePlayer)
		{
		case 0:
			p[1].KiriSignal = true;
			p[1].KiriItemTime = 0.0f;
			p[2].KiriSignal = true;
			p[2].KiriItemTime = 0.0f;
			p[3].KiriSignal = true;
			p[3].KiriItemTime = 0.0f;
			break;
		case 1:
			p[0].KiriSignal = true;
			p[0].KiriItemTime = 0.0f;
			p[2].KiriSignal = true;
			p[2].KiriItemTime = 0.0f;
			p[3].KiriSignal = true;
			p[3].KiriItemTime = 0.0f;
			break;
		case 2:
			p[0].KiriSignal = true;
			p[0].KiriItemTime = 0.0f;
			p[1].KiriSignal = true;
			p[1].KiriItemTime = 0.0f;
			p[3].KiriSignal = true;
			p[3].KiriItemTime = 0.0f;
			break;
		case 3:
			p[0].KiriSignal = true;
			p[0].KiriItemTime = 0.0f;
			p[1].KiriSignal = true;
			p[1].KiriItemTime = 0.0f;
			p[2].KiriSignal = true;
			p[2].KiriItemTime = 0.0f;
			break;
		default:
			break;
		}
	}
}