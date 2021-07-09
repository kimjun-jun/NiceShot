/**
* @file bulletprediction.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../../h/object/objectclass.h"

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

private:
	void		Init(void);						//!< ������
	void		Update(PLAYER_HONTAI *player);	//!< �X�V
	void		Draw(PLAYER_HONTAI *player, int CntPlayer);		//!< �`��

	void		UpdateInstance(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY, int CntBulletPrediction);

	TEXTURE		tex;				//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx[OBJECT_PLAYER_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_PLAYER_MAX][OBJECT_BULLETPREDICTION_MAX];		//!< �g�����X�t�H�[�����

};


