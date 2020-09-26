/**
* @file result.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/scene/result.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT_OK		_T("../data/TEXTURE/result/result_gameclear.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_NO		_T("../data/TEXTURE/result/result_gameover.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_OK_POS_X			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_OK_POS_Y			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_OK_SIZE_X		(SCREEN_W)						// �^�C�g���̕�
#define	RESULT_OK_SIZE_Y		(SCREEN_H)						// �^�C�g���̍���
#define	RESULT_NO_POS_X			(0)						
#define	RESULT_NO_POS_Y			(0)						
#define	RESULT_NO_SIZE_X		(SCREEN_W)					
#define	RESULT_NO_SIZE_Y		(SCREEN_H)					

//=============================================================================
// ����������
//=============================================================================
void RESULT::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPDIRECT3DTEXTURE9 pD3DTexture[OBJECT_RESULT_MAX];
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_OK, &pD3DTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_NO, &pD3DTexture[1]);

	this[0].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[1].tex2D.SetpD3DTexture(pD3DTexture[1]);

	pD3DTexture[0]->Release();
	pD3DTexture[1]->Release();

	// ���_���̍쐬
	MakeVertexResult();
}

//=============================================================================
// �ď���������
//=============================================================================
void RESULT::Reinit(void)
{

}

//=============================================================================
// �I������
//=============================================================================
void RESULT::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void RESULT::Update(void)
{
	if (IsButtonTriggered(0, BUTTON_A))
	{
		ReinitGame(&this[0]);				// �Q�[���̍ď���������
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void RESULT::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, this[0].tex2D.GetpD3DTexture());

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[0].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT RESULT::MakeVertexResult(void)
{
	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		// ���_���W�̐ݒ�
		vtx2d[0].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y, 0.0f);
		vtx2d[1].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y, 0.0f);
		vtx2d[2].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);
		vtx2d[3].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vtx2d[0].rhw =
			vtx2d[1].rhw =
			vtx2d[2].rhw =
			vtx2d[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vtx2d[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		this[0].tex2D.SettextureVTX(vtx2d);
	}

	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		// ���_���W�̐ݒ�
		vtx2d[0].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y, 0.0f);
		vtx2d[1].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y, 0.0f);
		vtx2d[2].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);
		vtx2d[3].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vtx2d[0].rhw =
			vtx2d[1].rhw =
			vtx2d[2].rhw =
			vtx2d[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		vtx2d[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		this[1].tex2D.SettextureVTX(vtx2d);
	}

	return S_OK;
}

