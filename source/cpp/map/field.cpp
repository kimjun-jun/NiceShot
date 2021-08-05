/**
* @file field.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/field.h"
#include "../../h/object/item.h"
#include "../../h/object/bullet/bullet.h"
#include "../../h/other/sound.h"
#include "../../h/effect/explosion.h"
#include "../../h/object/player.h"
#include "../../h/library.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME				"../data/TEXTURE/map/white.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
//#define	TEXTURE_NOISE					"../data/TEXTURE/noise.bmp"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define SPHERELINEAR_LEN				(10.0f)							//!< ���ʐ��`��ԂŎg���A�v���C���[�̍��W��3�p�|���S���ӂ܂ł̋���(�v���C���[�̍��W�����̒l���ӂɋ߂���΋��ʐ��`��ԊJ�n����)

#define FIELD_Y_HIGH					(120.0f)						//!< �t�B�[���h�̍����ŐF��ς���B����
#define FIELD_Y_LOW						(20.0f)							//!< �t�B�[���h�̍����ŐF��ς���B�Ⴂ

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4�J���[
};

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
FIELD::FIELD(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_FILENAME);


	// �u���b�N���̐ݒ�
	this->FieldPara.nNumBlockXField = 32;
	this->FieldPara.nNumBlockZField = 32;

	// ���_���̐ݒ�@���_���u���b�N�̐��{�P���p��
	this->Attribute.NumVertex((this->FieldPara.nNumBlockXField + 1) * (this->FieldPara.nNumBlockZField + 1));

	// �C���f�b�N�X���̐ݒ�@TriangleList�`��
	this->Attribute.NumVertexIndex((this->FieldPara.nNumBlockXField + 1) * 2 * this->FieldPara.nNumBlockZField + (this->FieldPara.nNumBlockZField - 1) * 2);

	// �|���S�����̐ݒ�
	this->Attribute.NumPolygon(this->FieldPara.nNumBlockXField * this->FieldPara.nNumBlockZField * 2 + (this->FieldPara.nNumBlockZField - 1) * 4);

	// �u���b�N�T�C�Y�̐ݒ�
	this->FieldPara.fBlockSizeXField = 250.0f;
	this->FieldPara.fBlockSizeZField = 250.0f;

	//�@�ӃT�C�Y�̐ݒ�
	this->FieldPara.fSideSizeXMax = this->FieldPara.nNumBlockXField * this->FieldPara.fBlockSizeXField;
	this->FieldPara.fSideSizeZMax = this->FieldPara.nNumBlockZField * this->FieldPara.fBlockSizeZField;
	this->FieldPara.fSideSizeXHalf = this->FieldPara.fSideSizeXMax / 2.0f;
	this->FieldPara.fSideSizeZHalf = this->FieldPara.fSideSizeZMax / 2.0f;
	this->FieldPara.fSideSizeXQuarter = this->FieldPara.fSideSizeXHalf / 2.0f;
	this->FieldPara.fSideSizeZQuarter = this->FieldPara.fSideSizeZHalf / 2.0f;
	this->FieldPara.fSideSizeXEighth = this->FieldPara.fSideSizeXQuarter / 2.0f;
	this->FieldPara.fSideSizeZEighth = this->FieldPara.fSideSizeZQuarter / 2.0f;



	for (int CntField = 0; CntField < FIELD_VTX_MAX; CntField++)
	{
		//���_�쐬
		this->vtx[CntField].MakeVertex3D(this->Attribute.NumVertex(), FVF_VERTEX_3D);
		this->vtx[CntField].MakeIdxVertex(sizeof(WORD) *this->Attribute.NumVertex());
	}








	//�W��������
	this->FieldPara.InterPolationFieldSignal = true;
	this->FieldPara.InterPolationFieldSignalEnd = false;
	this->FieldPara.InterPolationFieldType = -1;
	this->FieldPara.InterPolationFieldPlayerNum = -1;


	//���_�ݒ�
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
FIELD::~FIELD(void)
{
	//�e�N�X�`�����
	this->tex.~TEXTURE();

	for (int CntField = 0; CntField < FIELD_VTX_MAX; CntField++)
	{
		//���_���
		this->vtx[CntField].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void FIELD::Init(void)
{
	this->FieldPara.InterPolationFieldSignal = true;
	this->FieldPara.InterPolationFieldSignalEnd = false;
	this->FieldPara.InterPolationFieldType = -1;
	this->FieldPara.InterPolationFieldPlayerNum = -1;
	this->FieldPara.TikeiSeed = 0;
	this->FieldPara.OldTikeiSeed = 0;
	this->FieldPara.ItemGetPlayerNum = -1;
	this->FieldPara.NetTikei = false;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//�����Z�b�g
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
		VERTEX_3D *pVtxE;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		//���_�o�b�t�@�̃Z�b�g
		LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
		VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();
		LPDIRECT3DVERTEXBUFFER9 VtxBuffS;
		VtxBuffS = this->vtx[FIELD_VTX_START].VtxBuff();
		LPDIRECT3DVERTEXBUFFER9 VtxBuffE;
		VtxBuffE = this->vtx[FIELD_VTX_END].VtxBuff();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);
		VtxBuffS->Lock(0, 0, (void**)&pVtxS, 0);
		VtxBuffE->Lock(0, 0, (void**)&pVtxE, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�@�`��p
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeXField + nCntVtxX * this->FieldPara.fBlockSizeXField;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.z = (this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeZField - nCntVtxZ * this->FieldPara.fBlockSizeZField;

				// ���_���W�̐ݒ�@��ԗp
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeXField + nCntVtxX * this->FieldPara.fBlockSizeXField;
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.z = (this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeZField - nCntVtxZ * this->FieldPara.fBlockSizeZField;
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeXField + nCntVtxX * this->FieldPara.fBlockSizeXField;
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.z = (this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeZField - nCntVtxZ * this->FieldPara.fBlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		VtxBuffS->Unlock();
		VtxBuffD->Unlock();
		VtxBuffE->Unlock();

		//�Z�b�g�A�b�v�����f�[�^�𔽉f
		this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
		this->vtx[FIELD_VTX_START].VtxBuff(VtxBuffS);
		this->vtx[FIELD_VTX_END].VtxBuff(VtxBuffE);
	}

	//�����Z�b�g
	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		//���_�C���f�b�N�X�̃Z�b�g
		LPDIRECT3DINDEXBUFFER9	IdxBuff;
		IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
		WORD *pIdx;
		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < this->FieldPara.nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (this->FieldPara.nNumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + this->FieldPara.nNumBlockZField;
				nCntIdx++;
			}
		}
		// �C���f�b�N�X�f�[�^���A�����b�N����
		IdxBuff->Unlock();

		//�Z�b�g�A�b�v�����f�[�^�𔽉f
		this->vtx[FIELD_VTX_DRAW].IdxBuff(IdxBuff);
	}

	//�n�`�����@Y�l�ƃJ���[�Z�b�g
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;

		//���_�o�b�t�@�̃Z�b�g
		LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
		VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();
		LPDIRECT3DVERTEXBUFFER9 VtxBuffS;
		VtxBuffS = this->vtx[FIELD_VTX_START].VtxBuff();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);
		VtxBuffS->Lock(0, 0, (void**)&pVtxS, 0);

		//���
		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				//pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = float((rand() % 200));
				pVtx[(rand() % this->Attribute.NumVertex())].vtx.y = 0.0f;
			}
		}

		//�k�ރ|���S���̍��W�𒲐�
		//SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//�㑤
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
					//pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				}
				//����
				else
				{
					float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 20.0f;
				}

				// ���ˌ��̐ݒ�
				if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
				}
				else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//��
				}
				else
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//��
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		VtxBuffS->Unlock();
		VtxBuffD->Unlock();

		//�Z�b�g�A�b�v�����f�[�^�𔽉f
		this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
		this->vtx[FIELD_VTX_START].VtxBuff(VtxBuffS);
	}

	//�����蔻�荂�����p�B�ꉞ�e�q�֌W���o�^�@���g�p
	//InitCntPartition();

}

//=============================================================================
// �X�V����
//=============================================================================
void FIELD::Update(PLAYER *player,ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow)
{
	this->FieldPara.OldTikeiSeed = this->FieldPara.TikeiSeed;

	//�A�C�e�����E����InterPolationFieldType���ς��B���̃X�C�b�`��SetFieldType�֐��Ō��-1������
	switch (this->FieldPara.InterPolationFieldType)
	{
		//�n�`�ό`����Ƃ��͂܂��n�`�N���A
		this->vtx[FIELD_VTX_DRAW].VtxBuff(this->ClearField(this->vtx[FIELD_VTX_DRAW].VtxBuff()));
		//�n�`�̎�ނɂ���ăA���S���Y���𕪂���
	case FIELD_TYPE_BOKOBOKO:
		this->vtx[FIELD_VTX_END].VtxBuff(this->SetFieldType01(this->vtx[FIELD_VTX_END].VtxBuff()));
		break;
	case FIELD_TYPE_NADARAKA:
		this->vtx[FIELD_VTX_END].VtxBuff(this->SetFieldType02(this->vtx[FIELD_VTX_END].VtxBuff()));
		break;
	case FIELD_TYPE_PLAYERADVANTAGE:
		this->vtx[FIELD_VTX_END].VtxBuff(this->SetFieldType03(&player[0], this->vtx[FIELD_VTX_END].VtxBuff(), this->vtx[FIELD_VTX_DRAW].IdxBuff()));
		break;

		//������Ƀt���O�Z�b�g
		this->FieldPara.InterPolationFieldType = -1;
		this->FieldPara.InterPolationFieldSignal = false;
	}

	//SetFieldType��InterPolationFieldSignal��false�ɂ��Ă���BInterPolationField()�̕Ԃ�l�Œn�`�̕�Ԃ��I�����������
	if (this->FieldPara.InterPolationFieldSignal == false)
	{
		this->FieldPara.InterPolationFieldSignal = this->InterPolationField();
	}

	this->FieldHitCheck(player,item,bullet,explosion,shadow);
}

//=============================================================================
// �`�揈��
//=============================================================================
void FIELD::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate,mtxWorld;
	D3DXVECTOR3 pos = this->Transform.Pos();
	D3DXVECTOR3 rot = this->Transform.Rot();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, this->vtx[FIELD_VTX_DRAW].VtxBuff(), 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(this->vtx[FIELD_VTX_DRAW].IdxBuff());

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this->tex.Texture());

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this->Attribute.NumVertex(), 0, this->Attribute.NumPolygon());
}

//=============================================================================
// �n�`�Ƃ̓����蔻��
//=============================================================================
void FIELD::FieldHitCheck(PLAYER *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow)
{
	//�v���C���[�ƒn�ʂ̓����蔻��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//-------------------�I�u�W�F�N�g�l�ǂݍ���
		D3DXVECTOR3 rayS = player->modelDraw[PLAYER_PARTS_TYPE_HOUDAI].Transform[CntPlayer].Pos();
		D3DXVECTOR3 rayG = rayS;
		D3DXVECTOR3 ReturnPos = rayS;
		D3DXVECTOR3 FieldNor = player->PostureVec[CntPlayer].FNVecFunc();
		rayS.y += 1000.0f;
		rayG.y -= 1000.0f;

		//����
		this->FieldHitGetSphereVec(rayS, rayG, &FieldNor, &ReturnPos.y);

		//-------------------�I�u�W�F�N�g�l��������
		player->modelDraw[PLAYER_PARTS_TYPE_HOUDAI].Transform[CntPlayer].Pos(ReturnPos);
		player->PostureVec[CntPlayer].FNVecFunc(FieldNor);
	}

	//�A�C�e���ƒn�ʂ̓����蔻��
	//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		bool use = item->iUseType[CntItem].Use();
		if (use == true)
		{
			//-------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 rayS = item->Transform[CntItem].Pos();
			D3DXVECTOR3 rayG = rayS;
			D3DXVECTOR3 ReturnPos = rayS;
			D3DXVECTOR3 FieldNor = item->PostureVec[CntItem].FNVecFunc();
			if (item->ItemParaAll[CntItem].CollisionFieldEnd != true || this->FieldPara.InterPolationFieldSignal == false)
			{
				//���C����
				rayS.y += 5.0f;
				rayG.y -= 5.0f;

				//����
				item->ItemParaAll[CntItem].CollisionFieldEnd = this->FieldHitItem(rayS, rayG, &FieldNor, &ReturnPos.y);
			}
			else if (this->FieldPara.InterPolationFieldSignal == true && item->ItemParaAll[CntItem].CollisionFieldEnd == true)
			{
				//���C����
				rayS.y -= 1000.0f;
				rayG.y += 1000.0f;

				//����
				this->FieldHit(rayS, rayG, &FieldNor, &ReturnPos.y);
			}
			//-------------------�I�u�W�F�N�g�l��������
			item->Transform[CntItem].Pos(ReturnPos);
			item->PostureVec[CntItem].FNVecFunc(FieldNor);
		}
	}

	//�o���b�g�ƒn�ʂ̓����蔻��
	for (int Cntbullet = 0; Cntbullet < OBJECT_BULLET_MAX; Cntbullet++)
	{
		bool buse = bullet->iUseType[Cntbullet].Use();
		if (buse == true)
		{
			D3DXVECTOR3 kari = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//-------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 rayS = bullet->Transform[Cntbullet].Pos();
			D3DXVECTOR3 rayG = rayS;
			rayS.y += 1000.0f;
			rayG.y -= 1000.0f;

			//����
			this->FieldHit(rayS, rayG, &kari, &bullet->BulletPara[Cntbullet].FieldPosY);

			//�n�`�Ƃ̓����蔻����ɂ�(����)���邽�߂�bullet[Cntbullet].FieldPosY-3.0f
			D3DXVECTOR3 bpos = bullet->Transform[Cntbullet].Pos();
			if (bullet->BulletPara[Cntbullet].FieldPosY - 3.0f > bpos.y)
			{
				// �����̐���
				D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bullet->BulletPara[Cntbullet].FieldPosY, bpos.z);
				explosion->SetInstance(ExploPos, 40.0f, 40.0f, eEXPLOSION_TYPE(bullet->BulletPara[Cntbullet].UsePlayerType), PLAYER_COLOR[bullet->BulletPara[Cntbullet].UsePlayerType]);
				// �o���b�g�j��
				bullet->ReleaseInstance(Cntbullet, shadow);
				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_damage);
			}
		}
	}
}

//=============================================================================
// �n�`�ό`�^�C�v
//=============================================================================
void FIELD::SetFieldInterPolationFieldType(int type,int CntPlayer,ITEM *item)
{
	//�t���O�Z�b�g
	this->FieldPara.InterPolationFieldType = type;
	this->FieldPara.InterPolationFieldPlayerNum = CntPlayer;
	//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		item->ItemParaAll[CntItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// �n�`�̃N���A
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::ClearField(LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			// ���_���W�̐ݒ�
			pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
		}
	}
	VtxBuff->Unlock();
	return VtxBuff;
}

//=============================================================================
// �n�`�̎�������01 �ڂ��ڂ����ݒn�`�@�u���b�N��32*32�@�u���b�N�T�C�Y60*60�`100*100
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::SetFieldType01(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd)
{
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	//����@������ݒ�
	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertex() / 4); nCntVtx++)
	{
		//���������߂钸�_������
		int YTXrandNum(rand() % this->Attribute.NumVertex());

		//���������ߑ�� �k�ރ|���S���������ɓ�����
		int VTXrandY(rand() % 200);
		if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
		{
			pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
		{
			pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		pVtx[YTXrandNum].vtx.y = float((VTXrandY / 1.0f));
	}

	//�k�ރ|���S���̍��W�𒲐�
	//SetDegenerationPoly();

	//���������@���_�ψʏ���
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//�k�ރ|���S���悯�Ȃ���
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

			// ���_���W�̐ݒ�
			//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
			if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
			//����
			//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
			else
			{
				float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
					pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
			}
		}
	}
	// ���_�f�[�^���A�����b�N����
	VtxBuffEnd->Unlock();
	return VtxBuffEnd;
}

//=============================================================================
// �n�`�̎�������02�@���ʂ��̂����n�`�@�u���b�N��32*32�@�u���b�N�T�C�Y60*60�`100*100
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::SetFieldType02(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd)
{
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	//����@������ݒ�
	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertex() / 4); nCntVtx++)
	{
		//���������߂钸�_������
		int YTXrandNum(rand() % this->Attribute.NumVertex());

		//���������ߑ�� �k�ރ|���S���������ɓ�����
		int VTXrandY(rand() % 200);
		if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
		{
			pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
		{
			pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		pVtx[YTXrandNum].vtx.y = float((VTXrandY / 1.0f));
	}

	//�k�ރ|���S���̍��W�𒲐�
	//SetDegenerationPoly();

	//���������@���_�ψʏ���
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//�k�ރ|���S���悯�Ȃ���
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

			// ���_���W�̐ݒ�
			//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
			//�㑤
			if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 100.0f;
			//�����@�㉺���E�̕��ϒl���Z�o
			//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
			else
			{
				float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
					pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = float(fabs(y));
			}
		}
	}
	// ���_�f�[�^���A�����b�N����
	VtxBuffEnd->Unlock();
	return VtxBuffEnd;
}

//=============================================================================
// �n�`�̎�������03�@�擾�v���C���[���L���ɂȂ�n�`(����v���C���[�t�߂�~�n)�@�@�u���b�N��32*32�@�u���b�N�T�C�Y250*250
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::SetFieldType03(PLAYER *player, LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd, LPDIRECT3DINDEXBUFFER9 IdxBuff)
{
	//���������߂�
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	//����@������ݒ�
	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertex()); nCntVtx++)
	{
		//���������߂钸�_������@�����Ĉ����𒆓r���[�Ȑ��ɂ��ďo�͂ɕ�����������
		int YTXrandNum = MyRandFunc(&this->FieldPara.TikeiSeed, this->Attribute.NumVertex() - 103);
		//���������ߑ��
		int VTXrandY = MyRandFunc(&this->FieldPara.TikeiSeed, 200);

		VTXrandY += 20;//�I�t�Z�b�g
		if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
		{
			pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float(VTXrandY);
			continue;
		}
		else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
		{
			pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float(VTXrandY);
			continue;
		}
		pVtx[YTXrandNum].vtx.y = float(VTXrandY);
	}

	//�k�ރ|���S���̍��W�𒲐�
	//SetDegenerationPoly();

	//�אڒ��_�̍������ϒl��(�_�C�A�����h�X�N�G�A�A�t���N�^�����Q�l)
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//�k�ރ|���S���悯�Ȃ���
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
			// ���_���W�̐ݒ�
			//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
			if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 200.0f;
			//�����@�㉺���E�̕��ϒl���Z�o
			//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
			else
			{
				float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
					pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4.0f;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = fabsf(y);
			}
		}
	}
	// ���_�f�[�^���A�����b�N����
	VtxBuffEnd->Unlock();

	//����v���C���[�t�߂̒n�`��~�n��
	VERTEX_3D *pVtxP;
	WORD *pIdx;
	VtxBuffEnd->Lock(0, 0, (void**)&pVtxP, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);		//���_�C���f�b�N�X�o�b�t�@�͋��ʂ�1�����Ȃ��̂�Draw�̂���g���Ă���

	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (CntPlayer != this->FieldPara.InterPolationFieldPlayerNum)
		{
			//�ŏ��P��(����������)�̍��W
			float HitPosUp;
			float HitPosDown;
			float HitPosLeft;
			float HitPosRight;
			HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

			//�v���C���[�̏���Ă���G���A�����B4���؂Ŕ͈͂��i��B
			//-------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = player->modelDraw[PLAYER_PARTS_TYPE_HOUDAI].Transform[CntPlayer].Pos();
			//�����������蔻��
			SpeedUpFieldHitPoly(pos, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
				this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth,
				this->FieldPara.fSideSizeXEighth / 2, this->FieldPara.fSideSizeZEighth / 2);

			for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
			{
				//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
				if (nCntVtx == this->Attribute.NumVertexIndex() - 2) break;
				else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])continue;
				else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])continue;
				//���������蔻��p�|���S���̍��W���Ȃ瓖���蔻����s�@X�`�F�b�N����Z�`�F�b�N�B�Ƃ���true���Ɣ���
				if (pVtxP[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtxP[pIdx[nCntVtx]].vtx.x <= HitPosRight)
				{
					if (pVtxP[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtxP[pIdx[nCntVtx]].vtx.z >= HitPosDown)
					{
						// ���_���W�̐ݒ�
						pVtxP[pIdx[nCntVtx]].vtx.y = 20.0f;
					}
				}
			}
		}
	}

	// ���_�f�[�^���A�����b�N����
	VtxBuffEnd->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	IdxBuff->Unlock();
	return VtxBuffEnd;
}

//=============================================================================
// �n�`�̕ό`�����@�������ꂽ�t�B�[���h�ɕό`����
//=============================================================================
bool FIELD::InterPolationField(void)
{
	VERTEX_3D *pVtx;
	VERTEX_3D *pVtxS;
	VERTEX_3D *pVtxE;

	//���_�o�b�t�@�̃Z�b�g
	LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
	VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DVERTEXBUFFER9 VtxBuffS;
	VtxBuffS = this->vtx[FIELD_VTX_START].VtxBuff();
	LPDIRECT3DVERTEXBUFFER9 VtxBuffE;
	VtxBuffE = this->vtx[FIELD_VTX_END].VtxBuff();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);
	VtxBuffS->Lock(0, 0, (void**)&pVtxS, 0);
	VtxBuffE->Lock(0, 0, (void**)&pVtxE, 0);

	float dt = 1.0f / 60.0f;	// 1�t���[���Ői�߂鎞��
	this->FieldPara.time += dt;

	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//�k�ރ|���S���悯�Ȃ���
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx) continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

			//��ԊJ�n
			pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx = MyInterpolation(
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx,
				pVtxE[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx,
				0.0, 1.0f, dt, this->FieldPara.time, LerpEaseInEaseOut);

			// ���ˌ��̐ݒ�
			if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y >= FIELD_Y_HIGH)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
			}
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y < FIELD_Y_HIGH && pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y > FIELD_Y_LOW)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//��
			}
			else
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//��
			}
		}
	}
	if (this->FieldPara.time >= 1.0f)
	{
		//�ό`���I�������猻�ݕ`�悵�Ă���n�`��pD3DVtxBuffFieldStart�֕ۑ��B���̕ό`�J�n���̒n�`�ɂ���
		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx = pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx;
			}
		}
		this->FieldPara.time = 0.0f;
		this->FieldPara.InterPolationFieldSignalEnd = true;
	}
	// ���_�f�[�^���A�����b�N����
	VtxBuffS->Unlock();
	VtxBuffD->Unlock();
	VtxBuffE->Unlock();

	//�Z�b�g�A�b�v�����f�[�^�𔽉f
	this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
	this->vtx[FIELD_VTX_START].VtxBuff(VtxBuffS);
	this->vtx[FIELD_VTX_END].VtxBuff(VtxBuffE);

	if (this->FieldPara.InterPolationFieldSignalEnd == true)
	{
		this->FieldPara.InterPolationFieldSignalEnd = false;
		StopSound(SOUND_LABEL_SE_quake);
		this->FieldPara.NetTikei = false;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// �k�ރ|���S���̍��W���Čv�Z����֐��@�n�`�ό`��A�k�ރ|���S���̍��W��ݒ肷��
//=============================================================================
void FIELD::SetDegenerationPoly(void)
{
	VERTEX_3D *pVtx;
	//���_�o�b�t�@�̃Z�b�g
	LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
	VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//�k�ރ|���S����
			if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx = pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx;
			}
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx = pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx;
			}
		}
	}
	// ���_�f�[�^���A�����b�N����
	VtxBuffD->Unlock();
	//�Z�b�g�A�b�v�����f�[�^�𔽉f
	this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
}

//=============================================================================
// �n�`�Ƃ̓����蔻�� rayS,rayG,vtxHOUSEN,posy �Ԃ�lvoid �o���b�g�̓����蔻��Ŏg�p
//=============================================================================
void FIELD::FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//�����蔻�荂�����p�̍��W�l��
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//�����������B4���؂œ����蔻�������͈͂��i��B
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight, 
		this->FieldPara.fSideSizeXHalf, this->FieldPara.fSideSizeZHalf,
		this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DINDEXBUFFER9 IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
	{
		//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
		if (nCntVtx == this->Attribute.NumVertexIndex() - 2)break;
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])continue;
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])continue;
		//���������蔻��p�|���S���̍��W���Ȃ瓖���蔻����s�@X�`�F�b�N����Z�`�F�b�N�B�Ƃ���true���Ɣ���
		else if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
		{
			if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
			{
				// ���_���W�̐ݒ�
				D3DXVECTOR3 vtx[3];
				D3DXVECTOR3 vtxHen[2];
				D3DXVECTOR3 NaibunPos;

				//-----------------------�I�u�W�F�N�g�ǂݍ���
				D3DXVECTOR3 pos = this->Transform.Pos();
				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

				//���C�L���X�g�Œ��ׂ�Btrue�ō��Wy��ύX
				if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
				{
					vtxHen[0] = vtx[1] - vtx[0];
					vtxHen[1] = vtx[2] - vtx[0];

					//�v���C��̌X����(�n�`�̖@��)
					D3DXVECTOR3 vtxHOUSEN;
					D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
					if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
					*vtxNor = vtxHOUSEN;
					*posY = NaibunPos.y + 1.0f;
					break;
				}
			}
		}
	}

	// ���_�f�[�^���A�����b�N����
	VtxBuffEnd->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	IdxBuff->Unlock();

}

//=============================================================================
// �n�`�Ƃ̓����蔻�� rayS,rayG,vtxHOUSEN,posy �Ԃ�lvoid�@�v���C���[�̓����蔻��Ŏg�p�B���ʕ�ԂŎg�p���邽�߁B
//=============================================================================
void FIELD::FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//�����蔻�荂�����p�̍��W�l��
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//�����������B4���؂œ����蔻�������͈͂��i��B
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
		this->FieldPara.fSideSizeXHalf, this->FieldPara.fSideSizeZHalf,
		this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DINDEXBUFFER9 IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
	{
		//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
		if (nCntVtx == this->Attribute.NumVertexIndex() - 2)break;
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])continue;
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])continue;
		//���������蔻��p�|���S���̍��W���Ȃ瓖���蔻����s�@X�`�F�b�N����Z�`�F�b�N�B�Ƃ���true���Ɣ���
		else if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
		{
			if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
			{
				// ���_���W�̐ݒ�
				D3DXVECTOR3 vtx[3];
				D3DXVECTOR3 vtxHen[2];
				D3DXVECTOR3 NaibunPos;

				//-----------------------�I�u�W�F�N�g�ǂݍ���
				D3DXVECTOR3 pos = this->Transform.Pos();
				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

				//���C�L���X�g�Œ��ׂ�Btrue�ō��Wy��ύX
				if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
				{
					//�v���C���[�̌X����(�v���C���[������Ă���n�`�̖@��)
					vtxHen[0] = vtx[1] - vtx[0];
					vtxHen[1] = vtx[2] - vtx[0];
					D3DXVECTOR3 vtxHOUSEN;
					D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
					if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
					*vtxNor = vtxHOUSEN;
					*posY = NaibunPos.y + 1.0f;

					//pVtx[pIdx[nCntVtx]]=���̒��_
					//����Ă�|���S�����킩�����炻������㉺���E�̃|���S���@�����v�Z����
					//�|���S���͎O�p�`�Ȃ̂ŗאڕӂ�3��(�אڃ|���S����3��)�@
					//�|���S���̍Œ[�l(�u���b�Nx,z�̐؂�Ԃ��ԍ�)��������A�Œ[�l���̃|���S���@���̓v���C���[������Ă�|���S���@���Ɠ���
					//��@-1
					//���@+1
					//���E�@�������W�ɂ��钸�_

					//���ǃ|�C���g
					//���̒��_z���W�Ɣ�ׁA����̒��_z���W���傫����΍������p�O�p�`�ɏ���Ă���
					if (pVtx[pIdx[nCntVtx]].vtx.z < pVtx[pIdx[nCntVtx + 1]].vtx.z)
					{
						//���ʂȒ��_
						/*
						if (nCntVtx == this[0].model.nNumVertexIndex - this->FieldPara.nNumBlockZField * 2)
						{
							//���_���E�ӂɋ߂Â��Ƌ��ʐ��`��ԂŎp���x�N�g���𐧌�
							float MinDistance = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							if (SPHERELINEAR_LEN > MinDistance)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}

						//�@���@�������p�O�p�`�ō��̒��_�ԍ���this->FieldPara.nNumBlockZField�ȉ��Ȃ�΁A�n�ʂ̍ō��[�ɂ���
						//���ʂȒ��_
						//else if (nCntVtx >= this->FieldPara.nNumBlockZField)
						//{
						//	int a = 0;
						//}
						//�@��@�������p�O�p�`�ō��̒��_���W�ƈ�O�̒��_���W�������������́A��O�̒��_���W�Ɠ�O�̒��_���W�������Ȃ�΁A�n�ʂ̍ŏ�[�ɂ���
						//���ʂȒ��_
						//���ʂȒ��_
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx - 1]].vtx || pVtx[pIdx[nCntVtx - 1]].vtx == pVtx[pIdx[nCntVtx - 2]].vtx)
						{
							int a = 0;

						}
						//�@���@�������p�O�p�`�ō��̒��_���W�ƈ���̒��_���W�������������́A����̒��_���W�Ɠ���̒��_���W�������Ȃ�΁A�n�ʂ̍ŉ��[�ɂ���(�k�ރ|���S���Ȃ̂�true�ɂȂ�Ȃ��͂�)
						//���ʂȒ��_
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx + 1]].vtx || pVtx[pIdx[nCntVtx + 1]].vtx == pVtx[pIdx[nCntVtx + 2]].vtx)
						{
							int a = 0;
						}
						*/

						//�ʏ�̒��_
						//else
						{
							//���_���E�ӂɋ߂Â��Ƌ��ʐ��`��ԂŎp���x�N�g���𐧌�
							//�v���C���[�̍��W�������Ă���|���S���̊e3�ӂ܂ł̍ŒZ����MinDistance���v�Z����
							float MinDistanceRightUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							float MinDistanceDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							float MinDistanceLeft = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
							//�v���C���[���W����2�ӂ܂ł̋�����SPHERELINEAR_LEN���Z���ƁA���̐�ɑ�����̒n�`�̖@�����ϒl���Ƃ�A���̒l�ŕ�Ԃ�����
							//�v���C���[��3�p�|���S�����E���t�߂ɂ���Ƃ��ɂ����ꂩ��if()�ɓ���Bif(&&)�̏������������̂̓|���S�����E����2�ӂɑ΂��ċ߂����Ɏ��s�����
							if (SPHERELINEAR_LEN > MinDistanceRightUp && SPHERELINEAR_LEN > MinDistanceDown)
							{
								// 1�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceRightUp
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceDown
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//��̖@�����ϒl�����߂�
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//�������Ă����̋����̕��ς����߂�
								float MinRtionDistance = (MinDistanceRightUp + MinDistanceDown) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
							else if (SPHERELINEAR_LEN > MinDistanceRightUp && SPHERELINEAR_LEN > MinDistanceLeft)
							{
								// 1�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceRightUp
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeft
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//��̖@�����ϒl�����߂�
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//�������Ă����̋����̕��ς����߂�
								float MinRtionDistance = (MinDistanceRightUp + MinDistanceLeft) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}
							else if (SPHERELINEAR_LEN > MinDistanceDown && SPHERELINEAR_LEN > MinDistanceLeft)
							{
								// 1�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceDown
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeft
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//��̖@�����ϒl�����߂�
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//�������Ă����̋����̕��ς����߂�
								float MinRtionDistance = (MinDistanceDown + MinDistanceLeft) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}
							//1�ӂ��������Ă��Ȃ�
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceRightUp)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceRightUp) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//
							else if (SPHERELINEAR_LEN > MinDistanceDown)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2) + 6]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceDown) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceLeft)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceLeft) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}
					}
					//����ȊO�͉E�㒼�p�O�p�`�ɏ���Ă���
					else
					{
						//���ʂȒ��_�ԍ�
						/*
						if (nCntVtx == this[0].model.nNumVertexIndex - this->FieldPara.nNumBlockZField * 2)
						{
							//���_���E�ӂɋ߂Â��Ƌ��ʐ��`��ԂŎp���x�N�g���𐧌�
							float MinDistance = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
							if (SPHERELINEAR_LEN > MinDistance)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx]].vtx - pVtx[pIdx[nCntVtx - 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx - 1]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}
						//�@��@�E�㒼�p�O�p�`�ō��̒��_���W�ƈ�O�̒��_���W�������������́A��O�̒��_���W�Ɠ�O�̒��_���W�������Ȃ�΁A�n�ʂ̍ŏ�[�ɂ���
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx - 1]].vtx || pVtx[pIdx[nCntVtx - 1]].vtx == pVtx[pIdx[nCntVtx - 2]].vtx)
						{
							//�k�ރ|���S��
						}
						//�@���@�E�㒼�p�O�p�`���̒��_���W�ƈ���̒��_���W�������������́A����̒��_���W�Ɠ���̒��_���W�������Ȃ�΁A�n�ʂ̍ŉ��[�ɂ���(�k�ރ|���S���Ȃ̂�true�ɂȂ�Ȃ��͂�)
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx + 1]].vtx || pVtx[pIdx[nCntVtx + 1]].vtx == pVtx[pIdx[nCntVtx + 2]].vtx)
						{

						}
						//�@�E�@�E�㒼�p�O�p�`�ō��̒��_�ԍ���this->FieldPara.nNumBlockZField�ȏ�Ȃ�΁A�n�ʂ̍ŉE�[�ɂ���
						//else if (nCntVtx >= this->FieldPara.nNumBlockZField * this->FieldPara.nNumBlockZField)
						//{

						//}
						*/

						//else
						{
							//���_���E�ӂɋ߂Â��Ƌ��ʐ��`��ԂŎp���x�N�g���𐧌�
							float MinDistanceRight = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 2]].vtx, pVtx[pIdx[nCntVtx + 3]].vtx);
							float MinDistanceUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 3]].vtx);
							float MinDistanceLeftDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							//float MinDistanceRight = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							//float MinDistanceUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							//float MinDistanceLeftDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
							//�v���C���[���W����2�ӂ܂ł̋�����SPHERELINEAR_LEN���Z���ƁA���̐�ɑ�����̒n�`�̖@�����ϒl���Ƃ�A���̒l�ŕ�Ԃ�����
							if (SPHERELINEAR_LEN > MinDistanceRight && SPHERELINEAR_LEN > MinDistanceUp)
							{
								// 1�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceRight
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceUp
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//��̖@�����ϒl�����߂�
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//�������Ă����̋����̕��ς����߂�
								float MinRtionDistance = (MinDistanceRight + MinDistanceUp) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
							else if (SPHERELINEAR_LEN > MinDistanceRight && SPHERELINEAR_LEN > MinDistanceLeftDown)
							{
								// 1�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceRight
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeftDown
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//��̖@�����ϒl�����߂�
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//�������Ă����̋����̕��ς����߂�
								float MinRtionDistance = (MinDistanceRight + MinDistanceLeftDown) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}
							else if (SPHERELINEAR_LEN > MinDistanceUp && SPHERELINEAR_LEN > MinDistanceLeftDown)
							{
								// 1�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceUp
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeftDown
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//��̖@�����ϒl�����߂�
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//�������Ă����̋����̕��ς����߂�
								float MinRtionDistance = (MinDistanceUp + MinDistanceLeftDown) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}

							//1�ӂ��������Ă��Ȃ�
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceRight)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceRight) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceUp)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceUp) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceLeftDown)
							{
								//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceLeftDown) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}
					}

					//�v���C���[�̍��W���|���S���̂Ȃ��ڕt�߂ɂ���ƁA���̃|���S���@�����m�ŋ��ʐ��`���
					//��Ԃŋ��߂����ԃx�N�g�����v���C���[�̎p���x�N�g���ɂȂ�

					break;
				}
			}
		}
	}

	// ���_�f�[�^���A�����b�N����
	VtxBuffEnd->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	IdxBuff->Unlock();

}

