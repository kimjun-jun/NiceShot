///**
//* @file life.cpp
//* @brief NiceShot(3D)��ԃQ�[��
//* @author �L�����W����
//*/
//#include "../../h/main.h"
//#include "../../h/object/player.h"
//#include "../../h/other/sound.h"
//#include "../../h/scene/rank.h"
//#include "../../h/object/life.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define	TEXTURE_LIFETEX		"../data/TEXTURE/life000.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//HRESULT MakeVertexLifeTex(LPDIRECT3DDEVICE9 pDevice, int type);
//
////*****************************************************************************
//// �O���[�o���ϐ��錾
////*****************************************************************************
//LPDIRECT3DTEXTURE9		g_pD3DTextureLifeTex;		// �e�N�X�`���ւ̃|�C���^
//
//LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLifeTex[PLAYER_MAX];	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//
//D3DXVECTOR3				g_posLifeTex;						// �ʒu
//D3DXVECTOR3				g_rotLifeTex;						// ����
//
//int						g_nLifeTex[PLAYER_MAX];			// �o���b�g�c�e
//int						g_nNumVertexLifeTex;			// ���_�̑���
//int						g_nNumPolygonLifeTex;			// �|���S���̑���
//int						g_nNumIndexLifeTex;				// �C���f�b�N�X�̑���
//
//
////=============================================================================
//// ����������
////=============================================================================
//HRESULT InitLifeTex(int type)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	if (type == 0)
//	{
//		// �e�N�X�`���̓ǂݍ���
//		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
//			TEXTURE_LIFETEX,			// �t�@�C���̖��O
//			&g_pD3DTextureLifeTex);	// �ǂݍ��ރ������[
//	}
//
//	g_nNumVertexLifeTex = (POLYGON_2D_VERTEX * MAX_LIFE) - 2;
//	g_nNumPolygonLifeTex = MAX_LIFE * 2;
//	g_nNumIndexLifeTex = g_nNumPolygonLifeTex;
//	g_posLifeTex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	g_rotLifeTex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		// �o���b�g�c�e�̏�����
//		g_nLifeTex[CntPlayer] = MAX_LIFE;
//		g_pD3DVtxBuffLifeTex[CntPlayer] = NULL;
//		// ���_���̍쐬
//		MakeVertexLifeTex(pDevice, CntPlayer);
//
//	}
//
//
//	return S_OK;
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void UninitLifeTex(void)
//{
//	if (g_pD3DTextureLifeTex != NULL)
//	{// �e�N�X�`���̊J��
//		g_pD3DTextureLifeTex->Release();
//		g_pD3DTextureLifeTex = NULL;
//	}
//
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		if (g_pD3DVtxBuffLifeTex[CntPlayer] != NULL)
//		{// ���_�o�b�t�@�̊J��
//			g_pD3DVtxBuffLifeTex[CntPlayer]->Release();
//			g_pD3DVtxBuffLifeTex[CntPlayer] = NULL;
//		}
//	}
//}
//
////=============================================================================
//// �X�V����
////=============================================================================
//void UpdateLifeTex(void)
//{
//	PLAYER_HONTAI *p = GetPlayerHoudai();
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		if (p[CntPlayer].use == true)
//		{
//			if (g_nLifeTex[CntPlayer] <= 0)
//			{
//				p[CntPlayer].use = false;
//				SetRank(CntPlayer);
//			}
//		}
//	}
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void DrawLifeTex(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
//	{
//		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
//		pDevice->SetStreamSource(0, g_pD3DVtxBuffLifeTex[CntPlayer], 0, sizeof(VERTEX_2D));
//
//		// ���_�t�H�[�}�b�g�̐ݒ�
//		pDevice->SetFVF(FVF_VERTEX_2D);
//
//		// �e�N�X�`���̐ݒ�
//		pDevice->SetTexture(0, g_pD3DTextureLifeTex);
//
//		// �|���S���̕`��
//		for (int nCntPlace = 0; nCntPlace < g_nLifeTex[CntPlayer]; nCntPlace++)
//		{
//			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
//		}
//	}
//}
//
////=============================================================================
//// ���_�̍쐬
////=============================================================================
//HRESULT MakeVertexLifeTex(LPDIRECT3DDEVICE9 pDevice, int type)
//{
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * MAX_LIFE + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
//		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
//		FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
//		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
//		&g_pD3DVtxBuffLifeTex[type],							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//		NULL)))										// NULL�ɐݒ�
//	{
//		return E_FAIL;
//	}
//
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_2D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffLifeTex[type]->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int CntPlace = 0; CntPlace < MAX_LIFE; CntPlace++, pVtx += 4)
//		{
//			//1�v���C���[�̎��͉�ʍ���
//			if (type == PLAYER01)
//			{
//				// ���_���W�̐ݒ�
//				pVtx[0].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//			}
//			//2�v���C���[�̎��͉�ʉE��
//			else if (type == PLAYER02)
//			{
//				// ���_���W�̐ݒ�
//				pVtx[0].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y, 0.0f);
//			}
//			else if (type == PLAYER03)
//			{
//				// ���_���W�̐ݒ�
//				pVtx[0].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3(LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//			}
//			else if (type == PLAYER04)
//			{
//				// ���_���W�̐ݒ�
//				pVtx[0].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[1].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[2].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//				pVtx[3].vtx = D3DXVECTOR3((LIFETEX_POS_X + CntPlace * LIFETEX_SIZE_X) + SCREEN_CENTER_X + LIFETEX_SIZE_X, LIFETEX_POS_Y + LIFETEX_SIZE_Y + SCREEN_CENTER_Y, 0.0f);
//			}
//			// rhw�̐ݒ�
//			pVtx[0].rhw =
//				pVtx[1].rhw =
//				pVtx[2].rhw =
//				pVtx[3].rhw = 1.0f;
//
//			// ���ˌ��̐ݒ�
//			pVtx[0].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);//�s���N�F
//			pVtx[1].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(0.96f, 0.56f, 0.59f, 1.0f);
//
//			// �e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffLifeTex[type]->Unlock();
//	}
//	return S_OK;
//}
//
////=============================================================================
//// �o���b�g�c�e�̕ύX
////=============================================================================
//void ChangeLife(int nValue, int type)
//{
//	g_nLifeTex[type] += nValue;
//	if (g_nLifeTex[type] < 0)
//	{
//		g_nLifeTex[type] = 0;
//	}
//	else if (g_nLifeTex[type] >= MAX_LIFE)
//	{
//		g_nLifeTex[type] = MAX_LIFE;
//	}
//
//}
