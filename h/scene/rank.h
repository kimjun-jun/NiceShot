/**
* @file rank.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RANK2		("../data/TEXTURE/2i.png")							//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANK3		("../data/TEXTURE/3i.png")							//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANK4		("../data/TEXTURE/4i.png")							//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	RANK_SIZE_X			(SCREEN_W/4)									//!< ���ꂽ���ʃe�N�X�`���̕�
#define	RANK_SIZE_Y			(SCREEN_H/4)									//!< ���ꂽ���ʃe�N�X�`���̍���
#define	RANK_POS_X			(SCREEN_W)										//!< ���ꂽ���ʃe�N�X�`���̕\���ʒu
#define	RANK_POS_Y			(SCREEN_H)										//!< ���ꂽ���ʃe�N�X�`���̕\���ʒu

#define RANK_GOUKEI			(3)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;								//!< �e�N�X�`���ւ̃|�C���^ 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];					//!< ���_���i�[���[�N
	D3DXVECTOR3				pos;											//!< �|���S���̍��W
	bool					use;											//!< �g�p���Ă��邩�ǂ�������
}RANK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
/**
* @brief Rank�������֐� InitRank
* @param[in] type �������^�C�v�@InitRank���p�B0:����@0�ȊO: 2��ڈȍ~
* @return HRESULT
*/
HRESULT InitRank(int type);

/**
* @brief Rank�J���֐� UninitRank
*/
void UninitRank(void);

/**
* @brief Rank�X�V�֐� UpdateRank
*/
void UpdateRank(void);

/**
* @brief Rank�`��֐� DrawRank
*/
void DrawRank(void);

/**
* @brief Rank�擾�֐� GetRank
* @return RANK
*/
RANK *GetRank(void);

/**
* @brief �������l���烉���N���Z�b�g����֐� SetRank
* @param[in] PlayerNum �v���C���[�i���o�[ PLAYER_TYPE�Q��
*/
void SetRank(int PlayerNum);

