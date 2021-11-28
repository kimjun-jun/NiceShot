/**
* @file Draw.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../h/main.h"
#include "../../h/Debug/debugproc.h"
#include "../../h/Other/input.h"
#include "../../h/Object/Scene/Scene.h"
#include "../../h/Object/Camera/camera.h"
#include "../../h/Other/sound.h"
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
#include "../../h/Net/sock.h"
#include "../../h/Object/ObjectClass/Instance/instance.h"
#include "../../h/Draw/Draw.h"

//インスタンス描画
void DRAW_MANAGER::DrawFunc(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	// バックバッファ＆Ｚバッファのクリア
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);
	// Direct3Dによる描画の開始
	if (SUCCEEDED(pD3DDevice->BeginScene()))
	{
		//毎回0で初期化
		this->DrawManagerNum = 0;
		// 画面遷移
		switch (obj->GetScene()->ScenePara.eNowScene)
		{
		case SCENE_TITLE:
			DrawSceneTitle(obj);
			break;
		case SCENE_TUTORIAL:
			DrawSceneTutorial(obj);
			break;
		case SCENE_GAMECOUNTDOWN:
			DrawSceneCountDown(obj);
			break;
		case SCENE_GAME:
			DrawSceneGame(obj);
			break;
		case SCENE_NETMATCH:
			DrawSceneNetMatch(obj);
			break;
		case SCENE_NETGAMECOUNTDOWN:
			this->DrawManagerNum = obj->GetMySocket()->GetNetMyNumber();	//ネット時はカウントをネット管理番号にする
			DrawSceneNetCountDown(obj);
			break;
		case SCENE_NETGAME:
			this->DrawManagerNum = obj->GetMySocket()->GetNetMyNumber();	//ネット時はカウントをネット管理番号にする
			DrawSceneNetGame(obj);
			break;
		case SCENE_RESULT:
			DrawSceneResult(obj);
			break;
		}
		// シーン切り替え時にフェード描画
		obj->GetScene()->Draw();

#ifdef _DEBUG
		// デバッグ表示
		DrawTextType();
		DrawDebugProc();
#endif
	}
	// Direct3Dによる描画の終了
	pD3DDevice->EndScene();
	// バックバッファとフロントバッファの入れ替え
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
	//スレッド間の共有変数を解除
	//m.unlock();
	//サブスレッドの更新ロックを解除する
	obj->GetMySocket()->SetNetShareDateFlag(false);
}

void DRAW_MANAGER::DrawSceneTitle(GAME_OBJECT_INSTANCE *obj)
{
	//タイトル描画
	obj->GetTitle()->Draw();
}

void DRAW_MANAGER::DrawSceneTutorial(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	for (int vpCnt = sizeof(vp) / sizeof(vp[0]); this->DrawManagerNum < vpCnt; this->DrawManagerNum++)
	{
		pD3DDevice->SetViewport(&vp[this->DrawManagerNum]);
		pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

		if (obj->GetPlayer()->PlayerPara[this->DrawManagerNum].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
		else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

		// カメラの設定
		SetCamera(this->DrawManagerNum);
		// map描画
		DrawMap(obj);
		//3D空間
		Draw3D(obj);
		//2d画面上
		Draw2D(obj);
		obj->GetTuto()->Draw();
	}
	pD3DDevice->SetViewport(&VpMaster);
}

void DRAW_MANAGER::DrawSceneCountDown(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	for (int vpCnt = sizeof(vp) / sizeof(vp[0]); this->DrawManagerNum < vpCnt; this->DrawManagerNum++)
	{
		pD3DDevice->SetViewport(&vp[this->DrawManagerNum]);
		pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);
		// カメラの設定
		SetCamera(this->DrawManagerNum);
		// map描画
		DrawMap(obj);
		//3D空間
		Draw3D(obj);
		//2d画面上
		Draw2D(obj);
	}
	pD3DDevice->SetViewport(&VpMaster);
	obj->GetCountdown()->Draw();
}

void DRAW_MANAGER::DrawSceneGame(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	for (int vpCnt = sizeof(vp) / sizeof(vp[0]); this->DrawManagerNum < vpCnt; this->DrawManagerNum++)
	{
		pD3DDevice->SetViewport(&vp[this->DrawManagerNum]);
		pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);
		bool puse = obj->GetPlayer()->iUseType[this->DrawManagerNum].Use();
		if (puse == true)
		{
			if (obj->GetPlayer()->PlayerPara[this->DrawManagerNum].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
			else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
			// カメラの設定
			SetCamera(this->DrawManagerNum);
			// map描画
			DrawMap(obj);
			//3D空間
			Draw3D(obj);
			//2d画面上
			Draw2D(obj);
		}
		else
		{
			pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
			// カメラの設定
			SetCamera(this->DrawManagerNum);
			// map描画
			DrawMap(obj);
			//3D空間
			Draw3D(obj);
			//2d画面上
			Draw2D(obj);
			obj->GetRank()->Draw();
		}
	}
	pD3DDevice->SetViewport(&VpMaster);

}

void DRAW_MANAGER::DrawSceneResult(GAME_OBJECT_INSTANCE *obj)
{
	//リザルト描画
	obj->GetResult()->Draw();
}

void DRAW_MANAGER::DrawSceneNetMatch(GAME_OBJECT_INSTANCE *obj)
{
	//マッチング画面描画
	obj->GetNetMatch()->Draw();
}

void DRAW_MANAGER::DrawSceneNetCountDown(GAME_OBJECT_INSTANCE *obj)
{
	// カメラの設定
	SetCamera(obj->GetMySocket()->GetNetMyNumber());
	// map描画
	DrawMap(obj);
	//3D空間
	Draw3D(obj);
	//2d画面上
	Draw2D(obj);
	obj->GetCountdown()->Draw();
}

void DRAW_MANAGER::DrawSceneNetGame(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	bool puseNet = obj->GetPlayer()->iUseType[obj->GetMySocket()->GetNetMyNumber()].Use();
	if (puseNet == true)
	{
		if (obj->GetPlayer()->PlayerPara[obj->GetMySocket()->GetNetMyNumber()].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
		else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
		// カメラの設定
		SetCamera(obj->GetMySocket()->GetNetMyNumber());
		// map描画
		DrawMap(obj);
		//3D空間
		Draw3D(obj);
		//2d画面上
		Draw2D(obj);
	}
	else
	{
		pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
		// カメラの設定
		SetCamera(obj->GetMySocket()->GetNetMyNumber());
		// map描画
		DrawMap(obj);
		//3D空間
		Draw3D(obj);
		//2d画面上
		Draw2D(obj);
		obj->GetRank()->Draw();
	}
}

void DRAW_MANAGER::DrawMap(GAME_OBJECT_INSTANCE *obj)
{
	// map描画
	obj->GetField()->Draw();
	obj->GetSky()->Draw();
	obj->GetWall()->Draw();
}

void DRAW_MANAGER::Draw3D(GAME_OBJECT_INSTANCE *obj)
{
	//3D空間
	obj->GetPlayer()->Draw();
	obj->GetItem()->Draw();
	obj->GetBulletPrediction()->Draw();
	obj->GetExplosion()->Draw();
	obj->GetEffect()->Draw();
	obj->GetShadow()->Draw();
}

void DRAW_MANAGER::Draw2D(GAME_OBJECT_INSTANCE *obj)
{
	//2d画面上
	obj->GetDamege()->Draw();
	obj->GetStatus()->Draw();
	obj->GetVitalGauge()->Draw();
	obj->GetBulletGauge()->Draw();
}


