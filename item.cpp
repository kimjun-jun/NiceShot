//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : �ؑ���
//
//=============================================================================
#include "main.h"
#include "item.h"
#include "field.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.01f)			// ��]���x
#define	ITEM_RADIUS				(20.0f)						// ���a

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�

//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureItem[ITEMTYPE_MAX];	// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			g_pMeshItem[ITEMTYPE_MAX];			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXMatBuffItem[ITEMTYPE_MAX];	// ���b�V���̃}�e���A�������i�[
DWORD				g_aNumMatItem[ITEMTYPE_MAX];			// �������̑���

D3DXMATRIX			g_mtxWorldItem;				// ���[���h�}�g���b�N�X

ITEM				g_aItem[MAX_ITEM];			// �A�C�e�����[�N

const char *c_aFileNameItemModel[ITEMTYPE_MAX] =
{
	"data/MODEL/tikeiItem.x",		// �n�`�ό`�A�C�e��
	"data/MODEL/lifeItem.x",		// ���C�t��
	"data/MODEL/sensyaItem.x",		// ��ԕό`�A�C�e��
	"data/MODEL/bulletItem.x",		// �o���b�g�A�C�e��
	"data/MODEL/speedItem.x",		// �X�s�[�h�A�b�v�A�C�e��
	"data/MODEL/cameraItem.x",		// �����o�b�N�J�����A�C�e��
	"data/MODEL/kiriItem.x",		// ���A�C�e��
};

