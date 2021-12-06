/**
* @file player.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
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

//画像の参照名																											
#define	TEXTURE_MEISAI	"../data/MODEL/28513607_p4_master1200.jpg"		//!< 読み込むテクスチャファイル名

constexpr float	PLAYER_INIT_POSX{ 700.0f };				//!< 初期座標プレイヤー
constexpr float	PLAYER_INIT_POSY{ 300.0f };				//!< 初期座標プレイヤー
constexpr float	PLAYER_INIT_POSZ{ 700.0f };				//!< 初期座標プレイヤー
constexpr float BULLET_BORN_TIME_ADD{ 1.0f };			//!< 弾薬復活させるための加算タイム
constexpr float BULLET_BORN_TIME_MAX{ 120.0f };			//!< 1弾薬復活するのに必要なタイム
constexpr float	EFFECT_SPEEDUP_SIZE_X{ 25.0f };			//!< エフェクトスピードアップの幅
constexpr float	EFFECT_SPEEDUP_SIZE_Y{ 10.0f };			//!< エフェクトスピードアップの高さ
constexpr int	EFFECT_SPEEDUP_TIME{ 10 };				//!< エフェクトスピードアップの生存時間
constexpr float	PLAYER_VALUE_DASHRATE{ 2.0f };			//!< 歩きに比べてのダッシュの速度倍率
constexpr float	PLAYER_MOVE_RATE_X{ 1.0f / 40000.0f };	//!< 左右歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
constexpr float	PLAYER_MOVE_RATE_Y{ 1.0f / 200.0f };	//!< 前後歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
constexpr float	PLAYER_MOVE_RATE_LR2{ 1.0f / 20000.0f };//!< キャタピラの移動量倍率(0～65000が入力範囲なのでそれに乗算する)
constexpr float VALUE_LEN_BULLET{ 10.0f };				//!< プレイヤー中心としたバレットの発射位置までの距離
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU{ 0.03f };	//!< プレイヤー砲塔回転量
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU_MAX{ 1.57f };//!< プレイヤー砲塔回転量最大値
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN{ 0.01f };	//!< プレイヤー砲身回転量
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN_MAX{ 0.2f };	//!< プレイヤー砲塔回転量最大値
constexpr float	MOVE_INERTIA_MOMENT{ 0.8f };			//!< 移動量の慣性モーメント、減速係数として使用
constexpr float	VALUE_SPEEDBUFF_SUB{ 1.0f };			//!< スピートアップ時間のデクリメントする値
constexpr float BACKCAMERA_TIME{ 150.0f };				//!< バックカメラアイテム有効時間
constexpr float KIRI_TIME{ 150.0f };					//!< フォグ霧アイテムの有効時間
constexpr float	PLAYER_VALUE_MOVE{ 0.1f };				//!< 移動加速度
constexpr float	PLAYER_VALUE_MOVE_MAX{ 4.0f };			//!< 移動加速度の最大値

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
PLAYER::PLAYER(void)
{
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
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumMat(nm);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].Mat(BuffMat);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumVertex(nv);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumPolygon(np);
			this->modelDraw[CntPlayer].Attribute[CntDrawPartsNum].NumVertexIndex(nvi);
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
		this->modelOri.Attribute[CntOriginalModelNum].NumMat(nm);
		this->modelOri.Attribute[CntOriginalModelNum].Mat(BuffMat);
		this->modelOri.Attribute[CntOriginalModelNum].NumVertex(nv);
		this->modelOri.Attribute[CntOriginalModelNum].NumPolygon(np);
		this->modelOri.Attribute[CntOriginalModelNum].NumVertexIndex(nvi);
	}
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
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].~VTXBUFFER();
		}
	}
	//カウントループ　オリジナル用モデルの数
	for (int CntDrawPartsNum = 0; CntDrawPartsNum < PLAYER_MODEL_ORIGINAL_TYPE_MAX; CntDrawPartsNum++)
	{
		//モデル解放
		this->modelOri.Vtx[CntDrawPartsNum].~VTXBUFFER();
	}
}

//=============================================================================
// 他クラスのアドレス取得
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
// 初期化処理
//=============================================================================
void PLAYER::Init(void)
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
		this->PlayerPara[CntPlayer].StandardPara.FUPFCross = VEC3_ALL0;

		//バレットパラメータ
		this->PlayerPara[CntPlayer].BulletPara.BulletBornTime = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[1] = VEC3_ALL0;
		this->PlayerPara[CntPlayer].BulletPara.BulletMove[2] = VEC3_ALL0;
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 0.0f;
		this->PlayerPara[CntPlayer].BulletPara.BulletStartPos = VEC3_ALL0;
		this->PlayerPara[CntPlayer].BulletPara.BulletStock = BULLET_MAX_STOCK;
		this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
		for (int i = 0; i < 3; i++) this->PlayerPara[CntPlayer].BulletPara.BulletMove[i] = VEC3_ALL0;

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
		this->PostureVec[CntPlayer].FNVecFunc(VEC3_ALL0);
		this->PostureVec[CntPlayer].FNUNCrossFunc(VEC3_ALL0);
		this->PostureVec[CntPlayer].QrotFunc(0.0f);

		//砲塔
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot(VEC3_ALL0);

		//砲身								 
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot(VEC3_ALL0);

		//モデルのリセット
		ResetModel(
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].VtxBuff(),
			this->modelOri.Vtx[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN].VtxBuff(),
			this->modelDraw[CntPlayer].Vtx[PLAYER_PARTS_TYPE_HOUSIN].IdxBuff(),
			&this->modelOri.Attribute[PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN]);
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
		D3DXVECTOR3 FIELDNORMALVec = VEC3_ALL0;

		//レイキャスト関数
		pfield->FieldHitGetSphereVec(RayStart, RayEnd, &FIELDNORMALVec, &ReturnPosY);

		//レイキャスト結果を反映
		D3DXVECTOR3 Pos = RayStart;
		Pos.y = ReturnPosY;
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(Pos);
		this->PostureVec[CntPlayer].FNVecFunc(FIELDNORMALVec);
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
void PLAYER::Update(void)
{
	//-----------------何人死んだか計算。三人死んだらゲーム終了。次のシーンへ
	int deadcnt = 0;
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (this->iUseType[CntPlayer].Use() == NoUse) deadcnt++;
		if (deadcnt >= 3)
		{
			//ローカル対戦時は3人死ぬとすぐシーン遷移
			if (pmysocket->GetNetGameStartFlag() ==false)
			{
				pscene->NextScene(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
			}
			//ネット対戦時は3人死んでから他のプレイヤーと同期した後にシーン遷移
			else
			{
				if (pmysocket->MultThreadFlagFunc() == false)
				{
					pscene->NextScene(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameclear01);
				}
			}
		}
	}

	//-----------------ローカル対戦制御
	if (pmysocket->GetNetGameStartFlag() == false)
	{
#ifdef _DEBUG
		this->Move->MoveKeybord(0, this);//デバッグ用
#endif
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			bool use = this->iUseType[CntPlayer].Use();
			//生きていれば制御可能
			if (use)
			{
				//---------------制御関数
				this->Move->MoveL(CntPlayer, this, pmysocket->GetNetGameStartFlag());
				this->CameraRotControl(CntPlayer, pmysocket->GetNetGameStartFlag());
				this->Quaternion(CntPlayer);
				this->CameraRevers(CntPlayer, pmysocket->GetNetGameStartFlag());
				this->BulletALL(CntPlayer, pmysocket->GetNetGameStartFlag());
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

	//-----------------ネット対戦制御
	else
	{
		bool use = this->iUseType[pmysocket->GetNetMyNumber()].Use();

		//生きていれば操作可能
		if (use)
		{
			//ネット用フラグ初期化　1フレーム中の変更を保存して変更があればサーバーにデータを送る
			//---------------初期化
			this->PlayerPara[pmysocket->GetNetMyNumber()].MorphingPara.NetGetMorphingOneFrame = false;
			this->PlayerPara[pmysocket->GetNetMyNumber()].BulletPara.BulletStartPos = VEC3_ALL0;
			for (int i = 0; i < 3; i++) this->PlayerPara[pmysocket->GetNetMyNumber()].BulletPara.BulletMove[i] = VEC3_ALL0;
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 0;
				this->PlayerPara[CntPlayer].StandardPara.OldVital = this->PlayerPara[CntPlayer].StandardPara.Vital;
			}

			//---------------制御関数
			this->Move->MoveKeybord(pmysocket->GetNetMyNumber(), this);//デバッグ用
			this->Move->MoveL(pmysocket->GetNetMyNumber(), this, pmysocket->GetNetGameStartFlag());
			this->CameraRotControl(pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->Quaternion(CntPlayer);
			this->CameraRevers(pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			this->BulletALL(pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			this->ItemTimeKiri(pmysocket->GetNetMyNumber());
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->ItemTimeMorphing(CntPlayer);
		}

		//それ以外はカメラだけ制御
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
	//		PositionShadow(g_EnemyHoudai[CntEnemy].parameter.obj->GetShadow()Idx,
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
	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//一分割で4プレイヤーループ描画させる　DRAW元が4画面分ループさせてるから、実質16体分ループで描画している
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (this->iUseType[CntPlayer].Use() == YesUseType1)
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
				D3DXVECTOR3 UpNorFIELDNORMALVec = this->PostureVec[CntPlayer].FNUNCrossFunc();
				float Qrot = this->PostureVec[CntPlayer].QrotFunc();
				D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();

				//q=(rotVecAxis法線)*(g_Player.rot回転)
				D3DXQuaternionRotationAxis(&q, &UpNorFIELDNORMALVec, -Qrot);
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
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				// 描画
				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].NumMat(); nCntMat++)
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
						this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].NumVertex(), 0,
						this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUDAI].NumPolygon());
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
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].NumMat(); nCntMat++)
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
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].NumVertex(), 0,
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUTOU].NumPolygon());
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
				LPD3DXBUFFER Mat = *this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].Mat();
				pD3DXMat = (D3DXMATERIAL*)Mat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].NumMat(); nCntMat++)
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
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].NumVertex(), 0,
							this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN].NumPolygon());
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
	D3DXVECTOR3 moverot = VEC3_ALL0;
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
}

//=============================================================================
// クォータニオン制御
//=============================================================================
void PLAYER::Quaternion(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 FIELDNORMALVec = this->PostureVec[CntPlayer].FNVecFunc();
	D3DXVECTOR3 FIELDNORMALUpNorCross = this->PostureVec[CntPlayer].FNUNCrossFunc();
	float Qrot = this->PostureVec[CntPlayer].QrotFunc();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
	D3DXVec3Cross(&FIELDNORMALUpNorCross, &FIELDNORMALVec, &UpVec);

	//地形法線とUpベクトルで内積をとる(軸に対する回転角度を求める)
	float Upkakezan = D3DXVec3Dot(&FIELDNORMALVec, &UpVec);

	//もし回転角度が0以外(平行でない)ならば回転角度θを求める
	//θはクォータニオンの任意軸を回転させる時の回転角度になる
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(FIELDNORMALVec.x*FIELDNORMALVec.x +
				FIELDNORMALVec.y *FIELDNORMALVec.y +
				FIELDNORMALVec.z * FIELDNORMALVec.z);
		Qrot = acosf(cossita);
	}
	else Qrot = 0.0f;

	//---------------------------------------------------------オブジェクト値セット
	this->PostureVec[CntPlayer].QrotFunc(Qrot);
	this->PostureVec[CntPlayer].FNUNCrossFunc(FIELDNORMALUpNorCross);

}

//=============================================================================
// バレット関連制御
//=============================================================================
void PLAYER::BulletALL(int CntPlayer, bool Netflag)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FIELDNORMALVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
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
		this->PlayerPara[CntPlayer].BulletPara.BulletRotY = 1.57f;		//下方向ベクトルrot=0.0f、上方向ベクトルrot=3.14、に対しての前方向ベクトルはrot=1.57f。
	}
	this->PlayerPara[CntPlayer].BulletPara.BulletRotY -= 1.57f;

	//プレイヤーposから発射方向に少しずらした値
	//地面の傾きに沿って発射するときは問題ない。その傾きから左右に回転してる時だけposがおかしい
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
			this->PlayerPara[CntPlayer].BulletPara.NetBulletShotFlagOneFrame = 1;
			pbullet->SetInstance(this->PlayerPara[CntPlayer].BulletPara.BulletStartPos, this->PlayerPara[CntPlayer].BulletPara.BulletMove[0], BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
			//拡散弾処理
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
void PLAYER::BulletALLMoveL2R2Ver(int CntPlayer)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
	D3DXVECTOR3 HoudaiRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
	D3DXVECTOR3 HoutouRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
	D3DXVECTOR3 HousinRot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

	D3DXVECTOR3 FIELDNORMALVec = this->PostureVec[CntPlayer].FNVecFunc();

	D3DXVECTOR3 Frontvec;
	Frontvec.x = sinf(HoudaiRot.y + HoutouRot.y);
	Frontvec.y = 0.0f;
	Frontvec.z = cosf(HoudaiRot.y + HoutouRot.y);

	//地形の角度とプレイヤーの角度を計算。バレット発射方向で使う
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
			pbullet->SetInstance(BposStart, bulletmove, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
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
				pbullet->SetInstance(BposStart, leftB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));
				pbullet->SetInstance(BposStart, rightB, BULLET_EFFECT_SIZE, BULLET_EFFECT_SIZE, BULLET_EFFECT_TIME, ePLAYER_TYPE(CntPlayer));

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
// スピードアップ制御　未使用
//=============================================================================
void PLAYER::ItemTimeSpeed(int CntPlayer, EFFECT *effect)
{
	//スピードバフ時間減少
	if (this->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
	{
		this->PlayerPara[CntPlayer].ItemPara.SpeedBuffTime -= VALUE_SPEEDBUFF_SUB;
		
		// エフェクトスピードアップの生成
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
		this->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = true;
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
				&this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN],
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
			&this->modelDraw[CntPlayer].Attribute[PLAYER_PARTS_TYPE_HOUSIN],
			0.01f, &this->PlayerPara[CntPlayer].MorphingPara.MorphingDTtime, &this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal);
		if (this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal == EndMorphing)
		{
			this->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
		}
	}
}

