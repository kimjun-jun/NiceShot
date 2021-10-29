/**
* @file explosion.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/effect/explosion.h"

//*****************************************************************************
// �}�N����` nPatternX
//*****************************************************************************
#define	TEXTURE_EXPLOSION	"../data/TEXTURE/effect/bomb.png"		//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PATTERN_X_UV_EXPLOSION		0.25f					//!< �e�N�X�`���X�v���C�gX
#define	TEXTURE_PATTERN_Y_UV_EXPLOSION		0.25f					//!< �e�N�X�`���X�v���C�gY
#define	TEXTURE_PATTERN_X_COUNT_EXPLOSION	4						//!< �e�N�X�`���X�v���C�gX
#define	TEXTURE_PATTERN_Y_COUNT_EXPLOSION	4						//!< �e�N�X�`���X�v���C�gY
#define	TEXTURE_PATTERN_COUNTER_EXPLOSION	4						//!< �e�N�X�`���X�v���C�g�̃J�E���g�^�C�}�[
#define	EXPLOSION_VERTEX_SIZE				(5.0f)					//!< EXPLOSION���_�T�C�Y
#define	EXPLOSION_VERTEX_ADD_SIZE			(0.5f)					//!< EXPLOSION���_�T�C�Y


//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
EXPLOSION::EXPLOSION(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬
	this->vtx.MakeVertex3DBill(OBJECT_EXPLOSION_MAX, FVF_VERTEX_3D);

	//�J�E���g���[�v
	for (int CntExplosion = 0; CntExplosion < OBJECT_EXPLOSION_MAX; CntExplosion++)
	{
		//�`��ʒu���f
		D3DXVECTOR3 pos[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(-EXPLOSION_VERTEX_SIZE / 2, 0.0f, EXPLOSION_VERTEX_SIZE / 2),
		D3DXVECTOR3(EXPLOSION_VERTEX_SIZE / 2, 0.0f, EXPLOSION_VERTEX_SIZE / 2),
		D3DXVECTOR3(-EXPLOSION_VERTEX_SIZE / 2, 0.0f, -EXPLOSION_VERTEX_SIZE / 2),
		D3DXVECTOR3(EXPLOSION_VERTEX_SIZE / 2, 0.0f, -EXPLOSION_VERTEX_SIZE / 2),
		};
		//�`��ʒu���f
		this->vtx.Vertex3D(CntExplosion, pos);

		//RHW�ݒ�
		this->vtx.Nor3D(CntExplosion);

		//UV�̐ݒ�
		this->vtx.UV3D(CntExplosion, 0, 0, 0, 0);

		//�J���[�ݒ�
		this->vtx.Color3D(CntExplosion, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntExplosion].Use(NoUse);

	}

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_EXPLOSION);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
EXPLOSION::~EXPLOSION(void)
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
void EXPLOSION::Init(void)
{
	//�J�E���g���[�v
	for (int CntExplosion = 0; CntExplosion < OBJECT_EXPLOSION_MAX; CntExplosion++)
	{
		//�������ݒ�
		this->Transform[CntExplosion].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[CntExplosion].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[CntExplosion].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this->vtx.Color3D(CntExplosion);
		this->vtx.UV3D(CntExplosion, 0, 0, 0, 0);
		this->ExploPara[CntExplosion].nPatternX = 0;
		this->ExploPara[CntExplosion].nPatternY = 0;
		this->ExploPara[CntExplosion].eType = EXPLOSIONTYPE_BULLET_NONE;
		this->ExploPara[CntExplosion].fSizeX = 0.0f;
		this->ExploPara[CntExplosion].fSizeY = 0.0f;
		this->iUseType[CntExplosion].Use(NoUse);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void EXPLOSION::Update(void)
{
	//�g�p���̏ꍇ�@�X�v���C�g�Đ���@���g�p���
	for(int CntExplosion = 0; CntExplosion < OBJECT_EXPLOSION_MAX; CntExplosion++)
	{
		if(this->iUseType[CntExplosion].Use()==YesUseType1)
		{
			//�X�v���C�g�Đ��@�J�E���g����UV�e�N�X�`����؂�ւ���
			this->ExploPara[CntExplosion].nCounter++;
			if((this->ExploPara[CntExplosion].nCounter % TEXTURE_PATTERN_COUNTER_EXPLOSION) == 0)
			{
				this->ExploPara[CntExplosion].nPatternX++;
				if(this->ExploPara[CntExplosion].nPatternY >= TEXTURE_PATTERN_Y_COUNT_EXPLOSION)
				{
					this->iUseType[CntExplosion].Use(NoUse);
				}
				else if (this->ExploPara[CntExplosion].nPatternX >= TEXTURE_PATTERN_X_COUNT_EXPLOSION)
				{
					this->ExploPara[CntExplosion].nPatternX = 0;
					this->ExploPara[CntExplosion].nPatternY++;
				}
				else
				{
					// �e�N�X�`�����W�̐ݒ�
					this->vtx.UV3D(CntExplosion,
						float(this->ExploPara[CntExplosion].nPatternX*TEXTURE_PATTERN_X_UV_EXPLOSION),
						float(this->ExploPara[CntExplosion].nPatternX*TEXTURE_PATTERN_X_UV_EXPLOSION + TEXTURE_PATTERN_X_UV_EXPLOSION),
						float(this->ExploPara[CntExplosion].nPatternY*TEXTURE_PATTERN_Y_UV_EXPLOSION),
						float(this->ExploPara[CntExplosion].nPatternY*TEXTURE_PATTERN_Y_UV_EXPLOSION + TEXTURE_PATTERN_Y_UV_EXPLOSION));
				}
			}

			// ���_�T�C�Y�̐ݒ�
			this->ExploPara[CntExplosion].fSizeX += EXPLOSION_VERTEX_ADD_SIZE;
			this->ExploPara[CntExplosion].fSizeY += EXPLOSION_VERTEX_ADD_SIZE;
			//�`��ʒu���f
			D3DXVECTOR3 pos[POLYGON_2D_VERTEX] =
			{
			D3DXVECTOR3(-this->ExploPara[CntExplosion].fSizeX / 2, this->ExploPara[CntExplosion].fSizeY / 2, 0.0f),
			D3DXVECTOR3(this->ExploPara[CntExplosion].fSizeX / 2, this->ExploPara[CntExplosion].fSizeY / 2, 0.0f),
			D3DXVECTOR3(-this->ExploPara[CntExplosion].fSizeX / 2, -this->ExploPara[CntExplosion].fSizeY / 2, 0.0f),
			D3DXVECTOR3(this->ExploPara[CntExplosion].fSizeX / 2, -this->ExploPara[CntExplosion].fSizeY / 2, 0.0f),
			};
			// ���_���W�̐ݒ�
			this->vtx.Vertex3D(CntExplosion, pos);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void EXPLOSION::Draw(int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int CntExplosion = 0; CntExplosion < OBJECT_EXPLOSION_MAX; CntExplosion++)
	{
		if (this->iUseType[CntExplosion].Use() == YesUseType1)
		{
			//--------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[CntExplosion].Pos();
			D3DXVECTOR3 scl = this->Transform[CntExplosion].Scl();
			D3DXMATRIX mtxWorldExplosion;

			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldExplosion);

			// �r���[�}�g���b�N�X���擾
			CAMERA *cam = GetCamera();

			mtxWorldExplosion._11 = cam[CntPlayer].mtxView._11;
			mtxWorldExplosion._12 = cam[CntPlayer].mtxView._21;
			mtxWorldExplosion._13 = cam[CntPlayer].mtxView._31;
			mtxWorldExplosion._21 = cam[CntPlayer].mtxView._12;
			mtxWorldExplosion._22 = cam[CntPlayer].mtxView._22;
			mtxWorldExplosion._23 = cam[CntPlayer].mtxView._32;
			mtxWorldExplosion._31 = cam[CntPlayer].mtxView._13;
			mtxWorldExplosion._32 = cam[CntPlayer].mtxView._23;
			mtxWorldExplosion._33 = cam[CntPlayer].mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldExplosion, &mtxWorldExplosion, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldExplosion);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, this->tex.Texture());

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntExplosion * 4), POLYGON_2D_NUM);

		}
	}
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// 
//=============================================================================
int EXPLOSION::SetInstance(D3DXVECTOR3 pos, float fSizeX, float fSizeY, eEXPLOSION_TYPE eType, D3DXCOLOR col)
{
	int nIdxExplosion = -1;

	for(int CntExplosion = 0; CntExplosion < OBJECT_EXPLOSION_MAX; CntExplosion++)
	{
		if (this->iUseType[CntExplosion].Use() ==NoUse)
		{
			this->Transform[CntExplosion].Pos(pos);
			this->Transform[CntExplosion].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[CntExplosion].Scl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
			this->vtx.Color3D(CntExplosion,col);
			this->iUseType[CntExplosion].Use(YesUseType1);
			this->ExploPara[CntExplosion].fSizeX = fSizeX;
			this->ExploPara[CntExplosion].fSizeY = fSizeY;
			this->ExploPara[CntExplosion].nCounter = 0;
			this->ExploPara[CntExplosion].nPatternX = 0;
			this->ExploPara[CntExplosion].nPatternY = 0;
			this->ExploPara[CntExplosion].eType = eType;

			// �e�N�X�`�����W�̐ݒ�
			this->vtx.UV3D(CntExplosion, 0, 0, 0, 0);

			nIdxExplosion = CntExplosion;

			break;
		}
	}

	return nIdxExplosion;
}
