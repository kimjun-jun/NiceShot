/**
* @file bulletprediction.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// クラス定義
//*****************************************************************************
class BULLETPREDICTION : public OBJECT_3D
{
public:
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	HRESULT						MakeVertexBulletprediction(LPDIRECT3DDEVICE9 pDevice);
	void						SetVertexBulletprediction(int PlayerType, int nIdxBulletprediction, float fSizeX, float fSizeY);
	void						SetColorBulletprediction(int PlayerType, int nIdxBulletprediction, D3DXCOLOR col);
	int							SetBulletprediction(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY);
};


