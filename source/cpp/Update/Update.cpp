/**
* @file Update.h
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
#include "../../h/net/netmatch.h"
#include "../../h/net/sock.h"
#include "../../h/Object/ObjectClass/Instance/instance.h"
#include "../../h/Update/Update.h"


//インスタンス更新
void UPDATE_MANAGER::UpdateFunc(GAME_OBJECT_INSTANCE *obj)
{
	//サブスレッドによる更新をロックする
	obj->GetMySocket()->SetNetShareDateFlag(true);

	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	// 入力更新
	UpdateInput();
#ifdef _DEBUG

	//一時停止処理
	if (GetKeyboardTrigger(DIK_F12) || IsButtonTriggered(0, BUTTON_SELECT))
	{
		obj->GetScene()->ScenePara.stop = !obj->GetScene()->ScenePara.stop;
	}
	if (GetKeyboardTrigger(DIK_F11) || IsButtonTriggered(0, BUTTON_L3))
	{
		MasterVolumeChange(1);
		obj->Init();
		obj->GetScene()->NextScene(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
#endif

	if (obj->GetScene()->ScenePara.stop == false)
	{
		// カメラの更新処理
		UpdateCamera();

		// 画面遷移
		switch (obj->GetScene()->ScenePara.eNowScene)
		{
		case SCENE_TITLE:
			UpdateSceneTitle(obj);
			break;
		case SCENE_TUTORIAL:
			UpdateSceneTutorial(obj);
			break;
		case SCENE_GAMECOUNTDOWN:
			UpdateSceneCountDown(obj);
			break;
		case SCENE_GAME:
			UpdateSceneGame(obj);
			break;
		case SCENE_NETMATCH:
			UpdateSceneNetMatch(obj);
			break;
		case SCENE_NETGAMECOUNTDOWN:
			UpdateSceneNetCountDown(obj);
			break;
		case SCENE_NETGAME:
			UpdateSceneNetGame(obj);
			break;
		case SCENE_RESULT:
			UpdateSceneResult(obj);
			break;
		}
		// シーン処理
		obj->GetScene()->Update();
	}
}

void UPDATE_MANAGER::UpdateSceneTitle(GAME_OBJECT_INSTANCE *obj)
{
	//タイトル更新
	obj->GetTitle()->Update();
}

void UPDATE_MANAGER::UpdateSceneTutorial(GAME_OBJECT_INSTANCE *obj)
{
	obj->GetTuto()->Update();
	// map更新
	UpdateMap(obj);
	//3D空間
	Update3D(obj);
	//当たり判定
	CollisionCheck(obj);
	//2d画面上
	Update2D(obj);
}

void UPDATE_MANAGER::UpdateSceneCountDown(GAME_OBJECT_INSTANCE *obj)
{
	obj->GetCountdown()->Update();
	obj->GetCountdown()->AddCountdown(-1);
}

void UPDATE_MANAGER::UpdateSceneGame(GAME_OBJECT_INSTANCE *obj)
{
	// map更新
	UpdateMap(obj);
	//3D空間
	Update3D(obj);
	//当たり判定
	CollisionCheck(obj);
	//2d画面上
	Update2D(obj);
}

void UPDATE_MANAGER::UpdateSceneResult(GAME_OBJECT_INSTANCE *obj)
{
	//リザルト更新
	obj->GetResult()->Update();
}

void UPDATE_MANAGER::UpdateSceneNetMatch(GAME_OBJECT_INSTANCE *obj)
{
	//マッチング画面更新
	obj->GetNetMatch()->Update();
}

void UPDATE_MANAGER::UpdateSceneNetCountDown(GAME_OBJECT_INSTANCE *obj)
{
	obj->GetCountdown()->Update();
	obj->GetCountdown()->AddCountdown(-1);
}

void UPDATE_MANAGER::UpdateSceneNetGame(GAME_OBJECT_INSTANCE *obj)
{
	// map更新
	UpdateMap(obj);
	//3D空間
	Update3D(obj);
	//当たり判定
	CollisionCheck(obj);
	//2d画面上
	Update2D(obj);
}

void UPDATE_MANAGER::UpdateMap(GAME_OBJECT_INSTANCE *obj)
{
	// map更新
	obj->GetField()->Update();
	obj->GetSky()->Update();
}

void UPDATE_MANAGER::Update3D(GAME_OBJECT_INSTANCE *obj)
{
	//3D空間
	obj->GetPlayer()->Update();
	obj->GetBullet()->Update();
	obj->GetBulletPrediction()->Update();
	obj->GetEffect()->Update();
	obj->GetExplosion()->Update();
	obj->GetItem()->Update();
	obj->GetShadow()->Update();
}

void UPDATE_MANAGER::Update2D(GAME_OBJECT_INSTANCE *obj)
{
	//2d画面上
	obj->GetDamege()->Update();
	obj->GetStatus()->Update();
	obj->GetVitalGauge()->Update();
	obj->GetBulletGauge()->Update();
}




