/**
* @file item.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/map/field.h"
#include "../../h/other/sound.h"
#include "../../h/object/item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.01f)			// ��]���x
#define	VALUE_FALLSPEED_ITEM	(2.0f)						// �������x
#define	ITEM_RADIUS				(20.0f)						// ���a
#define DROP_ITEM_MAX						(20)																		//!< �t�B�[���h�ɗ����Ă�A�C�e���̐�
#define DROP_ITEM_CHARGE_ADDTIME			(1.0f)																		//!< �A�C�e�������X�|�[�������鎞�̉��Z�^�C��
#define DROP_ITEM_CHARGE_CNT				(60.0f)																		//!< �A�C�e�������X�|�[�������鎞�̏��v�^�C��

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

const char *c_aFileNameItemModel[ITEMTYPE_MAX] =
{
	"../data/MODEL/tikeiItem.x",		// �n�`�ό`�A�C�e��
	"../data/MODEL/lifeItem.x",		// ���C�t��
	"../data/MODEL/sensyaItem.x",		// ��ԕό`�A�C�e��
	"../data/MODEL/bulletItem.x",		// �o���b�g�A�C�e��
	"../data/MODEL/speedItem.x",		// �X�s�[�h�A�b�v�A�C�e��
	"../data/MODEL/cameraItem.x",		// �����o�b�N�J�����A�C�e��
	"../data/MODEL/kiriItem.x",		// ���A�C�e��
};

const char *c_aFileNameItemTex[ITEMTYPE_MAX] =
{
	"../data/MODEL/landmark_aogashima.png",		// �n�`�ό`�A�C�e��
	"../data/MODEL/life000.png",					// ���C�t��
	"../data/MODEL/war_sensya_noman.png",			// ��ԕό`�A�C�e��
	"../data/MODEL/bullettex.png",					// �o���b�g�A�C�e��
	"../data/MODEL/1213810.png",					// �X�s�[�h�A�b�v�A�C�e��
	"../data/MODEL/mark_camera_satsuei_ok.png",	// �����o�b�N�J�����A�C�e��
	"../data/MODEL/yama_kiri.png",					// ���A�C�e��
};

//=============================================================================
// ����������
//=============================================================================
void ITEM::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		g_pD3DTextureItem[nCntItemType] = NULL;
		g_pMeshItem[nCntItemType] = NULL;
		g_pD3DXMatBuffItem[nCntItemType] = NULL;

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(c_aFileNameItemModel[nCntItemType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pD3DXMatBuffItem[nCntItemType],
			NULL,
			&g_aNumMatItem[nCntItemType],
			&g_pMeshItem[nCntItemType]);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
			c_aFileNameItemTex[nCntItemType],			// �t�@�C���̖��O
			&g_pD3DTextureItem[nCntItemType]);			// �ǂݍ��ރ������[

	}

	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		this[nCntItem].nIdxShadow = -1;
		this[nCntItem].nType = -1;
	}
	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X/4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
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
		SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
		//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);

	}
	this[0].GoukeiDrop = DROP_ITEM_MAX;

}

//=============================================================================
// �ď���������
//=============================================================================
void ITEM::Reinit(void)
{

	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		this[nCntItem].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		this[nCntItem].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetQrot(0.0f);
		this[nCntItem].SetUse(false);
		this[nCntItem].Droptime = 0.0f;
		this[nCntItem].nIdxShadow = -1;
		this[nCntItem].nType = -1;
		this[nCntItem].GettingSignal = false;
		this[nCntItem].GettingSignalEnd = false;
		this[nCntItem].CollisionFieldEnd = false;
	}
	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
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
		SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
		//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);

	}
	this[0].GoukeiDrop = DROP_ITEM_MAX;
}

//=============================================================================
// �I������
//=============================================================================
void ITEM::Uninit(void)
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
void ITEM::Update(void)
{
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			//-------------------------------------------�I�u�W�F�N�g�̒l�ǂݍ���
			D3DXVECTOR3 pos = this[nCntItem].GetPos();
			D3DXVECTOR3 rot = this[nCntItem].GetRot();
			D3DXVECTOR3 FieldNorVec = this[nCntItem].GetFieldNorVec();
			D3DXVECTOR3 FieldNorUpNorCross = this[nCntItem].GetFieldNorUpNorCross();
			float Qrot = this[nCntItem].GetQrot();

			//�t�B�[���h�ɗ����Ă�Ƃ��͂��邭���]������
			rot.y += VALUE_ROTATE_ITEM;

			//���X�ɗ����Ă���
			if (this[nCntItem].CollisionFieldEnd != true)
			{
				pos.y -= VALUE_FALLSPEED_ITEM;
			}

			//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
			D3DXVECTOR3 Upvec = D3DXVECTOR3(0.0, 1.0f, 0.0f);
			D3DXVec3Cross(&FieldNorVec, &FieldNorUpNorCross, &Upvec);
			float kakezan = D3DXVec3Dot(&FieldNorUpNorCross, &Upvec);
			if (kakezan != 0)
			{
				float cossita = kakezan /
					sqrtf(FieldNorUpNorCross.x*FieldNorUpNorCross.x +
						FieldNorUpNorCross.y *FieldNorUpNorCross.y +
						FieldNorUpNorCross.z * FieldNorUpNorCross.z);
				Qrot = acosf(cossita);
			}
			else Qrot = 0.0f;

			//-------------------------------------------�I�u�W�F�N�g�̒l��������
			this[nCntItem].SetPos(pos);
			this[nCntItem].SetRot(rot);
			this[nCntItem].SetFieldNorVec(FieldNorVec);
//			this[nCntItem].SetFieldNorUpNorCross(FieldNorUpNorCross);
			this[nCntItem].SetQrot(Qrot);
		}


		if (this[nCntItem].GettingSignal == true)
		{
			GettingItem(nCntItem);
		}


	}


	//�A�C�e���𕜊������鐧��B
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		if (this[0].GoukeiDrop > DROP_ITEM_MAX) break;
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			this[nCntItem].Droptime += DROP_ITEM_CHARGE_ADDTIME;
			if (this[nCntItem].Droptime >= DROP_ITEM_CHARGE_CNT)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
				int x = rand() % 2;
				int z = rand() % 2;
				if (x == 1) pos.x *= -1;
				if (z == 1) pos.z *= -1;
				int ItemNum = rand() % ITEMTYPE_MAX;
				//���C�t�A�J�����A���A�C�e���̎��͂�����x���I
				if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
				SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
				//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
				this[nCntItem].CollisionFieldEnd = false;
				this[nCntItem].Droptime = 0.0f;
				this[0].GoukeiDrop++;
				PlaySound(SOUND_LABEL_SE_nyu);
			}
			break;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void ITEM::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			D3DXQUATERNION q(0, 0, 0, 1);
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);

			//-------------------------------------------�I�u�W�F�N�g�̒l�ǂݍ���
			D3DXMATRIX mtxWorldItem = this[nCntItem].GetMatrix();
			D3DXVECTOR3 pos = this[nCntItem].GetPos();
			D3DXVECTOR3 rot = this[nCntItem].GetRot();
			D3DXVECTOR3 scl = this[nCntItem].GetScl();
			D3DXVECTOR3 FieldNorVec = this[nCntItem].GetFieldNorVec();
			float Qrot = this[nCntItem].GetQrot();


			//q=(rotVecAxis�@��)*(g_Player.rot��])  -���L���H
			D3DXQuaternionRotationAxis(&q, &FieldNorVec, -Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &q);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldItem);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxRot);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxQ);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldItem);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffItem[this[nCntItem].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatItem[this[nCntItem].nType]; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pD3DTextureItem[this[nCntItem].nType]);
				}

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureItem[this[nCntItem].nType]);

				// �`��
				g_pMeshItem[this[nCntItem].nType]->DrawSubset(nCntMat);
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
void ITEM::SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, int nType)
{
	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			this[nCntItem].SetPos(pos);
			this[nCntItem].SetScl(scl);
			this[nCntItem].SetRot(rot);
			this[nCntItem].SetUse(true);
			this[nCntItem].nType = nType;
			break;
		}
	}
}

//=============================================================================
// �A�C�e���̍폜
//=============================================================================
void ITEM::DeleteItem(int nIdxItem)
{
	if(nIdxItem >= 0 && nIdxItem < OBJECT_ITEM_MAX)
	{
		this[nIdxItem].SetUse(false);
		this[nIdxItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// �A�C�e�����擾�����v���C���[�֋߂Â���֐�
//=============================================================================
void ITEM::GettingItem(int nIdxItem)
{
	if (this[nIdxItem].GettingSignalEnd == false)
	{
		//-------------------------------------------�I�u�W�F�N�g�̒l�ǂݍ���
		D3DXVECTOR3 pos = this[nIdxItem].GetPos();
		D3DXVECTOR3 rot = this[nIdxItem].GetRot();
		D3DXVECTOR3 scl = this[nIdxItem].GetScl();

		//���邭���]������
		rot.y += VALUE_ROTATE_ITEM*10;
		
		//�v���C���[�ƃA�C�e���̋������v�Z��/5���Â߂Â���
		//PLAYER_HONTAI *p = this[nIdxItem].GetPlayer();
		PLAYER_HONTAI *p = p->GetPlayerPointer();
		D3DXVECTOR3 PlayerPos = this[nIdxItem].p[this[nIdxItem].GetPlayerType].GetPos();

		D3DXVECTOR3 distance = PlayerPos - pos;
		distance /= 4.0f;
		pos += distance;
		scl -= D3DXVECTOR3(ITEM_SMALL_SCL, ITEM_SMALL_SCL, ITEM_SMALL_SCL);
		if (scl.x <= ITEM_DELETE_SCL)
		{
			this[nIdxItem].GettingSignalEnd = true;
		}

		//-------------------------------------------�I�u�W�F�N�g�̒l��������
		this[nIdxItem].SetPos(pos);
		this[nIdxItem].SetRot(rot);
	}

	else
	{
		DeleteItem(nIdxItem);
		this[nIdxItem].GettingSignal = false;
		this[nIdxItem].GettingSignalEnd = false;
		this[0].GoukeiDrop--;
	}
}
