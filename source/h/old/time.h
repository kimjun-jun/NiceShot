///**
//* @file time.h
//* @brief NiceShot(3D)��ԃQ�[��
//* @author �L�����W����
//* @date 2020/01/15
//*/
//#pragma once
//
//
//// �}�N����`
//#define TEXTURE_GAME_DOT						"../data/TEXTURE/BG/dot.png"												//!< �ǂݍ��ރe�N�X�`���t�@�C����
//#define TEXTURE_GAME_TIMELOGO					"../data/TEXTURE/sozai/TIME.png" 											//!< �ǂݍ��ރe�N�X�`���t�@�C����
//#define TEXTURE_TIME_SIZE_X						(30)																	//!< �e�N�X�`���T�C�YY
//#define TEXTURE_TIME_SIZE_Y						(30)																	//!< �e�N�X�`���T�C�YX
//#define SCORE_TEXTURE_PATTERN_DIVIDE_X			(10)																	//!< �A�j���p�^�[���̃e�N�X�`�����������iX)  
//#define SCORE_TEXTURE_PATTERN_DIVIDE_Y			(1)																		//!< �A�j���p�^�[���̃e�N�X�`�����������iY)  
//#define SCORE_ANIM_PATTERN_NUM					(SCORE_TEXTURE_PATTERN_DIVIDE_X*SCORE_TEXTURE_PATTERN_DIVIDE_Y)			//!< �A�j���[�V�����p�^�[����  
//#define SCORE_TIME_ANIMATION					(4)																		//!< �A�j���[�V�����̐؂�ւ��J�E���g  
//#define TEXTURE_DOT_SIZE_X						(4)																		//!< �e�N�X�`���T�C�YX
//#define TEXTURE_DOT_SIZE_Y						(4)																		//!< �e�N�X�`���T�C�YY
//#define TIME_DIGIT								(3)																		//!< �^�C���̌���
//#define FPS_TIME_COUNT							(4200)																	//!< ���t���[���J�E���g���ă^�C���v��
//#define TEXTURE_TIMELOGO_SIZE_X					(35)																	//!< �^�C�����S�e�N�X�`���T�C�YX 
//#define TEXTURE_TIMELOGO_SIZE_Y					(30)																	//!< �^�C�����S�e�N�X�`���T�C�YY  
//#define TEXTURE_TIME_POS_X						(SCREEN_W-30)															//!< �^�C�����WX
//#define TEXTURE_TIME_POS_Y						(25)																	//!< �^�C�����WY  
//#define TEXTURE_TIMELOGO_POS_X					(TEXTURE_TIME_POS_X-110)												//!< �^�C�����S���WX 
//#define TEXTURE_TIMELOGO_POS_Y					(TEXTURE_TIME_POS_Y)													//!< �^�C�����S���WY
//#define NUM_INTERVAL_X							(float(TEXTURE_TIME_SIZE_X))											//!< �^�C�����̕\���Ԋu
//
////*****************************************************************************
////* �\���̒�`
////******************************************************************************* /
///**
//*�@@struct TIME
//*�@@brief 2D�|���S�����`����\����
//*/
//typedef struct
//{
//	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;							   //!< �e�N�X�`���ւ̃|�C���^ 
//	VERTEX_2D				texture[POLYGON_2D_VERTEX];					   //!< ���_���i�[���[�N
//	D3DXVECTOR3				pos;										   //!< �|���S���̈ړ���
//	D3DXVECTOR3				rot;										   //!< ��]�p
//	int						nCountAnim;									   //!< �A�j���J�E���g
//	int						nPatternAnim;								   //!< �A�j���p�^�[��
//	bool					use;										   //!< �g�p���Ă��邩�ǂ���
//} TIME;
//
///**
//*�@@struct DOT
//*�@@brief 2D�|���S�����`����\����
//*/
//typedef struct
//{
//	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;								//!< �e�N�X�`���ւ̃|�C���^
//	VERTEX_2D				texture[POLYGON_2D_VERTEX];						//!< ���_���i�[���[�N
//	D3DXVECTOR3				pos;											//!< �|���S���̍��W
//} DOT;																		
//
///**
//*�@@struct TIMELOGO
//*�@@brief 2D�|���S�����`����\����
//*/
//typedef struct
//{
//	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;								//!< �e�N�X�`���ւ̃|�C���^
//	VERTEX_2D				texture[POLYGON_2D_VERTEX];						//!< ���_���i�[���[�N
//	D3DXVECTOR3				pos;											//!< �|���S���̍��W
//} TIMELOGO;
//
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
///**
//* @brief Score�������֐� InitTime
//* @param[in] type �������^�C�v�@INIT_TIME���p�B0:����@0�ȊO: 2��ڈȍ~
//* @return HRESULT
//*/
//HRESULT InitTime(int type);
//
///**
//* @brief Time�ď������֐� ReInitTime
//*/
//void ReInitTime(void);
//
///**
//* @brief Time�J���֐� UninitTime
//*/
//void UninitTime(void);
//
///**
//* @brief Time�X�V�֐� UpdateTime
//*/
//void UpdateTime(void);
//
///**
//* @brief Time�`��֐� DrawTime
//*/
//void DrawTime(void);
//
///**
//* @brief Timemaneger�擾�֐� GetTimemaneger
//* @return int
//*/
//int *GetTimemaneger(void);
//
///**
//* @brief �^�C���������Z����֐� AddTime
//* @param[in] val ���l
//*/
//void AddTime(int val);
//
