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
#define	FVF_VERTEX_2D				(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)								//!< ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//!< ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define SCREEN_W					(1280)																		//!< ウインドウの幅
#define SCREEN_H					(960)																		//!< ウインドウの高さ
#define SCREEN_CENTER_X				(SCREEN_W / 2)																//!< ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y				(SCREEN_H / 2)																//!< ウインドウの中心Ｙ座標
#define SCREEN_SEPARATE_BUFF		(2.5f)																		//!< 画面分割時の描画範囲と描画範囲の隙間

//2Dポリゴンに関するパラメータ																					
#define	POLYGON_2D_VERTEX			(4)																			//!< 頂点数
#define	POLYGON_2D_NUM				(2)																			//!< ポリゴン数
																												
//画像の参照名																									
#define TEXTURE_GAME_SCORE			"../data/TEXTURE/BG/0-9.png"													//!< 読み込むテクスチャファイル名
#define	TEXTURE_MEISAI				"../data/MODEL/28513607_p4_master1200.jpg"										//!< 読み込むテクスチャファイル名
#define	TEXTURE_STATUS_TIKEI_ITEM	"../data/MODEL/landmark_aogashima.png"											//!< 地形変形アイテム
#define	TEXTURE_STATUS_LIFE_ITEM	"../data/MODEL/life000.png"													//!< ライフ回復
#define	TEXTURE_STATUS_SENSYA_ITEM	"../data/MODEL/war_sensya_noman.png"											//!< 戦車変形アイテム
#define	TEXTURE_STATUS_BULLET_ITEM	"../data/MODEL/bullettex.png"													//!< バレットアイテム
#define	TEXTURE_STATUS_SPEED_ITEM	"../data/MODEL/1213810.png"													//!< スピードアップアイテム
#define	TEXTURE_STATUS_CAMERA_ITEM	"../data/MODEL/mark_camera_satsuei_ok.png"										//!< 強制バックカメラアイテム
#define	TEXTURE_STATUS_KIRI_ITEM	"../data/MODEL/yama_kiri.png"													//!< 霧アイテム
																												
//モデルの参照																									
#define	MODEL_HOUDAI				"../data/MODEL/PlayerSensyaHoudai.x"											//!< 読み込むモデル名
#define	MODEL_HOUTOU				"../data/MODEL/PlayerSensyaHoutou.x"											//!< 読み込むモデル名
#define	MODEL_HOUSIN				"../data/MODEL/PlayerSensyaHousin.x"											//!< 読み込むモデル名
#define	MODEL_HOUSINMO				"../data/MODEL/PlayerSensyaHousinMo.x"											//!< 読み込むモデル名
#define	MODEL_BULLETPOS				"../data/MODEL/PlayerSensyaBulletPos.x"										//!< 読み込むモデル名

//キャラクターなどの合計
#define PLAYER_MAX							(4)																			//!< プレイヤー最大人数
#define BULLETPOINT_MAX						(PLAYER_MAX)																//!< 着地点の最大数

//移動量マクロ
#define	PLAYER_VALUE_DASHRATE			(2.0f)											//歩きに比べてのダッシュの速度倍率
#define	PLAYER_MOVE_RATE_X				(1.0f/40000.0f)									//左右歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
#define	PLAYER_MOVE_RATE_Y				(1.0f/300.0f)									//前後歩きの移動量倍率(-1000～1000が入力範囲なのでそれに乗算する)
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

