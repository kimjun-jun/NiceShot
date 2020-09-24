/**
* @file tutorial.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/game.h"
#include "../../h/scene/tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("../data/TEXTURE/tutorialbg.png")						//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TUTORIAL_SIZE_X			(SCREEN_W/4-50)										//!< �`���[�g���A���̕�
#define	TUTORIAL_SIZE_Y			(SCREEN_H/4-50)										//!< �`���[�g���A���̍���
#define	TUTORIAL_POS_X			(SCREEN_CENTER_X)									//!< �`���[�g���A���̕\���ʒu
#define	TUTORIAL_POS_Y			(SCREEN_CENTER_Y)									//!< �`���[�g���A���̕\���ʒu

//=============================================================================
// ����������
//=============================================================================
void TUTO::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPDIRECT3DTEXTURE9 pD3DTexture;
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TUTORIAL, &pD3DTexture);
	this[0].tex2D.SetpD3DTexture(pD3DTexture);
	pD3DTexture->Release();

	//�`��ʒu�ݒ�
	this[0].SetPos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_SIZE_X, TUTORIAL_POS_Y - TUTORIAL_SIZE_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y - TUTORIAL_SIZE_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(TUTORIAL_POS_X + TUTORIAL_SIZE_X, TUTORIAL_POS_Y + TUTORIAL_SIZE_Y, 0.0f));

	// ���_���̍쐬
	MakeVertexTutorial();
}

//=============================================================================
// �ď���������
//=============================================================================
void TUTO::Reinit(void)
{
	this[0].cnt = 0;
}

//=============================================================================
// �I������
//=============================================================================
void TUTO::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void TUTO::Update(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(1, BUTTON_START) || IsButtonTriggered(2, BUTTON_START) || IsButtonTriggered(3, BUTTON_START))
	{
		PlaySound(SOUND_LABEL_SE_enter02);
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
		InitGame();
	}
	for (int CntPlayer = 0; CntPlayer < OBJECT_TUTORIAL_MAX; CntPlayer++)
	{
		if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(CntPlayer, BUTTON_R3))
		{
			this[CntPlayer].cnt++;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void TUTO::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < OBJECT_TUTORIAL_MAX; CntPlayer++)
	{
		if (this[CntPlayer].cnt % 2 == 0)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, this[CntPlayer].tex2D.GetpD3DTexture());
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT TUTO::MakeVertexTutorial(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_TUTORIAL_MAX; CntPlayer++)
	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		// ���_���W�̐ݒ�
		vtx2d[0].vtx = D3DXVECTOR3(pos.x - TUTORIAL_SIZE_X, pos.y - TUTORIAL_SIZE_Y, 0.0f);
		vtx2d[1].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SIZE_X, pos.y - TUTORIAL_SIZE_Y, 0.0f);
		vtx2d[2].vtx = D3DXVECTOR3(pos.x - TUTORIAL_SIZE_X, pos.y + TUTORIAL_SIZE_Y, 0.0f);
		vtx2d[3].vtx = D3DXVECTOR3(pos.x + TUTORIAL_SIZE_X, pos.y + TUTORIAL_SIZE_Y, 0.0f);
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
		this[CntPlayer].tex2D.SettextureVTX(vtx2d);
	}
	return S_OK;
}

