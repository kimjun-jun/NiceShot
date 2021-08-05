/**
* @file wall.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"


/**
*�@@class WALL
*�@@brief GAMEOBJECT�h���N���X
*/
class WALL : public GAME_OBJECT
{
public:
	WALL();		//!< �f�[�^�ǂݍ��݁@������
	~WALL();	//!< �폜
	void		Init(void)override;		//!< ������
	void		Update(void)override;	//!< �X�V
	void		Draw(void)override;		//!< �`��
	TransForm				Transform[OBJECT_WALL_MAX];			//!< �g�����X�t�H�[�����

private:
	TEXTURE					tex;								//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	VTXBuffer				vtx[OBJECT_WALL_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��

	void SetUpMesh(void);			// WALL�Z�b�g�A�b�v����

} ;
