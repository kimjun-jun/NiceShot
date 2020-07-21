//#pragma once
///**
//* @file bullet.h
//* @brief NiceShot(3D)��ԃQ�[��
//* @author �L�����W����
//* @date 2020/01/15
//*/
//#pragma once
//
//
////*****************************************************************************
//// �\���̒�`
////*****************************************************************************
//
//#define	BULLET_PREDICTION_SIZE_X			(50.0f)							// �r���{�[�h�̕�
//#define	BULLET_PREDICTION_SIZE_Y			(50.0f)							// �r���{�[�h�̍���
//
//
//typedef struct
//{
//	D3DXVECTOR3			pos;						// �ʒu
//	D3DXVECTOR3			rot;						// ��]
//	D3DXVECTOR3			scale;						// �X�P�[��
//	D3DXVECTOR3			move;						// �ړ���
//	D3DXVECTOR3			Hormingmove;				// �z�[�~���O�ړ���
//	float				fSizeX;						// ��
//	float				fSizeY;						// ����
//	int					nTimer;						// �^�C�}�[
//	int					nIdxShadow;					// �eID
//	int					UsePlayerType;				// ���v���C���[���g�p���Ă邩���ʂ���
//	int					HormingPlayerType;			// ���v���C���[�Ƀz�[�~���O���Ă邩���ʂ���
//	float				fRadius;					// ���a
//	float				GravityAdd;					// �d�͉����x
//	float				Gravity;					// �d��
//	float				FieldPosY;					// �e�p�̌��ݒn�̒n�`POSY���L��
//	bool				HormingSignal;				// �z�[�~���O���Ă邩�ǂ����A�v���C���[�̈��͈͈ȓ��ɂȂ��true
//	bool				use;						// �g�p���Ă��邩�ǂ���
//} BULLET_PREDICTION;
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//HRESULT InitBulletPrediction(int type);
//void UninitBulletPrediction(void);
//void UpdateBulletPrediction(void);
//void DrawBulletPrediction(void);
//
//int SetBulletPrediction(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, int type);
//void ReleaseBulletPrediction(int nIdxBulletPrediction);
//BULLET_PREDICTION *GetBulletPrediction(void);
//
