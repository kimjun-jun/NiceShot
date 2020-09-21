/**
* @file countdown.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_COUNTDOWNGO			_T("../data/TEXTURE/sozai/go.png")													//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_COUNTDOWN_SIZE_X			(50)																			//!< �e�N�X�`���T�C�Y 
#define TEXTURE_COUNTDOWN_SIZE_Y			(50)																			//!< ����  
#define TEXTURE_COUNTDOWNGO_SIZE_X			(200)																			//!< �e�N�X�`�����S�T�C�Y 
#define TEXTURE_COUNTDOWNGO_SIZE_Y			(200)																			//!< ����  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X	(10)																			//!< �A�j���p�^�[���̃e�N�X�`�����������iX)  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y	(1)																				//!< �A�j���p�^�[���̃e�N�X�`�����������iY)  
#define COUNTDOWN_ANIM_PATTERN_NUM			(COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X*COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y)			//!< �A�j���[�V�����p�^�[����  
#define COUNTDOWN_TIME_ANIMATION			(4)																				//!< �A�j���[�V�����̐؂�ւ��J�E���g  
#define TEXTURE_COUNTDOWN_POS_X				(SCREEN_CENTER_X)																//!< �e�N�X�`�����W
#define TEXTURE_COUNTDOWN_POS_Y				(SCREEN_CENTER_Y)																//!< ����  
#define COUNTDOWN_GOUKEI					(2)																				//!< �J�E���g�_�E���\���̂̍��v
#define FPS_TIME_COUNTDOWN					(240)																			//!< ���t���[���J�E���g���ă^�C���v��
#define COUNTDOWN_A							(10)																			//!< �J�E���g�_�E����0�̎�GO��`�悵��GO�������Ȃ�X�s�[�h
#define COUNTDOWN_CHANGESIZE				(5)																				//!< �J�E���g�_�E�������̃T�C�Y���g�債�Ă����l

/**
*�@@struct COUNTDOWN
*�@@brief 2D�|���S�����`����\����
*/
class COUNTDOWN : public OBJECT_2D
{
public:
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	int						nCountAnim;														 //!< �A�j���J�E���g
	int						nPatternAnim;													 //!< �A�j���p�^�[��
	int						changeval;														 //!< �g�傷��ω���
	int						color;															 //!< �J���[�l(����̓A���t�@�l�Ƃ��Ďg�p)
	bool					signal;															 //!< �g�p���邩����p�t���O
} ;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
/**
* @brief Countdown�������֐� InitCountdown
* @param[in] type �������^�C�v�@INIT_TIME���p�B0:����@0�ȊO: 2��ڈȍ~
* @return HRESULT
*/
HRESULT InitCountdown(int type);

/**
* @brief Countdown�ď������֐� ReInitCountdown
* @details �Q�[�����[�v��̏������ȂǂŎg�p�B���d�Ńe�N�X�`�����̓ǂݍ��݂�h�����߂��̊֐��ŏ��������Ă���
*/
void ReInitCountdown(void);

/**
* @brief Countdown�J���֐� UninitCountdown
*/
void UninitCountdown(void);

/**
* @brief Countdown�X�V�֐� UpdateCountdown
*/
void UpdateCountdown(void);

/**
* @brief Countdown�`��֐� DrawCountdown
*/
void DrawCountdown(void);

/**
* @brief Countdown�擾�֐� GetCountdown
* @return COUNTDOWN
*/
COUNTDOWN *GetCountdown(void);

/**
* @brief Countdown�擾�֐� AddCountdown
* @param[in] val �����Z�l
*/
void AddCountdown(int val);

