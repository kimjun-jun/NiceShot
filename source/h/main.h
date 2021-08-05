/**
* @file main.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS			//!< scanf のwarning防止

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include <d3dx9.h>
#include <tchar.h>
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include <string.h>
#include <thread>
#include <mutex>

using namespace std;

#define DIRECTINPUT_VERSION (0x0800)	//!< 警告対策
#include <dinput.h>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FVF_VERTEX_2D				(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)										//!< ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)							//!< ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define SCREEN_W					(1280)																				//!< ウインドウの幅
#define SCREEN_H					(960)																				//!< ウインドウの高さ
#define SCREEN_CENTER_X				(SCREEN_W / 2)																		//!< ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y				(SCREEN_H / 2)																		//!< ウインドウの中心Ｙ座標
#define SCREEN_SEPARATE_BUFF		(2.5f)																				//!< 画面分割時の描画範囲と描画範囲の隙間

//開放マクロ
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif


//2Dポリゴンに関するパラメータ																							
#define	POLYGON_2D_VERTEX			(4)																					//!< 頂点数
#define	POLYGON_2D_NUM				(2)																					//!< ポリゴン数
																														
																													

//キャラクターなどのパラメータ
#define PLAYER_VITAL_MAX					(200)																		//!< プレイヤー体力
#define PLAYER_DEFENCE_STRONG				(15)																		//!< プレイヤー防御力強
#define PLAYER_DEFENCE_NORMAL				(10)																		//!< プレイヤー防御力中
#define PLAYER_DEFENCE_WEAK					(5)																			//!< プレイヤー防御力弱
#define PLAYER_ATTACK_STRONG				(30)																		//!< プレイヤー攻撃力強
#define PLAYER_ATTACK_NORMAL				(20)																		//!< プレイヤー攻撃力中
#define PLAYER_ATTACK_WEAK					(10)																		//!< プレイヤー攻撃力弱
#define PLAYER_SPEED_STRONG					(1.5f)																		//!< プレイヤースピード強
#define PLAYER_SPEED_NORMAL					(1.0f)																		//!< プレイヤースピード中
#define PLAYER_SPEED_WEAK					(0.7f)																		//!< プレイヤースピード弱

#define	BULLETPREDICTION_MAX			(100)		// 弾道予想エフェクト　100 * 4

//オブジェクトの合計(インスタンス総数)
enum eOBJECT_COUNT
{
	OBJECT_PLAYER_MAX = 4,
	OBJECT_CAMERA_MAX = OBJECT_PLAYER_MAX,
	OBJECT_TUTORIAL_MAX = 1,
	OBJECT_NETMATCH_MAX = 1,
	OBJECT_STATUS_MAX = OBJECT_PLAYER_MAX,
	OBJECT_BULLETPREDICTION_MAX = OBJECT_PLAYER_MAX * BULLETPREDICTION_MAX,
	OBJECT_VITAL_MAX = OBJECT_PLAYER_MAX,
	OBJECT_BULLETGAUGE_MAX = OBJECT_PLAYER_MAX,
	OBJECT_DAMEGE_MAX = 4,
	OBJECT_EFFECT_MAX = 8000,
	OBJECT_EXPLOSION_MAX = 240,
	OBJECT_BULLET_MAX = 240,
	OBJECT_ITEM_MAX = 20,
	OBJECT_SHADOW_MAX = 256,
	OBJECT_COUNTDOWN_MAX = 2,
	OBJECT_RANK_MAX = 4,
	OBJECT_RESULT_MAX = 2,
	OBJECT_TITLE_MAX = 7,
	OBJECT_TITLEANIM_MAX = 4,
	OBJECT_FIELD_MAX = 1,
	OBJECT_SKY_MAX = 2,
	OBJECT_WALL_MAX = 4,
	OBJECT_FADE_MAX = 1,
};


//モデル等の初期座標
#define	PLAYER_INIT_POSX					(700.0f)																	//!< 初期座標プレイヤー
#define	PLAYER_INIT_POSY					(300.0f)																	//!< 初期座標プレイヤー
#define	PLAYER_INIT_POSZ					(700.0f)																	//!< 初期座標プレイヤー
#define	ITEM_INIT_POSX						(700.0f)																	//!< 初期座標アイテム
#define	ITEM_INIT_POSY						(500.0f)																	//!< 初期座標アイテム
#define	ITEM_INIT_POSZ						(700.0f)																	//!< 初期座標アイテム
#define	WALL_INIT_POSX						(2600.0f)																	//!< 初期座標壁
#define	WALL_INIT_POSY						(0.0f)																		//!< 初期座標壁
#define	WALL_INIT_POSZ						(2600.0f)																	//!< 初期座標壁

//モデル等のサイズ
#define	BULLET_EFFECT_SIZE					(4.0f)		//!< プレイヤーで使用
#define	BULLET_EFFECT_TIME					(240)		//!< プレイヤーで使用
#define	WALL_SIZE_X							(WALL_INIT_POSX*2)															//!< 壁のサイズX
#define	WALL_SIZE_Y							(800.0f)																	//!< 壁のサイズY


//移動量マクロ
#define	PLAYER_VALUE_DASHRATE				(2.0f)																		//!< 歩きに比べてのダッシュの速度倍率
#define	PLAYER_MOVE_RATE_X					(1.0f/40000.0f)																//!< 左右歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
#define	PLAYER_MOVE_RATE_Y					(1.0f/200.0f)																//!< 前後歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
#define	PLAYER_MOVE_RATE_LR2				(1.0f/20000.0f)																//!< キャタピラの移動量倍率(0～65000が入力範囲なのでそれに乗算する)
#define VALUE_MOVE_BULLET					(15.0f)																		//!< バレットの速度
#define VALUE_LEN_BULLET					(10.0f)																		//!< プレイヤー中心としたバレットの発射位置までの距離
#define VALUE_LENTIMES_BULLET				(2.5f)																		//!< プレイヤー中心としたバレットの発射位置にたいする倍率
#define VALUE_GRAVITYMAX_BULLET				(10.0f)																		//!< バレットの重力最大値
#define VALUE_GRAVITYADD_BULLET				(0.05f)																		//!< バレットの重力加速度
#define	VALUE_MOVE							(0.1f)																		//!< 移動加速度
#define	VALUE_MOVE_MAX						(4.0f)																		//!< 移動加速度の最大値
#define	VALUE_ROTATE_PLAYER_HOUTOU			(0.03f)																		//!< プレイヤー砲塔回転量
#define	VALUE_ROTATE_PLAYER_HOUTOU_MAX		(1.57f)																		//!< プレイヤー砲塔回転量最大値
#define	VALUE_ROTATE_PLAYER_HOUSIN			(0.01f)																		//!< プレイヤー砲身回転量
#define	VALUE_ROTATE_PLAYER_HOUSIN_MAX		(0.2f)																		//!< プレイヤー砲塔回転量最大値
#define MAX_SPEEDBUFF						(1.5f)																		//!< スピードバフ移動量最大値(積)
#define MAX_SPEEDBUFFTIME					(120.0f*3)																	//!< スピードバフ有効時間
#define	FRONT_VEC							(1)																			//!< 前ベクトル
#define	BACK_VEC							(-1)																		//!< 後ろベクトル
#define	MOVE_INERTIA_MOMENT					(0.8f)																		//!< 移動量の慣性モーメント、減速係数として使用
#define	VALUE_SPEEDBUFF_SUB					(1.0f)																		//!< スピートアップ時間のデクリメントする値

//2Dテクスチャのサイズポジションなど																			
#define	LIFETEX_SIZE_X						(30.0f)																		//!< ライフの数字の幅
#define	LIFETEX_SIZE_Y						(40.0f)																		//!< ライフの数字の高さ
#define	LIFETEX_POS_X						(SCREEN_CENTER_X-200.0f)													//!< ライフの表示基準位置Ｘ座標
#define	LIFETEX_POS_Y						(SCREEN_CENTER_Y-100.0f)													//!< ライフの表示基準位置Ｙ座標
#define	BULLETTEX_SIZE_X					(LIFETEX_SIZE_X)															//!< ライフの数字の幅
#define	BULLETTEX_SIZE_Y					(LIFETEX_SIZE_Y)															//!< ライフの数字の高さ
#define	BULLETTEX_POS_X						(LIFETEX_POS_X)																//!< ライフの表示基準位置Ｘ座標
#define	BULLETTEX_POS_Y						(LIFETEX_POS_Y+LIFETEX_SIZE_Y)												//!< ライフの表示基準位置Ｙ座標
#define	EXPLOSION_COLLISIONPOS_BUFFSIZE		(5.0f)																		//!< 2D爆破を壁に当たった時の描画する座標を調整

#define MORPHING_TIME						(300.0f)																	//!< モーフィングアイテム有効時間
#define BACKCAMERA_TIME						(150.0f)																	//!< バックカメラアイテム有効時間
#define KIRI_TIME							(150.0f)																	//!< フォグ霧アイテムの有効時間
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define BULLET_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define ITEM_MODEL_SIZE						(15.0f)																		//!< モデルサイズ


#define VEC3_ALL0						D3DXVECTOR3(0.0f,0.0f,0.0f)		//!< 頻出文字列を短く
#define VEC3_ALL1						D3DXVECTOR3(1.0f,1.0f,1.0f)		//!< 頻出文字列を短く


#define BULLET_MAX_STOCK			(10)				//!< 1弾薬復活するのに必要なタイム


/**
 * @enum E_STAGE
 * 画面遷移定数
 */
