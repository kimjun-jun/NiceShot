/** 
* @file damege.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../h/Object/ObjectClass/Interface/interface.h"
#include "../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//�O�u�錾
class MySOCKET;

/**
*�@@class DAMEGE_PARAMETER
*�@@brief 
*/
class DAMEGE_PARAMETER 
{
public:
	DAMEGE_PARAMETER() { alpha = 0.0f; time = 0.0f; }
	~DAMEGE_PARAMETER() {}

	float	time;		//!< �`�掞��
	float	alpha;		//!< �A���t�@�l
};

/**
*�@@class DAMEGE
*�@@brief GAMEOBJECT�h���N���X
*/
class DAMEGE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	DAMEGE();	//!< �f�[�^�ǂݍ��݁@������
	~DAMEGE();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override;			//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

	iUseCheck	iUseType[OBJECT_DAMEGE_MAX];			//!< �g�p���
	DAMEGE_PARAMETER	DamegePara[OBJECT_DAMEGE_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

private:
	TEXTURE		tex;	//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER	vtx;	//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM	Transform[OBJECT_DAMEGE_MAX];			//!< �g�����X�t�H�[�����

	//------���N���X�̃A�h���X
	MySOCKET *pmysocket;

	//------�`�惋�[�v�J�E���g�̃A�h���X
	DRAW_MANAGER *pDrawManager;

};

