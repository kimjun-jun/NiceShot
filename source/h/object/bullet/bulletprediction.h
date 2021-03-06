/**
* @file bulletprediction.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../../h/object/objectclass.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class BULLETPREDICTION : public OBJECT_3D
{
public:
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(PLAYER_HONTAI *player);				//!< 更新
	virtual void						Draw(PLAYER_HONTAI *player, int CntPlayer);					//!< 描画
	HRESULT						MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice);
	void						SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY);
	void						SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col);
	void						SetBulletprediction(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY, int CntBulletPrediction);
};


