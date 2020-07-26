/**
* @file bulletprediction.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "main.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "bulletprediction.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLETPREDICTION		"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLETPREDICTION_SIZE_X			(50.0f)							// �r���{�[�h�̕�
#define	BULLETPREDICTION_SIZE_Y			(50.0f)							// �r���{�[�h�̍���

#define	MAX_BULLETPREDICTION			(4096)							// �G�t�F�N�g�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			//!< �ʒu
	D3DXVECTOR3 rot;			//!< ��]
	D3DXVECTOR3 scale;			//!< �X�P�[��
	D3DXCOLOR	col;			//!< �F
	float		fSizeX;			//!< ��
	float		fSizeY;			//!< ����
	int			PlayerType;		//!< �v���C���[�̎��
	bool		bUse;			//!< �g�p���Ă��邩�ǂ���
} BULLETPREDICTION;

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletprediction(int nIdxBulletprediction, float fSizeX, float fSizeY);
void SetColorBulletprediction(int nIdxBulletprediction, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletprediction = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletprediction = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldBulletprediction;					// ���[���h�}�g���b�N�X

BULLETPREDICTION		g_aBulletprediction[MAX_BULLETPREDICTION];	// �e���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBulletprediction(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexBulletprediction(pDevice);
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETPREDICTION,			// �t�@�C���̖��O
			&g_pD3DTextureBulletprediction);	// �ǂݍ��ރ������[
	}
	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		g_aBulletprediction[nCntBulletprediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBulletprediction[nCntBulletprediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBulletprediction[nCntBulletprediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBulletprediction[nCntBulletprediction].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBulletprediction[nCntBulletprediction].fSizeX = BULLETPREDICTION_SIZE_X;
		g_aBulletprediction[nCntBulletprediction].fSizeY = BULLETPREDICTION_SIZE_Y;
		g_aBulletprediction[nCntBulletprediction].bUse = false;
	}
	g_aBulletprediction[0].PlayerType = PLAYER01;
	g_aBulletprediction[1].PlayerType = PLAYER02;
	g_aBulletprediction[2].PlayerType = PLAYER03;
	g_aBulletprediction[3].PlayerType = PLAYER04;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBulletprediction(void)
{
	if (g_pD3DTextureBulletprediction != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBulletprediction->Release();
		g_pD3DTextureBulletprediction = NULL;
	}

	if (g_pD3DVtxBuffBulletprediction != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBulletprediction->Release();
		g_pD3DVtxBuffBulletprediction = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBulletprediction(void)
{
	PLAYER_HONTAI *player = GetPlayerHoudai();

	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		//g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x + (g_aBullet[nCntBullet].Hormingmove.x / 30.0f);
		//g_aBullet[nCntBullet].pos.y -= g_aBullet[nCntBullet].Gravity + g_aBullet[nCntBullet].move.y - (g_aBullet[nCntBullet].Hormingmove.y / 10.0f);
		//g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z + (g_aBullet[nCntBullet].Hormingmove.z / 30.0f);
		//g_aBullet[nCntBullet].Gravity += g_aBullet[nCntBullet].GravityAdd;
		//if (g_aBullet[nCntBullet].Gravity > 10.0f) g_aBullet[nCntBullet].Gravity = 10.0f;


		if (g_aBulletprediction[nCntBulletprediction].bUse)
		{
			//int SetBulletprediction(D3DXVECTOR3 pos, D3DXCOLOR col, float fSizeX, float fSizeY)

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBulletprediction(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// Z��r�Ȃ�
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		if (g_aBulletprediction[nCntBulletprediction].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldBulletprediction);

			// �r���[�}�g���b�N�X���擾
			CAMERA *cam = GetCamera();

			g_mtxWorldBulletprediction._11 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._11;
			g_mtxWorldBulletprediction._12 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._21;
			g_mtxWorldBulletprediction._13 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._31;
			g_mtxWorldBulletprediction._21 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._12;
			g_mtxWorldBulletprediction._22 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._22;
			g_mtxWorldBulletprediction._23 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._32;
			g_mtxWorldBulletprediction._31 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._13;
			g_mtxWorldBulletprediction._32 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._23;
			g_mtxWorldBulletprediction._33 = cam[g_aBulletprediction[nCntBulletprediction].PlayerType].mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aBulletprediction[nCntBulletprediction].scale.x, g_aBulletprediction[nCntBulletprediction].scale.y, g_aBulletprediction[nCntBulletprediction].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletprediction, &g_mtxWorldBulletprediction, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aBulletprediction[nCntBulletprediction].pos.x, g_aBulletprediction[nCntBulletprediction].pos.y, g_aBulletprediction[nCntBulletprediction].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletprediction, &g_mtxWorldBulletprediction, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBulletprediction);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletprediction, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureBulletprediction);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBulletprediction * 4), POLYGON_2D_NUM);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * MAX_BULLETPREDICTION,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffBulletprediction,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletprediction->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
		g_pD3DVtxBuffBulletprediction->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBulletprediction(int nIdxBulletprediction, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletprediction->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletprediction->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorBulletprediction(int nIdxBulletprediction, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletprediction->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletprediction->Unlock();
	}
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int SetBulletprediction(D3DXVECTOR3 pos, D3DXCOLOR col, float fSizeX, float fSizeY)
{
	int nIdxBulletprediction = -1;

	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		if (!g_aBulletprediction[nCntBulletprediction].bUse)
		{
			g_aBulletprediction[nCntBulletprediction].pos = pos;
			g_aBulletprediction[nCntBulletprediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBulletprediction[nCntBulletprediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aBulletprediction[nCntBulletprediction].col = col;
			g_aBulletprediction[nCntBulletprediction].fSizeX = fSizeX;
			g_aBulletprediction[nCntBulletprediction].fSizeY = fSizeY;
			g_aBulletprediction[nCntBulletprediction].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexBulletprediction(nCntBulletprediction, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorBulletprediction(nCntBulletprediction,
				D3DXCOLOR(g_aBulletprediction[nCntBulletprediction].col.r, g_aBulletprediction[nCntBulletprediction].col.g,
					g_aBulletprediction[nCntBulletprediction].col.b, g_aBulletprediction[nCntBulletprediction].col.a));

			nIdxBulletprediction = nCntBulletprediction;

			break;
		}
	}
	return nIdxBulletprediction;
}