//=============================================================================
// �n�`�Ƃ̓����蔻�� rayS,rayG,vtxHOUSEN,posy �Ԃ�lbool �A�C�e���̓����蔻��Ŏg�p
//=============================================================================
bool FIELD::FieldHitItem(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	bool Cheack = false;
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//�����蔻�荂�����p�̍��W�l��
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//�����������B4���؂œ����蔻�������͈͂��i��B
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
		this->FieldPara.fSideSizeXHalf, this->FieldPara.fSideSizeZHalf, 
		this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DINDEXBUFFER9 IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
	{
		//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
		if (nCntVtx == this->Attribute.NumVertexIndex() - 2)
		{
			break;
		}
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])
		{
			continue;
		}
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])
		{
			continue;
		}
		else if (InrayS.y <= 400.0f)
		{
			//���������蔻��p�|���S���̍��W���Ȃ瓖���蔻����s�@X�`�F�b�N����Z�`�F�b�N�B�Ƃ���true���Ɣ���
			if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
			{
				if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
				{
					// ���_���W�̐ݒ�
					D3DXVECTOR3 vtx[3];
					D3DXVECTOR3 vtxHen[2];
					D3DXVECTOR3 NaibunPos;

					//-----------------------�I�u�W�F�N�g�ǂݍ���
					D3DXVECTOR3 pos = this->Transform.Pos();
					vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
					vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
					vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

					//���C�L���X�g�Œ��ׂ�Btrue�ō��Wy��ύX
					if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
					{
						vtxHen[0] = vtx[1] - vtx[0];
						vtxHen[1] = vtx[2] - vtx[0];

						//�v���C��̌X����(�n�`�̖@��)
						D3DXVECTOR3 vtxHOUSEN;
						D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
						if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
						*vtxNor = vtxHOUSEN;
						*posY = NaibunPos.y + 1.0f;
						Cheack = true;
					}
				}
			}
		}
	}

	// ���_�f�[�^���A�����b�N����
	VtxBuffEnd->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	IdxBuff->Unlock();
	if (Cheack == true) return true;

	return false;
}


