/** 
* @file effect.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../h/Object/ObjectClass/Interface/interface.h"
#include "../../h/Object/ObjectClass/Instance/instance.h"
#include "../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"
#include "../../h/Object/ObjectClass/StandardComponent/Move/Move.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class EFFECT_PARAMETER
*�@@brief 
*/
class EFFECT_PARAMETER
{
public:
	EFFECT_PARAMETER() { nTimer = 0; nDecAlpha = 1.0f; }
	~EFFECT_PARAMETER() {}
	int			nTimer;				//!< �^�C�}�[
	float		nDecAlpha;			//!< �����l
};

/**
*�@@class EFFECT
*�@@brief GAMEOBJECT�h���N���X
*/
class EFFECT : public GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	EFFECT();	//!< �f�[�^�ǂݍ��݁@������
	~EFFECT();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

	//�C���X�^���X�쐬�Ɖ���@������Ɋւ��Ă̓^�C�}�[�Ŏ������g�p�ɂȂ�̂ŗp�ӂ��Ȃ�
	int		SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);	//!< �C���X�^���X�Z�b�g

private:
	TEXTURE		tex;				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER	vtx;				//!< ���_���@�����g�p����Ȃ炱����z��

	TRANSFORM	Transform[OBJECT_EFFECT_MAX];		//!< �g�����X�t�H�[�����
	iUseCheck	iUseType[OBJECT_EFFECT_MAX];		//!< �g�p���
	MOVE	move[OBJECT_EFFECT_MAX];			//!< �ړ���

	EFFECT_PARAMETER EffectPara[OBJECT_EFFECT_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	//------�`�惋�[�v�J�E���g�̃A�h���X
	DRAW_MANAGER *pDrawManager;

};

