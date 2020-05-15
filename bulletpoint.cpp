//=============================================================================
//
// ����A�C�R������ [bulletpoint.cpp]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "camera.h"
#include "bulletpoint.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLETPOINT			"data/TEXTURE/rakatiten.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLETPOINT_WIDTH			(50.0f)								// �A�C�R���̔��a��
#define	BULLETPOINT_HEIGHT			(80.0f)								// �A�C�R���̔��a��
#define	BULLETPOINT_LEN				(300.0f)							// �v���C���[�ƃA�C�R���̋���
#define	VALUE_MOVE_BULLETPOINT		(3.00f)								// �ړ����x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBulletPoint(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletPoint(int nIdxBulletPoint, float fWidth, float fHeight);
void SetColorBulletPoint(int nIdxBulletPoint);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureBulletPoint = {};					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletPoint = NULL;				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

BULLETPOINT					g_aBulletPoint[BULLETPOINT_MAX];

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.8f, 0.6f, 0.2f, 1.0f),//p4�J���[
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBulletPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MakeVertexBulletPoint(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BULLETPOINT,									// �t�@�C���̖��O
		&g_apD3DTextureBulletPoint);							// �ǂݍ��ރ������[

	for (int nCntBulletPoint = 0; nCntBulletPoint < BULLETPOINT_MAX; nCntBulletPoint++)
	{
		g_aBulletPoint[nCntBulletPoint].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBulletPoint[nCntBulletPoint].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBulletPoint[nCntBulletPoint].fWidth = BULLETPOINT_WIDTH;
		g_aBulletPoint[nCntBulletPoint].fHeight = BULLETPOINT_HEIGHT;
		g_aBulletPoint[nCntBulletPoint].bUse = false;
	}

	// �v���C�L�������̃A�C�R�����Z�b�g
	for (int Cnt = 0; Cnt < BULLETPOINT_MAX; Cnt++)
	{
		// ���_���̍쐬
		SetBulletPoint(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 60.0f, 90.0f, Cnt);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBulletPoint(void)
{
	if (g_apD3DTextureBulletPoint != NULL)
	{// �e�N�X�`���̊J��
		g_apD3DTextureBulletPoint->Release();
		g_apD3DTextureBulletPoint = NULL;
	}

	if (g_pD3DVtxBuffBulletPoint != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBulletPoint->Release();
		g_pD3DVtxBuffBulletPoint = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBulletPoint(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PLAYER_MAX; nCntPlayer++)
	{
		PLAYER_HONTAI *p = GetPlayerHoudai();
		if (p[nCntPlayer].OneParameter.use)
		{
			if (g_aBulletPoint[nCntPlayer].bUse)
			{
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBulletPoint(int type)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();
	if (p[type].OneParameter.use)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX  mtxScale, mtxTranslate;

		// ���e�X�g��L����
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ���C���e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		if (g_aBulletPoint[type].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBulletPoint[type].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			CAMERA *cam = GetCamera();
			g_aBulletPoint[type].mtxWorld._11 = cam[type].mtxView._11;
			g_aBulletPoint[type].mtxWorld._12 = cam[type].mtxView._21;
			g_aBulletPoint[type].mtxWorld._13 = cam[type].mtxView._31;
			g_aBulletPoint[type].mtxWorld._21 = cam[type].mtxView._12;
			g_aBulletPoint[type].mtxWorld._22 = cam[type].mtxView._22;
			g_aBulletPoint[type].mtxWorld._23 = cam[type].mtxView._32;
			g_aBulletPoint[type].mtxWorld._31 = cam[type].mtxView._13;
			g_aBulletPoint[type].mtxWorld._32 = cam[type].mtxView._23;
			g_aBulletPoint[type].mtxWorld._33 = cam[type].mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aBulletPoint[type].scl.x, g_aBulletPoint[type].scl.y, g_aBulletPoint[type].scl.z);
			D3DXMatrixMultiply(&g_aBulletPoint[type].mtxWorld, &g_aBulletPoint[type].mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aBulletPoint[type].pos.x, g_aBulletPoint[type].pos.y, g_aBulletPoint[type].pos.z);
			D3DXMatrixMultiply(&g_aBulletPoint[type].mtxWorld, &g_aBulletPoint[type].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBulletPoint[type].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletPoint, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apD3DTextureBulletPoint);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (type * 4), POLYGON_2D_NUM);
		}
		// ���C���e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// ���e�X�g�𖳌���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBulletPoint(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * BULLETPOINT_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffBulletPoint,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletPoint->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntBulletPoint = 0; nCntBulletPoint < BULLETPOINT_MAX; nCntBulletPoint++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-BULLETPOINT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-BULLETPOINT_WIDTH / 2.0f, BULLETPOINT_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(BULLETPOINT_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLETPOINT_WIDTH / 2.0f, BULLETPOINT_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffBulletPoint->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBulletPoint(int nIdxBulletPoint, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletPoint->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletPoint * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, fHeight);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, fHeight);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletPoint->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorBulletPoint(int nIdxBulletPoint)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletPoint->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletPoint * 4);

		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletPoint->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void SetBulletPoint(D3DXVECTOR3 pos, float fWidth, float fHeight, int type)
{
	int nIdxBulletPoint = -1;

	if (!g_aBulletPoint[type].bUse)
	{
		g_aBulletPoint[type].pos = pos;
		g_aBulletPoint[type].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBulletPoint[type].fWidth = fWidth;
		g_aBulletPoint[type].fHeight = fHeight;
		g_aBulletPoint[type].bUse = true;

		// ���_���W�̐ݒ�
		SetVertexBulletPoint(type, fWidth, fHeight);

		// ���_�J���[�̐ݒ�
		SetColorBulletPoint(type);

	}
}

//=============================================================================
// �w�b�h�A�C�R�����擾
//=============================================================================
BULLETPOINT *GetBulletPoint(void)
{
	return &g_aBulletPoint[0];
}


