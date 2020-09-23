/**
* @file countdown.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


/**
*　@struct COUNTDOWN
*　@brief 2Dポリゴンを定義する構造体
*/
class COUNTDOWN : public OBJECT_2D
{
public:
	COUNTDOWN() { nCountAnim = 0, nPatternAnim = 0, changeval = 0, color = 0, time_maneger = 0, fps_maneger = 0, signal = false; };
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	void						AddCountdown(int val);
	int							nCountAnim;					//!< アニメカウント
	int							nPatternAnim;				//!< アニメパターン
	int							changeval;					//!< 拡大する変化量
	int							color;						//!< カラー値(今回はアルファ値として使用)
	int							time_maneger;				//!< カウントダウン数値	→毎フレーム減算される
	int							fps_maneger;				//!< カウントダウン減算値	→[3,2,1]GO
	bool						signal;						//!< 使用するか判定用フラグ
private:
	HRESULT						MakeVertexCountdown(void);
	void						SetTextureCountdown(void);
	void						SetVertexCountdown(void);
} ;

