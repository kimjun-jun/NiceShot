//=============================================================================
//
// �A�^�b�N�Q�[�W���� [bulletgauge.cpp]
// Author : 
//
//=============================================================================
#include "../../../h/main.h"
#include "../../../h/object/player.h"
#include "../../../h/object/bullet/bulletgauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_ATTACKGAUGE		"../data/TEXTURE/UI_ATTACKGAUGE.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	ATTACKGAUGE_SIZE_X			(0.2f)							// �A�^�b�N�Q�[�W�̐����̕�
#define	ATTACKGAUGE_SIZE_Y			(20.0f)							// �A�^�b�N�Q�[�W�̐����̍���
#define	ATTACKGAUGE_POS_X			(20.0f)							// �A�^�b�N�Q�[�W�̕\����ʒu�w���W
#define	ATTACKGAUGE_POS_Y			(225.0f)							// �A�^�b�N�Q�[�W�̕\����ʒu�x���W

#define ATTACKGAUGE_P1_POS_X		(SCREEN_W * 1 / 2 - (SCREEN_SEPARATE_BUFF)) - ATTACKGAUGE_POS_X
#define ATTACKGAUGE_P1_POS_Y		(SCREEN_H * 1 / 2) + ATTACKGAUGE_POS_Y
#define ATTACKGAUGE_P2_POS_X		(SCREEN_W * 4 / 4 - (SCREEN_SEPARATE_BUFF)) - ATTACKGAUGE_POS_X
#define ATTACKGAUGE_P2_POS_Y		(SCREEN_H * 1 / 2) + ATTACKGAUGE_POS_Y


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexAttackGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureAttackGauge = {NULL};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffAttackGauge[PLAYER_MAX] = { NULL };		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXVECTOR3				g_posAttackGauge[PLAYER_MAX];						// �ʒu

int						g_nAttackGauge[PLAYER_MAX];							// ���C�t

//=============================================================================
// ����������
//=============================================================================
HRESULT InitAttackGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posAttackGauge[0] = D3DXVECTOR3(ATTACKGAUGE_P1_POS_X, ATTACKGAUGE_P1_POS_Y, 0.0f);
	g_posAttackGauge[1] = D3DXVECTOR3(ATTACKGAUGE_P2_POS_X, ATTACKGAUGE_P2_POS_Y, 0.0f);

	// ���C�t�̏�����
	//g_nAttackGauge[0] = PLAYER_VALUE_POWER;
	//g_nAttackGauge[1] = PLAYER_VALUE_POWER;

	// ���_���̍쐬
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		MakeVertexAttackGauge(pDevice,CntPlayer);
	}
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_ATTACKGAUGE,			// �t�@�C���̖��O
								&g_pD3DTextureAttackGauge);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAttackGauge(void)
{
	if (g_pD3DTextureAttackGauge != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureAttackGauge->Release();
		g_pD3DTextureAttackGauge = NULL;
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_pD3DVtxBuffAttackGauge[CntPlayer] != NULL)
		{// ���_�o�b�t�@�̊J��
			g_pD3DVtxBuffAttackGauge[CntPlayer]->Release();
			g_pD3DVtxBuffAttackGauge[CntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateAttackGauge(void)
{
	//PLAYER *p =GetPlayer();
	//for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	//{
	//	if (p[CntPlayer].TeamFlag == TEAMTYPE_HIT)
	//	{
	//		g_nAttackGauge[CntPlayer] = p[CntPlayer].power;
	//	}
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawAttackGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//PLAYER *p = GetPlayer();
		//if (p[CntPlayer].TeamFlag == TEAMTYPE_HIT)
		//{
		//	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		//	pDevice->SetStreamSource(0, g_pD3DVtxBuffAttackGauge[CntPlayer], 0, sizeof(VERTEX_2D));

		//	// ���_�t�H�[�}�b�g�̐ݒ�
		//	pDevice->SetFVF(FVF_VERTEX_2D);

		//	// �e�N�X�`���̐ݒ�
		//	pDevice->SetTexture(0, g_pD3DTextureAttackGauge);

		//	// �|���S���̕`��
		//	for (int nCntPlace = 0; nCntPlace < g_nAttackGauge[CntPlayer]; nCntPlace++)
		//	{
		//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		//	}
		//}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexAttackGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
 //   if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_VALUE_POWER + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
	//											D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
	//											FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
	//											D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
	//											&g_pD3DVtxBuffAttackGauge[CntPlayer],							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	//											NULL)))										// NULL�ɐݒ�
	//{
 //       return E_FAIL;
	//}

	//{//���_�o�b�t�@�̒��g�𖄂߂�
	//	VERTEX_2D *pVtx;

	//	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	//	g_pD3DVtxBuffAttackGauge[CntPlayer]->Lock(0, 0, (void**)&pVtx, 0);

	//	for(int nCntPlace = 0; nCntPlace < PLAYER_VALUE_POWER; nCntPlace++, pVtx += 4)
	//	{
	//		// ���_���W�̐ݒ�
	//		if (CntPlayer == 0)
	//		{
	//			pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, -ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//			pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X*2, -ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//			pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//			pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X*2, ATTACKGAUGE_SIZE_Y/2, 0.0f) + g_posAttackGauge[0];
	//		}
	//		else if (CntPlayer == 1)
	//		{
	//			pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, -ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1];
	//			pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X * 2, -ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1];
	//			pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X, ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1];
	//			pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * ATTACKGAUGE_SIZE_X * 2, ATTACKGAUGE_SIZE_Y / 2, 0.0f) + g_posAttackGauge[1
	//			];
	//		}
	//		// rhw�̐ݒ�
	//		pVtx[0].rhw =
	//		pVtx[1].rhw =
	//		pVtx[2].rhw =
	//		pVtx[3].rhw = 1.0f;

	//		// ���ˌ��̐ݒ�
	//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//		// �e�N�X�`�����W�̐ݒ�
	//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//	}


	//	// ���_�f�[�^���A�����b�N����
	//	g_pD3DVtxBuffAttackGauge[CntPlayer]->Unlock();
	//}

	return S_OK;
}

