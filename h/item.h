/**
* @file item.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_SMALL_SCL				0.01f			//アイテム取得後がアイテムがだんだん小さくなる値
#define ITEM_DELETE_SCL				0.5f			//アイテムが消える値

//**************************************
// 種類
//**************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3					pos;				// 現在の位置
	D3DXVECTOR3					scl;				//!< モデルの大きさ(スケール)
	D3DXVECTOR3					rot;				// 現在の向き
	D3DXVECTOR3					Upvec;				// モデルの上方向
	D3DXVECTOR3					rotVecAxis;			//くおの時に使う地形の法線ベクトル
	D3DXVECTOR3					rotTOaxis;			// 地形法線とプレイヤーUpベクトルの外積値
	float						Qrot;				// Upベクトルから地形法線への回転角度
	float						fRadius;			// 半径
	float						Droptime;			// 出現するまでの時間
	int							nIdxShadow;			// 影ID
	int							nType;				// 種類
	int							GoukeiDrop;			// ドロップ合計
	int							GetPlayerType;		// どのプレイヤーが取得したか
	bool						bUse;				// 使用しているかどうか
	bool						GettingSignal;		// プレイヤーがアイテムを取得したかどうか　真:取得中　偽:取得していない
	bool						GettingSignalEnd;	// プレイヤーがアイテムを取得終了したかどうか　真:終了　偽:終了していない、もしくは取得していない
	bool						fCollisionEnd;		// 地形との当たり判定が終わったかどうか
} ITEM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, int nType);
void DeleteItem(int nIdxItem);
ITEM *GetItem(void);
void GettingItem(int nIdxItem);

