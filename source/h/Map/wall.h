/**
* @file wall.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#include "../../../h/Object/ObjectClass/objectclass.h"


/**
*�@@class WALL
*�@@brief GAMEOBJECT�h���N���X
*/
class WALL : public GAME_OBJECT
{
public:
	WALL();		//!< �f�[�^�ǂݍ��݁@������
	~WALL();	//!< �폜
	void		Init(void);		//!< ������
	void		Update(void);	//!< �X�V
	void		Draw(void);		//!< �`��
	TransForm				Transform[OBJECT_WALL_MAX];			//!< �g�����X�t�H�[�����

private:
	TEXTURE					tex;								//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	VTXBuffer				vtx[OBJECT_WALL_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��

	void SetUpMesh(void);			// WALL�Z�b�g�A�b�v����

} ;
