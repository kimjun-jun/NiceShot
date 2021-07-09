/**
* @file this.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../h/object/status.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	STATUS_SIZE_X			(20.0f)							// �X�e�[�^�X�̕�
#define	STATUS_SIZE_Xhalf		(STATUS_SIZE_X/2)				// �X�e�[�^�X�̕� ����
#define	STATUS_SIZE_Y			(20.0f)							// �X�e�[�^�X�̍���
#define	STATUS_SIZE_Yhalf		(STATUS_SIZE_Y/2)				// �X�e�[�^�X�̍��� ����
#define	STATUS_SIZE_X_OFFSET	(0.8f)							// �X�e�[�^�X�̕�
#define	STATUS_SIZE_Y_OFFSET	(0.8f)							// �X�e�[�^�X�̍���
#define	STATUS_POS_X			(SCREEN_CENTER_X)				// �X�e�[�^�X�̕\���ʒu
#define	STATUS_POS_Y			(SCREEN_CENTER_Y)				// �X�e�[�^�X�̕\���ʒu
#define	STATUS_POS_X_OFFSET		(220.0f)						// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g
#define	STATUS_POS_Y_OFFSET		(80.0f)							// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g
#define	STATUS_POS_X_OFFSETBUFF	(1.0f)							// �X�e�[�^�X�̕\���ʒu�I�t�Z�b�g

#define STATUS_NET_POS_X_OFFSET		(260.0f)
#define STATUS_NET_POS_Y_OFFSET		(48.0f)

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
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	//�J�E���g���[�v�@�X�e�[�^�X���ƃv���C���[���̓�d���[�v
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			//�`��ʒu���f
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos;
			//Half�T�C�Y+(�I�t�Z�b�g�l*�X�e�[�^�X�J�E���g)
			this->vtx[CntPlayer].SetVertex(CntStatus, STATUS_SIZE_Xhalf + CntStatus*(STATUS_SIZE_Xhalf + STATUS_POS_X_OFFSETBUFF), STATUS_SIZE_Yhalf, pos);

			//�e�N�X�`���`��g���~���O�����l�@�\����\���������Őݒ�ł���
			this->PtternV[CntPlayer][CntStatus] = 0.0f;
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
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX*OBJECT_PLAYER_MAX; CntStatus++)
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
	//�`��UV��ݒ肷��
}

//=============================================================================
// �ď���������
//=============================================================================
void STATUS::ReinitNet(int MyNumber)
{
	/*
		float buffsize = 48.0f;
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));

		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			//-----------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this[MyNumber].Pos();

			//Base
			// ���_���W�̐ݒ�
			this[MyNumber].TexEmpty[CntStatus].textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2, 0.0f);
			this[MyNumber].TexEmpty[CntStatus].textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2, 0.0f);
			this[MyNumber].TexEmpty[CntStatus].textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2, 0.0f);
			this[MyNumber].TexEmpty[CntStatus].textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2, 0.0f);

			//���g
			// ���_���W�̐ݒ�
			this[MyNumber].Tex[CntStatus].textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
			this[MyNumber].Tex[CntStatus].textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
			this[MyNumber].Tex[CntStatus].textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
			this[MyNumber].Tex[CntStatus].textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
		}
		*/

}

//=============================================================================
// �X�V����
//=============================================================================
void STATUS::Update(PLAYER_HONTAI *p)
{
	//�`��UV��ݒ肷��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//�X�s�[�h
		if (p[CntPlayer].PlayerPara.speedbuffsignal == true) this[CntPlayer].use[STATUSTYPE_SPEED] = true;
		else this[CntPlayer].use[STATUSTYPE_SPEED] = false;
		//���
		if (p[CntPlayer].ModelType == PLAYER_MODEL_ATTACK) this[CntPlayer].use[STATUSTYPE_SENSYA] = true;
		else this[CntPlayer].use[STATUSTYPE_SENSYA] = false;
		//�J����
		if (p[CntPlayer].PlayerPara.BackCameraItemSignal == true) this[CntPlayer].use[STATUSTYPE_CAMERA] = true;
		else this[CntPlayer].use[STATUSTYPE_CAMERA] = false;
		//��
		if (p[CntPlayer].PlayerPara.KiriSignal == true) this[CntPlayer].use[STATUSTYPE_KIRI] = true;
		else this[CntPlayer].use[STATUSTYPE_KIRI] = false;
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
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, *this->vtx[CntPlayer].GetVtxBuff(), 0, sizeof(VERTEX_2D));
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);
				// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
				pDevice->SetTexture(0, this->tex[CntStatus].GetTexture());
				// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4), POLYGON_2D_NUM);
		}
	}

	//�l�b�g�ΐ�
	else
	{
		//�J�E���g���[�v
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, *this->vtx[NetMyNumber].GetVtxBuff(), 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[CntStatus].GetTexture());
			// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4), POLYGON_2D_NUM);
		}
	}
}

