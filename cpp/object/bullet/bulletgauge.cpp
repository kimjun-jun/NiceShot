/**
* @file bulletgauge.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/object/player.h"
#include "../../../h/object/bullet/bulletgauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLETGAUGE			"../data/TEXTURE/UI_BULLETGAUGE.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLETGAUGE_SIZE_X			(15.0f)										// �A�^�b�N�Q�[�W�̐����̕�
#define	BULLETGAUGE_SIZE_Y			(20.0f)										// �A�^�b�N�Q�[�W�̐����̍���
#define	BULLETGAUGE_POS_X			(18.0f)										// �A�^�b�N�Q�[�W�̕\����ʒu�w���W
#define	BULLETGAUGE_POS_Y			(30.0f)										// �A�^�b�N�Q�[�W�̕\����ʒu�x���W

#define BULLETGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletGauge = {NULL};						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletGauge[PLAYER_MAX] = { NULL };		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXVECTOR3				g_posBulletGauge[PLAYER_MAX];							// �ʒu
int						g_nBulletGauge[PLAYER_MAX];								// �e��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posBulletGauge[0] = D3DXVECTOR3(BULLETGAUGE_P1_POS_X, BULLETGAUGE_P1_POS_Y, 0.0f);
	g_posBulletGauge[1] = D3DXVECTOR3(BULLETGAUGE_P2_POS_X, BULLETGAUGE_P2_POS_Y, 0.0f);
	g_posBulletGauge[2] = D3DXVECTOR3(BULLETGAUGE_P3_POS_X, BULLETGAUGE_P3_POS_Y, 0.0f);
	g_posBulletGauge[3] = D3DXVECTOR3(BULLETGAUGE_P4_POS_X, BULLETGAUGE_P4_POS_Y, 0.0f);

	// �e��̏�����
	g_nBulletGauge[0] = PLAYER_AMMOPOWER_NORMAL;
	g_nBulletGauge[1] = PLAYER_AMMOPOWER_NORMAL;
	g_nBulletGauge[2] = PLAYER_AMMOPOWER_NORMAL;
	g_nBulletGauge[3] = PLAYER_AMMOPOWER_NORMAL;

	// ���_���̍쐬
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		MakeVertexBulletGauge(pDevice,CntPlayer);
	}
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_BULLETGAUGE,			// �t�@�C���̖��O
								&g_pD3DTextureBulletGauge);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBulletGauge(void)
{
	if (g_pD3DTextureBulletGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBulletGauge->Release();
		g_pD3DTextureBulletGauge = NULL;
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DVtxBuffBulletGauge[CntPlayer] != NULL)
		{// ���_�o�b�t�@�̊J��
			g_pD3DVtxBuffBulletGauge[CntPlayer]->Release();
			g_pD3DVtxBuffBulletGauge[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBulletGauge(void)
{
	PLAYER_HONTAI *p =GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
			g_nBulletGauge[CntPlayer] = p[CntPlayer].AmmoNum;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBulletGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletGauge[CntPlayer], 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureBulletGauge);

		// �|���S���̕`��
		for (int nCntPlace = 0; nCntPlace < g_nBulletGauge[CntPlayer]; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_AMMOPOWER_STRONG + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffBulletGauge[CntPlayer],							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < PLAYER_AMMOPOWER_STRONG; nCntPlace++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			if (CntPlayer == 0)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X *2, -BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X *2, BULLETGAUGE_SIZE_Y/2, 0.0f) + g_posBulletGauge[0];
			}
			else if (CntPlayer == 1)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[1];
			}
			else if (CntPlayer == 2)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[2];
			}
			else if (CntPlayer == 3)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, -BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X * 2, BULLETGAUGE_SIZE_Y / 2, 0.0f) + g_posBulletGauge[3];
			}

			// rhw�̐ݒ�
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffBulletGauge[CntPlayer]->Unlock();
	}

	return S_OK;
}