#define MAX_AMMO							(5)																			//!< 所持弾薬の最大値
#define MAX_LIFE							(6)																			//!< 体力最大値
#define BORN_AMMO_ADDTIME					(1.0f)																		//!< 弾薬復活させるための加算タイム
#define BORN_AMMO_MAXTIME					(120.0f)																	//!< 1弾薬復活するのに必要なタイム
#define MORPHING_TIME						(300.0f)																	//!< モーフィングアイテム有効時間
#define BACKCAMERA_TIME						(150.0f)																	//!< バックカメラアイテム有効時間
#define KIRI_TIME							(150.0f)																	//!< フォグ霧アイテムの有効時間
#define SCREENDAMEGE_TIME					(30.0f)																		//!< 被ダメージ時の画面フェード時間
#define	MAX_SHADOW							(256)																		//!< 影最大数
#define	BULLETKAKUSAN_MAX					(3)																			//!< バレット拡散数
#define	BULLET_MAX							((MAX_AMMO*BULLETKAKUSAN_MAX*PLAYER_MAX)*2)									//!< 合計のバレットワーク数
#define	BULLET_PREDICTION_MAX				(4)																			//!< バレット予測線の数
#define HEADICON_MAX						(PLAYER_MAX*(PLAYER_MAX-1))													//!< 敵の頭上アイコン最大数
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define BULLET_MODEL_SIZE					(15.0f)																		//!< モデルサイズ
#define ITEM_MODEL_SIZE						(15.0f)																		//!< モデルサイズ
#define DROP_ITEM_MAX						(10)																		//!< フィールドに落ちてるアイテムの数
#define DROP_ITEM_CHARGE_ADDTIME			(1.0f)																		//!< アイテムをリスポーンさせる時の加算タイム
#define DROP_ITEM_CHARGE_CNT				(60.0f)																		//!< アイテムをリスポーンさせる時の所要タイム
#define	MAX_ITEM							(20)																		//!< アイテムワーク最大数


//エフェクト関連定数
#define	EFFECT_NORMALSET_SIZE_X				(16.0f)																		//!< エフェクト標準の幅
#define	EFFECT_NORMALSET_SIZE_Y				(16.0f)																		//!< エフェクト標準の高さ
#define	EFFECT_BULLET_SIZE_X				(16.0f)																		//!< エフェクトバレットの幅
#define	EFFECT_BULLET_SIZE_Y				(16.0f)																		//!< エフェクトバレットの高さ
#define	EFFECT_BULLET_TIME					(10)																		//!< エフェクトバレットの生存時間
#define	EFFECT_SPEEDUP_SIZE_X				(25.0f)																		//!< エフェクトスピードアップの幅
#define	EFFECT_SPEEDUP_SIZE_Y				(10.0f)																		//!< エフェクトスピードアップの高さ
#define	EFFECT_SPEEDUP_TIME					(10)																			//!< エフェクトスピードアップの生存時間

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
	SCENE_RESULT,					//!< 4リザルト
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

/**
 * @class VERTEX_2D
 * ポリゴン頂点フォーマットに合わせたCLASSを定義
 */
class VERTEX_2D
{
public:
	D3DXVECTOR3 vtx;		//!< 頂点座標
	float		rhw;		//!< テクスチャのパースペクティブコレクト用
	D3DCOLOR	diffuse;	//!< 反射光
	D3DXVECTOR2 tex;		//!< テクスチャ座標
};

/**
 * @class VERTEX_3D
 * ３Ｄポリゴン頂点フォーマットに合わせたCLASSを定義
 */

class VERTEX_3D
{
public:
	D3DXVECTOR3 vtx;		//!< 頂点座標
	D3DXVECTOR3 nor;		//!< 法線ベクトル
	D3DCOLOR	diffuse;	//!< 反射光
	D3DXVECTOR2 tex;		//!< テクスチャ座標
};

/**
 * @class INTERPOLATION_DATA
 * 補間用のデータCLASSを定義
 */
class INTERPOLATION_DATA
{
public:
	D3DXVECTOR3 pos;		//!< 頂点座標
	D3DXVECTOR3 rot;		//!< 回転
	D3DXVECTOR3 scl;		//!< 拡大縮小
	float		frame;		//!< 実行フレーム数 ( dt = 1.0f/frame )
};

/**
 * @class ALLCHARACTER
 * キャラクターの標準的な変数をまとめたCLASS
 */
class ALLCHARACTER
{
public:
	D3DXMATRIX					mtxWorld;			//!< ワールドマトリックス
	D3DXVECTOR3					pos;				//!< モデルの位置
	D3DXVECTOR3					oldpos;				//!< モデルの位置
	D3DXVECTOR3					rot;				//!< モデルの向き(回転)
	D3DXVECTOR3					scl;				//!< モデルの大きさ(スケール)
	D3DXVECTOR3					move;				//!< モデルの移動量
};

