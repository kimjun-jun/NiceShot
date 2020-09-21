/**
* @file explosion.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//**************************************
// 種類
//**************************************
enum
{
	EXPLOSIONTYPE_BULLET_PLAYER = 0,	// プレイヤーの弾の爆発
	EXPLOSIONTYPE_BULLET_ENEMY,			// 敵の弾の爆発
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class EXPLOSION : public OBJECT_3D
{
public:
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	int nCounter;			// カウンター
	int nPatternX;			// パターンNo.X
	int nPatternY;			// パターンNo.Y
	int nType;				// 種類
} ;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitExplosion(int type);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(int CntPlayer);

int SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType, D3DXCOLOR col);

