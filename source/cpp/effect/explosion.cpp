/**
* @file explosion.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/effect/explosion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EXPLOSION		"../data/TEXTURE/bomb.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExplosion = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//=============================================================================
// ����������
//=============================================================================
void EXPLOSION::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexExplosion(pDevice);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EXPLOSION,			// �t�@�C���̖��O
			&g_pD3DTextureExplosion);	// �ǂݍ��ރ������[
}

//=============================================================================
// �ď���������
//=============================================================================
void EXPLOSION::Reinit(void)
{
	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		this[nCntExplosion].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntExplosion].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntExplosion].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[nCntExplosion].SetCol(D3DXCOLOR(DWORD(0)));
		this[nCntExplosion].nCounter = 0;
		this[nCntExplosion].nPatternX = 0;
		this[nCntExplosion].nPatternY = 0;
		this[nCntExplosion].fSizeX = 0;
		this[nCntExplosion].fSizeY = 0;
		this[nCntExplosion].nType = nType;
		this[nCntExplosion].SetUse(false);
	}
}

//=============================================================================
// �I������
//=============================================================================
void EXPLOSION::Uninit(void)
{
	if(g_pD3DTextureExplosion != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureExplosion->Release();
		g_pD3DTextureExplosion = NULL;
	}

	if(g_pD3DVtxBuffExplosion != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffExplosion->Release();
		g_pD3DVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void EXPLOSION::Update(void)
{
	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this[nCntExplosion].GetUse();
		if(use)
		{
			this[nCntExplosion].nCounter--;
			if((this[nCntExplosion].nCounter % 4) == 0)
			{
				this[nCntExplosion].nPatternX++;
				if(this[nCntExplosion].nPatternY >= 4)
				{
					this[nCntExplosion].SetUse(false);
				}
				else if (this[nCntExplosion].nPatternX >= 4)
				{
					this[nCntExplosion].nPatternX = 0;
					this[nCntExplosion].nPatternY++;
				}
				else
				{
					// �e�N�X�`�����W�̐ݒ�
					SetTextureExplosion(nCntExplosion, this[nCntExplosion].nPatternX, this[nCntExplosion].nPatternY);
				}
			}

			// ���_���W�̐ݒ�
			this[nCntExplosion].fSizeX += 0.50f;
			this[nCntExplosion].fSizeY += 0.50f;
			SetVertexExplosion(nCntExplosion, this[nCntExplosion].fSizeX, this[nCntExplosion].fSizeY);

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void EXPLOSION::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this[nCntExplosion].GetUse();
		if (use)
		{
			//--------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this[nCntExplosion].GetPos();
			D3DXVECTOR3 scl = this[nCntExplosion].GetScl();
			D3DXCOLOR col = this[nCntExplosion].GetCol();
			D3DXMATRIX mtxWorldExplosion = this[nCntExplosion].GetMatrix();

			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				D3DXMATRIX mtxView, mtxScale, mtxTranslate;

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorldExplosion);

				// �r���[�}�g���b�N�X���擾
				CAMERA *cam = GetCamera();

				mtxWorldExplosion._11 = cam[CntPlayer].mtxView._11;
				mtxWorldExplosion._12 = cam[CntPlayer].mtxView._21;
				mtxWorldExplosion._13 = cam[CntPlayer].mtxView._31;
				mtxWorldExplosion._21 = cam[CntPlayer].mtxView._12;
				mtxWorldExplosion._22 = cam[CntPlayer].mtxView._22;
				mtxWorldExplosion._23 = cam[CntPlayer].mtxView._32;
				mtxWorldExplosion._31 = cam[CntPlayer].mtxView._13;
				mtxWorldExplosion._32 = cam[CntPlayer].mtxView._23;
				mtxWorldExplosion._33 = cam[CntPlayer].mtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldExplosion);

				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, g_pD3DVtxBuffExplosion, 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureExplosion);

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntExplosion * 4), POLYGON_2D_NUM);

				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			}
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT EXPLOSION::MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * OBJECT_EXPLOSION_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffExplosion,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, -EXPLOSION_SIZE / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EXPLOSION_SIZE / 2, EXPLOSION_SIZE / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
		g_pD3DVtxBuffExplosion->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void EXPLOSION::SetVertexExplosion(int nIdxExplosion, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxExplosion * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void EXPLOSION::SetColorExplosion(int nIdxExplosion, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxExplosion * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void EXPLOSION::SetTextureExplosion(int nIdxExplosion, int nPatternX, int nPatternY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxExplosion * 4);

		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(nPatternX * 0.25f, nPatternY * 0.25f);
		pVtx[1].tex = D3DXVECTOR2((nPatternX + 1) * 0.25f, nPatternY * 0.25f);
		pVtx[2].tex = D3DXVECTOR2(nPatternX * 0.25f, (nPatternY + 1) * 0.25f);
		pVtx[3].tex = D3DXVECTOR2((nPatternX + 1) * 0.25f, (nPatternY + 1) * 0.25f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int EXPLOSION::SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType, D3DXCOLOR col)
{
	int nIdxExplosion = -1;

	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this[nCntExplosion].GetUse();
		if (use!=true)
		{
			this[nCntExplosion].SetPos(pos);
			this[nCntExplosion].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this[nCntExplosion].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this[nCntExplosion].SetCol(col);
			this[nCntExplosion].SetUse(true);
			this[nCntExplosion].fSizeX = fSizeX;
			this[nCntExplosion].fSizeY = fSizeY;
			this[nCntExplosion].nCounter = 0;
			this[nCntExplosion].nPatternX = 0;
			this[nCntExplosion].nPatternY = 0;
			this[nCntExplosion].nType = nType;

			// ���_���W�̐ݒ�
			SetVertexExplosion(nCntExplosion, fSizeX, fSizeY);

			SetColorExplosion(nCntExplosion, col);

			// �e�N�X�`�����W�̐ݒ�
			SetTextureExplosion(nCntExplosion, 0,0);

			nIdxExplosion = nCntExplosion;

			break;
		}
	}

	return nIdxExplosion;
}
