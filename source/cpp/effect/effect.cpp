/**
* @file effect.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/effect/effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT			"../data/TEXTURE/effect/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//=============================================================================
// ����������
//=============================================================================
void EFFECT::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexEffect(pDevice);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EFFECT,			// �t�@�C���̖��O
			&g_pD3DTextureEffect);	// �ǂݍ��ރ������[

}

//=============================================================================
// �ď���������
//=============================================================================
void EFFECT::Reinit(void)
{
	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		this[nCntEffect].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntEffect].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntEffect].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[nCntEffect].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntEffect].SetCol(D3DXCOLOR(DWORD(0)));
		this[nCntEffect].nTimer = 0;
		this[nCntEffect].nDecAlpha = 0.0f;
		this[nCntEffect].SetUse(false);
	}
}

//=============================================================================
// �I������
//=============================================================================
void  EFFECT::Uninit(void)
{
	if (g_pD3DTextureEffect != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if (g_pD3DVtxBuffEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void  EFFECT::Update(void)
{
	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this[nCntEffect].GetUse();
		if (use)
		{
			//--------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this[nCntEffect].GetPos();
			D3DXVECTOR3 move = this[nCntEffect].GetMove();
			D3DXCOLOR col = this[nCntEffect].GetCol();

			pos.x += move.x;
			pos.z += move.z;

			col.a -= this[nCntEffect].nDecAlpha;
			if (col.a <= 0.0f)
			{
				col.a = 0.0f;
			}

			//�J���[�Z�b�g
			SetColorEffect(nCntEffect, col);

			this[nCntEffect].nTimer--;
			if (this[nCntEffect].nTimer <= 0)
			{
				this[nCntEffect].SetUse(false);
			}

			//--------------------------------------------�I�u�W�F�N�g�l��������
			this[nCntEffect].SetPos(pos);
			this[nCntEffect].SetCol(col);

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  EFFECT::Draw(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this[nCntEffect].GetUse();
		if (use)
		{
			//--------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this[nCntEffect].GetPos();
			D3DXVECTOR3 scl = this[nCntEffect].GetScl();
			D3DXCOLOR col = this[nCntEffect].GetCol();
			D3DXMATRIX mtxWorldEffect = this[nCntEffect].GetMatrix();

			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldEffect);

			// �r���[�}�g���b�N�X���擾
			CAMERA *cam = GetCamera();

			mtxWorldEffect._11 = cam[CntPlayer].mtxView._11;
			mtxWorldEffect._12 = cam[CntPlayer].mtxView._21;
			mtxWorldEffect._13 = cam[CntPlayer].mtxView._31;
			mtxWorldEffect._21 = cam[CntPlayer].mtxView._12;
			mtxWorldEffect._22 = cam[CntPlayer].mtxView._22;
			mtxWorldEffect._23 = cam[CntPlayer].mtxView._32;
			mtxWorldEffect._31 = cam[CntPlayer].mtxView._13;
			mtxWorldEffect._32 = cam[CntPlayer].mtxView._23;
			mtxWorldEffect._33 = cam[CntPlayer].mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldEffect);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureEffect);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEffect * 4), POLYGON_2D_NUM);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT EFFECT::MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * OBJECT_EFFECT_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffEffect,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_NORMALSET_SIZE_X / 2, -EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EFFECT_NORMALSET_SIZE_X / 2, -EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f,0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void  EFFECT::SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void  EFFECT::SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int EFFECT::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this[nCntEffect].GetUse();
		if (use != true)
		{
			this[nCntEffect].SetPos(pos);
			this[nCntEffect].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this[nCntEffect].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this[nCntEffect].SetMove(move);
			this[nCntEffect].SetCol(col);
			this[nCntEffect].nTimer = nTimer;
			this[nCntEffect].nDecAlpha = col.a / nTimer;
			this[nCntEffect].SetUse(true);

			// ���_���W�̐ݒ�
			SetVertexEffect(nCntEffect, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorEffect(nCntEffect,col);

			nIdxEffect = nCntEffect;

			break;
		}
	}
	return nIdxEffect;
}
