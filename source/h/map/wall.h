/**
* @file wall.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"


class WALL : public OBJECT_3D
{
public:
	WALL() { cnt++;}
	~WALL() { cnt--; }
	virtual void Init(void);
	virtual void Reinit(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	static int cnt;								// �C���X�^���X����
} ;
