/**
* @file player.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
 * @enum ePLAYER_PARTS_TYPE
 * @brief 
 */
enum ePLAYER_PARTS_TYPE
{
	PLAYER_PARTS_TYPE_HOUDAI = 0,		//!< 砲台
	PLAYER_PARTS_TYPE_HOUTOU,			//!< 砲塔
	PLAYER_PARTS_TYPE_HOUSIN,			//!< 砲身
	PLAYER_PARTS_TYPE_MAX,				//!< 合計数
};

/**
 * @enum ePLAYER_PARTS_TYPE
 * @brief
 */
enum ePLAYER_MODEL_ORIGINAL_TYPE
{
	PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN = 0,			//!< 砲身通常
	PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN_MORPHING,		//!< 砲身アタック
	PLAYER_MODEL_ORIGINAL_TYPE_MAX,					//!< 合計数
};

/**
 * @enum ePLAYER_MODEL_ALL_TYPE
 * @brief
 */
enum ePLAYER_MODEL_ALL_TYPE
{
	PLAYER_MODEL_DRAW_ALL_HOUDAI = 0,		//!< 砲台
	PLAYER_MODEL_DRAW_ALL_HOUTOU,			//!< 砲塔
	PLAYER_MODEL_DRAW_ALL_HOUSIN,			//!< 砲身
	PLAYER_MODEL_DRAW_ALL_HOUSIN_MORPHING,	//!< 砲身アタック
	PLAYER_MODEL_DRAW_ALL_MAX,				//!< 合計数
};

/**
*　@class PLAYER_PARAMETER_BULLET
*　@brief プレイヤーバレット用パラメータ
*/
class PLAYER_PARAMETER_BULLET
{
public:
	PLAYER_PARAMETER_BULLET() {
		BulletStartPos = VEC3_ALL0;	for (int i = 0; i < 3; i++) BulletMove[i] = VEC3_ALL0;
		BulletRotY = 0.0f; BulletBornTime = 0.0f; BulletStock = 0; NetBulletShotFlagOneFrame = 0;
	};
	~PLAYER_PARAMETER_BULLET() {}

	D3DXVECTOR3		BulletStartPos;				//!< バレットの発射地点
	D3DXVECTOR3		BulletMove[3];				//!< バレットの移動量　パワーアップ中は三方向に発射
	float			BulletRotY;					//!< バレットのFrontベクトルから地形法線への回転角度
	float			BulletBornTime;				//!< 残弾回復カウント。時間で回復 BULLET_BORN_MAXTIME
	int				BulletStock;				//!< 残弾数
	int				NetBulletShotFlagOneFrame;	//!< ネット対戦用　バレットを発射していなと0,通常モデルは発射1,アタックモデルは発射3　毎フレーム確認して、バレット発射するとフラグを立てる
};

/**
*　@class PLAYER_PARAMETER_STANDARD
*　@brief プレイヤー標準パラメータ　法線　姿勢ベクトル　体力など
*/
class PLAYER_PARAMETER_STANDARD
{
public:
	PLAYER_PARAMETER_STANDARD() {
		FUPFCross = VEC3_ALL0; Speed = 0.0f; Vital = PLAYER_VITAL_MAX; OldVital = PLAYER_VITAL_MAX; ShadowIdx = -1;
		eModelType = PLAYER_MODEL_TYPE_NORMAL; eOldModelType = PLAYER_MODEL_TYPE_NORMAL;
	}
	~PLAYER_PARAMETER_STANDARD() {}

	D3DXVECTOR3		FUPFCross;			//!< 地形法線とプレイヤーFrontベクトルの外積値 FieldUpPlayerFrontCross
	float			Speed;				//!< 移動スピード
	int				Vital;				//!< 体力
	int				OldVital;			//!< 体力
	int				ShadowIdx;			//!< 影のインデックス番号
	ePLAYER_MODEL_TYPE	eModelType;		//!< どのモデル状態か判定
	ePLAYER_MODEL_TYPE	eOldModelType;	//!< どのモデル状態か判定

};

