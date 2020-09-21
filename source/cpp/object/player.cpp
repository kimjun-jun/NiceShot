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
#include "../../h/object/shadow.h"
#include "../../h/map/field.h"
#include "../../h/other/sound.h"
#include "../../h/effect/effect.h"
#include "../../h/library.h"
#include "../../h/object/objectclass.h"

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),//p4カラー
};

//=============================================================================
// 初期化処理
//=============================================================================
void PLAYER_HONTAI::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//PLAYER 初期化
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////砲台
		this[CntPlayer].SetRot(D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		this[CntPlayer].SetUse = true;

		this[CntPlayer].speedbuff = 1.0f;
		this[CntPlayer].MorphingTime = MORPHING_TIME;
		this[CntPlayer].MorphingEnd = true;
		this[CntPlayer].Parent = NULL;
		this[CntPlayer].AmmoCnt = PLAYER_AMMOPOWER_NORMAL;
		this[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
		this[CntPlayer].vital = PLAYER_VITAL;
		this[CntPlayer].MorphingSignal = NoMorphing;

		// Xファイルの読み込み
		if (LoadMesh(MODEL_HOUDAI, &this[CntPlayer].model.pD3DXBuffMat,
			&this[CntPlayer].model.nNumMat, &this[CntPlayer].model.pD3DXMesh,
			&this[CntPlayer].model.pD3DVtxBuff, &this[CntPlayer].model.pD3DIdxBuff,
			&this[CntPlayer].model.nNumVertex, &this[CntPlayer].model.nNumPolygon,
			&this[CntPlayer].model.nNumVertexIndex, &this[CntPlayer].model.pD3DTexture));

		//頂点カラーをプレイヤー色に変更
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].model.pD3DVtxBuff,
			this[CntPlayer].model.pD3DIdxBuff, this[CntPlayer].model.nNumPolygon, CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&this[CntPlayer].model.pD3DTexture);	// 読み込むメモリー


		/////////////////////////////////////////////////////////////////////////////////////////砲塔
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai = &this[CntPlayer];

		// Xファイルの読み込み
		if (LoadMesh(MODEL_HOUTOU, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DXBuffMat,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumMat, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DXMesh,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DVtxBuff, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DIdxBuff,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumVertex, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumPolygon,
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumVertexIndex, &this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DTexture));

		//頂点カラーをプレイヤー色に変更
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DVtxBuff,
			this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DIdxBuff, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumPolygon, CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DTexture);	// 読み込むメモリー

		///////////////////////////////////////////////////////////////////////////////////////////////砲身
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts = &this[CntPlayer].parts[PARTSTYPE_HOUTOU];

		// Xファイルの読み込み
		if (LoadMesh(MODEL_HOUSIN, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DXBuffMat,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumMat, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DXMesh,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DVtxBuff, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DIdxBuff,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumVertex, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumPolygon,
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumVertexIndex, &this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DTexture));

			//頂点カラーをプレイヤー色に変更
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DVtxBuff,
			this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DIdxBuff, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumPolygon, CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DTexture);	// 読み込むメモリー


		///////////////////////////////////////////////////////////////////////////////////////////////////////////砲身通常データ
		// 位置・回転・スケールの初期設定
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture = NULL;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXMesh = NULL;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXBuffMat = NULL;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumMat = 0;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DVtxBuff = NULL;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DIdxBuff = NULL;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumVertex = 0;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumPolygon = 0;
		this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumVertexIndex = 0;

		// Xファイルの読み込み
		if (LoadMesh(MODEL_HOUSIN, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXBuffMat,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumMat, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXMesh,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DVtxBuff, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DIdxBuff,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumVertex, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumPolygon,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumVertexIndex, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture));

		//頂点カラーをプレイヤー色に変更
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DVtxBuff,
			this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DIdxBuff, this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].nNumPolygon, CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture);	// 読み込むメモリー


		///////////////////////////////////////////////////////////////////////////////////////////////////////砲身攻撃データ

		// Xファイルの読み込み
		if (LoadMesh(MODEL_HOUSINMO, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DXBuffMat,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumMat, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DXMesh,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DVtxBuff, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DIdxBuff,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumVertex, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumPolygon,
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumVertexIndex, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DTexture));

		//頂点カラーをプレイヤー色に変更
		this[CntPlayer].SetPlayerMeshColor(this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DVtxBuff,
			this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DIdxBuff, this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].nNumPolygon, CntPlayer);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_MEISAI,									// ファイルの名前
			&this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DTexture);	// 読み込むメモリー

	}



	//初期化段階で座標と角度をランダムで設定
	this[0].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[1].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[2].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));
	this[3].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));

	this[0].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[1].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[2].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[3].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));

	//初期化後の状態で一度カメラ等の処理をする
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		D3DXVECTOR3 RayStart = this[CntPlayer].GetPos();
		RayStart.y += 10.0f;
		D3DXVECTOR3 RayEnd = this[CntPlayer].GetPos();
		RayEnd.y -= 1000.0f;

		float ReturnPosY = 0.0f;
		D3DXVECTOR3 FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		FieldHitGetSphereVec(RayStart, RayEnd, &FieldNorVec, &ReturnPosY);

		D3DXVECTOR3 SetPos = RayStart;
		SetPos.y = ReturnPosY;
		this[CntPlayer].SetPos(SetPos);
		this[CntPlayer].SetFieldNorVec(FieldNorVec);
		this[CntPlayer].SetQ(CntPlayer);
		this[CntPlayer].SetCamera(CntPlayer);

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
// 再初期化処理
//=============================================================================
void PLAYER_HONTAI::Reinit(void)
{
	//PLAYER 初期化
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//砲台
		this[CntPlayer].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetOldPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetRot(D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		this[CntPlayer].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[CntPlayer].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].SetUse(true);

		this[CntPlayer].FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this[CntPlayer].speed = 0.0f;
		this[CntPlayer].speedbuff = 1.0f;
		this[CntPlayer].speedbufftime = 0.0f;
		this[CntPlayer].SetQrot = 0.0f;
		this[CntPlayer].Brot = 0.0f;
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
		this[CntPlayer].vital = PLAYER_VITAL;
		this[CntPlayer].MorphingSignal = NoMorphing;

		//砲塔
		// 位置・回転・スケールの初期設定
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//砲身								 
		// 位置・回転・スケールの初期設定
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetScl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		ResetModel(&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model, &this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL]);
	}

	//初期化段階で座標と角度をランダムで設定
	this[0].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[1].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this[2].SetPos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));
	this[3].SetPos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));

	this[0].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[1].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[2].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this[3].SetRot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));

	//初期化後の状態で一度カメラ等の処理をする
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		D3DXVECTOR3 RayStart = this[CntPlayer].GetPos();
		RayStart.y += 10.0f;
		D3DXVECTOR3 RayEnd = this[CntPlayer].GetPos();
		RayEnd.y -= 1000.0f;

		float ReturnPosY = 0.0f;
		D3DXVECTOR3 FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		FieldHitGetSphereVec(RayStart, RayEnd, &FieldNorVec, &ReturnPosY);

		D3DXVECTOR3 SetPos = RayStart;
		SetPos.y = ReturnPosY;
		this[CntPlayer].SetPos(SetPos);
		this[CntPlayer].SetFieldNorVec(FieldNorVec);
		this[CntPlayer].SetQ(CntPlayer);
		this[CntPlayer].SetCamera(CntPlayer);

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
// 終了処理
//=============================================================================
void PLAYER_HONTAI::Uninit(void)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DTexture != NULL)
		{// テクスチャの開放
			this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DTexture->Release();
			this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DTexture = NULL;
		}
		if (this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DXBuffMat->Release();
			this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK].pD3DXBuffMat = NULL;
		}

		if (this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture != NULL)
		{// テクスチャの開放
			this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture->Release();
			this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DTexture = NULL;
		}
		if (this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXBuffMat != NULL)
		{// マテリアルの開放
			this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXBuffMat->Release();
			this[CntPlayer].ModelDate[PLAYER_MODEL_NORMAL].pD3DXBuffMat = NULL;
		}
	}
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void PLAYER_HONTAI::Update(void)
{
	//何人死んだか計算。三人死んだらゲーム終了。次のシーンへ
	int deadcnt = 0;
	//プレイヤー人数分ループ
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool use = this[CntPlayer].GetUse();
		if (use == false) deadcnt++;
		if (deadcnt >= 3) SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
	}

	//プレイヤー制御
	//プレイヤー人数分ループ
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		bool use = this[CntPlayer].GetUse();
		//生きていれば制御可能
		if (use)
		{
			//this[CntPlayer].SetMoveL2R2(CntPlayer);
			this[CntPlayer].SetMoveL(CntPlayer);
			this[CntPlayer].SetQ(CntPlayer);
			this[CntPlayer].SetCamera(CntPlayer);
			//this[CntPlayer].SetBulletALLMoveL2R2Ver(CntPlayer);
			this[CntPlayer].SetBulletALL(CntPlayer);
			this[CntPlayer].SetKiri(CntPlayer);
			this[CntPlayer].SetMorphing(CntPlayer);
		}

		//それ以外はカメラだけ制御
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

