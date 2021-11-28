/**
* @file Update.h
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
#include "../../h/net/netmatch.h"
#include "../../h/net/sock.h"
#include "../../h/Object/ObjectClass/Instance/instance.h"
#include "../../h/Update/Update.h"


//�C���X�^���X�X�V
void UPDATE_MANAGER::UpdateFunc(GAME_OBJECT_INSTANCE *obj)
{
	//�T�u�X���b�h�ɂ��X�V�����b�N����
	obj->GetMySocket()->SetNetShareDateFlag(true);

	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	// ���͍X�V
	UpdateInput();
#ifdef _DEBUG

	//�ꎞ��~����
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
		// �J�����̍X�V����
		UpdateCamera();

		// ��ʑJ��
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
		// �V�[������
		obj->GetScene()->Update();
	}
}

void UPDATE_MANAGER::UpdateSceneTitle(GAME_OBJECT_INSTANCE *obj)
{
	//�^�C�g���X�V
	obj->GetTitle()->Update();
}

void UPDATE_MANAGER::UpdateSceneTutorial(GAME_OBJECT_INSTANCE *obj)
{
	obj->GetTuto()->Update();
	// map�X�V
	UpdateMap(obj);
	//3D���
	Update3D(obj);
	//�����蔻��
	CollisionCheck(obj);
	//2d��ʏ�
	Update2D(obj);
}

void UPDATE_MANAGER::UpdateSceneCountDown(GAME_OBJECT_INSTANCE *obj)
{
	obj->GetCountdown()->Update();
	obj->GetCountdown()->AddCountdown(-1);
}

void UPDATE_MANAGER::UpdateSceneGame(GAME_OBJECT_INSTANCE *obj)
{
	// map�X�V
	UpdateMap(obj);
	//3D���
	Update3D(obj);
	//�����蔻��
	CollisionCheck(obj);
	//2d��ʏ�
	Update2D(obj);
}

void UPDATE_MANAGER::UpdateSceneResult(GAME_OBJECT_INSTANCE *obj)
{
	//���U���g�X�V
	obj->GetResult()->Update();
}

void UPDATE_MANAGER::UpdateSceneNetMatch(GAME_OBJECT_INSTANCE *obj)
{
	//�}�b�`���O��ʍX�V
	obj->GetNetMatch()->Update();
}

void UPDATE_MANAGER::UpdateSceneNetCountDown(GAME_OBJECT_INSTANCE *obj)
{
	obj->GetCountdown()->Update();
	obj->GetCountdown()->AddCountdown(-1);
}

void UPDATE_MANAGER::UpdateSceneNetGame(GAME_OBJECT_INSTANCE *obj)
{
	// map�X�V
	UpdateMap(obj);
	//3D���
	Update3D(obj);
	//�����蔻��
	CollisionCheck(obj);
	//2d��ʏ�
	Update2D(obj);
}

void UPDATE_MANAGER::UpdateMap(GAME_OBJECT_INSTANCE *obj)
{
	// map�X�V
	obj->GetField()->Update();
	obj->GetSky()->Update();
}

void UPDATE_MANAGER::Update3D(GAME_OBJECT_INSTANCE *obj)
{
	//3D���
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
	//2d��ʏ�
	obj->GetDamege()->Update();
	obj->GetStatus()->Update();
	obj->GetVitalGauge()->Update();
	obj->GetBulletGauge()->Update();
}




