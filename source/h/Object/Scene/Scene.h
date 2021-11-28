/**
* @file Scene.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
 * @enum e_SCENE
 * ��ʑJ�ڒ萔
 */
enum e_SCENE//�񋓌^�Bdefine�̔ԍ��������Ŋ��蓖�ĂĂ����B
{
	SCENE_TITLE,					//!< 0�B1�߂�����0�B�񋓌^�̗񋓒萔���B��`�������O���V���{�����Ă����̂���ʂ炵���B
	SCENE_TUTORIAL,					//!< 1�`���[�g���A��
	SCENE_GAMECOUNTDOWN,			//!< 2�J�E���g�_�E��
	SCENE_GAME,						//!< 3�Q�[��
	SCENE_NETMATCH,					//!< 4�l�b�g�}�b�`
	SCENE_NETGAMECOUNTDOWN,			//!< 5�l�b�g�J�E���g�_�E��
	SCENE_NETGAME,					//!< 6�l�b�g�Q�[��
	SCENE_RESULT,					//!< 7���U���g
	SCENE_MAX
};

/**
 * @enum FADE_TYPE
 * �t�F�[�h�̏��
 */
enum eFADE_TYPE
{
	FADE_NONE = 0,		//!< �����Ȃ����
	FADE_IN,			//!< �t�F�[�h�C������
	FADE_MUSIC_STOP,	//!< ���y�X�g�b�v
	FADE_OUT,			//!< �t�F�[�h�A�E�g����
	FADE_MAX			//!< �}�b�N�X
};

/**
*�@@class SCENE_PARAMETER
*�@@brief
*/
class SCENE_PARAMETER
{
public:
	SCENE_PARAMETER() {	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); eFade = FADE_IN; eNowScene = SCENE_TITLE; eNextScene = SCENE_TITLE; sno = 0; stop = false;}
	~SCENE_PARAMETER() { }

	D3DXCOLOR	color;							//!< �J���[���
	eFADE_TYPE	eFade;							//!< �t�F�[�h�ԍ�
	int			sno;							//!< �T�E���h�i���o�[
	e_SCENE		eNowScene;							//!< ���݂�Scene
	e_SCENE		eNextScene;						//!< ���ɔ�ԗ\���Scene
	bool		stop;							//!< �f�o�b�O���̈ꎞ��~�p�ϐ� true=��~�@false=��~���Ȃ�
};

/**
*�@@class SCENE
*�@@brief GAMEOBJECT�h���N���X�@�V�[���؂�ւ����̃t�F�[�h���ʁ@�������^�C����ɃV�[����؂�ւ���
*/
class SCENE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	SCENE();		//!< �f�[�^�ǂݍ��݁@������
	~SCENE();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��
		 
	void NextScene(eFADE_TYPE eFade, e_SCENE next, int sno);		//!< �V�[���؂�ւ��@�V�[���؂�ւ����󂯎������t�F�[�h�����Ȃ���V�[����؂�ւ��@��{�I�ɃV�[���؂�ւ��͂��̊֐�
	void SetScene(e_SCENE next);									//!< �V�[���Z�b�g �V�[���؂�ւ����Ɏg�p�@ ��O�I�Ƀt�F�[�h���������ɐ؂�ւ��邱�Ƃ��\(countdown�Ŏg�p)

	SCENE_PARAMETER	ScenePara;			//!< �p�����[�^�[�ɕK�v�ȃf�[�^�Q

private:
	VTXBUFFER	vtx;												//!< ���_���@�����g�p����Ȃ炱����z��
};