/**
 * @class ONECHARACTER
 * キャラクターの標準的な変数をまとめたCLASS(階層関係で1つだけ持ってれば良い変数)
 */
class ONECHARACTER
{
public:
	D3DXMATRIX					mtxWorldRotBuff;	//!< ワールドマトリックス回転角度保存
	D3DXVECTOR3					Upvec;				//!< モデルの上方向
	D3DXVECTOR3					Frontvec;			//!< モデルの前方向
	D3DXVECTOR3					RotVecAxis;			//!< クオータニオンの時に使う地形の法線ベクトル
	D3DXVECTOR3					UpFieldNorVec;		//!< 姿勢の球面線形補間時に使う地形の法線ベクトル　プレイヤーが乗っているポリゴンの一つ上のポリゴン
	D3DXVECTOR3					RightFieldNorVec;	//!< 姿勢の球面線形補間時に使う地形の法線ベクトル　プレイヤーが乗っているポリゴンの一つ右のポリゴン
	D3DXVECTOR3					LeftFieldNorVec;	//!< 姿勢の球面線形補間時に使う地形の法線ベクトル　プレイヤーが乗っているポリゴンの一つ左のポリゴン
	D3DXVECTOR3					DownFieldNorVec;	//!< 姿勢の球面線形補間時に使う地形の法線ベクトル　プレイヤーが乗っているポリゴンの一つ下のポリゴン
	D3DXVECTOR3					UpRotTOaxis;		//!< 地形法線とプレイヤーUpベクトルの外積値
	D3DXVECTOR3					FrontRotTOaxis;		//!< 地形法線とプレイヤーFrontベクトルの外積値
	D3DXQUATERNION				q;					//!< プレイヤー用クオータニオン
	float						speed;				//!< 移動スピード
	float						speedbuff;			//!< 移動スピードアイテムバフ
	float						speedbufftime;		//!< 移動スピードアイテム効果時間
	float						Qrot;				//!< Upベクトルから地形法線への回転角度
	float						Brot;				//!< Frontベクトルから地形法線への回転角度
	float						MorphingTime;		//!< モーフィングできる時間
	int							shadowIdx;			//!< 影のインデックス番号
	int							dir;				//!< 進行方向
	bool						use;				//!< 生きてる判定
	bool						Morphing;			//!< 変形判定
	bool						MorphingEnd;		//!< 変形終了判定
	bool						speedbuffsignal;	//!< スピードアップバフ判定
};

/**
 * @class GPUMODEL
 * モデルの頂点データを使用　モーフィング実行時に元データと変化データの頂点情報を保存
 */
class GPUMODEL
{
public:
	LPDIRECT3DTEXTURE9			pD3DTexture;			//!< テクスチャへのポインタ
	LPD3DXMESH					pD3DXMesh;				//!< メッシュ情報へのポインタ
	LPD3DXBUFFER				pD3DXBuffMat;			//!< マテリアル情報へのポインタ
	DWORD						nNumMat;				//!< マテリアル情報の数
	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;			//!< 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9		pD3DIdxBuff;			//!< インデックスバッファインターフェースへのポインタ
	DWORD						nNumVertex;				//!< 頂点の数
	DWORD						nNumVertexIndex;		//!< 頂点のインデックス
	DWORD						nNumPolygon;			//!< ポリゴンの数
	float						time;					//!< モーフィング時に補間タイムとして使用
	int							MorphingSignal;			//!< 1,なにもしない　2,モーフィング中　3,モーフィング完了
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

/**
* @brief ゲームシーンをセットする関数 SetScene
* @param[in] Scene 移動先シーンを入力
* @details ゲームシーンはE_STAGEを参照
*/
void SetScene(int Scene);

/**
* @brief 現在のゲームシーンを入手する関数 GetScene
* @return int 現在のゲームシーンを返す
* @details ゲームシーンはE_STAGEを参照
*/
int GetScene(void);

/**
* @brief ゲームを初期化する関数 InitGame
* @details ゲーム起動時に使用ゲームループ時に使用
*/
void InitGame(void);



