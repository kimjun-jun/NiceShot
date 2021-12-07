/**
* @file Attack.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
*　@class PLAYER_PARAMETER_BULLET
*　@brief プレイヤーバレット用パラメータ
*/
class PLAYER_PARAMETER_BULLET
{
public:
	PLAYER_PARAMETER_BULLET() {
		BulletStartPos = VEC3_ALL0;	for (int i = 0; i < 3; i++) BulletMove[i] = VEC3_ALL0;
		BulletRotY = 0.0f; BulletBornTime = 0.0f; BulletStock = 0; NetBulletShotFlagOneFrame = 0;
	};
	~PLAYER_PARAMETER_BULLET() {}

	//------バレット制御
	//使用
	void BulletALL(PLAYER *Player, int CntPlayer, bool Netflag);	//!< バレット関連制御
	//未使用
	//void BulletALLMoveL2R2Ver(PLAYER *Player, int CntPlayer);		//!< バレット関連制御 発射ボタンを十字カーソル

	//------------------------get関数
	inline D3DXVECTOR3 GetBulletStartPos(void) { return BulletStartPos; };
	inline D3DXVECTOR3 *GetBulletMove(void) { return &BulletMove[0]; };
	inline float GetBulletRotY(void) { return BulletRotY; };
	inline float GetBulletBornTime(void) { return BulletBornTime; };
	inline int GetBulletStock(void) { return BulletStock; };
	inline int GetNetBulletShotFlagOneFrame(void) { return NetBulletShotFlagOneFrame; };
	//------------------------set関数
	inline void SetBulletStartPos(const D3DXVECTOR3 InPos) { BulletStartPos = InPos; };
	inline void SetBulletMove(const D3DXVECTOR3 InMove,int Indx) { BulletMove[Indx] = InMove; };
	inline void SetBulletRotY(const float InRotY) { BulletRotY = InRotY; };
	inline void SetBulletBornTime(const float InTime) { BulletBornTime = InTime; };
	inline void SetBulletStock(const int InStockNum) { BulletStock = InStockNum; };
	inline void SetNetBulletShotFlagOneFrame(const int InFlag) { NetBulletShotFlagOneFrame = InFlag; };

private:
	D3DXVECTOR3		BulletStartPos;				//!< バレットの発射地点
	D3DXVECTOR3		BulletMove[3];				//!< バレットの移動量　パワーアップ中は三方向に発射
	float			BulletRotY;					//!< バレットのFrontベクトルから地形法線への回転角度
	float			BulletBornTime;				//!< 残弾回復カウント。時間で回復 BULLET_BORN_MAXTIME
	int				BulletStock;				//!< 残弾数
	int				NetBulletShotFlagOneFrame;	//!< ネット対戦用　バレットを発射していなと0,通常モデルは発射1,アタックモデルは発射3　毎フレーム確認して、バレット発射するとフラグを立てる
};
