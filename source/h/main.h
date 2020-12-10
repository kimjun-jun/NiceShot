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
																														
//画像の参照名																											
#define TEXTURE_GAME_SCORE			"../data/TEXTURE/BG/0-9.png"														//!< 読み込むテクスチャファイル名
#define	TEXTURE_MEISAI				"../data/MODEL/28513607_p4_master1200.jpg"											//!< 読み込むテクスチャファイル名
#define	TEXTURE_STATUS_EMPTY_ITEM	"../data/TEXTURE/UI/UI_Skill_Empty.png"												//!< 戦車変形アイテム
#define	TEXTURE_STATUS_SENSYA_ITEM	"../data/TEXTURE/UI/UI_Skill_Power.png"												//!< 戦車変形アイテム
#define	TEXTURE_STATUS_SPEED_ITEM	"../data/TEXTURE/UI/UI_Skill_Speed.png"															//!< スピードアップアイテム
#define	TEXTURE_STATUS_CAMERA_ITEM	"../data/TEXTURE/UI/UI_Skill_Flash.png"											//!< 強制バックカメラアイテム
#define	TEXTURE_STATUS_KIRI_ITEM	"../data/TEXTURE/UI/UI_Skill_Blind.png"														//!< 霧アイテム
																													
//モデルの参照																										
#define	MODEL_HOUDAI				"../data/MODEL/PlayerSensyaHoudai.x"												//!< 読み込むモデル名
#define	MODEL_HOUTOU				"../data/MODEL/PlayerSensyaHoutou.x"												//!< 読み込むモデル名
#define	MODEL_HOUSIN				"../data/MODEL/PlayerSensyaHousin.x"												//!< 読み込むモデル名
#define	MODEL_HOUSINMO				"../data/MODEL/PlayerSensyaHousinMo.x"												//!< 読み込むモデル名
#define	MODEL_BULLETPOS				"../data/MODEL/PlayerSensyaBulletPos.x"												//!< 読み込むモデル名

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
#define PLAYER_AMMOPOWER_STRONG				(15)																		//!< プレイヤー弾薬強
#define PLAYER_AMMOPOWER_NORMAL				(10)																		//!< プレイヤー弾薬中
#define PLAYER_AMMOPOWER_WEAK				(5)																			//!< プレイヤー弾薬弱

#define	BULLETPREDICTION_MAX			(100)							// エフェクト最大数
#define DAMEGE_MAX			(4)

/**
 * @enum STATUSTYPE
 * ステータス定数
 */
enum STATUSTYPE
{
	STATUSTYPE_SPEED = 0,		//!< スピードアップ状態
	STATUSTYPE_SENSYA,			//!< 戦車強化状態
	STATUSTYPE_CAMERA,			//!< バックカメラ状態
	STATUSTYPE_KIRI,			//!< もやもや状態
	STATUSTYPE_MAX
};

