/**
* @file rank.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/scene/rank.h"
#include "../../h/object/objectclass.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RANK2		("../data/TEXTURE/2i.png")							//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANK3		("../data/TEXTURE/3i.png")							//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RANK4		("../data/TEXTURE/4i.png")							//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	RANK_SIZE_X			(SCREEN_W/4)									//!< ���ꂽ���ʃe�N�X�`���̕�
#define	RANK_SIZE_Y			(SCREEN_H/4)									//!< ���ꂽ���ʃe�N�X�`���̍���
#define	RANK_POS_X			(SCREEN_W)										//!< ���ꂽ���ʃe�N�X�`���̕\���ʒu
#define	RANK_POS_Y			(SCREEN_H)										//!< ���ꂽ���ʃe�N�X�`���̕\���ʒu

//=============================================================================
// ����������
//=============================================================================
void RANK::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPDIRECT3DTEXTURE9 pD3DTexture[OBJECT_RANK_MAX];
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_RANK4,
		&pD3DTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_RANK3,
		&pD3DTexture[1]);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_RANK2,
		&pD3DTexture[2]);

	this[0].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[1].tex2D.SetpD3DTexture(pD3DTexture[1]);
	this[2].tex2D.SetpD3DTexture(pD3DTexture[2]);
	// ���_���̍쐬
	MakeVertexRank();
}

//=============================================================================
// �I������
//=============================================================================
void RANK::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void RANK::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void RANK::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		bool use = this[CntRank].GetUse();
		if (use == true)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, this[CntRank].tex2D.GetpD3DTexture());
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntRank].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT RANK::MakeVertexRank(void)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
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
		this[CntRank].tex2D.SettextureVTX(vtx2d);
	}
	return S_OK;
}

//=============================================================================
// �����N���Z�b�g
//=============================================================================
void RANK::SetRank(int PlayerNum)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		bool use = this[CntRank].GetUse();
		if (use != true)
		{
			VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
			switch(PlayerNum)
			{
			case 0:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 1:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(1.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 2:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(1.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			case 3:
				vtx2d[0].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[1].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) - RANK_SIZE_Y, 0.0f);
				vtx2d[2].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) - RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				vtx2d[3].vtx = D3DXVECTOR3(RANK_POS_X*(3.0f / 4.0f) + RANK_SIZE_X, RANK_POS_Y *(3.0f / 4.0f) + RANK_SIZE_Y, 0.0f);
				break;
			default:
				break;
			}
			this[CntRank].tex2D.SettextureVTX(vtx2d);
			this[CntRank].SetUse(true);
			break;
		}
	}
}
