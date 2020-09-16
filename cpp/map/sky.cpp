/**
* @file field.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/sky.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	"../data/TEXTURE/sky001.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_SKY		(5.0f)							// �ړ����x
#define	VALUE_ROTATE_SKY	(D3DX_PI * 0.001f)				// ��]���x

#define	SKY_HEIGHT_RATE		(2.0f)		// ��h�[���̍����W��

#define	TEX_COUNT_LOOP		(1)			// �e�N�X�`���̌J��Ԃ���

#define	MAX_MESH_SKY		(2)			// ��̑���

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffTop;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuffTop;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;						// �|���S���\���ʒu�̒��S���W
	D3DXVECTOR3 rot;						// �|���S���̉�]�p

	int nNumBlockH;							// �u���b�N��(����)
	int nNumBlockV;							// �u���b�N��(����)
	int nNumVertex;							// �����_��	
	int nNumVertexIndex;					// ���C���f�b�N�X��
	int nNumPolygon;						// ���|���S����
	float fRadius;							// ���a
	int nTexType;							// �e�N�X�`���^�C�v
	float fRotY;							// ��]��

	bool bReverse;							// ���]ON/OFF
} MESH_SKY;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSky = NULL;	// �e�N�X�`���ǂݍ��ݏꏊ
MESH_SKY g_aMeshSky[2] = {};				// �󃏁[�N
int g_nNumMeshSky = 0;						// ��̐�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockH, int nNumBlockV,
						float fRadius, float fRotY, int nTexType, bool bReverse)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_SKY *pMesh;

	if(g_nNumMeshSky >= MAX_MESH_SKY)
	{
		return E_FAIL;
	}

	pMesh = &g_aMeshSky[g_nNumMeshSky];
	g_nNumMeshSky++;

	pMesh->bReverse = bReverse;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	pMesh->pos = pos;
	pMesh->rot = rot;
	pMesh->fRadius = fRadius;

	// �e�N�X�`���̓ǂݍ���
	if(nTexType == -1)
	{
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_FILENAME,	// �t�@�C���̖��O
									&g_pTextureSky);	// �ǂݍ��ރ������[
	}
	pMesh->nTexType = nTexType;

	// �u���b�N���̐ݒ�
	pMesh->nNumBlockH = nNumBlockH;
	pMesh->nNumBlockV = nNumBlockV;

	// ���_���̐ݒ�
	pMesh->nNumVertex = (nNumBlockH + 1) * (nNumBlockV + 1);

	// �C���f�b�N�X���̐ݒ�
	pMesh->nNumVertexIndex = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);

	// �|���S�����̐ݒ�
	pMesh->nNumPolygon = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);

	// ��]��
	pMesh->fRotY = fRotY;
	
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * pMesh->nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&pMesh->pVtxBuff,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (pMesh->nNumBlockH + 1),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&pMesh->pVtxBuffTop,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))									// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * pMesh->nNumVertexIndex,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&pMesh->pIdxBuff,					// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * (pMesh->nNumBlockH + 2),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&pMesh->pIdxBuffTop,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		const float ANGLE_H = (D3DX_PI * 2.0f) / pMesh->nNumBlockH;
		const float ANGLE_V = (D3DX_PI / 8.0f) / (pMesh->nNumBlockV + 1);
		const float WIDTH = 1.0f / pMesh->nNumBlockH;
		const float HEIGHT = 1.0f / pMesh->nNumBlockV;
		float fLengthXZ;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntV = 0; nCntV < (pMesh->nNumBlockV + 1); nCntV++)
		{
			fLengthXZ = cosf(ANGLE_V * nCntV) * pMesh->fRadius;

			for(int nCntH = 0; nCntH < (pMesh->nNumBlockH + 1); nCntH++, pVtx++)
			{
				// ���_���W�̐ݒ�
				pVtx->vtx.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
				if(pMesh->bReverse)
				{
					pVtx->vtx.y = -sinf(ANGLE_V * nCntV) * pMesh->fRadius * SKY_HEIGHT_RATE;
				}
				else
				{
					pVtx->vtx.y = sinf(ANGLE_V * nCntV) * pMesh->fRadius * SKY_HEIGHT_RATE;
				}
				pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

				// �@���̐ݒ�
				pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���_�J���[�̐ݒ�
				if(pMesh->bReverse)
				{
					pVtx->diffuse = D3DXCOLOR(0.0625f, 0.0625f, 0.375f, 0.5f);
				}
				else
				{
					pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				// �e�N�X�`�����W�̐ݒ�
				pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

				if(nCntV == pMesh->nNumBlockV)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * HEIGHT;
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		pMesh->pVtxBuff->Unlock();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pVtxBuffTop->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx->vtx.x = 0.0f;
		if(pMesh->bReverse)
		{
			pVtx->vtx.y = -sinf(ANGLE_V * (pMesh->nNumBlockV + 1)) * pMesh->fRadius * SKY_HEIGHT_RATE;
		}
		else
		{
			pVtx->vtx.y = sinf(ANGLE_V * (pMesh->nNumBlockV + 1)) * pMesh->fRadius * SKY_HEIGHT_RATE;
		}
		pVtx->vtx.z = 0.0f;

		// �@���̐ݒ�
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		if(pMesh->bReverse)
		{
			pVtx->diffuse = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
		}
		else
		{
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;

		pVtx++;

		fLengthXZ = cosf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius;
		for(int nCntH = 0; nCntH < pMesh->nNumBlockH; nCntH++, pVtx++)
		{
			// ���_���W�̐ݒ�
			pVtx->vtx.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
			if(pMesh->bReverse)
			{
				pVtx->vtx.y = -sinf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius * SKY_HEIGHT_RATE;
			}
			else
			{
				pVtx->vtx.y = sinf(ANGLE_V * pMesh->nNumBlockV) * pMesh->fRadius * SKY_HEIGHT_RATE;
			}
			pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// �@���̐ݒ�
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			if(pMesh->bReverse)
			{
				pVtx->diffuse = D3DXCOLOR(0.0675f, 0.0675f, 0.375f, 0.5f);
			}
			else
			{
				pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = 0.01f;
			pVtx->tex.y = 0.01f;
		}

		// ���_�f�[�^���A�����b�N����
		pMesh->pVtxBuffTop->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for(int nCntV = 0; nCntV < pMesh->nNumBlockV; nCntV++)
		{
			if(nCntV != 0)
			{
				*pIdx = nCntV * (pMesh->nNumBlockH + 1);
				pIdx++;
			}

			for(int nCntH = 0; nCntH < pMesh->nNumBlockH + 1; nCntH++)
			{
				*pIdx = nCntV * (pMesh->nNumBlockH + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (pMesh->nNumBlockH + 1) + nCntH;
				pIdx++;
			}

			if(nCntV != (pMesh->nNumBlockV - 1))
			{
				*pIdx = (nCntV + 1) * (pMesh->nNumBlockH + 1);
				pIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		pMesh->pIdxBuff->Unlock();

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pIdxBuffTop->Lock(0, 0, (void**)&pIdx, 0);

		for(int nCntH = 0; nCntH < pMesh->nNumBlockH + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// �C���f�b�N�X�f�[�^���A�����b�N����
		pMesh->pIdxBuffTop->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshSky(void)
{
	MESH_SKY *pMesh;

	for(int nCntSky = 0; nCntSky < MAX_MESH_SKY; nCntSky++)
	{
		pMesh = &g_aMeshSky[nCntSky];

		if(pMesh->pVtxBuff)
		{// ���_�o�b�t�@�̊J��
			pMesh->pVtxBuff->Release();
			pMesh->pVtxBuff = NULL;
		}

		if(pMesh->pIdxBuff)
		{// �C���f�b�N�X�o�b�t�@�̊J��
			pMesh->pIdxBuff->Release();
			pMesh->pIdxBuff = NULL;
		}

		if(pMesh->pVtxBuffTop)
		{// ���_�o�b�t�@�̊J��
			pMesh->pVtxBuffTop->Release();
			pMesh->pVtxBuffTop = NULL;
		}

		if(pMesh->pIdxBuffTop)
		{// �C���f�b�N�X�o�b�t�@�̊J��
			pMesh->pIdxBuffTop->Release();
			pMesh->pIdxBuffTop = NULL;
		}
	}

	if(g_pTextureSky)
	{// �e�N�X�`���̊J��
		g_pTextureSky->Release();
		g_pTextureSky = NULL;
	}

	g_nNumMeshSky = 0;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshSky(void)
{
	//MESH_SKY *pMesh;
	//for(int nCntSky = 0; nCntSky < MAX_MESH_SKY; nCntSky++)
	//{
	//	pMesh = &g_aMeshSky[nCntSky];

	//	pMesh->rot.y += D3DX_PI * pMesh->fRotY;
	//	if(pMesh->rot.y > D3DX_PI)
	//	{
	//		pMesh->rot.y -= D3DX_PI * 2.0f;
	//	}
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	DWORD dwSettingLighting, dwSettingCullmode;
	MESH_SKY *pMesh;

	pDevice->GetRenderState(D3DRS_LIGHTING, &dwSettingLighting);
	pDevice->GetRenderState(D3DRS_CULLMODE, &dwSettingCullmode);

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pMesh = &g_aMeshSky[0];
	for(int nCntSky = 0; nCntSky < MAX_MESH_SKY; nCntSky++, pMesh++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pMesh->mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMesh->rot.y,
													pMesh->rot.x,
													pMesh->rot.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, pMesh->pos.x,
												pMesh->pos.y,
												pMesh->pos.z);
		D3DXMatrixMultiply(&pMesh->mtxWorld, &pMesh->mtxWorld, &mtxTranslate);

		if(pMesh->bReverse)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// �\�ʂ��J�����O
		}
		else
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pMesh->mtxWorld);

		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetStreamSource(0, pMesh->pVtxBuff, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(pMesh->pIdxBuff);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		if(pMesh->nTexType == -1)
		{
			pDevice->SetTexture(0, g_pTextureSky);
		}
		else
		{
			pDevice->SetTexture(0, 0);
		}

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, pMesh->nNumVertex, 0, pMesh->nNumPolygon);


		// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetStreamSource(0, pMesh->pVtxBuffTop, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(g_aMeshSky[nCntSky].pIdxBuffTop);

		// �|���S���̕`��
		//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (pMesh->nNumBlockH + 2), 0, pMesh->nNumBlockH);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O

		pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, dwSettingLighting);
	pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
}

