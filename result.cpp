//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultok = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureResultno = NULL;	// �e�N�X�`���ւ̃|�C���^

VERTEX_2D				g_vertexWkResultok[POLYGON_2D_VERTEX];			// ���_���i�[���[�N
VERTEX_2D				g_vertexWkResultno[POLYGON_2D_VERTEX];		// ���_���i�[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT_OK,				// �t�@�C���̖��O
			&g_pD3DTextureResultok);		// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_RESULT_NO,		// �t�@�C���̖��O
			&g_pD3DTextureResultno);	// �ǂݍ��ރ������[
	}

	// ���_���̍쐬
	MakeVertexResult();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if (g_pD3DTextureResultok != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultok->Release();
		g_pD3DTextureResultok = NULL;
	}

	if (g_pD3DTextureResultno != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResultno->Release();
		g_pD3DTextureResultno = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
	{// Enter��������A�X�e�[�W��؂�ւ���
		InitGame();				// �Q�[���̍ď���������
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResultok);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, g_vertexWkResultok, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(void)
{
	// ���_���W�̐ݒ�
	g_vertexWkResultok[0].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y, 0.0f);
	g_vertexWkResultok[1].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y, 0.0f);
	g_vertexWkResultok[2].vtx = D3DXVECTOR3(RESULT_OK_POS_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);
	g_vertexWkResultok[3].vtx = D3DXVECTOR3(RESULT_OK_POS_X + RESULT_OK_SIZE_X, RESULT_OK_POS_Y + RESULT_OK_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResultok[0].rhw =
		g_vertexWkResultok[1].rhw =
		g_vertexWkResultok[2].rhw =
		g_vertexWkResultok[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResultok[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultok[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultok[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultok[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResultok[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultok[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultok[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultok[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̐ݒ�
	g_vertexWkResultno[0].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y, 0.0f);
	g_vertexWkResultno[1].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y, 0.0f);
	g_vertexWkResultno[2].vtx = D3DXVECTOR3(RESULT_NO_POS_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);
	g_vertexWkResultno[3].vtx = D3DXVECTOR3(RESULT_NO_POS_X + RESULT_NO_SIZE_X, RESULT_NO_POS_Y + RESULT_NO_SIZE_Y, 0.0f);

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	g_vertexWkResultno[0].rhw =
		g_vertexWkResultno[1].rhw =
		g_vertexWkResultno[2].rhw =
		g_vertexWkResultno[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_vertexWkResultno[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultno[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultno[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkResultno[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	g_vertexWkResultno[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkResultno[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkResultno[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkResultno[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

