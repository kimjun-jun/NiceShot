//=============================================================================
//
// メイン処理 [main.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once
#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

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

#define DIRECTINPUT_VERSION (0x0800)	// 警告対策
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
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_W		(1280)				// ウインドウの幅
#define SCREEN_H		(960)				// ウインドウの高さ
#define SCREEN_CENTER_X				(SCREEN_W / 2)		// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y				(SCREEN_H / 2)		// ウインドウの中心Ｙ座標

//2Dポリゴンに関するパラメータ
#define	POLYGON_2D_VERTEX			(4)					// 頂点数
#define	POLYGON_2D_NUM				(2)					// ポリゴン数

//画像の参照名
#define TEXTURE_GAME_SCORE		"data/TEXTURE/BG/0-9.png"						// 読み込むテクスチャファイル名
#define	TEXTURE_MEISAI			"data/MODEL/28513607_p4_master1200.jpg"			// 読み込むテクスチャファイル名

#define	TEXTURE_TIKEI_ITEM			"data/MODEL/landmark_aogashima.png"			// 地形変形アイテム
#define	TEXTURE_LIFE_ITEM			"data/MODEL/life000.png"					// ライフ回復
#define	TEXTURE_SENSYA_ITEM			"data/MODEL/war_sensya_noman.png"			// 戦車変形アイテム
#define	TEXTURE_BULLET_ITEM			"data/MODEL/bullettex.png"					// バレットアイテム
#define	TEXTURE_SPEED_ITEM			"data/MODEL/1213810.png"					// スピードアップアイテム
#define	TEXTURE_CAMERA_ITEM			"data/MODEL/mark_camera_satsuei_ok.png"		// 強制バックカメラアイテム
#define	TEXTURE_KIRI_ITEM			"data/MODEL/yama_kiri.png"					// 霧アイテム


//モデルの参照
#define	MODEL_HOUDAI		"data/MODEL/PlayerSensyaHoudai.x"		// 読み込むモデル名
#define	MODEL_HOUTOU		"data/MODEL/PlayerSensyaHoutou.x"		// 読み込むモデル名
#define	MODEL_HOUSIN		"data/MODEL/PlayerSensyaHousin.x"		// 読み込むモデル名
#define	MODEL_HOUSINMO		"data/MODEL/PlayerSensyaHousinMo.x"		// 読み込むモデル名


//キャラクターなどの合計
#define PLAYER_MAX					(4)
#define BULLETPOINT_MAX				(PLAYER_MAX)

//移動量マクロ
#define VALUE_MOVE_BULLET			(6.0f)
#define	VALUE_MOVE					(0.1f)							// 移動加速度
#define	VALUE_MOVE_MAX				(4.0f)							// 移動量のMAX
#define	VALUE_ROTATE				(D3DX_PI * 0.02f)				// 回転量
#define	VALUE_ROTATE_PLAYER			(0.03f)							// プレイヤー回転量
#define MAX_SPEEDBUFF				(2.0f)
#define MAX_SPEEDBUFFTIME			(120.0f*3)

//2Dテクスチャのサイズポジションなど
#define	LIFETEX_SIZE_X				(30.0f)										// ライフの数字の幅
#define	LIFETEX_SIZE_Y				(40.0f)										// ライフの数字の高さ
#define	LIFETEX_POS_X				(SCREEN_CENTER_X-200.0f)					// ライフの表示基準位置Ｘ座標
#define	LIFETEX_POS_Y				(SCREEN_CENTER_Y-100.0f)					// ライフの表示基準位置Ｙ座標
#define	BULLETTEX_SIZE_X			(LIFETEX_SIZE_X)							// ライフの数字の幅
#define	BULLETTEX_SIZE_Y			(LIFETEX_SIZE_Y)							// ライフの数字の高さ
#define	BULLETTEX_POS_X				(LIFETEX_POS_X)								// ライフの表示基準位置Ｘ座標
#define	BULLETTEX_POS_Y				(LIFETEX_POS_Y+LIFETEX_SIZE_Y)				// ライフの表示基準位置Ｙ座標

#define	NUM_PLACE			(8)													// ライフの数
#define MAX_AMMO			(5)
#define MAX_LIFE			(6)
#define BORN_AMMO_ADDTIME	(1.0f)							
#define BORN_AMMO_MAXTIME	(120.0f)							
#define MORPHING_TIME		(300.0f)							
#define BACKCAMERA_TIME		(300.0f)
#define KIRI_TIME			(300.0f)							
#define SCREENDAMEGE_TIME	(30.0f)							
#define	MAX_SHADOW			(256)							// 影最大数

#define	BULLETNORMAL_MAX			(5)							// バレット通常持ち数
#define	BULLETKAKUSAN_MAX			(3)							// バレット拡散数
#define	BULLET_MAX					((BULLETNORMAL_MAX*BULLETKAKUSAN_MAX*PLAYER_MAX)*2)				// バレット数

#define HEADICON_MAX				(PLAYER_MAX*(PLAYER_MAX-1))

#define PLAYER_MODEL_SIZE			(15.0f)
#define BULLET_MODEL_SIZE			(15.0f)
#define ITEM_MODEL_SIZE				(15.0f)
#define DROP_ITEM_MAX				(10)
#define DROP_ITEM_CHARGE_ADDTIME	(0.1f)
#define DROP_ITEM_CHARGE_CNT		(300.0f)
#define	MAX_ITEM					(20)						// アイテム最大数

