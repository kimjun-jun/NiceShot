/**
* @file player.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/object/player.h"
#include "../../h/object/bullet/bullet.h"
#include "../../h/scene/fade.h"
#include "../../h/object/camera.h"
#include "../../h/other/shadow.h"
#include "../../h/object/bullet/bullettex.h"
#include "../../h/map/field.h"
#include "../../h/other/sound.h"
#include "../../h/effect/effect.h"
#include "../../h/library.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER_HONTAI		g_PlayerHoudai[PLAYER_MAX];								 //!< �C��f�[�^�N���X(���f���̖{��)�@�`��L
static PLAYER_PRATS			g_PlayerHoutou[PLAYER_MAX];								 //!< �C���f�[�^�N���X(���f���̃p�[�c)�@�`��L
static PLAYER_PRATS			g_PlayerHousin[PLAYER_MAX];								 //!< �C�g�f�[�^�N���X(���f���̃p�[�c)�@�`��L
static PLAYER_PRATS			g_PlayerBulletStartPos[PLAYER_MAX];						 //!< �o���b�g���˃f�[�^�N���X(���f���̃p�[�c)�@�`�斳
static GPUMODEL				g_PlayerHousinOriginal[PLAYER_MAX];						 //!< �C�g�f�[�^�N���X(���_�f�[�^�̂ݕێ�)�@�񃂁[�t�B���O���ɒ��_�f�[�^�g�p
static GPUMODEL				g_PlayerHousinAtack[PLAYER_MAX];						 //!< �C�g�f�[�^�N���X(���_�f�[�^�̂ݕێ�)�@���[�t�B���O���ɒ��_�f�[�^�g�p

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4�J���[
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//PLAYER ������
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHoudai[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
		g_PlayerHoudai[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoudai[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].bulletmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].movepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_PlayerHoudai[CntPlayer].q = D3DXQUATERNION(0,0,0,1);
		g_PlayerHoudai[CntPlayer].RotVecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].Upvec = D3DXVECTOR3(0, 1, 0);
		g_PlayerHoudai[CntPlayer].Frontvec = D3DXVECTOR3(0, 0, 1);
		g_PlayerHoudai[CntPlayer].UpRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].UpFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].RightFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].LeftFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].DownFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].speed = 0.0f;
		g_PlayerHoudai[CntPlayer].speedbuff = 1.0f;
		g_PlayerHoudai[CntPlayer].speedbufftime = 0.0f;
		g_PlayerHoudai[CntPlayer].Qrot = 0.0f;
		g_PlayerHoudai[CntPlayer].Brot = 0.0f;
		g_PlayerHoudai[CntPlayer].dir = 1;
		g_PlayerHoudai[CntPlayer].use = true;
		g_PlayerHoudai[CntPlayer].Morphing = false;
		g_PlayerHoudai[CntPlayer].MorphingTime = MORPHING_TIME;
		g_PlayerHoudai[CntPlayer].MorphingEnd = true;
		g_PlayerHoudai[CntPlayer].speedbuffsignal = false;
		g_PlayerHoudai[CntPlayer].dashFlag = false;

		g_PlayerHoudai[CntPlayer].pD3DTexture = NULL;
		g_PlayerHoudai[CntPlayer].pD3DXMesh = NULL;
		g_PlayerHoudai[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerHoudai[CntPlayer].nNumMat = 0;
		g_PlayerHoudai[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerHoudai[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerHoudai[CntPlayer].nNumVertex = 0;
		g_PlayerHoudai[CntPlayer].nNumPolygon = 0;
		g_PlayerHoudai[CntPlayer].nNumVertexIndex = 0;

		g_PlayerHoudai[CntPlayer].Parent = NULL;
		g_PlayerHoudai[CntPlayer].KiriSignal = false;
		g_PlayerHoudai[CntPlayer].KiriItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].BackCameraItemSignal = false;
		g_PlayerHoudai[CntPlayer].BackCameraItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].AmmoNum = MAX_AMMO;
		g_PlayerHoudai[CntPlayer].AmmoBornCnt = 0.0f;
		g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
		g_PlayerHoudai[CntPlayer].RidePolygonNum = -1;

		// X�t�@�C���̓ǂݍ���
		if (LoadMesh(MODEL_HOUDAI, &g_PlayerHoudai[CntPlayer].pD3DXBuffMat,
			&g_PlayerHoudai[CntPlayer].nNumMat, &g_PlayerHoudai[CntPlayer].pD3DXMesh,
			&g_PlayerHoudai[CntPlayer].pD3DVtxBuff, &g_PlayerHoudai[CntPlayer].pD3DIdxBuff,
			&g_PlayerHoudai[CntPlayer].nNumVertex, &g_PlayerHoudai[CntPlayer].nNumPolygon,
			&g_PlayerHoudai[CntPlayer].nNumVertexIndex, &g_PlayerHoudai[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}
		
		//���_�J���[���v���C���[�F(��)�ɕύX
		SetPlayerMeshColor(&g_PlayerHoudai[CntPlayer], CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHoudai[CntPlayer].pD3DTexture);	// �ǂݍ��ރ������[


		/////////////////////////////////////////////////////////////////////////////////////////�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHoutou[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoutou[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].pD3DTexture = NULL;
		g_PlayerHoutou[CntPlayer].pD3DXMesh = NULL;
		g_PlayerHoutou[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerHoutou[CntPlayer].nNumMat = 0;
		g_PlayerHoutou[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerHoutou[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerHoutou[CntPlayer].nNumVertex = 0;
		g_PlayerHoutou[CntPlayer].nNumPolygon = 0;
		g_PlayerHoutou[CntPlayer].nNumVertexIndex = 0;
		g_PlayerHoutou[CntPlayer].ParentHontai = &g_PlayerHoudai[CntPlayer];

		// X�t�@�C���̓ǂݍ���
		if(LoadMesh(MODEL_HOUTOU, &g_PlayerHoutou[CntPlayer].pD3DXBuffMat,
			&g_PlayerHoutou[CntPlayer].nNumMat, &g_PlayerHoutou[CntPlayer].pD3DXMesh,
			&g_PlayerHoutou[CntPlayer].pD3DVtxBuff, &g_PlayerHoutou[CntPlayer].pD3DIdxBuff,
			&g_PlayerHoutou[CntPlayer].nNumVertex, &g_PlayerHoutou[CntPlayer].nNumPolygon,
			&g_PlayerHoutou[CntPlayer].nNumVertexIndex, &g_PlayerHoutou[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//���_�J���[���v���C���[�F�ɕύX
		SetPlayerMeshColor(&g_PlayerHoutou[CntPlayer], CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHoutou[CntPlayer].pD3DTexture);	// �ǂݍ��ރ������[

		///////////////////////////////////////////////////////////////////////////////////////////////�C�g
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHousin[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHousin[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].pD3DTexture = NULL;
		g_PlayerHousin[CntPlayer].pD3DXMesh = NULL;
		g_PlayerHousin[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerHousin[CntPlayer].nNumMat = 0;
		g_PlayerHousin[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerHousin[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerHousin[CntPlayer].nNumVertex = 0;
		g_PlayerHousin[CntPlayer].nNumPolygon = 0;
		g_PlayerHousin[CntPlayer].nNumVertexIndex = 0;
		g_PlayerHousin[CntPlayer].MorphingSignal = NoMorphing;
		g_PlayerHousin[CntPlayer].ParentParts = &g_PlayerHoutou[CntPlayer];

		// X�t�@�C���̓ǂݍ���
		if(LoadMesh(MODEL_HOUSIN, &g_PlayerHousin[CntPlayer].pD3DXBuffMat,
			&g_PlayerHousin[CntPlayer].nNumMat, &g_PlayerHousin[CntPlayer].pD3DXMesh,
			&g_PlayerHousin[CntPlayer].pD3DVtxBuff, &g_PlayerHousin[CntPlayer].pD3DIdxBuff,
			&g_PlayerHousin[CntPlayer].nNumVertex, &g_PlayerHousin[CntPlayer].nNumPolygon, 
			&g_PlayerHousin[CntPlayer].nNumVertexIndex,&g_PlayerHousin[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//���_�J���[���v���C���[�F(��)�ɕύX
		SetPlayerMeshColor(&g_PlayerHousin[CntPlayer], CntPlayer);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_MEISAI,									// �t�@�C���̖��O
			&g_PlayerHousin[CntPlayer].pD3DTexture);	// �ǂݍ��ރ������[


		///////////////////////////////////////////////////////////////////////////////////////////////�o���b�g���ˍ��W
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerBulletStartPos[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerBulletStartPos[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerBulletStartPos[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerBulletStartPos[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerBulletStartPos[CntPlayer].pD3DTexture = NULL;
		g_PlayerBulletStartPos[CntPlayer].pD3DXMesh = NULL;
		g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerBulletStartPos[CntPlayer].nNumMat = 0;
		g_PlayerBulletStartPos[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerBulletStartPos[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerBulletStartPos[CntPlayer].nNumVertex = 0;
		g_PlayerBulletStartPos[CntPlayer].nNumPolygon = 0;
		g_PlayerBulletStartPos[CntPlayer].nNumVertexIndex = 0;
		g_PlayerBulletStartPos[CntPlayer].MorphingSignal = NoMorphing;
		g_PlayerBulletStartPos[CntPlayer].ParentParts = &g_PlayerHousin[CntPlayer];

		// X�t�@�C���̓ǂݍ���
		if (LoadMesh(MODEL_BULLETPOS, &g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat,
			&g_PlayerBulletStartPos[CntPlayer].nNumMat, &g_PlayerBulletStartPos[CntPlayer].pD3DXMesh,
			&g_PlayerBulletStartPos[CntPlayer].pD3DVtxBuff, &g_PlayerBulletStartPos[CntPlayer].pD3DIdxBuff,
			&g_PlayerBulletStartPos[CntPlayer].nNumVertex, &g_PlayerBulletStartPos[CntPlayer].nNumPolygon,
			&g_PlayerBulletStartPos[CntPlayer].nNumVertexIndex, &g_PlayerBulletStartPos[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////�C�g�U���f�[�^
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


		///////////////////////////////////////////////////////////////////////////////////////////////////////////�C�g�ʏ�f�[�^
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



	//�������i�K�ō��W�Ɗp�x�������_���Őݒ�
	g_PlayerHoudai[0].pos = D3DXVECTOR3(699.0f + rand() % 10 , 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[1].pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[2].pos = D3DXVECTOR3(699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);
	g_PlayerHoudai[3].pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);

	g_PlayerHoudai[0].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[1].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[2].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[3].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHit(D3DXVECTOR3(g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y + 10.0f, g_PlayerHoudai[CntPlayer].pos.z),
			D3DXVECTOR3(g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y - 1000.0f, g_PlayerHoudai[CntPlayer].pos.z),
			&g_PlayerHoudai[CntPlayer].RotVecAxis, &g_PlayerHoudai[CntPlayer].pos.y);

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
		g_PlayerHoudai[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
		g_PlayerHoudai[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoudai[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].bulletmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].movepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].q = D3DXQUATERNION(0, 0, 0, 1);
		g_PlayerHoudai[CntPlayer].RotVecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].UpFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].RightFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].LeftFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].DownFieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].Upvec = D3DXVECTOR3(0, 1, 0);
		g_PlayerHoudai[CntPlayer].Frontvec = D3DXVECTOR3(0, 0, 1);
		g_PlayerHoudai[CntPlayer].UpRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoudai[CntPlayer].speed = 0.0f;
		g_PlayerHoudai[CntPlayer].speedbuff = 1.0f;
		g_PlayerHoudai[CntPlayer].speedbufftime = 0.0f;
		g_PlayerHoudai[CntPlayer].Qrot = 0.0f;
		g_PlayerHoudai[CntPlayer].Brot = 0.0f;
		g_PlayerHoudai[CntPlayer].dir = 1;
		g_PlayerHoudai[CntPlayer].use = true;
		g_PlayerHoudai[CntPlayer].Morphing = false;
		g_PlayerHoudai[CntPlayer].MorphingTime = MORPHING_TIME;
		g_PlayerHoudai[CntPlayer].MorphingEnd = true;
		g_PlayerHoudai[CntPlayer].speedbuffsignal = false;
		g_PlayerHoudai[CntPlayer].dashFlag = false;

		g_PlayerHoudai[CntPlayer].KiriSignal = false;
		g_PlayerHoudai[CntPlayer].KiriItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].BackCameraItemSignal = false;
		g_PlayerHoudai[CntPlayer].BackCameraItemTime = 0.0f;
		g_PlayerHoudai[CntPlayer].AmmoNum = MAX_AMMO;
		g_PlayerHoudai[CntPlayer].AmmoBornCnt = 0.0f;
		g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
		g_PlayerHoudai[CntPlayer].RidePolygonNum = -1;

		//�C��
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHoutou[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoutou[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//�C�g
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerHousin[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHousin[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].MorphingSignal = NoMorphing;

		//�o���b�g���ˍ��W
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_PlayerBulletStartPos[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerBulletStartPos[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerBulletStartPos[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerBulletStartPos[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ResetModel(&g_PlayerHousin[CntPlayer], &g_PlayerHousinOriginal[CntPlayer]);
	}

	// �e�̏�����
	//D3DXVECTOR3 pos = g_PlayerHoudai[0].parameter.pos;
	//pos.y = 0.0f;
	//g_PlayerHoudai[0].parameter.shadowIdx = CreateShadow(pos, g_PlayerHoudai[0].parameter.scl);

	g_PlayerHoudai[0].pos = D3DXVECTOR3(699.0f + rand() % 10, 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[1].pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, 699.0f + rand() % 10);
	g_PlayerHoudai[2].pos = D3DXVECTOR3(699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);
	g_PlayerHoudai[3].pos = D3DXVECTOR3(-699.0f + rand() % 10, 300.0f, -699.0f + rand() % 10);

	g_PlayerHoudai[0].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[1].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[2].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);
	g_PlayerHoudai[3].rot = D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f);

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		FieldHit(D3DXVECTOR3(g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y + 10.0f, g_PlayerHoudai[CntPlayer].pos.z),
			D3DXVECTOR3(g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y - 1000.0f, g_PlayerHoudai[CntPlayer].pos.z),
			&g_PlayerHoudai[CntPlayer].RotVecAxis, &g_PlayerHoudai[CntPlayer].pos.y);
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
		if (g_PlayerHoudai[CntPlayer].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHoudai[CntPlayer].pD3DTexture->Release();
			g_PlayerHoudai[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHoudai[CntPlayer].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHoudai[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHoudai[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerHoutou[CntPlayer].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHoutou[CntPlayer].pD3DTexture->Release();
			g_PlayerHoutou[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHoutou[CntPlayer].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHoutou[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHoutou[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerHousin[CntPlayer].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerHousin[CntPlayer].pD3DTexture->Release();
			g_PlayerHousin[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHousin[CntPlayer].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerHousin[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHousin[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerBulletStartPos[CntPlayer].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_PlayerBulletStartPos[CntPlayer].pD3DTexture->Release();
			g_PlayerBulletStartPos[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat != NULL)
		{// �}�e���A���̊J��
			g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat = NULL;
		}


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
	//���l���񂾂��v�Z�B�O�l���񂾂�Q�[���I���B���̃V�[����
	int deadcnt = 0;
	//�v���C���[�l�������[�v
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHoudai[CntPlayer].use == false) deadcnt++;
		if (deadcnt >= 3) SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
	}

	//�v���C���[����
	//�v���C���[�l�������[�v
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//�����Ă���ΐ���\
		if (g_PlayerHoudai[CntPlayer].use)
		{
			g_PlayerHoudai[CntPlayer].SetMoveL(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetQ(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetCamera(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetBulletALL(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetKiri(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetMorphing(CntPlayer);
		}
		//����ȊO�̓J������������
		else
		{
			g_PlayerHoudai[CntPlayer].SetQ(CntPlayer);
			CAMERA *cam = GetCamera();
			cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].pos.x - (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));
			cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].pos.y + (g_PlayerHousin[CntPlayer].rot.x*100.0f);
			cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].pos.z - (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));

			cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x + sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
			cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + POS_H_CAM;
			cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z + cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
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
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);
			
			//q=(rotVecAxis�@��)*(g_Player.rot��])
			D3DXQuaternionRotationAxis(&g_PlayerHoudai[CntPlayer].q, &g_PlayerHoudai[CntPlayer].UpRotTOaxis, -g_PlayerHoudai[CntPlayer].Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &g_PlayerHoudai[CntPlayer].q);

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_PlayerHoudai[CntPlayer].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_PlayerHoudai[CntPlayer].scl.x, g_PlayerHoudai[CntPlayer].scl.y, g_PlayerHoudai[CntPlayer].scl.z);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHoudai[CntPlayer].rot.y, g_PlayerHoudai[CntPlayer].rot.x, g_PlayerHoudai[CntPlayer].rot.z);

			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxRot);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxQ);


			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y, g_PlayerHoudai[CntPlayer].pos.z);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHoudai[CntPlayer].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHoudai[CntPlayer].pD3DXBuffMat->GetBufferPointer();

			// �`��
			for (int nCntMat = 0; nCntMat < (int)g_PlayerHoudai[CntPlayer].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_PlayerHoudai[CntPlayer].pD3DTexture);
				}

				// ���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);
				// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetStreamSource(0, g_PlayerHoudai[CntPlayer].pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
				pDevice->SetIndices(g_PlayerHoudai[CntPlayer].pD3DIdxBuff);
				//�`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHoudai[CntPlayer].nNumVertex, 0, g_PlayerHoudai[CntPlayer].nNumPolygon);
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
			D3DXMatrixIdentity(&g_PlayerHoutou[CntPlayer].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_PlayerHoutou[CntPlayer].scl.x, g_PlayerHoutou[CntPlayer].scl.y, g_PlayerHoutou[CntPlayer].scl.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHoutou[CntPlayer].rot.y, g_PlayerHoutou[CntPlayer].rot.x, g_PlayerHoutou[CntPlayer].rot.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHoutou[CntPlayer].pos.x, g_PlayerHoutou[CntPlayer].pos.y, g_PlayerHoutou[CntPlayer].pos.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &mtxTranslate);

			if (g_PlayerHoutou[CntPlayer].ParentHontai != NULL)
			{
				D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].ParentHontai->mtxWorld);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHoutou[CntPlayer].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHoutou[CntPlayer].pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PlayerHoutou[CntPlayer].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_PlayerHoutou[CntPlayer].pD3DTexture);
				}
				// �`��
				{
					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);
					// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetStreamSource(0, g_PlayerHoutou[CntPlayer].pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetIndices(g_PlayerHoutou[CntPlayer].pD3DIdxBuff);
					//�`��
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHoutou[CntPlayer].nNumVertex, 0, g_PlayerHoutou[CntPlayer].nNumPolygon);
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
			D3DXMatrixIdentity(&g_PlayerHousin[CntPlayer].mtxWorld);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_PlayerHousin[CntPlayer].scl.x, g_PlayerHousin[CntPlayer].scl.y, g_PlayerHousin[CntPlayer].scl.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHousin[CntPlayer].rot.y, g_PlayerHousin[CntPlayer].rot.x, g_PlayerHousin[CntPlayer].rot.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHousin[CntPlayer].pos.x, g_PlayerHousin[CntPlayer].pos.y, g_PlayerHousin[CntPlayer].pos.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &mtxTranslate);

			if (g_PlayerHousin[CntPlayer].ParentParts != NULL)
			{
				D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].ParentParts->mtxWorld);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHousin[CntPlayer].mtxWorld);

			if (g_PlayerBulletStartPos[CntPlayer].ParentParts != NULL)
			{
				g_PlayerBulletStartPos[CntPlayer].pos.x = g_PlayerBulletStartPos[CntPlayer].ParentParts->mtxWorld._41;
				g_PlayerBulletStartPos[CntPlayer].pos.y = g_PlayerBulletStartPos[CntPlayer].ParentParts->mtxWorld._42;
				g_PlayerBulletStartPos[CntPlayer].pos.z = g_PlayerBulletStartPos[CntPlayer].ParentParts->mtxWorld._43;
			}

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHousin[CntPlayer].pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PlayerHousin[CntPlayer].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_PlayerHousin[CntPlayer].pD3DTexture);
				}
				// �`��
				{
					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);
					// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetStreamSource(0, g_PlayerHousin[CntPlayer].pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
					pDevice->SetIndices(g_PlayerHousin[CntPlayer].pD3DIdxBuff);
					//�`��
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHousin[CntPlayer].nNumVertex, 0, g_PlayerHousin[CntPlayer].nNumPolygon);
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
// �v���C���[�o���b�g���ˍ��W�����擾
//=============================================================================
D3DXVECTOR3 GetPlayerBulletStartPos(int PlayerType)
{
	return g_PlayerBulletStartPos[PlayerType].pos;
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

//=============================================================================
// �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetMoveABL(int CntPlayer)
{
	g_PlayerHoudai[CntPlayer].oldpos = g_PlayerHoudai[CntPlayer].pos;

	//�ړ�����
	if (IsButtonPressed(CntPlayer, BUTTON_A))
	{
		g_PlayerHoudai[CntPlayer].dir = FRONT_VEC;
		g_PlayerHoudai[CntPlayer].speed += VALUE_MOVE;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_B))
	{
		g_PlayerHoudai[CntPlayer].dir = BACK_VEC;
		g_PlayerHoudai[CntPlayer].speed -= VALUE_MOVE;
	}
	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		g_PlayerHoudai[CntPlayer].speed *= MOVE_INERTIA_MOMENT;
	}

	//���_�ω��̃A�i���O�l�����ɑ�����ăA�i���O����Ő���
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//����
	if (!IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT) && !IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		g_PlayerHoudai[CntPlayer].dir = 1;
	}
	else if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
	{
		g_PlayerHoudai[CntPlayer].rot.y += LAnalogX * g_PlayerHoudai[CntPlayer].dir*g_PlayerHoudai[CntPlayer].speedbuff;
	}
	else if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT))
	{
		g_PlayerHoudai[CntPlayer].rot.y += LAnalogX * g_PlayerHoudai[CntPlayer].dir*g_PlayerHoudai[CntPlayer].speedbuff;
	}

	//�p�x�̐����l
	if (g_PlayerHoudai[CntPlayer].rot.y >= D3DX_PI*2) g_PlayerHoudai[CntPlayer].rot.y = 0.0f;
	else if (g_PlayerHoudai[CntPlayer].rot.y <= -D3DX_PI * 2) g_PlayerHoudai[CntPlayer].rot.y = 0.0f;

	// �ړ����x�̐���
	if (g_PlayerHoudai[CntPlayer].speed >= VALUE_MOVE_MAX) g_PlayerHoudai[CntPlayer].speed = VALUE_MOVE_MAX;
	else if (g_PlayerHoudai[CntPlayer].speed <= -VALUE_MOVE_MAX) g_PlayerHoudai[CntPlayer].speed = -VALUE_MOVE_MAX;

	// �v���C���[�̍��W���X�V
	g_PlayerHoudai[CntPlayer].pos.x -= sinf(g_PlayerHoudai[CntPlayer].rot.y) * (g_PlayerHoudai[CntPlayer].speed * g_PlayerHoudai[CntPlayer].speedbuff);
	g_PlayerHoudai[CntPlayer].pos.z -= cosf(g_PlayerHoudai[CntPlayer].rot.y) * (g_PlayerHoudai[CntPlayer].speed * g_PlayerHoudai[CntPlayer].speedbuff);

	//�X�s�[�h�o�t���Ԍ���
	if (g_PlayerHoudai[CntPlayer].speedbuffsignal == true)
	{
		g_PlayerHoudai[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y, g_PlayerHoudai[CntPlayer].pos.z);
		SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (g_PlayerHoudai[CntPlayer].speedbufftime <= 0.0f)
		{
			g_PlayerHoudai[CntPlayer].speedbuffsignal = false;
			g_PlayerHoudai[CntPlayer].speedbuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//�C������@�o���b�g���e�_(���E�G�C��)
	if (IsButtonPressed(CntPlayer, BUTTON_R1))
	{
		g_PlayerHoutou[CntPlayer].rot.y += VALUE_ROTATE_PLAYER_HOUTOU;
		if (g_PlayerHoutou[CntPlayer].rot.y >= VALUE_ROTATE_PLAYER_HOUTOU_MAX) g_PlayerHoutou[CntPlayer].rot.y = VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}
	else if (IsButtonPressed(CntPlayer, BUTTON_L1))
	{
		g_PlayerHoutou[CntPlayer].rot.y -= VALUE_ROTATE_PLAYER_HOUTOU;
		if (g_PlayerHoutou[CntPlayer].rot.y <= -VALUE_ROTATE_PLAYER_HOUTOU_MAX) g_PlayerHoutou[CntPlayer].rot.y = -VALUE_ROTATE_PLAYER_HOUTOU_MAX;
	}

	//�C�g����@�o���b�g���e�_(�O��G�C��)
	if (IsButtonPressed(CntPlayer, BUTTON_R2))
	{
		g_PlayerHousin[CntPlayer].rot.x += VALUE_ROTATE_PLAYER_HOUSIN;
		if (g_PlayerHousin[CntPlayer].rot.x >= VALUE_ROTATE_PLAYER_HOUSIN_MAX) g_PlayerHousin[CntPlayer].rot.x = VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(CntPlayer, BUTTON_L2))
	{
		g_PlayerHousin[CntPlayer].rot.x -= VALUE_ROTATE_PLAYER_HOUSIN;
		if (g_PlayerHousin[CntPlayer].rot.x <= -VALUE_ROTATE_PLAYER_HOUSIN_MAX) g_PlayerHousin[CntPlayer].rot.x = -VALUE_ROTATE_PLAYER_HOUSIN_MAX;
	}
}

//=============================================================================
// �J��������(AB�{�^��L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetCamera(int CntPlayer)
{
	CAMERA *cam = GetCamera();
	//�o�b�N�J��������
	//�o�b�N�J�����I���@�J�������_�A�����_
	//Y�{�^���������Ă���������́A�o�b�N�J�����A�C�e����ON�ɂȂ��Ă���Ƃ��̓J�������]
	if (IsButtonPressed(CntPlayer, BUTTON_Y) || g_PlayerHoudai[CntPlayer].BackCameraItemSignal == true)
	{
		cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].pos.x + (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));
		cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].pos.y + (g_PlayerHousin[CntPlayer].rot.x*100.0f);
		cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].pos.z + (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));

		cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x - sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z - cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
	}
	//�o�b�N�J�����I�t�@�J�������_�A�����_
	//����ȊO�͒ʏ�J����
	else
	{
		cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].pos.x - (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));
		cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].pos.y + (g_PlayerHousin[CntPlayer].rot.x*100.0f);
		cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].pos.z - (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));

		cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x + sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z + cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
	}
	//�����A�o�b�N�J�����A�C�e����ON�̎���Y�������ƒʏ�J�����ɂȂ�
	if (g_PlayerHoudai[CntPlayer].BackCameraItemSignal == true && IsButtonPressed(CntPlayer, BUTTON_Y))
	{
		cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].pos.x - (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));
		cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].pos.y + (g_PlayerHousin[CntPlayer].rot.x*100.0f);
		cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].pos.z - (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));

		cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x + sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z + cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
	}
	//�o�b�N�J�����̎��ԏ���
	if (g_PlayerHoudai[CntPlayer].BackCameraItemSignal == true)
	{
		g_PlayerHoudai[CntPlayer].BackCameraItemTime += 1.0f;
		if (g_PlayerHoudai[CntPlayer].BackCameraItemTime >= BACKCAMERA_TIME)
		{
			g_PlayerHoudai[CntPlayer].BackCameraItemTime = 0.0f;
			g_PlayerHoudai[CntPlayer].BackCameraItemSignal = false;
		}
	}
	cam[CntPlayer].up = g_PlayerHoudai[CntPlayer].Upvec;
}

//=============================================================================
// �ړ�����(L�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetMoveL(int CntPlayer)
{
	g_PlayerHoudai[CntPlayer].oldpos = g_PlayerHoudai[CntPlayer].pos;

	//�ړ��ω���L�X�e�B�b�N�A�i���O�l���g�p
	float LAnalogX = 0.0f;		//�c����
	float LAnalogY = 0.0f;		//������
	float DashRate = 1.0f;		//�X�s�[�h�A�b�v���[�g

	//�_�b�V������
	if (g_PlayerHoudai[CntPlayer].speedbuffsignal == true)
	{
		//�X�s�[�h�o�t���Ԍ���
		g_PlayerHoudai[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
		g_PlayerHoudai[CntPlayer].dashFlag = true;

		// �G�t�F�N�g�X�s�[�h�A�b�v�̐���
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y, g_PlayerHoudai[CntPlayer].pos.z);
		SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (g_PlayerHoudai[CntPlayer].speedbufftime <= 0.0f)
		{
			g_PlayerHoudai[CntPlayer].dashFlag = false;
			g_PlayerHoudai[CntPlayer].speedbuffsignal = false;
		}
	}

	if (g_PlayerHoudai[CntPlayer].dashFlag == true)
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
	}

	// ���ړ����͈ړ��ʂɊ�����������
	else
	{
		g_PlayerHoudai[CntPlayer].movepos *= 0.5f;
		g_PlayerHoudai[CntPlayer].dashFlag = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//�ړ��ʂ𔽉f
	g_PlayerHoudai[CntPlayer].movepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_PlayerHoudai[CntPlayer].rot.y += LAnalogX;
	g_PlayerHoudai[CntPlayer].movepos.x = LAnalogY * sinf(g_PlayerHoudai[CntPlayer].rot.y);
	g_PlayerHoudai[CntPlayer].movepos.z = LAnalogY * cosf(g_PlayerHoudai[CntPlayer].rot.y);

	//�v���C���[���W���X�V
	g_PlayerHoudai[CntPlayer].pos += g_PlayerHoudai[CntPlayer].movepos;

	SetCameraR(CntPlayer);
}

//=============================================================================
// �J��������(R�X�e�B�b�N�ňړ�����)
//=============================================================================
void PLAYER_HONTAI::SetCameraR(int CntPlayer)
{
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
	g_PlayerHoudai[CntPlayer].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_PlayerHoudai[CntPlayer].moverot.y = RAnalogX;
	g_PlayerHoudai[CntPlayer].moverot.x = -RAnalogY;


	g_PlayerHoutou[CntPlayer].rot.y += g_PlayerHoudai[CntPlayer].moverot.y;
	g_PlayerHousin[CntPlayer].rot.x += g_PlayerHoudai[CntPlayer].moverot.x;

	//�p�x�̐����l
	{
		if (g_PlayerHousin[CntPlayer].rot.x >= 0.3f)
		{
			g_PlayerHousin[CntPlayer].rot.x = 0.3f;
		}
		else if (g_PlayerHousin[CntPlayer].rot.x <= -0.3f)
		{
			g_PlayerHousin[CntPlayer].rot.x = -0.3f;
		}
	}

	////�J�����̎��_�X�V
	//cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x - (POS_W_CAM * cosf(-g_PlayerHoudai[CntPlayer].rot.y));
	//cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + (POS_H_CAM);
	//cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z - (POS_W_CAM * sinf(-g_PlayerHoudai[CntPlayer].rot.y));

	////�J�����̒����_���X�V
	////�e�N�j�b�N�@�J���������_����ŉ�]��atrotX���グ�܂����Ă��A���ꂾ���ł͐^��������Ȃ��B
	////atrotX�̑����ɔ�Ⴕ��atXZ�l�����������Ă����Ȃ��Ɛ^���^�������悤�Ȓ����_�𐧌�ł��Ȃ�
	////�΍�͂܂��v�����ĂȂ��BatrotX�̒l���傫���ق�rotY�̉e��������������B
	//cam[CntPlayer].at.x = cam[CntPlayer].pos.x + (POS_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y));
	//cam[CntPlayer].at.y = cam[CntPlayer].pos.y - (POS_H_CAM * sinf(g_PlayerHoudai[CntPlayer].atrot.x));
	//cam[CntPlayer].at.z = cam[CntPlayer].pos.z + (POS_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y));


}

//=============================================================================
// �N�H�[�^�j�I������
//=============================================================================
void PLAYER_HONTAI::SetQ(int CntPlayer)
{
	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
	D3DXVec3Cross(&g_PlayerHoudai[CntPlayer].UpRotTOaxis, &g_PlayerHoudai[CntPlayer].RotVecAxis, &g_PlayerHoudai[CntPlayer].Upvec);
	float Upkakezan = D3DXVec3Dot(&g_PlayerHoudai[CntPlayer].RotVecAxis, &g_PlayerHoudai[CntPlayer].Upvec);
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(g_PlayerHoudai[CntPlayer].RotVecAxis.x*g_PlayerHoudai[CntPlayer].RotVecAxis.x +
				g_PlayerHoudai[CntPlayer].RotVecAxis.y *g_PlayerHoudai[CntPlayer].RotVecAxis.y +
				g_PlayerHoudai[CntPlayer].RotVecAxis.z * g_PlayerHoudai[CntPlayer].RotVecAxis.z);
		g_PlayerHoudai[CntPlayer].Qrot = acosf(cossita);
	}
	else g_PlayerHoudai[CntPlayer].Qrot = 0.0f;
}

//=============================================================================
// �o���b�g�֘A����
//=============================================================================
void PLAYER_HONTAI::SetBulletALL(int CntPlayer)
{
	g_PlayerHoudai[CntPlayer].Frontvec.x = sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y);
	g_PlayerHoudai[CntPlayer].Frontvec.y = 0.0f;
	g_PlayerHoudai[CntPlayer].Frontvec.z = cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�B�o���b�g���˕����Ŏg��
	D3DXVec3Cross(&g_PlayerHoudai[CntPlayer].FrontRotTOaxis, &g_PlayerHoudai[CntPlayer].RotVecAxis, &g_PlayerHoudai[CntPlayer].Frontvec);
	float Bkakezan = D3DXVec3Dot(&g_PlayerHoudai[CntPlayer].RotVecAxis, &g_PlayerHoudai[CntPlayer].Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(g_PlayerHoudai[CntPlayer].RotVecAxis.x*g_PlayerHoudai[CntPlayer].RotVecAxis.x +
				g_PlayerHoudai[CntPlayer].RotVecAxis.y *g_PlayerHoudai[CntPlayer].RotVecAxis.y +
				g_PlayerHoudai[CntPlayer].RotVecAxis.z * g_PlayerHoudai[CntPlayer].RotVecAxis.z)
			*
			sqrtf(g_PlayerHoudai[CntPlayer].Frontvec.x*g_PlayerHoudai[CntPlayer].Frontvec.x +
				g_PlayerHoudai[CntPlayer].Frontvec.y *g_PlayerHoudai[CntPlayer].Frontvec.y +
				g_PlayerHoudai[CntPlayer].Frontvec.z * g_PlayerHoudai[CntPlayer].Frontvec.z);
		g_PlayerHoudai[CntPlayer].Brot = acosf(cossita);
	}
	else
	{
		g_PlayerHoudai[CntPlayer].Brot = 1.57f;		//�������x�N�g��rot=0.0f�A������x�N�g��rot=3.14�A�ɑ΂��Ă̑O�����x�N�g����rot=1.57f�B
	}
	g_PlayerHoudai[CntPlayer].Brot -= 1.57f;


	//�v���C���[pos���甭�˕����ɏ������炵���l
	//�n�ʂ̌X���ɉ����Ĕ��˂���Ƃ��͖��Ȃ��B���̌X�����獶�E�ɉ�]���Ă鎞����pos����������
	g_PlayerHoudai[CntPlayer].BposStart.x = g_PlayerHoudai[CntPlayer].pos.x - (sinf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y) * VALUE_LEN_BULLET);
	g_PlayerHoudai[CntPlayer].BposStart.y = g_PlayerHoudai[CntPlayer].pos.y + (sinf(g_PlayerHoudai[CntPlayer].Brot - g_PlayerHousin[CntPlayer].rot.x) * VALUE_LEN_BULLET) + 20.0f;
	g_PlayerHoudai[CntPlayer].BposStart.z = g_PlayerHoudai[CntPlayer].pos.z - (cosf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y) * VALUE_LEN_BULLET);


	g_PlayerHoudai[CntPlayer].BmoveRot.x = -sinf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y);
	g_PlayerHoudai[CntPlayer].BmoveRot.y = sinf(g_PlayerHoudai[CntPlayer].Brot - g_PlayerHousin[CntPlayer].rot.x);
	g_PlayerHoudai[CntPlayer].BmoveRot.z = -cosf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y);

	g_PlayerHoudai[CntPlayer].bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	g_PlayerHoudai[CntPlayer].bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	g_PlayerHoudai[CntPlayer].bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

	// �e����
	if (g_PlayerHoudai[CntPlayer].AmmoNum > 0)
	{
		//if (IsButtonTriggered(CntPlayer, BUTTON_X))
		//{
		if (IsButtonTriggered(CntPlayer, BUTTON_R1))
		{

			SetBullet(g_PlayerHoudai[CntPlayer].BposStart, g_PlayerHoudai[CntPlayer].bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);
			//SetBullet(g_PlayerBulletStartPos[CntPlayer].pos, move, 4.0f, 4.0f, 60 * 4, CntPlayer);

			//�g�U�e����
			if (g_PlayerHoudai[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
			{
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y + 0.3f)*VALUE_MOVE_BULLET,
					g_PlayerHoudai[CntPlayer].bulletmove.y,
					-cosf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y - 0.3f)*VALUE_MOVE_BULLET,
					g_PlayerHoudai[CntPlayer].bulletmove.y,
					-cosf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y - 0.3f) *VALUE_MOVE_BULLET);
				//SetBullet(BposStart, leftB, 4.0f, 4.0f, 60 * 4, CntPlayer);
				//SetBullet(BposStart, rightB, 4.0f, 4.0f, 60 * 4, CntPlayer);
				SetBullet(g_PlayerHoudai[CntPlayer].BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);
				SetBullet(g_PlayerHoudai[CntPlayer].BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);

			}
			g_PlayerHoudai[CntPlayer].AmmoNum -= 1;
			ChangeBulletTex(-1, CntPlayer);
			// SE�Đ�
			PlaySound(SOUND_LABEL_SE_attack03);
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

//=============================================================================
// �t�H�O����
//=============================================================================
void PLAYER_HONTAI::SetKiri(int CntPlayer)
{
	if (g_PlayerHoudai[CntPlayer].KiriSignal == true)
	{
		g_PlayerHoudai[CntPlayer].KiriItemTime += 1.0f;
		if (g_PlayerHoudai[CntPlayer].KiriItemTime >= KIRI_TIME)
		{
			g_PlayerHoudai[CntPlayer].KiriItemTime = 0.0f;
			g_PlayerHoudai[CntPlayer].KiriSignal = false;
		}
	}
}

//=============================================================================
// ���[�t�B���O����
//=============================================================================
void PLAYER_HONTAI::SetMorphing(int CntPlayer)
{
	// ���[�t�B���Otrue
	if (g_PlayerHoudai[CntPlayer].Morphing == true)
	{
		///////////////////////////////////////////////////////////////////////�o���b�g3���ԊJ�n
		// ���[�t�B���O���Ԍ��Z�J�n
		g_PlayerHoudai[CntPlayer].MorphingTime -= 1.0f;

		// ���[�t�B���O�U���^�C�v�ɕύX�J�n
		if (g_PlayerHousin[CntPlayer].MorphingSignal == NowMorphing)
		{
			g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
			DoMorphing(&g_PlayerHousin[CntPlayer], &g_PlayerHousinAtack[CntPlayer]);
		}
		///////////////////////////////////////////////////////////////////////�o���b�g3���ԏI��

		// ���Ԍo�߂Ń��f�������ɖ߂�
		else if (g_PlayerHoudai[CntPlayer].MorphingTime <= 0.0f)
		{
			g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
			g_PlayerHousin[CntPlayer].MorphingSignal = NowMorphing;
			g_PlayerHoudai[CntPlayer].Morphing = false;
		}
	}

	// ���[�t�B���O�I���W�i���^�C�v�ɕύX�J�n
	if (g_PlayerHoudai[CntPlayer].Morphing == false && g_PlayerHoudai[CntPlayer].MorphingTime <= 0.0f)
	{
		DoMorphing(&g_PlayerHousin[CntPlayer], &g_PlayerHousinOriginal[CntPlayer]);
		if (g_PlayerHousin[CntPlayer].MorphingSignal == EndMorphing)
		{
			g_PlayerHoudai[CntPlayer].MorphingTime = MORPHING_TIME;
			g_PlayerHousin[CntPlayer].time = 0.0f;
		}
	}
}

