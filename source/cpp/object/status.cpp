/**
* @file this.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../h/object/status.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr float	STATUS_SIZE_X{ 40.0f };						// �X�e�[�^�X�̕�
constexpr float	STATUS_SIZE_Y{ 40.0f };						// �X�e�[�^�X�̍���
constexpr float	STATUS_SIZE_X_NET{ STATUS_SIZE_X * 2 };		// �X�e�[�^�X�̕�
constexpr float	STATUS_SIZE_Y_NET{ STATUS_SIZE_X * 2 };		// �X�e�[�^�X�̍���
constexpr float	STATUS_SIZE_X_OFFSET{ 5.0f };				// �X�e�[�^�X�̕�
constexpr float	STATUS_POS_X_OFFSET{ 240.0f };				// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g
constexpr float	STATUS_POS_Y_OFFSET{ 100.0f };				// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g
constexpr float	STATUS_POS_X_OFFSETBUFF{ 1.0f };			// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g
constexpr int	STATUS_POS_X{ SCREEN_CENTER_X };			// �X�e�[�^�X�̕\���ʒu
constexpr int	STATUS_POS_Y{ SCREEN_CENTER_Y };			// �X�e�[�^�X�̕\���ʒu

constexpr float STATUS_NET_POS_X_OFFSET{ 260.0f };			// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g�@�l�b�g�p
constexpr float STATUS_NET_POS_Y_OFFSET{ 150.0f };			// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g�@�l�b�g�p

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
STATUS::STATUS(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx[PLAYER01].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);
	this->vtx[PLAYER02].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);
	this->vtx[PLAYER03].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);
	this->vtx[PLAYER04].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(SCREEN_CENTER_X - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(SCREEN_CENTER_X * 2 - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(SCREEN_CENTER_X - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(SCREEN_CENTER_X * 2 - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	//�J�E���g���[�v�@�X�e�[�^�X���ƃv���C���[���̓�d���[�v
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			//�`��ʒu���f
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos();
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
				vtx[0] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET), pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET), pos.y + STATUS_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y + STATUS_SIZE_Y, 0.0f);

			//�`��ʒu���f
			this->vtx[CntPlayer].Vertex2D(CntStatus, vtx);

			//�e�N�X�`���`��g���~���O�����l�@�\����\���������Őݒ�ł���
			this->PtternV[CntPlayer][CntStatus] = 0.0f;

			//RHW�ݒ�
			this->vtx[CntPlayer].RHW2D(CntStatus);

			//UV�ݒ�
			this->vtx[CntPlayer].UV2D(CntStatus);

			//�J���[�ݒ�
			this->vtx[CntPlayer].Color2D(CntStatus, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
		// �e�N�X�`���̓ǂݍ���
		this->tex[CntStatus].LoadTexture(this->c_FileNameTex[CntStatus]);
	}
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
STATUS::~STATUS(void)
{
	//�J�E���g���[�v
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		//�e�N�X�`�����
		this->tex[CntStatus].~TEXTURE();
	}
	//�J�E���g���[�v
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//���_���
		this->vtx[CntPlayer].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void STATUS::Init(void)
{
	//�J�E���g���[�v�@�X�e�[�^�X���ƃv���C���[���̓�d���[�v
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			//�`��ʒu���f
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos();
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			vtx[0] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET), pos.y, 0.0f);
			vtx[1] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y, 0.0f);
			vtx[2] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET), pos.y + STATUS_SIZE_Y, 0.0f);
			vtx[3] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y + STATUS_SIZE_Y, 0.0f);

			//�`��ʒu���f
			this->vtx[CntPlayer].Vertex2D(CntStatus, vtx);

			//�e�N�X�`���`��g���~���O�����l�@�\����\���������Őݒ�ł���
			this->PtternV[CntPlayer][CntStatus] = 0.0f;

			//RHW�ݒ�
			this->vtx[CntPlayer].RHW2D(CntStatus);

			//UV�ݒ�
			this->vtx[CntPlayer].UV2D(CntStatus);

			//�J���[�ݒ�
			this->vtx[CntPlayer].Color2D(CntStatus, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

}

//=============================================================================
// �ď���������
//=============================================================================
void STATUS::InitNet(int MyNumber)
{
	//�T�C�Y�����p
	float buffsize = 48.0f;
	//�`��ʒu�ݒ�
	this->Transform[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET, 0.0f));

	//�J�E���g���[�v�@�X�e�[�^�X��
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		//�`��ʒu���f
		D3DXVECTOR3 pos = this->Transform[MyNumber].Pos();
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
		vtx[0] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET), pos.y, 0.0f);
		vtx[1] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X_NET, pos.y, 0.0f);
		vtx[2] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET), pos.y + STATUS_SIZE_Y_NET, 0.0f);
		vtx[3] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X_NET, pos.y + STATUS_SIZE_Y_NET, 0.0f);

		//�`��ʒu���f
		this->vtx[MyNumber].Vertex2D(CntStatus, vtx);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void STATUS::Update(PLAYER *p)
{
	//�J�E���g���[�v�@�v���C���[���̓�d���[�v
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//speed
		if (p->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SPEED].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SPEED, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SPEED].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SPEED, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
		//���
		if (p->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SENSYA].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SENSYA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SENSYA].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SENSYA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
		//�J����
		if (p->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal == true)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_CAMERA].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_CAMERA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_CAMERA].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_CAMERA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
		//��
		if (p->PlayerPara[CntPlayer].ItemPara.KiriSignal == true)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_KIRI].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_KIRI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_KIRI].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_KIRI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void STATUS::Draw(bool Netflag, int NetMyNumber,int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ΐ�
	if (Netflag == false)
	{
		//�J�E���g���[�v
		for (int CntStatus = 0; CntStatus < STATUS_TYPE_MAX; CntStatus++)
		{
			//if (this->iUseType[CntPlayer][CntStatus].Use() == YesUseType1)
			{
				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_2D));
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);
				// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
				pDevice->SetTexture(0, this->tex[CntStatus].Texture());
				// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4) , POLYGON_2D_NUM);
			}
		}
	}

	//�l�b�g�ΐ�
	else
	{
		//�J�E���g���[�v
		for (int CntStatus = 0; CntStatus < STATUS_TYPE_MAX; CntStatus++)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx[NetMyNumber].VtxBuff(), 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[CntStatus].Texture());
			// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4), POLYGON_2D_NUM);
		}
	}
}

