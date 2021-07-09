/**
* @file item.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/map/field.h"
#include "../../h/other/sound.h"
#include "../../h/object/player.h"
#include "../../h/library.h"
#include "../../h/object/item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_ROTATE_ITEM_LO	(D3DX_PI * 0.01f)			// ��]���x�@�ᑬ
#define	VALUE_ROTATE_ITEM_HI	(D3DX_PI * 0.1f)			// ��]���x�@����
#define	APPROADHING_RATE		(0.25f)						// �v���C���[�ɋ߂Â��Ă������� (���� * APPROADHING_RATE)
#define	VALUE_FALLSPEED_ITEM	(2.0f)						// �������x
#define	ITEM_RADIUS				(20.0f)						// ���a
#define DROP_ITEM_MAX						(20)			//!< �t�B�[���h�ɗ����Ă�A�C�e���̐�
#define DROP_ITEM_CHARGE_ADDTIME			(1.0f)			//!< �A�C�e�������X�|�[�������鎞�̉��Z�^�C��
#define DROP_ITEM_CHARGE_CNT				(60.0f)			//!< �A�C�e�������X�|�[�������鎞�̏��v�^�C��

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
ITEM::ITEM(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//�J�E���g���[�v(�A�C�e���̎��)�@
	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		// �e�N�X�`���̓ǂݍ���
		this->tex[nCntItemType].LoadTexture(this->c_aFileNameTex[nCntItemType]);
	}

	// X�t�@�C���̓ǂݍ��݃f�[�^�Q
	LPD3DXMESH	Mesh;
	DWORD nv, nvi, nm, np;
	LPD3DXBUFFER BuffMat;

	// X�t�@�C���̓ǂݍ���
	// ���f���͕�����ނ��邪�A�e�N�X�`���ȊO�̃��f���f�[�^�͑S�ē������߁A���f���̓ǂݍ��݂͈�x�����ɂ��Ă���
	LoadMesh(this->c_aFileNameModel[0], &BuffMat,
		&nm, &Mesh, &nv, &np, &nvi, NULL);

	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)�@
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		//���_�̍쐬
		LPDIRECT3DVERTEXBUFFER9 *VtxBuff;
		LPDIRECT3DINDEXBUFFER9	*IdxBuff;
		this->vtx[CntItem].MakeVertex3D(nv,FVF_VERTEX_3D);
		this->vtx[CntItem].MakeIdxVertex(nvi);

		//�o�b�t�@�̎擾���甽�f
		VtxBuff = this->vtx[CntItem].VtxBuff();
		IdxBuff = this->vtx[CntItem].IdxBuff();
		Mesh->GetVertexBuffer(VtxBuff);
		Mesh->GetIndexBuffer(IdxBuff);
		this->vtx[CntItem].VtxBuff(*VtxBuff);
		this->vtx[CntItem].IdxBuff(*IdxBuff);

		//�g�p�̐ݒ�
		this->iUseType[CntItem].ChangeUse(NoUse);
	}

	//���f���f�[�^���f
	this->model.SetNumMat(nm);
	this->model.SetMat(BuffMat);
	this->model.SetNumVertex(nv);
	this->model.SetNumPolygon(np);
	this->model.SetNumVertexIndex(nvi);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
ITEM::~ITEM(void)
{
	//�J�E���g���[�v(�A�C�e���̎��)
	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++) {
		//�e�N�X�`�����
		this->tex[nCntItemType].~TEXTURE();
		//���f�����
		this->model.~ModelAttribute();
	}

	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)�@
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		//���_���
		this->vtx[CntItem].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void ITEM::Init(void)
{
	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)
	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		//�������ݒ�
		this->Transform[nCntItem].Scl(this->ItemParaOne.MaxSize);
		this->ItemParaAll[nCntItem].nIdxShadow = -1;
		this->ItemParaAll[nCntItem].eType = ITEMTYPE_NONE;
		this->ItemParaAll[nCntItem].NetUse = false;
		this->ItemParaAll[nCntItem].NetGetItemFlag = false;
		this->ItemParaAll[nCntItem].NetGetItemFlagOld = false;
		this->iUseType[nCntItem].Use(NoUse);
	}

	//!< ���݃h���b�v���Ă��鐔�@�h���b�v������܂ł̃^�C�}�[
	this->ItemParaOne.GoukeiDrop = 0;
	this->ItemParaOne.Droptime = 0;
}


////=============================================================================
//// �ď���������
////=============================================================================
//void ITEM::Reinit(void)
//{
//	//������ �S�ăZ�b�g���Ȃ�
//	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
//	{
//		this[nCntItem].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
//		this[nCntItem].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetQrot(0.0f);
//		this[nCntItem].NetUse = false;
//		this[nCntItem].Use(false);
//		this[nCntItem].Droptime = 0.0f;
//		this[nCntItem].nIdxShadow = -1;
//		this[nCntItem].nType = -1;
//		this[nCntItem].GettingSignal = false;
//		this[nCntItem].GettingSignalEnd = false;
//		this[nCntItem].CollisionFieldEnd = false;
//		this[nCntItem].NetGetItemFlag = false;
//		this[nCntItem].NetGetItemFlagOld = false;
//	}
//
//	//������2�@���ׂăZ�b�g
//	/*
//	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
//	{
//		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
//		int x = rand() % 2;
//		int z = rand() % 2;
//		if (x == 1) pos.x *= -1;
//		if (z == 1) pos.z *= -1;
//		int ItemTypeNum = rand() % ITEMTYPE_MAX;
//		//���C�t�A�J�����A���A�C�e���̎��͂�����x���I
//		if (ItemTypeNum == ITEMTYPE_LIFE && ItemTypeNum == ITEMTYPE_CAMERA && ItemTypeNum == ITEMTYPE_KIRI) ItemTypeNum = rand() % ITEMTYPE_MAX;
//		this[0].SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemTypeNum);
//		//this[0].SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
//		this[nCntItem].Use(true);
//	}
//	*/
//
//	//this[0].GoukeiDrop = DROP_ITEM_MAX;
//	this[0].GoukeiDrop = 0;
//}
//


