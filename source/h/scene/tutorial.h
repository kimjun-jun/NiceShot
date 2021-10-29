/**
* @file tutorial.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
*�@@class TUTO
*�@@brief GAMEOBJECT�h���N���X
*/
class TUTO : public GAME_OBJECT
{
public:
	TUTO();		//!< �f�[�^�ǂݍ��݁@������
	~TUTO();	//!< �폜

	void	Init(void);								//!< ������
	void	Update(GAME_OBJECT* obj, FADE *fade);	//!< �X�V
	void	Draw(void);								//!< �`��

private:

	TEXTURE		tex;								//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;								//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX];		//!< �g�����X�t�H�[�����@�e�v���C���[�̐� * �`���[�g���A���摜���쐬
	iUseCheak	iUseType[OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX];		//!< �g�p���@�e�v���C���[�̐����쐬 * �`���[�g���A���摜���쐬
};

