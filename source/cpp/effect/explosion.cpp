/**
* @file explosion.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
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
	this->vtx.MakeVertex2D(OBJECT_EXPLOSION_MAX, FVF_VERTEX_2D);

	//�J�E���g���[�v
	for (int CntExplosion = 0; CntExplosion < OBJECT_EXPLOSION_MAX; CntExplosion++)
	{
		//�`��ʒu���f
		this->vtx.Vertex2D(CntExplosion, EXPLOSION_VERTEX_SIZE / 2, EXPLOSION_VERTEX_SIZE / 2);

		//RHW�ݒ�
		this->vtx.RHW2D(CntExplosion);

		//UV�̐ݒ�
		this->vtx.UV2D(CntExplosion);

		//�J���[�ݒ�
		this->vtx.Color2D(CntExplosion, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//�g�p�ݒ�
		this->iUseType[CntExplosion].ChangeUse(NoUse);
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
	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		//�������ݒ�
		this->Transform[nCntExplosion].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[nCntExplosion].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->Transform[nCntExplosion].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this->vtx.Color2D(nCntExplosion);
		this->vtx.UV2D(nCntExplosion);
		this->ExploPara[nCntExplosion].nCounter = 0;
		this->ExploPara[nCntExplosion].nPatternX = 0;
		this->ExploPara[nCntExplosion].nPatternY = 0;
		this->ExploPara[nCntExplosion].eType = EXPLOSIONTYPE_BULLET_NONE;;
		this->ExploPara[nCntExplosion].fSizeX = 0.0f;
		this->ExploPara[nCntExplosion].fSizeY = 0.0f;
		this->iUseType[nCntExplosion].Use(NoUse);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void EXPLOSION::Update(void)
{
	//�g�p���̏ꍇ�@�X�v���C�g�Đ���@���g�p���
	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this->iUseType[nCntExplosion].Use();
		if(use)
		{
			//�X�v���C�g�Đ��@�J�E���g����UV�e�N�X�`����؂�ւ���
			this->ExploPara[nCntExplosion].nCounter--;
			if((this->ExploPara[nCntExplosion].nCounter % TEXTURE_PATTERN_COUNTER_EXPLOSION) == 0)
			{
				this->ExploPara[nCntExplosion].nPatternX++;
				if(this->ExploPara[nCntExplosion].nPatternY >= TEXTURE_PATTERN_Y_COUNT_EXPLOSION)
				{
					this->iUseType[nCntExplosion].Use(false);
				}
				else if (this->ExploPara[nCntExplosion].nPatternX >= TEXTURE_PATTERN_X_COUNT_EXPLOSION)
				{
					this->ExploPara[nCntExplosion].nPatternX = 0;
					this->ExploPara[nCntExplosion].nPatternY++;
				}
				else
				{
					// �e�N�X�`�����W�̐ݒ�
					this->vtx.UV2D(nCntExplosion,
						float(this->ExploPara[nCntExplosion].nPatternX*TEXTURE_PATTERN_X_UV_EXPLOSION),
						float(this->ExploPara[nCntExplosion].nPatternY*TEXTURE_PATTERN_Y_UV_EXPLOSION));
				}
			}

			// ���_�T�C�Y�̐ݒ�
			this->ExploPara[nCntExplosion].fSizeX += EXPLOSION_VERTEX_ADD_SIZE;
			this->ExploPara[nCntExplosion].fSizeY += EXPLOSION_VERTEX_ADD_SIZE;
			this->vtx.Vertex2D(nCntExplosion,
				this->ExploPara[nCntExplosion].fSizeX, 
				this->ExploPara[nCntExplosion].fSizeY);

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

	for (int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this->iUseType[nCntExplosion].Use();
		if (use)
		{
			//--------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[nCntExplosion].Pos();
			D3DXVECTOR3 scl = this->Transform[nCntExplosion].Scl();
			D3DXCOLOR col = this->vtx.GetColor2D(nCntExplosion);
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

				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
				pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_3D));

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, this->tex.Texture());

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntExplosion * 4), POLYGON_2D_NUM);

				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
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

	for(int nCntExplosion = 0; nCntExplosion < OBJECT_EXPLOSION_MAX; nCntExplosion++)
	{
		bool use = this->iUseType[nCntExplosion].Use();
		if (use!=true)
		{
			this->Transform[nCntExplosion].Pos(pos);
			this->Transform[nCntExplosion].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[nCntExplosion].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this->vtx.Color2D(nCntExplosion,col);
			this->iUseType[nCntExplosion].Use(YesUse);
			this->ExploPara[nCntExplosion].fSizeX = fSizeX;
			this->ExploPara[nCntExplosion].fSizeY = fSizeY;
			this->ExploPara[nCntExplosion].nCounter = 0;
			this->ExploPara[nCntExplosion].nPatternX = 0;
			this->ExploPara[nCntExplosion].nPatternY = 0;
			this->ExploPara[nCntExplosion].eType = eType;

			// ���_���W�̐ݒ�
			this->vtx.Vertex2D(nCntExplosion, fSizeX, fSizeY);

			// �e�N�X�`�����W�̐ݒ�
			this->vtx.UV2D(nCntExplosion, 0,0);

			nIdxExplosion = nCntExplosion;

			break;
		}
	}

	return nIdxExplosion;
}
