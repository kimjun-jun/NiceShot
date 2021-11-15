/**
* @file sound.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum SOUND_TRACK//ここの順番と.CPPのSOUNDPARAM g_aParam[SOUND_LABEL_MAX] =順番を一致させること。ここの名前を使って音を鳴らす
{
	SOUND_LABEL_BGM_title01,			// タイトルBGM
	SOUND_LABEL_BGM_tutorial01,			// チュートリアルBGM
	SOUND_LABEL_BGM_select01,			// セレクトBGM
	SOUND_LABEL_BGM_normal01,			// ゲーム通常時BGM
	SOUND_LABEL_BGM_boss01,				// ゲームボス戦BGM
	SOUND_LABEL_BGM_gameclear01,		// ゲームクリアBGM
	SOUND_LABEL_BGM_gameover01,			// ゲームオーバーBGM
	SOUND_LABEL_SE_attack01,			// ボールを打つSE カーン
	SOUND_LABEL_SE_attack02,			// ボールを打つSE ポーン
	SOUND_LABEL_SE_attack03,			// ボールを打つSE ポッ
	SOUND_LABEL_SE_damage,				// 打球が当たるSE
	SOUND_LABEL_SE_enter01,				// エンターを押すSE ちゃららーん
	SOUND_LABEL_SE_enter02,				// エンターを押すSE ちゅりん
	SOUND_LABEL_SE_enter03,				// エンターを押すSE くりっ
	SOUND_LABEL_SE_strat,				// 開始SE
	SOUND_LABEL_SE_swing,				// スイングSE
	SOUND_LABEL_SE_nyu,					// にゅ
	SOUND_LABEL_SE_rap1,				// ラッパ1
	SOUND_LABEL_SE_rap2,				// ラッパ2
	SOUND_LABEL_SE_speed,				// スピードアップ
	SOUND_LABEL_SE_kiri,				// 霧
	SOUND_LABEL_SE_quake,				// 地震
	SOUND_LABEL_SE_kurukuru,			// くるくる
	SOUND_LABEL_SE_parin,				// パリーン
	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);
void MasterVolumeChange(float num);
void SourceVolumeChange(float num,int sno);
