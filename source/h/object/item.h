/**
* @file item.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"
class PLAYER_HONTAI;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_BIG_SCL				2.0f			//アイテムサイズ初期値
#define ITEM_SMALL_SCL				0.03f			//アイテム取得後がアイテムがだんだん小さくなる値
#define ITEM_DELETE_SCL				0.5f			//アイテムが消える値

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
		nIdxShadow = -1; GetPlayerType = 0; GettingSignal = false; GettingSignalEnd = false;
		CollisionFieldEnd = false; NetUse = false; NetGetItemFlag = false; NetGetItemFlagOld = false;
		eType = ITEMTYPE_NONE;
	}
	~ITEM_PARAMETER_ALL() {}

	int		nIdxShadow;				//!< 影ID
	int		GetPlayerType;			//!< どのプレイヤーが取得したか
	bool	GettingSignal;			//!< プレイヤーがアイテムを取得したかどうか　真:取得中　偽:取得していない
	bool	GettingSignalEnd;		//!< プレイヤーがアイテムを取得終了したかどうか　真:終了　偽:終了していない、もしくは取得していない
	bool	CollisionFieldEnd;		//!< 地形との当たり判定が終わったかどうか

	bool	NetUse;
	bool	NetGetItemFlag;			//!< アイテムを取得した1フレーム分だけtrueにする
	bool	NetGetItemFlagOld;		//!< アイテムを取得した1フレーム分だけtrueにする 比較値

	eITEMTYPE		eType;			//!< 種類

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
class ITEM : public GAME_OBJECT
{
public:
	ITEM();		//!< データ読み込み　初期化
	~ITEM();	//!< 削除

private:
	void	Init(void);										//!< 初期化
	void	Update(PLAYER_HONTAI *p,bool NetGameStartFlag);	//!< 更新
	void	Draw(void);										//!< 描画

	//void	ReinitNet(void);								//!< 初期化　同期

	void	SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType);
	void	ReleaseInstance(int nIdxItem);
	void	GettingItem(int nIdxItem, PLAYER_HONTAI *p);

	ModelAttribute			model;								//!< モデル情報　マテリアルや頂点数など　複数使用するならここを配列化
	TEXTURE					tex[ITEMTYPE_MAX];					//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX
	VTXBuffer				vtx[OBJECT_ITEM_MAX];				//!< 頂点情報　複数使用するならここを配列化
	TransForm				Transform[OBJECT_ITEM_MAX];			//!< トランスフォーム情報
	iUseCheak				iUseType[OBJECT_ITEM_MAX];			//!< 使用情報
	FieldNor				PostureVec[OBJECT_ITEM_MAX];		//!< 姿勢ベクトル
	ITEM_PARAMETER_ALL		ItemParaAll[OBJECT_ITEM_MAX];		//!< 各インスタンスに必要なデータ群
	ITEM_PARAMETER_ONE		ItemParaOne;						//!< マネージャーに必要なデータ群

	const char *c_aFileNameModel[ITEMTYPE_MAX] =
	{
		"../data/MODEL/tikeiItem.x",		// 地形変形アイテム
		"../data/MODEL/lifeItem.x",			// ライフ回復
		"../data/MODEL/sensyaItem.x",		// 戦車変形アイテム
		"../data/MODEL/bulletItem.x",		// バレットアイテム
		"../data/MODEL/speedItem.x",		// スピードアップアイテム
		"../data/MODEL/cameraItem.x",		// 強制バックカメラアイテム
		"../data/MODEL/kiriItem.x",			// 霧アイテム
	};

	const char *c_aFileNameTex[ITEMTYPE_MAX] =
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

