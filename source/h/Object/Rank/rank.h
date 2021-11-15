/**
* @file rank.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once


#include "../../../h/Object/ObjectClass/objectclass.h"

constexpr int	RANK_COUNTDOWN_NUM{ 2 };					//!< ���ʂ̐��@4�l�Ȃ̂�2,1,0,���g�� �e�N�X�`���z��0-2�Ȃ̂ōő�l��2

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@struct RANK_PARAMETER_ALL
*�@@brief
*/
class RANK_PARAMETER_ALL
{
public:
	RANK_PARAMETER_ALL() { RankNum = RANK_COUNTDOWN_NUM; }
	~RANK_PARAMETER_ALL() {}
	int			RankNum;			//!< �����̏���	4�l������@2,1,0�ƃJ�E���g�_�E�������Ă���
};

/**
*�@@struct RANK_PARAMETER_ONE
*�@@brief
*/
class RANK_PARAMETER_ONE
{
public:
	RANK_PARAMETER_ONE() { NetUse = false; }
	~RANK_PARAMETER_ONE() {}
	bool		NetUse;				//!< �l�b�g�ΐ펞�Ɏ����̏��ʂ��m�肵��������
};

/**
*�@@class RANK
*�@@brief GAMEOBJECT�h���N���X
*/
class RANK : public GAME_OBJECT
{
public:
	RANK();		//!< �f�[�^�ǂݍ��݁@������
	~RANK();	//!< �폜

	void		Init(void);								//!< ������
	void		InitNet(int NetMyNumber);				//!< ������
	void		Update(void);							//!< �X�V
	void		Draw(bool Netflag, int NetMyNumber);	//!< �`��

	void		SetRank(int PlayerNum);
	void		SetRankNet(int PlayerNum, int NetMyNumber);

private:
	TEXTURE		tex[OBJECT_RANK_MAX-1];			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@������ƃP�`����-1�@��ʂ̕��͂Ȃ�
	VTXBuffer	vtx;							//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm	Transform[OBJECT_RANK_MAX];		//!< �g�����X�t�H�[�����
	iUseCheak	iUseType[OBJECT_RANK_MAX];		//!< �g�p���

	RANK_PARAMETER_ALL	RankParaAll[OBJECT_RANK_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q
	RANK_PARAMETER_ONE	RankParaOne;					//!< �}�l�[�W���[�ɕK�v�ȃf�[�^�Q

	//������ƃP�`����-1�@��ʂ̕��͂Ȃ�
	const char *c_aFileNameTex[OBJECT_RANK_MAX - 1] =
	{
		"../data/TEXTURE/rank/2i.png",	//!< ���
		"../data/TEXTURE/rank/3i.png",	//!< �O��
		"../data/TEXTURE/rank/4i.png",	//!< �l��
	};
};