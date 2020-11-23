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
#define	TEXTURE_FILENAME				"../data/TEXTURE/white.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_NOISE					"../data/TEXTURE/noise.bmp"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define SPHERELINEAR_LEN				(8.0f)							//!< ���ʐ��`��ԂŎg���A�v���C���[�̍��W��3�p�|���S���ӂ܂ł̋���(�v���C���[�̍��W�����̒l���ӂɋ߂���΋��ʐ��`��ԊJ�n����)

#define FIELD_Y_HIGH					(120.0f)						//!< �t�B�[���h�̍����ŐF��ς���B����
#define FIELD_Y_LOW						(20.0f)							//!< �t�B�[���h�̍����ŐF��ς���B�Ⴂ

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4�J���[
};

int FIELD::cnt = 0;


//=============================================================================
// ����������
//=============================================================================
void FIELD::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	this[0].InterPolationFieldSignal = true;
	this[0].InterPolationFieldSignalEnd = false;

	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldPlayerNum = -1;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,								// �t�@�C���̖��O
		&this[0].model.pD3DTexture);							// �ǂݍ��ރ������[

	// �u���b�N���̐ݒ�
	this[0].nNumBlockXField = 32;
	this[0].nNumBlockZField = 32;

	// ���_���̐ݒ�
	this[0].model.nNumVertex = (nNumBlockXField + 1) * (nNumBlockZField + 1);


	// �C���f�b�N�X���̐ݒ�
	this[0].model.nNumVertexIndex = (nNumBlockXField + 1) * 2 * nNumBlockZField + (nNumBlockZField - 1) * 2;

	// �|���S�����̐ݒ�
	this[0].model.nNumPolygon = nNumBlockXField * nNumBlockZField * 2 + (nNumBlockZField - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	this[0].fBlockSizeXField = 250.0f;
	this[0].fBlockSizeZField = 250.0f;

	//�@�ӃT�C�Y�̐ݒ�
	fSideSizeXMax = nNumBlockXField * fBlockSizeXField;
	fSideSizeZMax = nNumBlockZField * fBlockSizeZField;
	fSideSizeXHalf = fSideSizeXMax / 2.0f;
	fSideSizeZHalf = fSideSizeZMax / 2.0f;
	fSideSizeXQuarter = fSideSizeXHalf / 2.0f;
	fSideSizeZQuarter = fSideSizeZHalf / 2.0f;
	fSideSizeXEighth = fSideSizeXQuarter / 2.0f;
	fSideSizeZEighth = fSideSizeZQuarter / 2.0f;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffFieldDraw,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);								// NULL�ɐݒ�
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffFieldEnd,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);								// NULL�ɐݒ�

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffFieldStart,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);							// NULL�ɐݒ�

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * this[0].model.nNumVertexIndex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this[0].model.pD3DIdxBuff,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);								// NULL�ɐݒ�

	//�`��p
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldStart->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < this[0].nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this[0].nNumBlockZField + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (this[0].nNumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (this[0].nNumBlockZField + 1) + this[0].nNumBlockZField;
				nCntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		this[0].model.pD3DIdxBuff->Unlock();
	}

	//�`��p
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		//���
		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1)/2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1)/2; nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = float((rand() % 200));
				//pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = 0.0f;
			}
		}

		//�k�ރ|���S���̍��W�𒲐�
		SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//�㑤
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
					//pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				}
				//����
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
				}

				// ���ˌ��̐ݒ�
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
				}
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//��
				}
				else
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//��
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldStart->Unlock();
	}



	//���������̃o�b�t�@�B�n�`�ύX����Ƃ��ɂ��̌`�ɕό`	F
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}
		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}


	//�����蔻�荂�����p�B�ꉞ�e�q�֌W���o�^
	InitCntPartition();

}

