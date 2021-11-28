/**
* @file wall.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
*�@@class WALL
*�@@brief GAMEOBJECT�h���N���X
*/
class WALL : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	WALL();		//!< �f�[�^�ǂݍ��݁@������
	~WALL();	//!< �폜
	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��
	TRANSFORM Transform[OBJECT_WALL_MAX];			//!< �g�����X�t�H�[�����

private:
	TEXTURE	tex;								//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	VTXBUFFER vtx[OBJECT_WALL_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��

	void SetUpMesh(void);			// WALL�Z�b�g�A�b�v����

} ;
