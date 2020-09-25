/**
* @file game.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/

#include "../h/main.h"
#include "../h/other/debugproc.h"
#include "../h/other/input.h"
#include "../h/object/camera.h"
#include "../h/map/field.h"
#include "../h/map/wall.h"
#include "../h/map/sky.h"
#include "../h/collision.h"
#include "../h/other/fade.h"
#include "../h/other/sound.h"
#include "../h/object/player.h"
#include "../h/map/field.h"
#include "../h/map/wall.h"
#include "../h/map/sky.h"
#include "../h/object/shadow.h"
#include "../h/scene/title.h"
#include "../h/scene/result.h"
#include "../h/other/fade.h"
#include "../h/scene/tutorial.h"
#include "../h/effect/effect.h"
#include "../h/object/bullet/bullet.h"
#include "../h/effect/explosion.h"
#include "../h/scene/rank.h"
#include "../h/scene/countdown.h"
#include "../h/object/item.h"
#include "../h/effect/damege.h"
#include "../h/object/status.h"
#include "../h/other/sound.h"
#include "../h/object/bullet/bulletprediction.h"
#include "../h/object/bullet/bulletgauge.h"
#include "../h/object/vitalgauge.h"
#include "../h/game.h"


int					g_nScene = SCENE_TITLE;		//!< �X�e�[�W�ԍ�
int					stop = 0;					//!< �f�o�b�O���̈ꎞ��~�p�ϐ�


//=============================================================================
// �Q�[�����[�v���̍ď�������������
// �߂�l�F����
//=============================================================================
void ReinitGame(GAME_OBJECT *obj)
{
	InitCamera();

	for (int CntOBJ = 0; CntOBJ < OBJECT_ALL_MAX; CntOBJ++,obj++)
	{
		obj[CntOBJ].Reinit();
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(GAME_OBJECT *GameObj)
{
	// ���͍X�V
	UpdateInput();
#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_F12) || IsButtonTriggered(0, BUTTON_SELECT)) stop++;
	if (GetKeyboardTrigger(DIK_F11) || IsButtonTriggered(0, BUTTON_L3))
	{
		MasterVolumeChange(1);
		ReinitGame(&GameObj[0]);
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
#endif

	if (stop % 2 == 0)
	{
		// �J�����̍X�V����
		UpdateCamera();

		switch (g_nScene)
		{
		case SCENE_TITLE:
		{
			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *titleobj = GameObj->GetPointerTitle();
			TITLE *title = dynamic_cast<TITLE*>(&titleobj[0]);
			//�^�C�g���X�V
			title->Update();
			break;
		}
		case SCENE_TUTORIAL:
		{
			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *tutoobj = GameObj->GetPointerTuto();
			TUTO *tuto = dynamic_cast<TUTO*>(&tutoobj[0]);
			//�`���[�g���A���X�V
			tuto->Update();

			// map�X�V
			UpdateMeshField();
			UpdateMeshSky();
			UpdateMeshWall();
			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *playerobj = GameObj->GetPointerPlayer();
			PLAYER_HONTAI *player = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);
			GAME_OBJECT *bulletobj = GameObj->GetPointerBullet();
			BULLET *bullet = dynamic_cast<BULLET*>(&bulletobj[0]);
			GAME_OBJECT *bulletpredictionobj = GameObj->GetPointerBulletprediction();
			BULLETPREDICTION *bulletprediction = dynamic_cast<BULLETPREDICTION*>(&bulletpredictionobj[0]);
			GAME_OBJECT *effectobj = GameObj->GetPointerEffect();
			EFFECT *effect = dynamic_cast<EFFECT*>(&effectobj[0]);
			GAME_OBJECT *explosionobj = GameObj->GetPointerExplosion();
			EXPLOSION *explosion = dynamic_cast<EXPLOSION*>(&explosionobj[0]);
			GAME_OBJECT *itemobj = GameObj->GetPointerItem();
			ITEM *item = dynamic_cast<ITEM*>(&itemobj[0]);
			GAME_OBJECT *shadowobj = GameObj->GetPointerShadow();
			SHADOW *shadow = dynamic_cast<SHADOW*>(&shadowobj[0]);

			//3D���
			player->Update();
			bullet->Update();
			bulletprediction->Update();
			effect->Update();
			explosion->Update();
			item->Update();
			shadow->Update();

			CheakHit(0, &GameObj[0]);

			//2D���
			GAME_OBJECT *bulletgaugeobj = GameObj->GetPointerBulletgauge();
			BULLETGAUGE *bulletgauge = dynamic_cast<BULLETGAUGE*>(&bulletgaugeobj[0]);
			GAME_OBJECT *damegeobj = GameObj->GetPointerDamege();
			DAMEGE *damege = dynamic_cast<DAMEGE*>(&damegeobj[0]);
			GAME_OBJECT *statusobj = GameObj->GetPointerStatus();
			STATUS *status = dynamic_cast<STATUS*>(&statusobj[0]);
			bulletgauge->Update();
			damege->Update();
			status->Update();
			break;
		}
		case SCENE_GAMECOUNTDOWN:
		{
			GAME_OBJECT *countdownobj = GameObj->GetPointerCountdown();
			COUNTDOWN *countdown = dynamic_cast<COUNTDOWN*>(&countdownobj[0]);
			//�J�E���g�_�E���̍X�V
			countdown->Update();
			countdown->AddCountdown(-1);
			break;
		}
		case SCENE_GAME:
		{
			// map�����̍X�V
			UpdateMeshField();
			UpdateMeshSky();
			UpdateMeshWall();

			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *playerobj = GameObj->GetPointerPlayer();
			PLAYER_HONTAI *player = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);
			GAME_OBJECT *bulletobj = GameObj->GetPointerBullet();
			BULLET *bullet = dynamic_cast<BULLET*>(&bulletobj[0]);
			GAME_OBJECT *bulletpredictionobj = GameObj->GetPointerBulletprediction();
			BULLETPREDICTION *bulletprediction = dynamic_cast<BULLETPREDICTION*>(&bulletpredictionobj[0]);
			GAME_OBJECT *effectobj = GameObj->GetPointerEffect();
			EFFECT *effect = dynamic_cast<EFFECT*>(&effectobj[0]);
			GAME_OBJECT *explosionobj = GameObj->GetPointerExplosion();
			EXPLOSION *explosion = dynamic_cast<EXPLOSION*>(&explosionobj[0]);
			GAME_OBJECT *itemobj = GameObj->GetPointerItem();
			ITEM *item = dynamic_cast<ITEM*>(&itemobj[0]);
			GAME_OBJECT *shadowobj = GameObj->GetPointerShadow();
			SHADOW *shadow = dynamic_cast<SHADOW*>(&shadowobj[0]);
			//�I�u�W�F�N�g�̍X�V
			player->Update();
			bullet->Update();
			bulletprediction->Update();
			effect->Update();
			explosion->Update();
			item->Update();
			shadow->Update();

			//�����蔻��
			CheakHit(1, &GameObj[0]);

			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *bulletgaugeobj = GameObj->GetPointerBulletgauge();
			BULLETGAUGE *bulletgauge = dynamic_cast<BULLETGAUGE*>(&bulletgaugeobj[0]);
			GAME_OBJECT *damegeobj = GameObj->GetPointerDamege();
			DAMEGE *damege = dynamic_cast<DAMEGE*>(&damegeobj[0]);
			GAME_OBJECT *statusobj = GameObj->GetPointerStatus();
			STATUS *status = dynamic_cast<STATUS*>(&statusobj[0]);
			GAME_OBJECT *vitalgaugeobj = GameObj->GetPointerVitalgauge();
			VITALGAUGE *vitalgauge = dynamic_cast<VITALGAUGE*>(&vitalgaugeobj[0]);

			//2D���
			bulletgauge->Update();
			damege->Update();
			status->Update();
			vitalgauge->Update();
			break;
		}
		case SCENE_RESULT:
		{
			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *resultobj = GameObj->GetPointerResult();
			RESULT *result = dynamic_cast<RESULT*>(&resultobj[0]);
			//���U���g�̍X�V
			result->Update();
			break;
		}
		}
		// �t�F�[�h����
		UpdateFade();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(GAME_OBJECT *GameObj)
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
		switch (g_nScene)
		{
		case SCENE_TITLE:
		{
			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *titleobj = GameObj->GetPointerTitle();
			TITLE *title = dynamic_cast<TITLE*>(&titleobj[0]);
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
				GAME_OBJECT *playerobj = GameObj->GetPointerPlayer();
				PLAYER_HONTAI *player = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);

				if (player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

				// �J�����̐ݒ�
				SetCamera(CntPlayer);

				//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
				GAME_OBJECT *tutoobj = GameObj->GetPointerTuto();
				TUTO *tuto = dynamic_cast<TUTO*>(&tutoobj[0]);
				GAME_OBJECT *bulletobj = GameObj->GetPointerBullet();
				BULLET *bullet = dynamic_cast<BULLET*>(&bulletobj[0]);
				GAME_OBJECT *bulletpredictionobj = GameObj->GetPointerBulletprediction();
				BULLETPREDICTION *bulletprediction = dynamic_cast<BULLETPREDICTION*>(&bulletpredictionobj[0]);
				GAME_OBJECT *effectobj = GameObj->GetPointerEffect();
				EFFECT *effect = dynamic_cast<EFFECT*>(&effectobj[0]);
				GAME_OBJECT *explosionobj = GameObj->GetPointerExplosion();
				EXPLOSION *explosion = dynamic_cast<EXPLOSION*>(&explosionobj[0]);
				GAME_OBJECT *itemobj = GameObj->GetPointerItem();
				ITEM *item = dynamic_cast<ITEM*>(&itemobj[0]);
				GAME_OBJECT *shadowobj = GameObj->GetPointerShadow();
				SHADOW *shadow = dynamic_cast<SHADOW*>(&shadowobj[0]);
				GAME_OBJECT *bulletgaugeobj = GameObj->GetPointerBulletgauge();
				BULLETGAUGE *bulletgauge = dynamic_cast<BULLETGAUGE*>(&bulletgaugeobj[0]);
				GAME_OBJECT *vitalgaugeobj = GameObj->GetPointerVitalgauge();
				VITALGAUGE *vitalgauge = dynamic_cast<VITALGAUGE*>(&vitalgaugeobj[0]);
				GAME_OBJECT *damegeobj = GameObj->GetPointerDamege();
				DAMEGE *damege = dynamic_cast<DAMEGE*>(&damegeobj[0]);
				GAME_OBJECT *statusobj = GameObj->GetPointerStatus();
				STATUS *status = dynamic_cast<STATUS*>(&statusobj[0]);

				//map
				DrawMeshSky();
				DrawMeshField();
				DrawMeshWall();

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

				//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
				GAME_OBJECT *playerobj = GameObj->GetPointerPlayer();
				PLAYER_HONTAI *player = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);
				GAME_OBJECT *bulletobj = GameObj->GetPointerBullet();
				BULLET *bullet = dynamic_cast<BULLET*>(&bulletobj[0]);
				GAME_OBJECT *bulletpredictionobj = GameObj->GetPointerBulletprediction();
				BULLETPREDICTION *bulletprediction = dynamic_cast<BULLETPREDICTION*>(&bulletpredictionobj[0]);
				GAME_OBJECT *effectobj = GameObj->GetPointerEffect();
				EFFECT *effect = dynamic_cast<EFFECT*>(&effectobj[0]);
				GAME_OBJECT *explosionobj = GameObj->GetPointerExplosion();
				EXPLOSION *explosion = dynamic_cast<EXPLOSION*>(&explosionobj[0]);
				GAME_OBJECT *itemobj = GameObj->GetPointerItem();
				ITEM *item = dynamic_cast<ITEM*>(&itemobj[0]);
				GAME_OBJECT *shadowobj = GameObj->GetPointerShadow();
				SHADOW *shadow = dynamic_cast<SHADOW*>(&shadowobj[0]);
				GAME_OBJECT *bulletgaugeobj = GameObj->GetPointerBulletgauge();
				BULLETGAUGE *bulletgauge = dynamic_cast<BULLETGAUGE*>(&bulletgaugeobj[0]);
				GAME_OBJECT *vitalgaugeobj = GameObj->GetPointerVitalgauge();
				VITALGAUGE *vitalgauge = dynamic_cast<VITALGAUGE*>(&vitalgaugeobj[0]);
				GAME_OBJECT *damegeobj = GameObj->GetPointerDamege();
				DAMEGE *damege = dynamic_cast<DAMEGE*>(&damegeobj[0]);
				GAME_OBJECT *statusobj = GameObj->GetPointerStatus();
				STATUS *status = dynamic_cast<STATUS*>(&statusobj[0]);

				//map
				DrawMeshSky();
				DrawMeshField();
				DrawMeshWall();

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
			GAME_OBJECT *countdownobj = GameObj->GetPointerCountdown();
			COUNTDOWN *countdown = dynamic_cast<COUNTDOWN*>(&countdownobj[0]);
			countdown->Draw();
			break;
		}
		case SCENE_GAME:
		{
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

				//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
				GAME_OBJECT *playerobj = GameObj->GetPointerPlayer();
				PLAYER_HONTAI *player = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);

				bool puse = player[CntPlayer].GetUse();
				if (puse == true)
				{
					if (player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
					GAME_OBJECT *bulletobj = GameObj->GetPointerBullet();
					BULLET *bullet = dynamic_cast<BULLET*>(&bulletobj[0]);
					GAME_OBJECT *bulletpredictionobj = GameObj->GetPointerBulletprediction();
					BULLETPREDICTION *bulletprediction = dynamic_cast<BULLETPREDICTION*>(&bulletpredictionobj[0]);
					GAME_OBJECT *effectobj = GameObj->GetPointerEffect();
					EFFECT *effect = dynamic_cast<EFFECT*>(&effectobj[0]);
					GAME_OBJECT *explosionobj = GameObj->GetPointerExplosion();
					EXPLOSION *explosion = dynamic_cast<EXPLOSION*>(&explosionobj[0]);
					GAME_OBJECT *itemobj = GameObj->GetPointerItem();
					ITEM *item = dynamic_cast<ITEM*>(&itemobj[0]);
					GAME_OBJECT *shadowobj = GameObj->GetPointerShadow();
					SHADOW *shadow = dynamic_cast<SHADOW*>(&shadowobj[0]);
					GAME_OBJECT *bulletgaugeobj = GameObj->GetPointerBulletgauge();
					BULLETGAUGE *bulletgauge = dynamic_cast<BULLETGAUGE*>(&bulletgaugeobj[0]);
					GAME_OBJECT *vitalgaugeobj = GameObj->GetPointerVitalgauge();
					VITALGAUGE *vitalgauge = dynamic_cast<VITALGAUGE*>(&vitalgaugeobj[0]);
					GAME_OBJECT *damegeobj = GameObj->GetPointerDamege();
					DAMEGE *damege = dynamic_cast<DAMEGE*>(&damegeobj[0]);
					GAME_OBJECT *statusobj = GameObj->GetPointerStatus();
					STATUS *status = dynamic_cast<STATUS*>(&statusobj[0]);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

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

					//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
					GAME_OBJECT *bulletobj = GameObj->GetPointerBullet();
					BULLET *bullet = dynamic_cast<BULLET*>(&bulletobj[0]);
					GAME_OBJECT *bulletpredictionobj = GameObj->GetPointerBulletprediction();
					BULLETPREDICTION *bulletprediction = dynamic_cast<BULLETPREDICTION*>(&bulletpredictionobj[0]);
					GAME_OBJECT *effectobj = GameObj->GetPointerEffect();
					EFFECT *effect = dynamic_cast<EFFECT*>(&effectobj[0]);
					GAME_OBJECT *explosionobj = GameObj->GetPointerExplosion();
					EXPLOSION *explosion = dynamic_cast<EXPLOSION*>(&explosionobj[0]);
					GAME_OBJECT *itemobj = GameObj->GetPointerItem();
					ITEM *item = dynamic_cast<ITEM*>(&itemobj[0]);
					GAME_OBJECT *shadowobj = GameObj->GetPointerShadow();
					SHADOW *shadow = dynamic_cast<SHADOW*>(&shadowobj[0]);
					GAME_OBJECT *bulletgaugeobj = GameObj->GetPointerBulletgauge();
					BULLETGAUGE *bulletgauge = dynamic_cast<BULLETGAUGE*>(&bulletgaugeobj[0]);
					GAME_OBJECT *vitalgaugeobj = GameObj->GetPointerVitalgauge();
					VITALGAUGE *vitalgauge = dynamic_cast<VITALGAUGE*>(&vitalgaugeobj[0]);
					GAME_OBJECT *damegeobj = GameObj->GetPointerDamege();
					DAMEGE *damege = dynamic_cast<DAMEGE*>(&damegeobj[0]);
					GAME_OBJECT *statusobj = GameObj->GetPointerStatus();
					STATUS *status = dynamic_cast<STATUS*>(&statusobj[0]);
					GAME_OBJECT *rankobj = GameObj->GetPointerRank();
					RANK *rank = dynamic_cast<RANK*>(&rankobj[0]);

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

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
			//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
			GAME_OBJECT *resultobj = GameObj->GetPointerResult();
			RESULT *result = dynamic_cast<RESULT*>(&resultobj[0]);
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


//=============================================================================
// Scene�J��
//=============================================================================
void SetScene(int Scene)
{
	g_nScene = Scene;
}

//=============================================================================
// GetScene�J��
//=============================================================================
int GetScene(void)
{
	return g_nScene;
}

