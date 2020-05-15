//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "load.h"
#include "fade.h"
#include "camera.h"
#include "shadow.h"
#include "bullettex.h"
#include "morphing.h"
#include "field.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_RADIUS		(10.0f)						// ���a
#define	RATE_MOVE_PLAYER	(0.025f)					// �ړ������W��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER_HONTAI		g_PlayerHoudai[PLAYER_MAX];
static PLAYER_PRATS			g_PlayerHoutou[PLAYER_MAX];
static PLAYER_PRATS			g_PlayerHousin[PLAYER_MAX];
static GPUMODEL				g_PlayerHousinOriginal[PLAYER_MAX];
static GPUMODEL				g_PlayerHousinAtack[PLAYER_MAX];

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
};

D3DXMATRIX mtxQmove;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//PLAYER ������
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHoudai[CntPlayer].AllParameter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									  
		g_PlayerHoudai[CntPlayer].OneParameter.q = D3DXQUATERNION(0,0,0,1);
		g_PlayerHoudai[CntPlayer].OneParameter.BrotQ = D3DXQUATERNION(0, 0, 0, 1);
		g_PlayerHoudai[CntPlayer].OneParameter.kyouyakuQ = D3DXQUATERNION(0, 0, 0, 1);
		g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].OneParameter.Upvec = D3DXVECTOR3(0, 1, 0);
		g_PlayerHoudai[CntPlayer].OneParameter.Frontvec = D3DXVECTOR3(0, 0, 1);
		g_PlayerHoudai[CntPlayer].OneParameter.UpRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].OneParameter.FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].OneParameter.speed = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.speedbuff = 1.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.speedbufftime = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.Qrot = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.Brot = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.dir = 1;
		g_PlayerHoudai[CntPlayer].OneParameter.use = true;
		g_PlayerHoudai[CntPlayer].OneParameter.Morphing = false;
		g_PlayerHoudai[CntPlayer].OneParameter.MorphingTime = MORPHING_TIME;
		g_PlayerHoudai[CntPlayer].OneParameter.MorphingEnd = true;
		g_PlayerHoudai[CntPlayer].OneParameter.speedbuffsignal = false;

		g_PlayerHoudai[CntPlayer].Model.pD3DTexture = NULL;
		g_PlayerHoudai[CntPlayer].Model.pD3DXMesh = NULL;
		g_PlayerHoudai[CntPlayer].Model.pD3DXBuffMat = NULL;
		g_PlayerHoudai[CntPlayer].Model.nNumMat = 0;
		g_PlayerHoudai[CntPlayer].Model.pD3DVtxBuff = NULL;
		g_PlayerHoudai[CntPlayer].Model.pD3DIdxBuff = NULL;
		g_PlayerHoudai[CntPlayer].Model.nNumVertex = 0;
		g_PlayerHoudai[CntPlayer].Model.nNumPolygon = 0;
		g_PlayerHoudai[CntPlayer].Model.nNumVertexIndex = 0;

		g_PlayerHoudai[CntPlayer].Parent = NULL;
		g_PlayerHoudai[CntPlayer].KiriSignal = false;
		g_PlayerHoudai[CntPlayer].KiriItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].BackCameraItemSignal = false;
		g_PlayerHoudai[CntPlayer].BackCameraItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].AmmoNum = MAX_AMMO;
		g_PlayerHoudai[CntPlayer].AmmoBornCnt = 0.0f;
		g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;

		// X�t�@�C���̓ǂݍ���
		if (LoadMesh(MODEL_HOUDAI, &g_PlayerHoudai[CntPlayer].Model.pD3DXBuffMat,
			&g_PlayerHoudai[CntPlayer].Model.nNumMat, &g_PlayerHoudai[CntPlayer].Model.pD3DXMesh,
			&g_PlayerHoudai[CntPlayer].Model.pD3DVtxBuff, &g_PlayerHoudai[CntPlayer].Model.pD3DIdxBuff,
			&g_PlayerHoudai[CntPlayer].Model.nNumVertex, &g_PlayerHoudai[CntPlayer].Model.nNumPolygon,
			&g_PlayerHoudai[CntPlayer].Model.nNumVertexIndex, &g_PlayerHoudai[CntPlayer].Model.pD3DTexture))
		{
			return E_FAIL;
		}
		
		//���_�J���[���v���C���[�F(��)�ɕύX
		SetPlayerMeshColor(&g_PlayerHoudai[CntPlayer].Model, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHoudai[CntPlayer].Model.pD3DTexture);	// �ǂݍ��ރ������[


		//�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHoutou[CntPlayer].AllParameter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].AllParameter.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].AllParameter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoutou[CntPlayer].AllParameter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].Model.pD3DTexture = NULL;
		g_PlayerHoutou[CntPlayer].Model.pD3DXMesh = NULL;
		g_PlayerHoutou[CntPlayer].Model.pD3DXBuffMat = NULL;
		g_PlayerHoutou[CntPlayer].Model.nNumMat = 0;
		g_PlayerHoutou[CntPlayer].Model.pD3DVtxBuff = NULL;
		g_PlayerHoutou[CntPlayer].Model.pD3DIdxBuff = NULL;
		g_PlayerHoutou[CntPlayer].Model.nNumVertex = 0;
		g_PlayerHoutou[CntPlayer].Model.nNumPolygon = 0;
		g_PlayerHoutou[CntPlayer].Model.nNumVertexIndex = 0;
		g_PlayerHoutou[CntPlayer].ParentHontai = &g_PlayerHoudai[CntPlayer];

		// X�t�@�C���̓ǂݍ���
		if(LoadMesh(MODEL_HOUTOU, &g_PlayerHoutou[CntPlayer].Model.pD3DXBuffMat,
			&g_PlayerHoutou[CntPlayer].Model.nNumMat, &g_PlayerHoutou[CntPlayer].Model.pD3DXMesh,
			&g_PlayerHoutou[CntPlayer].Model.pD3DVtxBuff, &g_PlayerHoutou[CntPlayer].Model.pD3DIdxBuff,
			&g_PlayerHoutou[CntPlayer].Model.nNumVertex, &g_PlayerHoutou[CntPlayer].Model.nNumPolygon,
			&g_PlayerHoutou[CntPlayer].Model.nNumVertexIndex, &g_PlayerHoutou[CntPlayer].Model.pD3DTexture))
		{
			return E_FAIL;
		}

		//���_�J���[���v���C���[�F(��)�ɕύX
		SetPlayerMeshColor(&g_PlayerHoutou[CntPlayer].Model, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHoutou[CntPlayer].Model.pD3DTexture);	// �ǂݍ��ރ������[

		//�C�g
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHousin[CntPlayer].AllParameter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].AllParameter.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].AllParameter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHousin[CntPlayer].AllParameter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].Model.pD3DTexture = NULL;
		g_PlayerHousin[CntPlayer].Model.pD3DXMesh = NULL;
		g_PlayerHousin[CntPlayer].Model.pD3DXBuffMat = NULL;
		g_PlayerHousin[CntPlayer].Model.nNumMat = 0;
		g_PlayerHousin[CntPlayer].Model.pD3DVtxBuff = NULL;
		g_PlayerHousin[CntPlayer].Model.pD3DIdxBuff = NULL;
		g_PlayerHousin[CntPlayer].Model.nNumVertex = 0;
		g_PlayerHousin[CntPlayer].Model.nNumPolygon = 0;
		g_PlayerHousin[CntPlayer].Model.nNumVertexIndex = 0;
		g_PlayerHousin[CntPlayer].Model.MorphingSignal = NoMorphing;
		g_PlayerHousin[CntPlayer].ParentParts = &g_PlayerHoutou[CntPlayer];

		// X�t�@�C���̓ǂݍ���
		if(LoadMesh(MODEL_HOUSIN, &g_PlayerHousin[CntPlayer].Model.pD3DXBuffMat,
			&g_PlayerHousin[CntPlayer].Model.nNumMat, &g_PlayerHousin[CntPlayer].Model.pD3DXMesh,
			&g_PlayerHousin[CntPlayer].Model.pD3DVtxBuff, &g_PlayerHousin[CntPlayer].Model.pD3DIdxBuff,
			&g_PlayerHousin[CntPlayer].Model.nNumVertex, &g_PlayerHousin[CntPlayer].Model.nNumPolygon, 
			&g_PlayerHousin[CntPlayer].Model.nNumVertexIndex,&g_PlayerHousin[CntPlayer].Model.pD3DTexture))
		{
			return E_FAIL;
		}

		//���_�J���[���v���C���[�F(��)�ɕύX
		SetPlayerMeshColor(&g_PlayerHousin[CntPlayer].Model, CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHousin[CntPlayer].Model.pD3DTexture);	// �ǂݍ��ރ������[

		//�C�g�U���f�[�^
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHousinAtack[CntPlayer].pD3DTexture = NULL;
		g_PlayerHousinAtack[CntPlayer].pD3DXMesh = NULL;
		g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerHousinAtack[CntPlayer].nNumMat = 0;
		g_PlayerHousinAtack[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerHousinAtack[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerHousinAtack[CntPlayer].nNumVertex = 0;
		g_PlayerHousinAtack[CntPlayer].nNumPolygon = 0;
		g_PlayerHousinAtack[CntPlayer].nNumVertexIndex = 0;

		// X�t�@�C���̓ǂݍ���
		if(	LoadMesh(MODEL_HOUSINMO, &g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat,
			&g_PlayerHousinAtack[CntPlayer].nNumMat, &g_PlayerHousinAtack[CntPlayer].pD3DXMesh,
			&g_PlayerHousinAtack[CntPlayer].pD3DVtxBuff, &g_PlayerHousinAtack[CntPlayer].pD3DIdxBuff,
			&g_PlayerHousinAtack[CntPlayer].nNumVertex, &g_PlayerHousinAtack[CntPlayer].nNumPolygon,
			&g_PlayerHousinAtack[CntPlayer].nNumVertexIndex, &g_PlayerHousinAtack[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//���_�J���[���v���C���[�F�ɕύX
		SetPlayerMeshColor(&g_PlayerHousinAtack[CntPlayer], CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHousinAtack[CntPlayer].pD3DTexture);	// �ǂݍ��ރ������[


		//�C�g�ʏ�f�[�^
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHousinOriginal[CntPlayer].pD3DTexture = NULL;
		g_PlayerHousinOriginal[CntPlayer].pD3DXMesh = NULL;
		g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerHousinOriginal[CntPlayer].nNumMat = 0;
		g_PlayerHousinOriginal[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerHousinOriginal[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerHousinOriginal[CntPlayer].nNumVertex = 0;
		g_PlayerHousinOriginal[CntPlayer].nNumPolygon = 0;
		g_PlayerHousinOriginal[CntPlayer].nNumVertexIndex = 0;

		// X�t�@�C���̓ǂݍ���
		if(LoadMesh(MODEL_HOUSIN, &g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat,
			&g_PlayerHousinOriginal[CntPlayer].nNumMat, &g_PlayerHousinOriginal[CntPlayer].pD3DXMesh,
			&g_PlayerHousinOriginal[CntPlayer].pD3DVtxBuff, &g_PlayerHousinOriginal[CntPlayer].pD3DIdxBuff,
			&g_PlayerHousinOriginal[CntPlayer].nNumVertex, &g_PlayerHousinOriginal[CntPlayer].nNumPolygon,
			&g_PlayerHousinOriginal[CntPlayer].nNumVertexIndex, &g_PlayerHousinOriginal[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//���_�J���[���v���C���[�F�ɕύX
		SetPlayerMeshColor(&g_PlayerHousinOriginal[CntPlayer], CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHousinOriginal[CntPlayer].pD3DTexture);	// �ǂݍ��ރ������[

	}


	g_PlayerHoudai[0].AllParameter.pos = D3DXVECTOR3(699.0f + rand() % 10 , 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[1].AllParameter.pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[2].AllParameter.pos = D3DXVECTOR3(699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);
	g_PlayerHoudai[3].AllParameter.pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);

	g_PlayerHoudai[0].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[1].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[2].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[3].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHit(D3DXVECTOR3(g_PlayerHoudai[CntPlayer].AllParameter.pos.x, g_PlayerHoudai[CntPlayer].AllParameter.pos.y + 10.0f, g_PlayerHoudai[CntPlayer].AllParameter.pos.z),
			D3DXVECTOR3(g_PlayerHoudai[CntPlayer].AllParameter.pos.x, g_PlayerHoudai[CntPlayer].AllParameter.pos.y - 1000.0f, g_PlayerHoudai[CntPlayer].AllParameter.pos.z),
			&g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis, &g_PlayerHoudai[CntPlayer].AllParameter.pos.y);

	}

	return S_OK;
}

//=============================================================================
// �ď���������
//=============================================================================
HRESULT ReInitPlayer(void)
{
	//PLAYER ������
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHoudai[CntPlayer].AllParameter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoudai[CntPlayer].AllParameter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_PlayerHoudai[CntPlayer].OneParameter.q = D3DXQUATERNION(0, 0, 0, 1);
		g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].OneParameter.Upvec = D3DXVECTOR3(0, 1, 0);
		g_PlayerHoudai[CntPlayer].OneParameter.Frontvec = D3DXVECTOR3(0, 0, 1);
		g_PlayerHoudai[CntPlayer].OneParameter.UpRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].OneParameter.FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].OneParameter.speed = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.speedbuff = 1.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.speedbufftime = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.Qrot = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.Brot = 0.0f;
		g_PlayerHoudai[CntPlayer].OneParameter.dir = 1;
		g_PlayerHoudai[CntPlayer].OneParameter.use = true;
		g_PlayerHoudai[CntPlayer].OneParameter.Morphing = false;
		g_PlayerHoudai[CntPlayer].OneParameter.MorphingTime = MORPHING_TIME;
		g_PlayerHoudai[CntPlayer].OneParameter.MorphingEnd = true;
		g_PlayerHoudai[CntPlayer].OneParameter.speedbuffsignal = false;

		g_PlayerHoudai[CntPlayer].KiriSignal = false;
		g_PlayerHoudai[CntPlayer].KiriItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].BackCameraItemSignal = false;
		g_PlayerHoudai[CntPlayer].BackCameraItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].AmmoNum = MAX_AMMO;
		g_PlayerHoudai[CntPlayer].AmmoBornCnt = 0.0f;
		g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;

		//�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHoutou[CntPlayer].AllParameter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].AllParameter.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].AllParameter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoutou[CntPlayer].AllParameter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//�C�g
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHousin[CntPlayer].AllParameter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].AllParameter.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].AllParameter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHousin[CntPlayer].AllParameter.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].Model.MorphingSignal = NoMorphing;

		ResetMorphing(&g_PlayerHousin[CntPlayer].Model, &g_PlayerHousinOriginal[CntPlayer]);
	}

	// �e�̏�����
	//D3DXVECTOR3 pos = g_PlayerHoudai[0].parameter.pos;
	//pos.y = 0.0f;
	//g_PlayerHoudai[0].parameter.shadowIdx = CreateShadow(pos, g_PlayerHoudai[0].parameter.scl);

	g_PlayerHoudai[0].AllParameter.pos = D3DXVECTOR3(699.0f + rand() % 10, 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[1].AllParameter.pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[2].AllParameter.pos = D3DXVECTOR3(699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);
	g_PlayerHoudai[3].AllParameter.pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);

	g_PlayerHoudai[0].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[1].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[2].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[3].AllParameter.rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHit(D3DXVECTOR3(g_PlayerHoudai[CntPlayer].AllParameter.pos.x, g_PlayerHoudai[CntPlayer].AllParameter.pos.y + 10.0f, g_PlayerHoudai[CntPlayer].AllParameter.pos.z),
			D3DXVECTOR3(g_PlayerHoudai[CntPlayer].AllParameter.pos.x, g_PlayerHoudai[CntPlayer].AllParameter.pos.y - 1000.0f, g_PlayerHoudai[CntPlayer].AllParameter.pos.z),
			&g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis, &g_PlayerHoudai[CntPlayer].AllParameter.pos.y);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHoudai[CntPlayer].Model.pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHoudai[CntPlayer].Model.pD3DTexture->Release();
			g_PlayerHoudai[CntPlayer].Model.pD3DTexture = NULL;
		}
		if (g_PlayerHoudai[CntPlayer].Model.pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHoudai[CntPlayer].Model.pD3DXBuffMat->Release();
			g_PlayerHoudai[CntPlayer].Model.pD3DXBuffMat = NULL;
		}
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHoutou[CntPlayer].Model.pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHoutou[CntPlayer].Model.pD3DTexture->Release();
			g_PlayerHoutou[CntPlayer].Model.pD3DTexture = NULL;
		}
		if (g_PlayerHoutou[CntPlayer].Model.pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHoutou[CntPlayer].Model.pD3DXBuffMat->Release();
			g_PlayerHoutou[CntPlayer].Model.pD3DXBuffMat = NULL;
		}
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHousin[CntPlayer].Model.pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHousin[CntPlayer].Model.pD3DTexture->Release();
			g_PlayerHousin[CntPlayer].Model.pD3DTexture = NULL;
		}
		if (g_PlayerHousin[CntPlayer].Model.pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHousin[CntPlayer].Model.pD3DXBuffMat->Release();
			g_PlayerHousin[CntPlayer].Model.pD3DXBuffMat = NULL;
		}
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHousinOriginal[CntPlayer].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHousinOriginal[CntPlayer].pD3DTexture->Release();
			g_PlayerHousinOriginal[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat = NULL;
		}
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHousinAtack[CntPlayer].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHousinAtack[CntPlayer].pD3DTexture->Release();
			g_PlayerHousinAtack[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat = NULL;
		}
	}
}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	int deadcnt = 0;
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHoudai[CntPlayer].OneParameter.use == false) deadcnt++;
		if (deadcnt >= 3) SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
	}
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHoudai[CntPlayer].OneParameter.use)
		{
			g_PlayerHoudai[CntPlayer].AllParameter.oldpos = g_PlayerHoudai[CntPlayer].AllParameter.pos;

			//�ړ�����
			if (GetKeyboardPress(DIK_UP) || IsButtonPressed(CntPlayer, BUTTON_A))
			{	
				g_PlayerHoudai[CntPlayer].OneParameter.dir = 1;
				g_PlayerHoudai[CntPlayer].OneParameter.speed += VALUE_MOVE;
			}
			else if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(CntPlayer, BUTTON_B))
			{
				g_PlayerHoudai[CntPlayer].OneParameter.dir = -1;
				g_PlayerHoudai[CntPlayer].OneParameter.speed -= VALUE_MOVE;
			}
			// ���ړ����͈ړ��ʂɊ�����������
			else
			{
				g_PlayerHoudai[CntPlayer].OneParameter.speed *= 0.8f;
			}

			//���_�ω��̃A�i���O�l�����ɑ�����ăA�i���O����Ő���
			DIJOYSTATE2 *Button = GetIsButton();
			float LAnalogX = float(Button->lX/30000.0f);

			//����
			if (!IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT) && !IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
			{
				g_PlayerHoudai[CntPlayer].OneParameter.dir = 1;
			}
			else if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
			{
				g_PlayerHoudai[CntPlayer].AllParameter.rot.y += LAnalogX * g_PlayerHoudai[CntPlayer].OneParameter.dir;
			}
			else if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
			{
				g_PlayerHoudai[CntPlayer].AllParameter.rot.y += LAnalogX * g_PlayerHoudai[CntPlayer].OneParameter.dir;
			}

			//�p�x�̐����l
			if (g_PlayerHoudai[CntPlayer].AllParameter.rot.y >= 6.28f) g_PlayerHoudai[CntPlayer].AllParameter.rot.y = 0.0f;
			else if (g_PlayerHoudai[CntPlayer].AllParameter.rot.y <= -6.28f) g_PlayerHoudai[CntPlayer].AllParameter.rot.y = 0.0f;

			// �ړ����x�̐���
			if (g_PlayerHoudai[CntPlayer].OneParameter.speed >= VALUE_MOVE_MAX) g_PlayerHoudai[CntPlayer].OneParameter.speed = VALUE_MOVE_MAX;
			else if (g_PlayerHoudai[CntPlayer].OneParameter.speed <= -VALUE_MOVE_MAX) g_PlayerHoudai[CntPlayer].OneParameter.speed = -VALUE_MOVE_MAX;

			// �v���C���[�̍��W���X�V
			g_PlayerHoudai[CntPlayer].AllParameter.pos.x -= sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y) * (g_PlayerHoudai[CntPlayer].OneParameter.speed * g_PlayerHoudai[CntPlayer].OneParameter.speedbuff);
			g_PlayerHoudai[CntPlayer].AllParameter.pos.z -= cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y) * (g_PlayerHoudai[CntPlayer].OneParameter.speed * g_PlayerHoudai[CntPlayer].OneParameter.speedbuff);

			//�X�s�[�h�o�t���Ԍ���
			if (g_PlayerHoudai[CntPlayer].OneParameter.speedbuffsignal == true)
			{
				g_PlayerHoudai[CntPlayer].OneParameter.speedbufftime -= 1.0f;
				if (g_PlayerHoudai[CntPlayer].OneParameter.speedbufftime <= 0.0f)
				{
					g_PlayerHoudai[CntPlayer].OneParameter.speedbuffsignal = false;
					g_PlayerHoudai[CntPlayer].OneParameter.speedbuff = 1.0f;
				}
			}

			//�C������@�o���b�g���e�_(���E�G�C��)
			if (GetKeyboardPress(DIK_D) || IsButtonPressed(CntPlayer, BUTTON_R1))
			{
				g_PlayerHoutou[CntPlayer].AllParameter.rot.y += VALUE_ROTATE_PLAYER;
				if (g_PlayerHoutou[CntPlayer].AllParameter.rot.y >= 1.57f) g_PlayerHoutou[CntPlayer].AllParameter.rot.y = 1.57f;
			}
			else if (GetKeyboardPress(DIK_A) || IsButtonPressed(CntPlayer, BUTTON_L1))
			{
				g_PlayerHoutou[CntPlayer].AllParameter.rot.y -= VALUE_ROTATE_PLAYER;
				if (g_PlayerHoutou[CntPlayer].AllParameter.rot.y <= -1.57f) g_PlayerHoutou[CntPlayer].AllParameter.rot.y = -1.57f;
			}

			//�C�g����@�o���b�g���e�_(�O��G�C��)
			if (GetKeyboardPress(DIK_W) || IsButtonPressed(CntPlayer, BUTTON_R2))
			{
				g_PlayerHousin[CntPlayer].AllParameter.rot.x += 0.01f;
				if (g_PlayerHousin[CntPlayer].AllParameter.rot.x >= 0.2f) g_PlayerHousin[CntPlayer].AllParameter.rot.x = 0.2f;
			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(CntPlayer, BUTTON_L2))
			{
				g_PlayerHousin[CntPlayer].AllParameter.rot.x -= 0.01f;
				if (g_PlayerHousin[CntPlayer].AllParameter.rot.x <= -0.2) g_PlayerHousin[CntPlayer].AllParameter.rot.x = -0.2f;
			}

			//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
			D3DXVec3Cross(&g_PlayerHoudai[CntPlayer].OneParameter.UpRotTOaxis, &g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis, &g_PlayerHoudai[CntPlayer].OneParameter.Upvec);
			float Ukakezan = D3DXVec3Dot(&g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis, &g_PlayerHoudai[CntPlayer].OneParameter.Upvec);
			if (Ukakezan != 0)
			{
				float cossita = Ukakezan /
					sqrtf(g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.x*g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.x +
						g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.y *g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.y +
						g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.z * g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.z);
				g_PlayerHoudai[CntPlayer].OneParameter.Qrot = acosf(cossita);
			}
			else g_PlayerHoudai[CntPlayer].OneParameter.Qrot = 0.0f;

			g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.x = sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y);
			g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.y = 0.0f;
			g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.z = cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y);

			//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
			D3DXVec3Cross(&g_PlayerHoudai[CntPlayer].OneParameter.FrontRotTOaxis, &g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis, &g_PlayerHoudai[CntPlayer].OneParameter.Frontvec);
			float Bkakezan = D3DXVec3Dot(&g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis, &g_PlayerHoudai[CntPlayer].OneParameter.Frontvec);
			if (Bkakezan != 0)
			{
				float cossita = Bkakezan /
					sqrtf(g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.x*g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.x +
						g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.y *g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.y +
						g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.z * g_PlayerHoudai[CntPlayer].OneParameter.RotVecAxis.z)
					*
					sqrtf(g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.x*g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.x +
						g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.y *g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.y +
						g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.z * g_PlayerHoudai[CntPlayer].OneParameter.Frontvec.z);
				g_PlayerHoudai[CntPlayer].OneParameter.Brot = acosf(cossita);
			}
			else
			{
				g_PlayerHoudai[CntPlayer].OneParameter.Brot = 1.57f;		//�������x�N�g��rot=0.0f�A������x�N�g��rot=3.14�A�ɑ΂��Ă̑O�����x�N�g����rot=1.57f�B
			}
			g_PlayerHoudai[CntPlayer].OneParameter.Brot -= 1.57f;

			//�o�b�N�J��������
			//�o�b�N�J�����I���@�J�������_�A�����_
			if (GetKeyboardPress(DIK_B) || IsButtonPressed(CntPlayer, BUTTON_Y))
			{
				cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].AllParameter.pos.x + (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y));
				cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].AllParameter.pos.y + (g_PlayerHousin[CntPlayer].AllParameter.rot.x*100.0f);
				cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].AllParameter.pos.z + (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y));

				cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].AllParameter.pos.x - sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y) * cam[CntPlayer].len;
				cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].AllParameter.pos.y + POS_H_CAM;
				cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].AllParameter.pos.z - cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y) * cam[CntPlayer].len;
			}
			//�o�b�N�J�����I�t�@�J�������_�A�����_
			else
			{
				cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].AllParameter.pos.x - (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y));
				cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].AllParameter.pos.y + (g_PlayerHousin[CntPlayer].AllParameter.rot.x*100.0f);
				cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].AllParameter.pos.z - (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y));

				cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].AllParameter.pos.x + sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y) * cam[CntPlayer].len;
				cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].AllParameter.pos.y + POS_H_CAM;
				//cam[CntPlayer].pos.y = 100.0f;
				cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].AllParameter.pos.z + cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y) * cam[CntPlayer].len;
			}
			if (g_PlayerHoudai[CntPlayer].BackCameraItemSignal == true)
			{
				cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].AllParameter.pos.x + (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y));
				cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].AllParameter.pos.y + (g_PlayerHousin[CntPlayer].AllParameter.rot.x*100.0f);
				cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].AllParameter.pos.z + (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y));

				cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].AllParameter.pos.x - sinf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y) * cam[CntPlayer].len;
				cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].AllParameter.pos.y + POS_H_CAM;
				cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].AllParameter.pos.z - cosf(g_PlayerHoudai[CntPlayer].AllParameter.rot.y + g_PlayerHoutou[CntPlayer].AllParameter.rot.y) * cam[CntPlayer].len;
				g_PlayerHoudai[CntPlayer].BackCameraItemTime += 1.0f;
				if (g_PlayerHoudai[CntPlayer].BackCameraItemTime >= BACKCAMERA_TIME)
				{
					g_PlayerHoudai[CntPlayer].BackCameraItemTime = 0.0f;
					g_PlayerHoudai[CntPlayer].BackCameraItemSignal = false;
				}
			}

			if (g_PlayerHoudai[CntPlayer].KiriSignal == true)
			{
				g_PlayerHoudai[CntPlayer].KiriItemTime += 1.0f;
				if (g_PlayerHoudai[CntPlayer].KiriItemTime >= KIRI_TIME)
				{
					g_PlayerHoudai[CntPlayer].KiriItemTime = 0.0f;
					g_PlayerHoudai[CntPlayer].KiriSignal = false;
				}
			}

			// �e����
			if (g_PlayerHoudai[CntPlayer].AmmoNum > 0)
			{
				if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(CntPlayer, BUTTON_X))
				{
					D3DXVECTOR3 BposStart;
					D3DXVECTOR3 move;

					//�v���C���[pos���甭�˕����ɏ������炵���l
					//�n�ʂ̌X���ɉ����Ĕ��˂���Ƃ��͖��Ȃ��B���̌X�����獶�E�ɉ�]���Ă鎞����pos����������
					BposStart.x = g_PlayerHoudai[CntPlayer].AllParameter.pos.x - sinf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y) * VALUE_MOVE_BULLET;
					BposStart.y = g_PlayerHoudai[CntPlayer].AllParameter.pos.y + (-sinf(-g_PlayerHousin[CntPlayer].AllParameter.rot.x +
						g_PlayerHoudai[CntPlayer].OneParameter.Brot + g_PlayerHoudai[CntPlayer].OneParameter.Qrot) * VALUE_MOVE_BULLET) + 20;
					BposStart.z = g_PlayerHoudai[CntPlayer].AllParameter.pos.z - cosf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y) * VALUE_MOVE_BULLET;

					D3DXVECTOR3 BmoveRot;
					BmoveRot.x = -sinf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y);
					BmoveRot.y = sinf(g_PlayerHoudai[CntPlayer].OneParameter.Brot -g_PlayerHousin[CntPlayer].AllParameter.rot.x);
					BmoveRot.z = -cosf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y);

					move.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
					move.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
					move.z = (BmoveRot.z) *VALUE_MOVE_BULLET;
					//move.y *= -1;


					SetBullet(BposStart, move, 4.0f, 4.0f, 60 * 4, CntPlayer);

					//�g�U�e����
					if (g_PlayerHoudai[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
					{
						D3DXVECTOR3 leftB, rightB;
						leftB = D3DXVECTOR3(-sinf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y + 0.3f)*VALUE_MOVE_BULLET,
							move.y,
							-cosf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y + 0.3f) *VALUE_MOVE_BULLET);
						rightB = D3DXVECTOR3(-sinf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y - 0.3f)*VALUE_MOVE_BULLET,
							move.y,
							-cosf(g_PlayerHoutou[CntPlayer].AllParameter.rot.y + g_PlayerHoudai[CntPlayer].AllParameter.rot.y - 0.3f) *VALUE_MOVE_BULLET);
						SetBullet(BposStart, leftB, 4.0f, 4.0f, 60 * 4, CntPlayer);
						SetBullet(BposStart, rightB, 4.0f, 4.0f, 60 * 4, CntPlayer);
					}
					g_PlayerHoudai[CntPlayer].AmmoNum -= 1;
					ChangeBulletTex(-1, CntPlayer);
					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_attack03);
				}
			}

			// ���[�t�B���Otrue
			if (g_PlayerHoudai[CntPlayer].OneParameter.Morphing == true)
			{
				///////////////////////////////////////////////////////////////////////�o���b�g3���ԊJ�n
				// ���[�t�B���O���Ԍ��Z�J�n
				g_PlayerHoudai[CntPlayer].OneParameter.MorphingTime -= 1.0f;

				// ���[�t�B���O�U���^�C�v�ɕύX�J�n
				if (g_PlayerHousin[CntPlayer].Model.MorphingSignal == NowMorphing)
				{
					g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
					DoMorphing(&g_PlayerHousin[CntPlayer].Model, &g_PlayerHousinAtack[CntPlayer]);
				}
				///////////////////////////////////////////////////////////////////////�o���b�g3���ԏI��

				// ���Ԍo�߂Ń��f�������ɖ߂�
				else if (g_PlayerHoudai[CntPlayer].OneParameter.MorphingTime <= 0.0f)
				{
					g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
					g_PlayerHousin[CntPlayer].Model.MorphingSignal = NowMorphing;
					g_PlayerHoudai[CntPlayer].OneParameter.Morphing = false;
				}
			}

			// ���[�t�B���O�I���W�i���^�C�v�ɕύX�J�n
			if (g_PlayerHoudai[CntPlayer].OneParameter.Morphing == false && g_PlayerHoudai[CntPlayer].OneParameter.MorphingTime <= 0.0f)
			{
				DoMorphing(&g_PlayerHousin[CntPlayer].Model, &g_PlayerHousinOriginal[CntPlayer]);
				if (g_PlayerHousin[CntPlayer].Model.MorphingSignal == EndMorphing)
				{
					g_PlayerHoudai[CntPlayer].OneParameter.MorphingTime = MORPHING_TIME;
					g_PlayerHousin[CntPlayer].Model.time = 0.0f;
				}
			}

			//�c�e���� ��莞�Ԍo�߂�1�Â�����
			if (g_PlayerHoudai[CntPlayer].AmmoNum < MAX_AMMO) g_PlayerHoudai[CntPlayer].AmmoBornCnt += BORN_AMMO_ADDTIME;
			if (g_PlayerHoudai[CntPlayer].AmmoBornCnt >= BORN_AMMO_MAXTIME)
			{
				g_PlayerHoudai[CntPlayer].AmmoNum++;
				ChangeBulletTex(1, CntPlayer);
				g_PlayerHoudai[CntPlayer].AmmoBornCnt = 0.0f;
			}
		}
	}
}

