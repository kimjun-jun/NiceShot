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
			GameObj->title->Update();
			break;
		case SCENE_TUTORIAL:

			//�R���g���[���[�̃e�N�X�`��
			GameObj->tuto->Update();

			//3D���
			UpdateMeshField();
			UpdateMeshSky();
			GameObj->player->Update();
			GameObj->bullet->Update();
			GameObj->bulletprediction->Update();
			GameObj->effect->Update();
			GameObj->explosion->Update();
			GameObj->item->Update();
			GameObj->shadow->Update();

			CheakHit(0, &GameObj[0]);

			//2D���
			GameObj->bulletgauge->Update();
			GameObj->damege->Update();
			GameObj->status->Update();
			break;
		case SCENE_GAMECOUNTDOWN:
			GameObj->countdown->Update();
			GameObj->countdown->AddCountdown(-1);
			break;
		case SCENE_GAME:

			// map�����̍X�V
			UpdateMeshField();
			UpdateMeshSky();
			UpdateMeshWall();

			//3D���
			GameObj->player->Update();
			GameObj->bullet->Update();
			GameObj->bulletprediction->Update();
			GameObj->effect->Update();
			GameObj->explosion->Update();
			GameObj->item->Update();
			GameObj->shadow->Update();

			CheakHit(1, &GameObj[0]);

			//2D���
			GameObj->bulletgauge->Update();
			GameObj->damege->Update();
			GameObj->status->Update();
			GameObj->vitalgauge->Update();
			break;
		case SCENE_RESULT:
			GameObj->result->Update();
			break;
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
			GameObj->title->Draw();
			break;
		case SCENE_TUTORIAL:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
				if (GameObj->player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

				// �J�����̐ݒ�
				SetCamera(CntPlayer);

				//map
				DrawMeshSky();
				DrawMeshField();
				DrawMeshWall();

				//3D���
				GameObj->player->Draw();
				GameObj->item->Draw();
				GameObj->bulletprediction->Draw();
				GameObj->explosion->Draw();
				GameObj->effect->Draw();
				GameObj->shadow->Draw();

				//2d��ʏ�
				GameObj->damege->Draw();
				GameObj->status->Draw();
				GameObj->vitalgauge->Draw();
				GameObj->bulletgauge->Draw();
				GameObj->tuto->Draw();
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;
		case SCENE_GAMECOUNTDOWN:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					//3D���
					GameObj->player->Draw();
					GameObj->item->Draw();
					GameObj->bulletprediction->Draw();
					GameObj->explosion->Draw();
					GameObj->effect->Draw();
					GameObj->shadow->Draw();

					//2d��ʏ�
					GameObj->damege->Draw();
					GameObj->status->Draw();
					GameObj->vitalgauge->Draw();
					GameObj->bulletgauge->Draw();
			}
			pD3DDevice->SetViewport(&VpMaster);
			GameObj->countdown->Draw();
			break;
		case SCENE_GAME:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
				bool puse = GameObj->player[CntPlayer].GetUse();
				if (puse == true)
				{
					if (GameObj->player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					//3D���
					GameObj->player->Draw();
					GameObj->item->Draw();
					GameObj->bulletprediction->Draw();
					GameObj->explosion->Draw();
					GameObj->effect->Draw();
					GameObj->shadow->Draw();

					//2d��ʏ�
					GameObj->damege->Draw();
					GameObj->status->Draw();
					GameObj->vitalgauge->Draw();
					GameObj->bulletgauge->Draw();
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					//3D���
					GameObj->player->Draw();
					GameObj->item->Draw();
					GameObj->bulletprediction->Draw();
					GameObj->explosion->Draw();
					GameObj->effect->Draw();
					GameObj->shadow->Draw();

					//2d��ʏ�
					GameObj->damege->Draw();
					GameObj->status->Draw();
					GameObj->vitalgauge->Draw();
					GameObj->bulletgauge->Draw();
					GameObj->rank->Draw();
				}
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;
		case SCENE_RESULT:
			GameObj->result->Draw();
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

