/**
* @file bulletprediction.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../../h/main.h"
#include "../../../h/other/input.h"
#include "../../../h/object/camera.h"
#include "../../../h/object/player.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/bullet/bulletprediction.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_BULLETPREDICTION		"../data/TEXTURE/effect/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BULLETPREDICTION_SIZE_X			(5.0f)							// �r���{�[�h�̕�
#define	BULLETPREDICTION_SIZE_Y			(5.0f)							// �r���{�[�h�̍���

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.1f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.1f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.1f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 0.1f),//p4�J���[
};

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
BULLETPREDICTION::BULLETPREDICTION(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//���_�̍쐬 �S��400�@�e100�쐬
	this->vtx[PLAYER01].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);
	this->vtx[PLAYER02].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);
	this->vtx[PLAYER03].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);
	this->vtx[PLAYER04].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);

	//�J�E���g���[�v�@�v���C���[
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//�J�E���g���[�v�@�e���\���G�t�F�N�g
		for (int CntBulletprediction = 0; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			// ���_���W�̐ݒ�
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
			{
			D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			};
			this->vtx[CntPlayer].Vertex3D(CntBulletprediction, vtx);

			//RHW�ݒ�
			this->vtx[CntPlayer].Nor3D(CntBulletprediction);

			//UV�̐ݒ�
			this->vtx[CntPlayer].UV3D(CntBulletprediction);

			//�J���[�ݒ�
			this->vtx[CntPlayer].Color3D(CntBulletprediction, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_BULLETPREDICTION);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
BULLETPREDICTION::~BULLETPREDICTION(void)
{
	//�e�N�X�`�����
	this->tex.~TEXTURE();
	//�J�E���g���[�v�@�v���C���[
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//���_���
		this->vtx[CntPlayer].~VTXBuffer();
	}
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void BULLETPREDICTION::Init(void)
{
	//�J�E���g���[�v�@�v���C���[
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//�J�E���g���[�v�@�e���\���G�t�F�N�g
		for (int CntBulletprediction = 0; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			this->Transform[CntPlayer][CntBulletprediction].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[CntPlayer][CntBulletprediction].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this->vtx[CntPlayer].Color3D(CntBulletprediction, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void BULLETPREDICTION::Update(PLAYER *player)
{
	//�v���C���[�̏�񂩂甭�ˈʒu�p�x�ړ��ʂ𗘗p���ăo���b�g�̒��e�_���Z�o����
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//---------------------------------�I�u�W�F�N�g�l�ǂݍ���
		D3DXVECTOR3	BulletPredictionPos = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
		BulletPredictionPos.y += 20.0f;
		D3DXVECTOR3 HoudaiRot = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
		D3DXVECTOR3 HoutouRot = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
		D3DXVECTOR3 HousinRot = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

		//���ˊp�x�A���ˍ��p�v�Z
		D3DXVECTOR3 BmoveRot;
		BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
		BmoveRot.y = sinf(player->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x);
		BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);
		D3DXVECTOR3 bulletmove;
		bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
		bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
		bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

		D3DXCOLOR	col = PLAYER_COLOR[CntPlayer];
		float Gravity = 0.0f;
		//float time = 1.0f;
		//float maxtime = 10.0f;

		for (int CntBulletprediction = 0 ; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			BulletPredictionPos.x += bulletmove.x;
			BulletPredictionPos.y -= bulletmove.y + Gravity;
			BulletPredictionPos.z += bulletmove.z;

			//�d�͂�����������
			Gravity += VALUE_GRAVITYADD_BULLET;
			//���Ԋ�����i�߂�B�򋗗���������قǒe���\���Ԋu���L���Ȃ�
			//time++;
			//�d�͍ő�l����
			if (Gravity > VALUE_GRAVITYMAX_BULLET) Gravity = VALUE_GRAVITYMAX_BULLET;
			//���X�ɃA���t�@�l���������ĉ������n�_�����₷������
			col.a += 0.01f;
			this->UpdateInstance(BulletPredictionPos, CntPlayer, col, BULLETPREDICTION_SIZE_X, BULLETPREDICTION_SIZE_Y, CntBulletprediction);


		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void BULLETPREDICTION::Draw(PLAYER *player, int CntPlayer)
{
	bool puse = player->iUseType[CntPlayer].Use();
	if (puse)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		for (int CntBulletprediction = 0; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			//---------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3	pos = this->Transform[CntPlayer][CntBulletprediction].Pos();
			D3DXVECTOR3	scl = this->Transform[CntPlayer][CntBulletprediction].Scl();
			D3DXMATRIX mtxWorld;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// �r���[�}�g���b�N�X���擾
			CAMERA *cam = GetCamera();

			mtxWorld._11 = cam[CntPlayer].mtxView._11;
			mtxWorld._12 = cam[CntPlayer].mtxView._21;
			mtxWorld._13 = cam[CntPlayer].mtxView._31;
			mtxWorld._21 = cam[CntPlayer].mtxView._12;
			mtxWorld._22 = cam[CntPlayer].mtxView._22;
			mtxWorld._23 = cam[CntPlayer].mtxView._32;
			mtxWorld._31 = cam[CntPlayer].mtxView._13;
			mtxWorld._32 = cam[CntPlayer].mtxView._23;
			mtxWorld._33 = cam[CntPlayer].mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, this->tex.Texture());

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntBulletprediction * 4), POLYGON_2D_NUM);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//=============================================================================
// �C���X�^���X�̐ݒ�
//=============================================================================
void BULLETPREDICTION::UpdateInstance(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY,int CntBulletPrediction)
{
	this->Transform[PlayerType][CntBulletPrediction].Pos(pos);
	this->Transform[PlayerType][CntBulletPrediction].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	this->vtx[PlayerType].Color3D(CntBulletPrediction,col);
}