//=============================================================================
// �G�l�~�[�X�V����
//=============================================================================
void UpdateEnemy(void)
{
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

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;
			g_PlayerHoudai[CntPlayer].OneParameter.BrotQ = D3DXQUATERNION(0, 0, 0, 1);
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);
			D3DXMatrixIdentity(&mtxQmove);
			
			//q=(rotVecAxis�@��)*(g_Player.rot��])
			D3DXQuaternionRotationAxis(&g_PlayerHoudai[CntPlayer].OneParameter.q, &g_PlayerHoudai[CntPlayer].OneParameter.UpRotTOaxis, -g_PlayerHoudai[CntPlayer].OneParameter.Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &g_PlayerHoudai[CntPlayer].OneParameter.q);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld);
			D3DXMatrixIdentity(&g_PlayerHoudai[CntPlayer].OneParameter.mtxWorldRotBuff);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_PlayerHoudai[CntPlayer].AllParameter.scl.x, g_PlayerHoudai[CntPlayer].AllParameter.scl.y, g_PlayerHoudai[CntPlayer].AllParameter.scl.z);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHoudai[CntPlayer].AllParameter.rot.y, g_PlayerHoudai[CntPlayer].AllParameter.rot.x, g_PlayerHoudai[CntPlayer].AllParameter.rot.z);

			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].OneParameter.mtxWorldRotBuff, &g_PlayerHoudai[CntPlayer].OneParameter.mtxWorldRotBuff, &mtxQ);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &mtxRot);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &mtxQ);


			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHoudai[CntPlayer].AllParameter.pos.x, g_PlayerHoudai[CntPlayer].AllParameter.pos.y, g_PlayerHoudai[CntPlayer].AllParameter.pos.z);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHoudai[CntPlayer].AllParameter.mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHoudai[CntPlayer].Model.pD3DXBuffMat->GetBufferPointer();

			// �`��
			for (int nCntMat = 0; nCntMat < (int)g_PlayerHoudai[CntPlayer].Model.nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_PlayerHoudai[CntPlayer].Model.pD3DTexture);
				}

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);
				// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetStreamSource(0, g_PlayerHoudai[CntPlayer].Model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetIndices(g_PlayerHoudai[CntPlayer].Model.pD3DIdxBuff);
				//�`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHoudai[CntPlayer].Model.nNumVertex, 0, g_PlayerHoudai[CntPlayer].Model.nNumPolygon);
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_PlayerHoutou[CntPlayer].AllParameter.scl.x, g_PlayerHoutou[CntPlayer].AllParameter.scl.y, g_PlayerHoutou[CntPlayer].AllParameter.scl.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHoutou[CntPlayer].AllParameter.rot.y, g_PlayerHoutou[CntPlayer].AllParameter.rot.x, g_PlayerHoutou[CntPlayer].AllParameter.rot.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHoutou[CntPlayer].AllParameter.pos.x, g_PlayerHoutou[CntPlayer].AllParameter.pos.y, g_PlayerHoutou[CntPlayer].AllParameter.pos.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &mtxTranslate);

			if (g_PlayerHoutou[CntPlayer].ParentHontai != NULL)
			{
				D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld, &g_PlayerHoutou[CntPlayer].ParentHontai->AllParameter.mtxWorld);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHoutou[CntPlayer].AllParameter.mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHoutou[CntPlayer].Model.pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PlayerHoutou[CntPlayer].Model.nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_PlayerHoutou[CntPlayer].Model.pD3DTexture);
				}
				// �`��
				{
					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);
					// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetStreamSource(0, g_PlayerHoutou[CntPlayer].Model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetIndices(g_PlayerHoutou[CntPlayer].Model.pD3DIdxBuff);
					//�`��
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHoutou[CntPlayer].Model.nNumVertex, 0, g_PlayerHoutou[CntPlayer].Model.nNumPolygon);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_PlayerHousin[CntPlayer].AllParameter.mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_PlayerHousin[CntPlayer].AllParameter.scl.x, g_PlayerHousin[CntPlayer].AllParameter.scl.y, g_PlayerHousin[CntPlayer].AllParameter.scl.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHousin[CntPlayer].AllParameter.rot.y, g_PlayerHousin[CntPlayer].AllParameter.rot.x, g_PlayerHousin[CntPlayer].AllParameter.rot.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHousin[CntPlayer].AllParameter.pos.x, g_PlayerHousin[CntPlayer].AllParameter.pos.y, g_PlayerHousin[CntPlayer].AllParameter.pos.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &mtxTranslate);

			if (g_PlayerHousin[CntPlayer].ParentParts != NULL)
			{
				D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &g_PlayerHousin[CntPlayer].AllParameter.mtxWorld, &g_PlayerHousin[CntPlayer].ParentParts->AllParameter.mtxWorld);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHousin[CntPlayer].AllParameter.mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHousin[CntPlayer].Model.pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PlayerHousin[CntPlayer].Model.nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_PlayerHousin[CntPlayer].Model.pD3DTexture);
				}
				// �`��
				{
					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);
					// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetStreamSource(0, g_PlayerHousin[CntPlayer].Model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetIndices(g_PlayerHousin[CntPlayer].Model.pD3DIdxBuff);
					//�`��
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHousin[CntPlayer].Model.nNumVertex, 0, g_PlayerHousin[CntPlayer].Model.nNumPolygon);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// �v���C���[�C������擾
//=============================================================================
PLAYER_HONTAI *GetPlayerHoudai(void)
{
	return &g_PlayerHoudai[0];
}

//=============================================================================
// �v���C���[�C�������擾
//=============================================================================
PLAYER_PRATS *GetPlayerHoutou(void)
{
	return &g_PlayerHoutou[0];
}

//=============================================================================
// �v���C���[�C�g�����擾
//=============================================================================
PLAYER_PRATS *GetPlayerHousin(void)
{
	return &g_PlayerHousin[0];
}

//=============================================================================
// ���b�V���J���[���Z�b�g
//=============================================================================
void SetPlayerMeshColor(GPUMODEL *model, int type)
{
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	model->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	model->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(model->nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// ���ˌ��̐ݒ�
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[type];
	}
	// ���_�f�[�^���A�����b�N����
	model->pD3DVtxBuff->Unlock();
	model->pD3DIdxBuff->Unlock();
}
