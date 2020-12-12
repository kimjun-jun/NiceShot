/**
* @file status.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

class OBJECT_2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class STATUS : public OBJECT_2D
{
public:
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						ReinitNet(int MyNumber);			//!< �ď������l�b�g�ΐ�O	
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(PLAYER_HONTAI *p);				//!< �X�V
	virtual void						Draw(bool Netflag, int NetMyNumber,int CntPlayer);					//!< �`��
	HRESULT								MakeVertexStatus(void);
	TEXTURE_2D							Tex[OBJECT_STATUS_MAX];
	TEXTURE_2D							TexEmpty[OBJECT_STATUS_MAX];
	bool								use[OBJECT_STATUS_MAX];
	int									usebuff[OBJECT_STATUS_MAX];
};
