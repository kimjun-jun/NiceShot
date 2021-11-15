/**
* @file bulletprediction.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#include "../../../h/Object/ObjectClass/objectclass.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class BULLETPREDICTION
*�@@brief GAMEOBJECT�h���N���X
*/
class BULLETPREDICTION : public GAME_OBJECT
{
public:
	BULLETPREDICTION();		//!< �f�[�^�ǂݍ��݁@������
	~BULLETPREDICTION();	//!< �폜

	void		Init(void);						//!< ������
	void		Update(PLAYER *player);	//!< �X�V
	void		Draw(PLAYER *player, int CntPlayer);		//!< �`��

private:

	void		UpdateInstance(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY, int CntBulletPrediction);

	TEXTURE		tex;				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx[OBJECT_PLAYER_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_PLAYER_MAX][OBJECT_BULLETPREDICTION_MAX/ OBJECT_PLAYER_MAX];		//!< �g�����X�t�H�[�����

};

