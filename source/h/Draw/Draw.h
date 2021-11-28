/**
* @file Draw.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

//�O���錾
class GAME_OBJECT_INSTANCE;

/**
*�@@class DRAW_MANAGER
*�@@brief �I�u�W�F�N�g�̕`��Ǘ��N���X
*/
class DRAW_MANAGER
{
public:
	DRAW_MANAGER() {
		vp[0] = { DWORD(0),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f };
		vp[1] =	{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f};
		vp[2] = { DWORD(0),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f };
		vp[3] = { DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f };
		VpMaster={ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };
		DrawManagerNum = 0;};		//!< �R���X�g���N�^
	~DRAW_MANAGER() {};		//!< �f�X�g���N�^�@�@���Ɏg�p���Ȃ�

	void DrawFunc(GAME_OBJECT_INSTANCE *obj);
	inline int GetDrawManagerNum() { return DrawManagerNum; };
private:

	//�l�l���̉�ʕ���
	D3DVIEWPORT9 vp[OBJECT_PLAYER_MAX];
	D3DVIEWPORT9 VpMaster;
	int DrawManagerNum;	//�`�惋�[�v�J�E���g�@�v���C���[�̐����J�E���g����@�`�惋�[�v�����鎞�͕K���J�E���g�A�b�v�Ə������K�{

	//�V�[�����`�揈��
	void DrawSceneTitle(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneTutorial(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneCountDown(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneGame(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneResult(GAME_OBJECT_INSTANCE *obj);

	//�l�b�g�ΐ펞�V�[�����`�揈��
	void DrawSceneNetMatch(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneNetCountDown(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneNetGame(GAME_OBJECT_INSTANCE *obj);

	//�`��O���[�v����
	void DrawMap(GAME_OBJECT_INSTANCE *obj);
	void Draw3D(GAME_OBJECT_INSTANCE *obj);
	void Draw2D(GAME_OBJECT_INSTANCE *obj);

};
