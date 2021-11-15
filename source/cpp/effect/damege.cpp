/**
* @file damege.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../h/Effect/damege.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_DAMEGE			("../data/TEXTURE/effect/screendamage.png")	

constexpr int	DAMEGE_SIZE_X{ SCREEN_W / 4 };		//!< �_���[�W�e�N�X�`���̕�
constexpr int	DAMEGE_SIZE_Y{ SCREEN_H / 4 };		//!< �_���[�W�e�N�X�`���̍����l�b�g�p
constexpr int	DAMEGE_SIZE_X_NET{ SCREEN_W / 2 };		//!< �_���[�W�e�N�X�`���̕��l�b�g�p
constexpr int	DAMEGE_SIZE_Y_NET{ SCREEN_H / 2 };		//!< �_���[�W�e�N�X�`���̍���
constexpr int	DAMEGE_POS_X{ SCREEN_CENTER_X };	//!< �_���[�W�e�N�X�`���̕\���ʒu
constexpr int	DAMEGE_POS_Y{ SCREEN_CENTER_Y };	//!< �_���[�W�e�N�X�`���̕\���ʒu
constexpr float SCREENDAMEGE_TIME{ 20.0f };			//!< ��_���[�W���̉�ʃt�F�[�h����

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
DAMEGE::DAMEGE(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex2D(OBJECT_DAMEGE_MAX, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f));

	//�J�E���g���[�v
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		//�`��ʒu���f
		D3DXVECTOR3 pos = this->Transform[CntDamege].Pos();
		this->vtx.Vertex2D(CntDamege, DAMEGE_SIZE_X, DAMEGE_SIZE_Y, pos);

		//RHW�ݒ�
		this->vtx.RHW2D(CntDamege);

		//UV�̐ݒ�
		this->vtx.UV2D(CntDamege);

		//�J���[�ݒ�
		this->vtx.Color2D(CntDamege, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntDamege].Use(NoUse);
	}

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_DAMEGE);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
DAMEGE::~DAMEGE(void)
{
	//�e�N�X�`�����
	this->tex.~TEXTURE();
	//���_���
	this->vtx.~VTXBuffer();
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void DAMEGE::Init(void)
{
	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y - DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(DAMEGE_POS_X - DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(DAMEGE_POS_X + DAMEGE_SIZE_X, DAMEGE_POS_Y + DAMEGE_SIZE_Y, 0.0f));

	//�J�E���g���[�v
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		//�`��ʒu���f
		D3DXVECTOR3 pos = this->Transform[CntDamege].Pos();
		this->vtx.Vertex2D(CntDamege, DAMEGE_SIZE_X, DAMEGE_SIZE_Y, pos);

		//RHW�ݒ�
		this->vtx.RHW2D(CntDamege);

		//UV�̐ݒ�
		this->vtx.UV2D(CntDamege);

		//�J���[�ݒ�
		this->vtx.Color2D(CntDamege, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntDamege].Use(NoUse);

		//�p�����[�^�ݒ�
		this->DamegePara[CntDamege].time = 0.0f;
		this->DamegePara[CntDamege].alpha = 0.0f;

	}
}

//=============================================================================
// �ď���������
//=============================================================================
void DAMEGE::InitNet(int MyNumber)
{
	//�`��ʒu�ݒ�
	this->Transform[MyNumber].Pos(D3DXVECTOR3(DAMEGE_POS_X, DAMEGE_POS_Y, 0.0f));

	//�`��ʒu���f
	D3DXVECTOR3 pos = this->Transform[MyNumber].Pos();

	D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
	vtx[0] = D3DXVECTOR3(pos.x - DAMEGE_POS_X, pos.y - DAMEGE_POS_Y, 0.0f);
	vtx[1] = D3DXVECTOR3(pos.x + DAMEGE_POS_X, pos.y - DAMEGE_POS_Y, 0.0f);
	vtx[2] = D3DXVECTOR3(pos.x - DAMEGE_POS_X, pos.y + DAMEGE_POS_Y, 0.0f);
	vtx[3] = D3DXVECTOR3(pos.x + DAMEGE_POS_X, pos.y + DAMEGE_POS_Y, 0.0f);

	this->vtx.Vertex2D(MyNumber, DAMEGE_SIZE_X_NET, DAMEGE_SIZE_Y_NET, pos);
}

//=============================================================================
// �X�V����
//=============================================================================
void DAMEGE::Update(void)
{
	//�J�E���g���[�v
	for (int CntDamege = 0; CntDamege < OBJECT_DAMEGE_MAX; CntDamege++)
	{
		//�g�p���Ȃ珙�X�Ƀ�����������
		if (this->iUseType[CntDamege].Use() ==YesUseType1)
		{
			this->DamegePara[CntDamege].time += 1.0f;
			this->DamegePara[CntDamege].alpha = this->DamegePara[CntDamege].time / SCREENDAMEGE_TIME;

			// ���ˌ��̐ݒ�
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, this->DamegePara[CntDamege].alpha);
			this->vtx.Color2D(CntDamege, col);

			//��莞�Ԍo�߂ŏI��
			if (this->DamegePara[CntDamege].time >= SCREENDAMEGE_TIME)
			{
				this->DamegePara[CntDamege].time = 0;
				this->DamegePara[CntDamege].alpha = 0.0f;
				this->iUseType[CntDamege].Use(NoUse);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DAMEGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z��r�Ȃ�

	if (Netflag==false)
	{
		//�`�攻��@
		if (this->iUseType[CntPlayer].Use() == YesUseType1)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex.Texture());
			// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntPlayer * 4), POLYGON_2D_NUM);
		}
	}

	else
	{
		//�`�攻��@
		if (this->iUseType[NetMyNumber].Use() == YesUseType1)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex.Texture());
			// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NetMyNumber * 4), POLYGON_2D_NUM);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z��r����
}