//=============================================================================
// エネミー更新処理
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
//}

//=============================================================================
// 描画処理
//=============================================================================
void PLAYER_HONTAI::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//-----------------------------------------------------親
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);

			//---------------------------------------------------------オブジェクト値呼び出し
			D3DXVECTOR3 FieldNorVec= this[CntPlayer].GetFieldNorVec();
			float PlayerUpToFieldNorVec= this[CntPlayer].GetQrot();
			D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
			D3DXMATRIX mtxWorld=this[CntPlayer].GetMatrix();
			D3DXVECTOR3 scl = this[CntPlayer].GetScl();
			D3DXVECTOR3 rot = this[CntPlayer].GetRot();
			D3DXVECTOR3 pos = this[CntPlayer].GetPos();

			//q=(rotVecAxis法線)*(g_Player.rot回転)
			D3DXQuaternionRotationAxis(&q, &FieldNorVec, -PlayerUpToFieldNorVec);
			D3DXMatrixRotationQuaternion(&mtxQ, &q);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxQ);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)this[CntPlayer].model.pD3DXBuffMat->GetBufferPointer();

			// 描画
			for (int nCntMat = 0; nCntMat < (int)this[CntPlayer].model.nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, this[CntPlayer].model.pD3DTexture);
				}

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);
				// 頂点バッファをレンダリングパイプラインに設定
				pDevice->SetStreamSource(0, this[CntPlayer].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
				// インデックスバッファをレンダリングパイプラインに設定
				pDevice->SetIndices(this[CntPlayer].model.pD3DIdxBuff);
				//描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this[CntPlayer].model.nNumVertex, 0, this[CntPlayer].model.nNumPolygon);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
		//-----------------------------------------------------子
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			//---------------------------------------------------------オブジェクト値呼び出し
			D3DXMATRIX mtxWorld = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetMatrix();
			D3DXVECTOR3 scl = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetScl();
			D3DXVECTOR3 rot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
			D3DXVECTOR3 pos = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetPos();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			if (this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai != NULL)
			{
				//-------------------------------------------------親のワールドマトリクスを取得
				D3DXMATRIX ParentmtxWorld = this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai->GetMatrix();
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &ParentmtxWorld);
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DTexture);
				}
				// 描画
				{
					// 頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_3D);
					// 頂点バッファをレンダリングパイプラインに設定
					pDevice->SetStreamSource(0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// インデックスバッファをレンダリングパイプラインに設定
					pDevice->SetIndices(this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.pD3DIdxBuff);
					//描画
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumVertex, 0, this[CntPlayer].parts[PARTSTYPE_HOUTOU].model.nNumPolygon);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
		//-----------------------------------------------------孫
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			//---------------------------------------------------------オブジェクト値呼び出し
			D3DXMATRIX mtxWorld = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetMatrix();
			D3DXVECTOR3 scl = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetScl();
			D3DXVECTOR3 rot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();
			D3DXVECTOR3 pos = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetPos();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);



			if (this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts != NULL)
			{
				//-------------------------------------------------親のワールドマトリクスを取得
				D3DXMATRIX ParentmtxWorld = this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentHontai->GetMatrix();
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &ParentmtxWorld);
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pD3DXMat = (D3DXMATERIAL*)this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DXBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DTexture);
				}
				// 描画
				{
					// 頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_3D);
					// 頂点バッファをレンダリングパイプラインに設定
					pDevice->SetStreamSource(0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DVtxBuff, 0, sizeof(VERTEX_3D));
					// インデックスバッファをレンダリングパイプラインに設定
					pDevice->SetIndices(this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.pD3DIdxBuff);
					//描画
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumVertex, 0, this[CntPlayer].parts[PARTSTYPE_HOUSIN].model.nNumPolygon);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 先頭アドレス取得
//=============================================================================
PLAYER_HONTAI* PLAYER_HONTAI::GetpPlayer(void)
{
	return &this[0];
}

