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

#define	VITALGAUGE_SIZE_X			(500.0f)						//!< �A�^�b�N�Q�[�W�̐����̕�
#define	VITALGAUGE_SIZE_Y			(20.0f)							//!< �A�^�b�N�Q�[�W�̐����̍���
#define	VITALGAUGE_SIZE_X_OFFSET	(0.8f)							//!< �A�^�b�N�Q�[�W�̐����̕�
#define	VITALGAUGE_SIZE_Y_OFFSET	(0.8f)							//!< �A�^�b�N�Q�[�W�̐����̍���
#define	VITALGAUGE_BASE_POS_X		(570.0f)						//!< �A�^�b�N�Q�[�W�̕\����ʒu�w���W
#define	VITALGAUGE_BASE_POS_Y		(SCREEN_CENTER_Y/10)			//!< �A�^�b�N�Q�[�W�̕\����ʒu�x���W

#define VITALGAUGE_P1_POS_X			SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P1_POS_Y			SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P2_POS_X			SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P2_POS_Y			SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P3_POS_X			SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P3_POS_Y			SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P4_POS_X			SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P4_POS_Y			SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_NET_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X+55.0f
#define VITALGAUGE_NET_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y

#define	VITAL_GREEN_DIAMETER		(0.5f)	//!< 
#define	VITAL_ORANGE_DIAMETER		(0.2f)	//!< 
#define	VITAL_RED_DIAMETER			(0.1f)	//!< 

