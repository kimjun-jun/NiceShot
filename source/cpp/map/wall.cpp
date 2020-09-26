/**
* @file wall.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/wall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	"../data/TEXTURE/interior_fence01_01.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_WALL		(5.0f)							// �ړ����x
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.002f)				// ��]���x

int WALL::cnt = 0;

//=============================================================================
// ����������
//=============================================================================
void WALL::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���W�Ɗp�x��ݒ�
	D3DXVECTOR3 pos[OBJECT_WALL_MAX];
	D3DXVECTOR3 rot[OBJECT_WALL_MAX];
	pos[0] = D3DXVECTOR3(0.0f, 0.0f, WALL_INIT_POSZ);
	pos[1] = D3DXVECTOR3(-WALL_INIT_POSX, 0.0f, 0.0f);
	pos[2] = D3DXVECTOR3(WALL_INIT_POSX, 0.0f, 0.0f);
	pos[3] = D3DXVECTOR3(0.0f, 0.0f, -WALL_INIT_POSZ);

	rot[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot[1] = D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f);
	rot[2] = D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f);
	rot[3] = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	this[0].SetPos(pos[0]);
	this[1].SetPos(pos[1]);
	this[2].SetPos(pos[2]);
	this[3].SetPos(pos[3]);
	this[0].SetRot(rot[0]);
	this[1].SetRot(rot[1]);
	this[2].SetRot(rot[2]);
	this[3].SetRot(rot[3]);

	int nNumBlockX = 1;
	int nNumBlockY = 1;

	for (int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &this[nCntMeshField].model.pD3DTexture);

		// ���_���̐ݒ�
		this[nCntMeshField].model.nNumVertex = (nNumBlockX + 1) * (nNumBlockY + 1);

		// �C���f�b�N�X���̐ݒ�
		this[nCntMeshField].model.nNumVertexIndex = (nNumBlockX + 1) * 2 * nNumBlockY + (nNumBlockY - 1) * 2;

		// �|���S�����̐ݒ�
		this[nCntMeshField].model.nNumPolygon = nNumBlockX * nNumBlockY * 2 + (nNumBlockY - 1) * 4;

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[nCntMeshField].model.nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&this[nCntMeshField].model.pD3DVtxBuff,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL);								// NULL�ɐݒ�

		// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
		pDevice->CreateIndexBuffer(sizeof(WORD) * this[nCntMeshField].model.nNumVertexIndex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
			D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
			D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&this[nCntMeshField].model.pD3DIdxBuff,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL);				// NULL�ɐݒ�

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[nCntMeshField].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVtxY = 0; nCntVtxY < (nNumBlockY + 1); nCntVtxY++)
			{
				for (int nCntVtxX = 0; nCntVtxX < (nNumBlockX + 1); nCntVtxX++)
				{
					// ���_���W�̐ݒ�
					pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].vtx.x = -(nNumBlockX / 2.0f) * WALL_SIZE_X + nCntVtxX * WALL_SIZE_X;
					pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].vtx.y = nNumBlockY * WALL_SIZE_Y - nCntVtxY * WALL_SIZE_Y;
					pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].vtx.z = 0.0f;

					// �@���̐ݒ�
					pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					// ���ˌ��̐ݒ�
					pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].tex.x = 1.0;
					pVtx[nCntVtxY * (nNumBlockX + 1) + nCntVtxX].tex.y = 1.0;
				}
			}

			// ���_�f�[�^���A�����b�N����
			this[nCntMeshField].model.pD3DVtxBuff->Unlock();
		}

		{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
			WORD *pIdx;

			// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[nCntMeshField].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

			int nCntIdx = 0;
			for (int nCntVtxY = 0; nCntVtxY < nNumBlockY; nCntVtxY++)
			{
				if (nCntVtxY > 0)
				{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
					pIdx[nCntIdx] = (nCntVtxY + 1) * (nNumBlockX + 1);
					nCntIdx++;
				}

				for (int nCntVtxX = 0; nCntVtxX < (nNumBlockX + 1); nCntVtxX++)
				{
					pIdx[nCntIdx] = (nCntVtxY + 1) * (nNumBlockX + 1) + nCntVtxX;
					nCntIdx++;
					pIdx[nCntIdx] = nCntVtxY * (nNumBlockX + 1) + nCntVtxX;
					nCntIdx++;
				}

				if (nCntVtxY < (nNumBlockY - 1))
				{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
					pIdx[nCntIdx] = nCntVtxY * (nNumBlockX + 1) + nNumBlockX;
					nCntIdx++;
				}
			}

			// �C���f�b�N�X�f�[�^���A�����b�N����
			this[nCntMeshField].model.pD3DIdxBuff->Unlock();
		}
	}
}

//=============================================================================
// �ď���������
//=============================================================================
void WALL::Reinit(void)
{
}

//=============================================================================
// �I������
//=============================================================================
void WALL::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void WALL::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void WALL::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for(int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
		//-----------------------------�I�u�W�F�N�g�ǂݍ���
		mtxWorld = this[nCntMeshField].GetMatrix();
		D3DXVECTOR3 rot = this[nCntMeshField].GetRot();
		D3DXVECTOR3 pos = this[nCntMeshField].GetPos();

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
		pDevice->SetStreamSource(0, this[nCntMeshField].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(this[nCntMeshField].model.pD3DIdxBuff);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, this[nCntMeshField].model.pD3DTexture);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this[nCntMeshField].model.nNumVertex, 0, this[nCntMeshField].model.nNumPolygon);
	}

}