//=============================================================================
// メッシュカラーをセット
//=============================================================================
void PLAYER_HONTAI::SetPlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff, DWORD nNumPolygon, int CntPlayer)
{

	VERTEX_3D *pVtx;
	WORD *pIdx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// 反射光の設定
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[CntPlayer];
	}
	// 頂点データをアンロックする
	pD3DVtxBuff->Unlock();
	pD3DIdxBuff->Unlock();
}

//=============================================================================
// 移動制御(ABボタンLスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetMoveABL(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
	D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

	//座標を保存
	this[CntPlayer].SetOldPos(pos);

	int dir;
	//移動処理
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
	// 無移動時は移動量に慣性をかける
	else
	{
		this[CntPlayer].speed *= MOVE_INERTIA_MOMENT;
	}

	//視点変化のアナログ値を旋回に代入してアナログ操作で旋回
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//旋回
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

	//角度の制限値
	if (HoudaiRot.y >= D3DX_PI*2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// 移動速度の制限
	if (this[CntPlayer].speed >= VALUE_MOVE_MAX) this[CntPlayer].speed = VALUE_MOVE_MAX;
	else if (this[CntPlayer].speed <= -VALUE_MOVE_MAX) this[CntPlayer].speed = -VALUE_MOVE_MAX;

	// プレイヤーの座標を更新
	pos.x -= sinf(HoudaiRot.y) * (this[CntPlayer].speed * this[CntPlayer].speedbuff);
	pos.z -= cosf(HoudaiRot.y) * (this[CntPlayer].speed * this[CntPlayer].speedbuff);

	//スピードバフ時間減少
	if (this[CntPlayer].speedbuffsignal == true)
	{
		this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this[CntPlayer].speedbufftime <= 0.0f)
		{
			this[CntPlayer].speedbuffsignal = false;
			this[CntPlayer].speedbuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//砲塔操作　バレット着弾点(左右エイム)
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

	//砲身操作　バレット着弾点(前後エイム)
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

	//---------------------------------------------------------オブジェクト値セット
	this[CntPlayer].SetPos(pos);
	this[CntPlayer].SetRot(HoudaiRot);
	this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetRot(HoutouRot);
	this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetRot(HousinRot);

}

//=============================================================================
// カメラ制御(ABボタンLスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetCamera(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
	D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

	CAMERA *cam = GetCamera();
	//バックカメラ処理
	//バックカメラオン　カメラ視点、注視点
	//Yボタンを押しているもしくは、バックカメラアイテムがONになっているときはカメラ反転
	if (IsButtonPressed(CntPlayer, BUTTON_Y) || this[CntPlayer].BackCameraItemSignal == true)
	{
		cam[CntPlayer].at.x = pos.x + (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z + (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x - sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z - cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//バックカメラオフ　カメラ視点、注視点
	//それ以外は通常カメラ
	else
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//もし、バックカメラアイテムがONの時にYを押すと通常カメラになる
	if (this[CntPlayer].BackCameraItemSignal == true && IsButtonPressed(CntPlayer, BUTTON_Y))
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//バックカメラの時間処理
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

//=============================================================================
// 移動制御(Lスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetMoveL(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 move = this[CntPlayer].GetMove();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();

	//座標を保存
	this[CntPlayer].SetOldPos(pos);

	int dir;

	//移動変化はLスティックアナログ値を使用
	float LAnalogX = 0.0f;		//縦入力
	float LAnalogY = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (this[CntPlayer].speedbuffsignal == true)
	{
		//スピードバフ時間減少
		this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
		this[CntPlayer].dashFlag = true;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = pos;
		SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

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

	//移動処理
	if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_UP) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(CntPlayer);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//旋回入力は後退中に限りリバースする
	if (IsButtonPressed(CntPlayer, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		this[CntPlayer].dashFlag = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//移動量を反映
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	this[CntPlayer].SetPos(pos);
	this[CntPlayer].SetRot(HoudaiRot);

	SetCameraR(CntPlayer);
}

//=============================================================================
// カメラ制御(Rスティックで移動制御)
//=============================================================================
void PLAYER_HONTAI::SetCameraR(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

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
	D3DXVECTOR3 moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moverot.y = RAnalogX;
	moverot.x = -RAnalogY;


	HoutouRot.y += moverot.y;
	HousinRot.x += moverot.x;

	//角度の制限値
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

	//---------------------------------------------------------オブジェクト値セット
	this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetRot(HoutouRot);
	this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetRot(HousinRot);


	////カメラの視点更新
	//cam[CntPlayer].pos.x = this[CntPlayer].pos.x - (POS_W_CAM * cosf(-this[CntPlayer].rot.y));
	//cam[CntPlayer].pos.y = this[CntPlayer].pos.y + (POS_H_CAM);
	//cam[CntPlayer].pos.z = this[CntPlayer].pos.z - (POS_W_CAM * sinf(-this[CntPlayer].rot.y));
	////カメラの注視点を更新
	////テクニック　カメラ注視点制御で回転軸atrotXを上げまくっても、それだけでは真上を向かない。
	////atrotXの増減に比例してatXZ値も減少させていかないと真下真上を見るような注視点を制御できない
	////対策はまだ思いついてない。atrotXの値が大きいほどrotYの影響を小さくする。
	//cam[CntPlayer].at.x = cam[CntPlayer].pos.x + (POS_W_CAM * sinf(this[CntPlayer].rot.y));
	//cam[CntPlayer].at.y = cam[CntPlayer].pos.y - (POS_H_CAM * sinf(this[CntPlayer].atrot.x));
	//cam[CntPlayer].at.z = cam[CntPlayer].pos.z + (POS_W_CAM * cosf(this[CntPlayer].rot.y));
}

//=============================================================================
// 移動制御(LRスティックでキャタピラ移動制御)
//=============================================================================
void PLAYER_HONTAI::SetMoveL2R2(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 move = this[CntPlayer].GetMove();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();

	//座標を保存
	this[CntPlayer].SetOldPos(pos);

	//移動変化はLスティックアナログ値を使用
	float L2 = 0.0f;		//縦入力
	float R2 = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (this[CntPlayer].speedbuffsignal == true)
	{
		//スピードバフ時間減少
		this[CntPlayer].speedbufftime -= VALUE_SPEEDBUFF_SUB;
		this[CntPlayer].dashFlag = true;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = pos;
		SetEffect(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

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

	//左右のキャタピラを前進交代どちらかのフラグ
	//両キャタピラ前進方向=0、左キャタピラのみ前進=3.925f、右キャタピラのみ前進=-3.925f、左キャタピラのみ後退=-0.785f、右キャタピラのみ後退=0.785f、
	float Lrot = 3.925f;
	float Rrot = -3.925f;
	int Ldir = 1;
	int Rdir = 1;
	//移動処理
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

	//両キャタピラ使用
	if (IsButtonPressed(CntPlayer, BUTTON_L2) && IsButtonPressed(CntPlayer, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
		//入力中央値32767　R2最小0　L2最大64000
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		R2 = L2;
		
	}
	//左キャタピラのみ使用
	else if (IsButtonPressed(CntPlayer, BUTTON_L2))
	{
		DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		HoudaiRot.y += 0.1f*Ldir;
	}
	//右キャタピラのみ使用
	else if (IsButtonPressed(CntPlayer, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
		float IZbuf = Button->lZ * PLAYER_MOVE_RATE_LR2;
		R2 = IZbuf;
		R2 = 32767 * PLAYER_MOVE_RATE_LR2 + (-R2)+ 32767 * PLAYER_MOVE_RATE_LR2;
		HoudaiRot.y -= 0.1f*Rdir;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		this[CntPlayer].dashFlag = false;
	}

	//移動量を反映
	move.x = R2 * sinf(HoudaiRot.y + Rrot) + L2 * sinf(HoudaiRot.y + Lrot);
	move.z = R2 * cosf(HoudaiRot.y + Rrot) + L2 * cosf(HoudaiRot.y + Lrot);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	this[CntPlayer].SetPos(pos);
	this[CntPlayer].SetRot(HoudaiRot);

	SetCameraR(CntPlayer);
}

//=============================================================================
// クォータニオン制御
//=============================================================================
void PLAYER_HONTAI::SetQ(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 FieldNorVec = this[CntPlayer].GetFieldNorVec();
	D3DXVECTOR3 FieldNorUpNorCross = this[CntPlayer].GetFieldNorUpNorCross();
	float Qrot = this[CntPlayer].GetQrot();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
	D3DXVec3Cross(&FieldNorVec, &FieldNorUpNorCross, &UpVec);
	float Upkakezan = D3DXVec3Dot(&FieldNorUpNorCross, &UpVec);
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(FieldNorUpNorCross.x*FieldNorUpNorCross.x +
				FieldNorUpNorCross.y *FieldNorUpNorCross.y +
				FieldNorUpNorCross.z * FieldNorUpNorCross.z);
		Qrot = acosf(cossita);
	}
	else Qrot = 0.0f;

	//---------------------------------------------------------オブジェクト値セット
	this[CntPlayer].SetQrot(Qrot);
	this[CntPlayer].SetFieldNorVec(FieldNorVec);
//	this[CntPlayer].SetFieldNorUpNorCross(FieldNorUpNorCross);

}

//=============================================================================
// バレット関連制御
//=============================================================================
void PLAYER_HONTAI::SetBulletALL(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
	D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

//	D3DXVECTOR3 FieldNorVec = this[CntPlayer].GetFieldNorVec();
	D3DXVECTOR3 FieldNorUpNorCross = this[CntPlayer].GetFieldNorUpNorCross();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
	D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FieldNorUpNorCross, &Frontvec);
	float Bkakezan = D3DXVec3Dot(&FieldNorUpNorCross, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(FieldNorUpNorCross.x*FieldNorUpNorCross.x +
				FieldNorUpNorCross.y *FieldNorUpNorCross.y +
				FieldNorUpNorCross.z * FieldNorUpNorCross.z)
			*
			sqrtf(Frontvec.x*Frontvec.x +
				Frontvec.y *Frontvec.y +
				Frontvec.z * Frontvec.z);
		this[CntPlayer].Brot = acosf(cossita);
	}
	else
	{
		this[CntPlayer].Brot = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	this[CntPlayer].Brot -= 1.57f;


	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
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

	// 弾発射
	if (this[CntPlayer].AmmoCnt > 0)
	{
		//if (IsButtonTriggered(CntPlayer, BUTTON_X))
		//{
		if (IsButtonTriggered(CntPlayer, BUTTON_R1))
		{

			SetBullet(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);

			//拡散弾処理
			if (this[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
			{
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				SetBullet(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);
				SetBullet(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);

			}
			//残弾を減らす
			this[CntPlayer].AmmoCnt -= 1;
//			ChangeBulletTex(-1, CntPlayer);

			// SE再生
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}


	//残弾復活 一定時間経過で1個づつ自動回復
	if (this[CntPlayer].AmmoCnt < PLAYER_AMMOPOWER_NORMAL) this[CntPlayer].AmmoBornTime += BORN_AMMO_ADDTIME;
	if (this[CntPlayer].AmmoBornTime >= BORN_AMMO_MAXTIME)
	{
		this[CntPlayer].AmmoCnt++;
		//ChangeBulletTex(1, CntPlayer);
		this[CntPlayer].AmmoBornTime = 0.0f;
	}

}

//=============================================================================
// バレット関連制御
//=============================================================================
void PLAYER_HONTAI::SetBulletALLMoveL2R2Ver(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	D3DXVECTOR3 HoudaiRot = this[CntPlayer].GetRot();
	D3DXVECTOR3 HoutouRot = this[CntPlayer].parts[PARTSTYPE_HOUTOU].GetRot();
	D3DXVECTOR3 HousinRot = this[CntPlayer].parts[PARTSTYPE_HOUSIN].GetRot();

	//	D3DXVECTOR3 FieldNorVec = this[CntPlayer].GetFieldNorVec();
	D3DXVECTOR3 FieldNorUpNorCross = this[CntPlayer].GetFieldNorUpNorCross();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
	D3DXVec3Cross(&this[CntPlayer].FrontRotTOaxis, &FieldNorUpNorCross, &Frontvec);
	float Bkakezan = D3DXVec3Dot(&FieldNorUpNorCross, &Frontvec);
	if (Bkakezan != 0)
	{
		float cossita = Bkakezan /
			sqrtf(FieldNorUpNorCross.x*FieldNorUpNorCross.x +
				FieldNorUpNorCross.y *FieldNorUpNorCross.y +
				FieldNorUpNorCross.z * FieldNorUpNorCross.z)
			*
			sqrtf(Frontvec.x*Frontvec.x +
				Frontvec.y *Frontvec.y +
				Frontvec.z * Frontvec.z);
		this[CntPlayer].Brot = acosf(cossita);
	}
	else
	{
		this[CntPlayer].Brot = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	this[CntPlayer].Brot -= 1.57f;


	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
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

	// 弾発射
	if (this[CntPlayer].AmmoCnt > 0)
	{
		//if (IsButtonTriggered(CntPlayer, BUTTON_X))
		//{
		if (IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_UP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTUP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHT) ||
			IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTDOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_DOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTDOWN) ||
			IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFT) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTUP))
		{

			SetBullet(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);

			//拡散弾処理
			if (this[CntPlayer].ModelType == PLAYER_MODEL_ATTACK)
			{
				D3DXVECTOR3 leftB, rightB;
				leftB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y + 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y + 0.3f) *VALUE_MOVE_BULLET);
				rightB = D3DXVECTOR3(-sinf(HoutouRot.y + HoudaiRot.y - 0.3f)*VALUE_MOVE_BULLET,
					bulletmove.y,
					-cosf(HoutouRot.y + HoudaiRot.y - 0.3f) *VALUE_MOVE_BULLET);
				SetBullet(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);
				SetBullet(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, CntPlayer);

			}
			//残弾を減らす
			this[CntPlayer].AmmoCnt -= 1;
			//ChangeBulletTex(-1, CntPlayer);

			// SE再生
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}


	//残弾復活 一定時間経過で1個づつ自動回復
	if (this[CntPlayer].AmmoCnt < PLAYER_AMMOPOWER_NORMAL) this[CntPlayer].AmmoBornTime += BORN_AMMO_ADDTIME;
	if (this[CntPlayer].AmmoBornTime >= BORN_AMMO_MAXTIME)
	{
		this[CntPlayer].AmmoCnt++;
		//ChangeBulletTex(1, CntPlayer);
		this[CntPlayer].AmmoBornTime = 0.0f;
	}

}

//=============================================================================
// フォグ制御
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
// モーフィング制御
//=============================================================================
void PLAYER_HONTAI::SetMorphing(int CntPlayer)
{
	// モーフィングtrue
	if (this[CntPlayer].Morphing == true)
	{
		///////////////////////////////////////////////////////////////////////バレット3つ時間開始
		// モーフィング時間減算開始
		this[CntPlayer].MorphingTime -= 1.0f;

		// モーフィング攻撃タイプに変更開始
		if (this[CntPlayer].MorphingSignal == NowMorphing)
		{
			this[CntPlayer].ModelType = PLAYER_MODEL_ATTACK;
			DoMorphing(&this[CntPlayer].parts[PARTSTYPE_HOUSIN].model, &this[CntPlayer].ModelDate[PLAYER_MODEL_ATTACK],
				0.01f, &this[CntPlayer].time, &this[CntPlayer].MorphingSignal);
		}
		///////////////////////////////////////////////////////////////////////バレット3つ時間終了

		// 時間経過でモデルを元に戻す
		else if (this[CntPlayer].MorphingTime <= 0.0f)
		{
			this[CntPlayer].ModelType = PLAYER_MODEL_NORMAL;
			this[CntPlayer].MorphingSignal = NowMorphing;
			this[CntPlayer].Morphing = false;
		}
	}

	// モーフィングオリジナルタイプに変更開始
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