/**
*　@class PLAYER_PARAMETER_ITEM
*　@brief プレイヤーアイテム用パラメータ　効果時間　発動フラグ　など
*/
class PLAYER_PARAMETER_ITEM
{
public:
	PLAYER_PARAMETER_ITEM() {
		SpeedBuff = 0.0f; SpeedBuffTime = 0.0f; BackCameraTime = 0.0f; KiriTime = 0.0f;
		SpeedBuffSignal = false; BackCameraItemSignal = false; KiriSignal = false; DashSignal = false;
	};
	~PLAYER_PARAMETER_ITEM() {}

	float	SpeedBuff;					//!< 移動スピードアイテムバフ MAX_SPEEDBUFF
	float	SpeedBuffTime;				//!< 移動スピードアイテム効果時間
	float	BackCameraTime;				//!< バックカメラアイテムの効果時間
	float	KiriTime;					//!< 霧アイテムの効果時間
	bool	SpeedBuffSignal;			//!< スピードアップバフ判定
	bool	BackCameraItemSignal;		//!< バックカメラアイテムを使用しているかどうか
	bool	KiriSignal;					//!< 霧アイテムを使用しているかどうか
	bool	DashSignal;					//!< ダッシュ判定　入力がある間true.それ以外はfalse
};

/**
*　@class PLAYER_PARAMETER_MORPHING
*　@brief プレイヤーモーフィングパラメータ　時間　信号など
*/
class PLAYER_PARAMETER_MORPHING
{
public:
	PLAYER_PARAMETER_MORPHING() {
		MorphingTime = 0.0f; MorphingDTtime = 0.0f; MorphingSignal = NoMorphing;
		MorphingStart = false; MorphingEnd = false; NetGetMorphingOneFrame = false;
	}
	~PLAYER_PARAMETER_MORPHING() {}

	float			MorphingTime;				//!< モーフィングできる時間
	float			MorphingDTtime;				//!< モーフィング時に補間タイムとして使用
	eMORPHING_TYPE	MorphingSignal;				//!< 1,なにもしない　2,モーフィング中　3,モーフィング完了
	bool			MorphingStart;				//!< 変形判定　trueアタックモデル(アイテム取得時true)　false通常モデル(tureになって時間経過後false)
	bool			MorphingEnd;				//!< 変形終了判定　
	bool			NetGetMorphingOneFrame;		//!< ネット対戦用　trueでモーフィング中　モーフィングの同期で使用
};

/**
*　@class PLAYER_PARAMETER_SUMMARY
*　@brief プレイヤーパラメータまとめ
*/
class PLAYER_PARAMETER_SUMMARY
{
public:
	PLAYER_PARAMETER_SUMMARY() {}
	~PLAYER_PARAMETER_SUMMARY() {}

	PLAYER_PARAMETER_BULLET		BulletPara;
	PLAYER_PARAMETER_STANDARD	StandardPara;
	PLAYER_PARAMETER_ITEM		ItemPara;
	PLAYER_PARAMETER_MORPHING	MorphingPara;
};

/**
*　@class PLAYER_MODEL_DRAW
*　@brief プレイヤーモデル　描画用
*/
class PLAYER_MODEL_DRAW
{
public:
	PLAYER_MODEL_DRAW() {}
	~PLAYER_MODEL_DRAW() {}

	MODELATTRIBUTE		Attribute[PLAYER_PARTS_TYPE_MAX];		//!< モデルデータ　0ノーマル、1攻撃　モデルデータの種類　「プレイヤーの数」=4、「モデルパーツ数(砲台、砲塔、砲身、砲身変形後)」=2
	VTXBUFFER			Vtx[PLAYER_PARTS_TYPE_MAX];					//!< 頂点情報　描画用
	TRANSFORM			Transform[PLAYER_PARTS_TYPE_MAX];			//!< トランスフォーム情報

};

