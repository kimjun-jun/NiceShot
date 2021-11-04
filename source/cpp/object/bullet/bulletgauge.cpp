/**
* @file bulletgauge.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../../h/object/bullet/bulletgauge.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLETGAUGE			"../data/TEXTURE/UI/UI_Shell.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_BULLETGAUGE_EMPTY	"../data/TEXTURE/UI/UI_Shell_Empty.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

constexpr float	BULLETGAUGE_SIZE_X{ 15.0f };					// �����̕�
constexpr float	BULLETGAUGE_SIZE_Y{ 20.0f };					// �����̍���
constexpr float	BULLETGAUGE_SIZE_X_OFFSET{ 0.8f };				// �A�^�b�N�Q�[�W�̐����̕�
constexpr float	BULLETGAUGE_SIZE_Y_OFFSET{ 0.8f };				// �A�^�b�N�Q�[�W�̐����̍���
constexpr float	BULLETGAUGE_POS_X{ 570.0f };					// �\����ʒu�w���W
constexpr float	BULLETGAUGE_POS_Y{ SCREEN_CENTER_Y / 5.0f };	// �\����ʒu�x���W
constexpr float	BULLETGAUGE_POS_X_OFFSET{ 20.0f };				// �\����ʒu�w���W
constexpr float BULLETGAUGE_P1_POS_X{ SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X };
constexpr float BULLETGAUGE_P1_POS_Y{ SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y };
constexpr float BULLETGAUGE_P2_POS_X{ SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X };
constexpr float BULLETGAUGE_P2_POS_Y{ SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y };
constexpr float BULLETGAUGE_P3_POS_X{ SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X };
constexpr float BULLETGAUGE_P3_POS_Y{ SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y };
constexpr float BULLETGAUGE_P4_POS_X{ SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X };
constexpr float BULLETGAUGE_P4_POS_Y{ SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y };
constexpr float BULLETGAUGE_NET_POS_X{ SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X + 55.0f };
constexpr float BULLETGAUGE_NET_POS_Y{ SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y };

constexpr int PLAYER_AMMOSTOCK{ 10 };	//!< �e�̃X�g�b�N�@

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
BULLETGAUGE::BULLETGAUGE(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx[PLAYER01].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);	//!< *2�͘g�e�N�X�`���Ɩ{�̃e�N�X�`����2��ޕK�v������
	this->vtx[PLAYER02].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);
	this->vtx[PLAYER03].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);
	this->vtx[PLAYER04].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(BULLETGAUGE_P1_POS_X, BULLETGAUGE_P1_POS_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(BULLETGAUGE_P2_POS_X, BULLETGAUGE_P2_POS_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(BULLETGAUGE_P3_POS_X, BULLETGAUGE_P3_POS_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(BULLETGAUGE_P4_POS_X, BULLETGAUGE_P4_POS_Y, 0.0f));

	//�J�E���g���[�v �v���C���[��(�Q�[�W��)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//�x�[�X���W�擾
		D3DXVECTOR3 pos = this->Transform[CntBulletGauge].Pos();
		//�J�E���g���[�v �e��
		for (int CntAmmoStock = 0; CntAmmoStock < PLAYER_AMMOSTOCK*2; CntAmmoStock++)
		{
			//�u�g�v���_�Ɓu���g�v���_�ŕʂ̏���������
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			//�g
			if (CntAmmoStock < PLAYER_AMMOSTOCK)//0-9�͈̔�
			{
				vtx[0] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
			}
			//���g
			else
			{
				//�I�t�Z�b�g�p���[�v�J�E���g
				int cnt = CntAmmoStock - PLAYER_AMMOSTOCK;
				vtx[0] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET), pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
			}
			//�`��ʒu���f
			this->vtx[CntBulletGauge].Vertex2D(CntAmmoStock, vtx);

			//RHW�ݒ�
			this->vtx[CntBulletGauge].RHW2D(CntAmmoStock);

			//UV�̐ݒ�
			this->vtx[CntBulletGauge].UV2D(CntAmmoStock);

			//�J���[�ݒ�
			this->vtx[CntBulletGauge].Color2D(CntAmmoStock, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	// �e�N�X�`���̓ǂݍ���
	this->tex[BULLETGAUGE_TEX_BASE].LoadTexture(TEXTURE_BULLETGAUGE_EMPTY);
	this->tex[BULLETGAUGE_TEX_SHELL].LoadTexture(TEXTURE_BULLETGAUGE);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
BULLETGAUGE::~BULLETGAUGE(void)
{
	//�e�N�X�`�����
	this->tex[0].~TEXTURE();
	this->tex[1].~TEXTURE();
	//�J�E���g���[�v �v���C���[��(�o���b�g�Q�[�W��)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//���_���
		this->vtx[CntBulletGauge].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void BULLETGAUGE::Init(void)
{
	//�J�E���g���[�v �v���C���[��(�o���b�g�Q�[�W��)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//�����ݒ�͒e�򖞃^������X�^�[�g
		this->BulletGaugePara[CntBulletGauge].AmmoStock = PLAYER_AMMOSTOCK;
	}

	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(BULLETGAUGE_P1_POS_X, BULLETGAUGE_P1_POS_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(BULLETGAUGE_P2_POS_X, BULLETGAUGE_P2_POS_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(BULLETGAUGE_P3_POS_X, BULLETGAUGE_P3_POS_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(BULLETGAUGE_P4_POS_X, BULLETGAUGE_P4_POS_Y, 0.0f));

	//�J�E���g���[�v �v���C���[��(�Q�[�W��)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//�x�[�X���W�擾
		D3DXVECTOR3 pos = this->Transform[CntBulletGauge].Pos();
		//�J�E���g���[�v �e��
		for (int CntAmmoStock = 0; CntAmmoStock < PLAYER_AMMOSTOCK * 2; CntAmmoStock++)
		{
			//�u�g�v���_�Ɓu���g�v���_�ŕʂ̏���������
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			//�g
			if (CntAmmoStock < PLAYER_AMMOSTOCK)//0-9�͈̔�
			{
				vtx[0] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
			}
			//���g
			else
			{
				//�I�t�Z�b�g�p���[�v�J�E���g
				int cnt = CntAmmoStock - PLAYER_AMMOSTOCK;
				vtx[0] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET), pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
			}
			//�`��ʒu���f
			this->vtx[CntBulletGauge].Vertex2D(CntAmmoStock, vtx);

			//RHW�ݒ�
			this->vtx[CntBulletGauge].RHW2D(CntAmmoStock);

			//UV�̐ݒ�
			this->vtx[CntBulletGauge].UV2D(CntAmmoStock);

			//�J���[�ݒ�
			this->vtx[CntBulletGauge].Color2D(CntAmmoStock, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// �ď����������@�l�b�g�ΐ�O
//=============================================================================
void BULLETGAUGE::InitNet(int MyNumber)
{
	//�`��ʒu�ݒ�
	this->Transform[MyNumber].Pos(D3DXVECTOR3(BULLETGAUGE_NET_POS_X, BULLETGAUGE_NET_POS_Y, 0.0f));
	//�x�[�X���W�擾
	D3DXVECTOR3 pos = this->Transform[MyNumber].Pos();
	//�J�E���g���[�v �e��
	for (int CntAmmoStock = 0; CntAmmoStock < PLAYER_AMMOSTOCK * 2; CntAmmoStock++)
	{
		//�u�g�v���_�Ɓu���g�v���_�ŕʂ̏���������
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
		//�g
		if (CntAmmoStock < PLAYER_AMMOSTOCK)//0-9�͈̔�
		{
			vtx[0] = D3DXVECTOR3(pos.x + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET * 2), pos.y - BULLETGAUGE_SIZE_Y, 0.0f);
			vtx[1] = D3DXVECTOR3(pos.x + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y - BULLETGAUGE_SIZE_Y, 0.0f);
			vtx[2] = D3DXVECTOR3(pos.x + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
			vtx[3] = D3DXVECTOR3(pos.x + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
		}
		//���g
		else
		{
			//�I�t�Z�b�g�p���[�v�J�E���g
			int cnt = CntAmmoStock - PLAYER_AMMOSTOCK;
			vtx[0] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (cnt*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y_OFFSET - BULLETGAUGE_SIZE_Y, 0.0f);
			vtx[1] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (cnt*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y_OFFSET - BULLETGAUGE_SIZE_Y, 0.0f);
			vtx[2] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (cnt*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
			vtx[3] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (cnt*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
		}
		//�`��ʒu���f
		this->vtx[MyNumber].Vertex2D(CntAmmoStock, vtx);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void BULLETGAUGE::Update(PLAYER *player)
{
	//�J�E���g���[�v �v���C���[��(�o���b�g�Q�[�W��)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//�v���C���[�̒e�����L�^
		this->BulletGaugePara[CntBulletGauge].AmmoStock = player->PlayerPara[CntBulletGauge].BulletPara.BulletStock;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void BULLETGAUGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ΐ펞
	if (Netflag == false)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�J�E���g���[�v�@�g�̐�
		for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOSTOCK; CntAmmo++)
		{
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_BASE].Texture());//�g�Ȃ̂�[0]
			// �g��`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4), POLYGON_2D_NUM);
		}
		//�J�E���g���[�v�@���g�̐�
		for (int CntAmmo = 0; CntAmmo < this->BulletGaugePara[CntPlayer].AmmoStock; CntAmmo++)
		{
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_SHELL].Texture());		//�g�Ȃ̂�[1]
			// ���g��`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4)+ (PLAYER_AMMOSTOCK*4), POLYGON_2D_NUM);
		}
	}
	//�l�b�g�ΐ펞
	else
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, this->vtx[NetMyNumber].VtxBuff(), 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�J�E���g���[�v�@�g�̐�
		for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOSTOCK; CntAmmo++)
		{
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_BASE].Texture());//�g�Ȃ̂�[0]
			// �g��`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4), POLYGON_2D_NUM);
		}
		//�J�E���g���[�v�@���g�̐�
		for (int CntAmmo = 0; CntAmmo < this->BulletGaugePara[NetMyNumber].AmmoStock; CntAmmo++)
		{
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_SHELL].Texture());		//�g�Ȃ̂�[1]
			// ���g��`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4) + (PLAYER_AMMOSTOCK * 4), POLYGON_2D_NUM);
		}
	}
}

