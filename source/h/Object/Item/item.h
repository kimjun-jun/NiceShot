/**
* @file item.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float ITEM_BIG_SCL{ 2.0f };			//アイテムサイズ初期値
constexpr float ITEM_SMALL_SCL{ 0.03f };			//アイテム取得後がアイテムがだんだん小さくなる値
constexpr float ITEM_DELETE_SCL{ 0.5f };			//アイテムが消える値

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class ITEM_PARAMETER_ALL
*　@brief 
*/
class ITEM_PARAMETER_ALL
{
public:
	ITEM_PARAMETER_ALL() {
		nIdxShadow = -1; GetPlayerType = 0; LinkShadowPos = VEC3_ALL0; GettingSignal = false; GettingSignalEnd = false;
		CollisionFieldEnd = false; NetUse = false; NetGetItemFlag = false; NetGetItemFlagOld = false; ShadowPosSignal = false;
		eType = ITEM_TYPE_NONE;
	}
	~ITEM_PARAMETER_ALL() {}

	int		nIdxShadow;				//!< 影ID
	int		GetPlayerType;			//!< どのプレイヤーが取得したか
	bool	GettingSignal;			//!< プレイヤーがアイテムを取得したかどうか　真:取得中　偽:取得していない
	bool	GettingSignalEnd;		//!< プレイヤーがアイテムを取得終了したかどうか　真:終了　偽:終了していない、もしくは取得していない
	bool	CollisionFieldEnd;		//!< 地形との当たり判定が終わったかどうか
	bool	ShadowPosSignal;		//!< 影の高さ計算フラグ

	bool	NetUse;
	bool	NetGetItemFlag;			//!< アイテムを取得した1フレーム分だけtrueにする
	bool	NetGetItemFlagOld;		//!< アイテムを取得した1フレーム分だけtrueにする 比較値

	D3DXVECTOR3		LinkShadowPos;	//!< 影の座標
	eITEM_TYPE		eType;			//!< 種類

};

/**
*　@class ITEM_PARAMETER_ONE
*　@brief GAMEOBJECT派生クラス
*/
class ITEM_PARAMETER_ONE
{
public:
	ITEM_PARAMETER_ONE() { GoukeiDrop = 0, Droptime = -1; 
	MaxSize = D3DXVECTOR3(ITEM_BIG_SCL, ITEM_BIG_SCL, ITEM_BIG_SCL); MinSize = D3DXVECTOR3(ITEM_DELETE_SCL, ITEM_DELETE_SCL, ITEM_DELETE_SCL);}
	~ITEM_PARAMETER_ONE() {}

	int	GoukeiDrop;			//!< ドロップ合計
	int	Droptime;			//!< 出現するまでの時間
	D3DXVECTOR3	MaxSize;	//!< 最大サイズ
	D3DXVECTOR3	MinSize;	//!< 最小サイズ
};

/**
*　@class ITEM
*　@brief GAMEOBJECT派生クラス
*/
class ITEM : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	ITEM();		//!< データ読み込み　初期化
	~ITEM();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画

	ITEM_PARAMETER_ALL ItemParaAll[OBJECT_ITEM_MAX];	//!< 各インスタンスに必要なデータ群
	iUseCheck iUseType[OBJECT_ITEM_MAX];		//!< 使用情報
	TRANSFORM Transform[OBJECT_ITEM_MAX];		//!< トランスフォーム情報
	FIELDNORMAL	PostureVec[OBJECT_ITEM_MAX];	//!< 姿勢ベクトル
	void SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType);
	void SetInstance(int Index, D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType);
	void ReleaseInstance(int nIdxItem);

private:
	MODELATTRIBUTE model;				//!< モデル情報　マテリアルや頂点数など　複数使用するならここを配列化
	TEXTURE tex[ITEM_TYPE_MAX];			//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX
	VTXBUFFER vtx[OBJECT_ITEM_MAX];		//!< 頂点情報　複数使用するならここを配列化
	ITEM_PARAMETER_ONE ItemParaOne;		//!< マネージャーに必要なデータ群

	//------他クラスのアドレス
	PLAYER *pplayer;
	SHADOW *pshadow;
	MySOCKET *pmysocket;

	void	GettingItem(int nIdxItem);

	const char *c_aFileNameModel[ITEM_TYPE_MAX] =
	{
		"../data/MODEL/tikeiItem.x",		// 地形変形アイテム
		"../data/MODEL/lifeItem.x",			// ライフ回復
		"../data/MODEL/sensyaItem.x",		// 戦車変形アイテム
		"../data/MODEL/bulletItem.x",		// バレットアイテム
		"../data/MODEL/speedItem.x",		// スピードアップアイテム
		"../data/MODEL/cameraItem.x",		// 強制バックカメラアイテム
		"../data/MODEL/kiriItem.x",			// 霧アイテム
	};

	const char *c_aFileNameTex[ITEM_TYPE_MAX] =
	{
		"../data/MODEL/landmark_aogashima.png",			// 地形変形アイテム
		"../data/MODEL/life000.png",					// ライフ回復
		"../data/MODEL/war_sensya_noman.png",			// 戦車変形アイテム
		"../data/MODEL/bullettex.png",					// バレットアイテム
		"../data/MODEL/1213810.png",					// スピードアップアイテム
		"../data/MODEL/mark_camera_satsuei_ok.png",		// 強制バックカメラアイテム
		"../data/MODEL/yama_kiri.png",					// 霧アイテム
	};

} ;

