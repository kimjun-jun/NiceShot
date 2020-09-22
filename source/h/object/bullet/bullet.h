/**
* @file bullet.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// class定義
//*****************************************************************************
class BULLET : public OBJECT_3D
{
public:
	BULLET() { nTimer = 0, nIdxShadow = 0, UsePlayerType = 0, fRadius = 0.0f, Gravity = 0.0f, FieldPosY = 0.0f; };
	void				Init(void);					//!< 初期化
	void				Reinit(void);				//!< 再初期化
	void				Uninit(void);				//!< 終了
	void				Update(void);				//!< 更新
	void				Draw(void);					//!< 描画
	int					SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, int type);
	void				ReleaseBullet(int nIdxBullet);
	int					nTimer;						// タイマー
	int					nIdxShadow;					// 影ID
	int					UsePlayerType;				// 何プレイヤーが使用してるか識別する
	float				fRadius;					// 半径
	float				Gravity;					// 重力
	float				FieldPosY;					// 影用の現在地の地形POSYを記憶
} ;


