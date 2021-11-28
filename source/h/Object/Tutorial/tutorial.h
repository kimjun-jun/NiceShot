/**
* @file tutorial.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
*�@@class TUTO
*�@@brief GAMEOBJECT�h���N���X
*/
class TUTO : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	TUTO();		//!< �f�[�^�ǂݍ��݁@������
	~TUTO();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

private:

	TEXTURE	tex;	//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER vtx;	//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform[OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX];		//!< �g�����X�t�H�[�����@�e�v���C���[�̐� * �`���[�g���A���摜���쐬
	iUseCheck iUseType[OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX];		//!< �g�p���@�e�v���C���[�̐����쐬 * �`���[�g���A���摜���쐬

	//------���N���X�̃A�h���X
	SCENE *pscene;
	GAME_OBJECT_INSTANCE *pGameObjInstance; //�e�N���X�̃A�h���X�Ȃ̂ł���܂�ǂ��Ȃ�

};

