/**
* @file result.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

enum eRESULT_TEX_TYPE
{
	RESULT_TEX_BG,
	RESULT_TEX_LOGO,
	RESULT_TEX_MAX,
};

/**
*�@@class RESULT
*�@@brief GAMEOBJECT�h���N���X
*/
class RESULT : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	RESULT();		//!< �f�[�^�ǂݍ��݁@������
	~RESULT();		//!< �폜
	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

private:
	TEXTURE tex[RESULT_TEX_MAX];		//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER vtx;						//!< ���_���@�����g�p����Ȃ炱����z��

	//------���N���X�̃A�h���X
	SCENE *pscene;
	GAME_OBJECT_INSTANCE *pGameObjInstance; //�e�N���X�̃A�h���X�Ȃ̂ł���܂�ǂ��Ȃ�

	const char *c_aFileNameTex[OBJECT_RESULT_MAX] =
	{
		"../data/TEXTURE/other/GameClear_Background.png",	//!< ���U���g�w�i
		"../data/TEXTURE/other/GameClear.png"				//!< ���U���g���S
	};

};
