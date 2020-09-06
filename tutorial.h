/**
* @file tutorial.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("data/TEXTURE/tutorialbg.png")						//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TUTORIAL_SIZE_X			(SCREEN_W/4-50)										//!< �`���[�g���A���̕�
#define	TUTORIAL_SIZE_Y			(SCREEN_H/4-50)										//!< �`���[�g���A���̍���
#define	TUTORIAL_POS_X			(SCREEN_CENTER_X)									//!< �`���[�g���A���̕\���ʒu
#define	TUTORIAL_POS_Y			(SCREEN_CENTER_Y)									//!< �`���[�g���A���̕\���ʒu
#define TUTORIAL_GOUKEI			(4)													//!< �`���[�g���A���\���̂̍��v

/**
*�@@struct TUTO
*�@@brief 2D�|���S�����`����\����
*/
typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;										//!< �e�N�X�`���ւ̃|�C���^
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];							//!< ���_���i�[���[�N
	D3DXVECTOR3				pos;													//!< �|���S���̍��W
	int						cnt;													//!< �`�攻��p�@2�Ŋ������]�蔻��@0�\���@1��\���@����̃e�N�X�`����񂪑����邱�Ƃ���������int��`
}TUTO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
/**
* @brief Tutorial�������֐� InitTutorial
* @param[in] type �������^�C�v�@INIT_TIME���p�B0:����@0�ȊO: 2��ڈȍ~
* @return HRESULT
*/
HRESULT InitTutorial(int type);

/**
* @brief Tutorial�J���֐� UninitTutorial
*/
void UninitTutorial(void);

/**
* @brief Tutorial�X�V�֐� UpdateTutorial
*/
void UpdateTutorial(void);

/**
* @brief Tutorial�`��֐� DrawTutorial
*/
void DrawTutorial(void);

