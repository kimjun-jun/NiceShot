/**
* @file netmatch.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
*�@@class NETMATCH
*�@@brief GAME_OBJECT_INTERFACE_SUMMRY�h���N���X
*/
class NETMATCH : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	NETMATCH();		//!< �f�[�^�ǂݍ��݁@������
	~NETMATCH();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��
private:

	TEXTURE	tex;			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER vtx;			//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform;	//!< �g�����X�t�H�[�����@�e�v���C���[�̐� * �`���[�g���A���摜���쐬
};

