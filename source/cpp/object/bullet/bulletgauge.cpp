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
#define	TEXTURE_BULLETGAUGE			"../data/TEXTURE/UI/UI_Shell.png"				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_BULLETGAUGE_EMPTY	"../data/TEXTURE/UI/UI_Shell_Empty.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLETGAUGE_SIZE_X			(15.0f)										// �����̕�
#define	BULLETGAUGE_SIZE_Y			(20.0f)										// �����̍���
#define	BULLETGAUGE_SIZE_X_OFFSET	(0.8f)										// �A�^�b�N�Q�[�W�̐����̕�
#define	BULLETGAUGE_SIZE_Y_OFFSET	(0.8f)										// �A�^�b�N�Q�[�W�̐����̍���
#define	BULLETGAUGE_POS_X			(570.0f)									// �\����ʒu�w���W
#define	BULLETGAUGE_POS_Y			(SCREEN_CENTER_Y/5)							// �\����ʒu�x���W
#define	BULLETGAUGE_POS_X_OFFSET	(20.0f)										// �\����ʒu�w���W

#define BULLETGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_NET_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X+55.0f
#define BULLETGAUGE_NET_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y


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

	for (int CntPlayer = 0; CntPlayer < OBJECT_BULLETGAUGE_MAX; CntPlayer++)
	{

		// �e��̏�����
		this[CntPlayer].AmmoPower = PLAYER_AMMOPOWER_NORMAL;

		// ���_���̍쐬
		MakeVertexBulletGauge(pDevice, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETGAUGE_EMPTY,			// �t�@�C���̖��O
			&this[CntPlayer].TexEmpty[0].pD3DTexture);	// �ǂݍ��ރ������[

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_BULLETGAUGE,			// �t�@�C���̖��O
			&this[CntPlayer].Tex[0].pD3DTexture);	// �ǂݍ��ރ������[
	}



}

//=============================================================================
// �ď���������
//=============================================================================
void BULLETGAUGE::Reinit(void)
{
	this[0].SetPos(D3DXVECTOR3(BULLETGAUGE_P1_POS_X, BULLETGAUGE_P1_POS_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(BULLETGAUGE_P2_POS_X, BULLETGAUGE_P2_POS_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(BULLETGAUGE_P3_POS_X, BULLETGAUGE_P3_POS_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(BULLETGAUGE_P4_POS_X, BULLETGAUGE_P4_POS_Y, 0.0f));

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < OBJECT_BULLETGAUGE_MAX; CntPlayer++)
	{
		// �e��̏�����
		this[CntPlayer].AmmoPower = PLAYER_AMMOPOWER_NORMAL;

		// ���_���̍쐬
		MakeVertexBulletGauge(pDevice, CntPlayer);
	}
}

//=============================================================================
// �ď����������@�l�b�g�ΐ�O
//=============================================================================
void BULLETGAUGE::ReinitNet(int MyNumber)
{

	this[MyNumber].SetPos(D3DXVECTOR3(BULLETGAUGE_NET_POS_X, BULLETGAUGE_NET_POS_Y, 0.0f));

	//���W�擾
	D3DXVECTOR3 pos = this[MyNumber].GetPos();

	for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOPOWER_NORMAL; CntAmmo++)
	{
		//BASE�ݒ�
		// ���_���W�̐ݒ�
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[0].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[1].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X*2, pos.y - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[2].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[3].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);

		//���g�ݒ�
		// ���_���W�̐ݒ�
		this[MyNumber].Tex[CntAmmo].textureVTX[0].vtx = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y_OFFSET - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].Tex[CntAmmo].textureVTX[1].vtx = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y_OFFSET - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].Tex[CntAmmo].textureVTX[2].vtx = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].Tex[CntAmmo].textureVTX[3].vtx = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
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
void BULLETGAUGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (Netflag == false)
	{
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOPOWER_NORMAL; CntAmmo++)
		{
			//BASE��`��
			pDevice->SetTexture(0, this[CntPlayer].TexEmpty[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].TexEmpty[CntAmmo].textureVTX, sizeof(VERTEX_2D));//�����͍��W������0
		}
		for (int CntAmmo = 0; CntAmmo < this[CntPlayer].AmmoPower; CntAmmo++)
		{
			//���g��`��
			pDevice->SetTexture(0, this[CntPlayer].Tex[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].Tex[CntAmmo].textureVTX, sizeof(VERTEX_2D));//�����͍��W������0
		}
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOPOWER_NORMAL; CntAmmo++)
		{
			//BASE��`��
			pDevice->SetTexture(0, this[NetMyNumber].TexEmpty[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[NetMyNumber].TexEmpty[CntAmmo].textureVTX, sizeof(VERTEX_2D));//�����͍��W������0
		}
		for (int CntAmmo = 0; CntAmmo < this[NetMyNumber].AmmoPower; CntAmmo++)
		{
		   //���g��`��
			pDevice->SetTexture(0, this[NetMyNumber].Tex[0].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[NetMyNumber].Tex[CntAmmo].textureVTX, sizeof(VERTEX_2D));//�����͍��W������0
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT BULLETGAUGE::MakeVertexBulletGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	//���W�擾
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();

	for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOPOWER_NORMAL; CntAmmo++)
	{
		//BASE�ݒ�
		// ���_���W�̐ݒ�
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[0].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET), pos.y, 0.0f);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[1].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y, 0.0f);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[2].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[3].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[0].rhw =
			this[CntPlayer].TexEmpty[CntAmmo].textureVTX[1].rhw =
			this[CntPlayer].TexEmpty[CntAmmo].textureVTX[2].rhw =
			this[CntPlayer].TexEmpty[CntAmmo].textureVTX[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[CntPlayer].TexEmpty[CntAmmo].textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		//���g�ݒ�
		// ���_���W�̐ݒ�
		this[CntPlayer].Tex[CntAmmo].textureVTX[0].vtx = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
		this[CntPlayer].Tex[CntAmmo].textureVTX[1].vtx = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
		this[CntPlayer].Tex[CntAmmo].textureVTX[2].vtx = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
		this[CntPlayer].Tex[CntAmmo].textureVTX[3].vtx = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		this[CntPlayer].Tex[CntAmmo].textureVTX[0].rhw =
			this[CntPlayer].Tex[CntAmmo].textureVTX[1].rhw =
			this[CntPlayer].Tex[CntAmmo].textureVTX[2].rhw =
			this[CntPlayer].Tex[CntAmmo].textureVTX[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		this[CntPlayer].Tex[CntAmmo].textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntPlayer].Tex[CntAmmo].textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntPlayer].Tex[CntAmmo].textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		this[CntPlayer].Tex[CntAmmo].textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		this[CntPlayer].Tex[CntAmmo].textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		this[CntPlayer].Tex[CntAmmo].textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		this[CntPlayer].Tex[CntAmmo].textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		this[CntPlayer].Tex[CntAmmo].textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

