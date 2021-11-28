/**
* @file instance.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../../h/Object/ObjectClass/Interface/interface.h"

//�e�N���X�̑O�u�錾
class PLAYER;
class EFFECT;
class BULLET;
class SHADOW;
class COUNTDOWN;
class NETMATCH;
class TUTO;
class STATUS;
class BULLETPREDICTION;
class VITALGAUGE;
class BULLETGAUGE;
class DAMEGE;
class EXPLOSION;
class ITEM;
class RANK;
class RESULT;
class TITLE;
class FIELD;
class SKY;
class WALL;
class SCENE;
class MySOCKET;

class UPDATE_MANAGER;
class DRAW_MANAGER;

/**
 * @class GAME_OBJECT_INSTANCE
 * �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 * �h���N���X�ŃI�[�o���C�h�K�{(�v���C���[��A�C�e���Ȃǂ̑S�I�u�W�F�N�g���p��)
 * GAME_OBJECT_INTERFACE_SUMMRY�̔h���N���X�Z��(�H)�ւ̃A�N�Z�b�T�[
 */
class GAME_OBJECT_INSTANCE :public GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	GAME_OBJECT_INSTANCE() {}
	virtual ~GAME_OBJECT_INSTANCE() {}

	virtual void Addressor(GAME_OBJECT_INSTANCE *obj) override;		//�A�h���b�T�[
	virtual void Init(void) override;			//������(���l�Ȃ�)
	virtual void InitNet(void) override;		//�������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	virtual void Update(void) override;			//�X�V
	virtual void Draw(void) override;			//�`��

	//�����Ɣj��
	void Generate(void);			//�������m�ہ@�f�[�^�ǂݍ��݁@������
	void Delete(void);				//���������

	//�}���`�X���b�h����
	void MySubThreadFunc(void);		//�T�u�X���b�h�֐��@�A�v���I���M�����󂯂�܂Ń��[�v����

	PLAYER *GetPlayer(void) const { return Player; };
	EFFECT *GetEffect(void) const { return Effect; };
	BULLET *GetBullet(void) const { return Bullet; };
	SHADOW *GetShadow(void) const { return Shadow; };
	COUNTDOWN *GetCountdown(void) const { return CountDown; };
	TUTO *GetTuto(void) const { return Tuto; };
	NETMATCH *GetNetMatch(void) const { return Netmatch; };
	STATUS *GetStatus(void) const { return Status; };
	BULLETPREDICTION *GetBulletPrediction(void) const { return BulletPrediction; };
	VITALGAUGE *GetVitalGauge(void) const { return VitalGauge; };
	BULLETGAUGE *GetBulletGauge(void) const { return BulletGauge; };
	DAMEGE *GetDamege(void) const { return Damege; };
	EXPLOSION *GetExplosion(void) const { return Explosion; };
	ITEM *GetItem(void) const { return Item; };
	RANK *GetRank(void) const { return Rank; };
	RESULT *GetResult(void) const { return Result; };
	TITLE *GetTitle(void) const { return Title; };
	FIELD *GetField(void) const { return Field; };
	SKY *GetSky(void) const { return Sky; };
	WALL *GetWall(void) const { return Wall; };
	SCENE *GetScene(void) const { return Scene; };
	MySOCKET *GetMySocket(void) const { return MySocket; };

	DRAW_MANAGER *GetDrawManager(void) { return this->DrawManager; };

private:
	//�I�u�W�F�N�g�ւ̃|�C���^�[
	PLAYER *Player = NULL;
	EFFECT *Effect = NULL;
	BULLET *Bullet = NULL;
	SHADOW *Shadow = NULL;
	COUNTDOWN *CountDown = NULL;
	TUTO *Tuto = NULL;
	NETMATCH *Netmatch = NULL;
	STATUS *Status = NULL;
	BULLETPREDICTION *BulletPrediction = NULL;
	VITALGAUGE *VitalGauge = NULL;
	BULLETGAUGE *BulletGauge = NULL;
	DAMEGE *Damege = NULL;
	EXPLOSION *Explosion = NULL;
	ITEM *Item = NULL;
	RANK *Rank = NULL;
	RESULT *Result = NULL;
	TITLE *Title = NULL;
	FIELD *Field = NULL;
	SKY *Sky = NULL;
	WALL *Wall = NULL;
	SCENE *Scene = NULL;
	MySOCKET *MySocket = NULL;

	//�}�l�[�W���ւ̃|�C���^�[
	UPDATE_MANAGER *UpdateManager = NULL;
	DRAW_MANAGER *DrawManager = NULL;
};
