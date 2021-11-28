/**
* @file bullet.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//*****************************************************************************
// class��`
//*****************************************************************************
/**
*�@@class BULLET_PARAMETER
*�@@brief 
*/
class BULLET_PARAMETER
{
public:
	BULLET_PARAMETER() { Timer = IdxShadow = 0; Gravity = FieldPosY = 0.0f; UsePlayerType = PLAYER_NONE; }
	~BULLET_PARAMETER() {}

	int		Timer;							// �^�C�}�[
	int		IdxShadow;						// �eID
	float	Gravity;						// �d��
	float	FieldPosY;						// �e�p�̌��ݒn�̒n�`POSY���L��
	ePLAYER_TYPE	UsePlayerType;			// ���v���C���[���g�p���Ă邩���ʂ���

};

/**
*�@@class BULLET
*�@@brief GAMEOBJECT�h���N���X
*/
class BULLET : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	BULLET();		//!< �f�[�^�ǂݍ��݁@������
	~BULLET();		//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

	TRANSFORM Transform[OBJECT_BULLET_MAX];		//!< �g�����X�t�H�[�����
	iUseCheck iUseType[OBJECT_BULLET_MAX];		//!< �g�p���
	BULLET_PARAMETER BulletPara[OBJECT_BULLET_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	int SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, ePLAYER_TYPE type);	//!< �C���X�^���X�Z�b�g
	void ReleaseInstance(int nIdxBullet);	//!< �C���X�^���X���

private:

	MOVEMENT	move[OBJECT_BULLET_MAX];			//!< �ړ���

	//------���N���X�̃A�h���X
	SHADOW *pshadow;
	EFFECT *peffect;
} ;


