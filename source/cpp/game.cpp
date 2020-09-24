/**
* @file game.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
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

int					g_nScene = SCENE_TITLE;		//!< ステージ番号
int					stop = 0;					//!< デバッグ時の一時停止用変数


//=============================================================================
// ゲームループ時の再初期化処理処理
// 戻り値：無し
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
// 更新処理
//=============================================================================
void UpdateGame(GAME_OBJECT *GameObj)
{
	// 入力更新
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
		// カメラの更新処理
		UpdateCamera();

		switch (g_nScene)
		{
		case SCENE_TITLE:
			GameObj->title->Update();
			break;
		case SCENE_TUTORIAL:

			//コントローラーのテクスチャ
			GameObj->tuto->Update();

			//3D空間
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

			//2D空間
			GameObj->bulletgauge->Update();
			GameObj->damege->Update();
			GameObj->status->Update();
			break;
		case SCENE_GAMECOUNTDOWN:
			GameObj->countdown->Update();
			GameObj->countdown->AddCountdown(-1);
			break;
		case SCENE_GAME:

			// map処理の更新
			UpdateMeshField();
			UpdateMeshSky();
			UpdateMeshWall();

			//3D空間
			GameObj->player->Update();
			GameObj->bullet->Update();
			GameObj->bulletprediction->Update();
			GameObj->effect->Update();
			GameObj->explosion->Update();
			GameObj->item->Update();
			GameObj->shadow->Update();

			CheakHit(1, &GameObj[0]);

			//2D空間
			GameObj->bulletgauge->Update();
			GameObj->damege->Update();
			GameObj->status->Update();
			GameObj->vitalgauge->Update();
			break;
		case SCENE_RESULT:
			GameObj->result->Update();
			break;
		}
		// フェード処理
		UpdateFade();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(GAME_OBJECT *GameObj)
{
	//四人分の画面分割設定
	D3DVIEWPORT9 vp[]
	{
		{DWORD(0),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(0),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
	};
	D3DVIEWPORT9 VpMaster{ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };
	
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// バックバッファ＆Ｚバッファのクリア
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	// Direct3Dによる描画の開始
	if (SUCCEEDED(pD3DDevice->BeginScene()))
	{
		// 画面遷移
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
				if (GameObj->player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

				// カメラの設定
				SetCamera(CntPlayer);

				//map
				DrawMeshSky();
				DrawMeshField();
				DrawMeshWall();

				//3D空間
				GameObj->player->Draw();
				GameObj->item->Draw();
				GameObj->bulletprediction->Draw();
				GameObj->explosion->Draw();
				GameObj->effect->Draw();
				GameObj->shadow->Draw();

				//2d画面上
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
					// カメラの設定
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					//3D空間
					GameObj->player->Draw();
					GameObj->item->Draw();
					GameObj->bulletprediction->Draw();
					GameObj->explosion->Draw();
					GameObj->effect->Draw();
					GameObj->shadow->Draw();

					//2d画面上
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
					if (GameObj->player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					//3D空間
					GameObj->player->Draw();
					GameObj->item->Draw();
					GameObj->bulletprediction->Draw();
					GameObj->explosion->Draw();
					GameObj->effect->Draw();
					GameObj->shadow->Draw();

					//2d画面上
					GameObj->damege->Draw();
					GameObj->status->Draw();
					GameObj->vitalgauge->Draw();
					GameObj->bulletgauge->Draw();
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					//3D空間
					GameObj->player->Draw();
					GameObj->item->Draw();
					GameObj->bulletprediction->Draw();
					GameObj->explosion->Draw();
					GameObj->effect->Draw();
					GameObj->shadow->Draw();

					//2d画面上
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
		// フェード描画
		DrawFade();

		// デバッグ表示
#ifdef _DEBUG
		DrawTextType();
		DrawDebugProc();
#endif
	}
	// Direct3Dによる描画の終了
	pD3DDevice->EndScene();
	// バックバッファとフロントバッファの入れ替え
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//=============================================================================
// Scene遷移
//=============================================================================
void SetScene(int Scene)
{
	g_nScene = Scene;
}

//=============================================================================
// GetScene遷移
//=============================================================================
int GetScene(void)
{
	return g_nScene;
}

