/**
* @file netmatch.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
*�@@class TUTO
*�@@brief GAMEOBJECT�h���N���X
*/
class NETMATCH : public GAME_OBJECT
{
public:
	NETMATCH();		//!< �f�[�^�ǂݍ��݁@������
	~NETMATCH();	//!< �폜

	void	Init(void)override;								//!< ������
	void	Update(GAME_OBJECT* obj, FADE *fade);			//!< �X�V
	void	Draw(void)override;								//!< �`��

private:

	TEXTURE		tex;			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;			//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform;		//!< �g�����X�t�H�[�����@�e�v���C���[�̐� * �`���[�g���A���摜���쐬
};

