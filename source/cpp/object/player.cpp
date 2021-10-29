/**
* @file player.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../h/main.h"
#include "../../h/library.h"
#include "../../h/other/input.h"
#include "../../h/object/camera.h"
#include "../../h/map/field.h"
#include "../../h/other/sound.h"
#include "../../h/object/bullet/bullet.h"
#include "../../h/effect/effect.h"
#include "../../h/other/fade.h"
#include "../../h/net/sock.h"
#include "../../h/object/player.h"

//�摜�̎Q�Ɩ�																											
#define	TEXTURE_MEISAI				"../data/MODEL/28513607_p4_master1200.jpg"		//!< �ǂݍ��ރe�N�X�`���t�@�C����

#define BULLET_BORN_TIME_ADD		(1.0f)				//!< �e�򕜊������邽�߂̉��Z�^�C��
#define BULLET_BORN_TIME_MAX		(120.0f)			//!< 1�e�򕜊�����̂ɕK�v�ȃ^�C��

#define	EFFECT_SPEEDUP_SIZE_X		(25.0f)			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̕�
#define	EFFECT_SPEEDUP_SIZE_Y		(10.0f)			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̍���
#define	EFFECT_SPEEDUP_TIME			(10)			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̐�������

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4�J���[
};

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
PLAYER::PLAYER(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	// �e�N�X�`���̓ǂݍ���
	this->tex.LoadTexture(TEXTURE_MEISAI);

	//�J�E���g���[�v�@�v���C���[��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//�J�E���g���[�v�@�`��p�p�[�c�̐�
		for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_PARTS_TYPE_MAX; CntDrawPartsNum++)
		{
			// X�t�@�C���̓ǂݍ��݃f�[�^�Q
			LPD3DXMESH	Mesh;
			DWORD nv, nvi, nm, np;
			LPD3DXBUFFER BuffMat;

			// X�t�@�C���̓ǂݍ���
			LoadMesh(this->c_aFileNameModelDraw[CntDrawPartsNum], &BuffMat,
				&nm, &Mesh, &nv, &np, &nvi, NULL);

			//���_�̍쐬
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].MakeVertex3D(nv, FVF_VERTEX_3D);
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].MakeIdxVertex(nvi);

			//�o�b�t�@�̎擾���甽�f
			LPDIRECT3DVERTEXBUFFER9 *VtxBuff;
			LPDIRECT3DINDEXBUFFER9	*IdxBuff;
			VtxBuff = this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pVtxBuff();
			IdxBuff = this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pIdxBuff();
			Mesh->GetVertexBuffer(VtxBuff);
			Mesh->GetIndexBuffer(IdxBuff);

			//���_�J���[���v���C���[�F�ɕύX
			this->PlayerMeshColor(this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pVtxBuff(), this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pIdxBuff(), np, CntPlayer);

			//�f�[�^���f
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumMat(nm);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].Mat(BuffMat);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumVertex(nv);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumPolygon(np);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumVertexIndex(nvi);
		}


		//�g�p�̐ݒ�
		this->iUseType[CntPlayer].Use(YesUseType1);
	}

	//�J�E���g���[�v�@�I���W�i���p���f���̐�(���[�t�B���O��f�[�^)
	for (int CntOriginalModelNum = 0; CntOriginalModelNum < PLAYER_MODEL_ORIGINAL_TYPE_MAX; CntOriginalModelNum++)
	{
		// X�t�@�C���̓ǂݍ��݃f�[�^�Q
		LPD3DXMESH	Mesh;
		DWORD nv, nvi, nm, np;
		LPD3DXBUFFER BuffMat;

		// X�t�@�C���̓ǂݍ���
		LoadMesh(this->c_aFileNameModelOriginal[CntOriginalModelNum], &BuffMat,
			&nm, &Mesh, &nv, &np, &nvi, NULL);

		//���_�̍쐬
		this->modelOri.Vtx[CntOriginalModelNum].MakeVertex3D(nv, FVF_VERTEX_3D);
		this->modelOri.Vtx[CntOriginalModelNum].MakeIdxVertex(nvi);

		//�o�b�t�@�̎擾���甽�f
		LPDIRECT3DVERTEXBUFFER9 *VtxBuff;
		LPDIRECT3DINDEXBUFFER9	*IdxBuff;
		VtxBuff = this->modelOri.Vtx[CntOriginalModelNum].pVtxBuff();
		IdxBuff = this->modelOri.Vtx[CntOriginalModelNum].pIdxBuff();
		Mesh->GetVertexBuffer(VtxBuff);
		Mesh->GetIndexBuffer(IdxBuff);

		//���f���f�[�^���f
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumMat(nm);
		this->modelOri.ModelAttribute[CntOriginalModelNum].Mat(BuffMat);
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumVertex(nv);
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumPolygon(np);
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumVertexIndex(nvi);
	}

	//�e�q�K�w�A�h���X��ݒ�
	//this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai = &this[CntPlayer];
	//this[CntPlayer].Parent = NULL;
	//this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai = &this[CntPlayer];
	//this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts = &this[CntPlayer].parts[PARTSTYPE_HOUTOU];

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
PLAYER::~PLAYER(void)
{
	//�e�N�X�`�����
	this->tex.~TEXTURE();

	//�J�E���g���[�v�@�v���C���[��
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//�J�E���g���[�v�@�`��p���f���̐�
		for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_PARTS_TYPE_MAX; CntDrawPartsNum++)
		{
			//���f�����
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].~VTXBuffer();
		}
	}

	//�J�E���g���[�v�@�I���W�i���p���f���̐�
	for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_MODEL_ORIGINAL_TYPE_MAX; CntDrawPartsNum++)
	{
		//���f�����
		this->modelOri.Vtx[CntDrawPartsNum].~VTXBuffer();
	}

	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void PLAYER::Init(FIELD *field)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//PLAYER �������@�e���������֐���
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		this->iUseType[CntPlayer].Use(YesUseType1);

		//�W���p�����[�^
		this->PlayerPara[CntPlayer].StandardPara.Vital = this->PlayerPara[CntPlayer].StandardPara.OldVital = PLAYER_VITAL_MAX;
		this->PlayerPara[CntPlayer].StandardPara.Speed = 0.0f;
		this->PlayerPara[CntPlayer].StandardPara.eModelType = this->PlayerPara[CntPlayer].StandardPara.eOldModelType= PLAYER_MODEL_TYPE_NORMAL;
		this->PlayerPara[CntPlayer].StandardPara.FUPFCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�o���b�g�p�����[�^
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->PlayerPara[CntPlayer].BulletPara.BulletStock = BULLET_MAX_STOCK;
		this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
		for (int i = 0; i < 3; i++) this->PlayerPara[CntPlayer].BulletPara.BulletMove[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�A�C�e���p�����[�^
		this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.KiriSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = 1.0f;
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime = 0.0f;
		this->PlayerPara[CntPlayer].ItemPara.KiriTime = 0.0f;
		this->PlayerPara[CntPlayer].ItemPara.BackCameraTime = 0.0f;

		//���[�t�B���O�p�����[�^
		this->PlayerPara[CntPlayer].MorphingPara.MorphingDTtime = 0.0f;
		//this->PlayerPara[CntPlayer].MorphingPara.MorphingEnd=true
		this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NoMorphing;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingStart = false;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
		this->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = false;

		//���̑��p�����[�^
		this->PostureVec[CntPlayer].FNVecFunc(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->PostureVec[CntPlayer].FNUNCrossFunc(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->PostureVec[CntPlayer].QrotFunc(0.0f);
		this->Move[CntPlayer].Move(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//�C��
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//�C�g								 
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//���f���̃��Z�b�g
		ResetModel(
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(),
			this->modelOri.Vtx[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN].VtxBuff(),
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff(),
			&this->modelOri.ModelAttribute[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN]);
	}

	//�������i�K�ō��W�Ɗp�x�������_���Őݒ�
	this->modelDraw[PLAYER01].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this->modelDraw[PLAYER02].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this->modelDraw[PLAYER03].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));
	this->modelDraw[PLAYER04].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));

	this->modelDraw[PLAYER01].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this->modelDraw[PLAYER02].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this->modelDraw[PLAYER03].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this->modelDraw[PLAYER04].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));

	//��������̏�Ԃň�x�J�����A�p���̏���������
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//���C�L���X�g��������
		D3DXVECTOR3 RayStart = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
		RayStart.y += 10.0f;
		D3DXVECTOR3 RayEnd = RayStart;
		RayEnd.y -= 1000.0f;

		float ReturnPosY = 0.0f;
		D3DXVECTOR3 FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���C�L���X�g�֐�
		field->FieldHitGetSphereVec(RayStart, RayEnd, &FieldNorVec, &ReturnPosY);

		//���C�L���X�g���ʂ𔽉f
		D3DXVECTOR3 Pos = RayStart;
		Pos.y = ReturnPosY;
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(Pos);
		this->PostureVec[CntPlayer].FNVecFunc(FieldNorVec);
		this->Quaternion(CntPlayer);
		this->CameraRevers(CntPlayer, false);

		//�J������������
		D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
		D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
		D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

		CAMERA *cam = GetCamera();
		cam[CntPlayer].at.x = Pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = Pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = Pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = Pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = Pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = Pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}

}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void PLAYER::Update(EFFECT*effect, BULLET*bullet, SHADOW*shadow, FADE *fade, bool Netflag, int MyNumber)
{
	//���l���񂾂��v�Z�B�O�l���񂾂�Q�[���I���B���̃V�[����
	int deadcnt = 0;
	//�v���C���[�l�������[�v
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool use = this->iUseType[CntPlayer].Use();
		if (use == false) deadcnt++;
		if (deadcnt >= 3)
		{
			fade->SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
			//this->GameSceneFlagFuncSetGameSceneFlag(false);
		}
	}

	//�v���C���[����
	//�v���C���[�l�������[�v ���[�J���ΐ�
	if (Netflag == false)
	{
		this->MoveKeybord(0, &effect[0], Netflag);
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			bool use = this->iUseType[CntPlayer].Use();
			//�����Ă���ΐ���\
			if (use)
			{
				//this->SetMoveL2R2(CntPlayer, Netflag);
				//this->MoveL(CntPlayer, &effect[0], Netflag);
				this->Quaternion(CntPlayer);
				this->CameraRevers(CntPlayer, Netflag);
				this->BulletALL(CntPlayer, &bullet[0], shadow, Netflag);
				this->ItemTimeKiri(CntPlayer);
				this->ItemTimeMorphing(CntPlayer);
			}

			//����ȊO�̓J������������
			else
			{
				this[CntPlayer].Quaternion(CntPlayer);

				D3DXVECTOR3 Pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
				D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
				D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
				D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

				CAMERA *cam = GetCamera();
				cam[CntPlayer].at.x = Pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
				cam[CntPlayer].at.y = Pos.y + (HousinRot.x*100.0f);
				cam[CntPlayer].at.z = Pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

				cam[CntPlayer].pos.x = Pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
				cam[CntPlayer].pos.y = Pos.y + POS_H_CAM;
				cam[CntPlayer].pos.z = Pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
			}
		}
	}

	//�l�b�g�ΐ�
	else
	{
		this->MoveKeybord(0, &effect[0], Netflag);
		bool use = this->iUseType[MyNumber].Use();

		//�����Ă���Α���\
		if (use)
		{
			//�l�b�g�p�t���O�������@1�t���[�����̕ύX��ۑ����ĕύX������΃T�[�o�[�Ƀf�[�^�𑗂�
			this->PlayerPara[MyNumber].MorphingPara.NetGetMorphingOneFrame = false;
			this->PlayerPara[MyNumber].BulletPara.BulletStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			for (int i = 0; i < 3; i++) this->PlayerPara[MyNumber].BulletPara.BulletMove[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
				this->PlayerPara[CntPlayer].StandardPara.OldVital = this->PlayerPara[CntPlayer].StandardPara.Vital;
			}
			this->MoveLtype0(MyNumber, &effect[0], Netflag);
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this->Quaternion(CntPlayer);
			}
			this->CameraRevers(MyNumber, Netflag);
			this->BulletALL(MyNumber, &bullet[0], shadow, Netflag);
			this->ItemTimeKiri(MyNumber);
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this->ItemTimeMorphing(CntPlayer);
			}
		}

		//����ȊO�̓J������������
		else
		{
			this[MyNumber].Quaternion(MyNumber);

			D3DXVECTOR3 Pos = this->modelDraw[MyNumber].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
			D3DXVECTOR3 HoudaiRot = this->modelDraw[MyNumber].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
			D3DXVECTOR3 HoutouRot = this->modelDraw[MyNumber].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
			D3DXVECTOR3 HousinRot = this->modelDraw[MyNumber].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

			CAMERA *cam = GetCamera();
			cam[MyNumber].at.x = Pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
			cam[MyNumber].at.y = Pos.y + (HousinRot.x*100.0f);
			cam[MyNumber].at.z = Pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

			cam[MyNumber].pos.x = Pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[MyNumber].len;
			cam[MyNumber].pos.y = Pos.y + POS_H_CAM;
			cam[MyNumber].pos.z = Pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[MyNumber].len;
		}

	}
}

//=============================================================================
// �G�l�~�[�X�V����
//=============================================================================
//void PLAYER::Update(void)
//{
	//for (int CntEnemy = 0; CntEnemy < ENEMY_MAX; CntEnemy++)
	//{
	//	PLAYER *player = GetPlayerHoudai();
	//	D3DXVECTOR3 PEvec = player->parameter.pos - g_EnemyHoudai[CntEnemy].parameter.pos;
	//	float PElen = D3DXVec3Length(&PEvec);
	//	g_EnemyHoudai[CntEnemy].EyeLen = D3DXVec3Length(&g_EnemyHoudai[CntEnemy].EyeDistance);
	//	if (GetKeyboardPress(DIK_C))
	//	{//���E���Ƀv���C���[
	//		g_EnemyHoudai[CntEnemy].mode = 4;
	//		g_EnemyHoudai[CntEnemy].model.MorphingSignal = NowMorphing;
	//	}
	//	//else if (g_EnemyHoudai[CntEnemy].EyeLen >= EIlen)
	//	//{//���E���ɃA�C�e��
	//	//}
	//	else if (GetKeyboardPress(DIK_X))
	//	{//���E�O�Ƀv���C���[
	//		g_EnemyHoudai[CntEnemy].mode = SEARCH;					//�T�����[�h
	//	}
	//	switch (g_EnemyHoudai[CntEnemy].mode)
	//	{
	//	case SEARCH: //�����_���ɒT���@
	//		break;
	//	case ATTACK: //�v���C���[�ɍU��
	//		SetEnemyBullet(g_EnemyHoudai[CntEnemy].parameter.pos, ENEMY_BULLET_TYPE01);
	//		break;
	//	case GOITEM: //�A�C�e�����ɍs��
	//		break;
	//	case DEAD: //��
	//		if (g_EnemyHoudai[CntEnemy].model.MorphingSignal == EndMorphing) g_EnemyHoudai[CntEnemy].mode = -1;
	//		break;
	//	case 4:
	//		if (g_EnemyHoudai[CntEnemy].model.MorphingSignal == EndMorphing) g_EnemyHoudai[CntEnemy].mode = -1;
	//		break;
	//	default:
	//		break;
	//	}
	//	// �e�̈ʒu�ݒ�
	//	{
	//		PositionShadow(g_EnemyHoudai[CntEnemy].parameter.shadowIdx,
	//			D3DXVECTOR3(g_EnemyHoudai[CntEnemy].parameter.pos.x, 0.0f, g_EnemyHoudai[CntEnemy].parameter.pos.z), g_EnemyHoudai[CntEnemy].parameter.scl);
	//	}
	//}
//}

//=============================================================================
// �`�揈��
//=============================================================================
void PLAYER::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool use = this->iUseType[CntPlayer].Use();
		if (use == true)
		{
			D3DXMATRIX mtxWorldHoudai;
			D3DXMATRIX mtxWorldHoutou;
			D3DXMATRIX mtxWorldHousin;
			//for() PLAYER_PARTS_TYPE_HOUDAI mtxWorldHoudai
			//-----------------------------------------------------�e
			{
				D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
				D3DXMATERIAL *pD3DXMat;
				D3DMATERIAL9 matDef;
				D3DXMATRIX mtxQ;
				D3DXMatrixIdentity(&mtxQ);

				//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
				D3DXVECTOR3 UpNorFieldNorVec = this->PostureVec[CntPlayer].FNUNCrossFunc();
				float Qrot = this->PostureVec[CntPlayer].QrotFunc();
				D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();

				//q=(rotVecAxis�@��)*(g_Player.rot��])
				D3DXQuaternionRotationAxis(&q, &UpNorFieldNorVec, -Qrot);
				D3DXMatrixRotationQuaternion(&mtxQ, &q);

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorldHoudai);

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxScl);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxRot);
				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxQ);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxTranslate);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldHoudai);
				//this[CntPlayer].Matrix(mtxWorldHoudai);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				// �`��
				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].NumMat(); nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

					if (pD3DXMat[nCntMat].pTextureFilename != NULL)
					{
						// �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, this->tex.Texture());
					}

					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);
					// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetStreamSource(0, this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUDAI].VtxBuff(), 0, sizeof(VERTEX_3D));
					// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetIndices(this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUDAI].IdxBuff());
					//�`��
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
						this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].NumVertex(), 0,
						this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].NumPolygon());
				}
				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}

			//-----------------------------------------------------�q
			{
				D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
				D3DXMATERIAL *pD3DXMat;
				D3DMATERIAL9 matDef;

				//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Pos();

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorldHoutou);

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &mtxScl);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &mtxRot);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &mtxTranslate);

				//if (this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai != NULL)
				{
					//-------------------------------------------------�e�̃��[���h�}�g���N�X���擾
					D3DXMATRIX ParentmtxWorldHoutou = mtxWorldHoudai;
					D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &ParentmtxWorldHoutou);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldHoutou);
				//this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetMatrix(mtxWorldHoutou);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].NumMat(); nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
					if (pD3DXMat[nCntMat].pTextureFilename != NULL)
					{
						// �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, this->tex.Texture());
					}
					// �`��
					{
						// ���_�t�H�[�}�b�g�̐ݒ�
						pDevice->SetFVF(FVF_VERTEX_3D);
						// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
						pDevice->SetStreamSource(0, this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUTOU].VtxBuff(), 0, sizeof(VERTEX_3D));
						// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
						pDevice->SetIndices(this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUTOU].IdxBuff());
						//�`��
						pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].NumVertex(), 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].NumPolygon());
					}
				}
				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}

			//-----------------------------------------------------��
			{
				D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
				D3DXMATERIAL *pD3DXMat;
				D3DMATERIAL9 matDef;

				//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Pos();

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorldHousin);

				// �X�P�[���𔽉f
				D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &mtxScl);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &mtxRot);

				// �ړ��𔽉f
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &mtxTranslate);



				//if (this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts != NULL)
				{
					//-------------------------------------------------�e�̃��[���h�}�g���N�X���擾
					D3DXMATRIX ParentmtxWorldHousin = mtxWorldHoutou;
					D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &ParentmtxWorldHousin);
				}

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldHousin);
				//this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetMatrix(mtxWorldHousin);
				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A�����ɑ΂���|�C���^���擾
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].NumMat(); nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
					if (pD3DXMat[nCntMat].pTextureFilename != NULL)
					{
						// �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, this->tex.Texture());
					}
					// �`��
					{
						// ���_�t�H�[�}�b�g�̐ݒ�
						pDevice->SetFVF(FVF_VERTEX_3D);
						// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
						pDevice->SetStreamSource(0, this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(), 0, sizeof(VERTEX_3D));
						// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
						pDevice->SetIndices(this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff());
						//�`��
						pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].NumVertex(), 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].NumPolygon());
					}
				}
				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//=============================================================================
// ���b�V���J���[���Z�b�g
//=============================================================================
void PLAYER::PlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff, DWORD nNumPolygon, int CntPlayer)
{
	VERTEX_3D *pVtx;
	WORD *pIdx;

	LPDIRECT3DVERTEXBUFFER9 vtx = *pD3DVtxBuff;
	LPDIRECT3DINDEXBUFFER9 idx = *pD3DIdxBuff;
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	vtx->Lock(0, 0, (void**)&pVtx, 0);
	idx->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// ���ˌ��̐ݒ�
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[CntPlayer];
	}
	// ���_�f�[�^���A�����b�N����
	vtx->Unlock();
	idx->Unlock();
}

//=============================================================================
// �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER::MoveABL(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);

	int dir = FRONT_VEC;
	//�ړ�����
	if (IsButtonPressed(CntPlayer, BUTTON_A))
	{
		dir = FRONT_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed += VALUE_MOVE;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_B))
	{
		dir = BACK_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed -= VALUE_MOVE;
	}

	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//���_�ω��̃A�i���O�l�����ɑ�����ăA�i���O����Ő���
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//����
	if (!IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT) && !IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		dir = FRONT_VEC;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
	{
		HoudaiRot.y += LAnalogX * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		HoudaiRot.y += LAnalogX * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff;
	}

	//�p�x�̐����l
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// �ړ����x�̐���
	if (this->PlayerPara[CntPlayer].StandardPara.Speed >= VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = VALUE_MOVE_MAX;
	else if (this->PlayerPara[CntPlayer].StandardPara.Speed <= -VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = -VALUE_MOVE_MAX;

	// �v���C���[�̍��W���X�V
	pos.x -= sinf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//�X�s�[�h�o�t���Ԍ���
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		effect->SetInstance(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//�C������@�o���b�g���e�_(���E�G�C��)
	if (IsButtonPressed(CntPlayer, BUTTON_R1))
	{
		HoutouRot.y += VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y >= VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_L1))
	{
		HoutouRot.y -= VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y <= -VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = -VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}

	//�C�g����@�o���b�g���e�_(�O��G�C��)
	if (IsButtonPressed(CntPlayer, BUTTON_R2))
	{
		HousinRot.x += VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x >= VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(CntPlayer, BUTTON_L2))
	{
		HousinRot.x -= VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x <= -VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = -VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

}

//=============================================================================
// �ړ�����(ki-bo-do�ňړ�����)
//=============================================================================
void PLAYER::MoveKeybord(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);

	int dir = FRONT_VEC;
	//�ړ�����
	if (GetKeyboardPress(DIK_W))
	{
		dir = FRONT_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed += VALUE_MOVE * 10*4;
	}
	else if (GetKeyboardPress(DIK_S))
	{
		dir = BACK_VEC;
		this->PlayerPara[CntPlayer].StandardPara.Speed -= VALUE_MOVE * 10*4;
	}

	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//����
	if (GetKeyboardPress(DIK_D))
	{
		HoudaiRot.y += 0.02f * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff*2;
	}
	else if (GetKeyboardPress(DIK_A))
	{
		HoudaiRot.y -= 0.02f * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff*2;
	}


	//�p�x�̐����l
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// �ړ����x�̐���
	if (this->PlayerPara[CntPlayer].StandardPara.Speed >= VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = VALUE_MOVE_MAX;
	else if (this->PlayerPara[CntPlayer].StandardPara.Speed <= -VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = -VALUE_MOVE_MAX;

	// �v���C���[�̍��W���X�V
	pos.x -= sinf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//�X�s�[�h�o�t���Ԍ���
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		effect->SetInstance(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//�C������@�o���b�g���e�_(���E�G�C��)
	if (GetKeyboardPress(DIK_RIGHT))
	{
		HoutouRot.y += VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y >= VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		HoutouRot.y -= VALUE_ROTATE_PLAYER_HOUTOU;
		if (HoutouRot.y <= -VALUE_ROTATE_PLAYER_HOUTOU_MAX) HoutouRot.y = -VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}

	//�C�g����@�o���b�g���e�_(�O��G�C��)
	if (GetKeyboardPress(DIK_UP))
	{
		HousinRot.x += VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x >= VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		HousinRot.x -= VALUE_ROTATE_PLAYER_HOUSIN;
		if (HousinRot.x <= -VALUE_ROTATE_PLAYER_HOUSIN_MAX) HousinRot.x = -VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

}

//=============================================================================
// �ړ�����(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER::MoveL(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
	float LAnalogX = 0.0f;		//�c����
	float LAnalogY = 0.0f;		//������
	float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

	//�_�b�V������
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//�X�s�[�h�o�t���Ԍ���
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = pos;
		effect->SetInstance(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (this->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//�ړ�����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//������͂͌�ޒ��Ɍ��胊�o�[�X����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//�ړ��ʂ𔽉f
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//�v���C���[���W���X�V
	pos += move;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}

//=============================================================================
// �ړ�����(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER::MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
	float LAnalogX = 0.0f;		//�c����
	float LAnalogY = 0.0f;		//������
	float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

	//�_�b�V������
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//�X�s�[�h�o�t���Ԍ���
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = pos;
		effect->SetInstance(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (this->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//�ړ�����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//������͂͌�ޒ��Ɍ��胊�o�[�X����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//�ړ��ʂ𔽉f
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//�v���C���[���W���X�V
	pos += move;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}

//=============================================================================
// �ړ�����(LR�X�e�B�b�N�ŃL���^�s���ړ�����)
//=============================================================================
void PLAYER::MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
	float L2 = 0.0f;		//�c����
	float R2 = 0.0f;		//������
	float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

	//�_�b�V������
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//�X�s�[�h�o�t���Ԍ���
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = pos;
		effect->SetInstance(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		}
	}

	if (this->PlayerPara[CntPlayer].ItemPara.DashSignal == true)
	{
		DashRate = PLAYER_VALUE_DASHRATE;
	}

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//���E�̃L���^�s����O�i���ǂ��炩�̃t���O
	//���L���^�s���O�i����=0�A���L���^�s���̂ݑO�i=3.925f�A�E�L���^�s���̂ݑO�i=-3.925f�A���L���^�s���̂݌��=-0.785f�A�E�L���^�s���̂݌��=0.785f�A
	float Lrot = 3.925f;
	float Rrot = -3.925f;
	int Ldir = 1;
	int Rdir = 1;
	//�ړ�����
	if (IsButtonPressed(PadNum, BUTTON_L1))
	{
		Lrot = -0.785f;
		Ldir *= -1;
	}
	if (IsButtonPressed(PadNum, BUTTON_R1))
	{
		Rrot = 0.785f;
		Rdir *= -1;
	}

	//���L���^�s���g�p
	if (IsButtonPressed(PadNum, BUTTON_L2) && IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		//���͒����l32767�@R2�ŏ�0�@L2�ő�64000
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		R2 = L2;

	}
	//���L���^�s���̂ݎg�p
	else if (IsButtonPressed(PadNum, BUTTON_L2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		HoudaiRot.y += 0.1f*Ldir;
	}
	//�E�L���^�s���̂ݎg�p
	else if (IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		float IZbuf = Button->lZ * PLAYER_MOVE_RATE_LR2;
		R2 = IZbuf;
		R2 = 32767 * PLAYER_MOVE_RATE_LR2 + (-R2) + 32767 * PLAYER_MOVE_RATE_LR2;
		HoudaiRot.y -= 0.1f*Rdir;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}

	//�ړ��ʂ𔽉f
	move.x = R2 * sinf(HoudaiRot.y + Rrot) + L2 * sinf(HoudaiRot.y + Lrot);
	move.z = R2 * cosf(HoudaiRot.y + Rrot) + L2 * cosf(HoudaiRot.y + Lrot);

	//�v���C���[���W���X�V
	pos += move;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}

//=============================================================================
// �J�������]����(�蓮�ƃA�C�e���̗����Ő���)
//=============================================================================
void PLAYER::CameraRevers(int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	CAMERA *cam = GetCamera();

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//�o�b�N�J���������@�o�b�N�J�����I���@�J�������_�A�����_�AY�{�^���������Ă���������́A�o�b�N�J�����A�C�e����ON�ɂȂ��Ă���Ƃ��̓J�������]
	if (GetKeyboardPress(DIK_B) || IsButtonPressed(PadNum, BUTTON_Y) || this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal == true)
	{
		cam[CntPlayer].at.x = pos.x + (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z + (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x - sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z - cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}

	//�o�b�N�J�����I�t�@�J�������_�A�����_
	//����ȊO�͒ʏ�J����
	else
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//�����A�o�b�N�J�����A�C�e����ON�̎���Y�������ƒʏ�J�����ɂȂ�
	if (this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal == true && IsButtonPressed(PadNum, BUTTON_Y))
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//�o�b�N�J�����̎��ԏ���
	if (this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.BackCameraTime += 1.0f;
		if (this->PlayerPara[CntPlayer].ItemPara.BackCameraTime >= BACKCAMERA_TIME)
		{
			this->PlayerPara[CntPlayer].ItemPara.BackCameraTime = 0.0f;
			this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal = false;
		}
	}
}

//=============================================================================
// �J��������(R�X�e�B�b�N��Rot����)
//=============================================================================
void PLAYER::CameraRotControl(int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old�ۑ�
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

	CAMERA *cam = GetCamera();

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//����p�ω���R�X�e�B�b�N�A�i���O�l���g�p
	float RAnalogX = 0.0f;		//�c����
	float RAnalogY = 0.0f;		//������

	//����p����
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_R_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_R_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		//���͒����l32000�@�ŏ�0�@�ő�64000
		//�Ȃ̂�-32000���邱�ƂŁ@�����l0�@�ŏ�-32000�@�ő�32000�ɂ��Ă���
		//rot��32000�Ƃ��o�J�������l������ƂԂ���Ԃ̂ŁA����ɓ��͒l������������
		//�ő�0.03���炢�ɂȂ�悤�����@/1000000����(���삵�₷���l�ł���)
		RAnalogX = float(Button->lRx) - 32000.0f;
		RAnalogY = float(Button->lRy) - 32800.0f;

		RAnalogX = RAnalogX / 1000000.0f;
		RAnalogY = RAnalogY / 1000000.0f;

	}

	//��]�ʂ𔽉f
	D3DXVECTOR3 moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moverot.y = RAnalogX;
	moverot.x = -RAnalogY;

	HoutouRot.y += moverot.y;
	HousinRot.x += moverot.x;

	//�p�x�̐����l
	{
		if (HousinRot.x >= 0.3f)
		{
			HousinRot.x = 0.3f;
		}
		else if (HousinRot.x <= -0.3f)
		{
			HousinRot.x = -0.3f;
		}
	}

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);


	////�J�����̎��_�X�V
	//cam[CntPlayer].pos.x = this[CntPlayer].pos.x - (POS_W_CAM * cosf(-this[CntPlayer].rot.y));
	//cam[CntPlayer].pos.y = this[CntPlayer].pos.y + (POS_H_CAM);
	//cam[CntPlayer].pos.z = this[CntPlayer].pos.z - (POS_W_CAM * sinf(-this[CntPlayer].rot.y));
	////�J�����̒����_���X�V
	////�e�N�j�b�N�@�J���������_����ŉ�]��atrotX���グ�܂����Ă��A���ꂾ���ł͐^��������Ȃ��B
	////atrotX�̑����ɔ�Ⴕ��atXZ�l�����������Ă����Ȃ��Ɛ^���^�������悤�Ȓ����_�𐧌�ł��Ȃ�
	//cam[CntPlayer].at.x = cam[CntPlayer].pos.x + (POS_W_CAM * sinf(this[CntPlayer].rot.y));
	//cam[CntPlayer].at.y = cam[CntPlayer].pos.y - (POS_H_CAM * sinf(this[CntPlayer].atrot.x));
	//cam[CntPlayer].at.z = cam[CntPlayer].pos.z + (POS_W_CAM * cosf(this[CntPlayer].rot.y));
}

//=============================================================================
// �N�H�[�^�j�I������
//=============================================================================
void PLAYER::Quaternion(int CntPlayer)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 FieldNorVec = this->PostureVec[CntPlayer].FNVecFunc();
	D3DXVECTOR3 FieldNorUpNorCross = this->PostureVec[CntPlayer].FNUNCrossFunc();
	float Qrot = this->PostureVec[CntPlayer].QrotFunc();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
	D3DXVec3Cross(&FieldNorUpNorCross, &FieldNorVec, &UpVec);

	//�n�`�@����Up�x�N�g���œ��ς��Ƃ�(���ɑ΂����]�p�x�����߂�)
	float Upkakezan = D3DXVec3Dot(&FieldNorVec, &UpVec);

	//������]�p�x��0�ȊO(�����łȂ�)�Ȃ�Ή�]�p�x�Ƃ����߂�
	//�Ƃ̓N�H�[�^�j�I���̔C�ӎ�����]�����鎞�̉�]�p�x�ɂȂ�
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(FieldNorVec.x*FieldNorVec.x +
				FieldNorVec.y *FieldNorVec.y +
				FieldNorVec.z * FieldNorVec.z);
		Qrot = acosf(cossita);
	}
	else Qrot = 0.0f;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->PostureVec[CntPlayer].QrotFunc(Qrot);
	this->PostureVec[CntPlayer].FNUNCrossFunc(FieldNorUpNorCross);

}

//=============================================================================
// �o���b�g�֘A����
//=============================================================================
void PLAYER::BulletALL(int CntPlayer, BULLET *bullet, SHADOW *shadow, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FieldNorVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
	//D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FieldNorVec, &Frontvec);
	float Bkakezan = D3DXVec3Dot(&FieldNorVec, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(FieldNorVec.x*FieldNorVec.x +
				FieldNorVec.y *FieldNorVec.y +
				FieldNorVec.z * FieldNorVec.z)
			*
			sqrtf(Frontvec.x*Frontvec.x +
				Frontvec.y *Frontvec.y +
				Frontvec.z * Frontvec.z);
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = acosf(cossita);
	}
	else
	{
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 1.57f;		//�������x�N�g��rot=0.0f�A������x�N�g��rot=3.14�A�ɑ΂��Ă̑O�����x�N�g����rot=1.57f�B
	}
	this->PlayerPara[CntPlayer].BulletPara.BulletRotY -= 1.57f;

	//�v���C���[pos���甭�˕����ɏ������炵���l
	//�n�ʂ̌X���ɉ����Ĕ��˂���Ƃ��͖��Ȃ��B���̌X�����獶�E�ɉ�]���Ă鎞����pos����������
	D3DXVECTOR3 BposStart;
	BposStart.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
	BposStart.y = pos.y + (sinf(this->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
	BposStart.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);

	D3DXVECTOR3 BmoveRot;
	BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
	BmoveRot.y = sinf(this->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x);
	BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

	D3DXVECTOR3 bulletmove;
	bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

	//�l�b�g�ΐ펞�̓Q�[���p�b�h�i���o�[��0�@����ȊO�̓v���C���[�ԍ��őΉ�������
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	// �e����
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock > 0)
	{
		//if (IsButtonTriggered(PadNum, BUTTON_X))
		//{
		if (IsButtonTriggered(PadNum, BUTTON_R1) || GetKeyboardTrigger(DIK_SPACE))
		{
			bullet->SetInstance(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);
			//�g�U�e����
			if (this->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
			{
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				bullet->SetInstance(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);
				bullet->SetInstance(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);

			}
			//�c�e�����炷
			this->PlayerPara[CntPlayer].BulletPara.BulletStock -= 1;

			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}
	//�c�e���� ��莞�Ԍo�߂�1�Â�����
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock < BULLET_MAX_STOCK) this->PlayerPara[CntPlayer].BulletPara.BulletBornTime += BULLET_BORN_TIME_ADD;
	if (this->PlayerPara[CntPlayer].BulletPara.BulletBornTime >= BULLET_BORN_TIME_MAX)
	{
		this->PlayerPara[CntPlayer].BulletPara.BulletStock++;
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
	}
}

//=============================================================================
// �o���b�g�֘A����
//=============================================================================
void PLAYER::BulletALLMoveL2R2Ver(int CntPlayer, BULLET *bullet, SHADOW *shadow)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FieldNorVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
	//D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FieldNorVec, &Frontvec);
	float Bkakezan = D3DXVec3Dot(&FieldNorVec, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(FieldNorVec.x*FieldNorVec.x +
				FieldNorVec.y *FieldNorVec.y +
				FieldNorVec.z * FieldNorVec.z)
			*
			sqrtf(Frontvec.x*Frontvec.x +
				Frontvec.y *Frontvec.y +
				Frontvec.z * Frontvec.z);
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = acosf(cossita);
	}
	else
	{
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 1.57f;		//�������x�N�g��rot=0.0f�A������x�N�g��rot=3.14�A�ɑ΂��Ă̑O�����x�N�g����rot=1.57f�B
	}
	this->PlayerPara[CntPlayer].BulletPara.BulletRotY -= 1.57f;


	//�v���C���[pos���甭�˕����ɏ������炵���l
	//�n�ʂ̌X���ɉ����Ĕ��˂���Ƃ��͖��Ȃ��B���̌X�����獶�E�ɉ�]���Ă鎞����pos����������
	D3DXVECTOR3 BposStart;
	BposStart.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
	BposStart.y = pos.y + (sinf(this->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
	BposStart.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);


	D3DXVECTOR3 BmoveRot;
	BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
	BmoveRot.y = sinf(this->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x);
	BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

	D3DXVECTOR3 bulletmove;
	bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

	// �e����
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock > 0)
	{
		if (IsButtonTriggered(CntPlayer, BUTTON_X))
		{
		//if (IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_UP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTUP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHT) ||
			//IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTDOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_DOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTDOWN) ||
			//IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFT) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTUP))
		//{
			bullet->SetInstance(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);
			//�g�U�e����
			if (this->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
			{
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				bullet->SetInstance(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);
				bullet->SetInstance(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);

			}
			//�c�e�����炷
			this->PlayerPara[CntPlayer].BulletPara.BulletStock -= 1;

			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}

	//�c�e���� ��莞�Ԍo�߂�1�Â�����
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock < BULLET_MAX_STOCK) this->PlayerPara[CntPlayer].BulletPara.BulletBornTime += BULLET_BORN_TIME_ADD;
	if (this->PlayerPara[CntPlayer].BulletPara.BulletBornTime >= BULLET_BORN_TIME_MAX)
	{
		this->PlayerPara[CntPlayer].BulletPara.BulletStock++;
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
	}
}

//=============================================================================
// �t�H�O����
//=============================================================================
void PLAYER::ItemTimeKiri(int CntPlayer)
{
	if (this->PlayerPara[CntPlayer].ItemPara.KiriSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.KiriTime += 1.0f;
		if (this->PlayerPara[CntPlayer].ItemPara.KiriTime >= KIRI_TIME)
		{
			this->PlayerPara[CntPlayer].ItemPara.KiriTime = 0.0f;
			this->PlayerPara[CntPlayer].ItemPara.KiriSignal = false;
		}
	}
}

//=============================================================================
// ���[�t�B���O����
//=============================================================================
void PLAYER::ItemTimeMorphing(int CntPlayer)
{
	// ���[�t�B���Otrue
	if (this->PlayerPara[CntPlayer].MorphingPara.MorphingStart == true)
	{
		///////////////////////////////////////////////////////////////////////�o���b�g3���ԊJ�n
		// ���[�t�B���O���Ԍ��Z�J�n
		this->PlayerPara[CntPlayer].MorphingPara.MorphingTime -= 1.0f;

		// ���[�t�B���O�U���^�C�v�ɕύX�J�n
		if (this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal == NowMorphing)
		{
			this->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_ATTACK;
			DoMorphing(
				this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(),
				this->modelOri.Vtx[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN_MORPHING].VtxBuff(),
				this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff(), 
				&this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN],
				0.01f, &this->PlayerPara[CntPlayer].MorphingPara.MorphingDTtime, &this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal);
		}
		///////////////////////////////////////////////////////////////////////�o���b�g3���ԏI��

		// ���Ԍo�߂Ń��f�������ɖ߂�
		else if (this->PlayerPara[CntPlayer].MorphingPara.MorphingTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_NORMAL;
			this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
			this->PlayerPara[CntPlayer].MorphingPara.MorphingStart = false;
		}
	}

	// ���[�t�B���O�I���W�i���^�C�v�ɕύX�J�n
	if (this->PlayerPara[CntPlayer].MorphingPara.MorphingStart == false && this->PlayerPara[CntPlayer].MorphingPara.MorphingTime <= 0.0f)
	{
		DoMorphing(
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(),
			this->modelOri.Vtx[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN].VtxBuff(),
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff(),
			&this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN],
			0.01f, &this->PlayerPara[CntPlayer].MorphingPara.MorphingDTtime, &this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal);
		if (this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal == EndMorphing)
		{
			this->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
		}
	}
}

