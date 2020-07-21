///**
//* @file bullet.cpp
//* @brief NiceShot(3D)��ԃQ�[��
//* @author �L�����W����
//* @date 2020/01/15
//*/
//#include "main.h"
//#include "bulletprediction.h"
//#include "input.h"
//#include "camera.h"
//#include "shadow.h"
//#include "effect.h"
//#include "explosion.h"
//#include "player.h"
//#include "life.h"
//#include "collision.h"
//#include "sound.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define	TEXTURE_BULLET_PREDICTION			"data/TEXTURE/bullet000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
//
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//HRESULT MakeVertexBulletPrediction(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexBulletPrediction(int nIdxBulletPrediction, float fSizeX, float fSizeY);
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//LPDIRECT3DTEXTURE9		g_pD3DTextureBulletPrediction = NULL;		// �e�N�X�`���ւ̃|�C���^
//LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletPrediction = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//
//D3DXMATRIX				g_mtxWorldBulletPrediction;				// ���[���h�}�g���b�N�X
//
//BULLET_PREDICTION					g_aBulletPrediction[BULLET_PREDICTION_MAX];			// �e���[�N
//
//static D3DXCOLOR PLAYER_COLOR[] = {
//	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
//	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
//	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
//	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
//};
//
////=============================================================================
//// ����������
////=============================================================================
//HRESULT InitBulletPrediction(int type)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	// ���_���̍쐬
//	MakeVertexBulletPrediction(pDevice);
//
//	if (type == 0)
//	{
//		// �e�N�X�`���̓ǂݍ���
//		D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
//			TEXTURE_BULLET_PREDICTION,				// �t�@�C���̖��O
//			&g_pD3DTextureBulletPrediction);	// �ǂݍ��ރ������[
//	}
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		g_aBulletPrediction[nCntBulletPrediction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//		g_aBulletPrediction[nCntBulletPrediction].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].Hormingmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aBulletPrediction[nCntBulletPrediction].fSizeX = BULLET_PREDICTION_SIZE_X;
//		g_aBulletPrediction[nCntBulletPrediction].fSizeY = BULLET_PREDICTION_SIZE_Y;
//		g_aBulletPrediction[nCntBulletPrediction].nTimer = 0;
//		g_aBulletPrediction[nCntBulletPrediction].UsePlayerType = -1;
//		g_aBulletPrediction[nCntBulletPrediction].HormingPlayerType = -1;
//		g_aBulletPrediction[nCntBulletPrediction].use = false;
//		g_aBulletPrediction[nCntBulletPrediction].HormingSignal = false;
//		g_aBulletPrediction[nCntBulletPrediction].fRadius = 10.0f;
//		g_aBulletPrediction[nCntBulletPrediction].GravityAdd = 1.1f;
//		g_aBulletPrediction[nCntBulletPrediction].Gravity = 0.01f;
//		g_aBulletPrediction[nCntBulletPrediction].FieldPosY = 0.1f;
//		g_aBulletPrediction[nCntBulletPrediction].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_PlayerHoudai[CntPlayer].pD3DTexture = NULL;
//		g_PlayerHoudai[CntPlayer].pD3DXMesh = NULL;
//		g_PlayerHoudai[CntPlayer].pD3DXBuffMat = NULL;
//		g_PlayerHoudai[CntPlayer].nNumMat = 0;
//		g_PlayerHoudai[CntPlayer].pD3DVtxBuff = NULL;
//		g_PlayerHoudai[CntPlayer].pD3DIdxBuff = NULL;
//		g_PlayerHoudai[CntPlayer].nNumVertex = 0;
//		g_PlayerHoudai[CntPlayer].nNumPolygon = 0;
//		g_PlayerHoudai[CntPlayer].nNumVertexIndex = 0;
//
//
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void UninitBulletPrediction(void)
//{
//	if (g_pD3DTextureBulletPrediction != NULL)
//	{// �e�N�X�`���̊J��
//		g_pD3DTextureBulletPrediction->Release();
//		g_pD3DTextureBulletPrediction = NULL;
//	}
//
//	if (g_pD3DVtxBuffBulletPrediction != NULL)
//	{// ���_�o�b�t�@�̊J��
//		g_pD3DVtxBuffBulletPrediction->Release();
//		g_pD3DVtxBuffBulletPrediction = NULL;
//	}
//}
//
////=============================================================================
//// �X�V����
////=============================================================================
//void UpdateBulletPrediction(void)
//{
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		if (g_aBulletPrediction[nCntBulletPrediction].use)
//		{
//		}
//	}
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void DrawBulletPrediction(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
//
//	// ���C�e�B���O�𖳌���
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// ���e�X�g��L����
//	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
//	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		if (g_aBulletPrediction[nCntBulletPrediction].use)
//		{
//			// ���[���h�}�g���b�N�X�̏�����
//			D3DXMatrixIdentity(&g_mtxWorldBulletPrediction);
//
//			// �r���[�}�g���b�N�X���擾
//			mtxView = GetCamera()->mtxView;
//
//			g_mtxWorldBulletPrediction._11 = mtxView._11;
//			g_mtxWorldBulletPrediction._12 = mtxView._21;
//			g_mtxWorldBulletPrediction._13 = mtxView._31;
//			g_mtxWorldBulletPrediction._21 = mtxView._12;
//			g_mtxWorldBulletPrediction._22 = mtxView._22;
//			g_mtxWorldBulletPrediction._23 = mtxView._32;
//			g_mtxWorldBulletPrediction._31 = mtxView._13;
//			g_mtxWorldBulletPrediction._32 = mtxView._23;
//			g_mtxWorldBulletPrediction._33 = mtxView._33;
//
//			// �X�P�[���𔽉f
//			D3DXMatrixScaling(&mtxScale, g_aBulletPrediction[nCntBulletPrediction].scale.x, g_aBulletPrediction[nCntBulletPrediction].scale.y, g_aBulletPrediction[nCntBulletPrediction].scale.z);
//			D3DXMatrixMultiply(&g_mtxWorldBulletPrediction, &g_mtxWorldBulletPrediction, &mtxScale);
//
//			// �ړ��𔽉f
//			D3DXMatrixTranslation(&mtxTranslate, g_aBulletPrediction[nCntBulletPrediction].pos.x, g_aBulletPrediction[nCntBulletPrediction].pos.y, g_aBulletPrediction[nCntBulletPrediction].pos.z);
//			D3DXMatrixMultiply(&g_mtxWorldBulletPrediction, &g_mtxWorldBulletPrediction, &mtxTranslate);
//
//			// ���[���h�}�g���b�N�X�̐ݒ�
//			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBulletPrediction);
//
//			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
//			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletPrediction, 0, sizeof(VERTEX_3D));
//
//			// ���_�t�H�[�}�b�g�̐ݒ�
//			pDevice->SetFVF(FVF_VERTEX_3D);
//
//			// �e�N�X�`���̐ݒ�
//			pDevice->SetTexture(0, g_pD3DTextureBulletPrediction);
//
//			// �|���S���̕`��
//			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBulletPrediction * 4), POLYGON_2D_NUM);
//		}
//	}
//
//	// ���C�e�B���O��L����
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//	// ���e�X�g�𖳌���
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//}
//
////=============================================================================
//// ���_���̍쐬
////=============================================================================
//HRESULT MakeVertexBulletPrediction(LPDIRECT3DDEVICE9 pDevice)
//{
//	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * BULLET_PREDICTION_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
//		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
//		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
//		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
//		&g_pD3DVtxBuffBulletPrediction,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//		NULL)))										// NULL�ɐݒ�
//	{
//		return E_FAIL;
//	}
//
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_3D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffBulletPrediction->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++, pVtx += 4)
//		{
//			// ���_���W�̐ݒ�
//			pVtx[0].vtx = D3DXVECTOR3(-BULLET_PREDICTION_SIZE_X / 2, -BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//			pVtx[1].vtx = D3DXVECTOR3(BULLET_PREDICTION_SIZE_X / 2, -BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//			pVtx[2].vtx = D3DXVECTOR3(-BULLET_PREDICTION_SIZE_X / 2, BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//			pVtx[3].vtx = D3DXVECTOR3(BULLET_PREDICTION_SIZE_X / 2, BULLET_PREDICTION_SIZE_Y / 2, 0.0f);
//
//			// �@���̐ݒ�
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//
//			// ���ˌ��̐ݒ�
//			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// �e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffBulletPrediction->Unlock();
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// ���_���W�̐ݒ�
////=============================================================================
//void SetVertexBulletPrediction(int nIdxBulletPrediction, float fSizeX, float fSizeY)
//{
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_3D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffBulletPrediction->Lock(0, 0, (void**)&pVtx, 0);
//
//		pVtx += (nIdxBulletPrediction * 4);
//
//		// ���_���W�̐ݒ�
//		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffBulletPrediction->Unlock();
//	}
//}
//
////=============================================================================
//// �e�̐ݒ�
////=============================================================================
//int SetBulletPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, int type)
//{
//	int nIdxBulletPrediction = -1;
//
//	for (int nCntBulletPrediction = 0; nCntBulletPrediction < BULLET_PREDICTION_MAX; nCntBulletPrediction++)
//	{
//		if (!g_aBulletPrediction[nCntBulletPrediction].use)
//		{
//			g_aBulletPrediction[nCntBulletPrediction].pos = pos;
//			g_aBulletPrediction[nCntBulletPrediction].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			g_aBulletPrediction[nCntBulletPrediction].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//			g_aBulletPrediction[nCntBulletPrediction].Gravity = 0.01f;
//			g_aBulletPrediction[nCntBulletPrediction].move = move;
//			g_aBulletPrediction[nCntBulletPrediction].Hormingmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//			g_aBulletPrediction[nCntBulletPrediction].fSizeX = fSizeX;
//			g_aBulletPrediction[nCntBulletPrediction].fSizeY = fSizeY;
//			g_aBulletPrediction[nCntBulletPrediction].nTimer = nTimer;
//			g_aBulletPrediction[nCntBulletPrediction].use = true;
//			g_aBulletPrediction[nCntBulletPrediction].UsePlayerType = type;
//			g_aBulletPrediction[nCntBulletPrediction].HormingSignal = false;
//			g_aBulletPrediction[nCntBulletPrediction].HormingPlayerType = -1;
//
//
//			// �e�̐ݒ�
//			g_aBulletPrediction[nCntBulletPrediction].nIdxShadow = CreateShadow(pos, g_aBulletPrediction[nCntBulletPrediction].scale);		// �e�̐ݒ�
//
//			// ���_���W�̐ݒ�
//			SetVertexBulletPrediction(nCntBulletPrediction, fSizeX, fSizeY);
//
//			nIdxBulletPrediction = nCntBulletPrediction;
//
//			break;
//		}
//	}
//
//	return nIdxBulletPrediction;
//}
//
////=============================================================================
//// �e�̍폜
////=============================================================================
//void ReleaseBulletPrediction(int nIdxBulletPrediction)
//{
//	if (nIdxBulletPrediction >= 0 && nIdxBulletPrediction < BULLET_PREDICTION_MAX)
//	{
//		ReleaseShadow(g_aBulletPrediction[nIdxBulletPrediction].nIdxShadow);
//		g_aBulletPrediction[nIdxBulletPrediction].use = false;
//	}
//}
//
////=============================================================================
//// �e�̃f�[�^�擾
////=============================================================================
//BULLET_PREDICTION *GetBulletPrediction(void)
//{
//	return &g_aBulletPrediction[0];
//}
//