enum E_STAGE//列挙型。defineの番号を自動で割り当ててくれる。
{
	SCENE_TITLE,					//!< 0。1つめだから0。列挙型の列挙定数名。定義した名前をシンボルっていうのが一般らしい。
	SCENE_TUTORIAL,					//!< 1チュートリアル
	SCENE_GAMECOUNTDOWN,			//!< 2カウントダウン
	SCENE_GAME,						//!< 3ゲーム
	SCENE_NETMATCH,					//!< 4ネットマッチ
	SCENE_NETGAMECOUNTDOWN,			//!< 5ネットカウントダウン
	SCENE_NETGAME,					//!< 6ネットゲーム
	SCENE_RESULT,					//!< 7リザルト
	SCENE_MAX						
};

/**
 * @enum UseType
 * 使用タイプの種類
 */
enum eUse_Type
{
	NoUse=0,
	YesUse,
	YesUseType1=1,
	YesUseType2,
	YesUseType3,
	YesUseType4,
};

/**
 * @enum MASTER_VOLUMEOL_CHENGE
 * サウンドボリュームのアップダウン種類
 */
enum eMASTER_VOLUMEOL_CHENGE
{
	VOL_UP,
	VOL_DOWN,
};

/**
 * @enum RAND
 * ランダムの種類
 */
