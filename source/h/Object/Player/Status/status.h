/**
* @file status.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
*�@@class STATUS
*�@@brief GAMEOBJECT�h���N���X�@
*/
class STATUS : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	STATUS();		//!< �f�[�^�ǂݍ��݁@������
	~STATUS();		//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override;			//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��


private:
	TEXTURE tex[OBJECT_STATUS_MAX];			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@
	VTXBUFFER vtx[OBJECT_PLAYER_MAX];		//!< ���_���@�z��\����[P1tex1,P1tex2,P1tex3,P1tex4,P2tex1,P2tex2,,,]�\��
	TRANSFORM Transform[OBJECT_PLAYER_MAX];	//!< �g�����X�t�H�[�����@�e�v���C���[�p�̃X�N���[���ɕ`��
	iUseCheck iUseType[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< �g�p���

	//------���N���X�̃A�h���X
	PLAYER *pplayer;
	MySOCKET *pmysocket;
	//------�`�惋�[�v�J�E���g�̃A�h���X
	DRAW_MANAGER *pDrawManager;

	float PtternV[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< �擾����͑S���`�悷�邪�A���Ԃ��o�߂���ɂ�ăe�N�X�`�������X�ɏォ������Ă����@�\����\���������Őݒ�ł���
	//eSTATUS_TYPE e;
	const char *c_FileNameTex[OBJECT_STATUS_MAX] =
	{
		"../data/TEXTURE/UI/UI_Skill_Power.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Speed.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Flash.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
		"../data/TEXTURE/UI/UI_Skill_Blind.png",		// �ǂݍ��ރe�N�X�`���t�@�C����
	};

};
