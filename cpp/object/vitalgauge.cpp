/**
* @file vitalgauge.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/object/player.h"
#include "../../h/scene/rank.h"
#include "../../../h/object/vitalgauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_VITALGAUGE		"../data/TEXTURE/UI_VITALGAUGE.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VITALGAUGE_SIZE_X			(1.0f)							// �A�^�b�N�Q�[�W�̐����̕�
#define	VITALGAUGE_SIZE_Y			(30.0f)							// �A�^�b�N�Q�[�W�̐����̍���
#define	VITALGAUGE_POS_X			(25.0f)							// �A�^�b�N�Q�[�W�̕\����ʒu�w���W
#define	VITALGAUGE_POS_Y			(70.0f)							// �A�^�b�N�Q�[�W�̕\����ʒu�x���W

#define VITALGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y
#define VITALGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_X
#define VITALGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_POS_Y


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureVitalGauge = {NULL};					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffVitalGauge[PLAYER_MAX] = { NULL };		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXVECTOR3				g_posVitalGauge[PLAYER_MAX];						// �ʒu
int						g_nVitalGauge[PLAYER_MAX];							// ���C�t

//=============================================================================
// ����������
//=============================================================================
HRESULT InitVitalGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posVitalGauge[0] = D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f);
	g_posVitalGauge[1] = D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f);
	g_posVitalGauge[2] = D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f);
	g_posVitalGauge[3] = D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f);

	// ���C�t�̏�����
	g_nVitalGauge[0] = PLAYER_VITAL;
	g_nVitalGauge[1] = PLAYER_VITAL;
	g_nVitalGauge[2] = PLAYER_VITAL;
	g_nVitalGauge[3] = PLAYER_VITAL;

	// ���_���̍쐬
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		MakeVertexVitalGauge(pDevice,CntPlayer);
	}
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_VITALGAUGE,			// �t�@�C���̖��O
								&g_pD3DTextureVitalGauge);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitVitalGauge(void)
{
	if (g_pD3DTextureVitalGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureVitalGauge->Release();
		g_pD3DTextureVitalGauge = NULL;
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DVtxBuffVitalGauge[CntPlayer] != NULL)
		{// ���_�o�b�t�@�̊J��
			g_pD3DVtxBuffVitalGauge[CntPlayer]->Release();
			g_pD3DVtxBuffVitalGauge[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateVitalGauge(void)
{
	PLAYER_HONTAI *p = GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (p[CntPlayer].use == true)
		{
			if (g_nVitalGauge[CntPlayer] <= 0)
			{
				p[CntPlayer].use = false;
				SetRank(CntPlayer);
			}
		}
		g_nVitalGauge[CntPlayer] = p[CntPlayer].vital;
		//�̗͈��ȏ�ŗΐF
		if (g_nVitalGauge[CntPlayer] >= PLAYER_VITAL * (0.2f))
		{
			{//���_�o�b�t�@�̒��g�𖄂߂�
				VERTEX_2D *pVtx;
				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
				g_pD3DVtxBuffVitalGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// ���ˌ��̐ݒ�
					pVtx[0].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(0.7f, 1.0f, 0.7f, 1.0f);
				}
			}
			// ���_�f�[�^���A�����b�N����
			g_pD3DVtxBuffVitalGauge[CntPlayer]->Unlock();
		}
		//�̗͈��ȉ��ŐԐF
		else
		{
			{//���_�o�b�t�@�̒��g�𖄂߂�
				VERTEX_2D *pVtx;
				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
				g_pD3DVtxBuffVitalGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// ���ˌ��̐ݒ�
					pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
				}
			}
			// ���_�f�[�^���A�����b�N����
			g_pD3DVtxBuffVitalGauge[CntPlayer]->Unlock();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawVitalGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffVitalGauge[CntPlayer], 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureVitalGauge);

		// �|���S���̕`��
		for (int nCntPlace = 0; nCntPlace < g_nVitalGauge[CntPlayer]; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_VITAL + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffVitalGauge[CntPlayer],							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffVitalGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			if (CntPlayer == 0)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, -VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, VITALGAUGE_SIZE_Y/2, 0.0f) + g_posVitalGauge[0];
			}
			else if (CntPlayer == 1)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[1];
			}
			else if (CntPlayer == 2)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[2];
			}
			else if (CntPlayer == 3)
			{
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, -VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X * 2, VITALGAUGE_SIZE_Y / 2, 0.0f) + g_posVitalGauge[3];
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
		g_pD3DVtxBuffVitalGauge[CntPlayer]->Unlock();
	}

	return S_OK;
}

