/**
* @file countdown.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


#include "../../h/object/objectclass.h"


/**
*�@@struct COUNTDOWN
*�@@brief 2D�|���S�����`����\����
*/
class COUNTDOWN : public OBJECT_2D
{
public:
	COUNTDOWN() { nCountAnim = 0, nPatternAnim = 0, changeval = 0, color = 0, time_maneger = 0, fps_maneger = 0, signal = false; }
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(GAME_OBJECT*obj);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	void						AddCountdown(int val);
	int							nCountAnim;					//!< �A�j���J�E���g
	int							nPatternAnim;				//!< �A�j���p�^�[��
	int							changeval;					//!< �g�傷��ω���
	int							color;						//!< �J���[�l(����̓A���t�@�l�Ƃ��Ďg�p)
	int							time_maneger;				//!< �J�E���g�_�E�����l	�����t���[�����Z�����
	int							fps_maneger;				//!< �J�E���g�_�E�����Z�l	��[3,2,1]GO
	bool						signal;						//!< �g�p���邩����p�t���O
private:
	HRESULT						MakeVertexCountdown(void);
	void						SetTextureCountdown(void);
	void						SetVertexCountdown(void);
} ;

