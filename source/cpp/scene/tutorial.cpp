/**
* @file tutorial.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/object/objectclass.h"
#include "../../h/scene/tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("../data/TEXTURE/other/tutorialbg.png")			//!< �ǂݍ��ރe�N�X�`���t�@�C����

constexpr int	TUTORIAL_SIZE_X{ 300 };					//!< �`���[�g���A���̕�
constexpr int	TUTORIAL_SIZE_Y{ 200 };					//!< �`���[�g���A���̍���
constexpr int	TUTORIAL_POS_X{ SCREEN_CENTER_X };		//!< �`���[�g���A���̕\���ʒu
constexpr int	TUTORIAL_POS_Y{ SCREEN_CENTER_Y };		//!< �`���[�g���A���̕\���ʒu
constexpr int	TUTORIAL_POS_X_OFFSET{ 170 };			//!< �`���[�g���A���̕\���ʒu
constexpr int	TUTORIAL_POS_Y_OFFSET{ 200 };			//!< �`���[�g���A���̕\���ʒu

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
TUTO::TUTO(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex2D(OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y - TUTORIAL_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(TUTORIAL_POS_X * 2 - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y - TUTORIAL_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y * 2 - TUTORIAL_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(TUTORIAL_POS_X * 2 - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y * 2 - TUTORIAL_POS_Y_OFFSET, 0.0f));


	//�J�E���g���[�v
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//�`��ʒu���f
		D3DXVECTOR3 pos = this->Transform[CntTuto].Pos();
		this->vtx.Vertex2D(CntTuto, TUTORIAL_SIZE_X/2, TUTORIAL_SIZE_Y/2, pos);

		//RHW�ݒ�
		this->vtx.RHW2D(CntTuto);

		//UV�ݒ�
		this->vtx.UV2D(CntTuto);

		//�J���[�ݒ�
		this->vtx.Color2D(CntTuto, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

		//�g�p�ݒ�
		this->iUseType[CntTuto].Use(YesUseType1);
	}

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_TUTORIAL);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
TUTO::~TUTO(void)
{
	//�e�N�X�`�����
	this->tex.~TEXTURE();
	//���_���
	this->vtx.~VTXBuffer();
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ������
//=============================================================================
void TUTO::Init(void)
{	
	//�J�E���g���[�v
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//�����l�ł͑S���\��������
		this->iUseType[CntTuto].Use(YesUseType1);
	}
}

//=============================================================================
// �X�V
//=============================================================================
void TUTO::Update(GAME_OBJECT* obj, FADE *fade)
{

	//�J�E���g���[�v
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//�X�^�[�g�{�^��orENTER�Ń^�C�g���ɖ߂�
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(CntTuto, BUTTON_START))
		{
			PlaySound(SOUND_LABEL_SE_enter02);
			fade->SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
			obj->Init();
		}

		//������@�摜�@�\����\���؂�ւ�
		if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(CntTuto, BUTTON_R3))
		{
			if (this->iUseType[CntTuto].Use() == YesUseType1)
			{
				this->iUseType[CntTuto].Use(NoUse);
			}
			else if (this->iUseType[CntTuto].Use() == NoUse)
			{
				this->iUseType[CntTuto].Use(YesUseType1);
			}
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void TUTO::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�J�E���g���[�v
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//�`�攻��@
		if (this->iUseType[CntTuto].Use()== YesUseType1)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex.Texture());
			// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntTuto * 4), POLYGON_2D_NUM);
		}
	}
}

