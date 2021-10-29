/**
* @file damege.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
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
	DAMEGE_PARAMETER() { alpha = 0.0f; time = 0.0f; }
	~DAMEGE_PARAMETER() {}

	float	time;		//!< �`�掞��
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
	void	InitNet(int MyNumber);	//!< ������
	void	Update(void);		//!< �X�V
	void	Draw(bool Netflag, int NetMyNumber, int CntPlayer);			//!< �`��
	iUseCheak	iUseType[OBJECT_DAMEGE_MAX];			//!< �g�p���
	DAMEGE_PARAMETER	DamegePara[OBJECT_DAMEGE_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

private:


	TEXTURE		tex;	//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;	//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_DAMEGE_MAX];			//!< �g�����X�t�H�[�����

};

