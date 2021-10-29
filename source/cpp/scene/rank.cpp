/**
* @file rank.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../h/scene/rank.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	RANK_SIZE_X				(SCREEN_W/4)				//!< ���ꂽ���ʃe�N�X�`���̕�
#define	RANK_SIZE_Y				(SCREEN_H/4)				//!< ���ꂽ���ʃe�N�X�`���̍���
#define	RANK_POS_X				(SCREEN_W)					//!< ���ꂽ���ʃe�N�X�`���̕\���ʒu
#define	RANK_POS_Y				(SCREEN_H)					//!< ���ꂽ���ʃe�N�X�`���̕\���ʒu
#define	RANK_POS_OFFSET_LOW		(1.0f/4.0f)					//!< �X�N���[��������
#define	RANK_POS_OFFSET_HI		(3.0f/4.0f)					//!< �X�N���[��������

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
RANK::RANK(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex2D(OBJECT_RANK_MAX, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_LOW, RANK_POS_Y *RANK_POS_OFFSET_LOW, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_HI, RANK_POS_Y *RANK_POS_OFFSET_LOW, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_LOW, RANK_POS_Y *RANK_POS_OFFSET_HI, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_HI, RANK_POS_Y *RANK_POS_OFFSET_HI, 0.0f));

	//�J�E���g���[�v
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		//�`��ʒu�ݒ�
		D3DXVECTOR3 pos = this->Transform[CntRank].Pos();
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(pos.x - RANK_SIZE_X,pos.y - RANK_SIZE_Y,0.0f),
		D3DXVECTOR3(pos.x + RANK_SIZE_X,pos.y - RANK_SIZE_Y,0.0f),
		D3DXVECTOR3(pos.x - RANK_SIZE_X,pos.y + RANK_SIZE_Y,0.0f),
		D3DXVECTOR3(pos.x + RANK_SIZE_X,pos.y + RANK_SIZE_Y,0.0f),
		};
		this->vtx.Vertex2D(CntRank, vtx);

		//RHW�ݒ�
		this->vtx.RHW2D(CntRank);

		//UV�̐ݒ�
		this->vtx.UV2D(CntRank);

		//�J���[�ݒ�
		this->vtx.Color2D(CntRank, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntRank].Use(NoUse);
	}

	//�J�E���g���[�v
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX-1; CntRank++)
	{
		// �e�N�X�`���̓ǂݍ���
		this->tex[CntRank].LoadTexture(this->c_aFileNameTex[CntRank]);
	}
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
RANK::~RANK(void)
{
	//�J�E���g���[�v
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX - 1; CntRank++)
	{
		//�e�N�X�`�����
		this->tex[CntRank].~TEXTURE();
	}
	//���_���
	this->vtx.~VTXBuffer();
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void RANK::Init(void)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		this->iUseType[CntRank].Use(NoUse);
		this->RankParaOne.NetUse = false;
		this->RankParaAll[CntRank].RankNum = RANK_COUNTDOWN_NUM;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void RANK::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void RANK::Draw(bool Netflag)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ΐ펞
	if (Netflag == false)
	{
		for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
		{
			if (this->iUseType[CntRank].Use() == YesUseType1)
			{
				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_2D);
				// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
				pDevice->SetTexture(0, this->tex[this->RankParaAll[CntRank].RankNum].Texture());
				// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntRank * 4), POLYGON_2D_NUM);
			}
		}
	}

	//�l�b�g�ΐ펞
	else
	{
		for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
		{
			bool use = this->iUseType[CntRank].Use();
			if (use == true)
			{
				if (this->RankParaOne.NetUse == true)
				{
					// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
					pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_2D);
					// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
					pDevice->SetTexture(0, this->tex[this->RankParaAll[CntRank].RankNum].Texture());
					// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntRank * 4), POLYGON_2D_NUM);
				}
			}
		}
	}
}
//=============================================================================
// �����N���Z�b�g
//=============================================================================
void RANK::SetRank(int PlayerNum)
{
	//�g�p�\�ɂ���
	this->iUseType[PlayerNum].Use(YesUseType1);
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		if (this->iUseType[CntRank].Use() == NoUse)
		{
			//4�X�^�[�g�ň�l���ʂ��тɃJ�E���g�_�E������@���񂾐l�̓J�E���g�_�E�����ꂸ�ɏ��ʂ��m��ɂȂ�
			this->RankParaAll[CntRank].RankNum--;
		}
	}
}

//=============================================================================
// �����N���Z�b�g
//=============================================================================
void RANK::SetRankNet(int PlayerNum, int NetMyNumber)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		if (this->iUseType[CntRank].Use() == NoUse)
		{
			//�l�b�g�ΐ�p�ɕ`��ʒu�𒲐�
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
			{
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, SCREEN_H, 0.0f),
			D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f),
			};
			this->vtx.Vertex2D(CntRank, vtx);

			//�����̏��ʂ��L�^���ď��ʂ��g�p����
			this->RankParaAll[CntRank].RankNum = PlayerNum;
			this->iUseType[CntRank].Use(YesUseType1);

			//�v���C���[�i���o�[�ƃl�b�g�ŗL�}�C�i���o�[�������Ȃ�l�b�g�p�t���O�I��
			//�������I���ɂ��Ȃ��ƃl�b�g�ΐ펞�ɏ��ʂ��\������Ȃ�
			if (PlayerNum == NetMyNumber)
			{
				this->RankParaOne.NetUse = true;
			}
			break;
		}
	}
}
