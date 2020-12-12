/**
* @file status.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

class OBJECT_2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class STATUS : public OBJECT_2D
{
public:
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						ReinitNet(int MyNumber);			//!< 再初期化ネット対戦前	
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(PLAYER_HONTAI *p);				//!< 更新
	virtual void						Draw(bool Netflag, int NetMyNumber,int CntPlayer);					//!< 描画
	HRESULT								MakeVertexStatus(void);
	TEXTURE_2D							Tex[OBJECT_STATUS_MAX];
	TEXTURE_2D							TexEmpty[OBJECT_STATUS_MAX];
	bool								use[OBJECT_STATUS_MAX];
	int									usebuff[OBJECT_STATUS_MAX];
};
