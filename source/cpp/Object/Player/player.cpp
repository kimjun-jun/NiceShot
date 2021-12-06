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
constexpr float BULLET_BORN_TIME_ADD{ 1.0f };			//!< �e�򕜊������邽�߂̉��Z�^�C��
constexpr float BULLET_BORN_TIME_MAX{ 120.0f };			//!< 1�e�򕜊�����̂ɕK�v�ȃ^�C��
constexpr float	EFFECT_SPEEDUP_SIZE_X{ 25.0f };			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̕�
constexpr float	EFFECT_SPEEDUP_SIZE_Y{ 10.0f };			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̍���
constexpr int	EFFECT_SPEEDUP_TIME{ 10 };				//!< �G�t�F�N�g�X�s�[�h�A�b�v�̐�������
constexpr float	PLAYER_VALUE_DASHRATE{ 2.0f };			//!< �����ɔ�ׂẴ_�b�V���̑��x�{��
constexpr float	PLAYER_MOVE_RATE_X{ 1.0f / 40000.0f };	//!< ���E�����̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	PLAYER_MOVE_RATE_Y{ 1.0f / 200.0f };	//!< �O������̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float	PLAYER_MOVE_RATE_LR2{ 1.0f / 20000.0f };//!< �L���^�s���̈ړ��ʔ{��(0�`65000�����͔͈͂Ȃ̂ł���ɏ�Z����)
constexpr float VALUE_LEN_BULLET{ 10.0f };				//!< �v���C���[���S�Ƃ����o���b�g�̔��ˈʒu�܂ł̋���
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU{ 0.03f };	//!< �v���C���[�C����]��
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU_MAX{ 1.57f };//!< �v���C���[�C����]�ʍő�l
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN{ 0.01f };	//!< �v���C���[�C�g��]��
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN_MAX{ 0.2f };	//!< �v���C���[�C����]�ʍő�l
constexpr float	MOVE_INERTIA_MOMENT{ 0.8f };			//!< �ړ��ʂ̊������[�����g�A�����W���Ƃ��Ďg�p
constexpr float	VALUE_SPEEDBUFF_SUB{ 1.0f };			//!< �X�s�[�g�A�b�v���Ԃ̃f�N�������g����l
constexpr float BACKCAMERA_TIME{ 150.0f };				//!< �o�b�N�J�����A�C�e���L������
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
			this->PlayerMeshColor(this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pVtxBuff(), this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pIdxBuff(), np, CntPlayer);

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
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[1] = VEC3_ALL0;
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[2] = VEC3_ALL0;
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletStartPos = VEC3_ALL0;
		this->PlayerPara[CntPlayer].BulletPara.BulletStock = BULLET_MAX_STOCK;
		this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
		for (int i = 0; i < 3; i++) this->PlayerPara[CntPlayer].BulletPara.BulletMove[i] = VEC3_ALL0;

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
		this->PostureVec[CntPlayer].FNVecFunc(VEC3_ALL0);
		this->PostureVec[CntPlayer].FNUNCrossFunc(VEC3_ALL0);
		this->PostureVec[CntPlayer].QrotFunc(0.0f);

		//�C��
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
		D3DXVECTOR3 FIELDNORMALVec = VEC3_ALL0;

		//���C�L���X�g�֐�
		pfield->FieldHitGetSphereVec(RayStart, RayEnd, &FIELDNORMALVec, &ReturnPosY);

		//���C�L���X�g���ʂ𔽉f
		D3DXVECTOR3 Pos = RayStart;
		Pos.y = ReturnPosY;
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(Pos);
		this->PostureVec[CntPlayer].FNVecFunc(FIELDNORMALVec);
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
		this->Move->MoveKeybord(0, this);//�f�o�b�O�p
