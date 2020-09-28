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

	for (int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &this[nCntMeshField].model.pD3DTexture);

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&this[nCntMeshField].model.pD3DVtxBuff,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL);								// NULL�ɐݒ�

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[nCntMeshField].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// ���_���W�̐ݒ�
					pVtx[0].vtx = D3DXVECTOR3(-WALL_SIZE_X / 2, WALL_SIZE_Y, 0.0f);
					pVtx[1].vtx = D3DXVECTOR3(WALL_SIZE_X / 2, WALL_SIZE_Y, 0.0f);
					pVtx[2].vtx = D3DXVECTOR3(-WALL_SIZE_X / 2, 0.0f, 0.0f);
					pVtx[3].vtx = D3DXVECTOR3(WALL_SIZE_X / 2, 0.0f, 0.0f);

					// �@���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					// ���ˌ��̐ݒ�
					pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			this[nCntMeshField].model.pD3DVtxBuff->Unlock();
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

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, this[nCntMeshField].model.pD3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}
}

