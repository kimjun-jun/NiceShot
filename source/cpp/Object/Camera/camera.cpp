/**
* @file camera.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../../h/main.h"
#include "../../../h/Debug/debugproc.h"
#include "../../../h/Other/input.h"
#include "../../../h/Object/Player/player.h"
#include "../../../h/Object/Camera/camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr float	VIEW_ANGLE{ D3DXToRadian(45.0f) };							// �r���[���ʂ̎���p
constexpr float	VIEW_NEAR_Z{ 10.0f };										// �r���[���ʂ�NearZ�l
constexpr float	VIEW_FAR_Z{ 10000.0f };										// �r���[���ʂ�FarZ�l
constexpr float	VALUE_MOVE_CAMERA{ 2.0f };									// �J�����̈ړ���
constexpr float	VALUE_ROTATE_CAMERA{ D3DX_PI * 0.005f };					// �J�����̉�]��
constexpr float	VIEW_ASPECT{ (float)SCREEN_W / (float)SCREEN_H };			// �r���[���ʂ̃A�X�y�N�g��	

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA			g_Camera[OBJECT_CAMERA_MAX];				// �J�����f�[�^

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	for (int CntCam = 0; CntCam < OBJECT_CAMERA_MAX; CntCam++)
	{
		g_Camera[CntCam].pos = D3DXVECTOR3(0.0f, POS_H_CAM, -POS_W_CAM);
		g_Camera[CntCam].at = VEC3_ALL0;
		g_Camera[CntCam].up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera[CntCam].rot = VEC3_ALL0;
		// ���_�ƒ����_�̋������v�Z
		float vx, vz;
		vx = g_Camera[CntCam].pos.x - g_Camera[CntCam].at.x;
		vz = g_Camera[CntCam].pos.z - g_Camera[CntCam].at.z;
		g_Camera[CntCam].len = sqrtf(vx * vx + vz * vz);

	}
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{

}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(int CamNum) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// �r���[�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Camera[CamNum].mtxView);

		// �r���[�}�g���b�N�X�̍쐬
		D3DXMatrixLookAtLH(&g_Camera[CamNum].mtxView,
			&g_Camera[CamNum].pos,		// �J�����̎��_(�ʒu)
			&g_Camera[CamNum].at,		// �J�����̒����_
			&g_Camera[CamNum].up);		// �J�����̏�����x�N�g��

		// �r���[�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_VIEW, &g_Camera[CamNum].mtxView);

		// �v���W�F�N�V�����}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Camera[CamNum].mtxProjection);

		// �v���W�F�N�V�����}�g���b�N�X�̍쐬
		D3DXMatrixPerspectiveFovLH(&g_Camera[CamNum].mtxProjection,
			VIEW_ANGLE,			// �r���[���ʂ̎���p
			VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
			VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
			VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

		// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�(�����ϊ��̐ݒ�)
		pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera[CamNum].mtxProjection);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera[0];
}