/**
* @file bulletgauge.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class BULLETGAUGE : public OBJECT_2D_VERTEXBUFFER 
{
public:
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletGauge(void);
HRESULT ReInitBulletGauge(void);
void UninitBulletGauge(void);
void UpdateBulletGauge(void);
void DrawBulletGauge(void);

