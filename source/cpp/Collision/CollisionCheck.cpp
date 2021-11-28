/**
* @file CollisionCheck.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
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

#define MAX_SPEEDBUFF						(1.5f)																		//!< �X�s�[�h�o�t�ړ��ʍő�l(��)
#define MAX_SPEEDBUFFTIME					(120.0f*3)																	//!< �X�s�[�h�o�t�L������
#define	EXPLOSION_COLLISIONPOS_BUFFSIZE		(5.0f)																		//!< 2D���j��ǂɓ����������̕`�悷����W�𒲐�
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< ���f���T�C�Y
#define ITEM_MODEL_SIZE						(15.0f)																		//!< ���f���T�C�Y


void UPDATE_MANAGER::CollisionCheck(GAME_OBJECT_INSTANCE *obj)
{
	//�v���C���[�ɑ΂��铖���蔻��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (obj->GetPlayer()->iUseType[CntPlayer].Use() == YesUseType1)
		{
			//�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 ppos = obj->GetPlayer()->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
			D3DXVECTOR3 poldpos = obj->GetPlayer()->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldPos();

			//�ۑ�
			obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.OldVital = obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital;

			//�v���C���[�Ε�
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
				//�I�u�W�F�N�g�l��������
				obj->GetPlayer()->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(ppos);
			}

			//�v���C���[�o���b�g�΃v���C���[�A�ǁA��
			for (int CntPlayerBullet = 0; CntPlayerBullet < OBJECT_BULLET_MAX; CntPlayerBullet++)
			{
				//�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 bpos = obj->GetBullet()->Transform[CntPlayerBullet].Pos();
				eUse_Type buse = obj->GetBullet()->iUseType[CntPlayerBullet].Use();

				if (buse == YesUseType1)
				{
					//�΃v���C���[
					if (obj->GetBullet()->BulletPara[CntPlayerBullet].UsePlayerType != CntPlayer)
					{
						if (CollisionBC(ppos, PLAYER_MODEL_SIZE, bpos, BULLET_MODEL_SIZE))
						{
							// �G�t�F�N�g�����̐���
							obj->GetEffect()->SetInstance(bpos, VEC3_ALL0,
								PLAYER_COLOR[obj->GetBullet()->BulletPara[CntPlayerBullet].UsePlayerType], 150.0f, 150.0f, 40);
							if (obj->GetScene()->ScenePara.eNowScene == SCENE_GAME || obj->GetScene()->ScenePara.eNowScene == SCENE_NETGAME)
							{
								obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital -= PLAYER_ATTACK_NORMAL;
							}
							//��ʃ_���[�W�G�t�F�N�g
							obj->GetDamege()->iUseType[CntPlayer].Use(YesUseType1);
							obj->GetDamege()->DamegePara[CntPlayer].time = 0;
							obj->GetDamege()->DamegePara[CntPlayer].alpha = 0;

							// �o���b�g�j��
							obj->GetBullet()->ReleaseInstance(CntPlayerBullet);

							// SE�Đ�
							PlaySound(SOUND_LABEL_SE_attack02);
							break;
						}
					}
					//�Ε�
					for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
					{
						D3DXVECTOR3 wpos = obj->GetWall()->Transform[CntWall].Pos();
						switch (CntWall)
						{
							//�t���O���ĂāA�֐��Ăяo�����������炷
						case 0:
							if (bpos.z >= wpos.z)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z- EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 1:
							if (bpos.x <= wpos.x)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x + EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 2:
							if (bpos.x >= wpos.x)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x - EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 3:
							if (bpos.z <= wpos.z)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z + EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								obj->GetBullet()->ReleaseInstance(CntPlayerBullet);
								// SE�Đ�
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

			//�v���C���[�΃A�C�e��
			for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
			{
				//�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 ipos = obj->GetItem()->Transform[CntItem].Pos();
				bool iuse = obj->GetItem()->iUseType[CntItem].Use();
				if (iuse == false || obj->GetItem()->ItemParaAll[CntItem].GettingSignal == true || obj->GetItem()->ItemParaAll[CntItem].GettingSignalEnd == true) continue;
				if (CollisionBC(ppos, PLAYER_MODEL_SIZE, ipos, ITEM_MODEL_SIZE))
				{
					switch (obj->GetItem()->ItemParaAll[CntItem].eType)
					{
					case ITEM_TYPE_TIKEI:
						//�n�`�ό`�M��ON
						if (obj->GetMySocket()->GetNetGameStartFlag()==true&& CntPlayer== obj->GetMySocket()->GetNetMyNumber())
						{
							obj->GetField()->FieldPara.ItemGetPlayerNum = CntPlayer;
							//�����_���֐���seed�l�����߂�
							obj->GetField()->FieldPara.TikeiSeed = (rand() % 1000000);
							//�ʐM���Ă���Ƃ��͂��̃V�[�h�l�𓯊������āA��������Ƃɓ����n�`�𐶐�����
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
						//�̗͉�
						obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital += PLAYER_ATTACK_NORMAL;
						if (obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital >= PLAYER_VITAL_MAX) obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.Vital = PLAYER_VITAL_MAX;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_SENSYA:
						//�ʏ탂�f���Ȃ烂�[�t�B���O�M��ON
						if (obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_NORMAL)
						{
							//���[�t�B���O�J�n�M���A���f���^�C�v�A���[�t�B���O���M���A���[�t�B���O���ԁA���[�t�B���O�I���J�E���g�̃Z�b�g
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//���[�t�B���O�I���ω����ɃA�C�e�����E�������͂�����x���[�t�B���O������
						else if (obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingStart == false && obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingTime <= 0.0f)
						{
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							obj->GetPlayer()->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							obj->GetPlayer()->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//�A�^�b�N���f�����ɃA�C�e���E�����玞�ԉ���
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
	//�J�������]
	if (type == ITEM_TYPE_CAMERA)
	{
		//�Ώۃv���C���[�̂݃J�������]
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraTime = 0.0f;
		}
	}
	//�t�H�O
	else if (type == ITEM_TYPE_KIRI)
	{
		//�Ώۃv���C���[�̂݃t�H�O����
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.KiriSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.KiriTime = 0.0f;
		}
	}
}
