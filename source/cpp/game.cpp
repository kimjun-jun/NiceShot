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
		{
			//-----------------------------------オブジェクト先頭アドレスを読み込み
			GAME_OBJECT *titleobj = GameObj->GetPointerTitle();
			TITLE *title = dynamic_cast<TITLE*>(&titleobj[0]);
			//タイトル更新
			title->Update();
			break;
		}
		case SCENE_TUTORIAL:
		{
			//-----------------------------------オブジェクト先頭アドレスを読み込み
			GAME_OBJECT *tutoobj = GameObj->GetPointerTuto();
			TUTO *tuto = dynamic_cast<TUTO*>(&tutoobj[0]);
			//チュートリアル更新
			tuto->Update();

			// map更新
			UpdateMeshField();
			UpdateMeshSky();
			UpdateMeshWall();
			//-----------------------------------オブジェクト先頭アドレスを読み込み
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

			//3D空間
			player->Update();
			bullet->Update();
			bulletprediction->Update();
			effect->Update();
			explosion->Update();
			item->Update();
			shadow->Update();

			CheakHit(0, &GameObj[0]);

			//2D空間
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
			//カウントダウンの更新
			countdown->Update();
			countdown->AddCountdown(-1);
			break;
		}
		case SCENE_GAME:
		{
			// map処理の更新
			UpdateMeshField();
			UpdateMeshSky();
			UpdateMeshWall();

			//-----------------------------------オブジェクト先頭アドレスを読み込み
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
			//オブジェクトの更新
			player->Update();
			bullet->Update();
			bulletprediction->Update();
			effect->Update();
			explosion->Update();
			item->Update();
			shadow->Update();

			//当たり判定
			CheakHit(1, &GameObj[0]);

			//-----------------------------------オブジェクト先頭アドレスを読み込み
			GAME_OBJECT *bulletgaugeobj = GameObj->GetPointerBulletgauge();
			BULLETGAUGE *bulletgauge = dynamic_cast<BULLETGAUGE*>(&bulletgaugeobj[0]);
			GAME_OBJECT *damegeobj = GameObj->GetPointerDamege();
			DAMEGE *damege = dynamic_cast<DAMEGE*>(&damegeobj[0]);
			GAME_OBJECT *statusobj = GameObj->GetPointerStatus();
			STATUS *status = dynamic_cast<STATUS*>(&statusobj[0]);
			GAME_OBJECT *vitalgaugeobj = GameObj->GetPointerVitalgauge();
			VITALGAUGE *vitalgauge = dynamic_cast<VITALGAUGE*>(&vitalgaugeobj[0]);

			//2D空間
			bulletgauge->Update();
			damege->Update();
			status->Update();
			vitalgauge->Update();
			break;
		}
		case SCENE_RESULT:
		{
			//-----------------------------------オブジェクト先頭アドレスを読み込み
			GAME_OBJECT *resultobj = GameObj->GetPointerResult();
			RESULT *result = dynamic_cast<RESULT*>(&resultobj[0]);
			//リザルトの更新
			result->Update();
			break;
		}
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
		{
			//-----------------------------------オブジェクト先頭アドレスを読み込み
			GAME_OBJECT *titleobj = GameObj->GetPointerTitle();
			TITLE *title = dynamic_cast<TITLE*>(&titleobj[0]);
			//タイトル描画
			title->Draw();
			break;
		}
		case SCENE_TUTORIAL:
		{
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

				//-----------------------------------オブジェクト先頭アドレスを読み込み
				GAME_OBJECT *playerobj = GameObj->GetPointerPlayer();
				PLAYER_HONTAI *player = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);

				if (player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

				// カメラの設定
				SetCamera(CntPlayer);

				//-----------------------------------オブジェクト先頭アドレスを読み込み
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

				//3D空間
				player->Draw();
				item->Draw();
				bulletprediction->Draw();
				explosion->Draw();
				effect->Draw();
				shadow->Draw();

				//2d画面上
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
				// カメラの設定
				SetCamera(CntPlayer);

				//-----------------------------------オブジェクト先頭アドレスを読み込み
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

				//3D空間
				player->Draw();
				item->Draw();
				bulletprediction->Draw();
				explosion->Draw();
				effect->Draw();
				shadow->Draw();

				//2d画面上
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

				//-----------------------------------オブジェクト先頭アドレスを読み込み
				GAME_OBJECT *playerobj = GameObj->GetPointerPlayer();
				PLAYER_HONTAI *player = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);

				bool puse = player[CntPlayer].GetUse();
				if (puse == true)
				{
					if (player[CntPlayer].KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(CntPlayer);

					//-----------------------------------オブジェクト先頭アドレスを読み込み
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

					//3D空間
					player->Draw();
					item->Draw();
					bulletprediction->Draw();
					explosion->Draw();
					effect->Draw();
					shadow->Draw();

					//2d画面上
					damege->Draw();
					status->Draw();
					vitalgauge->Draw();
					bulletgauge->Draw();
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					//-----------------------------------オブジェクト先頭アドレスを読み込み
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

					// カメラの設定
					SetCamera(CntPlayer);

					//map
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					//3D空間
					player->Draw();
					item->Draw();
					bulletprediction->Draw();
					explosion->Draw();
					effect->Draw();
					shadow->Draw();

					//2d画面上
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
			//-----------------------------------オブジェクト先頭アドレスを読み込み
			GAME_OBJECT *resultobj = GameObj->GetPointerResult();
			RESULT *result = dynamic_cast<RESULT*>(&resultobj[0]);
			//リザルト描画
			result->Draw();
			break;
		}
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

