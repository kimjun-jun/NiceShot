/**
* @file status.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	STATUS_SIZE_X			(20.0f)							// ステータスの幅
#define	STATUS_SIZE_Y			(20.0f)							// ステータスの高さ
#define	STATUS_POS_X			(SCREEN_CENTER_X)				// ステータスの表示位置
#define	STATUS_POS_Y			(SCREEN_CENTER_Y)				// ステータスの表示位置
#define	STATUS_POS_X_OFFSET		(220.0f)						// ステータスの表示位置オフセット
#define	STATUS_POS_Y_OFFSET		(80.0f)						// ステータスの表示位置オフセット


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
