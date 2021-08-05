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
#define	WALL_TEXTURE_FILENAME	"../data/TEXTURE/map/interior_fence01_01.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

enum eWALL_TYPE
{
	WALL_TYPE_FRONT = 0,
	WALL_TYPE_LEFT,
	WALL_TYPE_RIGHT,
	WALL_TYPE_BACK,
};

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
WALL::WALL(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(WALL_TEXTURE_FILENAME);

	// �|���S���\���ʒu�̒��S���W��ݒ�
	this->Transform[WALL_TYPE_FRONT].Pos(D3DXVECTOR3(0.0f, 0.0f, WALL_INIT_POSZ));
	this->Transform[WALL_TYPE_LEFT].Pos(D3DXVECTOR3(-WALL_INIT_POSX, 0.0f, 0.0f));
	this->Transform[WALL_TYPE_RIGHT].Pos(D3DXVECTOR3(WALL_INIT_POSX, 0.0f, 0.0f));
	this->Transform[WALL_TYPE_BACK].Pos(D3DXVECTOR3(0.0f, 0.0f, -WALL_INIT_POSZ));

	//�|���S���̉�]�p�p�x��ݒ�
	this->Transform[WALL_TYPE_FRONT].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	this->Transform[WALL_TYPE_LEFT].Rot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f));
	this->Transform[WALL_TYPE_RIGHT].Rot(D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f));
	this->Transform[WALL_TYPE_BACK].Rot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	//���_�쐬
	this->vtx[WALL_TYPE_FRONT].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);
	this->vtx[WALL_TYPE_LEFT].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);
	this->vtx[WALL_TYPE_RIGHT].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);
	this->vtx[WALL_TYPE_BACK].MakeVertex3D(POLYGON_2D_VERTEX, FVF_VERTEX_3D);

	//���_�ݒ�
	this->SetUpMesh();
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
WALL::~WALL(void)
{
	//�e�N�X�`�����
	this->tex.~TEXTURE();

	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)�@
	for (int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		//���_���
		this->vtx[nCntMeshField].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void WALL::Init(void)
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
		D3DXVECTOR3 pos = this->Transform[nCntMeshField].Pos();
		D3DXVECTOR3 rot = this->Transform[nCntMeshField].Rot();

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
		pDevice->SetStreamSource(0, this->vtx[nCntMeshField].VtxBuff(), 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, this->tex.Texture());

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}
}

//=============================================================================
// WALL�Z�b�g�A�b�v����
//=============================================================================
void WALL::SetUpMesh(void)
{
	for (int nCntMeshField = 0; nCntMeshField < OBJECT_WALL_MAX; nCntMeshField++)
	{
		//���_�o�b�t�@�̃Z�b�g
		VERTEX_3D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 VtxBuff;
		VtxBuff = this->vtx[nCntMeshField].VtxBuff();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		VtxBuff->Unlock();

		//�Z�b�g�A�b�v�����f�[�^�𔽉f
		this->vtx[nCntMeshField].VtxBuff(VtxBuff);
	}
}