/**
* @file shadow.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
/**
*�@@class SHADOW
*�@@brief GAMEOBJECT�h���N���X
*/
class SHADOW : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	SHADOW();	//!< �f�[�^�ǂݍ��݁@������
	~SHADOW();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

	int	SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl);	//!< �C���X�^���X�Z�b�g
	void ReleaseInstance(int Idx);						//!< �C���X�^���X���
	void UpdateInstance(int Idx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);	//�C���X�^���X�A�b�v�f�[�g�@�e�C���X�^���X���ɔC�ӌĂяo��

private:

	TEXTURE	tex;	//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER vtx;	//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform[OBJECT_SHADOW_MAX];		//!< �g�����X�t�H�[�����
	iUseCheck iUseType[OBJECT_SHADOW_MAX];		//!< �g�p���

} ;

