/**
* @file Move.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/

#include "../../../../../h/main.h"
#include "../../../../../h/Other/input.h"
#include "../../../../../h/Effect/effect.h"
#include "../../../../../h/Object/Player/player.h"
#include "../../../../../h/Object/ObjectClass/StandardComponent/Move/Move.h"

constexpr float	EFFECT_SPEEDUP_SIZE_X{ 25.0f };			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̕�
constexpr float	EFFECT_SPEEDUP_SIZE_Y{ 10.0f };			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̍���
constexpr int	EFFECT_SPEEDUP_TIME{ 10 };				//!< �G�t�F�N�g�X�s�[�h�A�b�v�̐�������
constexpr float	PLAYER_VALUE_DASHRATE{ 2.0f };			//!< �����ɔ�ׂẴ_�b�V���̑��x�{��
constexpr float	PLAYER_MOVE_RATE_X{ 1.0f / 40000.0f };	//!< ���E�����̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	PLAYER_MOVE_RATE_Y{ 1.0f / 200.0f };	//!< �O������̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	PLAYER_MOVE_RATE_LR2{ 1.0f / 20000.0f };//!< �L���^�s���̈ړ��ʔ{��(0�`65000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU{ 0.03f };	//!< �v���C���[�C����]��
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU_MAX{ 1.57f };//!< �v���C���[�C����]�ʍő�l
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN{ 0.01f };	//!< �v���C���[�C�g��]��
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN_MAX{ 0.2f };	//!< �v���C���[�C����]�ʍő�l
constexpr float	MOVE_INERTIA_MOMENT{ 0.8f };			//!< �ړ��ʂ̊������[�����g�A�����W���Ƃ��Ďg�p
constexpr float	VALUE_SPEEDBUFF_SUB{ 1.0f };			//!< �X�s�[�g�A�b�v���Ԃ̃f�N�������g����l
constexpr float	PLAYER_VALUE_MOVE{ 0.1f };				//!< �ړ������x
constexpr float	PLAYER_VALUE_MOVE_MAX{ 4.0f };			//!< �ړ������x�̍ő�l
constexpr int	FRONT_VEC{ 1 };							//!< �O�x�N�g��
constexpr int	BACK_VEC{ -1 };							//!< ���x�N�g��

//=============================================================================
// �ړ�����(ki-bo-do�ňړ�����)
//=============================================================================
void MOVE::MoveKeybord(int CntPlayer, PLAYER *Player)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	int dir = FRONT_VEC;
	//�ړ�����
	if (GetKeyboardPress(DIK_W))
	{
		dir = FRONT_VEC;
		Player->PlayerPara[CntPlayer].StandardPara.Speed += PLAYER_VALUE_MOVE * 10 * 4;
	}
	else if (GetKeyboardPress(DIK_S))
	{
		dir = BACK_VEC;
		Player->PlayerPara[CntPlayer].StandardPara.Speed -= PLAYER_VALUE_MOVE * 10 * 4;
	}

	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		Player->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//����
	if (GetKeyboardPress(DIK_D))
	{
		HoudaiRot.y += 0.02f * dir*Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff * 2;
	}
	else if (GetKeyboardPress(DIK_A))
	{
		HoudaiRot.y -= 0.02f * dir*Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff * 2;
	}

	//�p�x�̐����l
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// �ړ����x�̐���
	if (Player->PlayerPara[CntPlayer].StandardPara.Speed >= PLAYER_VALUE_MOVE_MAX) Player->PlayerPara[CntPlayer].StandardPara.Speed = PLAYER_VALUE_MOVE_MAX;
	else if (Player->PlayerPara[CntPlayer].StandardPara.Speed <= -PLAYER_VALUE_MOVE_MAX) Player->PlayerPara[CntPlayer].StandardPara.Speed = -PLAYER_VALUE_MOVE_MAX;

	// �v���C���[�̍��W���X�V
	pos.x -= sinf(HoudaiRot.y) * (Player->PlayerPara[CntPlayer].StandardPara.Speed * Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (Player->PlayerPara[CntPlayer].StandardPara.Speed * Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//�X�s�[�h�o�t���Ԍ���
	if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		Player->peffect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			Player->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
	}


	//�C������@�o���b�g���e�_(���E�G�C��)
	if (GetKeyboardPress(DIK_RIGHT))
	{
		HoutouRot.y += VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y >= VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		HoutouRot.y -= VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y <= -VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = -VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}

	//�C�g����@�o���b�g���e�_(�O��G�C��)
	if (GetKeyboardPress(DIK_UP))
	{
		HousinRot.x += VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x >= VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		HousinRot.x -= VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x <= -VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = -VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);
}

//=============================================================================
// �ړ�����(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void MOVE::MoveL(int CntPlayer, PLAYER *Player, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = Player->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old�ۑ�
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
	float LAnalogX = 0.0f;		//�c����
	float LAnalogY = 0.0f;		//������
	float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

	//�_�b�V������
	if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//�X�s�[�h�o�t���Ԍ���
		Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		Player->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = pos;
		Player->peffect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			Player->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			Player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (Player->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//�ړ�����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//������͂͌�ޒ��Ɍ��胊�o�[�X����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		Player->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//�ړ��ʂ𔽉f
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//�v���C���[���W���X�V
	pos += move;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	Player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

}







//���g�p
/*

//=============================================================================
// �ړ�����(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void MOVE::MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
	float LAnalogX = 0.0f;		//�c����
	float LAnalogY = 0.0f;		//������
	float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

	//�_�b�V������
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//�X�s�[�h�o�t���Ԍ���
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = pos;
		effect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (this->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//�ړ�����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//������͂͌�ޒ��Ɍ��胊�o�[�X����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//�ړ��ʂ𔽉f
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//�v���C���[���W���X�V
	pos += move;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
}

//=============================================================================
// �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
//=============================================================================
void MOVE::MoveABL(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);

	int dir = FRONT_VEC;
	//�ړ�����
	if (IsButtonPressed(CntPlayer, BUTTON_A))
	{
		dir = FRONT_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed += PLAYER_VALUE_MOVE;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_B))
	{
		dir = BACK_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed -= PLAYER_VALUE_MOVE;
	}

	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//���_�ω��̃A�i���O�l�����ɑ�����ăA�i���O����Ő���
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//����
	if (!IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT) && !IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		dir = FRONT_VEC;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
	{
		HoudaiRot.y += LAnalogX * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		HoudaiRot.y += LAnalogX * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff;
	}

	//�p�x�̐����l
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// �ړ����x�̐���
	if (this->PlayerPara[CntPlayer].StandardPara.Speed >= PLAYER_VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = PLAYER_VALUE_MOVE_MAX;
	else if (this->PlayerPara[CntPlayer].StandardPara.Speed <= -PLAYER_VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = -PLAYER_VALUE_MOVE_MAX;

	// �v���C���[�̍��W���X�V
	pos.x -= sinf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//�X�s�[�h�o�t���Ԍ���
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		effect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//�C������@�o���b�g���e�_(���E�G�C��)
	if (IsButtonPressed(CntPlayer, BUTTON_R1))
	{
		HoutouRot.y += VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y >= VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_L1))
	{
		HoutouRot.y -= VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y <= -VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = -VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}

	//�C�g����@�o���b�g���e�_(�O��G�C��)
	if (IsButtonPressed(CntPlayer, BUTTON_R2))
	{
		HousinRot.x += VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x >= VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(CntPlayer, BUTTON_L2))
	{
		HousinRot.x -= VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x <= -VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = -VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

}

//=============================================================================
// �ړ�����(LR�X�e�B�b�N�ŃL���^�s���ړ�����)
//=============================================================================
void MOVE::MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
	float L2 = 0.0f;		//�c����
	float R2 = 0.0f;		//������
	float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

	//�_�b�V������
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//�X�s�[�h�o�t���Ԍ���
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = pos;
		effect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (this->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//���E�̃L���^�s����O�i���ǂ��炩�̃t���O
	//���L���^�s���O�i����=0�A���L���^�s���̂ݑO�i=3.925f�A�E�L���^�s���̂ݑO�i=-3.925f�A���L���^�s���̂݌��=-0.785f�A�E�L���^�s���̂݌��=0.785f�A
	float Lrot = 3.925f;
	float Rrot = -3.925f;
	int Ldir = 1;
	int Rdir = 1;
	//�ړ�����
	if (IsButtonPressed(PadNum, BUTTON_L1))
	{
		Lrot = -0.785f;
		Ldir *= -1;
	}
	if (IsButtonPressed(PadNum, BUTTON_R1))
	{
		Rrot = 0.785f;
		Rdir *= -1;
	}

	//���L���^�s���g�p
	if (IsButtonPressed(PadNum, BUTTON_L2) && IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		//���͒����l32767�@R2�ŏ�0�@L2�ő�64000
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		R2 = L2;

	}
	//���L���^�s���̂ݎg�p
	else if (IsButtonPressed(PadNum, BUTTON_L2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		HoudaiRot.y += 0.1f*Ldir;
	}
	//�E�L���^�s���̂ݎg�p
	else if (IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		float IZbuf = Button->lZ * PLAYER_MOVE_RATE_LR2;
		R2 = IZbuf;
		R2 = 32767 * PLAYER_MOVE_RATE_LR2 + (-R2) + 32767 * PLAYER_MOVE_RATE_LR2;
		HoudaiRot.y -= 0.1f*Rdir;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}

	//�ړ��ʂ𔽉f
	move.x = R2 * sinf(HoudaiRot.y + Rrot) + L2 * sinf(HoudaiRot.y + Lrot);
	move.z = R2 * cosf(HoudaiRot.y + Rrot) + L2 * cosf(HoudaiRot.y + Lrot);

	//�v���C���[���W���X�V
	pos += move;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}
*/
