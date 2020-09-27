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
#define ITEM_SMALL_SCL				0.03f			//アイテム取得後がアイテムがだんだん小さくなる値
#define ITEM_DELETE_SCL				0.5f			//アイテムが消える値

//*****************************************************************************
// クラス定義
//*****************************************************************************
class ITEM : public OBJECT_3D
{
public:
	ITEM() {
		Droptime = 0.0f, nType, GoukeiDrop = 0, GetPlayerType = 0,
			GettingSignal = false, GettingSignalEnd = false, CollisionFieldEnd = false;
	};
	void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, int nType);
	void DeleteItem(int nIdxItem);
	void GettingItem(int nIdxItem,PLAYER_HONTAI *p);
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(PLAYER_HONTAI *p);				//!< 更新
	virtual void						Draw(void);					//!< 描画
	PLAYER_HONTAI				*p;						//!<プレイヤー先頭アドレス
	float						Droptime;				//!< 出現するまでの時間
	int							nIdxShadow;				//!< 影ID
	int							nType;					//!< 種類
	int							GoukeiDrop;				//!< ドロップ合計
	int							GetPlayerType;			//!< どのプレイヤーが取得したか
	bool						GettingSignal;			//!< プレイヤーがアイテムを取得したかどうか　真:取得中　偽:取得していない
	bool						GettingSignalEnd;		//!< プレイヤーがアイテムを取得終了したかどうか　真:終了　偽:終了していない、もしくは取得していない
	bool						CollisionFieldEnd;		//!< 地形との当たり判定が終わったかどうか
} ;