//=============================================================================
// �ď���������
//=============================================================================
void FIELD::Reinit(void)
{
	this[0].InterPolationFieldSignal = true;
	this[0].InterPolationFieldSignalEnd = false;
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldPlayerNum = -1;

	//--------------------------------DRAW��START�̃Z�b�g
	//�`��p
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		//�����N���A
		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx .y= 0.0f;
			}
		}

		//���
		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1) / 2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1) / 2; nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				//������������
				pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = float((rand() % 200));
			}
		}

		//�k�ރ|���S���̍��W�𒲐�
		//SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//�㑤
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
				//����
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
				}

				// ���ˌ��̐ݒ�
				if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
				}
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//��
				}
				else
				{
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//��
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldStart->Unlock();
	}
	//----------------------------END�̃Z�b�g
	//���������̃o�b�t�@�B�n�`�ύX����Ƃ��ɂ��̌`�ɕό`	F
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this[0].nNumBlockZField / 2.0f) * fBlockSizeXField + nCntVtxX * fBlockSizeXField;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.z = (this[0].nNumBlockZField / 2.0f) * fBlockSizeZField - nCntVtxZ * fBlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}
		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
}

//=============================================================================
// �I������
//=============================================================================
void FIELD::Uninit(void)
{
	this[0].pD3DVtxBuffFieldStart->Release();
	this[0].pD3DVtxBuffFieldEnd->Release();
	this[0].pD3DVtxBuffFieldDraw->Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void FIELD::Update(PLAYER_HONTAI *player,ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow)
{
	//�A�C�e�����E����InterPolationFieldType���ς��B���̃X�C�b�`��SetFieldType�֐��Ō��-1������
	switch (this[0].InterPolationFieldType)
	{
		this[0].ClearField();
	case FIELD_TYPE_BOKOBOKO:
		this[0].SetFieldType01();
		break;
	case FIELD_TYPE_NADARAKA:
		this[0].SetFieldType02();
		break;
	case FIELD_TYPE_PLAYERADVANTAGE:
		this[0].SetFieldType03(&player[0]);
		break;
	default:
		break;
	}
	//SetFieldType��InterPolationFieldSignal��false�ɂ��Ă���BInterPolationField()�̕Ԃ�l�Œn�`�̕�Ԃ��I�����������
	if (this[0].InterPolationFieldSignal == false)
	{
		this[0].InterPolationFieldSignal = this[0].InterPolationField();
	}

	//�v���C���[�ƒn�ʂ̓����蔻��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//-------------------�I�u�W�F�N�g�l�ǂݍ���
		D3DXVECTOR3 rayS = player[CntPlayer].GetPos();
		rayS.y += 1000.0f;
		D3DXVECTOR3 rayG = player[CntPlayer].GetPos();
		rayG.y -= 1000.0f;
		D3DXVECTOR3 FieldNor = player[CntPlayer].GetFieldNorVec();
		D3DXVECTOR3 ReturnPos = player[CntPlayer].GetPos();

		//����
		this[0].FieldHitGetSphereVec(rayS, rayG, &FieldNor, &ReturnPos.y);

		//-------------------�I�u�W�F�N�g�l��������
		player[CntPlayer].SetPos(ReturnPos);
		player[CntPlayer].SetFieldNorVec(FieldNor);
	}

	//�A�C�e���ƒn�ʂ̓����蔻��
	//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		bool use = item[CntItem].GetUse();
		if (use == true)
		{
			if (item[CntItem].CollisionFieldEnd != true || InterPolationFieldSignal == false)
			{
				//-------------------�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 rayS = item[CntItem].GetPos();
				rayS.y += 5.0f;
				D3DXVECTOR3 rayG = item[CntItem].GetPos();
				rayG.y -= 5.0f;
				D3DXVECTOR3 FieldNor = item[CntItem].GetFieldNorVec();
				D3DXVECTOR3 ReturnPos = item[CntItem].GetPos();

				//����
				item[CntItem].CollisionFieldEnd = this[0].FieldHitItem(rayS, rayG, &FieldNor, &ReturnPos.y);

				//-------------------�I�u�W�F�N�g�l��������
				item[CntItem].SetPos(ReturnPos);
				item[CntItem].SetFieldNorVec(FieldNor);

			}
			else if (InterPolationFieldSignal == true && item[CntItem].CollisionFieldEnd == true)
			{
				//-------------------�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 rayS = item[CntItem].GetPos();
				rayS.y -= 1000.0f;
				D3DXVECTOR3 rayG = item[CntItem].GetPos();
				rayG.y += 1000.0f;
				D3DXVECTOR3 FieldNor = item[CntItem].GetFieldNorVec();
				D3DXVECTOR3 ReturnPos = item[CntItem].GetPos();

				//����
				this[0].FieldHit(rayS, rayG, &FieldNor, &ReturnPos.y);

				//-------------------�I�u�W�F�N�g�l��������
				item[CntItem].SetPos(ReturnPos);
				item[CntItem].SetFieldNorVec(FieldNor);
			}
		}
	}

	//�o���b�g�ƒn�ʂ̓����蔻��
	for (int Cntbullet = 0; Cntbullet < OBJECT_BULLET_MAX; Cntbullet++)
	{
		bool buse = bullet[Cntbullet].GetUse();
		if (buse == true)
		{
			D3DXVECTOR3 kari = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//-------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 rayS = bullet[Cntbullet].GetPos();
			rayS.y += 1000.0f;
			D3DXVECTOR3 rayG = bullet[Cntbullet].GetPos();
			rayG.y -= 1000.0f;

			//����
			this[0].FieldHit(rayS, rayG, &kari, &bullet[Cntbullet].FieldPosY);

			//�n�`�Ƃ̓����蔻����ɂ�(����)���邽�߂�bullet[Cntbullet].FieldPosY-3.0f
			D3DXVECTOR3 bpos = bullet[Cntbullet].GetPos();
			if (bullet[Cntbullet].FieldPosY - 3.0f > bpos.y)
			{
				// �����̐���
				D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bullet[Cntbullet].FieldPosY, bpos.z);
				explosion->SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[bullet[Cntbullet].UsePlayerType]);
				// �o���b�g�j��
				bullet[0].ReleaseBullet(Cntbullet, shadow);
				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_damage);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void FIELD::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate,mtxWorld;
	mtxWorld = this[0].GetMatrix();
	D3DXVECTOR3 pos = this[0].GetPos();
	D3DXVECTOR3 rot = this[0].GetRot();

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
	pDevice->SetStreamSource(0, pD3DVtxBuffFieldDraw, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(this[0].model.pD3DIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this[0].model.pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this[0].model.nNumVertex, 0, this[0].model.nNumPolygon);
}

