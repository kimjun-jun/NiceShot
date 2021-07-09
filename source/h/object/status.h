/**
* @file status.h
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
*�@@class STATUS
*�@@brief GAMEOBJECT�h���N���X�@
*/
class STATUS : public GAME_OBJECT
{
public:
	STATUS();		//!< �f�[�^�ǂݍ��݁@������
	~STATUS();		//!< �폜

private:
	void		Init(void);						//!< ������
	void		Update(PLAYER_HONTAI *p);		//!< �X�V
	void		Draw(bool Netflag, int NetMyNumber, int CntPlayer);	//!< �`��

	void		ReinitNet(int MyNumber);		//!< �ď������l�b�g�ΐ�O	

	TEXTURE		tex[OBJECT_STATUS_MAX];			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@
	VTXBuffer	vtx[OBJECT_PLAYER_MAX];			//!< ���_���@�z��\����[P1tex1,P1tex2,P1tex3,P1tex4,P2tex1,P2tex2,,,]�\��
	TransForm	Transform[OBJECT_PLAYER_MAX];	//!< �g�����X�t�H�[�����@�e�v���C���[�p�̃X�N���[���ɕ`��
	float		PtternV[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< �擾����͑S���`�悷�邪�A���Ԃ��o�߂���ɂ�ăe�N�X�`�������X�ɏォ������Ă����@�\����\���������Őݒ�ł���
	eSTATUS_TYPE;
	const char *c_FileNameTex[OBJECT_STATUS_MAX] =
	{
		"../data/TEXTURE/UI/UI_Skill_Power.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Speed.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Flash.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Blind.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
	};

};
