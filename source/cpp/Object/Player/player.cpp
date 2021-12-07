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
constexpr float	EFFECT_SPEEDUP_SIZE_X{ 25.0f };			//!< エフェクトスピードアップの幅
constexpr float	EFFECT_SPEEDUP_SIZE_Y{ 10.0f };			//!< エフェクトスピードアップの高さ
constexpr int	EFFECT_SPEEDUP_TIME{ 10 };				//!< エフェクトスピードアップの生存時間
constexpr float	PLAYER_VALUE_DASHRATE{ 2.0f };			//!< 歩きに比べてのダッシュの速度倍率
constexpr float	PLAYER_MOVE_RATE_X{ 1.0f / 40000.0f };	//!< 左右歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
constexpr float	PLAYER_MOVE_RATE_Y{ 1.0f / 200.0f };	//!< 前後歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
constexpr float	PLAYER_MOVE_RATE_LR2{ 1.0f / 20000.0f };//!< キャタピラの移動量倍率(0～65000が入力範囲なのでそれに乗算する)
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU{ 0.03f };	//!< プレイヤー砲塔回転量
constexpr float	VALUE_ROTATE_PLAYER_HOUTOU_MAX{ 1.57f };//!< プレイヤー砲塔回転量最大値
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN{ 0.01f };	//!< プレイヤー砲身回転量
constexpr float	VALUE_ROTATE_PLAYER_HOUSIN_MAX{ 0.2f };	//!< プレイヤー砲塔回転量最大値
constexpr float	MOVE_INERTIA_MOMENT{ 0.8f };			//!< 移動量の慣性モーメント、減速係数として使用
constexpr float	VALUE_SPEEDBUFF_SUB{ 1.0f };			//!< スピートアップ時間のデクリメントする値
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
			this->modelDraw[CntPlayer].Vtx[CntDrawPartsNum].MeshColor(np, CntPlayer);

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
		this->PlayerPara[CntPlayer].BulletPara.SetBulletBornTime(0.0f);
		this->PlayerPara[CntPlayer].BulletPara.SetBulletRotY(0.0f);
		this->PlayerPara[CntPlayer].BulletPara.SetBulletStartPos(VEC3_ALL0);
		this->PlayerPara[CntPlayer].BulletPara.SetBulletStock(BULLET_MAX_STOCK);
		this->PlayerPara[CntPlayer].BulletPara.SetNetBulletShotFlagOneFrame(0);
		for (int i = 0; i < 3; i++) this->PlayerPara[CntPlayer].BulletPara.SetBulletMove(VEC3_ALL0, i);

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
		this->PlayerPara[CntPlayer].MorphingPara.MorphingSignal = NoMorphing;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingStart = false;
		this->PlayerPara[CntPlayer].MorphingPara.MorphingTime = MORPHING_TIME;
		this->PlayerPara[CntPlayer].MorphingPara.NetGetMorphingOneFrame = false;

		//その他パラメータ
		this->PlayerPara[CntPlayer].PostureVec.FNVecFunc(VEC3_ALL0);
		this->PlayerPara[CntPlayer].PostureVec.FNUNCrossFunc(VEC3_ALL0);
		this->PlayerPara[CntPlayer].PostureVec.QrotFunc(0.0f);

		//砲塔PlayerPara[CntPlayer].
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
		D3DXVECTOR3 POSTUREVec = VEC3_ALL0;

		//レイキャスト関数
		pfield->FieldHitGetSphereVec(RayStart, RayEnd, &POSTUREVec, &ReturnPosY);

		//レイキャスト結果を反映
		D3DXVECTOR3 Pos = RayStart;
		Pos.y = ReturnPosY;
		this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos(Pos);
		this->PlayerPara[CntPlayer].PostureVec.FNVecFunc(POSTUREVec);
		this->PlayerPara[CntPlayer].PostureVec.Quaternion();
		CameraRevers(this, CntPlayer, false);

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
		this->PlayerPara[0].MovePara.MoveKeybord(this, 0);//デバッグ用
#endif
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			bool use = this->iUseType[CntPlayer].Use();
			//生きていれば制御可能
			if (use)
			{
				//---------------制御関数
				this->PlayerPara[CntPlayer].MovePara.MoveL(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				CameraRotControl(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				this->PlayerPara[CntPlayer].PostureVec.Quaternion();
				CameraRevers(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				this->PlayerPara[CntPlayer].BulletPara.BulletALL(this, CntPlayer, pmysocket->GetNetGameStartFlag());
				this->ItemTimeSpeed(CntPlayer, this->peffect);
				this->ItemTimeKiri(CntPlayer);
				this->ItemTimeMorphing(CntPlayer);
			}

			//それ以外はカメラだけ制御
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
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
			{
				this->PlayerPara[CntPlayer].StandardPara.OldVital = this->PlayerPara[CntPlayer].StandardPara.Vital;
			}

			//---------------制御関数
			//移動
			this->PlayerPara[pmysocket->GetNetMyNumber()].MovePara.MoveKeybord(this, pmysocket->GetNetMyNumber());//デバッグ用
			this->PlayerPara[pmysocket->GetNetMyNumber()].MovePara.MoveL(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			//回転
			CameraRotControl(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			CameraRevers(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			//姿勢
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->PlayerPara[CntPlayer].PostureVec.Quaternion();
			//バレット
			this->PlayerPara[pmysocket->GetNetMyNumber()].BulletPara.BulletALL(this, pmysocket->GetNetMyNumber(), pmysocket->GetNetGameStartFlag());
			//アイテム効果
			this->ItemTimeSpeed(pmysocket->GetNetMyNumber(),this->peffect);
			this->ItemTimeKiri(pmysocket->GetNetMyNumber());
			for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++) this->ItemTimeMorphing(CntPlayer);
		}

		//それ以外はカメラだけ制御
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
				D3DXVECTOR3 UpNorPOSTUREVec = this->PlayerPara[CntPlayer].PostureVec.FNUNCrossFunc();
				float Qrot = this->PlayerPara[CntPlayer].PostureVec.QrotFunc();
				D3DXQUATERNION q = D3DXQUATERNION(0, 0, 0, 1);
				D3DXVECTOR3 scl = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Scl();
				D3DXVECTOR3 rot = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
				D3DXVECTOR3 pos = this->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();

				//q=(rotVecAxis法線)*(g_Player.rot回転)
				D3DXQuaternionRotationAxis(&q, &UpNorPOSTUREVec, -Qrot);
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
// スピードアップ制御
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

