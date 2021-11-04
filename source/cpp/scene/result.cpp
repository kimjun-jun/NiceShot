/**
* @file result.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/net/sock.h"
#include "../../h/scene/result.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr int	RESULT_LOGO_POS_X{ SCREEN_CENTER_X };			//!< ���S�̍��W
constexpr int	RESULT_LOGO_POS_Y{ SCREEN_CENTER_Y - 200 };		//!< ���S�̍��W
constexpr int	RESULT_BG_SIZE_X{ SCREEN_W };					//!< �w�i�̕�
constexpr int	RESULT_BG_SIZE_Y{ SCREEN_H };					//!< �w�i�̍���
constexpr float	RESULT_LOGO_SIZE_X{ 300.0f };					//!< ���S�̕�
constexpr float	RESULT_LOGO_SIZE_Y{200.0f};						//!< ���S�̍���

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
RESULT::RESULT(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex2D(OBJECT_RESULT_MAX, FVF_VERTEX_2D);

	//�`��ʒu���f�@�X�N���[���T�C�Y�ō쐬 BG��LOGO
	D3DXVECTOR3 vtxBG[POLYGON_2D_VERTEX] = {
	VEC3_ALL0,
	D3DXVECTOR3(RESULT_BG_SIZE_X, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, RESULT_BG_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_BG_SIZE_X, RESULT_BG_SIZE_Y, 0.0f),
	};
	this->vtx.Vertex2D(RESULT_TEX_BG, vtxBG);

	D3DXVECTOR3 vtxLOGO[POLYGON_2D_VERTEX] = {
	D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f),
	};
	this->vtx.Vertex2D(RESULT_TEX_LOGO, vtxLOGO);

	for (int CntResult = 0; CntResult < OBJECT_RESULT_MAX; CntResult++)
	{
		//RHW�ݒ�
		this->vtx.RHW2D(CntResult);
		//UV�̐ݒ�
		this->vtx.UV2D(CntResult);
		//�J���[�ݒ�
		this->vtx.Color2D(CntResult, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		this->tex[CntResult].LoadTexture(this->c_aFileNameTex[CntResult]);
	}
}


//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
RESULT::~RESULT(void)
{
	for (int CntResult = 0; CntResult < OBJECT_RESULT_MAX; CntResult++)
	{
		this->tex[CntResult].~TEXTURE();
	}
	//���_���
	this->vtx.~VTXBuffer();
}

//=============================================================================
// ����������
//=============================================================================
void RESULT::Init(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void RESULT::Update(GAME_OBJECT* obj, FADE *fade)
{
	//���U���g��ʒ��ɓ��͂�����Ώ��������ă^�C�g���V�[���ɖ߂�
	if (IsButtonTriggered(0, BUTTON_A)|| GetKeyboardTrigger(DIK_RETURN))
	{
		obj->Init();	// �Q�[���̏���������
		fade->SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void RESULT::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntResult = 0; CntResult < OBJECT_RESULT_MAX; CntResult++)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
		pDevice->SetTexture(0, this->tex[CntResult].Texture());
		// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntResult * 4), POLYGON_2D_NUM);
	}
}

