/**
* @file bullet.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// class��`
//*****************************************************************************
class BULLET : public OBJECT_3D
{
public:
	BULLET() { nTimer = 0, nIdxShadow = 0, UsePlayerType = 0, fRadius = 0.0f, Gravity = 0.0f, FieldPosY = 0.0f; };
	void				Init(void);					//!< ������
	void				Reinit(void);				//!< �ď�����
	void				Uninit(void);				//!< �I��
	void				Update(void);				//!< �X�V
	void				Draw(void);					//!< �`��
	int					SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, int type);
	void				ReleaseBullet(int nIdxBullet);
	int					nTimer;						// �^�C�}�[
	int					nIdxShadow;					// �eID
	int					UsePlayerType;				// ���v���C���[���g�p���Ă邩���ʂ���
	float				fRadius;					// ���a
	float				Gravity;					// �d��
	float				FieldPosY;					// �e�p�̌��ݒn�̒n�`POSY���L��
} ;


