//=============================================================================
//
// �������� [explosion.cpp]
// Author : �ؑ���
//
//=============================================================================
#include "main.h"
#include "explosion.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EXPLOSION		"data/TEXTURE/bomb.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLET_SIZE_X			(50.0f)							// �r���{�[�h�̕�
#define	BULLET_SIZE_Y			(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_EXPLOSION	(2.0f)							// �ړ����x

#define	MAX_EXPLOSION			(128)							// �r���{�[�h�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nCounter;			// �J�E���^�[
	int nPatternX;			// �p�^�[��No.X
	int nPatternY;			// �p�^�[��No.Y
	int nType;				// ���
	bool bUse;				// �g�p���Ă��邩�ǂ���
} EXPLOSION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void SetVertexExplosion(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorExplosion(int nIdxExplosion, D3DXCOLOR col);
void SetTextureExplosion(int nIdxExplosion, int nPatternX, int nPatternY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExplosion = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldExplosion;			// ���[���h�}�g���b�N�X

EXPLOSION				g_aExplosion[MAX_EXPLOSION];	// �������[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitExplosion(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexExplosion(pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
			TEXTURE_EXPLOSION,			// �t�@�C���̖��O
			&g_pD3DTextureExplosion);	// �ǂݍ��ރ������[
	}
	for(int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].fSizeX = BULLET_SIZE_X;
		g_aExplosion[nCntExplosion].fSizeY = BULLET_SIZE_Y;
		g_aExplosion[nCntExplosion].nCounter = 0;
		g_aExplosion[nCntExplosion].nPatternX = 0;
		g_aExplosion[nCntExplosion].nPatternY = 0;
		g_aExplosion[nCntExplosion].nType = EXPLOSIONTYPE_BULLET_PLAYER;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosion(void)
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
void UpdateExplosion(void)
{
	for(int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if(g_aExplosion[nCntExplosion].bUse)
		{
			g_aExplosion[nCntExplosion].nCounter--;
			if((g_aExplosion[nCntExplosion].nCounter % 4) == 0)
			{
				g_aExplosion[nCntExplosion].nPatternX++;
				if(g_aExplosion[nCntExplosion].nPatternY >= 4)
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}
				else if (g_aExplosion[nCntExplosion].nPatternX >= 4)
				{
					g_aExplosion[nCntExplosion].nPatternX = 0;
					g_aExplosion[nCntExplosion].nPatternY++;
				}
				else
				{
					// �e�N�X�`�����W�̐ݒ�
					SetTextureExplosion(nCntExplosion, g_aExplosion[nCntExplosion].nPatternX, g_aExplosion[nCntExplosion].nPatternY);
				}
			}

			// ���_���W�̐ݒ�
			g_aExplosion[nCntExplosion].fSizeX += 0.50f;
			g_aExplosion[nCntExplosion].fSizeY += 0.50f;
			SetVertexExplosion(nCntExplosion, g_aExplosion[nCntExplosion].fSizeX, g_aExplosion[nCntExplosion].fSizeY);

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosion(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScale,mtxTranslate;

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for(int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if(g_aExplosion[nCntExplosion].bUse)
		{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_mtxWorldExplosion);

				// �r���[�}�g���b�N�X���擾
				CAMERA *cam = GetCamera();

				g_mtxWorldExplosion._11 = cam[CntPlayer].mtxView._11;
				g_mtxWorldExplosion._12 = cam[CntPlayer].mtxView._21;
				g_mtxWorldExplosion._13 = cam[CntPlayer].mtxView._31;
				g_mtxWorldExplosion._21 = cam[CntPlayer].mtxView._12;
				g_mtxWorldExplosion._22 = cam[CntPlayer].mtxView._22;
				g_mtxWorldExplosion._23 = cam[CntPlayer].mtxView._32;
				g_mtxWorldExplosion._31 = cam[CntPlayer].mtxView._13;
				g_mtxWorldExplosion._32 = cam[CntPlayer].mtxView._23;
				g_mtxWorldExplosion._33 = cam[CntPlayer].mtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, g_aExplosion[nCntExplosion].scale.x, g_aExplosion[nCntExplosion].scale.y, g_aExplosion[nCntExplosion].scale.z);
				D3DXMatrixMultiply(&g_mtxWorldExplosion, &g_mtxWorldExplosion, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldExplosion, &g_mtxWorldExplosion, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldExplosion);

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

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * MAX_EXPLOSION,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
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

		for(int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, -BULLET_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X / 2, BULLET_SIZE_Y / 2, 0.0f);

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
void SetVertexExplosion(int nIdxExplosion, float fSizeX, float fSizeY)
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
void SetColorExplosion(int nIdxExplosion, D3DXCOLOR col)
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
void SetTextureExplosion(int nIdxExplosion, int nPatternX, int nPatternY)
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
int SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType, D3DXCOLOR col)
{
	int nIdxExplosion = -1;

	for(int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if(!g_aExplosion[nCntExplosion].bUse)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aExplosion[nCntExplosion].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].fSizeX = fSizeX;
			g_aExplosion[nCntExplosion].fSizeY = fSizeY;
			g_aExplosion[nCntExplosion].nCounter = 0;
			g_aExplosion[nCntExplosion].nPatternX = 0;
			g_aExplosion[nCntExplosion].nPatternY = 0;
			g_aExplosion[nCntExplosion].nType = nType;
			g_aExplosion[nCntExplosion].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexExplosion(nCntExplosion, fSizeX, fSizeY);

			SetColorExplosion(nCntExplosion, g_aExplosion[nCntExplosion].col);

			// �e�N�X�`�����W�̐ݒ�
			SetTextureExplosion(nCntExplosion, 0,0);

			nIdxExplosion = nCntExplosion;

			break;
		}
	}

	return nIdxExplosion;
}