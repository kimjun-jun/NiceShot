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
#define	TEXTURE_RESULT_LOGO		_T("../data/TEXTURE/other/GameClear.png")					// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_BG		_T("../data/TEXTURE/other/GameClear_Background.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_LOGO_POS_X		(SCREEN_CENTER_X)											// ���S�̍��W
#define	RESULT_LOGO_POS_Y		(SCREEN_CENTER_Y-200.0f)									// ���S�̍��W
#define	RESULT_LOGO_SIZE_X		(300.0f)													// ���S�̕�
#define	RESULT_LOGO_SIZE_Y		(200.0f)													// ���S�̍���
#define	RESULT_BG_SIZE_X		(SCREEN_W)					
#define	RESULT_BG_SIZE_Y		(SCREEN_H)					

//=============================================================================
// ����������
//=============================================================================
void RESULT::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_LOGO, &this[0].tex2D.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_BG, &this[1].tex2D.pD3DTexture);


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
void RESULT::Update(GAME_OBJECT* obj, FADE *fade)
{
	if (IsButtonTriggered(0, BUTTON_A)|| GetKeyboardTrigger(DIK_RETURN))
	{
		obj->Reinit();				// �Q�[���̍ď���������
		fade->SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void RESULT::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//BG
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this[1].tex2D.pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[1].tex2D.textureVTX, sizeof(VERTEX_2D));


	//LOGO
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this[0].tex2D.pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[0].tex2D.textureVTX, sizeof(VERTEX_2D));

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT RESULT::MakeVertexResult(void)
{
	{
		// ���_���W�̐ݒ�
		this[0].tex2D.textureVTX[0].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f);
		this[0].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f);
		this[0].tex2D.textureVTX[2].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f);
		this[0].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		this[0].tex2D.textureVTX[0].rhw =
			this[0].tex2D.textureVTX[1].rhw =
			this[0].tex2D.textureVTX[2].rhw =
			this[0].tex2D.textureVTX[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		this[0].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[0].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[0].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[0].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		this[0].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[0].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[0].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[0].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	{
		// ���_���W�̐ݒ�
		this[1].tex2D.textureVTX[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this[1].tex2D.textureVTX[1].vtx = D3DXVECTOR3(RESULT_BG_SIZE_X, 0.0f, 0.0f);
		this[1].tex2D.textureVTX[2].vtx = D3DXVECTOR3(0.0f, RESULT_BG_SIZE_Y, 0.0f);
		this[1].tex2D.textureVTX[3].vtx = D3DXVECTOR3(RESULT_BG_SIZE_X, RESULT_BG_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		this[1].tex2D.textureVTX[0].rhw =
			this[1].tex2D.textureVTX[1].rhw =
			this[1].tex2D.textureVTX[2].rhw =
			this[1].tex2D.textureVTX[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		this[1].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[1].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[1].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[1].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		this[1].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[1].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[1].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[1].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

