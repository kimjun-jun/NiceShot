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
#include "../h/other/light.h"
#include "../h/object/player.h"
#include "../h/map/field.h"
#include "../h/map/wall.h"
#include "../h/map/sky.h"
#include "../h/object/shadow.h"
#include "../h/collision.h"
#include "../h/scene/title.h"
#include "../h/scene/result.h"
#include "../h/scene/fade.h"
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
#include "../h/object/objectclass.h"

int					g_nScene = SCENE_TITLE;		//!< �X�e�[�W�ԍ�
int					stop = 0;					//!< �f�o�b�O���̈ꎞ��~�p�ϐ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	return S_OK;
}

//=============================================================================
// �Q�[�����[�v���̍ď�������������
// �߂�l�F����
//=============================================================================
void ReinitGame(void)
{
	//�ď�����
	InitTutorial(1);
	InitCountdown(1);
	InitBullet(1);
	InitEffect(1);
	InitExplosion(1);
	InitDamege(1);
	InitStatus(1);
	InitRank(1);
	InitCamera();
	InitLight();
	InitShadow(1);
	ReInitTitle();
	ReInitPlayer();
	ReInitItem();
	ReInitVitalGauge();
	ReInitBulletGauge();
}


//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(GAME_OBJECT* GameObj)
{
	// ���͍X�V
	UpdateInput();
#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_F12) || IsButtonTriggered(0, BUTTON_SELECT)) stop++;
	if (GetKeyboardTrigger(DIK_F11) || IsButtonTriggered(0, BUTTON_L3))
	{
		MasterVolumeChange(1);
		InitGame();
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
			UpdateTitle();
			break;
		case SCENE_TUTORIAL:
			UpdateTutorial();
			UpdateMeshField();
			UpdateMeshSky();
			UpdatePlayer();
			UpdateBullet();
			UpdateBulletprediction();
			UpdateEffect();
			UpdateExplosion();
			UpdateItem();
			UpdateShadow();
			CheakHit(0);
			//UpdateBulletTex();
			UpdateBulletGauge();
			UpdateDamege();
			UpdateStatus();
			break;
		case SCENE_GAMECOUNTDOWN:
			UpdateCountdown();
			AddCountdown(-1);
			break;
		case SCENE_GAME:
			// �n�ʏ����̍X�V
			UpdateMeshField();
			UpdateMeshSky();

			// �Ǐ����̍X�V
			UpdateMeshWall();

			// �L��������̍X�V����
			UpdatePlayer();
			UpdateBullet();
			UpdateBulletprediction();
			UpdateEffect();
			UpdateExplosion();
			UpdateItem();

			// �e�̍X�V����
			UpdateShadow();

			// �����蔻��
			CheakHit(1);

			//2D�̍X�V����
			UpdateBulletGauge();
			UpdateDamege();
			UpdateStatus();
			UpdateVitalGauge();

			//���Ԑ����p
			//AddTime(-1);
			break;
		case SCENE_RESULT:
			UpdateResult();
			break;
		}
		// �t�F�[�h����
		UpdateFade();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(GAME_OBJECT* GameObj)
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
			DrawTitle();
			break;
		case SCENE_TUTORIAL:
			for (int i = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); i < vpCnt; i++)
			{
				pD3DDevice->SetViewport(&vp[i]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
				PLAYER_HONTAI *p = GetPlayerHoudai();
				if (p[i].use == false) continue;
				if (p[i].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

				// �J�����̐ݒ�
				SetCamera(i);

				//DrawBullet();
				//DrawBulletPoint(i);

				//�t�B�[���h
				DrawMeshSky();
				DrawMeshField();
				DrawMeshWall();

				// 3d���f���̕`�揈��
				DrawPlayer();
				DrawItem();

				DrawBulletprediction(i);
				DrawExplosion(i);
				DrawEffect(i);

				//�e
				DrawShadow();

				//2d��ʏ�
				DrawDamege();
				DrawStatus();
				//DrawLifeTex();
				DrawVitalGauge();
				//DrawBulletTex();
				DrawBulletGauge();
				DrawTutorial();
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;
		case SCENE_GAMECOUNTDOWN:
			for (int i = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); i < vpCnt; i++)
			{
				pD3DDevice->SetViewport(&vp[i]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
				PLAYER_HONTAI *p = GetPlayerHoudai();
				if (p[i].use == true)
				{
					// �J�����̐ݒ�
					SetCamera(i);

					//DrawBullet();
					//DrawBulletPoint(i);

					//�t�B�[���h
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					// 3d���f���̕`�揈��
					DrawPlayer();
					DrawItem();

					DrawBulletprediction(i);
					DrawExplosion(i);
					DrawEffect(i);

					//�e
					DrawShadow();

					//2d��ʏ�
					DrawDamege();
					DrawStatus();
					//DrawLifeTex();
					DrawVitalGauge();
					//DrawBulletTex();
					DrawBulletGauge();
				}
			}
			pD3DDevice->SetViewport(&VpMaster);
			//DrawTime();
			DrawCountdown();
			break;
		case SCENE_GAME:
			for (int i = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); i < vpCnt; i++)
			{
				pD3DDevice->SetViewport(&vp[i]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
				PLAYER_HONTAI *p = GetPlayerHoudai();

				if (p[i].use == true)
				{

					if (p[i].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(i);

					//DrawBullet();
					//DrawBulletPoint(i);

					//�t�B�[���h
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					// 3d���f���̕`�揈��
					DrawPlayer();
					DrawItem();

					DrawBulletprediction(i);
					DrawExplosion(i);
					DrawEffect(i);
					//�e
					DrawShadow();

					//2d��ʏ�
					DrawDamege();
					DrawStatus();
					//DrawLifeTex();
					DrawVitalGauge();
					//DrawBulletTex();
					DrawBulletGauge();
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(i);

					//�e
					DrawShadow();


					// 3d���f���̕`�揈��
					//DrawBullet();
					//DrawBulletPoint(i);

					DrawPlayer();
					DrawItem();

					//�t�B�[���h
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					DrawEffect(i);
					DrawExplosion(i);


					//2d��ʏ�
					DrawDamege();
					DrawStatus();
					//DrawLifeTex();
					DrawVitalGauge();
					//DrawBulletTex();
					DrawBulletGauge();
					DrawRank();
				}
			}

			pD3DDevice->SetViewport(&VpMaster);
			//DrawTime();
			break;
		case SCENE_RESULT:
			DrawResult();
			break;
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