// 画面遷移定数
enum E_STAGE//列挙型。defineの番号を自動で割り当ててくれる。
{
	SCENE_TITLE,					//0。1つめだから0。列挙型の列挙定数名。定義した名前をシンボルっていうのが一般らしい。
	SCENE_SELECT,					//1人数決め
	SCENE_TUTORIAL,					//2チュートリアル
	SCENE_GAMECOUNTDOWN,			//3カウントダウン
	SCENE_GAME,						//4ゲーム
	SCENE_RESULT,					//5リザルト
	SCENE_MAX						
};

//サウンドボリュームのアップダウン種類
enum MASTER_VOLUMEOL_CHENGE
{
	VOL_UP,
	VOL_DOWN,
};

//ランダムの種類
enum RAND
{
	X,
	Y,
	Z,
};

//スコアの種類
enum SCORETYPE
{
	TIMEV,
	SCOREVAL,
};

//モーフィングの種類
enum MORPHINGTYPE
{
	NoMorphing,
	NowMorphing,
	EndMorphing
};

//移動方向
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

enum PLAYER_TYPE
{
	PLAYER01,
	PLAYER02,
	PLAYER03,
	PLAYER04
};

//モデルタイプ
enum PLAYER_MODEL_TYPE
{
	PLAYER_MODEL_NORMAL,
	PLAYER_MODEL_ATTACK
};

enum
{
	ITEMTYPE_TIKEI = 0,		// 地形
	ITEMTYPE_LIFE,			// ライフ
	ITEMTYPE_SENSYA,		// 戦車
	ITEMTYPE_BULLET,		// バレット
	ITEMTYPE_SPEEDUP,		// スピードアップ
	ITEMTYPE_CAMERA,		// お邪魔アイテム　強制バックカメラ
	ITEMTYPE_KIRI,			// お邪魔アイテム　霧
	ITEMTYPE_MAX
};

enum
{
	STATUSTYPE_SPEED=0,
	STATUSTYPE_SENSYA,
	STATUSTYPE_CAMERA,
	STATUSTYPE_KIRI,
	STATUSTYPE_MAX
};

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float		rhw;		// テクスチャのパースペクティブコレクト用
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

// 補間用のデータ構造体を定義
struct INTERPOLATION_DATA
{
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scl;		// 拡大縮小
	float		frame;		// 実行フレーム数 ( dt = 1.0f/frame )
};

// キャラクターの標準的な変数をまとめた構造体
struct ALLCHARACTER
{
	D3DXMATRIX					mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3					pos;				// モデルの位置
	D3DXVECTOR3					oldpos;				// モデルの位置
	D3DXVECTOR3					rot;				// モデルの向き(回転)
	D3DXVECTOR3					scl;				// モデルの大きさ(スケール)
	D3DXVECTOR3					move;				// モデルの移動量
};

// キャラクターの標準的な変数をまとめた構造体(階層関係でも1つだけ持ってれば良い変数)
struct ONECHARACTER
{
	D3DXMATRIX					mtxWorldRotBuff;	// ワールドマトリックス回転角度保存
	D3DXVECTOR3					Upvec;				// モデルの上方向
	D3DXVECTOR3					Frontvec;			// モデルの前方向
	D3DXVECTOR3					RotVecAxis;			// くおの時に使う地形の法線ベクトル
	D3DXVECTOR3					UpRotTOaxis;		// 地形法線とプレイヤーUpベクトルの外積値
	D3DXVECTOR3					FrontRotTOaxis;		// 地形法線とプレイヤーFrontベクトルの外積値
	D3DXQUATERNION				q;					// クオータニオン
	D3DXQUATERNION				BrotQ;				// クオータニオン
	D3DXQUATERNION				kyouyakuQ;			// 共役くお
	float						speed;				// 移動スピード
	float						speedbuff;			// 移動スピードアイテムバフ
	float						speedbufftime;		// 移動スピードアイテム効果時間
	float						Qrot;				// Upベクトルから地形法線への回転角度
	float						Brot;				// Frontベクトルから地形法線への回転角度
	float						MorphingTime;		// モーフィングできる時間
	int							shadowIdx;			// 影のインデックス番号
	int							dir;				// 進行方向
	bool						use;				// 生きてる判定
	bool						Morphing;			// 変形判定
	bool						MorphingEnd;		// 変形終了判定
	bool						speedbuffsignal;	// スピードアップバフ判定

};

//GPUを使用して描画するモデルの構造体
struct GPUMODEL
{
	LPDIRECT3DTEXTURE9			pD3DTexture;			// テクスチャへのポインタ
	LPD3DXMESH					pD3DXMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER				pD3DXBuffMat;			// マテリアル情報へのポインタ
	DWORD						nNumMat;				// マテリアル情報の数
	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;			// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9		pD3DIdxBuff;			// インデックスバッファインターフェースへのポインタ
	DWORD						nNumVertex;				// 頂点の数
	DWORD						nNumVertexIndex;		// 頂点のインデックス
	DWORD						nNumPolygon;			// ポリゴンの数
	float						time;					// モーフィング時に補間タイムとして使用
	int							MorphingSignal;			// 1,なにもしない　2,モーフィング中　3,モーフィング完了
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);		// デバイス取得関数
void DrawTextType(void);
void SetScene(int Scene);
void InitGame(void);
int GetScene(void);
float Random(int type);
DWORD GetFPS(void);
float SpdCal(D3DXVECTOR3 move);
bool RayCast(D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 *ReturnVtx);








