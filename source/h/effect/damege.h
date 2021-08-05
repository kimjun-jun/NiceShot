/**
* @file damege.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*�@@class DAMEGE_PARAMETER
*�@@brief 
*/
class DAMEGE_PARAMETER 
{
public:
	DAMEGE_PARAMETER() { alpha = 0; time = 0; }
	~DAMEGE_PARAMETER() {}

	int		time;		//!< �`�掞��
	float	alpha;		//!< �A���t�@�l
};

/**
*�@@class DAMEGE
*�@@brief GAMEOBJECT�h���N���X
*/
class DAMEGE : public GAME_OBJECT
{
public:
	DAMEGE();	//!< �f�[�^�ǂݍ��݁@������
	~DAMEGE();	//!< �폜

	void	Init(void);			//!< ������
	void	Update(void);		//!< �X�V
	void	Draw(bool Netflag, int NetMyNumber, int CntPlayer);			//!< �`��
	iUseCheak	iUseType[OBJECT_DAMEGE_MAX];			//!< �g�p���
	DAMEGE_PARAMETER	DamegePara[OBJECT_DAMEGE_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

private:

	void	ReinitNet(void);	//!< �ď�����

	TEXTURE		tex;	//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;	//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_DAMEGE_MAX];			//!< �g�����X�t�H�[�����

};

