/**
* @file fade.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/sound.h"
#include "../../h/other/fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	FADE_RATE		(0.02f)									//!< �t�F�[�h�W��

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
FADE::FADE(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex2D(OBJECT_FADE_MAX, FVF_VERTEX_2D);

	//�`��ʒu���f�@�X�N���[���T�C�Y�ō쐬
	D3DXVECTOR3 VTX[POLYGON_2D_VERTEX] = {
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, SCREEN_H, 0.0f),
	D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f),
	};
	this->vtx.Vertex2D(0, VTX);
}


//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
FADE::~FADE(void)
{
	//���_���
	this->vtx.~VTXBuffer();
}

//=============================================================================
// ����������
//=============================================================================
void FADE::Init(void)
{
	//�����l�ݒ�
	this->FadePara.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->FadePara.eFade = FADE_IN; 
	this->FadePara.eScene = SCENE_TITLE;
	this->FadePara.sno = -1;
}

//=============================================================================
// �X�V����
//=============================================================================
void FADE::Update(GAME_OBJECT*obj)
{
	if (this->FadePara.eFade != FADE_NONE)
	{// �t�F�[�h������
		if (this->FadePara.eFade == FADE_OUT)
		{// �t�F�[�h�A�E�g����
			this->FadePara.color.a += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���
			if (this->FadePara.color.a >= 1.0f)
			{
				// ��Ԃ�؂�ւ�
				obj->SetScene(this->FadePara.eScene);
				
				// �t�F�[�h�C�������ɐ؂�ւ�
				this->FadePara.color.a = 1.0f;
				this->FadePara.eFade = FADE_MUSIC_STOP;

				// BGM��~
				StopSound();
			}

			// �F��ݒ�
			this->vtx.Color2D(0,this->FadePara.color);
		}
		else if (this->FadePara.eFade == FADE_MUSIC_STOP)
		{
			// BGM�Đ�
			if (this->FadePara.sno > -1)
			{
				PlaySound(this->FadePara.sno);
			}

			this->FadePara.eFade = FADE_IN;
		}
		else if (this->FadePara.eFade == FADE_IN)
		{// �t�F�[�h�C������
			this->FadePara.color.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
			if (this->FadePara.color.a <= 0.0f)
			{
				// �t�F�[�h�����I��
				this->FadePara.color.a = 0.0f;
				this->FadePara.eFade = FADE_NONE;

			}

			// �F��ݒ�
			this->vtx.Color2D(0,this->FadePara.color);
		}
	}
}

//=============================================================================
// �t�F�[�h���
//=============================================================================
void FADE::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
}

//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void FADE::SetFade(FADE_TYPE fade, E_STAGE next, int sno)
{
	this->FadePara.eFade = fade;
	this->FadePara.eScene = next;
	this->FadePara.sno = sno;

}

