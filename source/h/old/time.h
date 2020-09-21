///**
//* @file time.h
//* @brief NiceShot(3D)戦車ゲーム
//* @author キムラジュン
//* @date 2020/01/15
//*/
//#pragma once
//
//
//// マクロ定義
//#define TEXTURE_GAME_DOT						"../data/TEXTURE/BG/dot.png"												//!< 読み込むテクスチャファイル名
//#define TEXTURE_GAME_TIMELOGO					"../data/TEXTURE/sozai/TIME.png" 											//!< 読み込むテクスチャファイル名
//#define TEXTURE_TIME_SIZE_X						(30)																	//!< テクスチャサイズY
//#define TEXTURE_TIME_SIZE_Y						(30)																	//!< テクスチャサイズX
//#define SCORE_TEXTURE_PATTERN_DIVIDE_X			(10)																	//!< アニメパターンのテクスチャ内分割数（X)  
//#define SCORE_TEXTURE_PATTERN_DIVIDE_Y			(1)																		//!< アニメパターンのテクスチャ内分割数（Y)  
//#define SCORE_ANIM_PATTERN_NUM					(SCORE_TEXTURE_PATTERN_DIVIDE_X*SCORE_TEXTURE_PATTERN_DIVIDE_Y)			//!< アニメーションパターン数  
//#define SCORE_TIME_ANIMATION					(4)																		//!< アニメーションの切り替わるカウント  
//#define TEXTURE_DOT_SIZE_X						(4)																		//!< テクスチャサイズX
//#define TEXTURE_DOT_SIZE_Y						(4)																		//!< テクスチャサイズY
//#define TIME_DIGIT								(3)																		//!< タイムの桁数
//#define FPS_TIME_COUNT							(4200)																	//!< 毎フレームカウントしてタイム計測
//#define TEXTURE_TIMELOGO_SIZE_X					(35)																	//!< タイムロゴテクスチャサイズX 
//#define TEXTURE_TIMELOGO_SIZE_Y					(30)																	//!< タイムロゴテクスチャサイズY  
//#define TEXTURE_TIME_POS_X						(SCREEN_W-30)															//!< タイム座標X
//#define TEXTURE_TIME_POS_Y						(25)																	//!< タイム座標Y  
//#define TEXTURE_TIMELOGO_POS_X					(TEXTURE_TIME_POS_X-110)												//!< タイムロゴ座標X 
//#define TEXTURE_TIMELOGO_POS_Y					(TEXTURE_TIME_POS_Y)													//!< タイムロゴ座標Y
//#define NUM_INTERVAL_X							(float(TEXTURE_TIME_SIZE_X))											//!< タイム数の表示間隔
//
////*****************************************************************************
////* 構造体定義
////******************************************************************************* /
///**
//*　@struct TIME
//*　@brief 2Dポリゴンを定義する構造体
//*/
//typedef struct
//{
//	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;							   //!< テクスチャへのポインタ 
//	VERTEX_2D				texture[POLYGON_2D_VERTEX];					   //!< 頂点情報格納ワーク
//	D3DXVECTOR3				pos;										   //!< ポリゴンの移動量
//	D3DXVECTOR3				rot;										   //!< 回転角
//	int						nCountAnim;									   //!< アニメカウント
//	int						nPatternAnim;								   //!< アニメパターン
//	bool					use;										   //!< 使用しているかどうか
//} TIME;
//
///**
//*　@struct DOT
//*　@brief 2Dポリゴンを定義する構造体
//*/
//typedef struct
//{
//	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;								//!< テクスチャへのポインタ
//	VERTEX_2D				texture[POLYGON_2D_VERTEX];						//!< 頂点情報格納ワーク
//	D3DXVECTOR3				pos;											//!< ポリゴンの座標
//} DOT;																		
//
///**
//*　@struct TIMELOGO
//*　@brief 2Dポリゴンを定義する構造体
//*/
//typedef struct
//{
//	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;								//!< テクスチャへのポインタ
//	VERTEX_2D				texture[POLYGON_2D_VERTEX];						//!< 頂点情報格納ワーク
//	D3DXVECTOR3				pos;											//!< ポリゴンの座標
//} TIMELOGO;
//
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
///**
//* @brief Score初期化関数 InitTime
//* @param[in] type 初期化タイプ　INIT_TIME利用。0:初回　0以外: 2回目以降
//* @return HRESULT
//*/
//HRESULT InitTime(int type);
//
///**
//* @brief Time再初期化関数 ReInitTime
//*/
//void ReInitTime(void);
//
///**
//* @brief Time開放関数 UninitTime
//*/
//void UninitTime(void);
//
///**
//* @brief Time更新関数 UpdateTime
//*/
//void UpdateTime(void);
//
///**
//* @brief Time描画関数 DrawTime
//*/
//void DrawTime(void);
//
///**
//* @brief Timemaneger取得関数 GetTimemaneger
//* @return int
//*/
//int *GetTimemaneger(void);
//
///**
//* @brief タイムを加減算する関数 AddTime
//* @param[in] val 数値
//*/
//void AddTime(int val);
//
