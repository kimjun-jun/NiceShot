/**
* @file collision.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../h/main.h"
#include "../h/map/wall.h"
#include "../h/map/field.h"
#include "../h/other/fade.h"
#include "../h/other/sound.h"
#include "../h/object/objectclass.h"
#include "../h/object/player.h"
#include "../h/object/shadow.h"
#include "../h/effect/effect.h"
#include "../h/object/bullet/bullet.h"
#include "../h/effect/explosion.h"
#include "../h/object/item.h"
#include "../h/effect/damege.h"
#include "../h/object/status.h"
#include "../h/collision.h"


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
};

void SetOjama(int type, int UsePlayer, PLAYER_HONTAI *p);

//=============================================================================
// �����蔻��BB2D�@xz��
//=============================================================================
bool CollisionBB2D(D3DXVECTOR3 pos1, float h1, float w1, D3DXVECTOR3 pos2, float h2, float w2)
{
	if (pos1.x + w1 >= pos2.x - w2
		&& pos1.x - w1 <= pos2.x + w2
		&& pos1.z + h1 >= pos2.z - h2
		&& pos1.z - h1 <= pos2.z + h2)
	{
		//�������Ă���
		return true;
	}
	//�������Ă��Ȃ�
	return false;
}

//=============================================================================
// �����蔻��BB3D
//=============================================================================
bool CollisionBB3D(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{
	if (pos1.x + size1.x >= pos2.x - size2.x && pos1.x - size1.x <= pos2.x + size2.x
		&& pos1.y + size1.y >= pos2.y - size2.y  && pos1.y - size1.y <= pos2.y + size2.y
		&& pos1.z + size1.z >= pos2.z - size2.z  && pos1.z - size1.z <= pos2.y + size2.z)
	{
		//�������Ă���
		return true;
	}
	//�������Ă��Ȃ�
	return false;
}

//=============================================================================
// �����蔻��BC
//=============================================================================
bool CollisionBC(D3DXVECTOR3 pos1, float r1, D3DXVECTOR3 pos2, float r2)
{
	//��1 �v���C���[�̒��S����G�l�~�[�̒��S�܂ł̋�����player_ballett�ɑ��
	D3DXVECTOR3 distance_pos = pos1 - pos2;

	//��2 �v���C���[�T�[�N��(�{�f�B)�̔��a
	//player��pos(D3DXVECTOR3�Ȃ̂�3����)��x���W���瓖���蔻��p�e�N�X�`���T�C�Y(�v���C���[���a)�������������l��player_r(�v���C���[���a)�ɑ��
	float wr1 = r1;
	//��2 �G�l�~�[�T�[�N��(�{�f�B)�̔��a
	float wr2 = r2;

	//D3DXVec2LengthSq()�֐���distance_pos(3����)��2�悵�Ă���
	float dista = D3DXVec3LengthSq(&distance_pos);

	float wr3 = (wr1 + wr2)*(wr1 + wr2);

	//�����蔻��
	if (dista <= wr3)
	{
		//�������Ă���
		return true;
	}
	return false;
}

//=============================================================================
//�Փ˔���
//=============================================================================
void CheakHit(int scene, GAME_OBJECT* GameObj)
{
	//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
	GAME_OBJECT *pobj = GameObj->GetPointerPlayer();
	PLAYER_HONTAI *p = dynamic_cast<PLAYER_HONTAI*>(&pobj[0]);//�v���C���[
	GAME_OBJECT *bobj = GameObj->GetPointerBullet();
	BULLET *b = dynamic_cast<BULLET*>(&bobj[0]);//�o���b�g
	GAME_OBJECT *iobj = GameObj->GetPointerItem();
	ITEM *i = dynamic_cast<ITEM*>(&iobj[0]);//�A�C�e��
	GAME_OBJECT *dobj = GameObj->GetPointerDamege();
	DAMEGE *d = dynamic_cast<DAMEGE*>(&dobj[0]);//�G�t�F�N�g(2d�X�N���[��)
	GAME_OBJECT *eobj = GameObj->GetPointerEffect();
	EFFECT *e = dynamic_cast<EFFECT*>(&eobj[0]);//�G�t�F�N�g(3d�r���{�[�h)
	GAME_OBJECT *wobj = GameObj->GetPointerWall();
	WALL *w = dynamic_cast<WALL*>(&wobj[0]);//��
	GAME_OBJECT *fobj = GameObj->GetPointerField();
	FIELD *f = dynamic_cast<FIELD*>(&fobj[0]);//��

	//�v���C���[�ɑ΂��铖���蔻��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool puse = p[CntPlayer].GetUse();
		if (puse == true)
		{
			//�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 ppos = p[CntPlayer].GetPos();
			D3DXVECTOR3 poldpos = p[CntPlayer].GetOldPos();

			//�v���C���[�Ε�
			for (int CntWall = 0; CntWall < 4; CntWall++)
			{

				D3DXVECTOR3 wpos = w[CntWall].GetPos();
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
				p[CntPlayer].SetPos(ppos);
			}

			//�v���C���[�o���b�g�΃v���C���[�A�ǁA��
			for (int CntPlayerBullet = 0; CntPlayerBullet < OBJECT_BULLET_MAX; CntPlayerBullet++)
			{
				//�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 bpos = b[CntPlayerBullet].GetPos();
				bool buse = b[CntPlayerBullet].GetUse();

				if (buse == true)
				{
					//�΃v���C���[
					if (b[CntPlayerBullet].UsePlayerType != CntPlayer)
					{
						if (CollisionBC(ppos, PLAYER_MODEL_SIZE, bpos, BULLET_MODEL_SIZE))
						{
							// �G�t�F�N�g�����̐���
							e->SetEffect(bpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType], 150.0f, 150.0f, 40);
							if (scene == 1)
							{
								p[CntPlayer].vital -= PLAYER_ATTACK_NORMAL;
							}
							//��ʃ_���[�W�G�t�F�N�g
							d[CntPlayer].SetUse(true);
							d[CntPlayer].time = 0.0f;
							d[CntPlayer].alpha = 0;

							// �o���b�g�j��
							b[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);

							// SE�Đ�
							PlaySound(SOUND_LABEL_SE_attack02);
							break;
						}
					}
					//�Ε�
					for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
					{
						D3DXVECTOR3 wpos = w[CntWall].GetPos();
						switch (CntWall)
						{
						case 0:
							if (bpos.z >= wpos.z)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z- EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								b[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
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
								b[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
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
								b[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
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
								b[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						default:
							break;
						}
						if (buse == false) break;
					}
				}
			}

			//�v���C���[�΃A�C�e��
			for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
			{
				//�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 ipos = i[CntItem].GetPos();
				bool iuse = i[CntItem].GetUse();
				if (iuse == false || i[CntItem].GettingSignal == true || i[CntItem].GettingSignalEnd == true) continue;
				if (CollisionBC(ppos, PLAYER_MODEL_SIZE, ipos, ITEM_MODEL_SIZE))
				{
					switch (i[CntItem].nType)
					{
					case ITEMTYPE_TIKEI:
						f->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, CntPlayer);
						//SetFieldInterPolationFieldType(0);
						PlaySound(SOUND_LABEL_SE_enter03);
						PlaySound(SOUND_LABEL_SE_quake);
						break;
					case ITEMTYPE_LIFE:
						p[CntPlayer].vital += PLAYER_ATTACK_NORMAL;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_SENSYA:
						if (p[CntPlayer].ModelType == PLAYER_MODEL_NORMAL)
						{
							p[CntPlayer].Morphing = true;
							p[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
							p[CntPlayer].MorphingSignal = NowMorphing;
							p[CntPlayer].time = 0.0f;
							p[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						else if (p[CntPlayer].Morphing == false && p[CntPlayer].MorphingTime <= 0.0f)
						{
							p[CntPlayer].Morphing = true;
							p[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
							p[CntPlayer].MorphingSignal = NowMorphing;
							p[CntPlayer].time = 0.0f;
							p[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						else
						{
							p[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						PlaySound(SOUND_LABEL_SE_rap1);
						break;
					case ITEMTYPE_BULLET:
						p[CntPlayer].AmmoCnt = PLAYER_AMMOPOWER_NORMAL;
						p[CntPlayer].AmmoBornTime = 0.0f;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_SPEEDUP:
						p[CntPlayer].speedbuff = MAX_SPEEDBUFF;
						p[CntPlayer].speedbufftime = MAX_SPEEDBUFFTIME;
						p[CntPlayer].speedbuffsignal = true;
						PlaySound(SOUND_LABEL_SE_speed);
						break;
					case ITEMTYPE_CAMERA:
						SetOjama(ITEMTYPE_CAMERA, CntPlayer, &p[0]);
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_KIRI:
						SetOjama(ITEMTYPE_KIRI, CntPlayer, &p[0]);
						PlaySound(SOUND_LABEL_SE_kiri);
						break;
					default:

						break;
					}
					PlaySound(SOUND_LABEL_SE_kurukuru);
					i[CntItem].GettingSignal = true;
					i[CntItem].GetPlayerType = CntPlayer;
				}
			}
		}
	}
}

void SetOjama(int type, int UsePlayer, PLAYER_HONTAI *p)
{
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