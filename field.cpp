/**
* @file field.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "main.h"
#include "input.h"
#include "field.h"
#include "item.h"
#include "bullet.h"
#include "sound.h"
#include "explosion.h"
#include "player.h"
#include "library.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME				"data/TEXTURE/white.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_NOISE					"data/TEXTURE/noise.bmp"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define FIELD_PARTITION_FIRST_NUM		(4)
#define FIELD_PARTITION_SECOND_NUM		(16)
#define FIELD_PARTITION_THIRD_NUM		(64)
#define SPHERELINEAR_LEN				(8.0f)							//!< ���ʐ��`��ԂŎg���A�v���C���[�̍��W��3�p�|���S���ӂ܂ł̋���(�v���C���[�̍��W�����̒l���ӂɋ߂���΋��ʐ��`��ԊJ�n����)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ClearField(void);				//������
void SetFieldType01(void);			//�I���W�i��1 
void SetFieldType02(void);			//�I���W�i��2 
bool InterPolationField(void);		//�n�`�����X�ɕω�������
void SetDegenerationPoly(void);		//�k�ރ|���S���̍��W���Čv�Z����֐��@�n�`�ό`��A�k�ރ|���S���̍��W��ݒ肷��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//�t�B�[���h�����p
LPDIRECT3DTEXTURE9 g_pD3DTextureField;				// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFieldDraw;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�`��p
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFieldEnd;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^��ԃf�[�^�G���h�o�b�t�@
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFieldStart;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^��ԃf�[�^�X�^�[�g�o�b�t�@
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffFieldDraw;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX g_mtxWorldField;							// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posField;								// �|���S���\���ʒu�̒��S���W
D3DXVECTOR3 g_rotField;								// �|���S���̉�]�p
int g_nNumBlockXField, g_nNumBlockZField;			// �u���b�N��
int g_nNumVertexField;								// �����_��	
int g_nNumVertexIndexField;							// ���C���f�b�N�X��
int g_nNumPolygonField;								// ���|���S����
float g_fBlockSizeXField, g_fBlockSizeZField;		// �u���b�N�T�C�Y
float g_fSideSizeXMax, g_fSideSizeZMax;				// �ӃT�C�Y�ő� 1920
float g_fSideSizeXHalf, g_fSideSizeZHalf;			// �ӃT�C�Y1/2   960
float g_fSideSizeXQuarter, g_fSideSizeZQuarter;		// �ӃT�C�Y1/4   480
float g_fSideSizeXEighth, g_fSideSizeZEighth;		// �ӃT�C�Y1/8   240


//�t�B�[���h�ό`�p
static float g_time = 0.0f;						// �n�`�̕ό`����
bool InterPolationFieldSignal;					// �n�`�ό`�M��
bool InterPolationFieldSignalEnd;				// �n�`�ό`�M���I���������ǂ���
bool InterPolationFieldSignalFirstTime;	// �n�`�ό`���߂Ă̓ǂݍ��ݎ��̐M���B��x�ό`�J�n���Ĉړ��ʂ����߂�B
static int InterPolationFieldType;				// �n�`�ό`�^�C�v


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
};

//�����蔻�荂�����p
FIELD_COLLISION g_FirstField_Collision[FIELD_PARTITION_FIRST_NUM];
FIELD_COLLISION g_SecondField_Collision[FIELD_PARTITION_SECOND_NUM];
FIELD_COLLISION g_ThirdField_Collision[FIELD_PARTITION_THIRD_NUM];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	InterPolationFieldSignal = true;
	InterPolationFieldSignalEnd = false;
	InterPolationFieldSignalFirstTime = false;

	InterPolationFieldType = -1;
	// �|���S���\���ʒu�̒��S���W��ݒ�
	g_posField = pos;

	g_rotField = rot;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,								// �t�@�C���̖��O
		&g_pD3DTextureField);							// �ǂݍ��ރ������[

	// �u���b�N���̐ݒ�
	g_nNumBlockXField = nNumBlockX;
	g_nNumBlockZField = nNumBlockZ;

	// ���_���̐ݒ�
	g_nNumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);


	// �C���f�b�N�X���̐ݒ�
	g_nNumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	g_nNumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;

	//�@�ӃT�C�Y�̐ݒ�
	g_fSideSizeXMax = nNumBlockX * fBlockSizeX;
	g_fSideSizeZMax = nNumBlockZ * fBlockSizeZ;
	g_fSideSizeXHalf = g_fSideSizeXMax / 2.0f;
	g_fSideSizeZHalf = g_fSideSizeZMax / 2.0f;
	g_fSideSizeXQuarter = g_fSideSizeXHalf / 2.0f;
	g_fSideSizeZQuarter = g_fSideSizeZHalf / 2.0f;
	g_fSideSizeXEighth = g_fSideSizeXQuarter / 2.0f;
	g_fSideSizeZEighth = g_fSideSizeZQuarter / 2.0f;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffFieldDraw,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffFieldEnd,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffFieldStart,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DIdxBuffFieldDraw,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	//�`��p
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		g_pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFieldDraw->Unlock();
		g_pD3DVtxBuffFieldStart->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (g_nNumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + g_nNumBlockXField;
				nCntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_pD3DIdxBuffFieldDraw->Unlock();
	}

	//�`��p
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
		g_pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxS, 0);

		//���
		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1)/2; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1)/2; nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;
				pVtx[(rand() % g_nNumVertexField)].vtx.y = float((rand() % 200));
			}
		}

		//�k�ރ|���S���̍��W�𒲐�
		SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//�㑤
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 50.0f;
				//����
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = y;
				}

				// ���ˌ��̐ݒ�
				if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
				}
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//��
				}
				else
				{
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//��
				}
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFieldDraw->Unlock(); 
		g_pD3DVtxBuffFieldStart->Unlock();
	}



	//���������̃o�b�t�@�B�n�`�ύX����Ƃ��ɂ��̌`�ɕό`	F
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFieldEnd->Unlock();
	}


	//�����蔻�荂�����p�B�ꉞ�e�q�֌W���o�^
	//�e����
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_FIRST_NUM; CntPartition++)
	{
		g_FirstField_Collision[CntPartition].Parent = NULL;
		g_FirstField_Collision[CntPartition].Num = CntPartition;
		g_FirstField_Collision[CntPartition].Cheak = false;
	}
	//�q����
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_SECOND_NUM; CntPartition++)
	{
		switch (CntPartition)
		{
		case 0:case 1:case 2:case 3:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[0];
			break;
		case 4:case 5:case 6:case 7:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[1];
			break;
		case 8:case 9:case 10:case 11:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[2];
			break;
		case 12:case 13:case 14:case 15:
			g_SecondField_Collision[CntPartition].Parent = &g_FirstField_Collision[3];
			break;
		default:
			break;
		}
		g_SecondField_Collision[CntPartition].Num = CntPartition;
		g_SecondField_Collision[CntPartition].Cheak = false;
	}
	//������
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_THIRD_NUM; CntPartition++)
	{
		switch (CntPartition)
		{
		case 0:case 1:case 2:case 3:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[0];
			break;
		case 4:case 5:case 6:case 7:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[1];
			break;
		case 8:case 9:case 10:case 11:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[2];
			break;
		case 12:case 13:case 14:case 15:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[3];
			break;

		case 16:case 17:case 18:case 19:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[4];
			break;
		case 20:case 21:case 22:case 23:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[5];
			break;
		case 24:case 25:case 26:case 27:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[6];
			break;
		case 28:case 29:case 30:case 31:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[7];
			break;

		case 32:case 33:case 34:case 35:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[8];
			break;
		case 36:case 37:case 38:case 39:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[9];
			break;
		case 40:case 41:case 42:case 43:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[10];
			break;
		case 44:case 45:case 46:case 47:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[11];
			break;

		case 48:case 49:case 50:case 51:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[12];
			break;
		case 52:case 53:case 54:case 55:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[13];
			break;
		case 56:case 57:case 58:case 59:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[14];
			break;
		case 60:case 61:case 62:case 63:
			g_ThirdField_Collision[CntPartition].Parent = &g_SecondField_Collision[15];
			break;
		}
		g_ThirdField_Collision[CntPartition].Num = CntPartition;
		g_ThirdField_Collision[CntPartition].Cheak = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if (g_pD3DVtxBuffFieldDraw)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffFieldDraw->Release();
		g_pD3DVtxBuffFieldDraw = NULL;
	}

	if (g_pD3DIdxBuffFieldDraw)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_pD3DIdxBuffFieldDraw->Release();
		g_pD3DIdxBuffFieldDraw = NULL;
	}

	if (g_pD3DTextureField)
	{// �e�N�X�`���̊J��
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
	if (InterPolationFieldType == 0)
	{
		ClearField();
		SetFieldType01();
	}
	else if (InterPolationFieldType == 1)
	{
		ClearField();
		SetFieldType02();
	}
	if (InterPolationFieldSignal == false)
	{
		InterPolationFieldSignal = InterPolationField();
	}

	PLAYER_HONTAI *player = GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHitGetSphereVec(D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y + 1000.0f, player[CntPlayer].pos.z),
			D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y - 1000.0f, player[CntPlayer].pos.z),
			&player[CntPlayer].RotVecAxis, &player[CntPlayer].pos.y);
	}

	ITEM *item = GetItem();
	for (int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		if (item[CntItem].bUse == true)
		{
			if (item[CntItem].fCollisionEnd == false)
			{
				FieldHit(D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y + 1000.0f, item[CntItem].pos.z),
					D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y - 1000.0f, item[CntItem].pos.z),
					&item[CntItem].rotVecAxis, &item[CntItem].pos.y);
				FieldHit(D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y - 1000.0f, item[CntItem].pos.z),
					D3DXVECTOR3(item[CntItem].pos.x, item[CntItem].pos.y + 1000.0f, item[CntItem].pos.z),
					&item[CntItem].rotVecAxis, &item[CntItem].pos.y);
				if (InterPolationFieldSignal == true)
				{
					item[CntItem].fCollisionEnd = true;
				}
			}
		}
	}

	BULLET *bullet = GetBullet();
	for (int Cntbullet = 0; Cntbullet < BULLET_MAX; Cntbullet++)
	{
		if (bullet[Cntbullet].use == true)
		{
			D3DXVECTOR3 kari;
			FieldHit(D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].pos.y + 1000.0f, bullet[Cntbullet].pos.z),
				D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].pos.y - 1000.0f, bullet[Cntbullet].pos.z),
				&kari, &bullet[Cntbullet].FieldPosY);
			if (bullet[Cntbullet].FieldPosY - 3.0f > bullet[Cntbullet].pos.y)//�n�`�Ƃ̓����蔻����ɂ�(����)���邽�߂�bullet[Cntbullet].FieldPosY-3.0f
			{
				// �����̐���
				D3DXVECTOR3 ExploPos = D3DXVECTOR3(bullet[Cntbullet].pos.x, bullet[Cntbullet].FieldPosY, bullet[Cntbullet].pos.z);
				SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[bullet[Cntbullet].UsePlayerType]);
				// �o���b�g�j��
				ReleaseBullet(Cntbullet);
				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_damage);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, g_pD3DVtxBuffFieldDraw, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(g_pD3DIdxBuffFieldDraw);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureField);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexField, 0, g_nNumPolygonField);
}

//=============================================================================
// ���_�f�[�^�̃A�h���X��n��
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 *GetFieldBuff(void)
{
	return &g_pD3DVtxBuffFieldDraw;
}

//=============================================================================
// ���_�f�[�^�̃A�h���X��n��
//=============================================================================
int GetFieldBlockXNum(void)
{
	return g_nNumBlockXField;
}

//=============================================================================
// ���_�f�[�^�̃A�h���X��n��
//=============================================================================
int GetFieldBlockZNum(void)
{
	return g_nNumBlockZField;
}

//=============================================================================
// ���_�f�[�^�̃A�h���X��n��
//=============================================================================
float GetFieldBlockXSize(void)
{
	return g_fBlockSizeXField;
}

//=============================================================================
// ���_�f�[�^�̃A�h���X��n��
//=============================================================================
float GetFieldBlockZSize(void)
{
	return g_fBlockSizeZField;
}

//=============================================================================
// �n�`�ό`�^�C�v
//=============================================================================
void SetFieldInterPolationFieldType(int type)
{
	InterPolationFieldType = type;
	ITEM *item = GetItem();
	for (int CntItem = 0; CntItem < MAX_ITEM; CntItem++)
	{
		item[CntItem].fCollisionEnd = false;
	}
}

//=============================================================================
// �n�`�̃N���A
//=============================================================================
void ClearField(void)
{
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
		{
			// ���_���W�̐ݒ�
			pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
		}
	}
	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffFieldEnd->Unlock();
}

//=============================================================================
// �n�`�̎�������01 �ڂ��ڂ����ݒn�`
//=============================================================================
void SetFieldType01(void)
{
	{
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//����@������ݒ�
		for (int nCntVtx = 0; nCntVtx < g_nNumVertexField/4; nCntVtx++)
		{
				//���������߂钸�_������
				int YTXrandNum(rand() % g_nNumVertexField);

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

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 50.0f;
				//����
				//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = y;
				}
			}
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFieldEnd->Unlock();
	}
	InterPolationFieldSignal = false;
}

//=============================================================================
// �n�`�̎�������02�@���ʂ��̂����n�`
//=============================================================================
void SetFieldType02(void)
{
	{
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);

		//����@������ݒ�
		for (int nCntVtx = 0; nCntVtx < g_nNumVertexField / 4; nCntVtx++)
		{
			//���������߂钸�_������
			int YTXrandNum(rand() % g_nNumVertexField);

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

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				// ���_���W�̐ݒ�
				//���_�Œ[�̍����͌Œ�B�Ǎۂ̒��_�̂��ƁB
				//�㑤
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == g_nNumBlockZField || nCntVtxX == g_nNumBlockXField)
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 100.0f;
				//�����@�㉺���E�̕��ϒl���Z�o
				//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = float(fabs(y));
				}
			}
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFieldEnd->Unlock();
	}
	InterPolationFieldSignal = false;
}

//=============================================================================
// �n�`�̕ό`�����@�������ꂽ�t�B�[���h�ɕό`����
//=============================================================================
bool InterPolationField(void)
{
	{
		VERTEX_3D *pVtxStart;
		VERTEX_3D *pVtxEnd;
		VERTEX_3D *pVtxDraw;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFieldStart->Lock(0, 0, (void**)&pVtxStart, 0);
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtxEnd, 0);
		g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

		float dt = 1.0f / 60.0f;	// 1�t���[���Ői�߂鎞��
		g_time += dt;		// �A�j���[�V�����̍��v���Ԃɑ���

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				//�k�ރ|���S���悯�Ȃ���
				if (nCntVtxZ*nCntVtxX == g_nNumVertexIndexField - 2) break;
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx) continue;
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx) continue;

				//���`�@���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
				//���`�@i(t)= s(1-v(t))+e*v(t)
				D3DXVECTOR3 vtxvec1 = pVtxEnd[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx - pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
				pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx + vtxvec1 * g_time;

				//�O���@v(t)= -2t^3 = t^2(3-2t)
			//t(�^�C��(dt)),b(�J�n�_),c(�J�n�_�ƖړI�_�̍���),d(���v����(t))
			//t/=d/2.0f
			//c/2.0f*t*t+b
			//-c/2.0f*(t*(t-2)-1)+b
			//D3DXVECTOR3 c = pVtxF[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx - pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
			//D3DXVECTOR3 oldpos = pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
			//pVtxG[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx =
			//	-c / 2.0f*(t*(t-2)-1) + pVtxG[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;

			//pVtxS[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = oldpos;

			// ���ˌ��̐ݒ�
				if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
				}
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//��
				}
				else
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//��
				}
			}
		}
		if (g_time >= 1.0f)
		{
			//�ό`���I�������猻�ݕ`�悵�Ă���n�`��g_pD3DVtxBuffFieldStart�֕ۑ��B���̕ό`�J�n���̒n�`�ɂ���
			for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
			{
				for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
				{
					pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx;
				}
			}
			g_time = 0.0f;
			InterPolationFieldType = -1;
			InterPolationFieldSignalEnd = true;
			InterPolationFieldSignalFirstTime = false;
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFieldStart->Unlock();
		g_pD3DVtxBuffFieldDraw->Unlock();
		g_pD3DVtxBuffFieldEnd->Unlock();
	}
	if (InterPolationFieldSignalEnd == true)
	{
		InterPolationFieldSignalEnd = false;
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
void SetDegenerationPoly(void)
{
	VERTEX_3D *pVtxDraw;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtxDraw, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
		{
			//�k�ރ|���S����
			if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx)
			{
				pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx;
			}
			else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx == pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx)
			{
				pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx = pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 2].vtx;
			}
		}
	}
}



//=============================================================================
// �n�`�Ƃ̓����蔻�� rayS,rayG,vtxHOUSEN,posy �Ԃ�lvoid �v���C���[�ȊO�̓����蔻��Ŏg�p
//=============================================================================
void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
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
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight);



	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < g_nNumVertexIndexField; nCntVtx++)
	{
		//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
		if (nCntVtx == g_nNumVertexIndexField - 2)
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

				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + g_posField;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + g_posField;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + g_posField;

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
	g_pD3DVtxBuffFieldDraw->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	g_pD3DIdxBuffFieldDraw->Unlock();

}


//=============================================================================
// �n�`�Ƃ̓����蔻�� rayS,rayG,vtxHOUSEN,posy �Ԃ�lvoid�@�v���C���[�̓����蔻��Ŏg�p�B���ʕ�ԂŎg�p���邽�߁B
//=============================================================================
void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
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
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight);



	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFieldDraw->Lock(0, 0, (void**)&pVtx, 0);
	g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);


		for (int nCntVtx = 0; nCntVtx < g_nNumVertexIndexField; nCntVtx++)
		{
			//�k�ރ|���S���̂Ƃ��̓R���e�B�j���[�B�ŏI�|���S���̎���break;
			if (nCntVtx == g_nNumVertexIndexField - 2)
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

					vtx[0] = pVtx[pIdx[nCntVtx]].vtx + g_posField;
					vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + g_posField;
					vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + g_posField;

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
							if (nCntVtx == g_nNumVertexIndexField - g_nNumBlockXField * 2)
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
							//�@���@�������p�O�p�`�ō��̒��_�ԍ���g_nNumBlockZField�ȉ��Ȃ�΁A�n�ʂ̍ō��[�ɂ���
							//else if (nCntVtx >= g_nNumBlockZField)
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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeft
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
							if (nCntVtx == g_nNumBlockXField * 2 - 3)
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
							//�@�E�@�E�㒼�p�O�p�`�ō��̒��_�ԍ���g_nNumBlockXField�ȏ�Ȃ�΁A�n�ʂ̍ŉE�[�ɂ���
							//else if (nCntVtx >= g_nNumBlockXField * g_nNumBlockZField)
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
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
									if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
									D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

									// 2�߁@�Ӑ�ɂ���n�ʃ|���S���̖@�������߂� MinDistanceLeftDown
									D3DXVECTOR3 NextPolyNorVec2;
									D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
									D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (g_nNumBlockXField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
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
	g_pD3DVtxBuffFieldDraw->Unlock();
	// �C���f�b�N�X�f�[�^���A�����b�N����
	g_pD3DIdxBuffFieldDraw->Unlock();

}

void SpeedUpFieldHitPoly(D3DXVECTOR3 InrayS, float *HitPosUp, float *HitPosDown, float *HitPosLeft, float *HitPosRight)
{
	//����p�t���O���������@g_ThirdField_Collision�̓v���C���[�̋��ʕ�Ԃ��v�Z���Ɍ��ݏ���Ă���|���S���ԍ��Ƃ��Ďg�p
	//�e����
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_FIRST_NUM; CntPartition++)
	{
		g_FirstField_Collision[CntPartition].Cheak = false;
	}
	//�q����
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_SECOND_NUM; CntPartition++)
	{
		g_SecondField_Collision[CntPartition].Cheak = false;
	}
	//������
	for (int CntPartition = 0; CntPartition < FIELD_PARTITION_THIRD_NUM; CntPartition++)
	{
		g_ThirdField_Collision[CntPartition].Cheak = false;
	}

	//����@X,Z���v���X���}�C�i�X���@���g_posField�̒��S(x0,z0)�@if()����3�����ǒ���
	if (InrayS.x >= 0.0f && InrayS.z >= 0.0f)			//�e++���ی�
	{
		g_FirstField_Collision[1].Cheak = true;
		//�q����@�e�̎��Ɠ������A���ی��̂Ȃ��ł���Ƀv���X���}�C�i�X������
		if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//�q++���ی�
		{
			g_SecondField_Collision[5].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[21].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[20].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[22].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[23].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//�q-+���ی�
		{
			g_SecondField_Collision[4].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[17].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[16].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[18].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[19].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//�q--��O�ی�
		{
			g_SecondField_Collision[6].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[25].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[24].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[26].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[27].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//�q+-��l�ی�
		{
			g_SecondField_Collision[7].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[29].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[28].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[30].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[31].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
	}
	else if (InrayS.x < 0.0f && InrayS.z >= 0.0f)			//�e-+���ی�
	{
		g_FirstField_Collision[0].Cheak = true;
		//�q����@�e�̎��Ɠ������A���ی��̂Ȃ��ł���Ƀv���X���}�C�i�X������
		if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//�q++���ی�
		{
			g_SecondField_Collision[1].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[5].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[4].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[6].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[7].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z >= g_fSideSizeZQuarter)			//�q-+���ی�
		{
			g_SecondField_Collision[0].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[1].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[0].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[2].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < g_fSideSizeZQuarter + g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[3].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter + g_fSideSizeZEighth;
				*HitPosDown = g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//�q--��O�ی�
		{
			g_SecondField_Collision[2].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[9].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[8].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[10].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[11].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z < g_fSideSizeZQuarter)			//�q+-��l�ی�
		{
			g_SecondField_Collision[3].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[13].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[12].Cheak = true;
				*HitPosUp = g_fSideSizeZQuarter;
				*HitPosDown = g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[14].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[15].Cheak = true;
				*HitPosUp = g_fSideSizeZEighth;
				*HitPosDown = 0.0f;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
		}
	}
	else if (InrayS.x < 0.0f && InrayS.z < 0.0f)			//�e--��O�ی�
	{
		g_FirstField_Collision[2].Cheak = true;
		if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//�q++���ی�
		{
			g_SecondField_Collision[9].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[37].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[36].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[38].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[39].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//�q-+���ی�
		{
			g_SecondField_Collision[8].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[33].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[32].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[34].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[35].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < -g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//�q--��O�ی�
		{
			g_SecondField_Collision[10].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[41].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[40].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[42].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXQuarter - g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[43].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter - g_fSideSizeXEighth;
				*HitPosRight = -g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= -g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//�q+-��l�ی�
		{
			g_SecondField_Collision[11].Cheak = true;
			//������@����
			if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[45].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[44].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x < -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[46].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXQuarter;
				*HitPosRight = -g_fSideSizeXEighth;
			}
			else if (InrayS.x >= -g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[47].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = -g_fSideSizeXEighth;
				*HitPosRight = 0.0f;
			}
		}
	}
	else if (InrayS.x >= 0.0f && InrayS.z < 0.0f)			//�e+-��l�ی�
	{
		g_FirstField_Collision[3].Cheak = true;
		if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//�q++���ی�
		{
			g_SecondField_Collision[13].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[53].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[52].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[54].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[55].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z >= -g_fSideSizeZQuarter)			//�q-+���ی�
		{
			g_SecondField_Collision[12].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[49].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[48].Cheak = true;
				*HitPosUp = 0.0f;
				*HitPosDown = -g_fSideSizeZEighth;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[50].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[51].Cheak = true;
				*HitPosUp = -g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}
		}
		else if (InrayS.x < g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//�q--��O�ی�
		{
			g_SecondField_Collision[14].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[57].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[56].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[58].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = 0.0f;
				*HitPosRight = g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[59].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter;
			}

		}
		else if (InrayS.x >= g_fSideSizeXQuarter && InrayS.z < -g_fSideSizeZQuarter)			//�q+-��l�ی�
		{
			g_SecondField_Collision[15].Cheak = true;
			//������@����
			if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��++���ی�
			{
				g_ThirdField_Collision[61].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z >= -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��-+���ی�
			{
				g_ThirdField_Collision[60].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x < g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��--��O�ی�
			{
				g_ThirdField_Collision[62].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXQuarter;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXEighth;
			}
			else if (InrayS.x >= g_fSideSizeXQuarter + g_fSideSizeXEighth && InrayS.z < -g_fSideSizeZQuarter - g_fSideSizeZEighth)			//��+-��l�ی�
			{
				g_ThirdField_Collision[63].Cheak = true;
				*HitPosUp = -g_fSideSizeZQuarter - g_fSideSizeZEighth;
				*HitPosDown = -g_fSideSizeZQuarter - g_fSideSizeZQuarter;
				*HitPosLeft = g_fSideSizeXQuarter + g_fSideSizeXEighth;
				*HitPosRight = g_fSideSizeXQuarter + g_fSideSizeXQuarter;
			}
		}
	}

}



//���ʐ��`��ԎZ�o�֐�
// ���ʐ��`��Ԋ֐�
// out   : ��ԃx�N�g���i�o�́j	��Ԃ��ꂽ�v���C���[�̎p���x�N�g��
// start : �J�n�x�N�g��			�v���C���[�̎p���x�N�g��
// end : �I���x�N�g��			�ړI�n�`�̖@���x�N�g��
// t : ��Ԓl�i0�`1�j			�X�^�[�g����G���h�܂ł̕�Ԓl�̊����@
void SphereLinear(D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t) 
{

	D3DXVECTOR3 s, e;
	D3DXVec3Normalize(&s, start);
	D3DXVec3Normalize(&e, end);


	// 2�x�N�g���Ԃ̊p�x�i�s�p���j
	float angle = acosf(D3DXVec3Dot(&s, &e));

	// sin��
	float SinTh = sinf(angle);

	// ��ԌW��
	float Ps = sinf(angle * (1 - t));
	float Pe = sinf(angle * t);

	*out = (Ps * s + Pe * e) / SinTh;

	// �ꉞ���K�����ċ��ʐ��`��Ԃ�
	D3DXVec3Normalize(out, out);
}


//���ʐ��`��Ԃɂ��p����Ԋ֐�
// ���ʐ��`��Ԃɂ���Ԏp���Z�o�֐�
// out : ��Ԏp���i�o�́j
// start : �J�n�p��
// end : �ڕW�p��
// t : ��ԌW���i0�`1�j
D3DXMATRIX* CalcInterPause(D3DXMATRIX* out, D3DXMATRIX* start, D3DXMATRIX* end, float t) 
{

	// �e�p���x�N�g�����o
	D3DXVECTOR3 Sy, Sz;
	D3DXVECTOR3 Ey, Ez;

	memcpy(&Sy, start->m[1], sizeof(float) * 3);
	memcpy(&Sz, start->m[2], sizeof(float) * 3);
	memcpy(&Ey, end->m[1], sizeof(float) * 3);
	memcpy(&Ez, end->m[2], sizeof(float) * 3);

	// ���ԃx�N�g���Z�o
	D3DXVECTOR3 IY, IZ;
	SphereLinear(&IY, &Sy, &Ey, t);
	SphereLinear(&IZ, &Sz, &Ez, t);

	// ���ԃx�N�g������p���x�N�g�����ĎZ�o
	D3DXVECTOR3 IX;
	D3DXVec3Cross(&IX, &IY, &IZ);
	D3DXVec3Cross(&IY, &IZ, &IX);
	D3DXVec3Normalize(&IX, &IX);
	D3DXVec3Normalize(&IY, &IY);
	D3DXVec3Normalize(&IZ, &IZ);

	memset(out, 0, sizeof(D3DXMATRIX));
	memcpy(out->m[0], &IX, sizeof(float) * 3);
	memcpy(out->m[1], &IY, sizeof(float) * 3);
	memcpy(out->m[2], &IZ, sizeof(float) * 3);
	out->_44 = 1.0f;

	return out;
}