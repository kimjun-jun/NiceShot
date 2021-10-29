/**
* @file status.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class STATUS
*�@@brief GAMEOBJECT�h���N���X�@
*/
class STATUS : public GAME_OBJECT
{
public:
	STATUS();		//!< �f�[�^�ǂݍ��݁@������
	~STATUS();		//!< �폜

	void		Init(void);				//!< ������
	void		InitNet(int MyNumber);	//!< ������
	void		Update(PLAYER *p);		//!< �X�V
	void		Draw(bool Netflag, int NetMyNumber, int CntPlayer);	//!< �`��

private:

	TEXTURE		tex[OBJECT_STATUS_MAX];			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@
	VTXBuffer	vtx[OBJECT_PLAYER_MAX];			//!< ���_���@�z��\����[P1tex1,P1tex2,P1tex3,P1tex4,P2tex1,P2tex2,,,]�\��
	TransForm	Transform[OBJECT_PLAYER_MAX];	//!< �g�����X�t�H�[�����@�e�v���C���[�p�̃X�N���[���ɕ`��
	iUseCheak	iUseType[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< �g�p���

	float		PtternV[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< �擾����͑S���`�悷�邪�A���Ԃ��o�߂���ɂ�ăe�N�X�`�������X�ɏォ������Ă����@�\����\���������Őݒ�ł���
	eSTATUS_TYPE e;
	const char *c_FileNameTex[OBJECT_STATUS_MAX] =
	{
		"../data/TEXTURE/UI/UI_Skill_Power.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Speed.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Flash.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Blind.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
	};

};