//オブジェクトの合計
enum OBJECT_COUNT
{
	OBJECT_PLAYER_MAX = 4,
	OBJECT_CAMERA_MAX = OBJECT_PLAYER_MAX,
	OBJECT_TUTORIAL_MAX = OBJECT_PLAYER_MAX,
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
	OBJECT_RANK_MAX = 3,
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
#define	BULLET_EFFECT_SIZE					(4.0f)																		//!< 
#define	BULLET_EFFECT_TIME					(240)																		//!< 
#define	WALL_SIZE_X							(WALL_INIT_POSX*2)															//!< 壁のサイズX
#define	WALL_SIZE_Y							(800.0f)																	//!< 壁のサイズY
#define	EXPLOSION_SIZE						(5.0f)																		//!< 
#define	BULLET_SIZE_X						(50.0f)							// ビルボードの幅
#define	BULLET_SIZE_Y						(50.0f)							// ビルボードの高さ
#define	BULLETPREDICTION_SIZE_X			(5.0f)							// ビルボードの幅
#define	BULLETPREDICTION_SIZE_Y			(5.0f)							// ビルボードの高さ
#define	SHADOW_SIZE_X		(25.0f)							// 影の幅
#define	SHADOW_SIZE_Z		(25.0f)							// 影の高さ



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

#define MAX_LIFE							(6)																			//!< 体力最大値
#define BORN_AMMO_ADDTIME					(1.0f)																		//!< 弾薬復活させるための加算タイム
#define BORN_AMMO_MAXTIME					(120.0f)																	//!< 1弾薬復活するのに必要なタイム
#define MORPHING_TIME						(300.0f)																	//!< モーフィングアイテム有効時間
#define BACKCAMERA_TIME						(150.0f)																	//!< バックカメラアイテム有効時間
#define KIRI_TIME							(150.0f)																	//!< フォグ霧アイテムの有効時間
#define SCREENDAMEGE_TIME					(30.0f)																		//!< 被ダメージ時の画面フェード時間
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define BULLET_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define ITEM_MODEL_SIZE						(15.0f)																		//!< モデルサイズ


//エフェクト関連定数
#define	EFFECT_NORMALSET_SIZE_X				(16.0f)																		//!< エフェクト標準の幅
#define	EFFECT_NORMALSET_SIZE_Y				(16.0f)																		//!< エフェクト標準の高さ
#define	EFFECT_BULLET_SIZE_X				(16.0f)																		//!< エフェクトバレットの幅
#define	EFFECT_BULLET_SIZE_Y				(16.0f)																		//!< エフェクトバレットの高さ
#define	EFFECT_BULLET_TIME					(12)																		//!< エフェクトバレットの生存時間
#define	EFFECT_SPEEDUP_SIZE_X				(25.0f)																		//!< エフェクトスピードアップの幅
#define	EFFECT_SPEEDUP_SIZE_Y				(10.0f)																		//!< エフェクトスピードアップの高さ
#define	EFFECT_SPEEDUP_TIME					(10)																		//!< エフェクトスピードアップの生存時間

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
 * @enum MASTER_VOLUMEOL_CHENGE
 * サウンドボリュームのアップダウン種類
 */
enum MASTER_VOLUMEOL_CHENGE
{
	VOL_UP,
	VOL_DOWN,
};

/**
 * @enum RAND
 * ランダムの種類
 */
enum RAND
{
	X,
	Y,
	Z,
};

/**
 * @enum MORPHINGTYPE
 * モーフィング種類定数
 */
enum MORPHINGTYPE
{
	NoMorphing,				//!< 非モーフィング
	NowMorphing,			//!< モーフィング中
	EndMorphing				//!< モーフィング完了
};

/**
 * @enum DIRECTION
 * 移動方向
 */
enum DIRECTION
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFTUP,
	DIRECTION_RIGHTUP,
	DIRECTION_LEFTDOWN,
	DIRECTION_RIGHTDOWN,
};

/**
 * @enum PLAYER_TYPE
 * プレイヤーNo定数
 */
enum PLAYER_TYPE
{
	PLAYER01,
	PLAYER02,
	PLAYER03,
	PLAYER04
};

/**
 * @enum PLAYER_MODEL_TYPE
 * モデルタイプ定数
 */
enum PLAYER_MODEL_TYPE
{
	PLAYER_MODEL_NORMAL,
	PLAYER_MODEL_ATTACK
};

/**
 * @enum ITEMTYPE
 * アイテム定数
 */
enum ITEMTYPE
{
	ITEMTYPE_TIKEI = 0,		//!< 地形
	ITEMTYPE_LIFE,			//!< ライフ
	ITEMTYPE_SENSYA,		//!< 戦車
	ITEMTYPE_BULLET,		//!< バレット
	ITEMTYPE_SPEEDUP,		//!< スピードアップ
	ITEMTYPE_CAMERA,		//!< お邪魔アイテム　強制バックカメラ
	ITEMTYPE_KIRI,			//!< お邪魔アイテム　霧
	ITEMTYPE_MAX
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
