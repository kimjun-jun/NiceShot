/**
* @file camera.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

class PLAYER;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr float	POS_H_CAM		{ 80.0f };		// �J�����̈ʒu����(�v���C���[����݂�)
constexpr float	POS_W_CAM		{ -150.0f };	// �J�����̈ʒu����(�v���C���[����݂�)
constexpr float	AT_H_CAM		{ 0.0f };		// �J�����̒����_����(�v���C���[����݂�)
constexpr float	AT_W_CAM		{ 300.0f };		// �J�����̒����_����(�v���C���[����݂�)

struct CAMERA
{
	D3DXVECTOR3			pos;			// �J�����̎��_(�ʒu)
	D3DXVECTOR3			at;				// �J�����̒����_
	D3DXVECTOR3			up;				// �J�����̏�����x�N�g��
	D3DXVECTOR3			rot;			// �J�����̉�]
	float				len;			// �J�����̎��_�ƒ����_�̋���
	D3DXMATRIX			mtxView;		// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int CamNum);
CAMERA *GetCamera();

//------�J��������
void CameraRevers(PLAYER *Player, int CntPlayer, bool Netflag);		//!< �J��������
void CameraRotControl(PLAYER *Player, int CntPlayer, bool Netflag);	//!< �J��������(LR�X�e�B�b�N�ňړ�����)
