/**
* @file main.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
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
constexpr int SCREEN_W{ 1280 };																				//!< ウインドウの幅
constexpr int SCREEN_H{ 960 };																				//!< ウインドウの高さ
constexpr int SCREEN_CENTER_X{ SCREEN_W / 2 };																		//!< ウインドウの中心Ｘ座標
constexpr int SCREEN_CENTER_Y{ SCREEN_H / 2 };																		//!< ウインドウの中心Ｙ座標
constexpr float SCREEN_SEPARATE_BUFF{ 2.5f };																				//!< 画面分割時の描画範囲と描画範囲の隙間

//開放マクロ
template <typename T>
inline void SafeDelete(T*& p) {
	if (p != NULL) {
		delete (p);
		(p) = NULL;
	}
}

template <typename T>
inline void SafeDeleteArray(T*& p) {
	if (p != NULL) {
		delete[](p);
		(p) = NULL;
	}
}

template <typename T>
inline void SafeRelease(T*& p) {
	if (p != NULL) {
		(p)->Release();
		(p) = NULL;
	}
}

#define VEC3_ALL0	D3DXVECTOR3(0.0f,0.0f,0.0f)		//!< 頻出文字列を短く　座標初期化など
#define VEC3_ALL1	D3DXVECTOR3(1.0f,1.0f,1.0f)		//!< 頻出文字列を短く　スケール初期化など

//2Dポリゴンに関するパラメータ																							
constexpr int	POLYGON_2D_VERTEX{ 4 };												//!< 頂点数
constexpr int	POLYGON_2D_NUM{ 2 };												//!< ポリゴン数
																					
//キャラクターなどのパラメータ 複数ファイルで使用するもの
constexpr int PLAYER_VITAL_MAX{ 200 };												//!< プレイヤー体力
constexpr int PLAYER_ATTACK_NORMAL{ 20 };											//!< プレイヤー攻撃力中
constexpr int BULLETPREDICTION_MAX{ 100 };											//!< 弾道予想エフェクト　100 * 4
constexpr int BULLET_MAX_STOCK{ 10 };												//!< 1弾薬復活するのに必要なタイム
constexpr float MORPHING_TIME{ 300.0f };											//!< モーフィングアイテム有効時間

//座標やサイズ 複数ファイルで使用するもの
constexpr float	ITEM_INIT_POSY{ 500.0f };											//!< 初期座標アイテム
constexpr float	WALL_INIT_POSX{ 2600.0f };											//!< 初期座標壁
constexpr float	WALL_INIT_POSZ{ 2600.0f };											//!< 初期座標壁
constexpr float	BULLET_EFFECT_SIZE{ 4.0f };											//!< プレイヤーで使用
constexpr int	BULLET_EFFECT_TIME{ 240 };											//!< プレイヤーで使用
constexpr float	WALL_SIZE_X{ WALL_INIT_POSX * 2 };									//!< 壁のサイズX
constexpr float	WALL_SIZE_Y{ 800.0f };												//!< 壁のサイズY

//移動量  複数ファイルで使用するもの
constexpr float VALUE_MOVE_BULLET{ 15.0f };											//!< バレットの速度
constexpr float VALUE_LENTIMES_BULLET{ 2.5f };										//!< プレイヤー中心としたバレットの発射位置にたいする倍率
constexpr float VALUE_GRAVITYMAX_BULLET{ 10.0f };									//!< バレットの重力最大値
constexpr float VALUE_GRAVITYADD_BULLET{ 0.05f };									//!< バレットの重力加速度
constexpr float BULLET_MODEL_SIZE{ 15.0f };											//!< モデルサイズ

//キャラクターなどのパラメータ 今後使う可能性があるもの
constexpr int PLAYER_DEFENCE_STRONG{ 15 };											//!< プレイヤー防御力強
constexpr int PLAYER_DEFENCE_NORMAL{ 10 };											//!< プレイヤー防御力中
constexpr int PLAYER_DEFENCE_WEAK{ 5 };												//!< プレイヤー防御力弱
constexpr int PLAYER_ATTACK_STRONG{ 30 };											//!< プレイヤー攻撃力強
constexpr int PLAYER_ATTACK_WEAK{ 10 };												//!< プレイヤー攻撃力弱
constexpr float PLAYER_SPEED_STRONG{ 1.5f };										//!< プレイヤースピード強
constexpr float PLAYER_SPEED_NORMAL{ 1.0f };										//!< プレイヤースピード中
constexpr float PLAYER_SPEED_WEAK{ 0.7f };											//!< プレイヤースピード弱

const static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4カラー
};

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
	OBJECT_EFFECT_MAX = 4000,
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
	OBJECT_SCENE_MAX = 1,
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
	STATUS_TYPE_SENSYA = 0,			//!< 戦車強化状態
	STATUS_TYPE_SPEED,		//!< スピードアップ状態
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

//線形合同法関数
int MyRandFunc(int *X, int M);

//ピークメッセージのセット
void SetMsg(UINT msg);

//アプリメッセージ関数群
MSG GetMsg(void);
void SetEndGame(bool flag);
bool GetEndGame(void);
bool GetGameLoop(void);

