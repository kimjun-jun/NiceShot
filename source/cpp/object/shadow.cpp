/**
* @file shadow.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SHADOW		"../data/TEXTURE/other/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SHADOW_SIZE_X		(25.0f)							// �e�̕�
#define	SHADOW_SIZE_Z		(25.0f)							// �e�̍���

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
SHADOW::SHADOW(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex3DBill(OBJECT_SHADOW_MAX, FVF_VERTEX_3D);

	//�J�E���g���[�v
	for (int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		//�`��ʒu���f
		D3DXVECTOR3 pos[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2),
		D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, SHADOW_SIZE_Z / 2),
		D3DXVECTOR3(-SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2),
		D3DXVECTOR3(SHADOW_SIZE_X / 2, 0.0f, -SHADOW_SIZE_Z / 2),
		};
		this->vtx.Vertex3D(CntShadow, pos);

		//NOR�ݒ�
		this->vtx.Nor3D(CntShadow);

		//UV�̐ݒ�
		this->vtx.UV3D(CntShadow);

		//�J���[�ݒ�
		this->vtx.Color3D(CntShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntShadow].ChangeUse(NoUse);
	}

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_SHADOW);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
SHADOW::~SHADOW(void)
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
void SHADOW::Init(void)
{
	for (int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		this->Transform[CntShadow].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[CntShadow].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[CntShadow].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this->iUseType[CntShadow].Use(false);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void SHADOW::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for(int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		bool use = this->iUseType[CntShadow].Use();
		if(use)
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

			//-----------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[CntShadow].Pos();
			D3DXVECTOR3 rot = this->Transform[CntShadow].Rot();
			D3DXVECTOR3 scl = this->Transform[CntShadow].Scl();
			D3DXMATRIX mtxWorld;
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
			pDevice->SetStreamSource(0,this->vtx.VtxBuff(), 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, this->tex.Texture());

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// �e�̍쐬
//=============================================================================
int SHADOW::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl)
{
	int IdxShadow = -1;

	for(int CntShadow = 0; CntShadow < OBJECT_SHADOW_MAX; CntShadow++)
	{
		bool use = this->iUseType[CntShadow].Use();
		if(use != true)
		{
			this->Transform[CntShadow].Pos(pos);
			this->Transform[CntShadow].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[CntShadow].Scl(scl);
			this->iUseType[CntShadow].Use(YesUse);

			IdxShadow = CntShadow;
			break;
		}
	}

	return IdxShadow;
}

//=============================================================================
// �e�̔j��
//=============================================================================
void SHADOW::ReleaseInstance(int Idx)
{
	if(Idx >= 0 && Idx < OBJECT_SHADOW_MAX)
	{
		this->iUseType[Idx].Use(NoUse);
	}

}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void SHADOW::UpdateInstance(int Idx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	if (Idx < 0) return;	// �s���Ȃ烊�^�[������

	this->Transform[Idx].Pos(pos);
	this->Transform[Idx].Rot(rot);
	this->Transform[Idx].Scl(scl);
}

