/**
* @file player.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************


class PLAYER_HONTAI : public ALLCHARACTER , public ONECHARACTER, public GPUMODEL
{
public:
	PLAYER_HONTAI				*Parent;								//!< 親子判定
	bool						BackCameraItemSignal;					//!< バックカメラアイテムを使用しているかどうか
	bool						KiriSignal;								//!< 霧アイテムを使用しているかどうか
	float						BackCameraItemTime;						//!< バックカメラアイテムの効果時間
	float						KiriItemTime;							//!< 霧アイテムの効果時間
	float						AmmoBornCnt;							//!< 残弾回復カウント。時間で回復
	int							AmmoNum;								//!< 残弾数。最大5個
	int							ModelType;								//!< どのモデル状態か判定
	void						SetMoveABL(int CntPlayer);				//!< 移動制御(ABボタンLスティックで制御)
	void						SetMoveLR(int CntPlayer);				//!< 移動制御(LRスティックで制御)
	void						SetCamera(int CntPlayer);				//!< カメラ制御
	void						SetQ(int CntPlayer);					//!< クォータニオン制御
	void						SetBulletALL(int CntPlayer);			//!< バレット関連制御
	void						SetKiri(int CntPlayer);					//!< フォグ制御
	void						SetMorphing(int CntPlayer);				//!< モーフィング制御
};

class PLAYER_PRATS : public ALLCHARACTER, public GPUMODEL
{
public:
	PLAYER_HONTAI				*ParentHontai;		// 親子判定
	PLAYER_PRATS				*ParentParts;		// 親子判定
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

