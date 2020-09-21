/**
* @file bulletprediction.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class BULLETPREDICTION : public OBJECT_3D
{
public:
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
} ;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBulletprediction(int type);
void UninitBulletprediction(void);
void UpdateBulletprediction(void);
void DrawBulletprediction(int CntPlayer);

int SetBulletprediction(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY);

