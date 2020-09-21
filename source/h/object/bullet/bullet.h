/**
* @file bullet.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

#define	BULLET_SIZE_X			(50.0f)							// �r���{�[�h�̕�
#define	BULLET_SIZE_Y			(50.0f)							// �r���{�[�h�̍���


class BULLET : public OBJECT_3D
{
public:
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	int					nTimer;						// �^�C�}�[
	int					nIdxShadow;					// �eID
	int					UsePlayerType;				// ���v���C���[���g�p���Ă邩���ʂ���
	float				fRadius;					// ���a
	float				Gravity;					// �d��
	float				FieldPosY;					// �e�p�̌��ݒn�̒n�`POSY���L��
} ;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(int type);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,  float fSizeX, float fSizeY, int nTimer, int type);
void ReleaseBullet(int nIdxBullet);
BULLET *GetBullet(void);

