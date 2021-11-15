/**
* @file objectclass.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
using namespace std;

#include "../../../h/main.h"
#include "../../../h/Debug/debugproc.h"
#include "../../../h/Other/input.h"
#include "../../../h/Object/Fade/fade.h"
#include "../../../h/Object/Camera/camera.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Collision/collision.h"
#include "../../../h/Map/field.h"
#include "../../../h/Map/wall.h"
#include "../../../h/Map/sky.h"
#include "../../../h/Object/Player/player.h"
#include "../../../h/Object/Shadow/shadow.h"
#include "../../../h/Object/Title/title.h"
#include "../../../h/Object/Result/result.h"
#include "../../../h/Object/Tutorial/tutorial.h"
#include "../../../h/Effect/effect.h"
#include "../../../h/Object/Bullet/bullet.h"
#include "../../../h/Effect/explosion.h"
#include "../../../h/Object/Rank/rank.h"
#include "../../../h/Object/CountDown/countdown.h"
#include "../../../h/Object/Item/item.h"
#include "../../../h/Effect/damege.h"
#include "../../../h/Object/Player/Status/status.h"
#include "../../../h/Object/Bullet/bulletprediction.h"
#include "../../../h/Object/Bullet/bulletgauge.h"
#include "../../../h/Object/Player/VitalGauge/vitalgauge.h"
#include "../../../h/net/netmatch.h"
#include "../../../h/net/sock.h"
#include "../../../h/Object/ObjectClass/objectclass.h"

int GAME_OBJECT::AllOBJCnt = 0;


#define MAX_SPEEDBUFF						(1.5f)																		//!< �X�s�[�h�o�t�ړ��ʍő�l(��)
#define MAX_SPEEDBUFFTIME					(120.0f*3)																	//!< �X�s�[�h�o�t�L������
#define	EXPLOSION_COLLISIONPOS_BUFFSIZE		(5.0f)																		//!< 2D���j��ǂɓ����������̕`�悷����W�𒲐�
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< ���f���T�C�Y
#define ITEM_MODEL_SIZE						(15.0f)																		//!< ���f���T�C�Y



void SetOjama(int type, int UsePlayer, PLAYER *p);

//�}�b�`�t���O
bool NetMatchFlag = false;
void SetNetMatchFlag(bool flag) { NetMatchFlag = flag; }

//�}�C�i���o�[
bool NetMyNumberFlag = false;
int NetMyNumber = -1;
void SetNetMyNumberFlag(bool flag) { NetMyNumberFlag = flag; }
void SetNetMyNumber(int num) { NetMyNumber = num; }
int GetNetMyNumber(void) { return NetMyNumber; }

//�A�C�e���t���O�@true�ŃA�C�e�����擾�����A�J�E���g�_�E���M�����󂯎��n�߂�
bool NetItemFlag = false;
void SetNetItemFlag(bool flag) { NetItemFlag = flag; }
bool GetNetItemFlag(void) { return NetItemFlag; }

//�l�b�g�t���O�@true�Ńl�b�g�ΐ�n�߂�
bool NetGameStartFlag = false;
void SetNetGameStartFlag(bool flag) { NetGameStartFlag = flag; }

//�l�b�g�ΐ�@���L�f�[�^�A�N�Z�X���t���O
bool NetShareDateFlag = false;
void SetNetShareDateFlag(bool flag) { NetShareDateFlag = flag; }
bool GetNetShareDateFlag(void) { return NetShareDateFlag; }

//�g�p����^�C�v��ύX����
//void iUseCheak::ChangeUse(eUse_Type UseType)
//{
//	this->Use(UseType);
//}

//�e�N�X�`���ǂݍ��ݏ���
void TEXTURE::LoadTexture(const char *FileName)
{
	D3DXCreateTextureFromFile(GetDevice(), FileName, &this->pD3DTexture);
}

//���_[�쐬]���ݒ�(3D)
//�쐬�̂� (3D)
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF)
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
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF)
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF)
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
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
void VTXBuffer::MakeIdxVertex(const int VTXIdxNum)
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
void VTXBuffer::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
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
void VTXBuffer::Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4])
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
void VTXBuffer::Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
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
void VTXBuffer::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY)
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
void VTXBuffer::Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY)
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
void VTXBuffer::Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4])
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
void VTXBuffer::Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos)
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
void VTXBuffer::Nor3D(const int Indx)
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
void VTXBuffer::Nor3D(const int Indx, const D3DXVECTOR3 Nor[4])
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
void VTXBuffer::RHW2D(const int Indx)
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
void VTXBuffer::Color3D(const int Indx)
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
void VTXBuffer::Color3D(const int Indx, const D3DXCOLOR col)
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
void VTXBuffer::Color2D(const int Indx)
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
void VTXBuffer::Color2D(const int Indx, const D3DXCOLOR col)
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
void VTXBuffer::UV3D(const int Indx)
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
void VTXBuffer::UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
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
void VTXBuffer::UV2D(const int Indx)
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
void VTXBuffer::UV2D(const int Indx, const float PatternU, const float PatternV)
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
void VTXBuffer::UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd)
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
void VTXBuffer::UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
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
D3DXCOLOR VTXBuffer::GetColor3D(const int Indx)
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
D3DXCOLOR VTXBuffer::GetColor2D(const int Indx)
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

void GAME_OBJECT::Generate()
{
	player = new PLAYER;								//ok 
	effect = new EFFECT;								//ok
	bullet = new BULLET;								//ok
	shadow = new SHADOW;								//ok
	countdown = new COUNTDOWN;							//ok
	tuto = new TUTO;									//ok
	netmatch = new NETMATCH;							//ok ���ǒ�
	status = new STATUS;								//ok
	bulletprediction = new BULLETPREDICTION;			//ok
	vitalgauge = new VITALGAUGE;						//ok
	bulletgauge = new BULLETGAUGE;						//ok
	damege = new DAMEGE;								//ok
	explosion = new EXPLOSION;							//ok
	item = new ITEM;									//ok
	rank = new RANK;									//ok
	result = new RESULT;								//ok
	title = new TITLE;									//ok
	field = new FIELD;									//ok
	sky = new SKY;										//ok
	wall = new WALL;									//ok
	fade = new FADE;									//ok

	mysocket = new MySOCKET;
	this->GameSceneFlagFunc(true);
	this->MultThreadFlagFunc(false);
}

void GAME_OBJECT::Init()
{
	//�l�b�g�ΐ�p
	NetMatchFlag = false;
	NetMyNumberFlag = false;
	NetMyNumber = -1;
	NetItemFlag = false;
	NetGameStartFlag = false;
	NetShareDateFlag = false;


	//�������̏������̂ق����Z���Ȃ�@�������K�v�ȏꍇ�͗�O���������邩�@������K�v�Ƃ��Ȃ��\���ɂ���
	//for (int CntOBJ = 0; CntOBJ < AllOBJCnt; CntOBJ++) { this[CntOBJ].Init(); }

	field->Init();
	player->Init(&field[0]);
	effect->Init();
	bullet->Init();
	shadow->Init();
	countdown->Init();
	tuto->Init();
	status->Init();
	bulletprediction->Init();
	vitalgauge->Init();
	bulletgauge->Init();
	damege->Init();
	explosion->Init();
	item->Init();
	rank->Init();
	result->Init();
	title->Init();
	sky->Init();
	wall->Init();
	mysocket->Init();

	this->MultThreadFlagFunc(false);
}

void GAME_OBJECT::InitNet()
{
	//�X�N���[����UI���Đݒ肵�Ă邾��
	status->InitNet(NetMyNumber);
	vitalgauge->InitNet(NetMyNumber);
	bulletgauge->InitNet(NetMyNumber);
	damege->InitNet(NetMyNumber);
	rank->InitNet(NetMyNumber);
}

void GAME_OBJECT::Delete()
{
	delete player;
	delete effect;
	delete bullet;
	delete shadow;
	delete countdown;
	delete tuto;
	delete netmatch;
	delete status;
	delete bulletprediction;
	delete vitalgauge;
	delete bulletgauge;
	delete damege;
	delete explosion;
	delete item;
	delete rank;
	delete result;
	delete title;
	delete field;
	delete sky;
	delete wall;
	delete fade;
	delete mysocket;
}

void GAME_OBJECT::Update()
{
	//���b�N����
	SetNetShareDateFlag(true);

	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();
	// ���͍X�V
	UpdateInput();
#ifdef _DEBUG

	//�ꎞ��~����
	if (GetKeyboardTrigger(DIK_F12) || IsButtonTriggered(0, BUTTON_SELECT)) stop++;
	if (GetKeyboardTrigger(DIK_F11) || IsButtonTriggered(0, BUTTON_L3))
	{
		MasterVolumeChange(1);
		this->Init();
		fade->SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_title01);
	}
#endif

	if (stop == false)
	{
		// �J�����̍X�V����
		UpdateCamera();

		switch (this->nScene)
		{
		case SCENE_TITLE:
			//�^�C�g���X�V
			title->Update(&this[0], &fade[0]);
			break;
		case SCENE_TUTORIAL:
			//�`���[�g���A���X�V
			tuto->Update(&this[0], &fade[0]);
			// map�X�V
			field->Update(&player[0], &item[0], &bullet[0], &explosion[0], &shadow[0]);
			sky->Update();

			//3D���
			player->Update(&effect[0], &bullet[0], &shadow[0], &fade[0], NetGameStartFlag, NetMyNumber);
			bullet->Update(&shadow[0], &effect[0]);
			bulletprediction->Update(&player[0]);
			effect->Update();
			explosion->Update();
			item->Update(&player[0], &shadow[0], NetGameStartFlag);
			shadow->Update();
			//
			CheckHit(0);
			//
			//2D���
			bulletgauge->Update(&player[0]);
			damege->Update();
			status->Update(&player[0]);
			break;

		case SCENE_GAMECOUNTDOWN:
			//�J�E���g�_�E���̍X�V
			countdown->Update(&this[0], NetGameStartFlag);
			countdown->AddCountdown(-1);
			break;

		case SCENE_GAME:
			// map�X�V
			field->Update(&player[0], &item[0], &bullet[0], &explosion[0], &shadow[0]);
			sky->Update();

			//�I�u�W�F�N�g�̍X�V
			player->Update(&effect[0], &bullet[0], &shadow[0], &fade[0], NetGameStartFlag, NetMyNumber);
			bullet->Update(&shadow[0], &effect[0]);
			bulletprediction->Update(&player[0]);
			effect->Update();
			explosion->Update();
			item->Update(&player[0], &shadow[0], NetGameStartFlag);
			shadow->Update();

			//�����蔻��
			CheckHit(1);

			//2D���
			bulletgauge->Update(&player[0]);
			damege->Update();
			status->Update(&player[0]);
			vitalgauge->Update(&player[0], &rank[0], NetGameStartFlag, NetMyNumber);
			break;

		case SCENE_NETMATCH:
			netmatch->Update(&this[0], &fade[0]);
			//�}�b�`���O��
			if (NetMatchFlag == false)
			{
				this->mysocket->NetMatch();
			}
			//�}�C�i���o�[�擾��
			if (NetMyNumberFlag == false)
			{
				if (NetMyNumber >= -1 && NetMyNumber <= 3) this->mysocket->NetMyNumberGet();
			}
			//�A�C�e�����擾��
			else if (NetItemFlag==false)
			{
				this->mysocket->NetItemGet(item);
			}
			//�J�E���g�_�E���M���҂���
			if(NetMatchFlag == true && NetMyNumberFlag == true && NetItemFlag == true&& NetGameStartFlag == false)
			{
				this->InitNet();
				this->mysocket->NetCountdown();
			}
			//�X�^�[�g�t���O�������Ă��ĐM����ON�ɂȂ�����J�E���g�_�E���J�n
			if (NetGameStartFlag == true)
			{
				fade->SetFade(FADE_OUT, SCENE_NETGAMECOUNTDOWN, SOUND_LABEL_BGM_boss01);
				SourceVolumeChange(0, SOUND_LABEL_BGM_boss01);
			}

			//�Q�[���I������
			//if (GetGameLoop() == true)
			//{
			//	SetEndGame(true);
			//	this->MultThreadFlagFunc(false);
			//	this->GameSceneFlagFunc(false);
			//}
			break;
		case SCENE_NETGAMECOUNTDOWN:
			//�J�E���g�_�E���̍X�V
			countdown->Update(&this[0], NetGameStartFlag);
			countdown->AddCountdown(-1);
			break;
		case SCENE_NETGAME:
			//�p�P�b�g�̓}���`�X���b�h�Ń��b�N����Ă��Ȃ��Ƃ�(Draw���̓��b�N����Ă���)�ɏ�ɔ��f������

			// map�X�V
			field->Update(&player[0], &item[0], &bullet[0], &explosion[0], &shadow[0]);//�p�P�b�g�L��
			sky->Update();

			//�I�u�W�F�N�g�̍X�V
			player->Update(&effect[0], &bullet[0], &shadow[0], &fade[0], NetGameStartFlag, NetMyNumber);//�p�P�b�g�L��
			bullet->Update(&shadow[0], &effect[0]);//�p�P�b�g�L��
			bulletprediction->Update(&player[0]);
			effect->Update();//�p�P�b�g�L��
			explosion->Update();//�p�P�b�g�L��
			item->Update(&player[0], &shadow[0], NetGameStartFlag);//�p�P�b�g�L��
			shadow->Update();//�p�P�b�g�L��

			//�����蔻��
			CheckHit(1);

			//2D���
			bulletgauge->Update(&player[0]);
			damege->Update();//�p�P�b�g�L��
			status->Update(&player[0]);
			vitalgauge->Update(&player[0], &rank[0], NetGameStartFlag, NetMyNumber);
			break;
		case SCENE_RESULT:
			//���U���g�̍X�V
			result->Update(&this[0], &fade[0]);
			break;
		}
		// �t�F�[�h����
		fade->Update(&this[0]);
	}
}

void GAME_OBJECT::Draw()
{
	//�l�l���̉�ʕ����ݒ�
	D3DVIEWPORT9 vp[]
	{
		{DWORD(0),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(0),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
	};
	D3DVIEWPORT9 VpMaster{ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(pD3DDevice->BeginScene()))
	{
		// ��ʑJ��
		switch (this->nScene)
		{
		case SCENE_TITLE:
			//�^�C�g���`��
			title->Draw();
			break;
		case SCENE_TUTORIAL:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

				if (player->PlayerPara[CntPlayer].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

				// �J�����̐ݒ�
				SetCamera(CntPlayer);

				// map�`��
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D���
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], CntPlayer);
				explosion->Draw(CntPlayer);
				effect->Draw(CntPlayer);
				shadow->Draw();

				//2d��ʏ�
				damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				tuto->Draw();
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;
		case SCENE_GAMECOUNTDOWN:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);
				// �J�����̐ݒ�
				SetCamera(CntPlayer);

				// map�`��
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D���
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], CntPlayer);
				explosion->Draw(CntPlayer);
				effect->Draw(CntPlayer);
				shadow->Draw();

				//2d��ʏ�
				damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
			}
			pD3DDevice->SetViewport(&VpMaster);
			countdown->Draw();
			break;
		case SCENE_GAME:
			for (int CntPlayer = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); CntPlayer < vpCnt; CntPlayer++)
			{
				pD3DDevice->SetViewport(&vp[CntPlayer]);
				pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(1, 1, 1, 1), 1.0f, 0);

				bool puse = player->iUseType[CntPlayer].Use();
				if (puse == true)
				{
					if (player->PlayerPara[CntPlayer].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
					else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					// map�`��
					field->Draw();
					sky->Draw();
					wall->Draw();

					//3D���
					player->Draw();
					item->Draw();
					bulletprediction->Draw(&player[0], CntPlayer);
					explosion->Draw(CntPlayer);
					effect->Draw(CntPlayer);
					shadow->Draw();

					//2d��ʏ�
					damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
				}
				else
				{
					pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(CntPlayer);

					// map�`��
					field->Draw();
					sky->Draw();
					wall->Draw();

					//3D���
					player->Draw();
					item->Draw();
					bulletprediction->Draw(&player[0], CntPlayer);
					explosion->Draw(CntPlayer);
					effect->Draw(CntPlayer);
					shadow->Draw();

					//2d��ʏ�
					damege->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					status->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					vitalgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					bulletgauge->Draw(NetGameStartFlag, NetMyNumber, CntPlayer);
					rank->Draw(NetGameStartFlag, NetMyNumber);
				}
			}
			pD3DDevice->SetViewport(&VpMaster);
			break;


		case SCENE_NETMATCH:
			netmatch->Draw();

			break;
		case SCENE_NETGAMECOUNTDOWN:
		{
			// �J�����̐ݒ�
			SetCamera(NetMyNumber);

			// map�`��
			field->Draw();
			sky->Draw();
			wall->Draw();

			//3D���
			player->Draw();
			item->Draw();
			bulletprediction->Draw(&player[0], NetMyNumber);
			explosion->Draw(0);
			effect->Draw(0);
			shadow->Draw();

			//2d��ʏ�
			damege->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			status->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			vitalgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			bulletgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			countdown->Draw();
			break;
		}
		case SCENE_NETGAME:
		{
			bool puseNet = player->iUseType[NetMyNumber].Use();
			if (puseNet == true)
			{
				if (player->PlayerPara[NetMyNumber].ItemPara.KiriSignal == true) pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
				else pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

				// �J�����̐ݒ�
				SetCamera(NetMyNumber);

				// map�`��
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D���
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], NetMyNumber);
				explosion->Draw(NetMyNumber);
				effect->Draw(NetMyNumber);
				shadow->Draw();

				//2d��ʏ�
				damege->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				status->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
			}
			else
			{
				pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

				// �J�����̐ݒ�
				SetCamera(NetMyNumber);

				// map�`��
				field->Draw();
				sky->Draw();
				wall->Draw();

				//3D���
				player->Draw();
				item->Draw();
				bulletprediction->Draw(&player[0], NetMyNumber);
				explosion->Draw(0);
				effect->Draw(0);
				shadow->Draw();

				//2d��ʏ�
				damege->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				status->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				vitalgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				bulletgauge->Draw(NetGameStartFlag, NetMyNumber, NetMyNumber);
				rank->Draw(NetGameStartFlag, NetMyNumber);
			}

			break;
		}
		case SCENE_RESULT:
			//���U���g�`��
			result->Draw();
			break;
		}

		// �t�F�[�h�`��
		fade->Draw();

#ifdef _DEBUG
		// �f�o�b�O�\��
		DrawTextType();
		DrawDebugProc();
#endif
	}
	// Direct3D�ɂ��`��̏I��
	pD3DDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	pD3DDevice->Present(NULL, NULL, NULL, NULL);


	//�X���b�h�Ԃ̋��L�ϐ�������
	//m.unlock();

	//��������
	SetNetShareDateFlag(false);
}

void GAME_OBJECT::CheckHit(int scene)
{
	//�v���C���[�ɑ΂��铖���蔻��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (player->iUseType[CntPlayer].Use() == YesUseType1)
		{
			//�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 ppos = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
			D3DXVECTOR3 poldpos = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].OldPos();

			//�ۑ�
			player->PlayerPara[CntPlayer].StandardPara.OldVital = player->PlayerPara[CntPlayer].StandardPara.Vital;

			//�v���C���[�Ε�
			for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
			{

				D3DXVECTOR3 wpos = wall->Transform[CntWall].Pos();
				switch (CntWall)
				{
				case 0:
					if (ppos.z + PLAYER_MODEL_SIZE >= wpos.z) ppos.z = poldpos.z;
					break;
				case 1:
					if (ppos.x - PLAYER_MODEL_SIZE <= wpos.x) ppos.x = poldpos.x;
					break;
				case 2:
					if (ppos.x + PLAYER_MODEL_SIZE >= wpos.x) ppos.x = poldpos.x;
					break;
				case 3:
					if (ppos.z - PLAYER_MODEL_SIZE <= wpos.z) ppos.z = poldpos.z;
					break;
				default:
					break;
				}
				//�I�u�W�F�N�g�l��������
				player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(ppos);
			}

			//�v���C���[�o���b�g�΃v���C���[�A�ǁA��
			for (int CntPlayerBullet = 0; CntPlayerBullet < OBJECT_BULLET_MAX; CntPlayerBullet++)
			{
				//�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 bpos = bullet->Transform[CntPlayerBullet].Pos();
				eUse_Type buse = bullet->iUseType[CntPlayerBullet].Use();

				if (buse == YesUseType1)
				{
					//�΃v���C���[
					if (bullet->BulletPara[CntPlayerBullet].UsePlayerType != CntPlayer)
					{
						if (CollisionBC(ppos, PLAYER_MODEL_SIZE, bpos, BULLET_MODEL_SIZE))
						{
							// �G�t�F�N�g�����̐���
							effect->SetInstance(bpos, VEC3_ALL0,
								PLAYER_COLOR[bullet->BulletPara[CntPlayerBullet].UsePlayerType], 150.0f, 150.0f, 40);
							if (scene == 1)
							{
								player->PlayerPara[CntPlayer].StandardPara.Vital -= PLAYER_ATTACK_NORMAL;
							}
							//��ʃ_���[�W�G�t�F�N�g
							damege->iUseType[CntPlayer].Use(YesUseType1);
							damege->DamegePara[CntPlayer].time = 0;
							damege->DamegePara[CntPlayer].alpha = 0;

							// �o���b�g�j��
							bullet->ReleaseInstance(CntPlayerBullet,shadow);

							// SE�Đ�
							PlaySound(SOUND_LABEL_SE_attack02);
							break;
						}
					}
					//�Ε�
					for (int CntWall = 0; CntWall < OBJECT_WALL_MAX; CntWall++)
					{
						D3DXVECTOR3 wpos = wall->Transform[CntWall].Pos();
						switch (CntWall)
						{
							//�t���O���ĂāA�֐��Ăяo�����������炷
						case 0:
							if (bpos.z >= wpos.z)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z- EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 1:
							if (bpos.x <= wpos.x)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x + EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 2:
							if (bpos.x >= wpos.x)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x - EXPLOSION_COLLISIONPOS_BUFFSIZE, bpos.y, bpos.z);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						case 3:
							if (bpos.z <= wpos.z)
							{
								// �����̐���
								//D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bpos.y, bpos.z + EXPLOSION_COLLISIONPOS_BUFFSIZE);
								//SetExplosion(ExploPos, 40.0f, 40.0f, EXPLOSIONTYPE_BULLET_PLAYER, PLAYER_COLOR[b[CntPlayerBullet].UsePlayerType]);
								// �o���b�g�j��
								bullet->ReleaseInstance(CntPlayerBullet, shadow);
								// SE�Đ�
								//PlaySound(SOUND_LABEL_SE_damage);
							}
							break;
						default:
							break;
						}
						if (buse == NoUse) break;
					}
				}
			}

			//�v���C���[�΃A�C�e��
			for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
			{
				//�I�u�W�F�N�g�l�ǂݍ���
				D3DXVECTOR3 ipos = item->Transform[CntItem].Pos();
				bool iuse = item->iUseType[CntItem].Use();
				if (iuse == false || item->ItemParaAll[CntItem].GettingSignal == true || item->ItemParaAll[CntItem].GettingSignalEnd == true) continue;
				if (CollisionBC(ppos, PLAYER_MODEL_SIZE, ipos, ITEM_MODEL_SIZE))
				{
					switch (item->ItemParaAll[CntItem].eType)
					{
					case ITEM_TYPE_TIKEI:
						//�n�`�ό`�M��ON
						if (NetGameStartFlag==true&& CntPlayer== NetMyNumber)
						{
							this->field->FieldPara.ItemGetPlayerNum = CntPlayer;
							//�����_���֐���seed�l�����߂�
							this->field->FieldPara.TikeiSeed = (rand() % 1000000);
							//�ʐM���Ă���Ƃ��͂��̃V�[�h�l�𓯊������āA��������Ƃɓ����n�`�𐶐�����
							//srand(this[0].field->TikeiSeed);
							//NetSetTikeiSeed(this[0].field->TikeiSeed, this[0].field->GetPlayerNum);
						}
						else if(NetGameStartFlag == false)
						{
							this->field->SetFieldInterPolationFieldType(FIELD_TYPE_PLAYERADVANTAGE, CntPlayer, &item[0]);
							//SetFieldInterPolationFieldType(0);
							PlaySound(SOUND_LABEL_SE_enter03);
							PlaySound(SOUND_LABEL_SE_quake);

						}
						break;
					case ITEM_TYPE_LIFE:
						//�̗͉�
						player->PlayerPara[CntPlayer].StandardPara.Vital += PLAYER_ATTACK_NORMAL;
						if (player->PlayerPara[CntPlayer].StandardPara.Vital >= PLAYER_VITAL_MAX) player->PlayerPara[CntPlayer].StandardPara.Vital = PLAYER_VITAL_MAX;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_SENSYA:
						//�ʏ탂�f���Ȃ烂�[�t�B���O�M��ON
						if (player->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_NORMAL)
						{
							//���[�t�B���O�J�n�M���A���f���^�C�v�A���[�t�B���O���M���A���[�t�B���O���ԁA���[�t�B���O�I���J�E���g�̃Z�b�g
							player->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							player->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//���[�t�B���O�I���ω����ɃA�C�e�����E�������͂�����x���[�t�B���O������
						else if (player->PlayerPara[CntPlayer].MorphingPara.MorphingStart == false && player->PlayerPara[CntPlayer].MorphingPara.MorphingTime <= 0.0f)
						{
							player->PlayerPara[CntPlayer].MorphingPara.MorphingStart = true;
							player->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
							player->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						//�A�^�b�N���f�����ɃA�C�e���E�����玞�ԉ���
						else
						{
							player->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
						}
						player->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = true;
						PlaySound(SOUND_LABEL_SE_rap1);
						break;
					case ITEM_TYPE_BULLET:
						player->PlayerPara[CntPlayer].BulletPara.BulletStock = BULLET_MAX_STOCK;
						player->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_SPEEDUP:
						player->PlayerPara[CntPlayer].ItemPara.SpeedBuff = MAX_SPEEDBUFF;
						player->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime = MAX_SPEEDBUFFTIME;
						player->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = true;
						PlaySound(SOUND_LABEL_SE_speed);
						break;
					case ITEM_TYPE_CAMERA:
						SetOjama(ITEM_TYPE_CAMERA, CntPlayer, &player[0]);
						PlaySound(SOUND_LABEL_SE_enter03);
						break;
					case ITEM_TYPE_KIRI:
						SetOjama(ITEM_TYPE_KIRI, CntPlayer, &player[0]);
						PlaySound(SOUND_LABEL_SE_kiri);
						break;
					default:

						break;
					}
					PlaySound(SOUND_LABEL_SE_kurukuru);
					item->ItemParaAll[CntItem].GettingSignal = true;
					item->ItemParaAll[CntItem].GetPlayerType = CntPlayer;
					item->ItemParaAll[CntItem].NetGetItemFlag = true;
				}
			}
		}
	}
}

void GAME_OBJECT::PackectAll(void)
{
	while (GetEndGame() == false)
	{
		mysocket->Packet(player, item, field, bullet,shadow);
	}
}

void SetOjama(int type, int UsePlayer, PLAYER *p)
{
	//�J�������]
	if (type == ITEM_TYPE_CAMERA)
	{
		//�Ώۃv���C���[�̂݃J�������]
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.BackCameraTime = 0.0f;
		}
	}
	//�t�H�O
	else if (type == ITEM_TYPE_KIRI)
	{
		//�Ώۃv���C���[�̂݃t�H�O����
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			if (UsePlayer == CntPlayer) continue;
			p->PlayerPara[CntPlayer].ItemPara.KiriSignal = true;
			p->PlayerPara[CntPlayer].ItemPara.KiriTime = 0.0f;
		}
	}
}





