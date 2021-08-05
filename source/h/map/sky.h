/**
* @file sky.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

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
class SKY : public GAME_OBJECT
{
public:
	SKY();		//!< �f�[�^�ǂݍ��݁@������
	~SKY();		//!< �폜
	void		Init(void)override;		//!< ������
	void		Update(void)override;	//!< �X�V
	void		Draw(void)override;		//!< �`��

private:
	ModelAttribute			Attribute[OBJECT_SKY_MAX];			//!< ���f�����@�}�e���A���Ⓒ�_���Ȃǁ@�����g�p����Ȃ炱����z��
	TEXTURE					tex;								//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	VTXBuffer				vtx[OBJECT_SKY_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm				Transform[OBJECT_SKY_MAX];			//!< �g�����X�t�H�[�����
	SKY_PARAMETER			SkyPara[OBJECT_SKY_MAX];

	void SetUpMeshSphere(void);			// ����SKY�Z�b�g�A�b�v����
	void SetUpMeshTop(void);			// �㕔SKY�Z�b�g�A�b�v����
	void ResetColor(void);				//�J���[�������@���̃��f���Ə㕔���f��

};