////=============================================================================
//// �ď���������
////=============================================================================
//void ITEM::ReinitNet(void)
//{
//	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
//	{
//		this[nCntItem].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
//		this[nCntItem].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetQrot(0.0f);
//		this[nCntItem].Use(false);
//		this[nCntItem].Droptime = 0.0f;
//		this[nCntItem].nIdxShadow = -1;
//		this[nCntItem].nType = -1;
//		this[nCntItem].GettingSignal = false;
//		this[nCntItem].GettingSignalEnd = false;
//		this[nCntItem].CollisionFieldEnd = false;
//		this[nCntItem].NetGetItemFlag = false;
//		this[nCntItem].NetGetItemFlagOld = false;
//	}
//}
//


//=============================================================================
// �X�V����
//=============================================================================
void ITEM::Update(PLAYER_HONTAI *p, bool NetGameStartFlag)
{
	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		this->ItemParaAll[nCntItem].NetGetItemFlagOld = this->ItemParaAll[nCntItem].NetGetItemFlag;
		this->ItemParaAll[nCntItem].NetGetItemFlag = false;
		bool use = this->iUseType[nCntItem].Use();
		if (use == true)
		{
			//-------------------------------------------�I�u�W�F�N�g�̒l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[nCntItem].Pos();
			D3DXVECTOR3 rot = this->Transform[nCntItem].Rot();
			D3DXVECTOR3 FieldNorVec = this->PostureVec[nCntItem].FNVec();
			D3DXVECTOR3 FieldNorUpNorCross = this->PostureVec[nCntItem].FNUNCross();
			float Qrot = this->PostureVec[nCntItem].Qrot();

			//�t�B�[���h�ɗ����Ă�Ƃ��͂��邭���]������
			rot.y += VALUE_ROTATE_ITEM_LO;

			//���X�ɗ����Ă���
			if (this->ItemParaAll[nCntItem].CollisionFieldEnd != true)
			{
				pos.y -= VALUE_FALLSPEED_ITEM;
			}

			//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
			D3DXVECTOR3 Upvec = D3DXVECTOR3(0.0, 1.0f, 0.0f);
			D3DXVec3Cross(&FieldNorUpNorCross, &FieldNorVec, &Upvec);
			float kakezan = D3DXVec3Dot(&FieldNorVec, &Upvec);
			if (kakezan != 0.0f)
			{
				float cossita = kakezan /
					sqrtf(FieldNorVec.x*FieldNorVec.x +
						FieldNorVec.y *FieldNorVec.y +
						FieldNorVec.z * FieldNorVec.z);
				Qrot = acosf(cossita);
			}
			else Qrot = 0.0f;

			//-------------------------------------------�I�u�W�F�N�g�̒l��������
			this->Transform[nCntItem].Pos(pos);
			this->Transform[nCntItem].Rot(rot);
			this->PostureVec[nCntItem].FNUNCross(FieldNorUpNorCross);
			this->PostureVec[nCntItem].Qrot(Qrot);
		}

		//���ꂩ���A�C�e�����擾�����炻�̃v���C���[�ɋ߂Â�����
		if (this->ItemParaAll[nCntItem].GettingSignal == true)
		{
			GettingItem(nCntItem,p);
		}

	}

	//���[�J���ΐ�Ȃ炱���Ń��X�|�[������
	if (NetGameStartFlag == false)
	{
		//�A�C�e���𕜊������鐧��B
		for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
		{
			//�A�C�e�����ő吔�܂Ŏg�p���Ă�����X���[
			if (this->ItemParaOne.GoukeiDrop > DROP_ITEM_MAX) break;
			bool use = this->iUseType[nCntItem].Use();
			if (use == false)
			{
				//�^�C�}�[�����Z���āA��萔�ȏ�ɂȂ�ƃA�C�e�����o��������
				this->ItemParaOne.Droptime += DROP_ITEM_CHARGE_ADDTIME;
				if (this->ItemParaOne.Droptime >= DROP_ITEM_CHARGE_CNT)
				{
					//�o������
					//���W�w��
					D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
					//�����_���Ő����𔽓]������
					int x = rand() % 2;
					int z = rand() % 2;
					if (x == 1) pos.x *= -1;
					if (z == 1) pos.z *= -1;
					//�����_���ŃA�C�e���̎�ޑI��
					int ItemNum = rand() % ITEMTYPE_MAX;
					//�m���ݒ�̂��߁@���C�t�A�J�����A���A�C�e���̎��͂�����x���I
					if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
					//�C���X�^���X�Z�b�g�@�g�p����
					this[0].SetInstance(pos, D3DXVECTOR3(ITEM_BIG_SCL, ITEM_BIG_SCL, ITEM_BIG_SCL), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
					this->ItemParaAll[nCntItem].CollisionFieldEnd = false;
					this->ItemParaOne.Droptime = 0.0f;
					this->ItemParaOne.GoukeiDrop++;
					PlaySound(SOUND_LABEL_SE_nyu);
				}
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void ITEM::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)�@�g�p�̐ݒ�
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		//�g�p���Ă���Ε`��
		bool use = this->iUseType[nCntItem].Use();
		if (use == true)
		{
			//���[���h�ϊ��ɕK�v�ȃf�[�^�Q�ƃ}�e���A�����
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			D3DXQUATERNION q(0, 0, 0, 1);
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);

			D3DXMATRIX mtxWorldItem;
			//-------------------------------------------�I�u�W�F�N�g�̒l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[nCntItem].Pos();
			D3DXVECTOR3 rot = this->Transform[nCntItem].Rot();
			D3DXVECTOR3 scl = this->Transform[nCntItem].Scl();
			D3DXVECTOR3 PlayerUpToFieldNorVec = this->PostureVec[nCntItem].FNUNCross();
			float Qrot = this->PostureVec[nCntItem].Qrot();

			//q=(rotVecAxis�@��)*(g_Player.rot��])  -���L���H
			D3DXQuaternionRotationAxis(&q, &PlayerUpToFieldNorVec, -Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &q);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldItem);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxRot);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxQ);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldItem);

			// �}�e���A���ݒ�
			pDevice->GetMaterial(&matDef);
			LPD3DXBUFFER mat;
			mat = *this->model.GetMat();
			pD3DXMat = (D3DXMATERIAL*)mat->GetBufferPointer();

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
			for (int nCntMat = 0; nCntMat < (int)this->model.GetNumMat(); nCntMat++)
			{
				// �f�o�C�X�Ƀ}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat->MatD3D);

				// �f�o�C�X�Ƀe�N�X�`���̐ݒ�
				pDevice->SetTexture(0, this->tex[this->ItemParaAll[nCntItem].eType].Texture());
				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);
				// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetStreamSource(0, *this->vtx[nCntItem].VtxBuff(), 0, sizeof(VERTEX_3D));
				// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetIndices(*this->vtx[nCntItem].IdxBuff());
				//�`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this->model.GetNumVertex(), 0, this->model.GetNumPolygon());
			}
		}
	}
	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �A�C�e���̐ݒ�
