/**
* @file damege.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/effect/damege.h"

//=============================================================================
// ����������
//=============================================================================
void DAMEGE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPDIRECT3DTEXTURE9 pD3DTexture = NULL;
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_DAMEGE,
		&pD3DTexture);
	this[0].tex2D.SetpD3DTexture(pD3DTexture);

	D3DXVECTOR3 pos[4];
	pos[0] = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	pos[1] = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f);
	pos[2] = D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);
	pos[3] = D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f);

	this[0].SetPos(pos[0]);
	this[1].SetPos(pos[1]);
	this[2].SetPos(pos[2]);
	this[3].SetPos(pos[3]);



	// ���_���̍쐬
	MakeVertexDamege();
}

//=============================================================================
// �ď���������
//=============================================================================
void DAMEGE::Reinit(void)
{
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		this[CntDamege].alpha = 0;
		this[CntDamege].time = 0.0f;
		this[CntDamege].SetUse(false);
	}
}

//=============================================================================
// �I������
//=============================================================================
void DAMEGE::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void DAMEGE::Update(void)
{
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		bool use = this[CntDamege].GetUse();
		if (use==true)
		{
			this[CntDamege].time += 1.0f;
			this[CntDamege].alpha = int((this[CntDamege].time/ SCREENDAMEGE_TIME)*255.0f);


			// ���ˌ��̐ݒ�
			VERTEX_2D vertexWk[POLYGON_2D_VERTEX];
			vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);
			vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);
			vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);
			vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, this[CntDamege].alpha);

			this[CntDamege].tex2D.SettextureVTX(vertexWk);
			if (this[CntDamege].time >= SCREENDAMEGE_TIME)
			{
				this[CntDamege].time = 0.0f;
				this[CntDamege].alpha = 0;
				this[CntDamege].SetUse(false);
			}
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DAMEGE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z��r�Ȃ�

	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		bool use = this[CntDamege].GetUse();
		if (use == true)
		{
			pDevice->SetFVF(FVF_VERTEX_2D);
			pDevice->SetTexture(0, LPDIRECT3DTEXTURE9(this[0].tex2D.GetpD3DTexture()));
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntDamege].tex2D.GettextureVTX(), sizeof(VERTEX_2D));
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z��r����
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT DAMEGE::MakeVertexDamege(void)
{
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		VERTEX_2D vertexWk[POLYGON_2D_VERTEX];
		D3DXVECTOR3 pos = this[CntDamege].GetPos();

		// ���_���W�̐ݒ�
		vertexWk[0].vtx = D3DXVECTOR3(pos.x - DAMEGE_SIZE_X, pos.y - DAMEGE_SIZE_Y, 0.0f);
		vertexWk[1].vtx = D3DXVECTOR3(pos.x + DAMEGE_SIZE_X, pos.y - DAMEGE_SIZE_Y, 0.0f);
		vertexWk[2].vtx = D3DXVECTOR3(pos.x - DAMEGE_SIZE_X, pos.y + DAMEGE_SIZE_Y, 0.0f);
		vertexWk[3].vtx = D3DXVECTOR3(pos.x + DAMEGE_SIZE_X, pos.y + DAMEGE_SIZE_Y, 0.0f);
		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		vertexWk[0].rhw =
			vertexWk[1].rhw =
			vertexWk[2].rhw =
			vertexWk[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);

		// �e�N�X�`�����W�̐ݒ�
		vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		this[CntDamege].tex2D.SettextureVTX(vertexWk);
	}
	return S_OK;
}