//=============================================================================
// �n�`�ό`�^�C�v
//=============================================================================
void FIELD::SetFieldInterPolationFieldType(int type,int CntPlayer,ITEM *item)
{
	//�t���O�Z�b�g
	this[0].InterPolationFieldType = type;
	this[0].InterPolationFieldPlayerNum = CntPlayer;
	//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		item[CntItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// �n�`�̃N���A
//=============================================================================
void FIELD::ClearField(void)
{
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
		{
			// ���_���W�̐ݒ�
			pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
		}
	}
	// ���_�f�[�^���A�����b�N����
	this[0].pD3DVtxBuffFieldEnd->Unlock();
}

//=============================================================================
// �n�`�̎�������01 �ڂ��ڂ����ݒn�`�@�u���b�N��32*32�@�u���b�N�T�C�Y60*60�`100*100
//=============================================================================
void FIELD::SetFieldType01(void)
{
	{
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//����@������ݒ�
		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertex/4); nCntVtx++)
		{
				//���������߂钸�_������
				int YTXrandNum(rand() % this[0].model.nNumVertex);

				//���������ߑ�� �k�ރ|���S���������ɓ�����
				int VTXrandY(rand() % 200);
				if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
				{
					pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float((VTXrandY / 1.0f));
					continue;
				}
				else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
				{
					pVtx[YTXrandNum+1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float((VTXrandY / 1.0f));
					continue;
				}
				pVtx[YTXrandNum].vtx.y = float((VTXrandY / 1.0f));
		}

		//�k�ރ|���S���̍��W�𒲐�
		//SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
				//����
				//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
				}
			}
		}
		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldSignal = false;
}

