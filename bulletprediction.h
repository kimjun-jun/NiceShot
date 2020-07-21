//#pragma once
///**
//* @file bullet.h
//* @brief NiceShot(3D)戦車ゲーム
//* @author キムラジュン
//* @date 2020/01/15
//*/
//#pragma once
//
//
////*****************************************************************************
//// 構造体定義
////*****************************************************************************
//
//#define	BULLET_PREDICTION_SIZE_X			(50.0f)							// ビルボードの幅
//#define	BULLET_PREDICTION_SIZE_Y			(50.0f)							// ビルボードの高さ
//
//
//typedef struct
//{
//	D3DXVECTOR3			pos;						// 位置
//	D3DXVECTOR3			rot;						// 回転
//	D3DXVECTOR3			scale;						// スケール
//	D3DXVECTOR3			move;						// 移動量
//	D3DXVECTOR3			Hormingmove;				// ホーミング移動量
//	float				fSizeX;						// 幅
//	float				fSizeY;						// 高さ
//	int					nTimer;						// タイマー
//	int					nIdxShadow;					// 影ID
//	int					UsePlayerType;				// 何プレイヤーが使用してるか識別する
//	int					HormingPlayerType;			// 何プレイヤーにホーミングしてるか識別する
//	float				fRadius;					// 半径
//	float				GravityAdd;					// 重力加速度
//	float				Gravity;					// 重力
//	float				FieldPosY;					// 影用の現在地の地形POSYを記憶
//	bool				HormingSignal;				// ホーミングしてるかどうか、プレイヤーの一定範囲以内になるとtrue
//	bool				use;						// 使用しているかどうか
//} BULLET_PREDICTION;
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//HRESULT InitBulletPrediction(int type);
//void UninitBulletPrediction(void);
//void UpdateBulletPrediction(void);
//void DrawBulletPrediction(void);
//
//int SetBulletPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, int type);
//void ReleaseBulletPrediction(int nIdxBulletPrediction);
//BULLET_PREDICTION *GetBulletPrediction(void);
//
