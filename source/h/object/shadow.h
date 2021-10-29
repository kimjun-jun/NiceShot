/**
* @file shadow.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
/**
*�@@class SHADOW
*�@@brief GAMEOBJECT�h���N���X
*/
class SHADOW : public GAME_OBJECT
{
public:
	SHADOW();	//!< �f�[�^�ǂݍ��݁@������
	~SHADOW();	//!< �폜

	void		Init(void);		//!< ������
	void		Update(void);	//!< �X�V
	void		Draw(void);		//!< �`��

	int			SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl);	//!< �C���X�^���X�Z�b�g
	void		ReleaseInstance(int Idx);				//!< �C���X�^���X���
	void		UpdateInstance(int Idx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);	//�C���X�^���X�A�b�v�f�[�g�@�e�C���X�^���X���ɔC�ӌĂяo��

private:

	TEXTURE		tex;			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;			//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_SHADOW_MAX];		//!< �g�����X�t�H�[�����
	iUseCheak	iUseType[OBJECT_SHADOW_MAX];		//!< �g�p���

} ;