//=============================================================================
// �n�`�̎�������02�@���ʂ��̂����n�`�@�u���b�N��32*32�@�u���b�N�T�C�Y60*60�`100*100
//=============================================================================
void FIELD::SetFieldType02(void)
{
	{
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//����@������ݒ�
		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertex / 4); nCntVtx++)
		{
			//���������߂钸�_������
			int YTXrandNum(rand() % this[0].model.nNumVertex);

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

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
				//�㑤
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 100.0f;
				//�����@�㉺���E�̕��ϒl���Z�o
				//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = float(fabs(y));
				}
			}
		}
		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldSignal = false;
}

//=============================================================================
// �n�`�̎�������03�@�擾�v���C���[���L���ɂȂ�n�`(����v���C���[�t�߂�~�n)�@�@�u���b�N��32*32�@�u���b�N�T�C�Y250*250
//=============================================================================
void FIELD::SetFieldType03(PLAYER_HONTAI *player)
{
	//���������߂�
	{
		VERTEX_3D *pVtx;
		WORD *pIdx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		//����@������ݒ�
		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertex); nCntVtx++)
		{
			//���������߂钸�_������
			int YTXrandNum(rand() % this[0].model.nNumVertex);

			//���������ߑ��
			int VTXrandY(rand() % 200);
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

		//for (int nCntVtx = 0; nCntVtx < this[0].model.nNumVertexIndex/2; nCntVtx++)
		//{
		//	//���������߂钸�_������
		//	int YTXrandNum(rand() % this[0].model.nNumVertex);
		//	//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
		//	if (YTXrandNum == this[0].model.nNumVertexIndex - 2) break;
		//	else if (pIdx[YTXrandNum] == pIdx[YTXrandNum + 1]) continue;
		//	else if (pIdx[YTXrandNum + 1] == pIdx[YTXrandNum + 2]) continue;
		//	// ���_���W�̐ݒ�
		//	//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
		//	//�㑤
		//	//if (nCntVtx == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
		//	//	pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 200.0f;
		//	//�����@�㉺���E�̕��ϒl���Z�o
		//	//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
		//	else
		//	{
		//		float y = (pVtx[YTXrandNum - 1].vtx.y + pVtx[YTXrandNum + 1].vtx.y +
		//			pVtx[(this[0].nNumBlockZField + 1) + YTXrandNum].vtx.y + pVtx[(this[0].nNumBlockZField - 1) + YTXrandNum].vtx.y) / 4.0f;
		//		pVtx[YTXrandNum].vtx.y = fabsf(y);
		//	}
		//}


		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				// ���_���W�̐ݒ�
				//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
				//�㑤
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this[0].nNumBlockZField || nCntVtxX == this[0].nNumBlockZField)
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = 200.0f;
				//�����@�㉺���E�̕��ϒl���Z�o
				//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
				else
				{
					float y = (pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4.0f;
					pVtx[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y = fabsf(y);
				}
			}
		}
		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldEnd->Unlock();
		this[0].model.pD3DIdxBuff->Unlock();

	}


	//����v���C���[�t�߂̒n�`��~�n��
	{
		VERTEX_3D *pVtx;
		WORD *pIdx;
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);
		//���_�C���f�b�N�X�o�b�t�@�͋��ʂ�1�����Ȃ��̂�Draw�̂���g���Ă���
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (CntPlayer != InterPolationFieldPlayerNum)
			{
				float HitPosUp;
				float HitPosDown;
				float HitPosLeft;
				float HitPosRight;
				HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

				//�v���C���[�̏���Ă���G���A�����B4���؂Ŕ͈͂��i��B
				//-------------------�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 pos = player[CntPlayer].GetPos();
				SpeedUpFieldHitPoly(pos, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
					fSideSizeXEighth, fSideSizeZEighth, fSideSizeXEighth/2, fSideSizeZEighth/2);

				for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
				{
					//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
					if (nCntVtx == this[0].model.nNumVertexIndex - 2)
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
					//���������蔻��p�|���S���̍��W���Ȃ瓖���蔻����s�@X�`�F�b�N����Z�`�F�b�N�B�Ƃ���true���Ɣ���
					if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
					{
						if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
						{
							// ���_���W�̐ݒ�
							pVtx[pIdx[nCntVtx]].vtx.y = 20.0f;
						}
					}
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldEnd->Unlock();
		// �C���f�b�N�X�f�[�^���A�����b�N����
		this[0].model.pD3DIdxBuff->Unlock();
	}
	this[0].InterPolationFieldType = -1;
	this[0].InterPolationFieldSignal = false;
}

