/**
* @file bullet.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#include "../../../h/object/objectclass.h"


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
class BULLET : public GAME_OBJECT
{
public:
	BULLET();		//!< �f�[�^�ǂݍ��݁@������
	~BULLET();		//!< �폜

	void	Init(void);						//!< ������
	void	Update(SHADOW *s, EFFECT *e);	//!< �X�V
	void	Draw(void);						//!< �`��

	TransForm	Transform[OBJECT_BULLET_MAX];		//!< �g�����X�t�H�[�����
	iUseCheak	iUseType[OBJECT_BULLET_MAX];		//!< �g�p���
	BULLET_PARAMETER	BulletPara[OBJECT_BULLET_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	int		SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, ePLAYER_TYPE type, SHADOW *s);	//!< �C���X�^���X�Z�b�g
	void	ReleaseInstance(int nIdxBullet, SHADOW *s);	//!< �C���X�^���X���

private:

	Movement	move[OBJECT_BULLET_MAX];			//!< �ړ���

} ;


