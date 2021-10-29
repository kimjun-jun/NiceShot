/**
* @file player.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
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

//画像の参照名																											
#define	TEXTURE_MEISAI				"../data/MODEL/28513607_p4_master1200.jpg"		//!< 読み込むテクスチャファイル名

#define BULLET_BORN_TIME_ADD		(1.0f)				//!< 弾薬復活させるための加算タイム
#define BULLET_BORN_TIME_MAX		(120.0f)			//!< 1弾薬復活するのに必要なタイム

#define	EFFECT_SPEEDUP_SIZE_X		(25.0f)			//!< エフェクトスピードアップの幅
#define	EFFECT_SPEEDUP_SIZE_Y		(10.0f)			//!< エフェクトスピードアップの高さ
#define	EFFECT_SPEEDUP_TIME			(10)			//!< エフェクトスピードアップの生存時間

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4カラー
};

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
PLAYER::PLAYER(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_MEISAI);

	//カウントループ　プレイヤー数
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//カウントループ　描画用パーツの数
		for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_PARTS_TYPE_MAX; CntDrawPartsNum++)
		{
			// Xファイルの読み込みデータ群
			LPD3DXMESH	Mesh;
			DWORD nv, nvi, nm, np;
			LPD3DXBUFFER BuffMat;

			// Xファイルの読み込み
			LoadMesh(this->c_aFileNameModelDraw[CntDrawPartsNum], &BuffMat,
				&nm, &Mesh, &nv, &np, &nvi, NULL);

			//頂点の作成
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].MakeVertex3D(nv, FVF_VERTEX_3D);
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].MakeIdxVertex(nvi);

			//バッファの取得から反映
			LPDIRECT3DVERTEXBUFFER9 *VtxBuff;
			LPDIRECT3DINDEXBUFFER9	*IdxBuff;
			VtxBuff = this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pVtxBuff();
			IdxBuff = this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pIdxBuff();
			Mesh->GetVertexBuffer(VtxBuff);
			Mesh->GetIndexBuffer(IdxBuff);

			//頂点カラーをプレイヤー色に変更
			this->PlayerMeshColor(this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pVtxBuff(), this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].pIdxBuff(), np, CntPlayer);

			//データ反映
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumMat(nm);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].Mat(BuffMat);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumVertex(nv);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumPolygon(np);
			this->modelDraw[CntPlayer].ModelAttribute[CntDrawPartsNum].NumVertexIndex(nvi);
		}


		//使用の設定
		this->iUseType[CntPlayer].Use(YesUseType1);
	}

	//カウントループ　オリジナル用モデルの数(モーフィング基準データ)
	for (int CntOriginalModelNum = 0; CntOriginalModelNum < PLAYER_MODEL_ORIGINAL_TYPE_MAX; CntOriginalModelNum++)
	{
		// Xファイルの読み込みデータ群
		LPD3DXMESH	Mesh;
		DWORD nv, nvi, nm, np;
		LPD3DXBUFFER BuffMat;

		// Xファイルの読み込み
		LoadMesh(this->c_aFileNameModelOriginal[CntOriginalModelNum], &BuffMat,
			&nm, &Mesh, &nv, &np, &nvi, NULL);

		//頂点の作成
		this->modelOri.Vtx[CntOriginalModelNum].MakeVertex3D(nv, FVF_VERTEX_3D);
		this->modelOri.Vtx[CntOriginalModelNum].MakeIdxVertex(nvi);

		//バッファの取得から反映
		LPDIRECT3DVERTEXBUFFER9 *VtxBuff;
		LPDIRECT3DINDEXBUFFER9	*IdxBuff;
		VtxBuff = this->modelOri.Vtx[CntOriginalModelNum].pVtxBuff();
		IdxBuff = this->modelOri.Vtx[CntOriginalModelNum].pIdxBuff();
		Mesh->GetVertexBuffer(VtxBuff);
		Mesh->GetIndexBuffer(IdxBuff);

		//モデルデータ反映
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumMat(nm);
		this->modelOri.ModelAttribute[CntOriginalModelNum].Mat(BuffMat);
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumVertex(nv);
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumPolygon(np);
		this->modelOri.ModelAttribute[CntOriginalModelNum].NumVertexIndex(nvi);
	}

	//親子階層アドレスを設定
	//this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai = &this[CntPlayer];
	//this[CntPlayer].Parent = NULL;
	//this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai = &this[CntPlayer];
	//this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts = &this[CntPlayer].parts[PARTSTYPE_HOUTOU];

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
PLAYER::~PLAYER(void)
{
	//テクスチャ解放
	this->tex.~TEXTURE();

	//カウントループ　プレイヤー数
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//カウントループ　描画用モデルの数
		for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_PARTS_TYPE_MAX; CntDrawPartsNum++)
		{
			//モデル解放
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].~VTXBuffer();
		}
	}

	//カウントループ　オリジナル用モデルの数
	for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_MODEL_ORIGINAL_TYPE_MAX; CntDrawPartsNum++)
	{
		//モデル解放
		this->modelOri.Vtx[CntDrawPartsNum].~VTXBuffer();
	}

	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void PLAYER::Init(FIELD *field)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//PLAYER 初期化　各初期化を関数化
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		this->iUseType[CntPlayer].Use(YesUseType1);

		//標準パラメータ
		this->PlayerPara[CntPlayer].StandardPara.Vital = this->PlayerPara[CntPlayer].StandardPara.OldVital = PLAYER_VITAL_MAX;
		this->PlayerPara[CntPlayer].StandardPara.Speed = 0.0f;
		this->PlayerPara[CntPlayer].StandardPara.eModelType = this->PlayerPara[CntPlayer].StandardPara.eOldModelType= PLAYER_MODEL_TYPE_NORMAL;
		this->PlayerPara[CntPlayer].StandardPara.FUPFCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//バレットパラメータ
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->PlayerPara[CntPlayer].BulletPara.BulletStock = BULLET_MAX_STOCK;
		this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
		for (int i = 0; i < 3; i++) this->PlayerPara[CntPlayer].BulletPara.BulletMove[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//アイテムパラメータ
		this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.KiriSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = 1.0f;
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime = 0.0f;
		this->PlayerPara[CntPlayer].ItemPara.KiriTime = 0.0f;
		this->PlayerPara[CntPlayer].ItemPara.BackCameraTime = 0.0f;

		//モーフィングパラメータ
		this->PlayerPara[CntPlayer].MorphingPara.MorphingDTtime = 0.0f;
		//this->PlayerPara[CntPlayer].MorphingPara.MorphingEnd=true
		this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NoMorphing;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingStart = false;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
		this->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = false;

		//その他パラメータ
		this->PostureVec[CntPlayer].FNVecFunc(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->PostureVec[CntPlayer].FNUNCrossFunc(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this->PostureVec[CntPlayer].QrotFunc(0.0f);
		this->Move[CntPlayer].Move(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//砲塔
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//砲身								 
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//モデルのリセット
		ResetModel(
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(),
			this->modelOri.Vtx[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN].VtxBuff(),
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff(),
			&this->modelOri.ModelAttribute[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN]);
	}

	//初期化段階で座標と角度をランダムで設定
	this->modelDraw[PLAYER01].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this->modelDraw[PLAYER02].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, PLAYER_INIT_POSZ + rand() % 200));
	this->modelDraw[PLAYER03].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));
	this->modelDraw[PLAYER04].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(D3DXVECTOR3(-PLAYER_INIT_POSX + rand() % 200, PLAYER_INIT_POSY, -PLAYER_INIT_POSZ + rand() % 200));

	this->modelDraw[PLAYER01].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this->modelDraw[PLAYER02].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this->modelDraw[PLAYER03].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));
	this->modelDraw[PLAYER04].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(D3DXVECTOR3(0.0f, float(rand() % 6), 0.0f));

	//初期化後の状態で一度カメラ、姿勢の処理をする
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//レイキャスト処理準備
		D3DXVECTOR3 RayStart = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
		RayStart.y += 10.0f;
		D3DXVECTOR3 RayEnd = RayStart;
		RayEnd.y -= 1000.0f;

		float ReturnPosY = 0.0f;
		D3DXVECTOR3 FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//レイキャスト関数
		field->FieldHitGetSphereVec(RayStart, RayEnd, &FieldNorVec, &ReturnPosY);

		//レイキャスト結果を反映
		D3DXVECTOR3 Pos = RayStart;
		Pos.y = ReturnPosY;
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(Pos);
		this->PostureVec[CntPlayer].FNVecFunc(FieldNorVec);
		this->Quaternion(CntPlayer);
		this->CameraRevers(CntPlayer, false);

		//カメラ初期調整
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
// プレイヤー更新処理
//=============================================================================
void PLAYER::Update(EFFECT*effect, BULLET*bullet, SHADOW*shadow, FADE *fade, bool Netflag, int MyNumber)
{
	//何人死んだか計算。三人死んだらゲーム終了。次のシーンへ
	int deadcnt = 0;
	//プレイヤー人数分ループ
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

	//プレイヤー制御
	//プレイヤー人数分ループ ローカル対戦
	if (Netflag == false)
	{
		this->MoveKeybord(0, &effect[0], Netflag);
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			bool use = this->iUseType[CntPlayer].Use();
			//生きていれば制御可能
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

			//それ以外はカメラだけ制御
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

	//ネット対戦
	else
	{
		this->MoveKeybord(0, &effect[0], Netflag);
		bool use = this->iUseType[MyNumber].Use();

		//生きていれば操作可能
		if (use)
		{
			//ネット用フラグ初期化　1フレーム中の変更を保存して変更があればサーバーにデータを送る
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

		//それ以外はカメラだけ制御
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
// エネミー更新処理
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
	//		PositionShadow(g_EnemyHoudai[CntEnemy].parameter.shadowIdx,
	//			D3DXVECTOR3(g_EnemyHoudai[CntEnemy].parameter.pos.x, 0.0f, g_EnemyHoudai[CntEnemy].parameter.pos.z), g_EnemyHoudai[CntEnemy].parameter.scl);
	//	}
	//}
//}

//=============================================================================
// 描画処理
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
			//-----------------------------------------------------親
			{
				D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
				D3DXMATERIAL *pD3DXMat;
				D3DMATERIAL9 matDef;
				D3DXMATRIX mtxQ;
				D3DXMatrixIdentity(&mtxQ);

				//---------------------------------------------------------オブジェクト値呼び出し
				D3DXVECTOR3 UpNorFieldNorVec = this->PostureVec[CntPlayer].FNUNCrossFunc();
				float Qrot = this->PostureVec[CntPlayer].QrotFunc();
				D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();

				//q=(rotVecAxis法線)*(g_Player.rot回転)
				D3DXQuaternionRotationAxis(&q, &UpNorFieldNorVec, -Qrot);
				D3DXMatrixRotationQuaternion(&mtxQ, &q);

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorldHoudai);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxRot);
				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxQ);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldHoudai, &mtxWorldHoudai, &mtxTranslate);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldHoudai);
				//this[CntPlayer].Matrix(mtxWorldHoudai);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				// 描画
				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].NumMat(); nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

					if (pD3DXMat[nCntMat].pTextureFilename != NULL)
					{
						// テクスチャの設定
						pDevice->SetTexture(0, this->tex.Texture());
					}

					// 頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_3D);
					// 頂点バッファをレンダリングパイプラインに設定
					pDevice->SetStreamSource(0, this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUDAI].VtxBuff(), 0, sizeof(VERTEX_3D));
					// インデックスバッファをレンダリングパイプラインに設定
					pDevice->SetIndices(this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUDAI].IdxBuff());
					//描画
					pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
						this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].NumVertex(), 0,
						this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUDAI].NumPolygon());
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
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Pos();

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorldHoutou);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &mtxTranslate);

				//if (this[CntPlayer].parts[PARTSTYPE_HOUTOU].ParentHontai != NULL)
				{
					//-------------------------------------------------親のワールドマトリクスを取得
					D3DXMATRIX ParentmtxWorldHoutou = mtxWorldHoudai;
					D3DXMatrixMultiply(&mtxWorldHoutou, &mtxWorldHoutou, &ParentmtxWorldHoutou);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldHoutou);
				//this[CntPlayer].parts[PARTSTYPE_HOUTOU].SetMatrix(mtxWorldHoutou);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].NumMat(); nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
					if (pD3DXMat[nCntMat].pTextureFilename != NULL)
					{
						// テクスチャの設定
						pDevice->SetTexture(0, this->tex.Texture());
					}
					// 描画
					{
						// 頂点フォーマットの設定
						pDevice->SetFVF(FVF_VERTEX_3D);
						// 頂点バッファをレンダリングパイプラインに設定
						pDevice->SetStreamSource(0, this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUTOU].VtxBuff(), 0, sizeof(VERTEX_3D));
						// インデックスバッファをレンダリングパイプラインに設定
						pDevice->SetIndices(this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUTOU].IdxBuff());
						//描画
						pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].NumVertex(), 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUTOU].NumPolygon());
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
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Pos();

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorldHousin);

				// スケールを反映
				D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
				D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &mtxScl);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
				D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &mtxRot);

				// 移動を反映
				D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &mtxTranslate);



				//if (this[CntPlayer].parts[PARTSTYPE_HOUSIN].ParentParts != NULL)
				{
					//-------------------------------------------------親のワールドマトリクスを取得
					D3DXMATRIX ParentmtxWorldHousin = mtxWorldHoutou;
					D3DXMatrixMultiply(&mtxWorldHousin, &mtxWorldHousin, &ParentmtxWorldHousin);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxWorldHousin);
				//this[CntPlayer].parts[PARTSTYPE_HOUSIN].SetMatrix(mtxWorldHousin);
				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].NumMat(); nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
					if (pD3DXMat[nCntMat].pTextureFilename != NULL)
					{
						// テクスチャの設定
						pDevice->SetTexture(0, this->tex.Texture());
					}
					// 描画
					{
						// 頂点フォーマットの設定
						pDevice->SetFVF(FVF_VERTEX_3D);
						// 頂点バッファをレンダリングパイプラインに設定
						pDevice->SetStreamSource(0, this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(), 0, sizeof(VERTEX_3D));
						// インデックスバッファをレンダリングパイプラインに設定
						pDevice->SetIndices(this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff());
						//描画
						pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].NumVertex(), 0,
							this->modelDraw[CntPlayer].ModelAttribute[PLAYER_PARTS_TYPE_HOUSIN].NumPolygon());
					}
				}
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//=============================================================================
// メッシュカラーをセット
//=============================================================================
void PLAYER::PlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff, DWORD nNumPolygon, int CntPlayer)
{
	VERTEX_3D *pVtx;
	WORD *pIdx;

	LPDIRECT3DVERTEXBUFFER9 vtx = *pD3DVtxBuff;
	LPDIRECT3DINDEXBUFFER9 idx = *pD3DIdxBuff;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	vtx->Lock(0, 0, (void**)&pVtx, 0);
	idx->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntPoly = 0; nCntPoly < int(nNumPolygon); nCntPoly++, pIdx += 3)
	{
		// 反射光の設定
		pVtx[pIdx[0]].diffuse =
			pVtx[pIdx[1]].diffuse =
			pVtx[pIdx[2]].diffuse = PLAYER_COLOR[CntPlayer];
	}
	// 頂点データをアンロックする
	vtx->Unlock();
	idx->Unlock();
}

//=============================================================================
// 移動制御(ABボタンLスティックで移動制御)
//=============================================================================
void PLAYER::MoveABL(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);

	int dir = FRONT_VEC;
	//移動処理
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

	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//視点変化のアナログ値を旋回に代入してアナログ操作で旋回
	DIJOYSTATE2 *Button = GetIsButton(CntPlayer);
	float LAnalogX = float(Button->lX / 30000.0f);

	//旋回
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

	//角度の制限値
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// 移動速度の制限
	if (this->PlayerPara[CntPlayer].StandardPara.Speed >= VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = VALUE_MOVE_MAX;
	else if (this->PlayerPara[CntPlayer].StandardPara.Speed <= -VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = -VALUE_MOVE_MAX;

	// プレイヤーの座標を更新
	pos.x -= sinf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//スピードバフ時間減少
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		effect->SetInstance(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
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
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

}

//=============================================================================
// 移動制御(ki-bo-doで移動制御)
//=============================================================================
void PLAYER::MoveKeybord(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);

	int dir = FRONT_VEC;
	//移動処理
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

	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].StandardPara.Speed *= MOVE_INERTIA_MOMENT;
	}

	//旋回
	if (GetKeyboardPress(DIK_D))
	{
		HoudaiRot.y += 0.02f * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff*2;
	}
	else if (GetKeyboardPress(DIK_A))
	{
		HoudaiRot.y -= 0.02f * dir*this->PlayerPara[CntPlayer].ItemPara.SpeedBuff*2;
	}


	//角度の制限値
	if (HoudaiRot.y >= D3DX_PI * 2) HoudaiRot.y = 0.0f;
	else if (HoudaiRot.y <= -D3DX_PI * 2) HoudaiRot.y = 0.0f;

	// 移動速度の制限
	if (this->PlayerPara[CntPlayer].StandardPara.Speed >= VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = VALUE_MOVE_MAX;
	else if (this->PlayerPara[CntPlayer].StandardPara.Speed <= -VALUE_MOVE_MAX) this->PlayerPara[CntPlayer].StandardPara.Speed = -VALUE_MOVE_MAX;

	// プレイヤーの座標を更新
	pos.x -= sinf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);
	pos.z -= cosf(HoudaiRot.y) * (this->PlayerPara[CntPlayer].StandardPara.Speed * this->PlayerPara[CntPlayer].ItemPara.SpeedBuff);

	//スピードバフ時間減少
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;

		// エフェクトスピードアップの生成
		D3DXVECTOR3 EffctSpeedupPos = D3DXVECTOR3(pos.x, pos.y, pos.z);
		effect->SetInstance(EffctSpeedupPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_COLOR[CntPlayer], EFFECT_SPEEDUP_SIZE_X, EFFECT_SPEEDUP_SIZE_Y, EFFECT_SPEEDUP_TIME);

		if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal = false;
			this->PlayerPara[CntPlayer].ItemPara.SpeedBuff = VALUE_SPEEDBUFF_SUB;
		}
	}

	//砲塔操作　バレット着弾点(左右エイム)
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

	//砲身操作　バレット着弾点(前後エイム)
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

	//---------------------------------------------------------オブジェクト値セット
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

}

//=============================================================================
// 移動制御(Lスティックで移動制御)
//=============================================================================
void PLAYER::MoveL(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//移動変化はLスティックアナログ値を使用
	float LAnalogX = 0.0f;		//縦入力
	float LAnalogY = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//スピードバフ時間減少
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// エフェクトスピードアップの生成
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

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//移動処理
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//旋回入力は後退中に限りリバースする
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//移動量を反映
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}

//=============================================================================
// 移動制御(Lスティックで移動制御)
//=============================================================================
void PLAYER::MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	int dir = FRONT_VEC;

	//移動変化はLスティックアナログ値を使用
	float LAnalogX = 0.0f;		//縦入力
	float LAnalogY = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//スピードバフ時間減少
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// エフェクトスピードアップの生成
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

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//移動処理
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_L_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_L_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);

		LAnalogX = float(Button->lX * PLAYER_MOVE_RATE_X);
		LAnalogY = float(Button->lY * PLAYER_MOVE_RATE_Y * DashRate);
		dir = FRONT_VEC;
	}
	//旋回入力は後退中に限りリバースする
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_L_DOWN))
	{
		dir = BACK_VEC;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}
	if (LAnalogY > 0.0f) LAnalogX *= -1;

	//移動量を反映
	HoudaiRot.y += LAnalogX * dir;
	move.x = LAnalogY * sinf(HoudaiRot.y);
	move.z = LAnalogY * cosf(HoudaiRot.y);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}

//=============================================================================
// 移動制御(LRスティックでキャタピラ移動制御)
//=============================================================================
void PLAYER::MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 move = this->Move[CntPlayer].Move();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	//移動変化はLスティックアナログ値を使用
	float L2 = 0.0f;		//縦入力
	float R2 = 0.0f;		//横入力
	float DashRate = 1.0f;		//スピードアップレート

	//ダッシュ判定
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		//スピードバフ時間減少
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = true;

		// エフェクトスピードアップの生成
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

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//左右のキャタピラを前進交代どちらかのフラグ
	//両キャタピラ前進方向=0、左キャタピラのみ前進=3.925f、右キャタピラのみ前進=-3.925f、左キャタピラのみ後退=-0.785f、右キャタピラのみ後退=0.785f、
	float Lrot = 3.925f;
	float Rrot = -3.925f;
	int Ldir = 1;
	int Rdir = 1;
	//移動処理
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

	//両キャタピラ使用
	if (IsButtonPressed(PadNum, BUTTON_L2) && IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		//入力中央値32767　R2最小0　L2最大64000
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		R2 = L2;

	}
	//左キャタピラのみ使用
	else if (IsButtonPressed(PadNum, BUTTON_L2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		L2 = float(Button->lZ * PLAYER_MOVE_RATE_LR2);
		HoudaiRot.y += 0.1f*Ldir;
	}
	//右キャタピラのみ使用
	else if (IsButtonPressed(PadNum, BUTTON_R2))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
		float IZbuf = Button->lZ * PLAYER_MOVE_RATE_LR2;
		R2 = IZbuf;
		R2 = 32767 * PLAYER_MOVE_RATE_LR2 + (-R2) + 32767 * PLAYER_MOVE_RATE_LR2;
		HoudaiRot.y -= 0.1f*Rdir;
	}
	// 無移動時は移動量に慣性をかける
	else
	{
		this->PlayerPara[CntPlayer].ItemPara.DashSignal = false;
	}

	//移動量を反映
	move.x = R2 * sinf(HoudaiRot.y + Rrot) + L2 * sinf(HoudaiRot.y + Lrot);
	move.z = R2 * cosf(HoudaiRot.y + Rrot) + L2 * cosf(HoudaiRot.y + Lrot);

	//プレイヤー座標を更新
	pos += move;

	//---------------------------------------------------------オブジェクト値セット
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(pos);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot(HoudaiRot);

	CameraRotControl(CntPlayer, Netflag);
}

//=============================================================================
// カメラ反転制御(手動とアイテムの両方で制御)
//=============================================================================
void PLAYER::CameraRevers(int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	CAMERA *cam = GetCamera();

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//バックカメラ処理　バックカメラオン　カメラ視点、注視点、Yボタンを押しているもしくは、バックカメラアイテムがONになっているときはカメラ反転
	if (GetKeyboardPress(DIK_B) || IsButtonPressed(PadNum, BUTTON_Y) || this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal == true)
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
	if (this->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal == true && IsButtonPressed(PadNum, BUTTON_Y))
	{
		cam[CntPlayer].at.x = pos.x - (AT_W_CAM * sinf(HoudaiRot.y + HoutouRot.y));
		cam[CntPlayer].at.y = pos.y + (HousinRot.x*100.0f);
		cam[CntPlayer].at.z = pos.z - (AT_W_CAM * cosf(HoudaiRot.y + HoutouRot.y));

		cam[CntPlayer].pos.x = pos.x + sinf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
		cam[CntPlayer].pos.y = pos.y + POS_H_CAM;
		cam[CntPlayer].pos.z = pos.z + cosf(HoudaiRot.y + HoutouRot.y) * cam[CntPlayer].len;
	}
	//バックカメラの時間処理
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
// カメラ制御(RスティックでRot制御)
//=============================================================================
void PLAYER::CameraRotControl(int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	//Old保存
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);

	CAMERA *cam = GetCamera();

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	//視野角変化はRスティックアナログ値を使用
	float RAnalogX = 0.0f;		//縦入力
	float RAnalogY = 0.0f;		//横入力

	//視野角処理
	if (IsButtonPressed(PadNum, BUTTON_ANALOG_R_UP) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_DOWN) ||
		IsButtonPressed(PadNum, BUTTON_ANALOG_R_LEFT) || IsButtonPressed(PadNum, BUTTON_ANALOG_R_RIGHT))
	{
		DIJOYSTATE2 *Button = GetIsButton(PadNum);
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
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(HoutouRot);
	this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(HousinRot);


	////カメラの視点更新
	//cam[CntPlayer].pos.x = this[CntPlayer].pos.x - (POS_W_CAM * cosf(-this[CntPlayer].rot.y));
	//cam[CntPlayer].pos.y = this[CntPlayer].pos.y + (POS_H_CAM);
	//cam[CntPlayer].pos.z = this[CntPlayer].pos.z - (POS_W_CAM * sinf(-this[CntPlayer].rot.y));
	////カメラの注視点を更新
	////テクニック　カメラ注視点制御で回転軸atrotXを上げまくっても、それだけでは真上を向かない。
	////atrotXの増減に比例してatXZ値も減少させていかないと真下真上を見るような注視点を制御できない
	//cam[CntPlayer].at.x = cam[CntPlayer].pos.x + (POS_W_CAM * sinf(this[CntPlayer].rot.y));
	//cam[CntPlayer].at.y = cam[CntPlayer].pos.y - (POS_H_CAM * sinf(this[CntPlayer].atrot.x));
	//cam[CntPlayer].at.z = cam[CntPlayer].pos.z + (POS_W_CAM * cosf(this[CntPlayer].rot.y));
}

//=============================================================================
// クォータニオン制御
//=============================================================================
void PLAYER::Quaternion(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 FieldNorVec = this->PostureVec[CntPlayer].FNVecFunc();
	D3DXVECTOR3 FieldNorUpNorCross = this->PostureVec[CntPlayer].FNUNCrossFunc();
	float Qrot = this->PostureVec[CntPlayer].QrotFunc();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
	D3DXVec3Cross(&FieldNorUpNorCross, &FieldNorVec, &UpVec);

	//地形法線とUpベクトルで内積をとる(軸に対する回転角度を求める)
	float Upkakezan = D3DXVec3Dot(&FieldNorVec, &UpVec);

	//もし回転角度が0以外(垂直でない)ならば回転角度θを求める
	//θはクォータニオンの任意軸を回転させる時の回転角度になる
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(FieldNorVec.x*FieldNorVec.x +
				FieldNorVec.y *FieldNorVec.y +
				FieldNorVec.z * FieldNorVec.z);
		Qrot = acosf(cossita);
	}
	else Qrot = 0.0f;

	//---------------------------------------------------------オブジェクト値セット
	this->PostureVec[CntPlayer].QrotFunc(Qrot);
	this->PostureVec[CntPlayer].FNUNCrossFunc(FieldNorUpNorCross);

}

//=============================================================================
// バレット関連制御
//=============================================================================
void PLAYER::BulletALL(int CntPlayer, BULLET *bullet, SHADOW *shadow, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FieldNorVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
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
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	this->PlayerPara[CntPlayer].BulletPara.BulletRotY -= 1.57f;

	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
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

	//ネット対戦時はゲームパッドナンバーは0　それ以外はプレイヤー番号で対応させる
	int PadNum = CntPlayer;
	if (Netflag == true)
	{
		PadNum = 0;
	}

	// 弾発射
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock > 0)
	{
		//if (IsButtonTriggered(PadNum, BUTTON_X))
		//{
		if (IsButtonTriggered(PadNum, BUTTON_R1) || GetKeyboardTrigger(DIK_SPACE))
		{
			bullet->SetInstance(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);
			//拡散弾処理
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
			//残弾を減らす
			this->PlayerPara[CntPlayer].BulletPara.BulletStock -= 1;

			// SE再生
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}
	//残弾復活 一定時間経過で1個づつ自動回復
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock < BULLET_MAX_STOCK) this->PlayerPara[CntPlayer].BulletPara.BulletBornTime += BULLET_BORN_TIME_ADD;
	if (this->PlayerPara[CntPlayer].BulletPara.BulletBornTime >= BULLET_BORN_TIME_MAX)
	{
		this->PlayerPara[CntPlayer].BulletPara.BulletStock++;
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
	}
}

//=============================================================================
// バレット関連制御
//=============================================================================
void PLAYER::BulletALLMoveL2R2Ver(int CntPlayer, BULLET *bullet, SHADOW *shadow)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FieldNorVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
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
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	this->PlayerPara[CntPlayer].BulletPara.BulletRotY -= 1.57f;


	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
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

	// 弾発射
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock > 0)
	{
		if (IsButtonTriggered(CntPlayer, BUTTON_X))
		{
		//if (IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_UP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTUP) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHT) ||
			//IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_RIGHTDOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_DOWN) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTDOWN) ||
			//IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFT) || IsButtonTriggered(CntPlayer, BUTTON_DIGITAL_LEFTUP))
		//{
			bullet->SetInstance(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer), shadow);
			//拡散弾処理
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
			//残弾を減らす
			this->PlayerPara[CntPlayer].BulletPara.BulletStock -= 1;

			// SE再生
			PlaySound(SOUND_LABEL_SE_attack03);
		}
	}

	//残弾復活 一定時間経過で1個づつ自動回復
	if (this->PlayerPara[CntPlayer].BulletPara.BulletStock < BULLET_MAX_STOCK) this->PlayerPara[CntPlayer].BulletPara.BulletBornTime += BULLET_BORN_TIME_ADD;
	if (this->PlayerPara[CntPlayer].BulletPara.BulletBornTime >= BULLET_BORN_TIME_MAX)
	{
		this->PlayerPara[CntPlayer].BulletPara.BulletStock++;
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
	}
}

//=============================================================================
// フォグ制御
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
// モーフィング制御
//=============================================================================
void PLAYER::ItemTimeMorphing(int CntPlayer)
{
	// モーフィングtrue
	if (this->PlayerPara[CntPlayer].MorphingPara.MorphingStart == true)
	{
		///////////////////////////////////////////////////////////////////////バレット3つ時間開始
		// モーフィング時間減算開始
		this->PlayerPara[CntPlayer].MorphingPara.MorphingTime -= 1.0f;

		// モーフィング攻撃タイプに変更開始
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
		///////////////////////////////////////////////////////////////////////バレット3つ時間終了

		// 時間経過でモデルを元に戻す
		else if (this->PlayerPara[CntPlayer].MorphingPara.MorphingTime <= 0.0f)
		{
			this->PlayerPara[CntPlayer].StandardPara.eModelType = PLAYER_MODEL_TYPE_NORMAL;
			this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NowMorphing;
			this->PlayerPara[CntPlayer].MorphingPara.MorphingStart = false;
		}
	}

	// モーフィングオリジナルタイプに変更開始
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

