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
// ����������
//=============================================================================
void FADE::Init(void)
{
	// ���_���̐ݒ�
	MakeVertexFade();
}

//=============================================================================
// �ď���������
//=============================================================================
void FADE::Reinit(void)
{
}

//=============================================================================
// �I������
//=============================================================================
void FADE::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void FADE::Update(GAME_OBJECT*obj)
{
	if (eFade != FADE_NONE)
	{// �t�F�[�h������
		if (eFade == FADE_OUT)
		{// �t�F�[�h�A�E�g����
			color.a += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���
			if (color.a >= 1.0f)
			{
				// ��Ԃ�؂�ւ�
				obj->SetScene(eScene);

				// �t�F�[�h�C�������ɐ؂�ւ�
				color.a = 1.0f;
				eFade = FADE_MUSIC_STOP;

				// BGM��~
				StopSound();
			}

			// �F��ݒ�
			SetColor(color);
		}
		else if (eFade == FADE_MUSIC_STOP)
		{
			// BGM�Đ�
			if (sno > -1)
			{
				PlaySound(sno);
			}

			eFade = FADE_IN;
		}
		else if (eFade == FADE_IN)
		{// �t�F�[�h�C������
			color.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
			if (color.a <= 0.0f)
			{
				// �t�F�[�h�����I��
				color.a = 0.0f;
				eFade = FADE_NONE;

			}

			// �F��ݒ�
			SetColor(color);
		}
	}
}

//=============================================================================
// �t�F�[�h���
//=============================================================================
void FADE::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[0].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void FADE::MakeVertexFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	vtx2d[0].rhw =
		vtx2d[1].rhw =
		vtx2d[2].rhw =
		vtx2d[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vtx2d[0].diffuse = color;
	vtx2d[1].diffuse = color;
	vtx2d[2].diffuse = color;
	vtx2d[3].diffuse = color;

	// ���_���W�̐ݒ�
	vtx2d[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx2d[1].vtx = D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f);
	vtx2d[2].vtx = D3DXVECTOR3(0.0f, SCREEN_H, 0.0f);
	vtx2d[3].vtx = D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f);

	// �e�N�X�`�����W�̐ݒ�
	vtx2d[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx2d[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx2d[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx2d[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	this[0].tex2D.SettextureVTX(vtx2d);
}

//=============================================================================
// �F��ݒ�
//=============================================================================
void FADE::SetColor(D3DCOLOR col)
{
	// ���ˌ��̐ݒ�
	VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
	vtx2d[0].diffuse = col;
	vtx2d[1].diffuse = col;
	vtx2d[2].diffuse = col;
	vtx2d[3].diffuse = col;
	this[0].tex2D.SettextureVTX(vtx2d);

}

//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void FADE::SetFade(FADE_TYPE fade, E_STAGE next, int sno)
{
	eFade = fade;
	eScene = next;
	sno = sno;

}

