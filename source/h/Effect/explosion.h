/** 
* @file explosion.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/objectclass.h"

//�������
enum eEXPLOSION_TYPE
{
	EXPLOSIONTYPE_BULLET_NONE = -1,		//!< ������
	EXPLOSIONTYPE_BULLET_PLAYER1 = 0,	//!< �v���C���[1
	EXPLOSIONTYPE_BULLET_PLAYER2,		//!< �v���C���[2
	EXPLOSIONTYPE_BULLET_PLAYER3,		//!< �v���C���[3
	EXPLOSIONTYPE_BULLET_PLAYER4,		//!< �v���C���[4
	EXPLOSIONTYPE_BULLET_ENEMY1,		//!< �G�l�~�[1
	EXPLOSIONTYPE_BULLET_ENEMY2,		//!< �G�l�~�[2
	EXPLOSIONTYPE_BULLET_ENEMY3,		//!< �G�l�~�[3
	EXPLOSIONTYPE_BULLET_ENEMY4,		//!< �G�l�~�[4
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@class EXPLOSION_PARAMETER
*�@@brief 
*/
class EXPLOSION_PARAMETER
{
public:
	EXPLOSION_PARAMETER() { nCounter = nPatternX = nPatternY = 0; fSizeX = fSizeY = 0.0f; 
	eType = EXPLOSIONTYPE_BULLET_NONE;}
	~EXPLOSION_PARAMETER() {}

	int			nCounter;			//!< �J�E���^�[
	int			nPatternX;			//!< �p�^�[��No.X
	int			nPatternY;			//!< �p�^�[��No.Y
	float		fSizeX;				//!< ���_�T�C�YX
	float		fSizeY;				//!< ���_�T�C�YY
	eEXPLOSION_TYPE	eType;			//!< ���
};

/**
*�@@class EXPLOSION
*�@@brief GAMEOBJECT�h���N���X
*/
class EXPLOSION : public GAME_OBJECT
{
public:
	EXPLOSION();	//!< �f�[�^�ǂݍ��݁@������
	~EXPLOSION();	//!< �폜

	int	SetInstance(D3DXVECTOR3 pos, float fSizeX, float fSizeY, eEXPLOSION_TYPE eType, D3DXCOLOR col);	//!< �C���X�^���X�Z�b�g

	void		Init(void);					//!< ������
	void		Update(void);				//!< �X�V
	void		Draw(int CntPlayer);		//!< �`��

private:


	TEXTURE		tex;	//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBuffer	vtx;	//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_EXPLOSION_MAX];			//!< �g�����X�t�H�[�����
	iUseCheak	iUseType[OBJECT_EXPLOSION_MAX];			//!< �g�p���

	EXPLOSION_PARAMETER	ExploPara[OBJECT_EXPLOSION_MAX];

} ;


