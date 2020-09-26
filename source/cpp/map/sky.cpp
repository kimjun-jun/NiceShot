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
#define	SKY_TEXTURE_FILENAME	"../data/TEXTURE/sky001.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_SKY		(5.0f)							// �ړ����x
#define	VALUE_ROTATE_SKY	(D3DX_PI * 0.01f)				// ��]���x
#define	VALUE_TIME_SKY		(0.001f)							// �ړ����x

#define	SKY_HEIGHT_RATE		(2.0f)		// ��h�[���̍����W��
#define	TEX_COUNT_LOOP		(1)			// �e�N�X�`���̌J��Ԃ���

int SKY::cnt = 0;

//=============================================================================
// ����������
//=============================================================================
void SKY::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ��̏�����
	//InitMeshSky(, ,, , );

	this[0].Addtime = VALUE_TIME_SKY;
	this[1].Addtime = VALUE_TIME_SKY;


	// �|���S���\���ʒu�̒��S���W��ݒ�
	this[0].SetPos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this[0].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	this[1].SetPos(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	this[1].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fRadius = 4000.0f;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		SKY_TEXTURE_FILENAME,	// �t�@�C���̖��O
		&this[0].model.pD3DTexture);	// �ǂݍ��ރ������[
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		SKY_TEXTURE_FILENAME,	// �t�@�C���̖��O
		&this[1].model.pD3DTexture);	// �ǂݍ��ރ������[


	// �u���b�N���̐ݒ�
	this[0].nNumBlockH = 16;
	this[0].nNumBlockV = 8;

	this[1].nNumBlockH = 16;
	this[1].nNumBlockV = 8;

	// ���_���̐ݒ�
	this[0].model.nNumVertex = (nNumBlockH + 1) * (nNumBlockV + 1);
	// �C���f�b�N�X���̐ݒ�
	this[0].model.nNumVertexIndex = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);
	// �|���S�����̐ݒ�
	this[0].model.nNumPolygon = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);
	// ��]��
	this[0].fRotY = 0.0001f;

	// ���_���̐ݒ�
	this[1].model.nNumVertex = (nNumBlockH + 1) * (nNumBlockV + 1);
	// �C���f�b�N�X���̐ݒ�
	this[1].model.nNumVertexIndex = (nNumBlockH * (nNumBlockV * 2) + nNumBlockV * 2 + (nNumBlockV - 1) * 2);
	// �|���S�����̐ݒ�
	this[1].model.nNumPolygon = (nNumBlockH * (nNumBlockV * 2) + (nNumBlockV - 1) * 4);
	// ��]��
	this[1].fRotY = 0.0001f;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * this[0].model.nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this[0].model.pD3DVtxBuff,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);								// NULL�ɐݒ�

// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumBlockH + 1),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this[1].model.pD3DVtxBuff,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);								// NULL�ɐݒ�

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) *this[0].model.nNumVertexIndex,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this[0].model.pD3DIdxBuff,					// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);								// NULL�ɐݒ�

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (nNumBlockH + 2),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this[1].model.pD3DIdxBuff,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);

	// NULL�ɐݒ�
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		const float ANGLE_H = (D3DX_PI * 2.0f) / nNumBlockH;
		const float ANGLE_V = (D3DX_PI / 8.0f) / (nNumBlockV + 1);
		const float WIDTH = 1.0f / nNumBlockH;
		const float HEIGHT = 1.0f / nNumBlockV;
		float fLengthXZ;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < (nNumBlockV + 1); nCntV++)
		{
			fLengthXZ = cosf(ANGLE_V * nCntV) * fRadius;

			for (int nCntH = 0; nCntH < (nNumBlockH + 1); nCntH++, pVtx++)
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

				if (nCntV == nNumBlockV)
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
		this[0].model.pD3DVtxBuff->Unlock();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[1].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx->vtx.x = 0.0f;
		pVtx->vtx.y = sinf(ANGLE_V * (nNumBlockV + 1)) * fRadius * SKY_HEIGHT_RATE;
		pVtx->vtx.z = 0.0f;

		// �@���̐ݒ�
		pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx->tex.x = 0.01f;
		pVtx->tex.y = 0.01f;

		pVtx++;

		fLengthXZ = cosf(ANGLE_V * nNumBlockV) * fRadius;
		for (int nCntH = 0; nCntH < nNumBlockH; nCntH++, pVtx++)
		{
			// ���_���W�̐ݒ�
			pVtx->vtx.x = -sinf(ANGLE_H * nCntH) * fLengthXZ;
			pVtx->vtx.y = sinf(ANGLE_V * nNumBlockV) * fRadius * SKY_HEIGHT_RATE;
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
		this[1].model.pD3DVtxBuff->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntV = 0; nCntV < nNumBlockV; nCntV++)
		{
			if (nCntV != 0)
			{
				*pIdx = nCntV * (nNumBlockH + 1);
				pIdx++;
			}

			for (int nCntH = 0; nCntH < nNumBlockH + 1; nCntH++)
			{
				*pIdx = nCntV * (nNumBlockH + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (nNumBlockH + 1) + nCntH;
				pIdx++;
			}

			if (nCntV != (nNumBlockV - 1))
			{
				*pIdx = (nCntV + 1) * (nNumBlockH + 1);
				pIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		this[0].model.pD3DIdxBuff->Unlock();

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[1].model.pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntH = 0; nCntH < nNumBlockH + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// �C���f�b�N�X�f�[�^���A�����b�N����
		this[1].model.pD3DIdxBuff->Unlock();
	}

}

//=============================================================================
// �ď���������
//=============================================================================
void SKY::Reinit(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[0].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < (nNumBlockV + 1); nCntV++)
		{

			for (int nCntH = 0; nCntH < (nNumBlockH + 1); nCntH++, pVtx++)
			{
				// ���_�J���[�̐ݒ�
				pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		// ���_�f�[�^���A�����b�N����
		this[0].model.pD3DVtxBuff->Unlock();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this[1].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx++;

		for (int nCntH = 0; nCntH < nNumBlockH; nCntH++, pVtx++)
		{
			// ���_�J���[�̐ݒ�
			pVtx->diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N����
		this[1].model.pD3DVtxBuff->Unlock();
	}

	this[0].fRotY = 0.0f;
	this[0].time = 0.0f;
	this[1].fRotY = 0.0f;
	this[1].time = 0.0f;
}

//=============================================================================
// �I������
//=============================================================================
void SKY::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void SKY::Update(void)
{
	for (int nCntSky = 0; nCntSky < OBJECT_SKY_MAX; nCntSky++)
	{
		D3DXVECTOR3 rot=this[nCntSky].GetRot();
		rot.y += D3DX_PI * this[nCntSky].fRotY;

		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		this[nCntSky].SetRot(rot);
	}


		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[0].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntV = 0; nCntV < (this[0].nNumBlockV + 1); nCntV++)
			{
				for (int nCntH = 0; nCntH < (this[0].nNumBlockH + 1); nCntH++, pVtx++)
				{
					// ���_�J���[�̐ݒ�
						pVtx->diffuse = D3DXCOLOR(1.0f*this[0].time, 1.0f*this[0].time, 1.0f*this[0].time, 1.0f);
				}
			}

			// ���_�f�[�^���A�����b�N����
			this[0].model.pD3DVtxBuff->Unlock();

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[1].model.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �@���̐ݒ�
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
				pVtx->diffuse = D3DXCOLOR(1.0f*this[1].time, 1.0f*this[1].time, 1.0f*this[1].time, 1.0f);

			pVtx++;

			for (int nCntH = 0; nCntH < this[0].nNumBlockH; nCntH++, pVtx++)
			{
				// ���_�J���[�̐ݒ�
					pVtx->diffuse = D3DXCOLOR(1.0f*this[1].time, 1.0f*this[1].time, 1.0f*this[1].time, 1.0f);
			}

			// ���_�f�[�^���A�����b�N����
			this[1].model.pD3DVtxBuff->Unlock();
		}


		this[0].time += this[0].Addtime;
		this[1].time += this[1].Addtime;


	if (this[0].time >= 1.0 || this[0].time <= -1.0) this[0].Addtime *= -1.0f;
	if (this[1].time >= 1.0 || this[1].time <= -1.0) this[1].Addtime *= -1.0f;
}

//=============================================================================
// �`�揈��
//=============================================================================
void SKY::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DWORD dwSettingLighting, dwSettingCullmode;

	pDevice->GetRenderState(D3DRS_LIGHTING, &dwSettingLighting);
	pDevice->GetRenderState(D3DRS_CULLMODE, &dwSettingCullmode);

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = this[0].GetMatrix();
	D3DXVECTOR3 pos = this[0].GetPos();
	D3DXVECTOR3 rot = this[0].GetRot();

	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y,
		rot.x,
		rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, pos.x,
		pos.y,
		pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, this[0].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(this[0].model.pD3DIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this[0].model.pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this[0].model.nNumVertex, 0, this[0].model.nNumPolygon);


	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, this[1].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(this[1].model.pD3DIdxBuff);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (this[1].nNumBlockH + 2), 0, this[1].nNumBlockH);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O

	pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_LIGHTING, dwSettingLighting);
	pDevice->SetRenderState(D3DRS_CULLMODE, dwSettingCullmode);
}

