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
	D3DXVECTOR3					bulletmove;								//!< バレット進行方向
	D3DXVECTOR3					BposStart;								//!< バレットの発射位置
	D3DXVECTOR3					BmoveRot;								//!< 弾が進行する角度
	D3DXVECTOR3					movepos;								//!< モデルの移動方向(量)
	D3DXVECTOR3					moverot;								//!< モデルの回転方向(量)
	bool						BackCameraItemSignal;					//!< バックカメラアイテムを使用しているかどうか
	bool						KiriSignal;								//!< 霧アイテムを使用しているかどうか
	bool						dashFlag;								//!< ダッシュ判定　入力がある間true.それ以外はfalse
	float						BackCameraItemTime;						//!< バックカメラアイテムの効果時間
	float						KiriItemTime;							//!< 霧アイテムの効果時間
	float						AmmoBornCnt;							//!< 残弾回復カウント。時間で回復
	int							vital;									//!< 体力
	int							AmmoNum;								//!< 残弾数。
	int							ModelType;								//!< どのモデル状態か判定
	int							RidePolygonNum;							//!< どの地形ポリゴンに乗っているか判定
	void						SetMoveABL(int CntPlayer);				//!< 移動制御(ABボタンLスティックで移動制御)
	void						SetCamera(int CntPlayer);				//!< カメラ更新
	void						SetMoveL(int CntPlayer);				//!< 移動制御(LRスティックで移動制御)
	void						SetCameraR(int CntPlayer);				//!< カメラ制御(LRスティックで移動制御)
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
D3DXVECTOR3 GetPlayerBulletStartPos(int PlayerType);
void SetPlayerMeshColor(GPUMODEL *model, int type);

