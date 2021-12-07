/**
* @file Model.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/

#include "../../../../../h/main.h"
#include "../../../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"

//�e�N�X�`���ǂݍ��ݏ���
void TEXTURE::LoadTexture(const char *FileName)
{
	D3DXCreateTextureFromFile(GetDevice(), FileName, &this->pD3DTexture);
}

//���_[�쐬]���ݒ�(3D)
//�쐬�̂� (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�
}

//���W�̂ݓ��́@������ (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�{�e�N�X�`�����́@������ (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�S���ړ��� (3D)
void VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_[�쐬]���ݒ�(3D�r���{�[�h)
//�쐬�̂� (3D�r���{�[�h)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�
}

//���W�̂ݓ��́@������ (3D�r���{�[�h)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�{�e�N�X�`�����́@������ (3D�r���{�[�h)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�S���ړ��� (3D�r���{�[�h)
void VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_[�쐬]���ݒ�(2D)
//�쐬�̂� (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

}

//���_�̂ݓ��́@������ (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�{�e�N�X�`�����́@������ (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}

		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�S���ړ��� (2D)
void VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
	const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_[�쐬]���ݒ�(3D)
void VTXBUFFER::MakeIdxVertex(const int VTXIdxNum)
{
	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	GetDevice()->CreateIndexBuffer(VTXIdxNum,						// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,													// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,														// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,													// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DIdxBuff,														// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);
}

//���_�T�C�Y�̐ݒ�@���E�㉺�Ώ� (3D) effect �Ŏg�p
void VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@���E�㉺��Ώ́@�䗦�Œ� (3D)
void VTXBUFFER::Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W���ڎw��@(pos����ver) (3D)
void VTXBUFFER::Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W�Ԑڎw��@(pos�L��ver) (3D)
void VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@���E�㉺�Ώ� (2D)
void VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@���E�㉺��Ώ́@�䗦�Œ� (2D)
void VTXBUFFER::Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W���ڎw��@(pos����ver) (2D)
void VTXBUFFER::Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W�Ԑڎw��@(pos�L��ver) (2D) damege �Ŏg�p
void VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		//pVtx += 4;
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�@���̐ݒ�
//�@������
void VTXBUFFER::Nor3D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �@���̐ݒ�
		pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�@������
void VTXBUFFER::Nor3D(const int Indx, const D3DXVECTOR3 Nor[4])
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �@���̐ݒ�
		pVtx[0].nor = Nor[0];
		pVtx[1].nor = Nor[1];
		pVtx[2].nor = Nor[2];
		pVtx[3].nor = Nor[3];
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
void VTXBUFFER::RHW2D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//=============================================================================
// ���b�V���J���[���Z�b�g
//=============================================================================
void VTXBUFFER::MeshColor(DWORD nNumPolygon, int CntPlayer)
{
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	this->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// ���ˌ��̐ݒ�
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[CntPlayer];
	}
	// ���_�f�[�^���A�����b�N����
	this->pD3DVtxBuff->Unlock();
	this->pD3DIdxBuff->Unlock();
}

//���_�J���[�̐ݒ�
//�J���[���� (3D)
void VTXBUFFER::Color3D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�J���[���� (3D)
void VTXBUFFER::Color3D(const int Indx, const D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�J���[���� (2D)
void VTXBUFFER::Color2D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�J���[���� (2D)
void VTXBUFFER::Color2D(const int Indx, const D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV�ݒ�(3D)
//UV���� (3D)
void VTXBUFFER::UV3D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@�����p (3D)
void VTXBUFFER::UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@���ړ��� (3D)
void VTXBUFFER::UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���� (2D)
void VTXBUFFER::UV2D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@�X�v���C�g�e�N�X�`���p (2D)
void VTXBUFFER::UV2D(const int Indx, const float PatternU, const float PatternV)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�@+1�͎��̃X�v���C�g�ł̒����p
		pVtx[0].tex = D3DXVECTOR2(PatternU, PatternV);
		pVtx[1].tex = D3DXVECTOR2((PatternU + 1), PatternV);
		pVtx[2].tex = D3DXVECTOR2(PatternU, (PatternV + 1));
		pVtx[3].tex = D3DXVECTOR2((PatternU + 1), (PatternV + 1));
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@�����p (2D)
void VTXBUFFER::UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@���ړ��� (2D)
void VTXBUFFER::UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//Color�擾(3D)
D3DXCOLOR VTXBUFFER::GetColor3D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}

//Color�擾(2D)
D3DXCOLOR VTXBUFFER::GetColor2D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}






//���_[�쐬]���ݒ�(3D)
//�쐬�̂� (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�
}

//���W�̂ݓ��́@������ (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�{�e�N�X�`�����́@������ (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�S���ړ��� (3D)
void My3D_VTXBUFFER::MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * VTXnum,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < VTXnum; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_[�쐬]���ݒ�(3D�r���{�[�h)
//�쐬�̂� (3D�r���{�[�h)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�
}

//���W�̂ݓ��́@������ (3D�r���{�[�h)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�{�e�N�X�`�����́@������ (3D�r���{�[�h)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�S���ړ��� (3D�r���{�[�h)
void My3D_VTXBUFFER::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
	const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// 3D���_�����@��{�I�Ƀr���{�[�h�����Ŏg�p
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// �@���̐ݒ�
			pVtx[0].nor = Nor[0];
			pVtx[1].nor = Nor[1];
			pVtx[2].nor = Nor[2];
			pVtx[3].nor = Nor[3];
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_[�쐬]���ݒ�(2D)
//�쐬�̂� (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

}

//���_�̂ݓ��́@������ (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�{�e�N�X�`�����́@������ (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}

		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�S���ړ��� (2D)
void My2D_VTXBUFFER::MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
	const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX * CntMax,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF,										// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DVtxBuff,							// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);										// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		for (int Cnt = 0; Cnt < CntMax; Cnt++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = col;
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
			pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
			pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
			pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		}
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_[�쐬]���ݒ�(3D)
void My3D_VTXBUFFER::MakeIdxVertex3D(const int VTXIdxNum)
{
	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	GetDevice()->CreateIndexBuffer(VTXIdxNum,						// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,													// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,														// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,													// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DIdxBuff,														// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);
}

//���_[�쐬]���ݒ�(2D)
void My2D_VTXBUFFER::MakeIdxVertex2D(const int VTXIdxNum)
{
	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	GetDevice()->CreateIndexBuffer(VTXIdxNum,						// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,													// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,														// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,													// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pD3DIdxBuff,														// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);
}

//���_�T�C�Y�̐ݒ�@���E�㉺�Ώ� (3D) effect �Ŏg�p
void My3D_VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@���E�㉺��Ώ́@�䗦�Œ� (3D)
void My3D_VTXBUFFER::Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W���ڎw��@(pos����ver) (3D)
void My3D_VTXBUFFER::Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W�Ԑڎw��@(pos�L��ver) (3D)
void My3D_VTXBUFFER::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeX, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeX, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@���E�㉺�Ώ� (2D)
void My2D_VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(HalfSizeX, -HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HalfSizeX, HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(HalfSizeX, HalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@���E�㉺��Ώ́@�䗦�Œ� (2D)
void My2D_VTXBUFFER::Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-LHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RHalfSizeX, -HHalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-LHalfSizeX, LHalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RHalfSizeX, LHalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W���ڎw��@(pos����ver) (2D)
void My2D_VTXBUFFER::Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4])
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(VTX[0].x, VTX[0].y, VTX[0].z);
		pVtx[1].vtx = D3DXVECTOR3(VTX[1].x, VTX[1].y, VTX[1].z);
		pVtx[2].vtx = D3DXVECTOR3(VTX[2].x, VTX[2].y, VTX[2].z);
		pVtx[3].vtx = D3DXVECTOR3(VTX[3].x, VTX[3].y, VTX[3].z);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�T�C�Y�̐ݒ�@�S���W�Ԑڎw��@(pos�L��ver) (2D) damege �Ŏg�p
void My2D_VTXBUFFER::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		//pVtx += 4;
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y - HalfSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(Pos.x - HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(Pos.x + HalfSizeX, Pos.y + HalfSizeY, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�@���̐ݒ�
//�@������
void My3D_VTXBUFFER::Nor3D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �@���̐ݒ�
		pVtx[0].nor = pVtx[1].nor = pVtx[2].nor = pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�@������
void My3D_VTXBUFFER::Nor3D(const int Indx, const D3DXVECTOR3 Nor[4])
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �@���̐ݒ�
		pVtx[0].nor = Nor[0];
		pVtx[1].nor = Nor[1];
		pVtx[2].nor = Nor[2];
		pVtx[3].nor = Nor[3];
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
void My2D_VTXBUFFER::RHW2D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//���_�J���[�̐ݒ�
//�J���[���� (3D)
void My3D_VTXBUFFER::Color3D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�J���[���� (3D)
void My3D_VTXBUFFER::Color3D(const int Indx, const D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�J���[���� (2D)
void My2D_VTXBUFFER::Color2D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = pVtx[1].diffuse = pVtx[2].diffuse = pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//�J���[���� (2D)
void My2D_VTXBUFFER::Color2D(const int Indx, const D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV�ݒ�(3D)
//UV���� (3D)
void My3D_VTXBUFFER::UV3D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@�����p (3D)
void My3D_VTXBUFFER::UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@���ړ��� (3D)
void My3D_VTXBUFFER::UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���� (2D)
void My2D_VTXBUFFER::UV2D(const int Indx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@�X�v���C�g�e�N�X�`���p (2D)
void My2D_VTXBUFFER::UV2D(const int Indx, const float PatternU, const float PatternV)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�@+1�͎��̃X�v���C�g�ł̒����p
		pVtx[0].tex = D3DXVECTOR2(PatternU, PatternV);
		pVtx[1].tex = D3DXVECTOR2((PatternU + 1), PatternV);
		pVtx[2].tex = D3DXVECTOR2(PatternU, (PatternV + 1));
		pVtx[3].tex = D3DXVECTOR2((PatternU + 1), (PatternV + 1));
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@�����p (2D)
void My2D_VTXBUFFER::UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart, vStart);
		pVtx[1].tex = D3DXVECTOR2(uEnd, vStart);
		pVtx[2].tex = D3DXVECTOR2(uStart, vEnd);
		pVtx[3].tex = D3DXVECTOR2(uEnd, vEnd);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//UV���́@���ړ��� (2D)
void My2D_VTXBUFFER::UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
	, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);
		// ���_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(uStart1, vStart1);
		pVtx[1].tex = D3DXVECTOR2(uEnd1, vStart1);
		pVtx[2].tex = D3DXVECTOR2(uStart2, vEnd2);
		pVtx[3].tex = D3DXVECTOR2(uEnd2, vEnd2);
		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
}

//Color�擾(3D)
D3DXCOLOR My3D_VTXBUFFER::GetColor3D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}

//Color�擾(2D)
D3DXCOLOR My2D_VTXBUFFER::GetColor2D(const int Indx)
{
	D3DXCOLOR col;
	{
		VERTEX_2D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		this->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (Indx * 4);

		col = pVtx[0].diffuse;

		// ���_�f�[�^���A�����b�N����
		this->pD3DVtxBuff->Unlock();
	}
	return col;
}

void My2D_VTXBUFFER::VtxBuffDraw2D(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, *vtxbuff, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
	pDevice->SetTexture(0, *texture);
	// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (StartVertex * 4), POLYGON_2D_NUM);
}

