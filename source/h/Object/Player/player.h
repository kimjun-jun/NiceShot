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
#include "../../../h/Object/ObjectClass/StandardComponent/Move/Move.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Attack/Attack.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Posture/Posture.h"

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

	//------アイテム制御
	void ItemTimeSpeed(PLAYER *Player, int CntPlayer);	//!< スピードアップ制御
	void ItemTimeKiri(int CntPlayer);				//!< フォグ制御

	//------------------------get関数
	inline float GetSpeedBuff(void) { return SpeedBuff; };
	inline float GetSpeedBuffTime(void) { return SpeedBuffTime; };
	inline float GetBackCameraTime(void) { return BackCameraTime; };
	inline float GetKiriTime(void) { return KiriTime; };
	inline bool GetSpeedBuffSignal(void) { return SpeedBuffSignal; };
	inline bool GetBackCameraItemSignal(void) { return BackCameraItemSignal; };
	inline bool GetKiriSignal(void) { return KiriSignal; };
	inline bool GetDashSignal(void) { return DashSignal; };
	//------------------------set関数
	inline void SetSpeedBuff(const float InBuff) { SpeedBuff = InBuff; };
	inline void SetSpeedBuffTime(const float InBuffTime) { SpeedBuffTime = InBuffTime; };
	inline void SetBackCameraTime(const float InTime) { BackCameraTime = InTime; };
	inline void SetKiriTime(const float InTime) { KiriTime = InTime; };
	inline void SetSpeedBuffSignal(const bool InFlag) { SpeedBuffSignal = InFlag; };
	inline void SetBackCameraItemSignal(const bool InFlag) { BackCameraItemSignal = InFlag; };
	inline void SetKiriSignal(const bool InFlag) { KiriSignal = InFlag; };
	inline void SetDashSignal(const bool InFlag) { DashSignal = InFlag; };

private:
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
		MorphingStart = false; MorphingEnd = false; NetMorphingOneFrame = false;
	}
	~PLAYER_PARAMETER_MORPHING() {}

	//------アイテム制御
	void ItemTimeMorphing(PLAYER *Player, int CntPlayer);		//!< モーフィング制御

	//------------------------get関数
	inline float GetMorphingTime(void) { return MorphingTime; };
	inline float GetMorphingDTtime(void) { return MorphingDTtime; };
	inline bool GetMorphingStart(void) { return MorphingStart; };
	inline bool GetMorphingEnd(void) { return MorphingEnd; };
	inline bool GetNetMorphingOneFrame(void) { return NetMorphingOneFrame; };
	inline eMORPHING_TYPE GetMorphingSignal(void) { return MorphingSignal; };
	//------------------------set関数
	inline void SetMorphingTime(const float InTime) { MorphingTime = InTime; };
	inline void SetMorphingDTtime(const float InDtTime) { MorphingDTtime = InDtTime; };
	inline void SetMorphingStart(const bool InFlag) { MorphingStart = InFlag; };
	inline void SetMorphingEnd(const bool InFlag) { MorphingEnd = InFlag; };
	inline void SetNetMorphingOneFrame(const bool InFlag) { NetMorphingOneFrame = InFlag; };
	inline void SetMorphingSignal(const eMORPHING_TYPE InType) { MorphingSignal = InType; };

private:
	float			MorphingTime;				//!< モーフィングできる時間
	float			MorphingDTtime;				//!< モーフィング時に補間タイムとして使用
	bool			MorphingStart;				//!< 変形判定　trueアタックモデル(アイテム取得時true)　false通常モデル(tureになって時間経過後false)
	bool			MorphingEnd;				//!< 変形終了判定　
	bool			NetMorphingOneFrame;		//!< ネット対戦用　trueでモーフィング中　モーフィングの同期で使用
	eMORPHING_TYPE	MorphingSignal;				//!< 1,なにもしない　2,モーフィング中　3,モーフィング完了
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

	PLAYER_PARAMETER_MOVE		MovePara;
	PLAYER_PARAMETER_BULLET		BulletPara;
	PLAYER_PARAMETER_ITEM		ItemPara;
	PLAYER_PARAMETER_MORPHING	MorphingPara;
	PLAYER_PARAMETER_STANDARD	StandardPara;
	POSTURE	PostureVec;					//!< 姿勢ベクトル
};

/**
*　@class PLAYER
*　@brief GAMEOBJECT派生クラス
*/
class PLAYER : public GAME_OBJECT_INTERFACE_SUMMRY
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
	PLAYER_MODEL_ORIGINAL modelOri;							//!< オリジナルモデルデータ　モーフィング時の基準用
	iUseCheck iUseType[OBJECT_PLAYER_MAX];					//!< 使用情報

	//------他クラスのアドレス 特別にパブリック
	EFFECT *peffect;
	BULLET *pbullet;

private:
	TEXTURE	tex;						//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX

	//------他クラスのアドレス
	FIELD *pfield;
	MySOCKET *pmysocket;
	SCENE *pscene;

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