enum eRAND
{
	X,
	Y,
	Z,
};

/**
 * @enum MORPHINGTYPE
 * モーフィング種類定数
 */
enum eMORPHING_TYPE
{
	NoMorphing,				//!< 非モーフィング
	NowMorphing,			//!< モーフィング中
	EndMorphing				//!< モーフィング完了
};

/**
 * @enum PLAYER_TYPE
 * プレイヤーNo定数
 */
enum ePLAYER_TYPE
{
	PLAYER_NONE = -1,
	PLAYER01,
	PLAYER02,
	PLAYER03,
	PLAYER04,
	PLAYER_MAX,
};

/**
 * @enum PLAYER_MODEL_TYPE
 * モデルタイプ定数
 */
enum ePLAYER_MODEL_TYPE
{
	PLAYER_MODEL_TYPE_NORMAL,
	PLAYER_MODEL_TYPE_ATTACK,
	PLAYER_MODEL_TYPE_MAX,
};


/**
 * @enum PLAYER_MODEL_NUM
 * モデルタイプ定数
 */
//num PLAYER_MODEL_NUM
//
//	PLAYER_MODEL_NUM_HOUDAI,
//	PLAYER_MODEL_NUM_HOUTOU,
//	PLAYER_MODEL_NUM_HOUSIN,
//	PLAYER_MODEL_NUM_HOUSIN_MORPHING,
//	PLAYER_MODEL_NUM_MAX,
//;


/**
 * @enum ITEMTYPE
 * アイテム定数
 */
enum eITEM_TYPE
{
	ITEM_TYPE_NONE = -1,		//!< 未設定
	ITEM_TYPE_TIKEI,			//!< 地形
	ITEM_TYPE_LIFE,			//!< ライフ
	ITEM_TYPE_SENSYA,		//!< 戦車
	ITEM_TYPE_BULLET,		//!< バレット
	ITEM_TYPE_SPEEDUP,		//!< スピードアップ
	ITEM_TYPE_CAMERA,		//!< お邪魔アイテム　強制バックカメラ
	ITEM_TYPE_KIRI,			//!< お邪魔アイテム　霧
	ITEM_TYPE_MAX
};

/**
 * @enum STATUSTYPE
 * ステータス定数
 */
enum eSTATUS_TYPE
{
	STATUS_TYPE_SPEED = 0,		//!< スピードアップ状態
	STATUS_TYPE_SENSYA,			//!< 戦車強化状態
	STATUS_TYPE_CAMERA,			//!< バックカメラ状態
	STATUS_TYPE_KIRI,			//!< もやもや状態
	STATUS_TYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief デバイス取得関数 GetDevice
* @return LPDIRECT3DDEVICE9
*/
LPDIRECT3DDEVICE9 GetDevice(void);

/**
* @brief 文字セットする関数 SelectText
* @param[in] moji 表示したい文字を入力
* @details 未使用 Debugのみで使用可能
*/
void SetText(char *moji);

/**
* @brief 文字を表示する関数 DrawTextType
* @details 未使用 Debugのみで使用可能
*/
void DrawTextType(void);

void SetTextSo(char *moji);
void DrawTextTypeSo(void);

LRESULT CALLBACK IDD_DIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK IP_DIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


bool GetEndGame(void);

class GAME_OBJECT;
GAME_OBJECT *GetSendObjectP(void);


//線形合同法関数
int MyRandFunc(int *X, int M);

//ピークメッセージのセット
void SetMsg(UINT msg);
