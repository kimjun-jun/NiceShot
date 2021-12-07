/**
* @file player.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../../h/main.h"
#include "../../../h/Library/library.h"
#include "../../../h/Other/input.h"
#include "../../../h/Object/Camera/camera.h"
#include "../../../h/Map/field.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Object/Bullet/bullet.h"
#include "../../../h/Effect/effect.h"
#include "../../../h/Object/Scene/Scene.h"
#include "../../../h/net/sock.h"
#include "../../../h/Object/Player/player.h"

//�摜�̎Q�Ɩ�																											
#define	TEXTURE_MEISAI	"../data/MODEL/28513607_p4_master1200.jpg"		//!< �ǂݍ��ރe�N�X�`���t�@�C����

constexpr float	PLAYER_INIT_POSX{ 700.0f };				//!< �������W�v���C���[
constexpr float	PLAYER_INIT_POSY{ 300.0f };				//!< �������W�v���C���[
constexpr float	PLAYER_INIT_POSZ{ 700.0f };				//!< �������W�v���C���[
constexpr float	EFFECT_SPEEDUP_SIZE_X{ 25.0f };			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̕�
constexpr float	EFFECT_SPEEDUP_SIZE_Y{ 10.0f };			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̍���
constexpr int	EFFECT_SPEEDUP_TIME{ 10 };				//!< �G�t�F�N�g�X�s�[�h�A�b�v�̐�������
constexpr float	PLAYER_VALUE_DASHRATE{ 2.0f };			//!< �����ɔ�ׂẴ_�b�V���̑��x�{��
constexpr float	PLAYER_MOVE_RATE_X{ 1.0f / 40000.0f };	//!< ���E�����̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	PLAYER_MOVE_RATE_Y{ 1.0f / 200.0f };	//!< �O������̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	PLAYER_MOVE_RATE_LR2{ 1.0f / 20000.0f };//!< �L���^�s���̈ړ��ʔ{��(0�`65000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU{ 0.03f };	//!< �v���C���[�C����]��
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU_MAX{ 1.57f };//!< �v���C���[�C����]�ʍő�l
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN{ 0.01f };	//!< �v���C���[�C�g��]��
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN_MAX{ 0.2f };	//!< �v���C���[�C����]�ʍő�l
constexpr float	MOVE_INERTIA_MOMENT{ 0.8f };			//!< �ړ��ʂ̊������[�����g�A�����W���Ƃ��Ďg�p
constexpr float	VALUE_SPEEDBUFF_SUB{ 1.0f };			//!< �X�s�[�g�A�b�v���Ԃ̃f�N�������g����l
constexpr float KIRI_TIME{ 150.0f };					//!< �t�H�O���A�C�e���̗L������
constexpr float	PLAYER_VALUE_MOVE{ 0.1f };				//!< �ړ������x
constexpr float	PLAYER_VALUE_MOVE_MAX{ 4.0f };			//!< �ړ������x�̍ő�l

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
PLAYER::PLAYER(void)
{
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
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].MeshColor(np, CntPlayer);

			//�f�[�^���f
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumMat(nm);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].Mat(BuffMat);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumVertex(nv);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumPolygon(np);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumVertexIndex(nvi);
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
		this->modelOri.Attribute[CntOriginalModelNum].NumMat(nm);
		this->modelOri.Attribute[CntOriginalModelNum].Mat(BuffMat);
		this->modelOri.Attribute[CntOriginalModelNum].NumVertex(nv);
		this->modelOri.Attribute[CntOriginalModelNum].NumPolygon(np);
		this->modelOri.Attribute[CntOriginalModelNum].NumVertexIndex(nvi);
	}
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
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].~VTXBUFFER();
		}
	}
	//�J�E���g���[�v�@�I���W�i���p���f���̐�
	for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_MODEL_ORIGINAL_TYPE_MAX; CntDrawPartsNum++)
	{
		//���f�����
		this->modelOri.Vtx[CntDrawPartsNum].~VTXBUFFER();
	}
}

//=============================================================================
// ���N���X�̃A�h���X�擾
//=============================================================================
void PLAYER::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pfield = obj->GetField();
	pmysocket = obj->GetMySocket();
	pscene = obj->GetScene();
	peffect = obj->GetEffect();
	pbullet = obj->GetBullet();
}

//=============================================================================
// ����������
//=============================================================================
void PLAYER::Init(void)
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
		this->PlayerPara[CntPlayer].StandardPara.FUPFCross = VEC3_ALL0;

		//�o���b�g�p�����[�^
		this->PlayerPara[CntPlayer].BulletPara.SetBulletBornTime(0.0f);
		this->PlayerPara[CntPlayer].BulletPara.SetBulletRotY(0.0f);
		this->PlayerPara[CntPlayer].BulletPara.SetBulletStartPos(VEC3_ALL0);
		this->PlayerPara[CntPlayer].BulletPara.SetBulletStock(BULLET_MAX_STOCK);
		this->PlayerPara[CntPlayer].BulletPara.SetNetBulletShotFlagOneFrame(0);
		for (int i = 0; i < 3; i++) this->PlayerPara[CntPlayer].BulletPara.SetBulletMove(VEC3_ALL0, i);

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
		this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NoMorphing;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingStart = false;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
		this->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = false;

		//���̑��p�����[�^
		this->PlayerPara[CntPlayer].PostureVec.FNVecFunc(VEC3_ALL0);
		this->PlayerPara[CntPlayer].PostureVec.FNUNCrossFunc(VEC3_ALL0);
		this->PlayerPara[CntPlayer].PostureVec.QrotFunc(0.0f);

		//�C��PlayerPara[CntPlayer].
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(VEC3_ALL0);

		//�C�g								 
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(VEC3_ALL0);

		//���f���̃��Z�b�g
		ResetModel(
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(),
			this->modelOri.Vtx[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN].VtxBuff(),
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff(),
			&this->modelOri.Attribute[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN]);
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
		D3DXVECTOR3 POSTUREVec = VEC3_ALL0;

		//���C�L���X�g�֐�
		pfield->FieldHitGetSphereVec(RayStart, RayEnd, &POSTUREVec, &ReturnPosY);

		//���C�L���X�g���ʂ𔽉f
		D3DXVECTOR3 Pos = RayStart;
		Pos.y = ReturnPosY;
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(Pos);
		this->PlayerPara[CntPlayer].PostureVec.FNVecFunc(POSTUREVec);
		this->PlayerPara[CntPlayer].PostureVec.Quaternion();
		CameraRevers(this, CntPlayer, false);

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
void PLAYER::Update(void)
{
	//-----------------���l���񂾂��v�Z�B�O�l���񂾂�Q�[���I���B���̃V�[����
	int deadcnt = 0;
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (this->iUseType[CntPlayer].Use() == NoUse) deadcnt++;
		if (deadcnt >= 3)
		{
			//���[�J���ΐ펞��3�l���ʂƂ����V�[���J��
			if (pmysocket->GetNetGameStartFlag() ==false)
			{
				pscene->NextScene(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
			}
			//�l�b�g�ΐ펞��3�l����ł��瑼�̃v���C���[�Ɠ���������ɃV�[���J��
			else
			{
				if (pmysocket->MultThreadFlagFunc() == false)
				{
					pscene->NextScene(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
				}
			}
		}
	}

	//-----------------���[�J���ΐ퐧��
	if (pmysocket->GetNetGameStartFlag() == false)
	{
#ifdef _DEBUG
		this->PlayerPara[0].MovePara.MoveKeybord(this, 0);//�f�o�b�O�p
#endif
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			bool use = this->iUseType[CntPlayer].Use();
			//�����Ă���ΐ���\
			if (use)
			{
				//---------------����֐�
				this->PlayerPara[CntPlayer].MovePara.MoveL(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				CameraRotControl(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				this->PlayerPara[CntPlayer].PostureVec.Quaternion();
				CameraRevers(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				this->PlayerPara[CntPlayer].BulletPara.BulletALL(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				this->ItemTimeSpeed(CntPlayer, this->peffect);
				this->ItemTimeKiri(CntPlayer);
				this->ItemTimeMorphing(CntPlayer);
			}

			//����ȊO�̓J������������
			else
			{
				this->PlayerPara[CntPlayer].PostureVec.Quaternion();
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

	//-----------------�l�b�g�ΐ퐧��
	else
	{
		bool use = this->iUseType[pmysocket->GetNetMyNumber()].Use();

		//�����Ă���Α���\
		if (use)
		{
			//�l�b�g�p�t���O�������@1�t���[�����̕ύX��ۑ����ĕύX������΃T�[�o�[�Ƀf�[�^�𑗂�
			//---------------������
			this->PlayerPara[pmysocket->GetNetMyNumber()].MorphingPara.NetGetMorphingOneFrame = false;
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this->PlayerPara[CntPlayer].StandardPara.OldVital = this->PlayerPara[CntPlayer].StandardPara.Vital;
			}

			//---------------����֐�
			//�ړ�
			this->PlayerPara[pmysocket->GetNetMyNumber()].MovePara.MoveKeybord(this, pmysocket->GetNetMyNumber());//�f�o�b�O�p
			this->PlayerPara[pmysocket->GetNetMyNumber()].MovePara.MoveL(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			//��]
			CameraRotControl(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			CameraRevers(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			//�p��
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->PlayerPara[CntPlayer].PostureVec.Quaternion();
			//�o���b�g
			this->PlayerPara[pmysocket->GetNetMyNumber()].BulletPara.BulletALL(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			//�A�C�e������
			this->ItemTimeSpeed(pmysocket->GetNetMyNumber(),this->peffect);
			this->ItemTimeKiri(pmysocket->GetNetMyNumber());
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->ItemTimeMorphing(CntPlayer);
		}

		//����ȊO�̓J������������
		else
		{
			this->PlayerPara[pmysocket->GetNetMyNumber()].PostureVec.Quaternion();
			D3DXVECTOR3 Pos = this->modelDraw[pmysocket->GetNetMyNumber()].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
			D3DXVECTOR3 HoudaiRot = this->modelDraw[pmysocket->GetNetMyNumber()].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
			D3DXVECTOR3 HoutouRot = this->modelDraw[pmysocket->GetNetMyNumber()].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
			D3DXVECTOR3 HousinRot = this->modelDraw[pmysocket->GetNetMyNumber()].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();
			CAMERA *cam = GetCamera();
			cam[pmysocket->GetNetMyNumber()].at.x = Pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
			cam[pmysocket->GetNetMyNumber()].at.y = Pos.y + (HousinRot.x*100.0f);
			cam[pmysocket->GetNetMyNumber()].at.z = Pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));
			cam[pmysocket->GetNetMyNumber()].pos.x = Pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[pmysocket->GetNetMyNumber()].len;
			cam[pmysocket->GetNetMyNumber()].pos.y = Pos.y + POS_H_CAM;
			cam[pmysocket->GetNetMyNumber()].pos.z = Pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[pmysocket->GetNetMyNumber()].len;
		}

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void PLAYER::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�ꕪ����4�v���C���[���[�v�`�悳����@DRAW����4��ʕ����[�v�����Ă邩��A����16�̕����[�v�ŕ`�悵�Ă���
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (this->iUseType[CntPlayer].Use() == YesUseType1)
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
				D3DXVECTOR3 UpNorPOSTUREVec = this->PlayerPara[CntPlayer].PostureVec.FNUNCrossFunc();
				float Qrot = this->PlayerPara[CntPlayer].PostureVec.QrotFunc();
				D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();

				//q=(rotVecAxis�@��)*(g_Player.rot��])
				D3DXQuaternionRotationAxis(&q, &UpNorPOSTUREVec, -Qrot);
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
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				// �`��
				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].NumMat(); nCntMat++)
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
						this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].NumVertex(), 0,
						this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].NumPolygon());
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
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].NumMat(); nCntMat++)
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
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].NumVertex(), 0,
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].NumPolygon());
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
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].NumMat(); nCntMat++)
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
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].NumVertex(), 0,
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].NumPolygon());
					}
				}
				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//=============================================================================
// �X�s�[�h�A�b�v����
//=============================================================================
void PLAYER::ItemTimeSpeed(int CntPlayer, EFFECT *effect)
{
	//�X�s�[�h�o�t���Ԍ���
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		
		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
		effect->SetInstance(EffctSpeedupPos, VEC3_ALL0, PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
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
		this->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = true;
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
				&this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN],
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
			&this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN],
			0.01f, &this->PlayerPara[CntPlayer].MorphingPara.MorphingDTtime, &this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal);
		if (this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal == EndMorphing)
		{
			this->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
		}
	}
}

