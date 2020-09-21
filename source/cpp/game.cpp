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

int					g_nScene = SCENE_TITLE;		//!< ステージ番号
int					stop = 0;					//!< デバッグ時の一時停止用変数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	return S_OK;
}

//=============================================================================
// ゲームループ時の再初期化処理処理
// 戻り値：無し
//=============================================================================
void ReinitGame(void)
{
	//再初期化
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
// 終了処理
//=============================================================================
void UninitGame(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(GAME_OBJECT* GameObj)
{
	// 入力更新
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
		// カメラの更新処理
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
			// 地面処理の更新
			UpdateMeshField();
			UpdateMeshSky();

			// 壁処理の更新
			UpdateMeshWall();

			// キャラ周りの更新処理
			UpdatePlayer();
			UpdateBullet();
			UpdateBulletprediction();
			UpdateEffect();
			UpdateExplosion();
			UpdateItem();

			// 影の更新処理
			UpdateShadow();

			// 当たり判定
			CheakHit(1);

			//2Dの更新処理
			UpdateBulletGauge();
			UpdateDamege();
			UpdateStatus();
			UpdateVitalGauge();

			//時間制限用
			//AddTime(-1);
			break;
		case SCENE_RESULT:
			UpdateResult();
			break;
		}
		// フェード処理
		UpdateFade();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(GAME_OBJECT* GameObj)
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
			DrawTitle();
			break;
		case SCENE_TUTORIAL:
			for (int i = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); i < vpCnt; i++)
			{
				pD3DDevice->SetViewport(&vp[i]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
				PLAYER_HONTAI *p = GetPlayerHoudai();
				if (p[i].use == false) continue;
				if (p[i].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

				// カメラの設定
				SetCamera(i);

				//DrawBullet();
				//DrawBulletPoint(i);

				//フィールド
				DrawMeshSky();
				DrawMeshField();
				DrawMeshWall();

				// 3dモデルの描画処理
				DrawPlayer();
				DrawItem();

				DrawBulletprediction(i);
				DrawExplosion(i);
				DrawEffect(i);

				//影
				DrawShadow();

				//2d画面上
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
					// カメラの設定
					SetCamera(i);

					//DrawBullet();
					//DrawBulletPoint(i);

					//フィールド
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					// 3dモデルの描画処理
					DrawPlayer();
					DrawItem();

					DrawBulletprediction(i);
					DrawExplosion(i);
					DrawEffect(i);

					//影
					DrawShadow();

					//2d画面上
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

					if (p[i].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(i);

					//DrawBullet();
					//DrawBulletPoint(i);

					//フィールド
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					// 3dモデルの描画処理
					DrawPlayer();
					DrawItem();

					DrawBulletprediction(i);
					DrawExplosion(i);
					DrawEffect(i);
					//影
					DrawShadow();

					//2d画面上
					DrawDamege();
					DrawStatus();
					//DrawLifeTex();
					DrawVitalGauge();
					//DrawBulletTex();
					DrawBulletGauge();
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(i);

					//影
					DrawShadow();


					// 3dモデルの描画処理
					//DrawBullet();
					//DrawBulletPoint(i);

					DrawPlayer();
					DrawItem();

					//フィールド
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					DrawEffect(i);
					DrawExplosion(i);


					//2d画面上
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

