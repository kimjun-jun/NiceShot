/**
* @file effect.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

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
class EFFECT : public GAME_OBJECT
{
public:
	EFFECT();	//!< �f�[�^�ǂݍ��݁@������
	~EFFECT();	//!< �폜

	void		Init(void);					//!< ������
	void		Update(void);				//!< �X�V
	void		Draw(int CntPlayer);		//!< �`��

	//�C���X�^���X�쐬�Ɖ���@������Ɋւ��Ă̓^�C�}�[�Ŏ������g�p�ɂȂ�̂ŗp�ӂ��Ȃ�
	int		SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);	//!< �C���X�^���X�Z�b�g

private:


	TEXTURE		tex;				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;				//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_EFFECT_MAX];		//!< �g�����X�t�H�[�����
	iUseCheak	iUseType[OBJECT_EFFECT_MAX];		//!< �g�p���
	Movement	move[OBJECT_EFFECT_MAX];			//!< �ړ���

	EFFECT_PARAMETER EffectPara[OBJECT_EFFECT_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

};

