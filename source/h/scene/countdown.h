/**
* @file countdown.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAME_COUNTDOWNGO			_T("../data/TEXTURE/sozai/go.png")													//!< 読み込むテクスチャファイル名
#define TEXTURE_COUNTDOWN_SIZE_X			(50)																			//!< テクスチャサイズ 
#define TEXTURE_COUNTDOWN_SIZE_Y			(50)																			//!< 同上  
#define TEXTURE_COUNTDOWNGO_SIZE_X			(200)																			//!< テクスチャロゴサイズ 
#define TEXTURE_COUNTDOWNGO_SIZE_Y			(200)																			//!< 同上  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X	(10)																			//!< アニメパターンのテクスチャ内分割数（X)  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y	(1)																				//!< アニメパターンのテクスチャ内分割数（Y)  
#define COUNTDOWN_ANIM_PATTERN_NUM			(COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X*COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y)			//!< アニメーションパターン数  
#define COUNTDOWN_TIME_ANIMATION			(4)																				//!< アニメーションの切り替わるカウント  
#define TEXTURE_COUNTDOWN_POS_X				(SCREEN_CENTER_X)																//!< テクスチャ座標
#define TEXTURE_COUNTDOWN_POS_Y				(SCREEN_CENTER_Y)																//!< 同上  
#define COUNTDOWN_GOUKEI					(2)																				//!< カウントダウン構造体の合計
#define FPS_TIME_COUNTDOWN					(240)																			//!< 毎フレームカウントしてタイム計測
#define COUNTDOWN_A							(10)																			//!< カウントダウンが0の時GOを描画してGOが薄くなるスピード
#define COUNTDOWN_CHANGESIZE				(5)																				//!< カウントダウン数字のサイズが拡大していく値

/**
*　@struct COUNTDOWN
*　@brief 2Dポリゴンを定義する構造体
*/
class COUNTDOWN : public OBJECT_2D
{
public:
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	int						nCountAnim;														 //!< アニメカウント
	int						nPatternAnim;													 //!< アニメパターン
	int						changeval;														 //!< 拡大する変化量
	int						color;															 //!< カラー値(今回はアルファ値として使用)
	bool					signal;															 //!< 使用するか判定用フラグ
} ;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief Countdown初期化関数 InitCountdown
* @param[in] type 初期化タイプ　INIT_TIME利用。0:初回　0以外: 2回目以降
* @return HRESULT
*/
HRESULT InitCountdown(int type);

/**
* @brief Countdown再初期化関数 ReInitCountdown
* @details ゲームループ後の初期化などで使用。多重でテクスチャ等の読み込みを防ぐためこの関数で初期化している
*/
void ReInitCountdown(void);

/**
* @brief Countdown開放関数 UninitCountdown
*/
void UninitCountdown(void);

/**
* @brief Countdown更新関数 UpdateCountdown
*/
void UpdateCountdown(void);

/**
* @brief Countdown描画関数 DrawCountdown
*/
void DrawCountdown(void);

/**
* @brief Countdown取得関数 GetCountdown
* @return COUNTDOWN
*/
COUNTDOWN *GetCountdown(void);

/**
* @brief Countdown取得関数 AddCountdown
* @param[in] val 加減算値
*/
void AddCountdown(int val);

