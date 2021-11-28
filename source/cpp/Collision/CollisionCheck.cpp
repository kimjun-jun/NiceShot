/**
* @file CollisionCheck.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../h/main.h"
#include "../../h/Debug/debugproc.h"
#include "../../h/Other/input.h"
#include "../../h/Object/Scene/Scene.h"
#include "../../h/Object/Camera/camera.h"
#include "../../h/Other/sound.h"
#include "../../h/Collision/CollisionCal.h"
#include "../../h/Map/field.h"
#include "../../h/Map/wall.h"
#include "../../h/Map/sky.h"
#include "../../h/Object/Player/player.h"
#include "../../h/Object/Shadow/shadow.h"
#include "../../h/Object/Title/title.h"
#include "../../h/Object/Result/result.h"
#include "../../h/Object/Tutorial/tutorial.h"
#include "../../h/Effect/effect.h"
#include "../../h/Object/Bullet/bullet.h"
#include "../../h/Effect/explosion.h"
#include "../../h/Object/Rank/rank.h"
#include "../../h/Object/CountDown/countdown.h"
#include "../../h/Object/Item/item.h"
#include "../../h/Effect/damege.h"
#include "../../h/Object/Player/Status/status.h"
#include "../../h/Object/Bullet/bulletprediction.h"
#include "../../h/Object/Bullet/bulletgauge.h"
#include "../../h/Object/Player/VitalGauge/vitalgauge.h"
#include "../../h/Net/netmatch.h"
#include "../../h/Net/sock.h"
#include "../../h/Update/Update.h"
#include "../../h/Object/ObjectClass/Instance/instance.h"

void SetOjama(int type, int UsePlayer, PLAYER *p);

#define MAX_SPEEDBUFF						(1.5f)																		//!< スピードバフ移動量最大値(積)
#define MAX_SPEEDBUFFTIME					(120.0f*3)																	//!< スピードバフ有効時間
#define	EXPLOSION_COLLISIONPOS_BUFFSIZE		(5.0f)																		//!< 2D爆破を壁に当たった時の描画する座標を調整
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define ITEM_MODEL_SIZE						(15.0f)																		//!< モデルサイズ


void UPDATE_MANAGER::CollisionCheck(GAME_OBJECT_INSTANCE *obj)
{
	//プレイヤーに対する当たり判定
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (obj->GetPlayer()->iUseType[CntPlayer].Use() == YesUseType1)
		{
			//オブジェクト値読み込み
			D3DXVECTOR3 ppos = obj->GetPlayer()->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
			D3DXVECTOR3 poldpos = obj->GetPlayer()->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldPos();

			//保存
			obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.OldVital = obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital;

			//プレイヤー対壁
			for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
			{

				D3DXVECTOR3 wpos = obj->GetWall()->Transform[CntWall].Pos();
				switch (CntWall)
				{
				case 0:
					if (ppos.z + PLAYER_MODEL_SIZE >= wpos.z) ppos.z = poldpos.z;
					break;
				case 1:
					if (ppos.x - PLAYER_MODEL_SIZE <= wpos.x) ppos.x = poldpos.x;
					break;
				case 2:
					if (ppos.x + PLAYER_MODEL_SIZE >= wpos.x) ppos.x = poldpos.x;
					break;
				case 3:
					if (ppos.z - PLAYER_MODEL_SIZE <= wpos.z) ppos.z = poldpos.z;
					break;
				default:
					break;
				}
				//オブジェクト値書き込み
				obj->GetPlayer()->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(ppos);
			}

			//プレイヤーバレット対プレイヤー、壁、床
			for (int CntPlayerBullet = 0; CntPlayerBullet < OBJECT_BULLET_MAX; CntPlayerBullet++)
			{
				//オブジェクト値読み込み
				D3DXVECTOR3 bpos = obj->GetBullet()->Transform[CntPlayerBullet].Pos();
				eUse_Type buse = obj->GetBullet()->iUseType[CntPlayerBullet].Use();

				if (buse == YesUseType1)
				{
					//対プレイヤー
					if (obj->GetBullet()->BulletPara[CntPlayerBullet].UsePlayerType != CntPlayer)
					{
						if (CollisionBC(ppos, PLAYER_MODEL_SIZE, bpos, BULLET_MODEL_SIZE))
						{
							// エフェクト爆発の生成
							obj->GetEffect()->SetInstance(bpos, VEC3_ALL0,
								PLAYER_COLOR[obj->GetBullet()->BulletPara[CntPlayerBullet].UsePlayerType], 150.0f, 150.0f, 40);
							if (obj->GetScene()->ScenePara.eNowScene == SCENE_GAME || obj->GetScene()->ScenePara.eNowScene == SCENE_NETGAME)
							{
								obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital -= PLAYER_ATTACK_NORMAL;
							}
							//画面ダメージエフェクト
							obj->GetDamege()->iUseType[CntPlayer].Use(YesUseType1);
							obj->GetDamege()->DamegePara[CntPlayer].time = 0;
							obj->GetDamege()->DamegePara[CntPlayer].alpha = 0;

							// バレット破棄
							obj->GetBullet()->ReleaseInstance(CntPlayerBullet);

							// SE再生
							PlaySound(SOUND_LABEL_SE_attack02);
							break;
						}
					}
					//対壁
					for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
					{
						D3DXVECTOR3 wpos = obj->GetWall()->Transform[CntWall].Pos();
						switch (CntWall)
						{
							//フラグ立てて、関数呼び出し書く数減らす
						case 0:
							if (bpos.z >= wpos.z)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z- EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 1:
							if (bpos.x <= wpos.x)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x + EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 2:
							if (bpos.x >= wpos.x)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x - EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 3:
							if (bpos.z <= wpos.z)
							{
								// 爆発の生成
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z + EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// バレット破棄
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE再生
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						default:
							break;
						}
						if (buse == NoUse) break;
					}
				}
			}

			//プレイヤー対アイテム
			for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
			{
				//オブジェクト値読み込み
				D3DXVECTOR3 ipos = obj->GetItem()->Transform[CntItem].Pos();
				bool iuse = obj->GetItem()->iUseType[CntItem].Use();
				if (iuse == false || obj->GetItem()->ItemParaAll[CntItem].GettingSignal == true || obj->GetItem()->ItemParaAll[CntItem].GettingSignalEnd == true) continue;
				if (CollisionBC(ppos, PLAYER_MODEL_SIZE, ipos, ITEM_MODEL_SIZE))
				{
					switch (obj->GetItem()->ItemParaAll[CntItem].eType)
					{
					case ITEM_TYPE_TIKEI:
						//地形変形信号ON
						if (obj->GetMySocket()->GetNetGameStartFlag()==true&& CntPlayer== obj->GetMySocket()->GetNetMyNumber())
						{
							obj->GetField()->FieldPara.ItemGetPlayerNum = CntPlayer;
							//ランダム関数のseed値を決める
							obj->GetField()->FieldPara.TikeiSeed = (rand() % 1000000);
							//通信しているときはこのシード値を同期させて、これをもとに同じ地形を生成する
							//srand(this[0].field->TikeiSeed);
							//NetSetTikeiSeed(this[0].field->TikeiSeed, this[0].field->GetPlayerNum);
						}
						else if(obj->GetMySocket()->GetNetGameStartFlag() == false)
						{
							obj->GetField()->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, CntPlayer);
							//SetFieldInterPolationFieldType(0);
							PlaySound(SOUND_LABEL_SE_enter03);
							PlaySound(SOUND_LABEL_SE_quake);

						}
						break;
					case ITEM_TYPE_LIFE:
						//体力回復
						obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital += PLAYER_ATTACK_NORMAL;
						if (obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital >= PLAYER_VITAL_MAX) obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital = PLAYER_VITAL_MAX;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_SENSYA:
						//通常モデルならモーフィング信号ON
						if (obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_NORMAL)
						{
							//モーフィング開始信号、モデルタイプ、モーフィング中信号、モーフィング時間、モーフィング終了カウントのセット
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//モーフィング終了変化中にアイテムを拾った時はもう一度モーフィングさせる
						else if (obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingStart == false && obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingTime <= 0.0f)
						{
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//アタックモデル中にアイテム拾ったら時間延長
						else
						{
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = true;
						PlaySound(SOUND_LABEL_SE_rap1);
						break;
					case ITEM_TYPE_BULLET:
						obj->GetPlayer()->PlayerPara[CntPlayer].BulletPara.BulletStock = BULLET_MAX_STOCK;
						obj->GetPlayer()->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_SPEEDUP:
						obj->GetPlayer()->PlayerPara[CntPlayer].ItemPara.SpeedBuff = MAX_SPEEDBUFF;
						obj->GetPlayer()->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime = MAX_SPEEDBUFFTIME;
						obj->GetPlayer()->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = true;
						PlaySound(SOUND_LABEL_SE_speed);
						break;
					case ITEM_TYPE_CAMERA:
						SetOjama(ITEM_TYPE_CAMERA, CntPlayer, obj->GetPlayer());
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_KIRI:
						SetOjama(ITEM_TYPE_KIRI, CntPlayer, obj->GetPlayer());
						PlaySound(SOUND_LABEL_SE_kiri);
						break;
					default:

						break;
					}
					PlaySound(SOUND_LABEL_SE_kurukuru);
					obj->GetItem()->ItemParaAll[CntItem].GettingSignal = true;
					obj->GetItem()->ItemParaAll[CntItem].GetPlayerType = CntPlayer;
					obj->GetItem()->ItemParaAll[CntItem].NetGetItemFlag = true;
				}
			}
		}
	}
}

void SetOjama(int type, int UsePlayer, PLAYER *p)
{
	//カメラ反転
	if (type == ITEM_TYPE_CAMERA)
	{
		//対象プレイヤーのみカメラ反転
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraTime = 0.0f;
		}
	}
	//フォグ
	else if (type == ITEM_TYPE_KIRI)
	{
		//対象プレイヤーのみフォグ発動
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.KiriSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.KiriTime = 0.0f;
		}
	}
}
