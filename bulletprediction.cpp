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
#define	BULLETPREDICTION_SIZE_X			(5.0f)							// �r���{�[�h�̕�
#define	BULLETPREDICTION_SIZE_Y			(5.0f)							// �r���{�[�h�̍���

#define	MAX_BULLETPREDICTION			(1000)							// �G�t�F�N�g�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX	mtxWorld;									//!< ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;										//!< �ʒu
	D3DXVECTOR3 rot;										//!< ��]
	D3DXVECTOR3 scale;										//!< �X�P�[��
	D3DXCOLOR	col;										//!< �F
	float		fSizeX;										//!< ��
	float		fSizeY;										//!< ����
	bool		bUse;										//!< �g�p���Ă��邩�ǂ���
} BULLETPREDICTION;

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.01f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.01f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.01f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.01f),//p4�J���[
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY);
void SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletprediction[PLAYER_MAX] = { NULL };		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletprediction[PLAYER_MAX] = { NULL };		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

BULLETPREDICTION		g_aBulletprediction[PLAYER_MAX][MAX_BULLETPREDICTION];	// �e���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBulletprediction(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexBulletprediction(pDevice);
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETPREDICTION,			// �t�@�C���̖��O
			&g_pD3DTextureBulletprediction[CntPlayer]);	// �ǂݍ��ރ������[
	}
		for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
		{

			g_aBulletprediction[CntPlayer][nCntBulletprediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aBulletprediction[CntPlayer][nCntBulletprediction].fSizeX = BULLETPREDICTION_SIZE_X;
			g_aBulletprediction[CntPlayer][nCntBulletprediction].fSizeY = BULLETPREDICTION_SIZE_Y;
			g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse = false;
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBulletprediction(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DTextureBulletprediction[CntPlayer] != NULL)
		{// �e�N�X�`���̊J��
			g_pD3DTextureBulletprediction[CntPlayer]->Release();
			g_pD3DTextureBulletprediction[CntPlayer] = NULL;
		}

		if (g_pD3DVtxBuffBulletprediction[CntPlayer] != NULL)
		{// ���_�o�b�t�@�̊J��
			g_pD3DVtxBuffBulletprediction[CntPlayer]->Release();
			g_pD3DVtxBuffBulletprediction[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBulletprediction(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
		{
			if (g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse)
			{
				g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse = false;
			}
		}
	}
	PLAYER_HONTAI *player = GetPlayerHoudai();

	//�v���C���[�̏�񂩂甭�ˈʒu�p�x�ړ��ʂ𗘗p���ăo���b�g�̒��e�_���Z�o����
	for (int CntPlayer=0;CntPlayer<PLAYER_MAX;CntPlayer++)
	{
		D3DXVECTOR3	BulletPredictionPos = player[CntPlayer].BposStart;
		D3DXCOLOR	col = PLAYER_COLOR[CntPlayer];
		float Gravity = 0.0f;
		float time = 1.0f;
		float maxtime = 10.0f;
		do
		{
			//�d�͂ƈړ��ʂ𔽉f
			//BulletPredictionPos.x += player[CntPlayer].bulletmove.x*(time / maxtime);
			//BulletPredictionPos.y -= player[CntPlayer].bulletmove.y*(time / maxtime) + Gravity;
			//BulletPredictionPos.z += player[CntPlayer].bulletmove.z*(time / maxtime);

			BulletPredictionPos.x += player[CntPlayer].bulletmove.x;
			BulletPredictionPos.y -= player[CntPlayer].bulletmove.y + Gravity;
			BulletPredictionPos.z += player[CntPlayer].bulletmove.z;

			//�d�͂�����������
			Gravity += VALUE_GRAVITYADD_BULLET;
			//���Ԋ�����i�߂�B�򋗗���������قǒe���\���Ԋu���L���Ȃ�
			time++;
			//�d�͍ő�l����
			if (Gravity > VALUE_GRAVITYMAX_BULLET) Gravity = VALUE_GRAVITYMAX_BULLET;
			//���X�ɃA���t�@�l���������ĉ������n�_�����₷������
			col.a += 0.01f;
			SetBulletprediction(BulletPredictionPos, CntPlayer, col, BULLETPREDICTION_SIZE_X, BULLETPREDICTION_SIZE_Y);


		} while (BulletPredictionPos.y >= 0.0f);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBulletprediction(int CntPlayer)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();
	if (p[CntPlayer].use)
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
			if (g_aBulletprediction[CntPlayer][nCntBulletprediction].bUse)
			{
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld);

				// �r���[�}�g���b�N�X���擾
				CAMERA *cam = GetCamera();

				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._11 = cam[CntPlayer].mtxView._11;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._12 = cam[CntPlayer].mtxView._21;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._13 = cam[CntPlayer].mtxView._31;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._21 = cam[CntPlayer].mtxView._12;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._22 = cam[CntPlayer].mtxView._22;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._23 = cam[CntPlayer].mtxView._32;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._31 = cam[CntPlayer].mtxView._13;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._32 = cam[CntPlayer].mtxView._23;
				g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld._33 = cam[CntPlayer].mtxView._33;

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScale, g_aBulletprediction[CntPlayer][nCntBulletprediction].scale.x, g_aBulletprediction[CntPlayer][nCntBulletprediction].scale.y, g_aBulletprediction[CntPlayer][nCntBulletprediction].scale.z);
				D3DXMatrixMultiply(&g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &mtxScale);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, g_aBulletprediction[CntPlayer][nCntBulletprediction].pos.x, g_aBulletprediction[CntPlayer][nCntBulletprediction].pos.y, g_aBulletprediction[CntPlayer][nCntBulletprediction].pos.z);
				D3DXMatrixMultiply(&g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aBulletprediction[CntPlayer][nCntBulletprediction].mtxWorld);

				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletprediction[CntPlayer], 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureBulletprediction[CntPlayer]);

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
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * MAX_BULLETPREDICTION,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&g_pD3DVtxBuffBulletprediction[CntPlayer],						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))										// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pD3DVtxBuffBulletprediction[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

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
			g_pD3DVtxBuffBulletprediction[CntPlayer]->Unlock();
		}
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletprediction[PlayerType]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletprediction[PlayerType]->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletprediction[PlayerType]->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxBulletprediction * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletprediction[PlayerType]->Unlock();
	}
}

