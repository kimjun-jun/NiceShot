/**
* @file status.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	STATUS_SIZE_X			(20.0f)							// �X�e�[�^�X�̕�
#define	STATUS_SIZE_Y			(20.0f)							// �X�e�[�^�X�̍���
#define	STATUS_POS_X			(SCREEN_CENTER_X)				// �X�e�[�^�X�̕\���ʒu
#define	STATUS_POS_Y			(SCREEN_CENTER_Y)				// �X�e�[�^�X�̕\���ʒu
#define	STATUS_POS_X_OFFSET		(180.0f)						// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g
#define	STATUS_POS_Y_OFFSET		(120.0f)						// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g


class STATUS : public OBJECT_2D
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
HRESULT InitStatus(int type);
void UninitStatus(void);
void UpdateStatus(void);
void DrawStatus(void);