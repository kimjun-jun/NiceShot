/**
* @file bulletgauge.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/object/objectclass.h"
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


//=============================================================================
// ����������
//=============================================================================
void BULLETGAUGE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this[0].SetPos(D3DXVECTOR3(BULLETGAUGE_P1_POS_X, BULLETGAUGE_P1_POS_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(BULLETGAUGE_P2_POS_X, BULLETGAUGE_P2_POS_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(BULLETGAUGE_P3_POS_X, BULLETGAUGE_P3_POS_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(BULLETGAUGE_P4_POS_X, BULLETGAUGE_P4_POS_Y, 0.0f));

	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{

		// �e��̏�����
		this[CntBulletGauge].AmmoPower = PLAYER_AMMOPOWER_NORMAL;

		// ���_���̍쐬
		MakeVertexBulletGauge(pDevice, CntBulletGauge);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETGAUGE,			// �t�@�C���̖��O
			&this[CntBulletGauge].tex2DVB.pD3DTexture);	// �ǂݍ��ރ������[

	}



}

//=============================================================================
// �ď���������
//=============================================================================
void BULLETGAUGE::Reinit(void)
{
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		// �e��̏�����
		this[CntBulletGauge].AmmoPower = PLAYER_AMMOPOWER_NORMAL;
	}
}

//=============================================================================
// �I������
//=============================================================================
void BULLETGAUGE::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void BULLETGAUGE::Update(PLAYER_HONTAI *player)
{
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		this[CntBulletGauge].AmmoPower = player[CntBulletGauge].AmmoCnt;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void BULLETGAUGE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, this[CntBulletGauge].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, this[CntBulletGauge].tex2DVB.pD3DTexture);

		// �|���S���̕`��
		for (int nCntPlace = 0; nCntPlace < this[CntBulletGauge].AmmoPower; nCntPlace++)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT BULLETGAUGE::MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntBulletGauge)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (POLYGON_2D_VERTEX * PLAYER_AMMOPOWER_STRONG + 4),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&this[CntBulletGauge].tex2DVB.pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[CntBulletGauge].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < PLAYER_AMMOPOWER_STRONG; nCntPlace++, pVtx += 4)
		{
			//---------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this[CntBulletGauge].GetPos();

			// ���_���W�̐ݒ�
				pVtx[1].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, -BULLETGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[0].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X *2, -BULLETGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[3].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X, BULLETGAUGE_SIZE_Y/2, 0.0f) + pos;
				pVtx[2].vtx = D3DXVECTOR3(-nCntPlace * BULLETGAUGE_SIZE_X - BULLETGAUGE_SIZE_X *2, BULLETGAUGE_SIZE_Y/2, 0.0f) + pos;

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
		this[CntBulletGauge].tex2DVB.pD3DVtxBuff->Unlock();
	}

	return S_OK;
}