//=============================================================================
// �n�`�̕ό`�����@�������ꂽ�t�B�[���h�ɕό`����
//=============================================================================
bool FIELD::InterPolationField(void)
{
	{
		VERTEX_3D *pVtxStart;
		VERTEX_3D *pVtxEnd;
		VERTEX_3D *pVtxDraw;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxStart, 0);
		this[0].pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtxEnd, 0);
		this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

		float dt = 1.0f / 60.0f;	// 1�t���[���Ői�߂鎞��
		time += dt;

		for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (nCntVtxZ*nCntVtxX == this[0].model.nNumVertexIndex - 2) break;
				else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx) continue;
				else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				//��ԊJ�n
				pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx = MyInterpolation(
					pVtxStart[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx, 
					pVtxEnd[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx,
					0.0, 1.0f, dt, time, LerpEaseInEaseOut);

				// ���ˌ��̐ݒ�
				if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y >= FIELD_Y_HIGH)
				{
					pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
				}
				else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y < FIELD_Y_HIGH && pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx.y > FIELD_Y_LOW)
				{
					pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//��
				}
				else
				{
					pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//��
				}
			}
		}
		if (time >= 1.0f)
		{
			//�ό`���I�������猻�ݕ`�悵�Ă���n�`��pD3DVtxBuffFieldStart�֕ۑ��B���̕ό`�J�n���̒n�`�ɂ���
			for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
			{
				for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
				{
					pVtxStart[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx;
				}
			}
			time = 0.0f;
			InterPolationFieldSignalEnd = true;
		}
		// ���_�f�[�^���A�����b�N����
		this[0].pD3DVtxBuffFieldStart->Unlock();
		this[0].pD3DVtxBuffFieldDraw->Unlock();
		this[0].pD3DVtxBuffFieldEnd->Unlock();
	}
	if (this[0].InterPolationFieldSignalEnd == true)
	{
		this[0].InterPolationFieldSignalEnd = false;
		StopSound(SOUND_LABEL_SE_quake);
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
	VERTEX_3D *pVtxDraw;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (this[0].nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this[0].nNumBlockZField + 1); nCntVtxX++)
		{
			//�k�ރ|���S����
			if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx)
			{
				pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx;
			}
			else if (pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx)
			{
				pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 1].vtx = pVtxDraw[nCntVtxZ * (this[0].nNumBlockZField + 1) + nCntVtxX + 2].vtx;
			}
		}
	}

	this[0].pD3DVtxBuffFieldDraw->Unlock();
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
		fSideSizeXHalf, fSideSizeZHalf, fSideSizeXEighth, fSideSizeZEighth);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
	{
		//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
		if (nCntVtx == this[0].model.nNumVertexIndex - 2)
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
				D3DXVECTOR3 pos = this[0].GetPos();
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
	this[0].pD3DVtxBuffFieldDraw->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	this[0].model.pD3DIdxBuff->Unlock();

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
		fSideSizeXQuarter, fSideSizeZQuarter, fSideSizeXEighth, fSideSizeZEighth);


	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


		for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
		{
			//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
			if (nCntVtx == this[0].model.nNumVertexIndex - 2)
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
					D3DXVECTOR3 pos = this[0].GetPos();
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

						//���̒��_z���W�Ɣ�ׁA����̒��_z���W���傫����΍������p�O�p�`�ɏ���Ă���
						if (pVtx[pIdx[nCntVtx]].vtx.z < pVtx[pIdx[nCntVtx + 1]].vtx.z)
						{
							//���ʂȒ��_�ԍ�
							if (nCntVtx == this[0].model.nNumVertexIndex - this[0].nNumBlockZField * 2)
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
							//�@���@�������p�O�p�`�ō��̒��_�ԍ���this[0].nNumBlockZField�ȉ��Ȃ�΁A�n�ʂ̍ō��[�ɂ���
							//else if (nCntVtx >= this[0].nNumBlockZField)
							//{
							//	int a = 0;

							//}
							//�@��@�������p�O�p�`�ō��̒��_���W�ƈ�O�̒��_���W�������������́A��O�̒��_���W�Ɠ�O�̒��_���W�������Ȃ�΁A�n�ʂ̍ŏ�[�ɂ���
							else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx - 1]].vtx || pVtx[pIdx[nCntVtx - 1]].vtx == pVtx[pIdx[nCntVtx - 2]].vtx)
							{
								int a = 0;

							}
							//�@���@�������p�O�p�`�ō��̒��_���W�ƈ���̒��_���W�������������́A����̒��_���W�Ɠ���̒��_���W�������Ȃ�΁A�n�ʂ̍ŉ��[�ɂ���(�k�ރ|���S���Ȃ̂�true�ɂȂ�Ȃ��͂�)
							else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx + 1]].vtx || pVtx[pIdx[nCntVtx + 1]].vtx == pVtx[pIdx[nCntVtx + 2]].vtx)
							{
								//�k�ރ|���S��
								int a = 0;
							}
							else
							{
								//���_���E�ӂɋ߂Â��Ƌ��ʐ��`��ԂŎp���x�N�g���𐧌�
								//�E��
								float MinDistanceRightUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								float MinDistanceDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
								float MinDistanceLeft = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								//�v���C���[���W����2�ӂ܂ł̋�����SPHERELINEAR_LEN���Z���ƁA���̐�ɑ�����̒n�`�̖@�����ϒl���Ƃ�A���̒l�ŕ�Ԃ�����
								if (SPHERELINEAR_LEN > MinDistanceRightUp && SPHERELINEAR_LEN > MinDistanceDown)
								{
									// 1�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceRightUp
									D3DXVECTOR3 NextPolyNorVec1;
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceDown
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

									//��̖@�����ϒl�����߂�
									D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

									//�������Ă����̋����̕��ς����߂�
									float MinRtionDistance = (MinDistanceRightUp+ MinDistanceDown)/2.0f;

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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeft
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
								else if (SPHERELINEAR_LEN > MinDistanceDown)
								{
									//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceDown) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceLeft)
								{
									//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
							if (nCntVtx == this[0].nNumBlockZField * 2 - 3)
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
							//�@�E�@�E�㒼�p�O�p�`�ō��̒��_�ԍ���this[0].nNumBlockZField�ȏ�Ȃ�΁A�n�ʂ̍ŉE�[�ɂ���
							//else if (nCntVtx >= this[0].nNumBlockZField * this[0].nNumBlockZField)
							//{

							//}
							else
							{
								//���_���E�ӂɋ߂Â��Ƌ��ʐ��`��ԂŎp���x�N�g���𐧌�
								float MinDistanceRight = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								float MinDistanceUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
								float MinDistanceLeftDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeftDown
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
								else if (SPHERELINEAR_LEN > MinDistanceUp)
								{
									//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this[0].nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
									D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
									D3DXVECTOR3 NowPolyNorVec;
									D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
									D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									//SPHERELINEAR_LEN(5)-MinDistance(0�`5)*10 = ��Ԕ{��(0�`0.5�͈̔�)
									SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceUp) / 10);
									*vtxNor = NewPolyNorVec;
								}
								else if (SPHERELINEAR_LEN > MinDistanceLeftDown)
								{
									//�Ӑ�ɂ���n�ʃ|���S���̖@�������߂�
									D3DXVECTOR3 NextPolyNorVec;
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
		this[0].pD3DVtxBuffFieldDraw->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	this[0].model.pD3DIdxBuff->Unlock();

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
		fSideSizeXHalf, fSideSizeZHalf, fSideSizeXEighth, fSideSizeZEighth);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	this[0].pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < int(this[0].model.nNumVertexIndex); nCntVtx++)
	{
		//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
		if (nCntVtx == this[0].model.nNumVertexIndex - 2)
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
					D3DXVECTOR3 pos = this[0].GetPos();
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
	this[0].pD3DVtxBuffFieldDraw->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	this[0].model.pD3DIdxBuff->Unlock();
	if (Cheack == true) return true;

	return false;
}