//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int SetBulletprediction(D3DXVECTOR3 pos, int PlayerType,D3DXCOLOR col, float fSizeX, float fSizeY)
{
	int nIdxBulletprediction = -1;

	for (int nCntBulletprediction = 0; nCntBulletprediction < MAX_BULLETPREDICTION; nCntBulletprediction++)
	{
		if (!g_aBulletprediction[PlayerType][nCntBulletprediction].bUse)
		{
			g_aBulletprediction[PlayerType][nCntBulletprediction].pos = pos;
			g_aBulletprediction[PlayerType][nCntBulletprediction].col = col;
			g_aBulletprediction[PlayerType][nCntBulletprediction].fSizeX = fSizeX;
			g_aBulletprediction[PlayerType][nCntBulletprediction].fSizeY = fSizeY;
			g_aBulletprediction[PlayerType][nCntBulletprediction].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexBulletprediction(PlayerType,nCntBulletprediction, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorBulletprediction(PlayerType,nCntBulletprediction,
				D3DXCOLOR(g_aBulletprediction[PlayerType][nCntBulletprediction].col.r,g_aBulletprediction[PlayerType][nCntBulletprediction].col.g,
					g_aBulletprediction[PlayerType][nCntBulletprediction].col.b, g_aBulletprediction[PlayerType][nCntBulletprediction].col.a));

			nIdxBulletprediction = nCntBulletprediction;

			break;
		}
	}
	return nIdxBulletprediction;
}