#define	VITAL_DRAW_TEX_NUM			(2)		//!< �e�v���C���[���`�悷��e�N�X�`���̐�(�g�A�{��)

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
VITALGAUGE::VITALGAUGE(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx[PLAYER01].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);
	this->vtx[PLAYER02].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);
	this->vtx[PLAYER03].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);
	this->vtx[PLAYER04].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));

	//�J�E���g���[�v �v���C���[��(�Q�[�W��)
	for (int CntVital = 0; CntVital < OBJECT_VITAL_MAX; CntVital++)
	{
		//�x�[�X���W�擾
		D3DXVECTOR3 pos = this->Transform[CntVital].Pos;
		//�J�E���g���[�v �e��
		for (int CntVitalTex = 0; CntVitalTex < VITAL_DRAW_TEX_NUM; CntVitalTex++)
		{
			//�u�g�v���_�Ɓu���g�v���_�ŕʂ̏���������
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			if (CntVitalTex==0)//�g 0
			{
				vtx[0] = D3DXVECTOR3(pos.x, pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
			}
			else//���g 1	 
			{
				float vitallen = float(VITALGAUGE_SIZE_X * (this->VitalGaugePara[CntVital].VitalPower / PLAYER_VITAL_MAX));//�o�C�^���̒���
				vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			}
			//�`��ʒu���f
			this->vtx[CntVital].Vertex2D(CntVitalTex, vtx);

			//RHW�ݒ�
			this->vtx[CntVital].RHW2D(CntVitalTex);

			//UV�̐ݒ�
			this->vtx[CntVital].UV2D(CntVitalTex);

			//�J���[�ݒ�
			this->vtx[CntVital].Color2D(CntVitalTex, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	//�J�E���g���[�v VitalGauge�̃e�N�X�`����
	for (int CntVitalTex = 0; CntVitalTex < VITAL_TEX_MAX; CntVitalTex++)
	{
		// �e�N�X�`���̓ǂݍ���
		this->tex[CntVitalTex].LoadTexture(this->c_aFileNameTex[CntVitalTex]);
	}
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
VITALGAUGE::~VITALGAUGE(void)
{
	//�J�E���g���[�v VitalGauge�̃e�N�X�`����
	for (int CntVitalTex = 0; CntVitalTex < VITAL_TEX_MAX; CntVitalTex++)
	{
		//�e�N�X�`�����
		this->tex[CntVitalTex].~TEXTURE();
	}
	//�J�E���g���[�v �v���C���[��(�Q�[�W��)
	for (int CntVital = 0; CntVital < OBJECT_BULLETGAUGE_MAX; CntVital++)
	{
		//���_���
		this->vtx[CntVital].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void VITALGAUGE::Init(void)
{
	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));

	//�J�E���g���[�v �v���C���[��(�Q�[�W��)
	for (int CntVital = 0; CntVital < OBJECT_VITAL_MAX; CntVital++)
	{
		//�̗͖��^��
		this->VitalGaugePara[CntVital].VitalPower = PLAYER_VITAL_MAX;

		//�x�[�X���W�擾
		D3DXVECTOR3 pos = this->Transform[CntVital].Pos;
		//�J�E���g���[�v �e��
		for (int CntVitalTex = 0; CntVitalTex < VITAL_DRAW_TEX_NUM; CntVitalTex++)
		{
			//�u�g�v���_�Ɓu���g�v���_�ŕʂ̏���������
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			if (CntVitalTex == 0)//�g
			{
				vtx[0] = D3DXVECTOR3(pos.x, pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
			}
			else//���g		
			{
				float vitallen = float(VITALGAUGE_SIZE_X * (this->VitalGaugePara[CntVital].VitalPower / PLAYER_VITAL_MAX));//�o�C�^���̒���
				vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			}
			//�`��ʒu���f
			this->vtx[CntVital].Vertex2D(CntVitalTex, vtx);
		}
	}
}

//=============================================================================
// �ď����������@�l�b�g�ΐ�O
//=============================================================================
void VITALGAUGE::ReinitNet(int MyNumber)
{
	/*
	this[MyNumber].Pos(D3DXVECTOR3(VITALGAUGE_NET_POS_X, VITALGAUGE_NET_POS_Y, 0.0f));
	//���W�擾
	D3DXVECTOR3 pos = this[MyNumber].Pos();

	//���_���W�̐ݒ�
	//BASE
	this[MyNumber].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	this[MyNumber].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X*2, pos.y, 0.0f);
	this[MyNumber].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x, pos.y + VITALGAUGE_SIZE_Y*2, 0.0f);
	this[MyNumber].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X*2, pos.y + VITALGAUGE_SIZE_Y*2, 0.0f);


	//���g
	//�o�C�^���̒���
	float vitallen = float(VITALGAUGE_SIZE_X*2 * (this[MyNumber].VitalPower / PLAYER_VITAL_MAX));
	// ���_���W�̐ݒ�
	this[MyNumber].Tex[0].textureVTX[0].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[MyNumber].Tex[0].textureVTX[1].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[MyNumber].Tex[0].textureVTX[2].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y*2 - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[MyNumber].Tex[0].textureVTX[3].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y*2 - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	*/
}

//=============================================================================
// �X�V����
//=============================================================================
void VITALGAUGE::Update(PLAYER_HONTAI *p, RANK *rank, bool Netflag, int NetMyNumber)
{
	//���g�X�V
	//���[�J���ΐ펞
	if (Netflag == false)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
		{
			//�o�C�^���L�^
			this->VitalGaugePara[CntPlayer].VitalPower = p->PlayerPara[CntPlayer].vital;

			//�v���C���[�������Ă���Ƃ��Ƀo�C�^����0�ȉ��Ŗ��g�p�ɂ���i���S�t���O�j�@���������_�ł̏��ʂ��Z�b�g����
			bool puse = p->iUseType[CntPlayer].Use();
			if (puse == true)
			{
				if (this->VitalGaugePara[CntPlayer].VitalPower <= 0)
				{
					p->iUseType[CntPlayer].Use(false);
					rank->SetRank(CntPlayer);
				}
			}

			//���W�擾
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos();

			//�o�C�^���̒���
			float vitallen = float(VITALGAUGE_SIZE_X) * float(this->VitalGaugePara[CntPlayer].VitalPower) / float(PLAYER_VITAL_MAX);

			// ���_���W�̐ݒ�
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this->vtx[CntPlayer].Vertex2D(1, vtx);//���g��1
		}
	}

	//�l�b�g�ΐ펞
	else
	{
		//�o�C�^���L�^
		this->VitalGaugePara[NetMyNumber].VitalPower = p->PlayerPara[NetMyNumber].vital;

		//�v���C���[�������Ă���Ƃ��Ƀo�C�^����0�ȉ��Ŗ��g�p�ɂ���i���S�t���O�j�@���������_�ł̏��ʂ��Z�b�g����
		for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
		{
			bool puse = p->iUseType[CntPlayer].Use();
			if (puse == true)
			{
				if (this->VitalGaugePara[CntPlayer].VitalPower <= 0)
				{
					p->iUseType[CntPlayer].Use(false);
					rank->SetRankNet(CntPlayer, NetMyNumber);
				}
			}
		}
		//���W�擾
		D3DXVECTOR3 pos = this->Transform[NetMyNumber].Pos();

		//�o�C�^���̒���
		float vitallen = float(VITALGAUGE_SIZE_X) * 2 * float(this->VitalGaugePara[NetMyNumber].VitalPower) / float(PLAYER_VITAL_MAX);

		// ���_���W�̐ݒ�
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
		vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		this->vtx[NetMyNumber].Vertex2D(1, vtx);//���g��1
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void VITALGAUGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//���[�J���ΐ펞
	if (Netflag == false)
	{
		//�g�`��[�����g�`��
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, *this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
		pDevice->SetTexture(0, this->tex[0].Texture());
		//BASE��`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		// �e�N�X�`���̐ݒ�
		if (this->VitalGaugePara[CntPlayer].VitalPower >= VITAL_GREEN_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_GREEN].Texture());
		else if (this->VitalGaugePara[CntPlayer].VitalPower >= VITAL_ORANGE_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_ORANGE].Texture());
		else pDevice->SetTexture(0, this->tex[VITAL_TEX_RED].Texture());
		// ���g��`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, POLYGON_2D_NUM);
	}
	//�l�b�g�ΐ펞
	else
	{
		//�g�`��[�����g�`��
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, *this->vtx[NetMyNumber].VtxBuff(), 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
		pDevice->SetTexture(0, this->tex[0].Texture());
		//BASE��`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		// �e�N�X�`���̐ݒ�
		if (this->VitalGaugePara[NetMyNumber].VitalPower >= VITAL_GREEN_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_GREEN].Texture());
		else if (this->VitalGaugePara[NetMyNumber].VitalPower >= VITAL_ORANGE_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_ORANGE].Texture());
		else pDevice->SetTexture(0, this->tex[VITAL_TEX_RED].Texture());
		// ���g��`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, POLYGON_2D_NUM);
	}
}

