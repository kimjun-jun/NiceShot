/**
* @file Draw.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
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

//�C���X�^���X�`��
void DRAW_MANAGER::DrawFunc(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);
	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pD3DDevice->BeginScene()))
	{
		//����0�ŏ�����
		this->DrawManagerNum = 0;
		// ��ʑJ��
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
			this->DrawManagerNum = obj->GetMySocket()->GetNetMyNumber();	//�l�b�g���̓J�E���g���l�b�g�Ǘ��ԍ��ɂ���
			DrawSceneNetCountDown(obj);
			break;
		case SCENE_NETGAME:
			this->DrawManagerNum = obj->GetMySocket()->GetNetMyNumber();	//�l�b�g���̓J�E���g���l�b�g�Ǘ��ԍ��ɂ���
			DrawSceneNetGame(obj);
			break;
		case SCENE_RESULT:
			DrawSceneResult(obj);
			break;
		}
		// �V�[���؂�ւ����Ƀt�F�[�h�`��
		obj->GetScene()->Draw();

#ifdef _DEBUG
		// �f�o�b�O�\��
		DrawTextType();
		DrawDebugProc();
#endif
	}
	// Direct3D�ɂ��`��̏I��
	pD3DDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
	//�X���b�h�Ԃ̋��L�ϐ�������
	//m.unlock();
	//�T�u�X���b�h�̍X�V���b�N����������
	obj->GetMySocket()->SetNetShareDateFlag(false);
}

void DRAW_MANAGER::DrawSceneTitle(GAME_OBJECT_INSTANCE *obj)
{
	//�^�C�g���`��
	obj->GetTitle()->Draw();
}

void DRAW_MANAGER::DrawSceneTutorial(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	for (int vpCnt = sizeof(vp) / sizeof(vp[0]); this->DrawManagerNum < vpCnt; this->DrawManagerNum++)
	{
		pD3DDevice->SetViewport(&vp[this->DrawManagerNum]);
		pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

		if (obj->GetPlayer()->PlayerPara[this->DrawManagerNum].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
		else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

		// �J�����̐ݒ�
		SetCamera(this->DrawManagerNum);
		// map�`��
		DrawMap(obj);
		//3D���
		Draw3D(obj);
		//2d��ʏ�
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
		// �J�����̐ݒ�
		SetCamera(this->DrawManagerNum);
		// map�`��
		DrawMap(obj);
		//3D���
		Draw3D(obj);
		//2d��ʏ�
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
			if (obj->GetPlayer()->PlayerPara[this->DrawManagerNum].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
			else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
			// �J�����̐ݒ�
			SetCamera(this->DrawManagerNum);
			// map�`��
			DrawMap(obj);
			//3D���
			Draw3D(obj);
			//2d��ʏ�
			Draw2D(obj);
		}
		else
		{
			pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
			// �J�����̐ݒ�
			SetCamera(this->DrawManagerNum);
			// map�`��
			DrawMap(obj);
			//3D���
			Draw3D(obj);
			//2d��ʏ�
			Draw2D(obj);
			obj->GetRank()->Draw();
		}
	}
	pD3DDevice->SetViewport(&VpMaster);

}

void DRAW_MANAGER::DrawSceneResult(GAME_OBJECT_INSTANCE *obj)
{
	//���U���g�`��
	obj->GetResult()->Draw();
}

void DRAW_MANAGER::DrawSceneNetMatch(GAME_OBJECT_INSTANCE *obj)
{
	//�}�b�`���O��ʕ`��
	obj->GetNetMatch()->Draw();
}

void DRAW_MANAGER::DrawSceneNetCountDown(GAME_OBJECT_INSTANCE *obj)
{
	// �J�����̐ݒ�
	SetCamera(obj->GetMySocket()->GetNetMyNumber());
	// map�`��
	DrawMap(obj);
	//3D���
	Draw3D(obj);
	//2d��ʏ�
	Draw2D(obj);
	obj->GetCountdown()->Draw();
}

void DRAW_MANAGER::DrawSceneNetGame(GAME_OBJECT_INSTANCE *obj)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	bool puseNet = obj->GetPlayer()->iUseType[obj->GetMySocket()->GetNetMyNumber()].Use();
	if (puseNet == true)
	{
		if (obj->GetPlayer()->PlayerPara[obj->GetMySocket()->GetNetMyNumber()].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
		else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
		// �J�����̐ݒ�
		SetCamera(obj->GetMySocket()->GetNetMyNumber());
		// map�`��
		DrawMap(obj);
		//3D���
		Draw3D(obj);
		//2d��ʏ�
		Draw2D(obj);
	}
	else
	{
		pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
		// �J�����̐ݒ�
		SetCamera(obj->GetMySocket()->GetNetMyNumber());
		// map�`��
		DrawMap(obj);
		//3D���
		Draw3D(obj);
		//2d��ʏ�
		Draw2D(obj);
		obj->GetRank()->Draw();
	}
}

void DRAW_MANAGER::DrawMap(GAME_OBJECT_INSTANCE *obj)
{
	// map�`��
	obj->GetField()->Draw();
	obj->GetSky()->Draw();
	obj->GetWall()->Draw();
}

void DRAW_MANAGER::Draw3D(GAME_OBJECT_INSTANCE *obj)
{
	//3D���
	obj->GetPlayer()->Draw();
	obj->GetItem()->Draw();
	obj->GetBulletPrediction()->Draw();
	obj->GetExplosion()->Draw();
	obj->GetEffect()->Draw();
	obj->GetShadow()->Draw();
}

void DRAW_MANAGER::Draw2D(GAME_OBJECT_INSTANCE *obj)
{
	//2d��ʏ�
	obj->GetDamege()->Draw();
	obj->GetStatus()->Draw();
	obj->GetVitalGauge()->Draw();
	obj->GetBulletGauge()->Draw();
}


