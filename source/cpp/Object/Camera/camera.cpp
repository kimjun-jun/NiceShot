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
constexpr float	VIEW_ANGLE{ D3DXToRadian(45.0f) };							//!< �r���[���ʂ̎���p
constexpr float	VIEW_NEAR_Z{ 10.0f };										//!< �r���[���ʂ�NearZ�l
constexpr float	VIEW_FAR_Z{ 10000.0f };										//!< �r���[���ʂ�FarZ�l
constexpr float	VALUE_MOVE_CAMERA{ 2.0f };									//!< �J�����̈ړ���
constexpr float	VALUE_ROTATE_CAMERA{ D3DX_PI * 0.005f };					//!< �J�����̉�]��
constexpr float	VIEW_ASPECT{ (float)SCREEN_W / (float)SCREEN_H };			//!< �r���[���ʂ̃A�X�y�N�g��	
constexpr float BACKCAMERA_TIME{ 150.0f };									//!< �o�b�N�J�����A�C�e���L������

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

//=============================================================================
// �J�������]����(�蓮�ƃA�C�e���̗����Ő���)
//=============================================================================
void CameraRevers(PLAYER *Player, int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	CAMERA *cam = GetCamera();

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//�o�b�N�J���������@�o�b�N�J�����I���@�J�������_�A�����_�AY�{�^���������Ă���������́A�o�b�N�J�����A�C�e����ON�ɂȂ��Ă���Ƃ��̓J�������]
	if (GetKeyboardPress(DIK_B) || IsButtonPressed(PadNum, BUTTON_Y) || Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraItemSignal() == true)
	{
		cam[CntPlayer].at.x = pos.x + (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z + (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x - sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z - cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}

	//�o�b�N�J�����I�t�@�J�������_�A�����_
	//����ȊO�͒ʏ�J����
	else
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//�����A�o�b�N�J�����A�C�e����ON�̎���Y�������ƒʏ�J�����ɂȂ�
	if (Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraItemSignal() == true && IsButtonPressed(PadNum, BUTTON_Y))
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//�o�b�N�J�����̎��ԏ���
	if (Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraItemSignal() == true)
	{
		Player->PlayerPara[CntPlayer].ItemPara.SetBackCameraTime(Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraTime() + 1.0f);
		if (Player->PlayerPara[CntPlayer].ItemPara.GetBackCameraTime() >= BACKCAMERA_TIME)
		{
			Player->PlayerPara[CntPlayer].ItemPara.SetBackCameraTime(0.0f);
			Player->PlayerPara[CntPlayer].ItemPara.SetBackCameraItemSignal(false);
		}
	}
}

//=============================================================================
// �J��������(R�X�e�B�b�N��Rot����)
//=============================================================================
void CameraRotControl(PLAYER *Player, int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old�ۑ�
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//����p�ω���R�X�e�B�b�N�A�i���O�l���g�p
	float RAnalogX = 0.0f;		//�c����
	float RAnalogY = 0.0f;		//������

	//����p����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_R_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_R_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		//���͒����l32000�@�ŏ�0�@�ő�64000
		//�Ȃ̂�-32000���邱�ƂŁ@�����l0�@�ŏ�-32000�@�ő�32000�ɂ��Ă���
		//rot��32000�Ƃ��o�J�������l������ƂԂ���Ԃ̂ŁA����ɓ��͒l������������
		//�ő�0.03���炢�ɂȂ�悤�����@/1000000����(���삵�₷���l�ł���)
		RAnalogX = float(Button->lRx) - 32000.0f;
		RAnalogY = float(Button->lRy) - 32800.0f;

		RAnalogX = RAnalogX / 1000000.0f;
		RAnalogY = RAnalogY / 1000000.0f;

	}

	//��]�ʂ𔽉f
	D3DXVECTOR3 moverot = VEC3_ALL0;
	moverot.y = RAnalogX;
	moverot.x = -RAnalogY;

	HoutouRot.y += moverot.y;
	HousinRot.x += moverot.x;

	//�p�x�̐����l
	{
		if (HousinRot.x >= 0.3f)
		{
			HousinRot.x = 0.3f;
		}
		else if (HousinRot.x <= -0.3f)
		{
			HousinRot.x = -0.3f;
		}
	}

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);
}