#endif
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			bool use = this->iUseType[CntPlayer].Use();
			//�����Ă���ΐ���\
			if (use)
			{
				//---------------����֐�
				this->Move->MoveL(CntPlayer, this, pmysocket->GetNetGameStartFlag());
				this->CameraRotControl(CntPlayer, pmysocket->GetNetGameStartFlag());
				this->Quaternion(CntPlayer);
				this->CameraRevers(CntPlayer, pmysocket->GetNetGameStartFlag());
				this->BulletALL(CntPlayer, pmysocket->GetNetGameStartFlag());
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
			this->PlayerPara[pmysocket->GetNetMyNumber()].BulletPara.BulletStartPos = VEC3_ALL0;
			for (int i = 0; i < 3; i++) this->PlayerPara[pmysocket->GetNetMyNumber()].BulletPara.BulletMove[i] = VEC3_ALL0;
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
				this->PlayerPara[CntPlayer].StandardPara.OldVital = this->PlayerPara[CntPlayer].StandardPara.Vital;
			}

			//---------------����֐�
			this->Move->MoveKeybord(pmysocket->GetNetMyNumber(), this);//�f�o�b�O�p
			this->Move->MoveL(pmysocket->GetNetMyNumber(), this, pmysocket->GetNetGameStartFlag());
			this->CameraRotControl(pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->Quaternion(CntPlayer);
			this->CameraRevers(pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			this->BulletALL(pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			this->ItemTimeKiri(pmysocket->GetNetMyNumber());
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->ItemTimeMorphing(CntPlayer);
		}

		//����ȊO�̓J������������
		else
		{
			this[pmysocket->GetNetMyNumber()].Quaternion(pmysocket->GetNetMyNumber());
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
	//		PositionShadow(g_EnemyHoudai[CntEnemy].parameter.obj->GetShadow()Idx,
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
				D3DXVECTOR3 UpNorFIELDNORMALVec = this->PostureVec[CntPlayer].FNUNCrossFunc();
				float Qrot = this->PostureVec[CntPlayer].QrotFunc();
				D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();

				//q=(rotVecAxis�@��)*(g_Player.rot��])
				D3DXQuaternionRotationAxis(&q, &UpNorFIELDNORMALVec, -Qrot);
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
	D3DXVECTOR3 moverot = VEC3_ALL0;
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
}

//=============================================================================
// �N�H�[�^�j�I������
//=============================================================================
void PLAYER::Quaternion(int CntPlayer)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 FIELDNORMALVec = this->PostureVec[CntPlayer].FNVecFunc();
	D3DXVECTOR3 FIELDNORMALUpNorCross = this->PostureVec[CntPlayer].FNUNCrossFunc();
	float Qrot = this->PostureVec[CntPlayer].QrotFunc();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
	D3DXVec3Cross(&FIELDNORMALUpNorCross, &FIELDNORMALVec, &UpVec);

	//�n�`�@����Up�x�N�g���œ��ς��Ƃ�(���ɑ΂����]�p�x�����߂�)
	float Upkakezan = D3DXVec3Dot(&FIELDNORMALVec, &UpVec);

	//������]�p�x��0�ȊO(���s�łȂ�)�Ȃ�Ή�]�p�x�Ƃ����߂�
	//�Ƃ̓N�H�[�^�j�I���̔C�ӎ�����]�����鎞�̉�]�p�x�ɂȂ�
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(FIELDNORMALVec.x*FIELDNORMALVec.x +
				FIELDNORMALVec.y *FIELDNORMALVec.y +
				FIELDNORMALVec.z * FIELDNORMALVec.z);
		Qrot = acosf(cossita);
	}
	else Qrot = 0.0f;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->PostureVec[CntPlayer].QrotFunc(Qrot);
	this->PostureVec[CntPlayer].FNUNCrossFunc(FIELDNORMALUpNorCross);

}

//=============================================================================
// �o���b�g�֘A����
//=============================================================================
void PLAYER::BulletALL(int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FIELDNORMALVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
	//D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FIELDNORMALVec, &Frontvec);
	float Bkakezan = D3DXVec3Dot(&FIELDNORMALVec, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(FIELDNORMALVec.x*FIELDNORMALVec.x +
				FIELDNORMALVec.y *FIELDNORMALVec.y +
				FIELDNORMALVec.z * FIELDNORMALVec.z)
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
	this->PlayerPara[CntPlayer].BulletPara.BulletStartPos.x = pos.x - (sinf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);
	this->PlayerPara[CntPlayer].BulletPara.BulletStartPos.y = pos.y + (sinf(this->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x) * VALUE_LEN_BULLET) + 20.0f;
	this->PlayerPara[CntPlayer].BulletPara.BulletStartPos.z = pos.z - (cosf(HoutouRot.y + HoudaiRot.y) * VALUE_LEN_BULLET);

	D3DXVECTOR3 BmoveRot;
	BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
	BmoveRot.y = sinf(this->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x);
	BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);

	this->PlayerPara[CntPlayer].BulletPara.BulletMove[0].x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	this->PlayerPara[CntPlayer].BulletPara.BulletMove[0].y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	this->PlayerPara[CntPlayer].BulletPara.BulletMove[0].z = (BmoveRot.z) *VALUE_MOVE_BULLET;

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
			this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 1;
			pbullet->SetInstance(this->PlayerPara[CntPlayer].BulletPara.BulletStartPos, this->PlayerPara[CntPlayer].BulletPara.BulletMove[0], BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
			//�g�U�e����
			if (this->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
			{
				this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 3;
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					this->PlayerPara[CntPlayer].BulletPara.BulletMove[0].y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					this->PlayerPara[CntPlayer].BulletPara.BulletMove[0].y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				pbullet->SetInstance(this->PlayerPara[CntPlayer].BulletPara.BulletStartPos, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
				pbullet->SetInstance(this->PlayerPara[CntPlayer].BulletPara.BulletStartPos, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));

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
void PLAYER::BulletALLMoveL2R2Ver(int CntPlayer)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FIELDNORMALVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
	//D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FIELDNORMALVec, &Frontvec);
	float Bkakezan = D3DXVec3Dot(&FIELDNORMALVec, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(FIELDNORMALVec.x*FIELDNORMALVec.x +
				FIELDNORMALVec.y *FIELDNORMALVec.y +
				FIELDNORMALVec.z * FIELDNORMALVec.z)
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
			pbullet->SetInstance(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
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
				pbullet->SetInstance(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
				pbullet->SetInstance(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));

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
// �X�s�[�h�A�b�v����@���g�p
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

