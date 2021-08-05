/**
* @file effect.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/effect/effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT			"../data/TEXTURE/effect/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

//�G�t�F�N�g�֘A�萔
#define	EFFECT_NORMALSET_SIZE_X				(16.0f)			//!< �G�t�F�N�g�W���̕�
#define	EFFECT_NORMALSET_SIZE_Y				(16.0f)			//!< �G�t�F�N�g�W���̍���


//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
EFFECT::EFFECT(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex3DBill(OBJECT_EFFECT_MAX, FVF_VERTEX_3D);

	//�J�E���g���[�v
	for (int CntEffect = 0; CntEffect < OBJECT_EFFECT_MAX; CntEffect++)
	{
		//�`��ʒu���f
		this->vtx.Vertex3D(CntEffect, EFFECT_NORMALSET_SIZE_X / 2, EFFECT_NORMALSET_SIZE_Y / 2);

		//RHW�ݒ�
		this->vtx.Nor3D(CntEffect);

		//UV�̐ݒ�
		this->vtx.UV3D(CntEffect);

		//�J���[�ݒ�
		this->vtx.Color3D(CntEffect, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntEffect].ChangeUse(NoUse);

		//�ړ��ʐݒ�
		this->move[CntEffect].Move(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_EFFECT);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
EFFECT::~EFFECT(void)
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
void EFFECT::Init(void)
{
	//�J�E���g���[�v
	for (int CntEffect = 0; CntEffect < OBJECT_EFFECT_MAX; CntEffect++)
	{
		//�J���[�ݒ�
		this->vtx.Color3D(CntEffect, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntEffect].ChangeUse(NoUse);

		//�ړ��ʐݒ�
		this->move[CntEffect].Move(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//�g�����X�t�H�[���ݒ�
		this->Transform[CntEffect].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[CntEffect].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		//�p�����[�^�ݒ�
		this->EffectPara[CntEffect].nTimer = 0;
		this->EffectPara[CntEffect].nDecAlpha = 0.0f;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void  EFFECT::Update(void)
{
	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this->iUseType[nCntEffect].Use();
		if (use)
		{
			//--------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[nCntEffect].Pos();
			D3DXVECTOR3 move = this->move[nCntEffect].Move();
			D3DXCOLOR col = this->vtx.GetColor3D(nCntEffect);

			pos.x += move.x;
			pos.z += move.z;

			col.a -= this->EffectPara[nCntEffect].nDecAlpha;
			if (col.a <= 0.0f)
			{
				col.a = 0.0f;
			}

			this->EffectPara[nCntEffect].nTimer--;
			if (this->EffectPara[nCntEffect].nTimer <= 0)
			{
				this->iUseType[nCntEffect].Use(NoUse);
			}

			//--------------------------------------------�I�u�W�F�N�g�l��������
			this->Transform[nCntEffect].Pos(pos);
			this->vtx.Color3D(col);

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  EFFECT::Draw(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this->iUseType[nCntEffect].Use();
		if (use)
		{
			//--------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[nCntEffect].Pos();
			D3DXVECTOR3 scl = this->Transform[nCntEffect].Scl();
			D3DXCOLOR col = this->vtx.GetColor3D(nCntEffect);
			D3DXMATRIX mtxWorldEffect;

			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldEffect);

			// �r���[�}�g���b�N�X���擾
			CAMERA *cam = GetCamera();

			mtxWorldEffect._11 = cam[CntPlayer].mtxView._11;
			mtxWorldEffect._12 = cam[CntPlayer].mtxView._21;
			mtxWorldEffect._13 = cam[CntPlayer].mtxView._31;
			mtxWorldEffect._21 = cam[CntPlayer].mtxView._12;
			mtxWorldEffect._22 = cam[CntPlayer].mtxView._22;
			mtxWorldEffect._23 = cam[CntPlayer].mtxView._32;
			mtxWorldEffect._31 = cam[CntPlayer].mtxView._13;
			mtxWorldEffect._32 = cam[CntPlayer].mtxView._23;
			mtxWorldEffect._33 = cam[CntPlayer].mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldEffect);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, this->tex.Texture());

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEffect * 4), POLYGON_2D_NUM);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// �C���X�^���X�Z�b�g
//=============================================================================
int EFFECT::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for (int nCntEffect = 0; nCntEffect < OBJECT_EFFECT_MAX; nCntEffect++)
	{
		bool use = this->iUseType[nCntEffect].Use();
		if (use != true)
		{
			this->Transform[nCntEffect].Pos(pos);
			this->Transform[nCntEffect].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[nCntEffect].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this->move[nCntEffect].Move(move);
			this->vtx.Color3D(col);
			this->EffectPara[nCntEffect].nTimer = nTimer;
			this->EffectPara[nCntEffect].nDecAlpha = col.a / nTimer;
			this->iUseType[nCntEffect].Use(YesUse);

			// ���_���W�̐ݒ�
			this->vtx.Vertex3D(nCntEffect, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			this->vtx.Color3D(nCntEffect, col);

			nIdxEffect = nCntEffect;

			break;
		}
	}
	return nIdxEffect;
}
