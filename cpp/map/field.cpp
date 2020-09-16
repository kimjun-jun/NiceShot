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

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ClearField(void);				//������
void SetFieldType01(void);			//�I���W�i��1 
void SetFieldType02(void);			//�I���W�i��2 
void SetFieldType03(void);			//�I���W�i��3 
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
bool InterPolationFieldSignalFirstTime;			// �n�`�ό`���߂Ă̓ǂݍ��ݎ��̐M���B��x�ό`�J�n���Ĉړ��ʂ����߂�B
static int InterPolationFieldType;				// �n�`�ό`�^�C�v
static int InterPolationFieldPlayerNum;			// �n�`�ό`�A�C�e�����擾�����v���C���[�i���o�[


static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
};


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
	InterPolationFieldPlayerNum = -1;
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
	InitCntPartition();

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
	switch (InterPolationFieldType)
	{
		ClearField();
	case FIELD_TYPE_BOKOBOKO:
		SetFieldType01();
		break;
	case FIELD_TYPE_NADARAKA:
		SetFieldType02();
		break;
	case FIELD_TYPE_PLAYERADVANTAGE:
		SetFieldType03();
		break;
	default:
		break;
	}

	if (InterPolationFieldSignal == false)
	{
		InterPolationFieldSignal = InterPolationField();
	}

	//�v���C���[�ƒn�ʂ̓����蔻��
	PLAYER_HONTAI *player = GetPlayerHoudai();
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHitGetSphereVec(D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y + 1000.0f, player[CntPlayer].pos.z),
			D3DXVECTOR3(player[CntPlayer].pos.x, player[CntPlayer].pos.y - 1000.0f, player[CntPlayer].pos.z),
			&player[CntPlayer].RotVecAxis, &player[CntPlayer].pos.y);
	}

	//�A�C�e���ƒn�ʂ̓����蔻��
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

	//�o���b�g�ƒn�ʂ̓����蔻��
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
void SetFieldInterPolationFieldType(int type,int CntPlayer)
{
	//�t���O�Z�b�g
	InterPolationFieldType = type;
	InterPolationFieldPlayerNum = CntPlayer;

	//�n�`���ς��ƃA�C�e���̓����蔻���������x���Ȃ���
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
// �n�`�̎�������01 �ڂ��ڂ����ݒn�`�@�u���b�N��32*32�@�u���b�N�T�C�Y60*60�`100*100
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
// �n�`�̎�������02�@���ʂ��̂����n�`�@�u���b�N��32*32�@�u���b�N�T�C�Y60*60�`100*100
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
// �n�`�̎�������03�@�擾�v���C���[���L���ɂȂ�n�`(����v���C���[�t�߂�~�n)�@�@�u���b�N��32*32�@�u���b�N�T�C�Y200*200
//=============================================================================
void SetFieldType03(void)
{
	{
		VERTEX_3D *pVtx;
		WORD *pIdx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFieldEnd->Lock(0, 0, (void**)&pVtx, 0);
		g_pD3DIdxBuffFieldDraw->Lock(0, 0, (void**)&pIdx, 0);

		//����@������ݒ�
		for (int nCntVtx = 0; nCntVtx < g_nNumVertexField / 4; nCntVtx++)
		{
			//���������߂钸�_������
			int YTXrandNum(rand() % g_nNumVertexField);

			//���������ߑ��
			int VTXrandY(rand() % 400);
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
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 400.0f;
				//�����@�㉺���E�̕��ϒl���Z�o
				//�אڒ��_�̍����̕��ϒl�����߁A���S�̒��_�̍����Ƃ���B
				else
				{
					float y = (pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = fabsf(y);
				}
			}
		}

		//����v���C���[�t�߂̒n�`��~�n��
		PLAYER_HONTAI *p = GetPlayerHoudai();
		for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
		{
			if (CntPlayer != InterPolationFieldPlayerNum)
			{
				float HitPosUp;
				float HitPosDown;
				float HitPosLeft;
				float HitPosRight;
				HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;
				//�v���C���[�̏���Ă���G���A�����B4���؂Ŕ͈͂��i��B
				SpeedUpFieldHitPoly(p[CntPlayer].pos, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
					g_fSideSizeXEighth, g_fSideSizeZEighth, g_fSideSizeXEighth/2, g_fSideSizeZEighth/2);

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
		g_pD3DVtxBuffFieldDraw->Unlock();
		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_pD3DIdxBuffFieldDraw->Unlock();
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

				//��ԊJ�n
				pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx = MyInterpolation(
					pVtxStart[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx, 
					pVtxEnd[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx,
					0.01f, 1.0f, dt, g_time, LerpEaseInEaseOut);

				// ���ˌ��̐ݒ�
				if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y >= 150.0f)
				{
					pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//�R
				}
				else if (pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y < 150.0f && pVtxDraw[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y > 20.0f)
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
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight, 
		g_fSideSizeXHalf, g_fSideSizeZHalf, g_fSideSizeXEighth, g_fSideSizeZEighth);

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
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
		g_fSideSizeXQuarter, g_fSideSizeZQuarter, g_fSideSizeXEighth, g_fSideSizeZEighth);


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




