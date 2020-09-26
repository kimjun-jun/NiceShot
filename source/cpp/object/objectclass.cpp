/**
* @file objectclass.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
using namespace std;

#include "../../h/main.h"
#include "../../h/other/debugproc.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/object/camera.h"
#include "../../h/other/sound.h"
#include "../../h/collision.h"
#include "../../h/map/field.h"
#include "../../h/map/wall.h"
#include "../../h/map/sky.h"
#include "../../h/object/player.h"
#include "../../h/object/shadow.h"
#include "../../h/scene/title.h"
#include "../../h/scene/result.h"
#include "../../h/scene/tutorial.h"
#include "../../h/effect/effect.h"
#include "../../h/object/bullet/bullet.h"
#include "../../h/effect/explosion.h"
#include "../../h/scene/rank.h"
#include "../../h/scene/countdown.h"
#include "../../h/object/item.h"
#include "../../h/effect/damege.h"
#include "../../h/object/status.h"
#include "../../h/object/bullet/bulletprediction.h"
#include "../../h/object/bullet/bulletgauge.h"
#include "../../h/object/vitalgauge.h"
#include "../../h/object/objectclass.h"

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
};

void GAME_OBJECT::Create()
{
	player = new PLAYER_HONTAI[OBJECT_PLAYER_MAX];
	effect = new EFFECT[OBJECT_EFFECT_MAX];
	bullet = new BULLET[OBJECT_BULLET_MAX];
	shadow = new SHADOW[OBJECT_SHADOW_MAX];
	countdown = new COUNTDOWN[OBJECT_COUNTDOWN_MAX];
	tuto = new TUTO[OBJECT_TUTORIAL_MAX];
	status = new STATUS[OBJECT_STATUS_MAX];
	bulletprediction = new BULLETPREDICTION[OBJECT_BULLETPREDICTION_MAX];
	vitalgauge = new VITALGAUGE[OBJECT_VITAL_MAX];
	bulletgauge = new BULLETGAUGE[OBJECT_BULLETGAUGE_MAX];
	damege = new DAMEGE[OBJECT_DAMEGE_MAX];
	explosion = new EXPLOSION[OBJECT_EXPLOSION_MAX];
	item = new ITEM[OBJECT_ITEM_MAX];
	rank = new RANK[OBJECT_RANK_MAX];
	result = new RESULT[OBJECT_RESULT_MAX];
	title = new TITLE[OBJECT_TITLE_MAX];
	field = new FIELD[OBJECT_FIELD_MAX];
	sky = new SKY[OBJECT_SKY_MAX];
	wall = new WALL[OBJECT_WALL_MAX];

}

void GAME_OBJECT::Init()
{
	player->Init();
	effect->Init();
	bullet->Init();
	shadow->Init();
	countdown->Init();
	tuto->Init();
	status->Init();
	bulletprediction->Init();
	vitalgauge->Init();
	bulletgauge->Init();
	damege->Init();
	explosion->Init();
	item->Init();
	rank->Init();
	result->Init();
	title->Init();
	field->Init();
	sky->Init();
	wall->Init();
}

void GAME_OBJECT::Reinit()
{
	player->Reinit();
	effect->Reinit();
	bullet->Reinit();
	shadow->Reinit();
	countdown->Reinit();
	tuto->Reinit();
	status->Reinit();
	bulletprediction->Reinit();
	vitalgauge->Reinit();
	bulletgauge->Reinit();
	damege->Reinit();
	explosion->Reinit();
	item->Reinit();
	rank->Reinit();
	result->Reinit();
	title->Reinit();
	field->Reinit();
	sky->Reinit();
	wall->Reinit();
}

void GAME_OBJECT::Update()
{
	// ���͍X�V
	UpdateInput();
#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_F12) || IsButtonTriggered(0, BUTTON_SELECT)) stop++;
	if (GetKeyboardTrigger(DIK_F11) || IsButtonTriggered(0, BUTTON_L3))
	{
		MasterVolumeChange(1);
		Reinit();
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
#endif

	if (stop % 2 == 0)
	{
		// �J�����̍X�V����
		UpdateCamera();

		switch (nScene)
		{
		case SCENE_TITLE:
			//�^�C�g���X�V
			title->Update();
			break;
		case SCENE_TUTORIAL:
			//�`���[�g���A���X�V
			tuto->Update();
			// map�X�V
			field->Update();
			sky->Update();

			//3D���
			player->Update();
			bullet->Update();
			bulletprediction->Update();
			effect->Update();
			explosion->Update();
			item->Update();
			shadow->Update();

			CheakHit(0);

			//2D���
			bulletgauge->Update();
			damege->Update();
			status->Update();
			break;

		case SCENE_GAMECOUNTDOWN:
			//�J�E���g�_�E���̍X�V
			countdown->Update();
			countdown->AddCountdown(-1);
			break;

		case SCENE_GAME:
			// map�X�V
			field->Update();
			sky->Update();

			//�I�u�W�F�N�g�̍X�V
			player->Update();
			bullet->Update();
			bulletprediction->Update();
			effect->Update();
			explosion->Update();
			item->Update();
			shadow->Update();

			//�����蔻��
			CheakHit(1);

			//2D���
			bulletgauge->Update();
			damege->Update();
			status->Update();
			vitalgauge->Update();
			break;

		case SCENE_RESULT:
			//���U���g�̍X�V
			result->Update();
			break;
		}
		// �t�F�[�h����
		UpdateFade();
	}

}

void GAME_OBJECT::Draw()
{
	//�l�l���̉�ʕ����ݒ�
	D3DVIEWPORT9 vp[]
	{
		{DWORD(0),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(0),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
	};
	D3DVIEWPORT9 VpMaster{ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pD3DDevice->BeginScene()))
	{
		// ��ʑJ��
		switch (nScene)
		{
		case SCENE_TITLE:
		{
			//�^�C�g���`��
			title->Draw();
			break;
		}
		case SCENE_TUTORIAL:
		{
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

				//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
				if (player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

				// �J�����̐ݒ�
				SetCamera(CntPlayer);

				// map�`��
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D���
				player->Draw();
				item->Draw();
				bulletprediction->Draw();
				explosion->Draw();
				effect->Draw();
				shadow->Draw();

				//2d��ʏ�
				damege->Draw();
				status->Draw();
				vitalgauge->Draw();
				bulletgauge->Draw();
				tuto->Draw();
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;
		}
		case SCENE_GAMECOUNTDOWN:
		{
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
				// �J�����̐ݒ�
				SetCamera(CntPlayer);

				// map�`��
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D���
				player->Draw();
				item->Draw();
				bulletprediction->Draw();
				explosion->Draw();
				effect->Draw();
				shadow->Draw();

				//2d��ʏ�
				damege->Draw();
				status->Draw();
				vitalgauge->Draw();
				bulletgauge->Draw();
			}
			pD3DDevice->SetViewport(&VpMaster);
			countdown->Draw();
			break;
		}
		case SCENE_GAME:
		{
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

				bool puse = player[CntPlayer].GetUse();
				if (puse == true)
				{
					if (player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					// map�`��
					field->Draw();
					sky->Draw();
					wall->Draw();

					//3D���
					player->Draw();
					item->Draw();
					bulletprediction->Draw();
					explosion->Draw();
					effect->Draw();
					shadow->Draw();

					//2d��ʏ�
					damege->Draw();
					status->Draw();
					vitalgauge->Draw();
					bulletgauge->Draw();
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					// map�`��
					field->Draw();
					sky->Draw();
					wall->Draw();

					//3D���
					player->Draw();
					item->Draw();
					bulletprediction->Draw();
					explosion->Draw();
					effect->Draw();
					shadow->Draw();

					//2d��ʏ�
					damege->Draw();
					status->Draw();
					vitalgauge->Draw();
					bulletgauge->Draw();
					rank->Draw();
				}
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;
		}
		case SCENE_RESULT:
		{
			//���U���g�`��
			result->Draw();
			break;
		}
		}
		// �t�F�[�h�`��
		DrawFade();

		// �f�o�b�O�\��
#ifdef _DEBUG
		DrawTextType();
		DrawDebugProc();
#endif
	}
	// Direct3D�ɂ��`��̏I��
	pD3DDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

void GAME_OBJECT::Uninit()
{
	player->Uninit();
	effect->Uninit();
	bullet->Uninit();
	shadow->Uninit();
	countdown->Uninit();
	tuto->Uninit();
	status->Uninit();
	bulletprediction->Uninit();
	vitalgauge->Uninit();
	bulletgauge->Uninit();
	damege->Uninit();
	explosion->Uninit();
	item->Uninit();
	rank->Uninit();
	result->Uninit();
	title->Uninit();
	field->Uninit();
	sky->Uninit();
	wall->Uninit();
}

void GAME_OBJECT::CheakHit(int scene)
{
	//�v���C���[�ɑ΂��铖���蔻��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool puse = player[CntPlayer].GetUse();
		if (puse == true)
		{
			//�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 ppos = player[CntPlayer].GetPos();
			D3DXVECTOR3 poldpos = player[CntPlayer].GetOldPos();

			//�v���C���[�Ε�
			for (int CntWall = 0; CntWall < 4; CntWall++)
			{

				D3DXVECTOR3 wpos = wall[CntWall].GetPos();
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
				player[CntPlayer].SetPos(ppos);
			}

			//�v���C���[�o���b�g�΃v���C���[�A�ǁA��
			for (int CntPlayerBullet = 0; CntPlayerBullet < OBJECT_BULLET_MAX; CntPlayerBullet++)
			{
				//�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 bpos = bullet[CntPlayerBullet].GetPos();
				bool buse = bullet[CntPlayerBullet].GetUse();

				if (buse == true)
				{
					//�΃v���C���[
					if (bullet[CntPlayerBullet].UsePlayerType != CntPlayer)
					{
						if (CollisionBC(ppos, PLAYER_MODEL_SIZE, bpos, BULLET_MODEL_SIZE))
						{
							// �G�t�F�N�g�����̐���
							effect->SetEffect(bpos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								PLAYER_COLOR[bullet[CntPlayerBullet].UsePlayerType], 150.0f, 150.0f, 40);
							if (scene == 1)
							{
								player[CntPlayer].vital -= PLAYER_ATTACK_NORMAL;
							}
							//��ʃ_���[�W�G�t�F�N�g
							damege[CntPlayer].SetUse(true);
							damege[CntPlayer].time = 0.0f;
							damege[CntPlayer].alpha = 0;

							// �o���b�g�j��
							bullet[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);

							// SE�Đ�
							PlaySound(SOUND_LABEL_SE_attack02);
							break;
						}
					}
					//�Ε�
					for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
					{
						D3DXVECTOR3 wpos = wall[CntWall].GetPos();
						switch (CntWall)
						{
						case 0:
							if (bpos.z >= wpos.z)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z- EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								bullet[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
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
								bullet[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
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
								bullet[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
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
								bullet[CntPlayerBullet].ReleaseBullet(CntPlayerBullet);
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
				D3DXVECTOR3 ipos = item[CntItem].GetPos();
				bool iuse = item[CntItem].GetUse();
				if (iuse == false || item[CntItem].GettingSignal == true || item[CntItem].GettingSignalEnd == true) continue;
				if (CollisionBC(ppos, PLAYER_MODEL_SIZE, ipos, ITEM_MODEL_SIZE))
				{
					switch (item[CntItem].nType)
					{
					case ITEMTYPE_TIKEI:
						field->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, CntPlayer);
						//SetFieldInterPolationFieldType(0);
						PlaySound(SOUND_LABEL_SE_enter03);
						PlaySound(SOUND_LABEL_SE_quake);
						break;
					case ITEMTYPE_LIFE:
						player[CntPlayer].vital += PLAYER_ATTACK_NORMAL;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_SENSYA:
						if (player[CntPlayer].ModelType == PLAYER_MODEL_NORMAL)
						{
							player[CntPlayer].Morphing = true;
							player[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
							player[CntPlayer].MorphingSignal = NowMorphing;
							player[CntPlayer].time = 0.0f;
							player[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						else if (player[CntPlayer].Morphing == false && player[CntPlayer].MorphingTime <= 0.0f)
						{
							player[CntPlayer].Morphing = true;
							player[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
							player[CntPlayer].MorphingSignal = NowMorphing;
							player[CntPlayer].time = 0.0f;
							player[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						else
						{
							player[CntPlayer].MorphingTime = MORPHING_TIME;
						}
						PlaySound(SOUND_LABEL_SE_rap1);
						break;
					case ITEMTYPE_BULLET:
						player[CntPlayer].AmmoCnt = PLAYER_AMMOPOWER_NORMAL;
						player[CntPlayer].AmmoBornTime = 0.0f;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_SPEEDUP:
						player[CntPlayer].speedbuff = MAX_SPEEDBUFF;
						player[CntPlayer].speedbufftime = MAX_SPEEDBUFFTIME;
						player[CntPlayer].speedbuffsignal = true;
						PlaySound(SOUND_LABEL_SE_speed);
						break;
					case ITEMTYPE_CAMERA:
						SetOjama(ITEMTYPE_CAMERA, CntPlayer, &player[0]);
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEMTYPE_KIRI:
						SetOjama(ITEMTYPE_KIRI, CntPlayer, &player[0]);
						PlaySound(SOUND_LABEL_SE_kiri);
						break;
					default:

						break;
					}
					PlaySound(SOUND_LABEL_SE_kurukuru);
					item[CntItem].GettingSignal = true;
					item[CntItem].GetPlayerType = CntPlayer;
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
	else if (type == ITEMTYPE_KIRI)
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



//-------------------------------------------------------------------2DVB�e�N�X�`���Q�b�g�֐�
LPDIRECT3DTEXTURE9 TEXTURE2D_VERTEXBUFFER::GetpD3DTexture() { return this->pD3DTexture; };
LPDIRECT3DVERTEXBUFFER9 TEXTURE2D_VERTEXBUFFER::GetpD3DVtxBuff() { return this->pD3DVtxBuff; };

//-------------------------------------------------------------------2DVB�e�N�X�`���Z�b�g�֐�
void TEXTURE2D_VERTEXBUFFER::SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture) { this->pD3DTexture = pD3DTexture; };
void TEXTURE2D_VERTEXBUFFER::SetpD3DVtxBuff(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff) { this->pD3DVtxBuff = pD3DVtxBuff; };


//-------------------------------------------------------------------2D�e�N�X�`���Q�b�g�֐�
LPDIRECT3DTEXTURE9 TEXTURE_2D::GetpD3DTexture() { return this->pD3DTexture; };
VERTEX_2D* TEXTURE_2D::GettextureVTX() { return &this->textureVTX[0]; };

//-------------------------------------------------------------------2D�e�N�X�`���Z�b�g�֐�
void TEXTURE_2D::SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture) {this->pD3DTexture = pD3DTexture;};
void TEXTURE_2D::SettextureVTX(VERTEX_2D *textureVTX) { this->textureVTX[0] = textureVTX[0], 
this->textureVTX[1] = textureVTX[1], this->textureVTX[2] = textureVTX[2], this->textureVTX[3] = textureVTX[3]; };


//-------------------------------------------------------------------2D�I�u�W�F�N�gVB�e�N�X�`���Q�b�g�֐�
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetPos() { return this->para.pos; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetOldPos() { return this->para.oldpos; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetRot() { return this->para.rot; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetScl() { return this->para.scl; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetMove() { return this->para.move; }
D3DXCOLOR OBJECT_2D_VERTEXBUFFER::GetCol() { return this->para.col; }
bool OBJECT_2D_VERTEXBUFFER::GetUse() { return this->para.use; }

//-------------------------------------------------------------------2D�I�u�W�F�N�gVB�e�N�X�`���Z�b�g�֐�
void OBJECT_2D_VERTEXBUFFER::SetPos(D3DXVECTOR3 pos) { this->para.pos = pos; }
void OBJECT_2D_VERTEXBUFFER::SetOldPos(D3DXVECTOR3 oldpos) { this->para.oldpos = oldpos; }
void OBJECT_2D_VERTEXBUFFER::SetRot(D3DXVECTOR3 rot) { this->para.rot = rot; }
void OBJECT_2D_VERTEXBUFFER::SetScl(D3DXVECTOR3 scl) { this->para.scl = scl; }
void OBJECT_2D_VERTEXBUFFER::SetMove(D3DXVECTOR3 move) { this->para.move = move; }
void OBJECT_2D_VERTEXBUFFER::SetCol(D3DXCOLOR col) { this->para.col = col; }
void OBJECT_2D_VERTEXBUFFER::SetUse(bool use) { this->para.use=use; }



//-------------------------------------------------------------------2D�I�u�W�F�N�g�Q�b�g�֐�
D3DXVECTOR3 OBJECT_2D::GetPos() { return this->para.pos; }
D3DXVECTOR3 OBJECT_2D::GetOldPos() { return this->para.oldpos; }
D3DXVECTOR3 OBJECT_2D::GetRot() { return this->para.rot; }
D3DXVECTOR3 OBJECT_2D::GetScl() { return this->para.scl; }
D3DXVECTOR3 OBJECT_2D::GetMove() { return this->para.move; }
D3DXCOLOR OBJECT_2D::GetCol() { return this->para.col; }
bool OBJECT_2D::GetUse() { return this->para.use; }

//-------------------------------------------------------------------2D�I�u�W�F�N�g�Z�b�g�֐�
void OBJECT_2D::SetPos(D3DXVECTOR3 pos) { this->para.pos = pos; }
void OBJECT_2D::SetOldPos(D3DXVECTOR3 oldpos) { this->para.oldpos = oldpos; }
void OBJECT_2D::SetRot(D3DXVECTOR3 rot) { this->para.rot = rot; }
void OBJECT_2D::SetScl(D3DXVECTOR3 scl) { this->para.scl = scl; }
void OBJECT_2D::SetMove(D3DXVECTOR3 move) { this->para.move = move; }
void OBJECT_2D::SetCol(D3DXCOLOR col) { this->para.col = col; }
void OBJECT_2D::SetUse(bool use) { this->para.use = use; }


//-------------------------------------------------------------------3D�I�u�W�F�N�g�Q�b�g�֐�
D3DXMATRIX OBJECT_3D::GetMatrix() { return this->para.mtxWorld; }
D3DXVECTOR3 OBJECT_3D::GetPos() { return this->para.pos; }
D3DXVECTOR3 OBJECT_3D::GetOldPos() { return this->para.oldpos; }
D3DXVECTOR3 OBJECT_3D::GetRot() { return this->para.rot; }
D3DXVECTOR3 OBJECT_3D::GetScl() { return this->para.scl; }
D3DXVECTOR3 OBJECT_3D::GetMove() { return this->para.move; }
D3DXVECTOR3 OBJECT_3D::GetFieldNorVec() { return this->para.FieldNorVec; }
D3DXVECTOR3 OBJECT_3D::GetFieldNorUpNorCross() { return this->para.FieldNorUpNorCross; }
float OBJECT_3D::GetQrot() { return this->para.Qrot; }
D3DXCOLOR OBJECT_3D::GetCol() { return this->para.col; }
bool OBJECT_3D::GetUse() { return this->para.use; }

//-------------------------------------------------------------------3D�I�u�W�F�N�g�Z�b�g�֐�
void OBJECT_3D::SetMatrix(D3DXMATRIX mtxWorld) { this->para.mtxWorld = mtxWorld; }
void OBJECT_3D::SetPos(D3DXVECTOR3 pos) { this->para.pos = pos; }
void OBJECT_3D::SetOldPos(D3DXVECTOR3 oldpos) { this->para.oldpos = oldpos; }
void OBJECT_3D::SetRot(D3DXVECTOR3 rot) { this->para.rot = rot; }
void OBJECT_3D::SetScl(D3DXVECTOR3 scl) { this->para.scl = scl; }
void OBJECT_3D::SetMove(D3DXVECTOR3 move) { this->para.move = move; }
void OBJECT_3D::SetFieldNorVec(D3DXVECTOR3 FieldNorVec) { this->para.FieldNorVec = FieldNorVec; }
void OBJECT_3D::SetFieldNorUpNorCross(D3DXVECTOR3 FieldNorUpNorCross) { this->para.FieldNorUpNorCross = FieldNorUpNorCross; }
void OBJECT_3D::SetQrot(float Qrot) { this->para.Qrot = Qrot; }
void OBJECT_3D::SetCol(D3DXCOLOR col) { this->para.col = col; }
void OBJECT_3D::SetUse(bool use) { this->para.use = use; }





