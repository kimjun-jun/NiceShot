/**
* @file rank.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


#include "../../h/object/objectclass.h"

class RANK : public OBJECT_2D
{
public:
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(void);				//!< �X�V
	virtual void						Draw(bool Netflag);					//!< �`��
	void								SetRank(int PlayerNum);
	void								SetRankNet(int PlayerNum, int NetMyNumber);
	HRESULT								MakeVertexRank(void);
	bool								NetUse = false;
};
