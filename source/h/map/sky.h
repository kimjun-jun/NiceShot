/**
* @file sky.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
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
	static int cnt;						// ��̐�
	float time;							// ��̎��ԁ@���Ԃɂ���ċ�̐F���ς��
	float Addtime;						// ��̎��ԁ@���Ԃɂ���ċ�̐F���ς��
	float fRotY;						// ��]��
	int nNumBlockH;
	int nNumBlockV;
};
