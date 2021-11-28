///**
//* @file instance.h
//* @brief NiceShot(3D)��ԃQ�[��
//* @author �L�����W����
//*/
//
#include "../../../../h/main.h"
#include "../../../../h/Debug/debugproc.h"
#include "../../../../h/Other/input.h"
#include "../../../../h/Object/Scene/Scene.h"
#include "../../../../h/Object/Camera/camera.h"
#include "../../../../h/Other/sound.h"
#include "../../../../h/Map/field.h"
#include "../../../../h/Map/wall.h"
#include "../../../../h/Map/sky.h"
#include "../../../../h/Object/Player/player.h"
#include "../../../../h/Object/Shadow/shadow.h"
#include "../../../../h/Object/Title/title.h"
#include "../../../../h/Object/Result/result.h"
#include "../../../../h/Object/Tutorial/tutorial.h"
#include "../../../../h/Effect/effect.h"
#include "../../../../h/Object/Bullet/bullet.h"
#include "../../../../h/Effect/explosion.h"
#include "../../../../h/Object/Rank/rank.h"
#include "../../../../h/Object/CountDown/countdown.h"
#include "../../../../h/Object/Item/item.h"
#include "../../../../h/Effect/damege.h"
#include "../../../../h/Object/Player/Status/status.h"
#include "../../../../h/Object/Bullet/bulletprediction.h"
#include "../../../../h/Object/Bullet/bulletgauge.h"
#include "../../../../h/Object/Player/VitalGauge/vitalgauge.h"
#include "../../../../h/net/netmatch.h"
#include "../../../../h/net/sock.h"
#include "../../../../h/Update/Update.h"
#include "../../../../h/Draw/Draw.h"
#include "../../../../h/Object/ObjectClass/Instance/instance.h"

//�C���X�^���X����
void GAME_OBJECT_INSTANCE::Generate(void)
{
	Player = new PLAYER;
	Effect = new EFFECT;
	Bullet = new BULLET;
	Shadow = new SHADOW;
	CountDown = new COUNTDOWN;
	Tuto = new TUTO;
	Netmatch = new NETMATCH;
	Status = new STATUS;
	BulletPrediction = new BULLETPREDICTION;
	VitalGauge = new VITALGAUGE;
	BulletGauge = new BULLETGAUGE;
	Damege = new DAMEGE;
	Explosion = new EXPLOSION;
	Item = new ITEM;
	Rank = new RANK;
	Result = new RESULT;
	Title = new TITLE;
	Field = new FIELD;
	Sky = new SKY;
	Wall = new WALL;
	Scene = new SCENE;
	MySocket = new MySOCKET;

	UpdateManager = new UPDATE_MANAGER;
	DrawManager = new DRAW_MANAGER;
}

//�C���X�^���X�폜
void GAME_OBJECT_INSTANCE::Delete(void)
{
	delete Scene;
	delete Player;
	delete Effect;
	delete Bullet;
	delete Shadow;
	delete CountDown;
	delete Tuto;
	delete Netmatch;
	delete Status;
	delete BulletPrediction;
	delete VitalGauge;
	delete BulletGauge;
	delete Damege;
	delete Explosion;
	delete Item;
	delete Rank;
	delete Result;
	delete Title;
	delete Field;
	delete Sky;
	delete Wall;
	delete MySocket;

	delete UpdateManager;
	delete DrawManager;
}

//�C���X�^���X�N���X�ԃA�h���X�擾
void GAME_OBJECT_INSTANCE::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	Scene->Addressor(obj);
	Player->Addressor(obj);
	Effect->Addressor(obj);
	Bullet->Addressor(obj);
	Shadow->Addressor(obj);
	CountDown->Addressor(obj);
	Tuto->Addressor(obj);
	Netmatch->Addressor(obj);
	Status->Addressor(obj);
	BulletPrediction->Addressor(obj);
	VitalGauge->Addressor(obj);
	BulletGauge->Addressor(obj);
	Damege->Addressor(obj);
	Explosion->Addressor(obj);
	Item->Addressor(obj);
	Rank->Addressor(obj);
	Result->Addressor(obj);
	Title->Addressor(obj);
	Field->Addressor(obj);
	Sky->Addressor(obj);
	Wall->Addressor(obj);
	MySocket->Addressor(obj);
}

//�f�[�^�ʐM����
void GAME_OBJECT_INSTANCE::MySubThreadFunc(void)
{
	while (GetEndGame() == false)
	{
		MySocket->Packet();
	}
}

//�C���X�^���X������
void GAME_OBJECT_INSTANCE::Init(void)
{
	//�������̏������̂ق����Z���Ȃ�@�������K�v�ȏꍇ�͗�O���������邩�@������K�v�Ƃ��Ȃ��\���ɂ���
	//for (int CntOBJ = 0; CntOBJ < this->AllOBJCnt; CntOBJ++) { this[CntOBJ].Init; }
	Scene->Init();
	Field->Init();
	Player->Init();
	Effect->Init();
	Bullet->Init();
	Shadow->Init();
	CountDown->Init();
	Tuto->Init();
	Status->Init();
	BulletPrediction->Init();
	VitalGauge->Init();
	BulletGauge->Init();
	Damege->Init();
	Explosion->Init();
	Item->Init();
	Rank->Init();
	Result->Init();
	Title->Init();
	Sky->Init();
	Wall->Init();
	MySocket->Init();
}

//�C���X�^���X�������@�l�b�g�ΐ펞�̂ݎg�p
void GAME_OBJECT_INSTANCE::InitNet(void)
{
	//�X�N���[����UI���Đݒ肵�Ă�
	Status->InitNet();
	VitalGauge->InitNet();
	BulletGauge->InitNet();
	Damege->InitNet();
	Rank->InitNet();
}

//�C���X�^���X�X�V
void GAME_OBJECT_INSTANCE::Update(void)
{
	UpdateManager->UpdateFunc(this);
}

//�C���X�^���X�`��
void GAME_OBJECT_INSTANCE::Draw(void)
{
	DrawManager->DrawFunc(this);
}

