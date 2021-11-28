/**
* @file countdown.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

enum COUNTDOWN_TEX_TYPE
{
	COUNTDOWN_TEX_NUMBER,
	COUNTDOWN_TEX_LOGO,
	COUNTDOWN_TEX_MAX,
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
/**
*�@@struct COUNTDOWN_PARAMETER
*�@@brief 
*/
class COUNTDOWN_PARAMETER
{
public:
	COUNTDOWN_PARAMETER() { color = 0.0f; CountAnim = 0; PatternAnim = 0; changeval = 0;  time_maneger = 0; fps_maneger = 0; CountEnd = false; }
	~COUNTDOWN_PARAMETER() {}
	float		color;				//!< �J���[�l(����̓A���t�@�l�Ƃ��Ďg�p)
	int			CountAnim;			//!< �A�j���J�E���g
	int			PatternAnim;		//!< �A�j���p�^�[��
	int			changeval;			//!< �g�傷��ω���
	int			time_maneger;		//!< �J�E���g�_�E�����l	�����t���[�����Z�����
	int			fps_maneger;		//!< �J�E���g�_�E�����Z�l	��[3,2,1]GO
	bool		CountEnd;			//!< �J�E���g�_�E���I������@���ꂪtrue�Ȃ�Q�[���J�n�V�[��
};

/**
*�@@class COUNTDOWN
*�@@brief GAMEOBJECT�h���N���X
*/
class COUNTDOWN : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	COUNTDOWN();	//!< �f�[�^�ǂݍ��݁@������
	~COUNTDOWN();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

	void AddCountdown(int val);		//!< �^�C�}�[�J�E���g�𔽉f������

private:
	TEXTURE	tex[OBJECT_COUNTDOWN_MAX];			//!< �e�N�X�`�����@�����g�p����Ȃ炱����z��
	VTXBUFFER vtx;								//!< ���_���@�����g�p����Ȃ炱����z��
	TRANSFORM Transform[OBJECT_COUNTDOWN_MAX];	//!< �g�����X�t�H�[�����
	iUseCheck iUseType[OBJECT_COUNTDOWN_MAX];	//!< �g�p���

	COUNTDOWN_PARAMETER CountdownPara;			//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q

	//------���N���X�̃A�h���X
	MySOCKET *pmysocket;
	SCENE *pscene;

	void NumberCountUpdate();	//!< �J�E���g�_�E�����l�̍X�V
	void NumberSizeUpdate();	//!< �J�E���g�_�E���̃X�P�[���X�V

} ;

