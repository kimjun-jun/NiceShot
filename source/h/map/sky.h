/**
* @file sky.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

class SKY : public OBJECT_3D
{
public:
	SKY() { cnt++; time = 0.01f; Addtime = 0.0f; fRotY = 0; nNumBlockH = 0; nNumBlockV = 0; }
	~SKY() { cnt--; }

	virtual void Init(void);
	virtual void Reinit(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	static int cnt;						// 空の数
	float time;							// 空の時間　時間によって空の色が変わる
	float Addtime;						// 空の時間　時間によって空の色が変わる
	float fRotY;						// 回転量
	int nNumBlockH;
	int nNumBlockV;
};