const char *c_aFileNameItemTex[ITEMTYPE_MAX] =
{
	"data/MODEL/landmark_aogashima.png",		// �n�`�ό`�A�C�e��
	"data/MODEL/life000.png",					// ���C�t��
	"data/MODEL/war_sensya_noman.png",			// ��ԕό`�A�C�e��
	"data/MODEL/bullettex.png",					// �o���b�g�A�C�e��
	"data/MODEL/1213810.png",					// �X�s�[�h�A�b�v�A�C�e��
	"data/MODEL/mark_camera_satsuei_ok.png",	// �����o�b�N�J�����A�C�e��
	"data/MODEL/yama_kiri.png",					// ���A�C�e��
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		g_pD3DTextureItem[nCntItemType] = NULL;
		g_pMeshItem[nCntItemType] = NULL;
		g_pD3DXMatBuffItem[nCntItemType] = NULL;

		// X�t�@�C���̓ǂݍ���
		if(FAILED(D3DXLoadMeshFromX(c_aFileNameItemModel[nCntItemType],
									D3DXMESH_SYSTEMMEM,
									pDevice,
									NULL,
									&g_pD3DXMatBuffItem[nCntItemType],
									NULL,
									&g_aNumMatItem[nCntItemType],
									&g_pMeshItem[nCntItemType])))
		{
			return E_FAIL;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			c_aFileNameItemTex[nCntItemType],			// �t�@�C���̖��O
			&g_pD3DTextureItem[nCntItemType]);			// �ǂݍ��ރ������[

	}

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].Upvec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_aItem[nCntItem].rotVecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].Qrot = 0.0f;
		g_aItem[nCntItem].fRadius = 0.0f;
		g_aItem[nCntItem].Droptime = 0.0f;
		g_aItem[nCntItem].nIdxShadow = -1;
		g_aItem[nCntItem].nType = -1;
		g_aItem[nCntItem].bUse = false;
		g_aItem[nCntItem].fCollisionEnd = false;
	}
	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % 500) + 100.0f, 1000.0f, float(rand() % 500) + 100.0f);
		int x = rand() % 2;
		int z = rand() % 2;
		if (x == 1) pos.x *= -1;
		if (z == 1) pos.z *= -1;

		/*
		enum
		{
			ITEMTYPE_TIKEI = 0,		// �n�`
			ITEMTYPE_LIFE,			// ���C�t
			ITEMTYPE_SENSYA,		// ���
			ITEMTYPE_BULLET,		// �o���b�g
			ITEMTYPE_SPEEDUP,		// �X�s�[�h�A�b�v
			ITEMTYPE_CAMERA,		// ���ז��A�C�e���@�����o�b�N�J����
			ITEMTYPE_KIRI,			// ���ז��A�C�e���@��
			ITEMTYPE_MAX
		};
		*/
		int ItemNum = rand() % ITEMTYPE_MAX;
		//���C�t�A�J�����A���A�C�e���̎��͂�����x���I
		if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
		SetItem(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
		//SetItem(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);

	}

	g_aItem[0].GoukeiDrop = DROP_ITEM_MAX;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	for(int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		if(g_pD3DTextureItem[nCntItemType] != NULL)
		{// �e�N�X�`���̊J��
			g_pD3DTextureItem[nCntItemType]->Release();
			g_pD3DTextureItem[nCntItemType] = NULL;
		}

		if(g_pMeshItem[nCntItemType] != NULL)
		{// ���b�V���̊J��
			g_pMeshItem[nCntItemType]->Release();
			g_pMeshItem[nCntItemType] = NULL;
		}

		if(g_pD3DXMatBuffItem[nCntItemType] != NULL)
		{// �}�e���A���̊J��
			g_pD3DXMatBuffItem[nCntItemType]->Release();
			g_pD3DXMatBuffItem[nCntItemType] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_aItem[nCntItem].rot.y += VALUE_ROTATE_ITEM;
			if (g_aItem[nCntItem].rot.y > D3DX_PI)
			{
				g_aItem[nCntItem].rot.y -= D3DX_PI * 2.0f;
			}

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_aItem[nCntItem].nIdxShadow, D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.1f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(g_aItem[nCntItem].fRadius * 2.0f, g_aItem[nCntItem].fRadius * 2.0f, g_aItem[nCntItem].fRadius * 2.0f));

			float fSizeX = 20.0f + (g_aItem[nCntItem].pos.y - 10.0f) * 0.05f;
			if (fSizeX < 20.0f)
			{
				fSizeX = 20.0f;
			}
			float fSizeY = 20.0f + (g_aItem[nCntItem].pos.y - 10.0f) * 0.05f;
			if (fSizeY < 20.0f)
			{
				fSizeY = 20.0f;
			}

			SetVertexShadow(g_aItem[nCntItem].nIdxShadow, fSizeX, fSizeY);

			float colA = (200.0f - (g_aItem[nCntItem].pos.y - 10.0f)) / 400.0f;
			if (colA < 0.0f)
			{
				colA = 0.0f;
			}
			SetColorShadow(g_aItem[nCntItem].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));

			//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
			D3DXVec3Cross(&g_aItem[nCntItem].rotTOaxis, &g_aItem[nCntItem].rotVecAxis, &g_aItem[nCntItem].Upvec);
			float kakezan = D3DXVec3Dot(&g_aItem[nCntItem].rotVecAxis, &g_aItem[nCntItem].Upvec);
			if (kakezan != 0)
			{
				float cossita = kakezan /
					sqrtf(g_aItem[nCntItem].rotVecAxis.x*g_aItem[nCntItem].rotVecAxis.x +
						g_aItem[nCntItem].rotVecAxis.y *g_aItem[nCntItem].rotVecAxis.y +
						g_aItem[nCntItem].rotVecAxis.z * g_aItem[nCntItem].rotVecAxis.z);
				g_aItem[nCntItem].Qrot = acosf(cossita);
			}
			else g_aItem[nCntItem].Qrot = 0.0f;
		}
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
		{
			if (g_aItem[0].GoukeiDrop > DROP_ITEM_MAX) break;
			if (g_aItem[nCntItem].bUse==false)
			{
				g_aItem[nCntItem].Droptime += DROP_ITEM_CHARGE_ADDTIME;
				if (g_aItem[nCntItem].Droptime >= DROP_ITEM_CHARGE_CNT)
				{
					D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % 500) + 100.0f, 1000.0f, float(rand() % 500) + 100.0f);
					int x = rand() % 2;
					int z = rand() % 2;
					if (x == 1) pos.x *= -1;
					if (z == 1) pos.z *= -1;
					int ItemNum = rand() % ITEMTYPE_MAX;
					//���C�t�A�J�����A���A�C�e���̎��͂�����x���I
					if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
					SetItem(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
					//SetItem(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
					g_aItem[nCntItem].fCollisionEnd = false;
					g_aItem[nCntItem].Droptime = 0.0f;
					g_aItem[0].GoukeiDrop++;
					PlaySound(SOUND_LABEL_SE_nyu);
				}
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse==true)
		{
			D3DXMATRIX mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			D3DXQUATERNION q(0, 0, 0, 1);
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);
			//q=(rotVecAxis�@��)*(g_Player.rot��])  -���L���H
			D3DXQuaternionRotationAxis(&q, &g_aItem[nCntItem].rotTOaxis, -g_aItem[nCntItem].Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &q);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldItem);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxQ);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffItem[g_aItem[nCntItem].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatItem[g_aItem[nCntItem].nType]; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pD3DTextureItem[g_aItem[nCntItem].nType]);
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureItem[g_aItem[nCntItem].nType]);

				// �`��
				g_pMeshItem[g_aItem[nCntItem].nType]->DrawSubset(nCntMat);
			}
			pDevice->SetMaterial(&matDef);
		}
	}
	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(!g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].rot = rot;
			g_aItem[nCntItem].fRadius = ITEM_RADIUS;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;

			// �e�̐ݒ�
			g_aItem[nCntItem].nIdxShadow = CreateShadow(g_aItem[nCntItem].pos, D3DXVECTOR3(g_aItem[nCntItem].fRadius * 2.0f, g_aItem[nCntItem].fRadius * 2.0f, g_aItem[nCntItem].fRadius * 2.0f));

			break;
		}
	}
}

//=============================================================================
// �A�C�e���̍폜
//=============================================================================
void DeleteItem(int nIdxItem)
{
	if(nIdxItem >= 0 && nIdxItem < MAX_ITEM)
	{
		ReleaseShadow(g_aItem[nIdxItem].nIdxShadow);
		g_aItem[nIdxItem].bUse = false;
		g_aItem[nIdxItem].fCollisionEnd = false;
	}
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}
