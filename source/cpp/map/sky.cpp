/**
* @file field.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/sky.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	SKY_TEXTURE_FILENAME	"../data/TEXTURE/map/sky001.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����

constexpr float	VALUE_MOVE_SKY		{ 5.0f };				// �ړ����x
constexpr int	SKY_BLOCK_H			{ 16 };					// �����̃|���S����
constexpr int	SKY_BLOCK_V			{ 8 };					// �����̃|���S����
constexpr float	SKY_RADIUS			{ 4000.0f };			// SKY���a
constexpr float	VALUE_ROTATE_SKY	{ D3DX_PI * 0.01f };	// ��]���x
constexpr float	VALUE_TIME_SKY		{ 0.01f };				// �ړ����x
constexpr float	SKY_HEIGHT_RATE		{ 2.0f };				// ��h�[���̍����W��
constexpr int	TEX_COUNT_LOOP		{ 1 };					// �e�N�X�`���̌J��Ԃ���

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
SKY::SKY(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(SKY_TEXTURE_FILENAME);

	//��]���x�̃Z�b�g
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].Addtime = VALUE_TIME_SKY;
	this->SkyPara[SKY_MODEL_TYPE_TOP].Addtime = VALUE_TIME_SKY;

	// �|���S���\���ʒu�̒��S���W��ݒ�
	this->Transform[SKY_MODEL_TYPE_SPHERE].Pos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this->Transform[SKY_MODEL_TYPE_SPHERE].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	this->Transform[SKY_MODEL_TYPE_TOP].Pos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this->Transform[SKY_MODEL_TYPE_TOP].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �u���b�N���̐ݒ�
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH = SKY_BLOCK_H;
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV = SKY_BLOCK_V;
	this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH = SKY_BLOCK_H;
	this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV = SKY_BLOCK_V;

	// ���_���A�C���f�b�N�X���A�|���S�����A��]�ʂ̐ݒ�
	this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertex((this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1) * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1));
	this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertexIndex(this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV * 2) + this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV * 2 + (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV - 1) * 2);
	this->Attribute[SKY_MODEL_TYPE_SPHERE].NumPolygon(this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV * 2) + (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV - 1) * 4);
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].fRotY = VALUE_ROTATE_SKY;

	// ���_���A�C���f�b�N�X���A�|���S�����A��]�ʂ̐ݒ�
	this->Attribute[SKY_MODEL_TYPE_TOP].NumVertex((this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH + 1) * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV + 1));
	this->Attribute[SKY_MODEL_TYPE_TOP].NumVertexIndex(this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV * 2) + this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV * 2 + (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV - 1) * 2);
	this->Attribute[SKY_MODEL_TYPE_TOP].NumPolygon(this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV * 2) + (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV - 1) * 4);
	this->SkyPara[SKY_MODEL_TYPE_TOP].fRotY = VALUE_ROTATE_SKY;

	//���_�쐬
	this->vtx[SKY_MODEL_TYPE_SPHERE].MakeVertex3D(this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertex(), FVF_VERTEX_3D);
	this->vtx[SKY_MODEL_TYPE_SPHERE].MakeIdxVertex(this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertexIndex());
	this->vtx[SKY_MODEL_TYPE_TOP].MakeVertex3D(this->Attribute[SKY_MODEL_TYPE_TOP].NumVertex(), FVF_VERTEX_3D);
	this->vtx[SKY_MODEL_TYPE_TOP].MakeIdxVertex(this->Attribute[SKY_MODEL_TYPE_TOP].NumVertexIndex());

	//���_�ݒ�
	this->SetUpMeshSphere();
	this->SetUpMeshTop();
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
SKY::~SKY(void)
{
	//�e�N�X�`�����
	this->tex.~TEXTURE();

	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)�@
	for (int CntSky = 0; CntSky < OBJECT_SKY_MAX; CntSky++)
	{
		//���_���
		this->vtx[CntSky].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void SKY::Init()
{
	//�F�̏�����
	this->ResetColor();

	//�^�C��������
	this->SkyPara[SKY_MODEL_TYPE_SPHERE].time = 1.0f;
	this->SkyPara[SKY_MODEL_TYPE_TOP].time = 1.0f;
}

//=============================================================================
// �X�V����
//=============================================================================
void SKY::Update(void)
{
	//�����]�����鏈��
	for (int nCntSky = 0; nCntSky < OBJECT_SKY_MAX; nCntSky++)
	{
		D3DXVECTOR3 rot = this->Transform[nCntSky].Rot();
		rot.y += D3DX_PI * this->SkyPara[nCntSky].fRotY;

		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		this->Transform[nCntSky].Rot(rot);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void SKY::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//DWORD dwSettingLighting, dwSettingCullmode;
	//pDevice->GetRenderState(D3DRS_LIGHTING, &dwSettingLighting);
	//pDevice->GetRenderState(D3DRS_CULLMODE, &dwSettingCullmode);
	//// ���C�e�B���O�𖳌���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	D3DXVECTOR3 pos = this->Transform[SKY_MODEL_TYPE_SPHERE].Pos();
	D3DXVECTOR3 rot = this->Transform[SKY_MODEL_TYPE_SPHERE].Rot();

	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y,rot.x,rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, pos.x,pos.y,pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff(), 0, sizeof(VERTEX_3D));
	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(this->vtx[SKY_MODEL_TYPE_SPHERE].IdxBuff());
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this->tex.Texture());
	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this->Attribute[SKY_MODEL_TYPE_SPHERE].NumVertex(), 0, this->Attribute[SKY_MODEL_TYPE_SPHERE].NumPolygon());

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff(), 0, sizeof(VERTEX_3D));
	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(this->vtx[SKY_MODEL_TYPE_TOP].IdxBuff());
	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH + 2), 0, this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH);

	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O
	//pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
	//// ���C�e�B���O��L����
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//pDevice->SetRenderState(D3DRS_LIGHTING, dwSettingLighting);
	//pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
}

//=============================================================================
// ����SKY�Z�b�g�A�b�v����
//=============================================================================
void SKY::SetUpMeshSphere(void)
{
	//�T�C�Y�ݒ�p����
	VERTEX_3D *pVtx;
	const float ANGLE_H = (D3DX_PI * 2.0f) / this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH;
	const float ANGLE_V = (D3DX_PI / 8.0f) / (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1);
	const float WIDTH = 1.0f / this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH;
	const float HEIGHT = 1.0f / this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV;
	float fLengthXZ;
	float fRadius = SKY_RADIUS;

	//���_�o�b�t�@�̃Z�b�g
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	VtxBuff = this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntV = 0; nCntV < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1); nCntV++)
	{
		fLengthXZ = cosf(ANGLE_V * nCntV) * fRadius;

		for (int nCntH = 0; nCntH < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1); nCntH++, pVtx++)
		{
			// ���_���W�̐ݒ�
			pVtx->vtx.x = sinf(ANGLE_H * nCntH) * fLengthXZ;
			pVtx->vtx.y = sinf(ANGLE_V * nCntV) * fRadius * SKY_HEIGHT_RATE;
			pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

			// �@���̐ݒ�
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = nCntH * WIDTH * TEX_COUNT_LOOP;

			if (nCntV == this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV)
			{
				pVtx->tex.y = 0.01f;
			}
			else
			{
				pVtx->tex.y = 1.0f - nCntV * HEIGHT;
			}
		}
	}
	// ���_�f�[�^���A�����b�N����
	VtxBuff->Unlock();

	//���_�C���f�b�N�X�̃Z�b�g
	LPDIRECT3DINDEXBUFFER9	IdxBuff;
	IdxBuff = this->vtx[SKY_MODEL_TYPE_SPHERE].IdxBuff();
	WORD *pIdx;
	// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntV = 0; nCntV < this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV; nCntV++)
	{
		if (nCntV != 0)
		{
			*pIdx = nCntV * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1);
			pIdx++;
		}

		for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1; nCntH++)
		{
			*pIdx = nCntV * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1) + nCntH;
			pIdx++;

			*pIdx = (nCntV + 1) * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1) + nCntH;
			pIdx++;
		}

		if (nCntV != (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV - 1))
		{
			*pIdx = (nCntV + 1) * (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1);
			pIdx++;
		}
	}

	// �C���f�b�N�X�f�[�^���A�����b�N����
	IdxBuff->Unlock();

	//�Z�b�g�A�b�v�����f�[�^�𔽉f
	this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff(VtxBuff);
	this->vtx[SKY_MODEL_TYPE_SPHERE].IdxBuff(IdxBuff);
}

//=============================================================================
// �㕔SKY�Z�b�g�A�b�v����
//=============================================================================
void SKY::SetUpMeshTop(void)
{
	//�T�C�Y�ݒ�p����
	VERTEX_3D *pVtx;
	const float ANGLE_H = (D3DX_PI * 2.0f) / this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH;
	const float ANGLE_V = (D3DX_PI / 8.0f) / (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV + 1);
	const float WIDTH = 1.0f / this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH;
	const float HEIGHT = 1.0f / this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV;
	float fLengthXZ;
	float fRadius = SKY_RADIUS;

	//���_�o�b�t�@�̃Z�b�g
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	VtxBuff = this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx->vtx.x = 0.0f;
	pVtx->vtx.y = sinf(ANGLE_V * (this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV + 1)) * fRadius * SKY_HEIGHT_RATE;
	pVtx->vtx.z = 0.0f;

	// �@���̐ݒ�
	pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx->tex.x = 0.01f;
	pVtx->tex.y = 0.01f;

	pVtx++;

	fLengthXZ = cosf(ANGLE_V * this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV) * fRadius;
	for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV; nCntH++, pVtx++)
	{
		// ���_���W�̐ݒ�
		pVtx->vtx.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
		pVtx->vtx.y = sinf(ANGLE_V * this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockV) * fRadius * SKY_HEIGHT_RATE;
		pVtx->vtx.z = cosf(ANGLE_H * nCntH) * fLengthXZ;

		// �@���̐ݒ�
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;
	}
	// ���_�f�[�^���A�����b�N����
	VtxBuff->Unlock();


	//���_�C���f�b�N�X�̃Z�b�g
	LPDIRECT3DINDEXBUFFER9	IdxBuff;
	IdxBuff = this->vtx[SKY_MODEL_TYPE_TOP].IdxBuff();
	WORD *pIdx;

	// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH + 1; nCntH++, pIdx++)
	{
		*pIdx = nCntH;
	}
	*pIdx = 1;

	// �C���f�b�N�X�f�[�^���A�����b�N����
	IdxBuff->Unlock();

	//�Z�b�g�A�b�v�����f�[�^�𔽉f
	this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff(VtxBuff);
	this->vtx[SKY_MODEL_TYPE_TOP].IdxBuff(IdxBuff);
}

//=============================================================================
//�J���[�������@���̃��f���Ə㕔���f��
//=============================================================================
void SKY::ResetColor(void)
{
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 vtxSphere = this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff();
	LPDIRECT3DVERTEXBUFFER9 vtxTop = this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff();

	//���̃��f������
	vtxSphere->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntV = 0; nCntV < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockV + 1); nCntV++)
	{
		for (int nCntH = 0; nCntH < (this->SkyPara[SKY_MODEL_TYPE_SPHERE].nNumBlockH + 1); nCntH++, pVtx++)
		{
			// ���_�J���[�̐ݒ�
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	vtxSphere->Unlock();

	//�㕔���f������
	vtxTop->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx++;
	for (int nCntH = 0; nCntH < this->SkyPara[SKY_MODEL_TYPE_TOP].nNumBlockH; nCntH++, pVtx++) pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vtxTop->Unlock();

	//�f�[�^���f
	this->vtx[SKY_MODEL_TYPE_SPHERE].VtxBuff(vtxSphere);
	this->vtx[SKY_MODEL_TYPE_TOP].VtxBuff(vtxTop);
}