//=============================================================================
void ITEM::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType)
{
	//�J�E���g���[�v(�A�C�e���̃C���X�^���X��)
	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		//�g�p����Ă��Ȃ��C���X�^���X�����݂���Έ�����g�p(�o��)����
		bool use = this->iUseType[nCntItem].Use();
		if (use != true)
		{
			//��{�p�����[�^�̐ݒ�
			this->Transform[nCntItem].Pos(pos);
			this->Transform[nCntItem].Scl(scl);
			this->Transform[nCntItem].Rot(rot);
			this->iUseType[nCntItem].Use(YesUse);
			this->ItemParaAll[nCntItem].eType = eType;
			break;
		}
	}
}

//=============================================================================
// �A�C�e���̍폜
//=============================================================================
void ITEM::ReleaseInstance(int nIdxItem)
{
	//�w��͈͊O�̐��l�Ȃ�X���[�@���݂��鐔�l�Ȃ�g�p(�`��)���Ȃ�
	if(nIdxItem >= 0 && nIdxItem < OBJECT_ITEM_MAX)
	{
		//��{�p�����[�^�̐ݒ�
		this->iUseType[nIdxItem].Use(NoUse);
		this->ItemParaAll[nIdxItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// �A�C�e�����擾�����v���C���[�֋߂Â���֐�
//=============================================================================
void ITEM::GettingItem(int nIdxItem, PLAYER_HONTAI *p)
{
	//�v���C���[�֋߂Â��鏈��
	if (this->ItemParaAll[nIdxItem].GettingSignalEnd == false)
	{
		//-------------------------------------------�I�u�W�F�N�g�̒l�ǂݍ���
		D3DXVECTOR3 ipos = this->Transform[nIdxItem].Pos();
		D3DXVECTOR3 irot = this->Transform[nIdxItem].Rot();
		D3DXVECTOR3 iscl = this->Transform[nIdxItem].Scl();

		//���邭���]������
		irot.y += VALUE_ROTATE_ITEM_HI;
		
		//�v���C���[�ƃA�C�e���̋������v�Z���߂Â���
		D3DXVECTOR3 ppos = p->Transform[this->ItemParaAll[nIdxItem].GetPlayerType].Pos();

		D3DXVECTOR3 distance = ppos - ipos;
		distance *= APPROADHING_RATE;
		ipos += distance;
		//�X�P�[�������X�ɏ��������Ă���
		iscl -= D3DXVECTOR3(ITEM_SMALL_SCL, ITEM_SMALL_SCL, ITEM_SMALL_SCL);
		//���l��菬�����Ȃ�΋߂Â��������I��������
		if (iscl.x <= ITEM_DELETE_SCL)
		{
			this->ItemParaAll[nIdxItem].GettingSignalEnd = true;
		}

		//-------------------------------------------�I�u�W�F�N�g�̒l��������
		this->Transform[nIdxItem].Pos(ipos);
		this->Transform[nIdxItem].Rot(irot);
		this->Transform[nIdxItem].Scl(iscl);
	}

	//�擾�������I�������A�C�e�����g�p(�`��)���Ȃ�
	else if(this->ItemParaAll[nIdxItem].GettingSignalEnd == true)
	{
		//�t���O���I�������g�p�C���X�^���X�������炷
		this->ReleaseInstance(nIdxItem);
		this->ItemParaAll[nIdxItem].GettingSignal = false;
		this->ItemParaAll[nIdxItem].GettingSignalEnd = false;
		this->ItemParaOne.GoukeiDrop--;
	}
}
