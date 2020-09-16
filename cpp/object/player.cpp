/**
* @file player.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
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
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER_HONTAI		g_PlayerHoudai[PLAYER_MAX];								 //!< 砲台データクラス(モデルの本体)　描画有
static PLAYER_PRATS			g_PlayerHoutou[PLAYER_MAX];								 //!< 砲塔データクラス(モデルのパーツ)　描画有
static PLAYER_PRATS			g_PlayerHousin[PLAYER_MAX];								 //!< 砲身データクラス(モデルのパーツ)　描画有
static PLAYER_PRATS			g_PlayerBulletStartPos[PLAYER_MAX];						 //!< バレット発射データクラス(モデルのパーツ)　描画無
static GPUMODEL				g_PlayerHousinOriginal[PLAYER_MAX];						 //!< 砲身データクラス(頂点データのみ保持)　非モーフィング時に頂点データ使用
static GPUMODEL				g_PlayerHousinAtack[PLAYER_MAX];						 //!< 砲身データクラス(頂点データのみ保持)　モーフィング時に頂点データ使用

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//PLAYER 初期化
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////砲台
		// 位置・回転・スケールの初期設定
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

		// Xファイルの読み込み
		if (LoadMesh(MODEL_HOUDAI, &g_PlayerHoudai[CntPlayer].pD3DXBuffMat,
			&g_PlayerHoudai[CntPlayer].nNumMat, &g_PlayerHoudai[CntPlayer].pD3DXMesh,
			&g_PlayerHoudai[CntPlayer].pD3DVtxBuff, &g_PlayerHoudai[CntPlayer].pD3DIdxBuff,
			&g_PlayerHoudai[CntPlayer].nNumVertex, &g_PlayerHoudai[CntPlayer].nNumPolygon,
			&g_PlayerHoudai[CntPlayer].nNumVertexIndex, &g_PlayerHoudai[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}
		
		//頂点カラーをプレイヤー色(緑)に変更
		SetPlayerMeshColor(&g_PlayerHoudai[CntPlayer], CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&g_PlayerHoudai[CntPlayer].pD3DTexture);	// 読み込むメモリー


		/////////////////////////////////////////////////////////////////////////////////////////砲塔
		// 位置・回転・スケールの初期設定
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

		// Xファイルの読み込み
		if(LoadMesh(MODEL_HOUTOU, &g_PlayerHoutou[CntPlayer].pD3DXBuffMat,
			&g_PlayerHoutou[CntPlayer].nNumMat, &g_PlayerHoutou[CntPlayer].pD3DXMesh,
			&g_PlayerHoutou[CntPlayer].pD3DVtxBuff, &g_PlayerHoutou[CntPlayer].pD3DIdxBuff,
			&g_PlayerHoutou[CntPlayer].nNumVertex, &g_PlayerHoutou[CntPlayer].nNumPolygon,
			&g_PlayerHoutou[CntPlayer].nNumVertexIndex, &g_PlayerHoutou[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//頂点カラーをプレイヤー色に変更
		SetPlayerMeshColor(&g_PlayerHoutou[CntPlayer], CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&g_PlayerHoutou[CntPlayer].pD3DTexture);	// 読み込むメモリー

		///////////////////////////////////////////////////////////////////////////////////////////////砲身
		// 位置・回転・スケールの初期設定
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

		// Xファイルの読み込み
		if(LoadMesh(MODEL_HOUSIN, &g_PlayerHousin[CntPlayer].pD3DXBuffMat,
			&g_PlayerHousin[CntPlayer].nNumMat, &g_PlayerHousin[CntPlayer].pD3DXMesh,
			&g_PlayerHousin[CntPlayer].pD3DVtxBuff, &g_PlayerHousin[CntPlayer].pD3DIdxBuff,
			&g_PlayerHousin[CntPlayer].nNumVertex, &g_PlayerHousin[CntPlayer].nNumPolygon, 
			&g_PlayerHousin[CntPlayer].nNumVertexIndex,&g_PlayerHousin[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//頂点カラーをプレイヤー色(緑)に変更
		SetPlayerMeshColor(&g_PlayerHousin[CntPlayer], CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&g_PlayerHousin[CntPlayer].pD3DTexture);	// 読み込むメモリー


		///////////////////////////////////////////////////////////////////////////////////////////////バレット発射座標
		// 位置・回転・スケールの初期設定
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

		// Xファイルの読み込み
		if (LoadMesh(MODEL_BULLETPOS, &g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat,
			&g_PlayerBulletStartPos[CntPlayer].nNumMat, &g_PlayerBulletStartPos[CntPlayer].pD3DXMesh,
			&g_PlayerBulletStartPos[CntPlayer].pD3DVtxBuff, &g_PlayerBulletStartPos[CntPlayer].pD3DIdxBuff,
			&g_PlayerBulletStartPos[CntPlayer].nNumVertex, &g_PlayerBulletStartPos[CntPlayer].nNumPolygon,
			&g_PlayerBulletStartPos[CntPlayer].nNumVertexIndex, &g_PlayerBulletStartPos[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////砲身攻撃データ
		// 位置・回転・スケールの初期設定
		g_PlayerHousinAtack[CntPlayer].pD3DTexture = NULL;
		g_PlayerHousinAtack[CntPlayer].pD3DXMesh = NULL;
		g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerHousinAtack[CntPlayer].nNumMat = 0;
		g_PlayerHousinAtack[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerHousinAtack[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerHousinAtack[CntPlayer].nNumVertex = 0;
		g_PlayerHousinAtack[CntPlayer].nNumPolygon = 0;
		g_PlayerHousinAtack[CntPlayer].nNumVertexIndex = 0;

		// Xファイルの読み込み
		if(	LoadMesh(MODEL_HOUSINMO, &g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat,
			&g_PlayerHousinAtack[CntPlayer].nNumMat, &g_PlayerHousinAtack[CntPlayer].pD3DXMesh,
			&g_PlayerHousinAtack[CntPlayer].pD3DVtxBuff, &g_PlayerHousinAtack[CntPlayer].pD3DIdxBuff,
			&g_PlayerHousinAtack[CntPlayer].nNumVertex, &g_PlayerHousinAtack[CntPlayer].nNumPolygon,
			&g_PlayerHousinAtack[CntPlayer].nNumVertexIndex, &g_PlayerHousinAtack[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//頂点カラーをプレイヤー色に変更
		SetPlayerMeshColor(&g_PlayerHousinAtack[CntPlayer], CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&g_PlayerHousinAtack[CntPlayer].pD3DTexture);	// 読み込むメモリー


		///////////////////////////////////////////////////////////////////////////////////////////////////////////砲身通常データ
		// 位置・回転・スケールの初期設定
		g_PlayerHousinOriginal[CntPlayer].pD3DTexture = NULL;
		g_PlayerHousinOriginal[CntPlayer].pD3DXMesh = NULL;
		g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat = NULL;
		g_PlayerHousinOriginal[CntPlayer].nNumMat = 0;
		g_PlayerHousinOriginal[CntPlayer].pD3DVtxBuff = NULL;
		g_PlayerHousinOriginal[CntPlayer].pD3DIdxBuff = NULL;
		g_PlayerHousinOriginal[CntPlayer].nNumVertex = 0;
		g_PlayerHousinOriginal[CntPlayer].nNumPolygon = 0;
		g_PlayerHousinOriginal[CntPlayer].nNumVertexIndex = 0;

		// Xファイルの読み込み
		if(LoadMesh(MODEL_HOUSIN, &g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat,
			&g_PlayerHousinOriginal[CntPlayer].nNumMat, &g_PlayerHousinOriginal[CntPlayer].pD3DXMesh,
			&g_PlayerHousinOriginal[CntPlayer].pD3DVtxBuff, &g_PlayerHousinOriginal[CntPlayer].pD3DIdxBuff,
			&g_PlayerHousinOriginal[CntPlayer].nNumVertex, &g_PlayerHousinOriginal[CntPlayer].nNumPolygon,
			&g_PlayerHousinOriginal[CntPlayer].nNumVertexIndex, &g_PlayerHousinOriginal[CntPlayer].pD3DTexture))
		{
			return E_FAIL;
		}

		//頂点カラーをプレイヤー色に変更
		SetPlayerMeshColor(&g_PlayerHousinOriginal[CntPlayer], CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&g_PlayerHousinOriginal[CntPlayer].pD3DTexture);	// 読み込むメモリー

	}



	//初期化段階で座標と角度をランダムで設定
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
// 再初期化処理
//=============================================================================
HRESULT ReInitPlayer(void)
{
	//PLAYER 初期化
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//砲台
		// 位置・回転・スケールの初期設定
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

		//砲塔
		// 位置・回転・スケールの初期設定
		g_PlayerHoutou[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHoutou[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHoutou[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//砲身
		// 位置・回転・スケールの初期設定
		g_PlayerHousin[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerHousin[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerHousin[CntPlayer].MorphingSignal = NoMorphing;

		//バレット発射座標
		// 位置・回転・スケールの初期設定
		g_PlayerBulletStartPos[CntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerBulletStartPos[CntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PlayerBulletStartPos[CntPlayer].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_PlayerBulletStartPos[CntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ResetModel(&g_PlayerHousin[CntPlayer], &g_PlayerHousinOriginal[CntPlayer]);
	}

	// 影の初期化
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
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHoudai[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			g_PlayerHoudai[CntPlayer].pD3DTexture->Release();
			g_PlayerHoudai[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHoudai[CntPlayer].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_PlayerHoudai[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHoudai[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerHoutou[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			g_PlayerHoutou[CntPlayer].pD3DTexture->Release();
			g_PlayerHoutou[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHoutou[CntPlayer].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_PlayerHoutou[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHoutou[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerHousin[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			g_PlayerHousin[CntPlayer].pD3DTexture->Release();
			g_PlayerHousin[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHousin[CntPlayer].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_PlayerHousin[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHousin[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerBulletStartPos[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			g_PlayerBulletStartPos[CntPlayer].pD3DTexture->Release();
			g_PlayerBulletStartPos[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerBulletStartPos[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerHousinOriginal[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			g_PlayerHousinOriginal[CntPlayer].pD3DTexture->Release();
			g_PlayerHousinOriginal[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHousinOriginal[CntPlayer].pD3DXBuffMat = NULL;
		}


		if (g_PlayerHousinAtack[CntPlayer].pD3DTexture != NULL)
		{// テクスチャの開放
			g_PlayerHousinAtack[CntPlayer].pD3DTexture->Release();
			g_PlayerHousinAtack[CntPlayer].pD3DTexture = NULL;
		}
		if (g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat->Release();
			g_PlayerHousinAtack[CntPlayer].pD3DXBuffMat = NULL;
		}
	}
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//何人死んだか計算。三人死んだらゲーム終了。次のシーンへ
	int deadcnt = 0;
	//プレイヤー人数分ループ
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		if (g_PlayerHoudai[CntPlayer].use == false) deadcnt++;
		if (deadcnt >= 3) SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
	}

	//プレイヤー制御
	//プレイヤー人数分ループ
	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
		//生きていれば制御可能
		if (g_PlayerHoudai[CntPlayer].use)
		{
			g_PlayerHoudai[CntPlayer].SetMoveL(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetQ(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetCamera(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetBulletALL(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetKiri(CntPlayer);
			g_PlayerHoudai[CntPlayer].SetMorphing(CntPlayer);
		}
		//それ以外はカメラだけ制御
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
// エネミー更新処理
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
	//	{//視界内にプレイヤー
	//		g_EnemyHoudai[CntEnemy].mode = 4;
	//		g_EnemyHoudai[CntEnemy].model.MorphingSignal = NowMorphing;
	//	}
	//	//else if (g_EnemyHoudai[CntEnemy].EyeLen >= EIlen)
	//	//{//視界内にアイテム

	//	//}
	//	else if (GetKeyboardPress(DIK_X))
	//	{//視界外にプレイヤー
	//		g_EnemyHoudai[CntEnemy].mode = SEARCH;					//探索モード
	//	}

	//	switch (g_EnemyHoudai[CntEnemy].mode)
	//	{
	//	case SEARCH: //ランダムに探索　
	//		break;
	//	case ATTACK: //プレイヤーに攻撃
	//		SetEnemyBullet(g_EnemyHoudai[CntEnemy].parameter.pos, ENEMY_BULLET_TYPE01);
	//		break;
	//	case GOITEM: //アイテム取りに行く
	//		break;
	//	case DEAD: //死
	//		if (g_EnemyHoudai[CntEnemy].model.MorphingSignal == EndMorphing) g_EnemyHoudai[CntEnemy].mode = -1;
	//		break;
	//	case 4:
	//		if (g_EnemyHoudai[CntEnemy].model.MorphingSignal == EndMorphing) g_EnemyHoudai[CntEnemy].mode = -1;
	//		break;
	//	default:

	//		break;
	//	}


	//	// 影の位置設定
	//	{
	//		SetPositionShadow(g_EnemyHoudai[CntEnemy].parameter.shadowIdx,
	//			D3DXVECTOR3(g_EnemyHoudai[CntEnemy].parameter.pos.x, 0.0f, g_EnemyHoudai[CntEnemy].parameter.pos.z), g_EnemyHoudai[CntEnemy].parameter.scl);
	//	}
	//}

}

//=============================================================================
// 描画処理
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
			
			//q=(rotVecAxis法線)*(g_Player.rot回転)
			D3DXQuaternionRotationAxis(&g_PlayerHoudai[CntPlayer].q, &g_PlayerHoudai[CntPlayer].UpRotTOaxis, -g_PlayerHoudai[CntPlayer].Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &g_PlayerHoudai[CntPlayer].q);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_PlayerHoudai[CntPlayer].mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_PlayerHoudai[CntPlayer].scl.x, g_PlayerHoudai[CntPlayer].scl.y, g_PlayerHoudai[CntPlayer].scl.z);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHoudai[CntPlayer].rot.y, g_PlayerHoudai[CntPlayer].rot.x, g_PlayerHoudai[CntPlayer].rot.z);

			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxRot);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxQ);


			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y, g_PlayerHoudai[CntPlayer].pos.z);
			D3DXMatrixMultiply(&g_PlayerHoudai[CntPlayer].mtxWorld, &g_PlayerHoudai[CntPlayer].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHoudai[CntPlayer].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHoudai[CntPlayer].pD3DXBuffMat->GetBufferPointer();

			// 描画
			for (int nCntMat = 0; nCntMat < (int)g_PlayerHoudai[CntPlayer].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, g_PlayerHoudai[CntPlayer].pD3DTexture);
				}

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);
				// 頂点バッファをレンダリングパイプラインに設定
				pDevice->SetStreamSource(0, g_PlayerHoudai[CntPlayer].pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				// インデックスバッファをレンダリングパイプラインに設定
				pDevice->SetIndices(g_PlayerHoudai[CntPlayer].pD3DIdxBuff);
				//描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHoudai[CntPlayer].nNumVertex, 0, g_PlayerHoudai[CntPlayer].nNumPolygon);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_PlayerHoutou[CntPlayer].mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_PlayerHoutou[CntPlayer].scl.x, g_PlayerHoutou[CntPlayer].scl.y, g_PlayerHoutou[CntPlayer].scl.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHoutou[CntPlayer].rot.y, g_PlayerHoutou[CntPlayer].rot.x, g_PlayerHoutou[CntPlayer].rot.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHoutou[CntPlayer].pos.x, g_PlayerHoutou[CntPlayer].pos.y, g_PlayerHoutou[CntPlayer].pos.z);
			D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &mtxTranslate);

			if (g_PlayerHoutou[CntPlayer].ParentHontai != NULL)
			{
				D3DXMatrixMultiply(&g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].mtxWorld, &g_PlayerHoutou[CntPlayer].ParentHontai->mtxWorld);
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHoutou[CntPlayer].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHoutou[CntPlayer].pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PlayerHoutou[CntPlayer].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, g_PlayerHoutou[CntPlayer].pD3DTexture);
				}
				// 描画
				{
					// 頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_3D);
					// 頂点バッファをレンダリングパイプラインに設定
					pDevice->SetStreamSource(0, g_PlayerHoutou[CntPlayer].pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// インデックスバッファをレンダリングパイプラインに設定
					pDevice->SetIndices(g_PlayerHoutou[CntPlayer].pD3DIdxBuff);
					//描画
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHoutou[CntPlayer].nNumVertex, 0, g_PlayerHoutou[CntPlayer].nNumPolygon);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
	}

	for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
	{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_PlayerHousin[CntPlayer].mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_PlayerHousin[CntPlayer].scl.x, g_PlayerHousin[CntPlayer].scl.y, g_PlayerHousin[CntPlayer].scl.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PlayerHousin[CntPlayer].rot.y, g_PlayerHousin[CntPlayer].rot.x, g_PlayerHousin[CntPlayer].rot.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_PlayerHousin[CntPlayer].pos.x, g_PlayerHousin[CntPlayer].pos.y, g_PlayerHousin[CntPlayer].pos.z);
			D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &mtxTranslate);

			if (g_PlayerHousin[CntPlayer].ParentParts != NULL)
			{
				D3DXMatrixMultiply(&g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].mtxWorld, &g_PlayerHousin[CntPlayer].ParentParts->mtxWorld);
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_PlayerHousin[CntPlayer].mtxWorld);

			if (g_PlayerBulletStartPos[CntPlayer].ParentParts != NULL)
			{
				g_PlayerBulletStartPos[CntPlayer].pos.x = g_PlayerBulletStartPos[CntPlayer].ParentParts->mtxWorld._41;
				g_PlayerBulletStartPos[CntPlayer].pos.y = g_PlayerBulletStartPos[CntPlayer].ParentParts->mtxWorld._42;
				g_PlayerBulletStartPos[CntPlayer].pos.z = g_PlayerBulletStartPos[CntPlayer].ParentParts->mtxWorld._43;
			}

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)g_PlayerHousin[CntPlayer].pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_PlayerHousin[CntPlayer].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, g_PlayerHousin[CntPlayer].pD3DTexture);
				}
				// 描画
				{
					// 頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_3D);
					// 頂点バッファをレンダリングパイプラインに設定
					pDevice->SetStreamSource(0, g_PlayerHousin[CntPlayer].pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// インデックスバッファをレンダリングパイプラインに設定
					pDevice->SetIndices(g_PlayerHousin[CntPlayer].pD3DIdxBuff);
					//描画
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_PlayerHousin[CntPlayer].nNumVertex, 0, g_PlayerHousin[CntPlayer].nNumPolygon);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// プレイヤー砲台情報を取得
//=============================================================================
PLAYER_HONTAI *GetPlayerHoudai(void)
{
	return &g_PlayerHoudai[0];
}

//=============================================================================
// プレイヤー砲塔情報を取得
//=============================================================================
PLAYER_PRATS *GetPlayerHoutou(void)
{
	return &g_PlayerHoutou[0];
}

//=============================================================================
// プレイヤー砲身情報を取得
//=============================================================================
PLAYER_PRATS *GetPlayerHousin(void)
{
	return &g_PlayerHousin[0];
}

//=============================================================================
// プレイヤーバレット発射座標情報を取得
//=============================================================================
D3DXVECTOR3 GetPlayerBulletStartPos(int PlayerType)
{
	return g_PlayerBulletStartPos[PlayerType].pos;
}

//=============================================================================
// メッシュカラーをセット
//=============================================================================
void SetPlayerMeshColor(GPUMODEL *model, int type)
{
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	model->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	model->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(model->nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// 反射光の設定
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[type];
	}
	// 頂点データをアンロックする
	model->pD3DVtxBuff->Unlock();
	model->pD3DIdxBuff->Unlock();
}

//=============================================================================
// 移動制御(ABボタンLスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetMoveABL(int CntPlayer)
{
	g_PlayerHoudai[CntPlayer].oldpos = g_PlayerHoudai[CntPlayer].pos;

	//移動処理
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
	// 無移動時は移動量に慣性をかける
	else
	{
		g_PlayerHoudai[CntPlayer].speed *= MOVE_INERTIA_MOMENT;
	}

	//視点変化のアナログ値を旋回に代入してアナログ操作で旋回
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//旋回
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

	//角度の制限値
	if (g_PlayerHoudai[CntPlayer].rot.y >= D3DX_PI*2) g_PlayerHoudai[CntPlayer].rot.y = 0.0f;
	else if (g_PlayerHoudai[CntPlayer].rot.y <= -D3DX_PI * 2) g_PlayerHoudai[CntPlayer].rot.y = 0.0f;

	// 移動速度の制限
	if (g_PlayerHoudai[CntPlayer].speed >= VALUE_MOVE_MAX) g_PlayerHoudai[CntPlayer].speed = VALUE_MOVE_MAX;
	else if (g_PlayerHoudai[CntPlayer].speed <= -VALUE_MOVE_MAX) g_PlayerHoudai[CntPlayer].speed = -VALUE_MOVE_MAX;

	// プレイヤーの座標を更新
	g_PlayerHoudai[CntPlayer].pos.x -= sinf(g_PlayerHoudai[CntPlayer].rot.y) * (g_PlayerHoudai[CntPlayer].speed * g_PlayerHoudai[CntPlayer].speedbuff);
	g_PlayerHoudai[CntPlayer].pos.z -= cosf(g_PlayerHoudai[CntPlayer].rot.y) * (g_PlayerHoudai[CntPlayer].speed * g_PlayerHoudai[CntPlayer].speedbuff);

	//スピードバフ時間減少
	if (g_PlayerHoudai[CntPlayer].speedbuffsignal == true)
	{
		g_PlayerHoudai[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(g_PlayerHoudai[CntPlayer].pos.x, g_PlayerHoudai[CntPlayer].pos.y, g_PlayerHoudai[CntPlayer].pos.z);
		SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (g_PlayerHoudai[CntPlayer].speedbufftime <= 0.0f)
		{
			g_PlayerHoudai[CntPlayer].speedbuffsignal = false;
			g_PlayerHoudai[CntPlayer].speedbuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//砲塔操作　バレット着弾点(左右エイム)
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

	//砲身操作　バレット着弾点(前後エイム)
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
// カメラ制御(ABボタンLスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetCamera(int CntPlayer)
{
	CAMERA *cam = GetCamera();
	//バックカメラ処理
	//バックカメラオン　カメラ視点、注視点
	//Yボタンを押しているもしくは、バックカメラアイテムがONになっているときはカメラ反転
	if (IsButtonPressed(CntPlayer, BUTTON_Y) || g_PlayerHoudai[CntPlayer].BackCameraItemSignal == true)
	{
		cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].pos.x + (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));
		cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].pos.y + (g_PlayerHousin[CntPlayer].rot.x*100.0f);
		cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].pos.z + (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));

		cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x - sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z - cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
	}
	//バックカメラオフ　カメラ視点、注視点
	//それ以外は通常カメラ
	else
	{
		cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].pos.x - (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));
		cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].pos.y + (g_PlayerHousin[CntPlayer].rot.x*100.0f);
		cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].pos.z - (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));

		cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x + sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z + cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
	}
	//もし、バックカメラアイテムがONの時にYを押すと通常カメラになる
	if (g_PlayerHoudai[CntPlayer].BackCameraItemSignal == true && IsButtonPressed(CntPlayer, BUTTON_Y))
	{
		cam[CntPlayer].at.x = g_PlayerHoudai[CntPlayer].pos.x - (AT_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));
		cam[CntPlayer].at.y = g_PlayerHoudai[CntPlayer].pos.y + (g_PlayerHousin[CntPlayer].rot.x*100.0f);
		cam[CntPlayer].at.z = g_PlayerHoudai[CntPlayer].pos.z - (AT_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y));

		cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x + sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z + cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y) * cam[CntPlayer].len;
	}
	//バックカメラの時間処理
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
// 移動制御(Lスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetMoveL(int CntPlayer)
{
	g_PlayerHoudai[CntPlayer].oldpos = g_PlayerHoudai[CntPlayer].pos;

	//移動変化はLスティックアナログ値を使用
	float LAnalogX = 0.0f;		//縦入力
	float LAnalogY = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (g_PlayerHoudai[CntPlayer].speedbuffsignal == true)
	{
		//スピードバフ時間減少
		g_PlayerHoudai[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
		g_PlayerHoudai[CntPlayer].dashFlag = true;

		// エフェクトスピードアップの生成
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

	//移動処理
	if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_UP) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(CntPlayer);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
	}

	// 無移動時は移動量に慣性をかける
	else
	{
		g_PlayerHoudai[CntPlayer].movepos *= 0.5f;
		g_PlayerHoudai[CntPlayer].dashFlag = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//移動量を反映
	g_PlayerHoudai[CntPlayer].movepos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_PlayerHoudai[CntPlayer].rot.y += LAnalogX;
	g_PlayerHoudai[CntPlayer].movepos.x = LAnalogY * sinf(g_PlayerHoudai[CntPlayer].rot.y);
	g_PlayerHoudai[CntPlayer].movepos.z = LAnalogY * cosf(g_PlayerHoudai[CntPlayer].rot.y);

	//プレイヤー座標を更新
	g_PlayerHoudai[CntPlayer].pos += g_PlayerHoudai[CntPlayer].movepos;

	SetCameraR(CntPlayer);
}

//=============================================================================
// カメラ制御(Rスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetCameraR(int CntPlayer)
{
	CAMERA *cam = GetCamera();

	//視野角変化はRスティックアナログ値を使用
	float RAnalogX = 0.0f;		//縦入力
	float RAnalogY = 0.0f;		//横入力

	//視野角処理
	if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_UP) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_DOWN) ||
		IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_R_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
		//入力中央値32000　最小0　最大64000
		//なので-32000することで　中央値0　最小-32000　最大32000にしている
		//rotに32000とかバカ高い数値を入れるとぶっ飛ぶので、さらに入力値を小さくする
		//最大0.03くらいになるよう調整　/1000000する(操作しやすい値でいい)
		RAnalogX = float(Button->lRx) - 32000.0f;
		RAnalogY = float(Button->lRy) - 32800.0f;

		RAnalogX = RAnalogX / 1000000.0f;
		RAnalogY = RAnalogY / 1000000.0f;

	}

	//回転量を反映
	g_PlayerHoudai[CntPlayer].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_PlayerHoudai[CntPlayer].moverot.y = RAnalogX;
	g_PlayerHoudai[CntPlayer].moverot.x = -RAnalogY;


	g_PlayerHoutou[CntPlayer].rot.y += g_PlayerHoudai[CntPlayer].moverot.y;
	g_PlayerHousin[CntPlayer].rot.x += g_PlayerHoudai[CntPlayer].moverot.x;

	//角度の制限値
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

	////カメラの視点更新
	//cam[CntPlayer].pos.x = g_PlayerHoudai[CntPlayer].pos.x - (POS_W_CAM * cosf(-g_PlayerHoudai[CntPlayer].rot.y));
	//cam[CntPlayer].pos.y = g_PlayerHoudai[CntPlayer].pos.y + (POS_H_CAM);
	//cam[CntPlayer].pos.z = g_PlayerHoudai[CntPlayer].pos.z - (POS_W_CAM * sinf(-g_PlayerHoudai[CntPlayer].rot.y));

	////カメラの注視点を更新
	////テクニック　カメラ注視点制御で回転軸atrotXを上げまくっても、それだけでは真上を向かない。
	////atrotXの増減に比例してatXZ値も減少させていかないと真下真上を見るような注視点を制御できない
	////対策はまだ思いついてない。atrotXの値が大きいほどrotYの影響を小さくする。
	//cam[CntPlayer].at.x = cam[CntPlayer].pos.x + (POS_W_CAM * sinf(g_PlayerHoudai[CntPlayer].rot.y));
	//cam[CntPlayer].at.y = cam[CntPlayer].pos.y - (POS_H_CAM * sinf(g_PlayerHoudai[CntPlayer].atrot.x));
	//cam[CntPlayer].at.z = cam[CntPlayer].pos.z + (POS_W_CAM * cosf(g_PlayerHoudai[CntPlayer].rot.y));


}

//=============================================================================
// クォータニオン制御
//=============================================================================
void PLAYER_HONTAI::SetQ(int CntPlayer)
{
	//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
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
// バレット関連制御
//=============================================================================
void PLAYER_HONTAI::SetBulletALL(int CntPlayer)
{
	g_PlayerHoudai[CntPlayer].Frontvec.x = sinf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y);
	g_PlayerHoudai[CntPlayer].Frontvec.y = 0.0f;
	g_PlayerHoudai[CntPlayer].Frontvec.z = cosf(g_PlayerHoudai[CntPlayer].rot.y + g_PlayerHoutou[CntPlayer].rot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
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
		g_PlayerHoudai[CntPlayer].Brot = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	g_PlayerHoudai[CntPlayer].Brot -= 1.57f;


	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
	g_PlayerHoudai[CntPlayer].BposStart.x = g_PlayerHoudai[CntPlayer].pos.x - (sinf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y) * VALUE_LEN_BULLET);
	g_PlayerHoudai[CntPlayer].BposStart.y = g_PlayerHoudai[CntPlayer].pos.y + (sinf(g_PlayerHoudai[CntPlayer].Brot - g_PlayerHousin[CntPlayer].rot.x) * VALUE_LEN_BULLET) + 20.0f;
	g_PlayerHoudai[CntPlayer].BposStart.z = g_PlayerHoudai[CntPlayer].pos.z - (cosf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y) * VALUE_LEN_BULLET);


	g_PlayerHoudai[CntPlayer].BmoveRot.x = -sinf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y);
	g_PlayerHoudai[CntPlayer].BmoveRot.y = sinf(g_PlayerHoudai[CntPlayer].Brot - g_PlayerHousin[CntPlayer].rot.x);
	g_PlayerHoudai[CntPlayer].BmoveRot.z = -cosf(g_PlayerHoutou[CntPlayer].rot.y + g_PlayerHoudai[CntPlayer].rot.y);

	g_PlayerHoudai[CntPlayer].bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
	g_PlayerHoudai[CntPlayer].bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
	g_PlayerHoudai[CntPlayer].bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

	// 弾発射
	if (g_PlayerHoudai[CntPlayer].AmmoNum > 0)
	{
		//if (IsButtonTriggered(CntPlayer, BUTTON_X))
		//{
		if (IsButtonTriggered(CntPlayer, BUTTON_R1))
		{

			SetBullet(g_PlayerHoudai[CntPlayer].BposStart, g_PlayerHoudai[CntPlayer].bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);
			//SetBullet(g_PlayerBulletStartPos[CntPlayer].pos, move, 4.0f, 4.0f, 60 * 4, CntPlayer);

			//拡散弾処理
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
			// SE再生
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}


	//残弾復活 一定時間経過で1個づつ自動回復
	if (g_PlayerHoudai[CntPlayer].AmmoNum < MAX_AMMO) g_PlayerHoudai[CntPlayer].AmmoBornCnt += BORN_AMMO_ADDTIME;
	if (g_PlayerHoudai[CntPlayer].AmmoBornCnt >= BORN_AMMO_MAXTIME)
	{
		g_PlayerHoudai[CntPlayer].AmmoNum++;
		ChangeBulletTex(1, CntPlayer);
		g_PlayerHoudai[CntPlayer].AmmoBornCnt = 0.0f;
	}

}

//=============================================================================
// フォグ制御
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
// モーフィング制御
//=============================================================================
void PLAYER_HONTAI::SetMorphing(int CntPlayer)
{
	// モーフィングtrue
	if (g_PlayerHoudai[CntPlayer].Morphing == true)
	{
		///////////////////////////////////////////////////////////////////////バレット3つ時間開始
		// モーフィング時間減算開始
		g_PlayerHoudai[CntPlayer].MorphingTime -= 1.0f;

		// モーフィング攻撃タイプに変更開始
		if (g_PlayerHousin[CntPlayer].MorphingSignal == NowMorphing)
		{
			g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
			DoMorphing(&g_PlayerHousin[CntPlayer], &g_PlayerHousinAtack[CntPlayer]);
		}
		///////////////////////////////////////////////////////////////////////バレット3つ時間終了

		// 時間経過でモデルを元に戻す
		else if (g_PlayerHoudai[CntPlayer].MorphingTime <= 0.0f)
		{
			g_PlayerHoudai[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
			g_PlayerHousin[CntPlayer].MorphingSignal = NowMorphing;
			g_PlayerHoudai[CntPlayer].Morphing = false;
		}
	}

	// モーフィングオリジナルタイプに変更開始
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

