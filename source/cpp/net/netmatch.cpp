/**
* @file netmatch.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
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
#define	NETMATCHRIAL_SIZE_X			(SCREEN_W/4-50)										//!< �`���[�g���A���̕�
#define	NETMATCHRIAL_SIZE_Y			(SCREEN_H/4-50)										//!< �`���[�g���A���̍���
#define	NETMATCHRIAL_POS_X			(SCREEN_CENTER_X)									//!< �`���[�g���A���̕\���ʒu
#define	NETMATCHRIAL_POS_Y			(SCREEN_CENTER_Y)									//!< �`���[�g���A���̕\���ʒu

//=============================================================================
// ����������
//=============================================================================
void NETMATCH::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntTuto = 0; CntTuto < OBJECT_NETMATCH_MAX; CntTuto++)
	{
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NETMATCHRIAL, &this[CntTuto].tex2D.pD3DTexture);
	}

	//�`��ʒu�ݒ�
	this[0].SetPos(D3DXVECTOR3(NETMATCHRIAL_POS_X - NETMATCHRIAL_SIZE_X, NETMATCHRIAL_POS_Y - NETMATCHRIAL_SIZE_Y, 0.0f));
	// ���_���̍쐬
	MakeVertexTutorial();
}

//=============================================================================
// �ď���������
//=============================================================================
void NETMATCH::Reinit(void)
{
}

//=============================================================================
// �I������
//=============================================================================
void NETMATCH::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void NETMATCH::Update(GAME_OBJECT* obj, FADE *fade)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void NETMATCH::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntTuto = 0; CntTuto < OBJECT_NETMATCH_MAX; CntTuto++)
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0, this[CntTuto].tex2D.pD3DTexture);

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntTuto].tex2D.textureVTX, sizeof(VERTEX_2D));

	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT NETMATCH::MakeVertexTutorial(void)
{
	for (int CntTuto = 0; CntTuto < OBJECT_NETMATCH_MAX; CntTuto++)
	{
		D3DXVECTOR3 pos = this[CntTuto].GetPos();
		// ���_���W�̐ݒ�
		this[CntTuto].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x - NETMATCHRIAL_SIZE_X, pos.y - NETMATCHRIAL_SIZE_Y, 0.0f);
		this[CntTuto].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + NETMATCHRIAL_SIZE_X, pos.y - NETMATCHRIAL_SIZE_Y, 0.0f);
		this[CntTuto].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x - NETMATCHRIAL_SIZE_X, pos.y + NETMATCHRIAL_SIZE_Y, 0.0f);
		this[CntTuto].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + NETMATCHRIAL_SIZE_X, pos.y + NETMATCHRIAL_SIZE_Y, 0.0f);
		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		this[CntTuto].tex2D.textureVTX[0].rhw =
			this[CntTuto].tex2D.textureVTX[1].rhw =
			this[CntTuto].tex2D.textureVTX[2].rhw =
			this[CntTuto].tex2D.textureVTX[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		this[CntTuto].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntTuto].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntTuto].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntTuto].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		this[CntTuto].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[CntTuto].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[CntTuto].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[CntTuto].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	return S_OK;
}

