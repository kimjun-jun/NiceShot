//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************


struct PLAYER_HONTAI
{
	ALLCHARACTER				AllParameter;				// 標準パラメータ
	ONECHARACTER				OneParameter;				// 可変パラメータ
	PLAYER_HONTAI				*Parent;					// 親子判定
	GPUMODEL					Model;						// モデル構造体
	bool						BackCameraItemSignal;		// バックカメラアイテムを使用しているかどうか
	bool						KiriSignal;					// 霧アイテムを使用しているかどうか
	float						BackCameraItemTime;			// バックカメラアイテムの効果時間
	float						KiriItemTime;				// 霧アイテムの効果時間
	float						AmmoBornCnt;				// 残弾回復カウント。時間で回復
	int							AmmoNum;					// 残弾数。最大5個
	int							ModelType;					// どのモデル状態か判定
};

struct PLAYER_PRATS
{
	ALLCHARACTER				AllParameter;		// 標準パラメータ
	PLAYER_HONTAI				*ParentHontai;		// 親子判定
	PLAYER_PRATS				*ParentParts;		// 親子判定
	GPUMODEL					Model;				// モデル構造体
	bool						CameraKey;			// カメラ制御を判定,trueプレイヤーに自動追尾させるか判定,falseカメラの回転はプレイヤーが行う
	float						AmmoBornCnt;		// 残弾回復カウント。時間で回復
	int							AmmoNum;			// 残弾数。最大5個
	int							ModelType;			// どのモデル状態か判定
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
HRESULT ReInitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void UpdateEnemy(void);
void DrawPlayer(void);
PLAYER_HONTAI *GetPlayerHoudai(void);
PLAYER_PRATS *GetPlayerHoutou(void);
PLAYER_PRATS *GetPlayerHousin(void);
void SetPlayerMeshColor(GPUMODEL *model, int type);