/**
*　@class PLAYER_MODEL_ORIGINAL
*　@brief プレイヤーモデル　オリジナル用
*/
class PLAYER_MODEL_ORIGINAL
{
public:
	PLAYER_MODEL_ORIGINAL() {}
	~PLAYER_MODEL_ORIGINAL() {}

	MODELATTRIBUTE		Attribute[PLAYER_MODEL_ORIGINAL_TYPE_MAX];		//!< モデルデータ　0ノーマル、1攻撃　モデルデータの種類　「砲身、砲身変形後」=2
	VTXBUFFER			Vtx[PLAYER_MODEL_ORIGINAL_TYPE_MAX];				//!< 頂点情報　モーフィング基準用
};

/**
*　@class PLAYER
*　@brief GAMEOBJECT派生クラス
*/
class PLAYER : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	PLAYER();	//!< データ読み込み　初期化
	~PLAYER();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;		//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	PLAYER_PARAMETER_SUMMARY PlayerPara[OBJECT_PLAYER_MAX];	//!< インスタンスに必要なデータ群	数が多いので複数包含している							
	PLAYER_MODEL_DRAW modelDraw[OBJECT_PLAYER_MAX];			//!< 描画用モデルデータ
	iUseCheck iUseType[OBJECT_PLAYER_MAX];					//!< 使用情報
	FIELDNORMAL	PostureVec[OBJECT_PLAYER_MAX];				//!< 姿勢ベクトル

private:
	TEXTURE	tex;						//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX
	PLAYER_MODEL_ORIGINAL modelOri;		//!< オリジナルモデルデータ　モーフィング時の基準用
	MOVEMENT Move[OBJECT_PLAYER_MAX];	//!< 移動量

	//------他クラスのアドレス
	FIELD *pfield;
	MySOCKET *pmysocket;
	SCENE *pscene;
	EFFECT *peffect;
	BULLET *pbullet;

	//------カラー
	void PlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff, DWORD nNumPolygon, int CntPlayer);

	//------移動制御
	void MoveABL(int CntPlayer, EFFECT *effect, bool Netflag);		//!< 移動制御(ABボタンLスティックで移動制御)
	void MoveL(int CntPlayer, EFFECT *effect, bool Netflag);		//!< 移動制御(LRスティックで移動制御)
	void MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag);	//!< 移動制御(LRスティックで移動制御)
	void MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag);		//!< 移動制御(L2R2で移動制御)
	void MoveKeybord(int CntPlayer, EFFECT *effect);				//!< 移動制御(ki-bo-doで移動制御)

	//------カメラ制御
	void CameraRevers(int CntPlayer, bool Netflag);		//!< カメラ制御
	void CameraRotControl(int CntPlayer, bool Netflag);	//!< カメラ制御(LRスティックで移動制御)

	//------姿勢制御
	void Quaternion(int CntPlayer);		//!< クォータニオン制御　地形法線とUPベクトルと外積

	//------バレット制御
	void BulletALL(int CntPlayer, bool Netflag);	//!< バレット関連制御
	void BulletALLMoveL2R2Ver(int CntPlayer);		//!< バレット関連制御 発射ボタンを十字カーソル

	//------アイテム制御
	void ItemTimeKiri(int CntPlayer);				//!< フォグ制御
	void ItemTimeMorphing(int CntPlayer);			//!< モーフィング制御

	//読み込むモデル　描画用
	const char *c_aFileNameModelDraw[PLAYER_MODEL_DRAW_ALL_MAX] =
	{
		"../data/MODEL/PlayerSensyaHoudai.x",
		"../data/MODEL/PlayerSensyaHoutou.x",
		"../data/MODEL/PlayerSensyaHousin.x",
	};

	//読み込むモデル　オリジナル用　モーフィング時の基準データ
	const char *c_aFileNameModelOriginal[PLAYER_MODEL_ORIGINAL_TYPE_MAX] =
	{
		"../data/MODEL/PlayerSensyaHousin.x",
		"../data/MODEL/PlayerSensyaHousinMo.x"
	};


};


