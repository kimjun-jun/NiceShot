/**
* @file vitalgauge.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../../h/scene/rank.h"
#include "../../h/object/vitalgauge.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_VITALGAUGE		"../data/TEXTURE/UI_VITALGAUGE.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
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

//=============================================================================
// ����������
//=============================================================================
void VITALGAUGE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this[0].SetPos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));

	// ���_���̍쐬
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		MakeVertexVitalGauge(pDevice,CntPlayer);

		LPDIRECT3DTEXTURE9 pD3DTexture;
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_VITALGAUGE,			// �t�@�C���̖��O
			&pD3DTexture);	// �ǂݍ��ރ������[
		this[CntPlayer].tex2DVB.SetpD3DTexture(pD3DTexture);
	}
}

//=============================================================================
// �ď���������
//=============================================================================
void VITALGAUGE::Reinit(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		// ���C�t�̏�����
		this[CntPlayer].VitalPower = PLAYER_VITAL;

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			pD3DVtxBuff[CntPlayer].Lock(0, 0, (void**)&pVtx, 0);
			for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
			{
				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			}
			// ���_�f�[�^���A�����b�N����
			pD3DVtxBuff[CntPlayer].Unlock();

			this[CntPlayer].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void VITALGAUGE::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void VITALGAUGE::Update(void)
{
	//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
	GAME_OBJECT *playerobj = this->GetPointerPlayer();
	PLAYER_HONTAI *p = dynamic_cast<PLAYER_HONTAI*>(&playerobj[0]);
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		bool puse = p[CntPlayer].GetUse();
		if (puse == true)
		{
			if (this[CntPlayer].VitalPower <= 0)
			{
				p[CntPlayer].SetUse(false);
				//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
				GAME_OBJECT *rankobj = this->GetPointerRank();
				RANK *rank = dynamic_cast<RANK*>(&rankobj[0]);
				rank->SetRank(CntPlayer);
			}
		}
		this[CntPlayer].VitalPower = p[CntPlayer].vital;
		//�̗͈��ȏ�ŗΐF
		if (this[CntPlayer].VitalPower >= PLAYER_VITAL * (0.2f))
		{
			{//���_�o�b�t�@�̒��g�𖄂߂�
				VERTEX_2D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
				pD3DVtxBuff[CntPlayer].Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// ���ˌ��̐ݒ�
					pVtx[0].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
				}
				// ���_�f�[�^���A�����b�N����
				pD3DVtxBuff[CntPlayer].Unlock();
			}
		}
		//�̗͈��ȉ��ŐԐF
		else
		{
			{//���_�o�b�t�@�̒��g�𖄂߂�
				VERTEX_2D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
				pD3DVtxBuff[CntPlayer].Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
				{

					// ���ˌ��̐ݒ�
					pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
				}
				// ���_�f�[�^���A�����b�N����
				pD3DVtxBuff[CntPlayer].Unlock();
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void VITALGAUGE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntPlayer].tex2DVB.GetpD3DVtxBuff();
		pDevice->SetStreamSource(0, pD3DVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		LPDIRECT3DTEXTURE9 pD3DTexture = this[CntPlayer].tex2DVB.GetpD3DTexture;
		pDevice->SetTexture(0, pD3DTexture);

		// �|���S���̕`��
		for (int nCntPlace = 0; nCntPlace < this[CntPlayer].VitalPower; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT VITALGAUGE::MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	LPDIRECT3DVERTEXBUFFER9 VtxBuffBulletGauge = NULL;
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_VITAL + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&VtxBuffBulletGauge,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxBuffBulletGauge->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < PLAYER_VITAL; nCntPlace++, pVtx += 4)
		{
			//--------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this[CntPlayer].GetPos();
			// ���_���W�̐ݒ�
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, -VITALGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, -VITALGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X, VITALGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * VITALGAUGE_SIZE_X - VITALGAUGE_SIZE_X *2, VITALGAUGE_SIZE_Y/2, 0.0f) + pos;

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
		VtxBuffBulletGauge->Unlock();

		//---------------------------�I�u�W�F�N�g�l��������
		this[CntPlayer].tex2DVB.SetpD3DVtxBuff(VtxBuffBulletGauge);
	}

	return S_OK;
}

