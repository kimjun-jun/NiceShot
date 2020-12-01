/**
* @file rank.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


#include "../../h/object/objectclass.h"

class RANK : public OBJECT_2D
{
public:
	virtual void						Init(void);					//!< 初期化
	virtual void						Reinit(void);				//!< 再初期化
	virtual void						Uninit(void);				//!< 終了
	virtual void						Update(void);				//!< 更新
	virtual void						Draw(bool Netflag);					//!< 描画
	void								SetRank(int PlayerNum);
	void								SetRankNet(int PlayerNum, int NetMyNumber);
	HRESULT								MakeVertexRank(void);
	bool								NetUse = false;
};
