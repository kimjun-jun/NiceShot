/**
* @file sky.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

enum eSKY_MODEL_TYPE
{
	SKY_MODEL_TYPE_SPHERE = 0,
	SKY_MODEL_TYPE_TOP,
	SKY_MODEL_TYPE_MAX,
};

//*****************************************************************************
// class��`
//*****************************************************************************
/**
*�@@class SKY_PARAMETER
*�@@brief 
*/
class SKY_PARAMETER
{
public:
	SKY_PARAMETER() { time = Addtime = fRotY = 0.0f; nNumBlockH = nNumBlockV = 0; }
	~SKY_PARAMETER() {}

	float time;					// ��̎��ԁ@���Ԃɂ���ċ�̐F���ς��@���g�p
	float Addtime;				// ��̎��ԁ@���Ԃɂ���ċ�̐F���ς��@���g�p
	float fRotY;				// ��]��
	int nNumBlockH;
	int nNumBlockV;
};

/**
*�@@class SKY
*�@@brief GAMEOBJECT�h���N���X
*/
class SKY : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	SKY();		//!< �f�[�^�ǂݍ��݁@������
	~SKY();		//!< �폜
	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

private:
	MODELATTRIBUTE Attribute[OBJECT_SKY_MAX];	//!< ���f�����@�}�e���A���Ⓒ�_���Ȃǁ@�����g�p����Ȃ炱����z��
	TEXTURE	tex;								//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	VTXBUFFER vtx[OBJECT_SKY_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform[OBJECT_SKY_MAX];		//!< �g�����X�t�H�[�����
	SKY_PARAMETER SkyPara[OBJECT_SKY_MAX];

	void SetUpMeshSphere(void);		// ����SKY�Z�b�g�A�b�v����
	void SetUpMeshTop(void);		// �㕔SKY�Z�b�g�A�b�v����
	void ResetColor(void);			//�J���[�������@���̃��f���Ə㕔���f��

};
