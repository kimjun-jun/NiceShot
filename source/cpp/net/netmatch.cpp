/**
* @file netmatch.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/net/netmatch.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_NETMATCHRIAL		("../data/TEXTURE/old/MatchLogo.png")						//!< �ǂݍ��ރe�N�X�`���t�@�C����
constexpr int	NETMATCHRIAL_SIZE_X			{ SCREEN_W/4-50 };										//!< �`���[�g���A���̕�
constexpr int	NETMATCHRIAL_SIZE_Y			{ SCREEN_H/4-50 };										//!< �`���[�g���A���̍���
constexpr int	NETMATCHRIAL_POS_X			{ SCREEN_CENTER_X };									//!< �`���[�g���A���̕\���ʒu
constexpr int	NETMATCHRIAL_POS_Y			{ SCREEN_CENTER_Y };									//!< �`���[�g���A���̕\���ʒu

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
NETMATCH::NETMATCH(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex2D(1, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform.Pos(D3DXVECTOR3(NETMATCHRIAL_POS_X - NETMATCHRIAL_SIZE_X, NETMATCHRIAL_POS_Y - NETMATCHRIAL_SIZE_Y, 0.0f));

	//�`��ʒu���f
	D3DXVECTOR3 pos = this->Transform.Pos();
	this->vtx.Vertex2D(0, NETMATCHRIAL_SIZE_X / 2, NETMATCHRIAL_SIZE_Y / 2, pos);

	//RHW�ݒ�
	this->vtx.RHW2D(0);

	//UV�ݒ�
	this->vtx.UV2D(0);

	//�J���[�ݒ�
	this->vtx.Color2D(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_NETMATCHRIAL);
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
NETMATCH::~NETMATCH(void)
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
void NETMATCH::Init(void)
{

}

//=============================================================================
// �X�V
//=============================================================================
void NETMATCH::Update(GAME_OBJECT* obj, FADE *fade)
{

}

//=============================================================================
// �`��
//=============================================================================
void NETMATCH::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
	pDevice->SetTexture(0, this->tex.Texture());
	// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
}
