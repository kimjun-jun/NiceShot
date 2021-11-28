/**
* @file bulletprediction.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class BULLETPREDICTION
*�@@brief GAMEOBJECT�h���N���X
*/
class BULLETPREDICTION : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	BULLETPREDICTION();		//!< �f�[�^�ǂݍ��݁@������
	~BULLETPREDICTION();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

private:
	TEXTURE	tex;				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER vtx[OBJECT_PLAYER_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform[OBJECT_PLAYER_MAX][OBJECT_BULLETPREDICTION_MAX/ OBJECT_PLAYER_MAX];		//!< �g�����X�t�H�[�����

	//------���N���X�̃A�h���X
	PLAYER *pplayer;
	//------�`�惋�[�v�J�E���g�̃A�h���X
	DRAW_MANAGER *pDrawManager;

	void UpdateInstance(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY, int CntBulletPrediction);

};


