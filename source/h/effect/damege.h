/**
* @file damege.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_DAMEGE			("../data/TEXTURE/screendamage.png")			
#define	DAMEGE_SIZE_X			(SCREEN_W/4)										// �`���[�g���A���̕�
#define	DAMEGE_SIZE_Y			(SCREEN_H/4)										// �`���[�g���A���̍���
#define	DAMEGE_POS_X			(SCREEN_CENTER_X)									// �`���[�g���A���̕\���ʒu
#define	DAMEGE_POS_Y			(SCREEN_CENTER_Y)									// �`���[�g���A���̕\���ʒu



class DAMEGE : public OBJECT_2D
{
public:
	DAMEGE() { alpha = 0, time = 0.0f; }
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	HRESULT						MakeVertexDamege(void);		//!< ���_�ݒ�
	int							alpha;						//!< �A���t�@�l
	float						time;						//!< �`�掞��
};

