/**
* @file bulletprediction.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/other/input.h"
#include "../../../h/object/camera.h"
#include "../../../h/object/player.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/bullet/bulletprediction.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLETPREDICTION		"../data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.01f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.01f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.01f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.01f),//p4�J���[ 
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletprediction[OBJECT_PLAYER_MAX] = { NULL };		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletprediction[OBJECT_PLAYER_MAX] = { NULL };		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//=============================================================================
// ����������
//=============================================================================
void BULLETPREDICTION::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexBulletprediction(pDevice);
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETPREDICTION,			// �t�@�C���̖��O
			&g_pD3DTextureBulletprediction[CntPlayer]);	// �ǂݍ��ރ������[
	}
}

//=============================================================================
// �ď���������
//=============================================================================
void BULLETPREDICTION::Reinit(void)
{
	for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++)
	{
		this[nCntBulletprediction].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntBulletprediction].SetCol(D3DXCOLOR(DWORD(0)));
		this[nCntBulletprediction].SetUse(false);
	}
}

//=============================================================================
// �I������
//=============================================================================
void BULLETPREDICTION::Uninit(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
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
void BULLETPREDICTION::Update(PLAYER_HONTAI *player)
{
	for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++)
	{
		bool use = this[nCntBulletprediction].GetUse();
		if (use)
		{
			this[nCntBulletprediction].SetUse(false);
		}
	}

	//�v���C���[�̏�񂩂甭�ˈʒu�p�x�ړ��ʂ𗘗p���ăo���b�g�̒��e�_���Z�o����
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//---------------------------------�I�u�W�F�N�g�l�ǂݍ���
		D3DXVECTOR3	BulletPredictionPos = player[CntPlayer].GetPos();
		D3DXVECTOR3 HoudaiRot = player[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = player[CntPlayer].parts[PARTSTYPE_HOUTOU]->GetRot();
		D3DXVECTOR3 HousinRot = player[CntPlayer].parts[PARTSTYPE_HOUSIN]->GetRot();

		//���ˊp�x�A���ˍ��p�v�Z
		D3DXVECTOR3 BmoveRot;
		BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
		BmoveRot.y = sinf(player[CntPlayer].Brot - HousinRot.x);
		BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);
		D3DXVECTOR3 bulletmove;
		bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
		bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
		bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

		D3DXCOLOR	col = PLAYER_COLOR[CntPlayer];
		float Gravity = 0.0f;
		//float time = 1.0f;
		//float maxtime = 10.0f;
		for (int nCntBulletprediction = 0; nCntBulletprediction < BULLETPREDICTION_MAX; nCntBulletprediction++)
		{
			//�d�͂ƈړ��ʂ𔽉f
			//BulletPredictionPos.x += player[CntPlayer].bulletmove.x*(time / maxtime);
			//BulletPredictionPos.y -= player[CntPlayer].bulletmove.y*(time / maxtime) + Gravity;
			//BulletPredictionPos.z += player[CntPlayer].bulletmove.z*(time / maxtime);

			BulletPredictionPos.x += bulletmove.x;
			BulletPredictionPos.y -= bulletmove.y + Gravity;
			BulletPredictionPos.z += bulletmove.z;

			//�d�͂�����������
			Gravity += VALUE_GRAVITYADD_BULLET;
			//���Ԋ�����i�߂�B�򋗗���������قǒe���\���Ԋu���L���Ȃ�
			//time++;
			//�d�͍ő�l����
			if (Gravity > VALUE_GRAVITYMAX_BULLET) Gravity = VALUE_GRAVITYMAX_BULLET;
			//���X�ɃA���t�@�l���������ĉ������n�_�����₷������
			col.a += 0.01f;
			SetBulletprediction(BulletPredictionPos, CntPlayer, col, BULLETPREDICTION_SIZE_X, BULLETPREDICTION_SIZE_Y);


		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void BULLETPREDICTION::Draw(PLAYER_HONTAI *player)
{
	//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool puse = player[CntPlayer].GetUse();
		if (puse)
		{
			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			for (int nCntBulletprediction = 0; nCntBulletprediction < BULLETPREDICTION_MAX; nCntBulletprediction++)
			{
				D3DXMATRIX mtxView, mtxScale, mtxTranslate;

				bool bpuse = this[nCntBulletprediction].GetUse();
				if (bpuse)
				{
					//---------------------------------�I�u�W�F�N�g�l�ǂݍ���
					D3DXVECTOR3	pos = this[nCntBulletprediction].GetPos();
					D3DXVECTOR3	scl = this[nCntBulletprediction].GetScl();
					D3DXMATRIX mtxWorld = this[nCntBulletprediction].GetMatrix();

					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtxWorld);

					// �r���[�}�g���b�N�X���擾
					CAMERA *cam = GetCamera();

					mtxWorld._11 = cam[CntPlayer].mtxView._11;
					mtxWorld._12 = cam[CntPlayer].mtxView._21;
					mtxWorld._13 = cam[CntPlayer].mtxView._31;
					mtxWorld._21 = cam[CntPlayer].mtxView._12;
					mtxWorld._22 = cam[CntPlayer].mtxView._22;
					mtxWorld._23 = cam[CntPlayer].mtxView._32;
					mtxWorld._31 = cam[CntPlayer].mtxView._13;
					mtxWorld._32 = cam[CntPlayer].mtxView._23;
					mtxWorld._33 = cam[CntPlayer].mtxView._33;

					// �X�P�[���𔽉f
					D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

					// �ړ��𔽉f
					D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

					// ���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

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
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT BULLETPREDICTION::MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * OBJECT_BULLETPREDICTION_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
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

			for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++, pVtx += 4)
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
void BULLETPREDICTION::SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY)
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
void BULLETPREDICTION::SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col)
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
int BULLETPREDICTION::SetBulletprediction(D3DXVECTOR3 pos, int PlayerType,D3DXCOLOR col, float fSizeX, float fSizeY)
{
	int nIdxBulletprediction = -1;

	for (int nCntBulletprediction = 0; nCntBulletprediction < OBJECT_BULLETPREDICTION_MAX; nCntBulletprediction++)
	{
		bool use = this[nCntBulletprediction].GetUse();
		if (use != true)
		{
			this[nCntBulletprediction].SetPos(pos);
			this[nCntBulletprediction].SetCol(col);
			this[nCntBulletprediction].SetUse(true);

			// ���_���W�̐ݒ�
			SetVertexBulletprediction(PlayerType,nCntBulletprediction, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorBulletprediction(PlayerType,nCntBulletprediction,col);
			nIdxBulletprediction = nCntBulletprediction;
			break;
		}
	}
	return nIdxBulletprediction;
}
