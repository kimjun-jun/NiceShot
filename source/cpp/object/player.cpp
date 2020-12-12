/**
* @file player.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
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

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4�J���[
};

//=============================================================================
// ����������
//=============================================================================
void PLAYER_HONTAI::Init(FIELD *field)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//PLAYER ������
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////�C��
		this[CntPlayer].SetRot(D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		this[CntPlayer].SetUse(true);

		this[CntPlayer].speedbuff = 1.0f;
		this[CntPlayer].MorphingTime = MORPHING_TIME;
		this[CntPlayer].MorphingEnd = true;
		this[CntPlayer].Parent = NULL;
		this[CntPlayer].AmmoCnt = PLAYER_AMMOPOWER_NORMAL;
		this[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
		this[CntPlayer].vital = this[CntPlayer].oldvital = PLAYER_VITAL_MAX;
		this[CntPlayer].MorphingSignal = NoMorphing;

		// X�t�@�C���̓ǂݍ���
		LoadMesh(MODEL_HOUDAI, &this[CntPlayer].model.pD3DXBuffMat,
			&this[CntPlayer].model.nNumMat, &this[CntPlayer].model.pD3DXMesh,
			&this[CntPlayer].model.pD3DVtxBuff, &this[CntPlayer].model.pD3DIdxBuff,
			&this[CntPlayer].model.nNumVertex, &this[CntPlayer].model.nNumPolygon,
			&this[CntPlayer].model.nNumVertexIndex, &this[CntPlayer].model.pD3DTexture);

		//���_�J���[���v���C���[�F�ɕύX
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].model.pD3DVtxBuff,
			this[CntPlayer].model.pD3DIdxBuff, this[CntPlayer].model.nNumPolygon, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&this[CntPlayer].model.pD3DTexture);	// �ǂݍ��ރ������[
	

		/////////////////////////////////////////////////////////////////////////////////////////�C��
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai = &this[CntPlayer];

		// X�t�@�C���̓ǂݍ���
		LoadMesh(MODEL_HOUTOU, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DXBuffMat,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumMat, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DXMesh,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DVtxBuff, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DIdxBuff,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumVertex, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumPolygon,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumVertexIndex, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DTexture);

		//���_�J���[���v���C���[�F�ɕύX
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DVtxBuff,
			this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DIdxBuff, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumPolygon, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DTexture);	// �ǂݍ��ރ������[

		///////////////////////////////////////////////////////////////////////////////////////////////�C�g
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts = &this[CntPlayer].parts[PARTSTYPE_HOUTOU];

		// X�t�@�C���̓ǂݍ���
		LoadMesh(MODEL_HOUSIN, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DXBuffMat,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumMat, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DXMesh,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DVtxBuff, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DIdxBuff,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumVertex, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumPolygon,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumVertexIndex, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DTexture);

			//���_�J���[���v���C���[�F�ɕύX
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DVtxBuff,
			this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DIdxBuff, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumPolygon, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DTexture);	// �ǂݍ��ރ������[


		///////////////////////////////////////////////////////////////////////////////////////////////////////////�C�g�ʏ�f�[�^
		// X�t�@�C���̓ǂݍ���
		LoadMesh(MODEL_HOUSIN, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXBuffMat,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumMat, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXMesh,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DVtxBuff, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DIdxBuff,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumVertex, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumPolygon,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumVertexIndex, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture);

		//���_�J���[���v���C���[�F�ɕύX
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DVtxBuff,
			this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DIdxBuff, this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumPolygon, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture);	// �ǂݍ��ރ������[


		///////////////////////////////////////////////////////////////////////////////////////////////////////�C�g�U���f�[�^

		// X�t�@�C���̓ǂݍ���
		LoadMesh(MODEL_HOUSINMO, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DXBuffMat,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumMat, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DXMesh,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DVtxBuff, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DIdxBuff,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumVertex, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumPolygon,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumVertexIndex, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DTexture);

		//���_�J���[���v���C���[�F�ɕύX
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DVtxBuff,
			this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DIdxBuff, this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumPolygon, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DTexture);	// �ǂݍ��ރ������[

	}



	//�������i�K�ō��W�Ɗp�x�������_���Őݒ�
	this[0].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[1].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[2].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));
	this[3].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));

	this[0].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[1].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[2].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[3].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));

	//��������̏�Ԃň�x�J�������̏���������
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		D3DXVECTOR3 RayStart = this[CntPlayer].GetPos();
		RayStart.y += 10.0f;
		D3DXVECTOR3 RayEnd = this[CntPlayer].GetPos();
		RayEnd.y -= 1000.0f;

		float ReturnPosY = 0.0f;
		D3DXVECTOR3 FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		field->FieldHitGetSphereVec(RayStart, RayEnd, &FieldNorVec, &ReturnPosY);

		D3DXVECTOR3 SetPos = RayStart;
		SetPos.y = ReturnPosY;
		this[CntPlayer].SetPos(SetPos);
		this[CntPlayer].SetFieldNorVec(FieldNorVec);
		this[CntPlayer].SetQ(CntPlayer);
		this[CntPlayer].SetCamera(CntPlayer, false);

		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		CAMERA *cam = GetCamera();
		cam[CntPlayer].at.x = SetPos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = SetPos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = SetPos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = SetPos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = SetPos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = SetPos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}

}

//=============================================================================
// �ď���������
//=============================================================================
void PLAYER_HONTAI::Reinit(FIELD *field)
{
	//PLAYER ������
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//�C��
		this[CntPlayer].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetOldPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetRot(D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		this[CntPlayer].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[CntPlayer].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetUse(true);
		this[CntPlayer].SetQrot(0.0f);

		this[CntPlayer].FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this[CntPlayer].Bpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for(int i=0;i<3;i++) this[CntPlayer].Bmove[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this[CntPlayer].speed = 0.0f;
		this[CntPlayer].speedbuff = 1.0f;
		this[CntPlayer].speedbufftime = 0.0f;
		this[CntPlayer].Brot = 0.0f;
		this[CntPlayer].BFlag = 0;
		this[CntPlayer].GetMorphing = false;
		this[CntPlayer].Morphing = false;
		this[CntPlayer].MorphingTime = MORPHING_TIME;
		this[CntPlayer].MorphingEnd = true;
		this[CntPlayer].speedbuffsignal = false;
		this[CntPlayer].dashFlag = false;
		this[CntPlayer].KiriSignal = false;
		this[CntPlayer].KiriItemTime = 0.0f;
		this[CntPlayer].BackCameraItemSignal = false;
		this[CntPlayer].BackCameraItemTime = 0.0f;
		this[CntPlayer].AmmoCnt = PLAYER_AMMOPOWER_NORMAL;
		this[CntPlayer].AmmoBornTime = 0.0f;
		this[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
		this[CntPlayer].vital = this[CntPlayer].oldvital = PLAYER_VITAL_MAX;
		this[CntPlayer].MorphingSignal = NoMorphing;

		//�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//�C�g								 
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		ResetModel(&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL]);
	}

	//�������i�K�ō��W�Ɗp�x�������_���Őݒ�
	this[0].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[1].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[2].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));
	this[3].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));

	this[0].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[1].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[2].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[3].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));

	//��������̏�Ԃň�x�J�������̏���������
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		D3DXVECTOR3 RayStart = this[CntPlayer].GetPos();
		RayStart.y += 10.0f;
		D3DXVECTOR3 RayEnd = this[CntPlayer].GetPos();
		RayEnd.y -= 1000.0f;

		float ReturnPosY = 0.0f;
		D3DXVECTOR3 FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		field->FieldHitGetSphereVec(RayStart, RayEnd, &FieldNorVec, &ReturnPosY);

		D3DXVECTOR3 SetPos = RayStart;
		SetPos.y = ReturnPosY;
		this[CntPlayer].SetPos(SetPos);
		this[CntPlayer].SetFieldNorVec(FieldNorVec);
		this[CntPlayer].SetQ(CntPlayer);
		this[CntPlayer].SetCamera(CntPlayer,false);

		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		CAMERA *cam = GetCamera();
		cam[CntPlayer].at.x = SetPos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = SetPos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = SetPos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = SetPos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = SetPos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = SetPos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}

}

//=============================================================================
// �ď��������� �l�b�g�ΐ�O
//=============================================================================
void PLAYER_HONTAI::ReinitNet(int MyNumber)
{

}

//=============================================================================
// �I������
//=============================================================================
void PLAYER_HONTAI::Uninit(void)
{

}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void PLAYER_HONTAI::Update(EFFECT*effect, BULLET*bullet, SHADOW*shadow, FADE *fade, bool Netflag, int MyNumber)
{
	//���l���񂾂��v�Z�B�O�l���񂾂�Q�[���I���B���̃V�[����
	int deadcnt = 0;
	//�v���C���[�l�������[�v
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool use = this[CntPlayer].GetUse();
		if (use == false) deadcnt++;
		if (deadcnt >= 3)
		{
			fade->SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
			SetGameSceneFlag(false);
		}
	}

	//�v���C���[����
	//�v���C���[�l�������[�v ���[�J���ΐ�
	if (Netflag == false)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			bool use = this[CntPlayer].GetUse();
			//�����Ă���ΐ���\
			if (use)
			{
				//this[CntPlayer].SetMoveL2R2(CntPlayer, Netflag);
				this[0].SetMoveL(CntPlayer, &effect[0], Netflag);
				this[0].SetQ(CntPlayer);
				this[0].SetCamera(CntPlayer, Netflag);
				//this[CntPlayer].SetBulletALLMoveL2R2Ver(CntPlayer);
				this[0].SetBulletALL(CntPlayer, &bullet[0], shadow, Netflag);
				this[0].SetKiri(CntPlayer);
				this[0].SetMorphing(CntPlayer);
			}

			//����ȊO�̓J������������
			else
			{
				this[CntPlayer].SetQ(CntPlayer);

				D3DXVECTOR3 SetPos = this[CntPlayer].GetPos();
				D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
				D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
				D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

				CAMERA *cam = GetCamera();
				cam[CntPlayer].at.x = SetPos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
				cam[CntPlayer].at.y = SetPos.y + (HousinRot.x*100.0f);
				cam[CntPlayer].at.z = SetPos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

				cam[CntPlayer].pos.x = SetPos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
				cam[CntPlayer].pos.y = SetPos.y + POS_H_CAM;
				cam[CntPlayer].pos.z = SetPos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
			}
		}
	}

	//�l�b�g�ΐ�
	else
	{
		bool use = this[MyNumber].GetUse();

		//old�ۑ�
		//D3DXVECTOR3 pos = this[MyNumber].GetPos();
		//D3DXVECTOR3 HoudaiRot = this[MyNumber].GetRot();
		//D3DXVECTOR3 HoutouRot = this[MyNumber].parts[PARTSTYPE_HOUTOU].GetRot();
		//D3DXVECTOR3 HousinRot = this[MyNumber].parts[PARTSTYPE_HOUSIN].GetRot();
		//���W��ۑ�
		//this[MyNumber].SetOldPos(pos);
		//this[MyNumber].SetOldRot(HoudaiRot);
		//this[MyNumber].parts[PARTSTYPE_HOUTOU].SetOldRot(HoutouRot);
		//this[MyNumber].parts[PARTSTYPE_HOUSIN].SetOldRot(HousinRot);
		//�����Ă���ΐ���\
		if (use)
		{
			//�l�b�g�p�t���O�������@1�t���[�����̕ύX��ۑ����ĕύX������΃T�[�o�[�Ƀf�[�^�𑗂�
			this[MyNumber].GetMorphing = false;
			this[MyNumber].Bpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			for (int i = 0; i < 3; i++) this[MyNumber].Bmove[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this[CntPlayer].BFlag = 0;
				this[CntPlayer].oldvital = this[CntPlayer].vital;
			}
			for(int i=0;i< OBJECT_PLAYER_MAX;i++) this[0].SetMoveLtype0(i, &effect[0], Netflag);
			//this[0].SetMoveL(MyNumber, &effect[0], Netflag);
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this[0].SetQ(CntPlayer);
			}
			this[0].SetCamera(MyNumber, Netflag);
			//this[MyNumber].SetBulletALLMoveL2R2Ver(MyNumber);
			this[0].SetBulletALL(MyNumber, &bullet[0], shadow, Netflag);
			this[0].SetKiri(MyNumber);
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this[0].SetMorphing(CntPlayer);
			}
		}

		//����ȊO�̓J������������
		else
		{
			this[MyNumber].SetQ(MyNumber);

			D3DXVECTOR3 SetPos = this[MyNumber].GetPos();
			D3DXVECTOR3 HoudaiRot = this[MyNumber].GetRot();
			D3DXVECTOR3 HoutouRot = this[MyNumber].parts[PARTSTYPE_HOUTOU].GetRot();
			D3DXVECTOR3 HousinRot = this[MyNumber].parts[PARTSTYPE_HOUSIN].GetRot();

			CAMERA *cam = GetCamera();
			cam[MyNumber].at.x = SetPos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
			cam[MyNumber].at.y = SetPos.y + (HousinRot.x*100.0f);
			cam[MyNumber].at.z = SetPos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

			cam[MyNumber].pos.x = SetPos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[MyNumber].len;
			cam[MyNumber].pos.y = SetPos.y + POS_H_CAM;
			cam[MyNumber].pos.z = SetPos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[MyNumber].len;
		}

	}
}

//=============================================================================
// �G�l�~�[�X�V����
//=============================================================================
//void PLAYER_HONTAI::Update(void)
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
	//		SetPositionShadow(g_EnemyHoudai[CntEnemy].parameter.shadowIdx,
	//			D3DXVECTOR3(g_EnemyHoudai[CntEnemy].parameter.pos.x, 0.0f, g_EnemyHoudai[CntEnemy].parameter.pos.z), g_EnemyHoudai[CntEnemy].parameter.scl);
	//	}
	//}
//}

//=============================================================================
// �`�揈��
//=============================================================================
void PLAYER_HONTAI::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//-----------------------------------------------------�e
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);

			//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
			D3DXVECTOR3 PlayerUpToFieldNorVec = this[CntPlayer].GetFieldNorUpNorCross();
			float Qrot= this[CntPlayer].GetQrot();
			D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
			D3DXMATRIX mtxWorldOYA=this[CntPlayer].GetMatrix();
			D3DXVECTOR3 scl = this[CntPlayer].GetScl();
			D3DXVECTOR3 rot = this[CntPlayer].GetRot();
			D3DXVECTOR3 pos = this[CntPlayer].GetPos();

			//q=(rotVecAxis�@��)*(g_Player.rot��])
			D3DXQuaternionRotationAxis(&q, &PlayerUpToFieldNorVec, -Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &q);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldOYA);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldOYA, &mtxWorldOYA, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

			D3DXMatrixMultiply(&mtxWorldOYA, &mtxWorldOYA, &mtxRot);
			D3DXMatrixMultiply(&mtxWorldOYA, &mtxWorldOYA, &mtxQ);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldOYA, &mtxWorldOYA, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldOYA);
			this[CntPlayer].SetMatrix(mtxWorldOYA);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)this[CntPlayer].model.pD3DXBuffMat->GetBufferPointer();

			// �`��
			for (int nCntMat = 0; nCntMat < (int)this[CntPlayer].model.nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, this[CntPlayer].model.pD3DTexture);
				}

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);
				// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetStreamSource(0, this[CntPlayer].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetIndices(this[CntPlayer].model.pD3DIdxBuff);
				//�`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this[CntPlayer].model.nNumVertex, 0, this[CntPlayer].model.nNumPolygon);
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
			D3DXMATRIX mtxWorldKO = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetMatrix();
			D3DXVECTOR3 scl = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetScl();
			D3DXVECTOR3 rot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
			D3DXVECTOR3 pos = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetPos();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldKO);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldKO, &mtxWorldKO, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorldKO, &mtxWorldKO, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldKO, &mtxWorldKO, &mtxTranslate);

			if (this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai != NULL)
			{
				//-------------------------------------------------�e�̃��[���h�}�g���N�X���擾
				D3DXMATRIX ParentmtxWorldKO = this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai->GetMatrix();
				D3DXMatrixMultiply(&mtxWorldKO, &mtxWorldKO, &ParentmtxWorldKO);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldKO);
			this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetMatrix(mtxWorldKO);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DTexture);
				}
				// �`��
				{
					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);
					// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetStreamSource(0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetIndices(this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DIdxBuff);
					//�`��
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumVertex, 0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumPolygon);
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
			D3DXMATRIX mtxWorldMAGO = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetMatrix();
			D3DXVECTOR3 scl = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetScl();
			D3DXVECTOR3 rot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();
			D3DXVECTOR3 pos = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetPos();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldMAGO);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldMAGO, &mtxWorldMAGO, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorldMAGO, &mtxWorldMAGO, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldMAGO, &mtxWorldMAGO, &mtxTranslate);



			if (this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts != NULL)
			{
				//-------------------------------------------------�e�̃��[���h�}�g���N�X���擾
				D3DXMATRIX ParentmtxWorldMAGO = this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts->GetMatrix();
				D3DXMatrixMultiply(&mtxWorldMAGO, &mtxWorldMAGO, &ParentmtxWorldMAGO);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldMAGO);
			this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetMatrix(mtxWorldMAGO);
			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DTexture);
				}
				// �`��
				{
					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);
					// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetStreamSource(0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetIndices(this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DIdxBuff);
					//�`��
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumVertex, 0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumPolygon);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// ���b�V���J���[���Z�b�g
//=============================================================================
void PLAYER_HONTAI::SetPlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff, DWORD nNumPolygon, int CntPlayer)
{

	VERTEX_3D *pVtx;
	WORD *pIdx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// ���ˌ��̐ݒ�
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[CntPlayer];
	}
	// ���_�f�[�^���A�����b�N����
	pD3DVtxBuff->Unlock();
	pD3DIdxBuff->Unlock();
}

//=============================================================================
// �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetMoveABL(int CntPlayer, EFFECT *effect)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
	D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

	//���W��ۑ�
	this[CntPlayer].SetOldPos(pos);

	int dir = FRONT_VEC;
	//�ړ�����
	if (IsButtonPressed(CntPlayer, BUTTON_A))
	{
		dir = FRONT_VEC;
		this[CntPlayer].speed += VALUE_MOVE;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_B))
	{
		dir = BACK_VEC;
		this[CntPlayer].speed -= VALUE_MOVE;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		this[CntPlayer].speed *= MOVE_INERTIA_MOMENT;
	}

	//���_�ω��̃A�i���O�l�����ɑ�����ăA�i���O����Ő���
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//����
	if (!IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT) && !IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		dir = FRONT_VEC;
	}
	else if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
	{
		HoudaiRot.y += LAnalogX * dir*this[CntPlayer].speedbuff;
	}
	else if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		HoudaiRot.y += LAnalogX * dir*this[CntPlayer].speedbuff;
	}

	//�p�x�̐����l
	if (HoudaiRot.y >= D3DX_PI*2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// �ړ����x�̐���
	if (this[CntPlayer].speed >= VALUE_MOVE_MAX) this[CntPlayer].speed = VALUE_MOVE_MAX;
	else if (this[CntPlayer].speed <= -VALUE_MOVE_MAX) this[CntPlayer].speed = -VALUE_MOVE_MAX;

	// �v���C���[�̍��W���X�V
	pos.x -= sinf(HoudaiRot.y) * (this[CntPlayer].speed * this[CntPlayer].speedbuff);
	pos.z -= cosf(HoudaiRot.y) * (this[CntPlayer].speed * this[CntPlayer].speedbuff);

	//�X�s�[�h�o�t���Ԍ���
	if (this[CntPlayer].speedbuffsignal == true)
	{
		this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		effect->SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this[CntPlayer].speedbufftime <= 0.0f)
		{
			this[CntPlayer].speedbuffsignal = false;
			this[CntPlayer].speedbuff = VALUE_SPEEDBUFF_SUB;
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
	this[CntPlayer].SetPos(pos);
	this[CntPlayer].SetRot(HoudaiRot);
	this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetRot(HoutouRot);
	this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetRot(HousinRot);

}

//=============================================================================
// �J��������(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetCamera(int CntPlayer, bool Netflag)
{
	if (Netflag == false)
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		CAMERA *cam = GetCamera();
		//�o�b�N�J��������
		//�o�b�N�J�����I���@�J�������_�A�����_
		//Y�{�^���������Ă���������́A�o�b�N�J�����A�C�e����ON�ɂȂ��Ă���Ƃ��̓J�������]
		if (IsButtonPressed(CntPlayer, BUTTON_Y) || this[CntPlayer].BackCameraItemSignal == true)
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
		if (this[CntPlayer].BackCameraItemSignal == true && IsButtonPressed(CntPlayer, BUTTON_Y))
		{
			cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
			cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
			cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

			cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
			cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
			cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		}
		//�o�b�N�J�����̎��ԏ���
		if (this[CntPlayer].BackCameraItemSignal == true)
		{
			this[CntPlayer].BackCameraItemTime += 1.0f;
			if (this[CntPlayer].BackCameraItemTime >= BACKCAMERA_TIME)
			{
				this[CntPlayer].BackCameraItemTime = 0.0f;
				this[CntPlayer].BackCameraItemSignal = false;
			}
		}
	}
	else
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		CAMERA *cam = GetCamera();
		//�o�b�N�J��������
		//�o�b�N�J�����I���@�J�������_�A�����_
		//Y�{�^���������Ă���������́A�o�b�N�J�����A�C�e����ON�ɂȂ��Ă���Ƃ��̓J�������]
		if (IsButtonPressed(0, BUTTON_Y) || this[CntPlayer].BackCameraItemSignal == true)
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
		if (this[CntPlayer].BackCameraItemSignal == true && IsButtonPressed(CntPlayer, BUTTON_Y))
		{
			cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
			cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
			cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

			cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
			cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
			cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		}
		//�o�b�N�J�����̎��ԏ���
		if (this[CntPlayer].BackCameraItemSignal == true)
		{
			this[CntPlayer].BackCameraItemTime += 1.0f;
			if (this[CntPlayer].BackCameraItemTime >= BACKCAMERA_TIME)
			{
				this[CntPlayer].BackCameraItemTime = 0.0f;
				this[CntPlayer].BackCameraItemSignal = false;
			}
		}
	}
}

//=============================================================================
// �ړ�����(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetMoveL(int CntPlayer, EFFECT *effect, bool Netflag)
{
	if (Netflag == false)
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 move = this[CntPlayer].GetMove();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();

		//�ۑ�
		this[CntPlayer].SetOldPos(pos);
		this[CntPlayer].SetOldRot(HoudaiRot);

		int dir = FRONT_VEC;

		//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
		float LAnalogX = 0.0f;		//�c����
		float LAnalogY = 0.0f;		//������
		float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

		//�_�b�V������
		if (this[CntPlayer].speedbuffsignal == true)
		{
			//�X�s�[�h�o�t���Ԍ���
			this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
			this[CntPlayer].dashFlag = true;

			// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
			D3DXVECTOR3 EffctSpeedupPos = pos;
			effect->SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

			if (this[CntPlayer].speedbufftime <= 0.0f)
			{
				this[CntPlayer].dashFlag = false;
				this[CntPlayer].speedbuffsignal = false;
			}
		}


		if (this[CntPlayer].dashFlag == true)
		{
			DashRate = PLAYER_VALUE_DASHRATE;
		}

		//�ړ�����
		if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_UP) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_DOWN) ||
			IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
		{
			DIJOYSTATE2 *Button = GetIsButton(CntPlayer);

			LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
			LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
			dir = FRONT_VEC;
		}
		//������͂͌�ޒ��Ɍ��胊�o�[�X����
		if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_DOWN))
		{
			dir = BACK_VEC;
		}
		// ���ړ����͈ړ��ʂɊ�����������
		else
		{
			this[CntPlayer].dashFlag = false;
		}
		if (LAnalogY > 0.0f) LAnalogX *= -1;

		//�ړ��ʂ𔽉f
		HoudaiRot.y += LAnalogX * dir;
		move.x = LAnalogY * sinf(HoudaiRot.y);
		move.z = LAnalogY * cosf(HoudaiRot.y);

		//�v���C���[���W���X�V
		pos += move;

		//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
		this[CntPlayer].SetPos(pos);
		this[CntPlayer].SetRot(HoudaiRot);

		SetCameraR(CntPlayer, Netflag);
	}
	else
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 move = this[CntPlayer].GetMove();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();

		//���W��ۑ�
		this[CntPlayer].SetOldPos(pos);
		this[CntPlayer].SetOldRot(HoudaiRot);

		int dir = FRONT_VEC;

		//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
		float LAnalogX = 0.0f;		//�c����
		float LAnalogY = 0.0f;		//������
		float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

		//�_�b�V������
		if (this[CntPlayer].speedbuffsignal == true)
		{
			//�X�s�[�h�o�t���Ԍ���
			this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
			this[CntPlayer].dashFlag = true;

			// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
			D3DXVECTOR3 EffctSpeedupPos = pos;
			effect->SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

			if (this[CntPlayer].speedbufftime <= 0.0f)
			{
				this[CntPlayer].dashFlag = false;
				this[CntPlayer].speedbuffsignal = false;
			}
		}


		if (this[CntPlayer].dashFlag == true)
		{
			DashRate = PLAYER_VALUE_DASHRATE;
		}

		//�ړ�����
		if (IsButtonPressed(0, BUTTON_ANALOG_L_UP) || IsButtonPressed(0, BUTTON_ANALOG_L_DOWN) ||
			IsButtonPressed(0, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(0, BUTTON_ANALOG_L_RIGHT))
		{
			DIJOYSTATE2 *Button = GetIsButton(0);

			LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
			LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
			dir = FRONT_VEC;
		}
		//������͂͌�ޒ��Ɍ��胊�o�[�X����
		if (IsButtonPressed(0, BUTTON_ANALOG_L_DOWN))
		{
			dir = BACK_VEC;
		}
		// ���ړ����͈ړ��ʂɊ�����������
		else
		{
			this[CntPlayer].dashFlag = false;
		}
		if (LAnalogY > 0.0f) LAnalogX *= -1;

		//�ړ��ʂ𔽉f
		HoudaiRot.y += LAnalogX * dir;
		move.x = LAnalogY * sinf(HoudaiRot.y);
		move.z = LAnalogY * cosf(HoudaiRot.y);

		//�v���C���[���W���X�V
		pos += move;

		//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
		this[CntPlayer].SetPos(pos);
		this[CntPlayer].SetRot(HoudaiRot);

		SetCameraR(CntPlayer, Netflag);

	}
}

//=============================================================================
// �ړ�����(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetMoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag)
{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 move = this[CntPlayer].GetMove();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();

		//�ۑ�
		this[CntPlayer].SetOldPos(pos);
		this[CntPlayer].SetOldRot(HoudaiRot);

		int dir = FRONT_VEC;

		//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
		float LAnalogX = 0.0f;		//�c����
		float LAnalogY = 0.0f;		//������
		float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

		//�_�b�V������
		if (this[CntPlayer].speedbuffsignal == true)
		{
			//�X�s�[�h�o�t���Ԍ���
			this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
			this[CntPlayer].dashFlag = true;

			// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
			D3DXVECTOR3 EffctSpeedupPos = pos;
			effect->SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

			if (this[CntPlayer].speedbufftime <= 0.0f)
			{
				this[CntPlayer].dashFlag = false;
				this[CntPlayer].speedbuffsignal = false;
			}
		}


		if (this[CntPlayer].dashFlag == true)
		{
			DashRate = PLAYER_VALUE_DASHRATE;
		}

		//�ړ�����
		if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_UP) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_DOWN) ||
			IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
		{
			DIJOYSTATE2 *Button = GetIsButton(CntPlayer);

			LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
			LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
			dir = FRONT_VEC;
		}
		//������͂͌�ޒ��Ɍ��胊�o�[�X����
		if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_DOWN))
		{
			dir = BACK_VEC;
		}
		// ���ړ����͈ړ��ʂɊ�����������
		else
		{
			this[CntPlayer].dashFlag = false;
		}
		if (LAnalogY > 0.0f) LAnalogX *= -1;

		//�ړ��ʂ𔽉f
		HoudaiRot.y += LAnalogX * dir;
		move.x = LAnalogY * sinf(HoudaiRot.y);
		move.z = LAnalogY * cosf(HoudaiRot.y);

		//�v���C���[���W���X�V
		pos += move;

		//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
		this[CntPlayer].SetPos(pos);
		this[CntPlayer].SetRot(HoudaiRot);

		SetCameraR(CntPlayer, Netflag);
}

//=============================================================================
// �J��������(R�X�e�B�b�N�Ő���)
//=============================================================================
void PLAYER_HONTAI::SetCameraR(int CntPlayer, bool Netflag)
{
	if (Netflag == false)
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		//�ۑ�
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetOldRot(HoutouRot);
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetOldRot(HousinRot);

		CAMERA *cam = GetCamera();

		//����p�ω���R�X�e�B�b�N�A�i���O�l���g�p
		float RAnalogX = 0.0f;		//�c����
		float RAnalogY = 0.0f;		//������

		//����p����
		if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_UP) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_DOWN) ||
			IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_RIGHT))
		{
			DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
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
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetRot(HoutouRot);
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetRot(HousinRot);


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
	else
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		//�ۑ�
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetOldRot(HoutouRot);
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetOldRot(HousinRot);

		CAMERA *cam = GetCamera();

		//����p�ω���R�X�e�B�b�N�A�i���O�l���g�p
		float RAnalogX = 0.0f;		//�c����
		float RAnalogY = 0.0f;		//������

		//����p����
		if (IsButtonPressed(0, BUTTON_ANALOG_R_UP) || IsButtonPressed(0, BUTTON_ANALOG_R_DOWN) ||
			IsButtonPressed(0, BUTTON_ANALOG_R_LEFT) || IsButtonPressed(0, BUTTON_ANALOG_R_RIGHT))
		{
			DIJOYSTATE2 *Button = GetIsButton(0);
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
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetRot(HoutouRot);
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetRot(HousinRot);
	}
}

//=============================================================================
// �ړ�����(LR�X�e�B�b�N�ŃL���^�s���ړ�����)
//=============================================================================
void PLAYER_HONTAI::SetMoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag)
{
	if (Netflag==false)
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 move = this[CntPlayer].GetMove();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();

		//���W��ۑ�
		this[CntPlayer].SetOldPos(pos);

		//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
		float L2 = 0.0f;		//�c����
		float R2 = 0.0f;		//������
		float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

		//�_�b�V������
		if (this[CntPlayer].speedbuffsignal == true)
		{
			//�X�s�[�h�o�t���Ԍ���
			this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
			this[CntPlayer].dashFlag = true;

			// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
			D3DXVECTOR3 EffctSpeedupPos = pos;
			effect->SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

			if (this[CntPlayer].speedbufftime <= 0.0f)
			{
				this[CntPlayer].dashFlag = false;
				this[CntPlayer].speedbuffsignal = false;
			}
		}

		if (this[CntPlayer].dashFlag == true)
		{
			DashRate = PLAYER_VALUE_DASHRATE;
		}

		//���E�̃L���^�s����O�i���ǂ��炩�̃t���O
		//���L���^�s���O�i����=0�A���L���^�s���̂ݑO�i=3.925f�A�E�L���^�s���̂ݑO�i=-3.925f�A���L���^�s���̂݌��=-0.785f�A�E�L���^�s���̂݌��=0.785f�A
		float Lrot = 3.925f;
		float Rrot = -3.925f;
		int Ldir = 1;
		int Rdir = 1;
		//�ړ�����
		if (IsButtonPressed(CntPlayer, BUTTON_L1))
		{
			Lrot = -0.785f;
			Ldir *= -1;
		}
		if (IsButtonPressed(CntPlayer, BUTTON_R1))
		{
			Rrot = 0.785f;
			Rdir *= -1;
		}

		//���L���^�s���g�p
		if (IsButtonPressed(CntPlayer, BUTTON_L2) && IsButtonPressed(CntPlayer, BUTTON_R2))
		{
			DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
			//���͒����l32767�@R2�ŏ�0�@L2�ő�64000
			L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
			R2 = L2;

		}
		//���L���^�s���̂ݎg�p
		else if (IsButtonPressed(CntPlayer, BUTTON_L2))
		{
			DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
			L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
			HoudaiRot.y += 0.1f*Ldir;
		}
		//�E�L���^�s���̂ݎg�p
		else if (IsButtonPressed(CntPlayer, BUTTON_R2))
		{
			DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
			float IZbuf = Button->lZ * PLAYER_MOVE_RATE_LR2;
			R2 = IZbuf;
			R2 = 32767 * PLAYER_MOVE_RATE_LR2 + (-R2) + 32767 * PLAYER_MOVE_RATE_LR2;
			HoudaiRot.y -= 0.1f*Rdir;
		}
		// ���ړ����͈ړ��ʂɊ�����������
		else
		{
			this[CntPlayer].dashFlag = false;
		}

		//�ړ��ʂ𔽉f
		move.x = R2 * sinf(HoudaiRot.y + Rrot) + L2 * sinf(HoudaiRot.y + Lrot);
		move.z = R2 * cosf(HoudaiRot.y + Rrot) + L2 * cosf(HoudaiRot.y + Lrot);

		//�v���C���[���W���X�V
		pos += move;

		//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
		this[CntPlayer].SetPos(pos);
		this[CntPlayer].SetRot(HoudaiRot);

		SetCameraR(CntPlayer, Netflag);
	}
	else
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 move = this[CntPlayer].GetMove();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();

		//���W��ۑ�
		this[CntPlayer].SetOldPos(pos);

		//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
		float L2 = 0.0f;		//�c����
		float R2 = 0.0f;		//������
		float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

		//�_�b�V������
		if (this[CntPlayer].speedbuffsignal == true)
		{
			//�X�s�[�h�o�t���Ԍ���
			this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
			this[CntPlayer].dashFlag = true;

			// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
			D3DXVECTOR3 EffctSpeedupPos = pos;
			effect->SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

			if (this[CntPlayer].speedbufftime <= 0.0f)
			{
				this[CntPlayer].dashFlag = false;
				this[CntPlayer].speedbuffsignal = false;
			}
		}

		if (this[CntPlayer].dashFlag == true)
		{
			DashRate = PLAYER_VALUE_DASHRATE;
		}

		//���E�̃L���^�s����O�i���ǂ��炩�̃t���O
		//���L���^�s���O�i����=0�A���L���^�s���̂ݑO�i=3.925f�A�E�L���^�s���̂ݑO�i=-3.925f�A���L���^�s���̂݌��=-0.785f�A�E�L���^�s���̂݌��=0.785f�A
		float Lrot = 3.925f;
		float Rrot = -3.925f;
		int Ldir = 1;
		int Rdir = 1;
		//�ړ�����
		if (IsButtonPressed(0, BUTTON_L1))
		{
			Lrot = -0.785f;
			Ldir *= -1;
		}
		if (IsButtonPressed(0, BUTTON_R1))
		{
			Rrot = 0.785f;
			Rdir *= -1;
		}

		//���L���^�s���g�p
		if (IsButtonPressed(0, BUTTON_L2) && IsButtonPressed(0, BUTTON_R2))
		{
			DIJOYSTATE2 *Button = GetIsButton(0);
			//���͒����l32767�@R2�ŏ�0�@L2�ő�64000
			L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
			R2 = L2;

		}
		//���L���^�s���̂ݎg�p
		else if (IsButtonPressed(0, BUTTON_L2))
		{
			DIJOYSTATE2 *Button = GetIsButton(0);
			L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
			HoudaiRot.y += 0.1f*Ldir;
		}
		//�E�L���^�s���̂ݎg�p
		else if (IsButtonPressed(0, BUTTON_R2))
		{
			DIJOYSTATE2 *Button = GetIsButton(0);
			float IZbuf = Button->lZ * PLAYER_MOVE_RATE_LR2;
			R2 = IZbuf;
			R2 = 32767 * PLAYER_MOVE_RATE_LR2 + (-R2) + 32767 * PLAYER_MOVE_RATE_LR2;
			HoudaiRot.y -= 0.1f*Rdir;
		}
		// ���ړ����͈ړ��ʂɊ�����������
		else
		{
			this[CntPlayer].dashFlag = false;
		}

		//�ړ��ʂ𔽉f
		move.x = R2 * sinf(HoudaiRot.y + Rrot) + L2 * sinf(HoudaiRot.y + Lrot);
		move.z = R2 * cosf(HoudaiRot.y + Rrot) + L2 * cosf(HoudaiRot.y + Lrot);

		//�v���C���[���W���X�V
		pos += move;

		//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
		this[CntPlayer].SetPos(pos);
		this[CntPlayer].SetRot(HoudaiRot);

		SetCameraR(CntPlayer, Netflag);
	}
}

//=============================================================================
// �N�H�[�^�j�I������
//=============================================================================
void PLAYER_HONTAI::SetQ(int CntPlayer)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 FieldNorVec = this[CntPlayer].GetFieldNorVec();
	D3DXVECTOR3 FieldNorUpNorCross = this[CntPlayer].GetFieldNorUpNorCross();
	float Qrot = this[CntPlayer].GetQrot();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
	D3DXVec3Cross(&FieldNorUpNorCross, &FieldNorVec, &UpVec);
	float Upkakezan = D3DXVec3Dot(&FieldNorVec, &UpVec);
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
	this[CntPlayer].SetQrot(Qrot);
	this[CntPlayer].SetFieldNorUpNorCross(FieldNorUpNorCross);

}

//=============================================================================
// �o���b�g�֘A����
//=============================================================================
void PLAYER_HONTAI::SetBulletALL(int CntPlayer, BULLET *bullet, SHADOW *shadow, bool Netflag)
{
	if (Netflag == false)
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		D3DXVECTOR3 FieldNorVec = this[CntPlayer].GetFieldNorVec();

		D3DXVECTOR3 Frontvec;
		Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
		Frontvec.y = 0.0f;
		Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

		//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
		D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FieldNorVec, &Frontvec);
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
			this[CntPlayer].Brot = acosf(cossita);
		}
		else
		{
			this[CntPlayer].Brot = 1.57f;		//�������x�N�g��rot=0.0f�A������x�N�g��rot=3.14�A�ɑ΂��Ă̑O�����x�N�g����rot=1.57f�B
		}
		this[CntPlayer].Brot -= 1.57f;

		//�v���C���[pos���甭�˕����ɏ������炵���l
		//�n�ʂ̌X���ɉ����Ĕ��˂���Ƃ��͖��Ȃ��B���̌X�����獶�E�ɉ�]���Ă鎞����pos����������
		D3DXVECTOR3 BposStart;
		BposStart.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
		BposStart.y = pos.y + (sinf(this[CntPlayer].Brot - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
		BposStart.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);

		D3DXVECTOR3 BmoveRot;
		BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
		BmoveRot.y = sinf(this[CntPlayer].Brot - HousinRot.x);
		BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

		D3DXVECTOR3 bulletmove;
		bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
		bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
		bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

		// �e����
		if (this[CntPlayer].AmmoCnt > 0)
		{
			//if (IsButtonTriggered(CntPlayer, BUTTON_X))
			//{
			if (IsButtonTriggered(CntPlayer, BUTTON_R1))
			{
				bullet->SetBullet(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);
				//�g�U�e����
				if (this[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
				{
					D3DXVECTOR3 leftB, rightB;
					leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
						bulletmove.y,
						-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
					rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
						bulletmove.y,
						-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
					bullet->SetBullet(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);
					bullet->SetBullet(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);

				}
				//�c�e�����炷
				this[CntPlayer].AmmoCnt -= 1;

				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_attack03);
			}
		}
		//�c�e���� ��莞�Ԍo�߂�1�Â�����
		if (this[CntPlayer].AmmoCnt < PLAYER_AMMOPOWER_NORMAL) this[CntPlayer].AmmoBornTime += BORN_AMMO_ADDTIME;
		if (this[CntPlayer].AmmoBornTime >= BORN_AMMO_MAXTIME)
		{
			this[CntPlayer].AmmoCnt++;
			this[CntPlayer].AmmoBornTime = 0.0f;
		}
	}
	else
	{
		//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
		D3DXVECTOR3 pos = this[CntPlayer].GetPos();
		D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
		D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
		D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

		D3DXVECTOR3 FieldNorVec = this[CntPlayer].GetFieldNorVec();
		//D3DXVECTOR3 FieldNorUpNorCross = this[CntPlayer].GetFieldNorUpNorCross();

		D3DXVECTOR3 Frontvec;
		Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
		Frontvec.y = 0.0f;
		Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

		//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
		D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FieldNorVec, &Frontvec);
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
			this[CntPlayer].Brot = acosf(cossita);
		}
		else
		{
			this[CntPlayer].Brot = 1.57f;		//�������x�N�g��rot=0.0f�A������x�N�g��rot=3.14�A�ɑ΂��Ă̑O�����x�N�g����rot=1.57f�B
		}
		this[CntPlayer].Brot -= 1.57f;


		//�v���C���[pos���甭�˕����ɏ������炵���l
		//�n�ʂ̌X���ɉ����Ĕ��˂���Ƃ��͖��Ȃ��B���̌X�����獶�E�ɉ�]���Ă鎞����pos����������
		D3DXVECTOR3 BposStart;
		BposStart.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
		BposStart.y = pos.y + (sinf(this[CntPlayer].Brot - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
		BposStart.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);


		D3DXVECTOR3 BmoveRot;
		BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
		BmoveRot.y = sinf(this[CntPlayer].Brot - HousinRot.x);
		BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

		D3DXVECTOR3 bulletmove;
		bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
		bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
		bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

		// �e����
		if (this[CntPlayer].AmmoCnt > 0)
		{
			//if (IsButtonTriggered(CntPlayer, BUTTON_X))
			//{
			if (IsButtonTriggered(0, BUTTON_R1))
			{
				//-----------------------------------�I�u�W�F�N�g�擪�A�h���X��ǂݍ���
				bullet->SetBullet(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);
				this[CntPlayer].Bpos = BposStart;
				this[CntPlayer].Bmove[0] = bulletmove;
				this[CntPlayer].BFlag = 1;

				//�g�U�e����
				if (this[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
				{
					D3DXVECTOR3 leftB, rightB;
					leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
						bulletmove.y,
						-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
					rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
						bulletmove.y,
						-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
					bullet->SetBullet(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);
					bullet->SetBullet(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);
					this[CntPlayer].Bmove[1] = leftB;
					this[CntPlayer].Bmove[2] = rightB;
					this[CntPlayer].BFlag = 3;

				}
				//�c�e�����炷
				this[CntPlayer].AmmoCnt -= 1;

				// SE�Đ�
				PlaySound(SOUND_LABEL_SE_attack03);
			}
		}
		//�c�e���� ��莞�Ԍo�߂�1�Â�����
		if (this[CntPlayer].AmmoCnt < PLAYER_AMMOPOWER_NORMAL) this[CntPlayer].AmmoBornTime += BORN_AMMO_ADDTIME;
		if (this[CntPlayer].AmmoBornTime >= BORN_AMMO_MAXTIME)
		{
			this[CntPlayer].AmmoCnt++;
			this[CntPlayer].AmmoBornTime = 0.0f;
		}
	}
}

//=============================================================================
// �o���b�g�֘A����
//=============================================================================
void PLAYER_HONTAI::SetBulletALLMoveL2R2Ver(int CntPlayer, BULLET *bullet, SHADOW *shadow)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
	D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

	D3DXVECTOR3 FieldNorVec = this[CntPlayer].GetFieldNorVec();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
	D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FieldNorVec, &Frontvec);
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
		this[CntPlayer].Brot = acosf(cossita);
	}
	else
	{
		this[CntPlayer].Brot = 1.57f;		//�������x�N�g��rot=0.0f�A������x�N�g��rot=3.14�A�ɑ΂��Ă̑O�����x�N�g����rot=1.57f�B
	}
	this[CntPlayer].Brot -= 1.57f;


	//�v���C���[pos���甭�˕����ɏ������炵���l
	//�n�ʂ̌X���ɉ����Ĕ��˂���Ƃ��͖��Ȃ��B���̌X�����獶�E�ɉ�]���Ă鎞����pos����������
	D3DXVECTOR3 BposStart;
	BposStart.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
	BposStart.y = pos.y + (sinf(this[CntPlayer].Brot - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
	BposStart.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);


	D3DXVECTOR3 BmoveRot;
	BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
	BmoveRot.y = sinf(this[CntPlayer].Brot - HousinRot.x);
	BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

	D3DXVECTOR3 bulletmove;
	bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

	// �e����
	if (this[CntPlayer].AmmoCnt > 0)
	{
		//if (IsButtonTriggered(CntPlayer, BUTTON_X))
		//{
		if (IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_UP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTUP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHT) ||
			IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTDOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_DOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTDOWN) ||
			IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFT) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTUP))
		{

			bullet->SetBullet(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);

			//�g�U�e����
			if (this[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
			{
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				bullet->SetBullet(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);
				bullet->SetBullet(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer, shadow);

			}
			//�c�e�����炷
			this[CntPlayer].AmmoCnt -= 1;

			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}


	//�c�e���� ��莞�Ԍo�߂�1�Â�����
	if (this[CntPlayer].AmmoCnt < PLAYER_AMMOPOWER_NORMAL) this[CntPlayer].AmmoBornTime += BORN_AMMO_ADDTIME;
	if (this[CntPlayer].AmmoBornTime >= BORN_AMMO_MAXTIME)
	{
		this[CntPlayer].AmmoCnt++;
		this[CntPlayer].AmmoBornTime = 0.0f;
	}

}

//=============================================================================
// �t�H�O����
//=============================================================================
void PLAYER_HONTAI::SetKiri(int CntPlayer)
{
	if (this[CntPlayer].KiriSignal == true)
	{
		this[CntPlayer].KiriItemTime += 1.0f;
		if (this[CntPlayer].KiriItemTime >= KIRI_TIME)
		{
			this[CntPlayer].KiriItemTime = 0.0f;
			this[CntPlayer].KiriSignal = false;
		}
	}
}

//=============================================================================
// ���[�t�B���O����
//=============================================================================
void PLAYER_HONTAI::SetMorphing(int CntPlayer)
{
	// ���[�t�B���Otrue
	if (this[CntPlayer].Morphing == true)
	{
		///////////////////////////////////////////////////////////////////////�o���b�g3���ԊJ�n
		// ���[�t�B���O���Ԍ��Z�J�n
		this[CntPlayer].MorphingTime -= 1.0f;

		// ���[�t�B���O�U���^�C�v�ɕύX�J�n
		if (this[CntPlayer].MorphingSignal == NowMorphing)
		{
			this[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
			DoMorphing(&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK],
				0.01f, &this[CntPlayer].time, &this[CntPlayer].MorphingSignal);
		}
		///////////////////////////////////////////////////////////////////////�o���b�g3���ԏI��

		// ���Ԍo�߂Ń��f�������ɖ߂�
		else if (this[CntPlayer].MorphingTime <= 0.0f)
		{
			this[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
			this[CntPlayer].MorphingSignal = NowMorphing;
			this[CntPlayer].Morphing = false;
		}
	}

	// ���[�t�B���O�I���W�i���^�C�v�ɕύX�J�n
	if (this[CntPlayer].Morphing == false && this[CntPlayer].MorphingTime <= 0.0f)
	{
		DoMorphing(&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL],
			0.01f, &this[CntPlayer].time, &this[CntPlayer].MorphingSignal);
		if (this[CntPlayer].MorphingSignal == EndMorphing)
		{
			this[CntPlayer].MorphingTime = MORPHING_TIME;
		}
	}
}

