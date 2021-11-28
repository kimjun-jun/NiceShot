/**
* @file Update.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
*�@@class UPDATE_MANAGER
*�@@brief �I�u�W�F�N�g�̍X�V�Ǘ��N���X
*/
class UPDATE_MANAGER
{
public:
	UPDATE_MANAGER() {};	//!< �R���X�g���N�^�@���Ɏg�p���Ȃ�
	~UPDATE_MANAGER() {};	//!< �f�X�g���N�^�@�@���Ɏg�p���Ȃ�

	void UpdateFunc(GAME_OBJECT_INSTANCE *obj);
private:
	//�����蔻�菈��
	void CollisionCheck(GAME_OBJECT_INSTANCE *obj);			//kurasuwakerubeki?

	//�V�[�����`�揈��
	void UpdateSceneTitle(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneTutorial(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneCountDown(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneGame(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneResult(GAME_OBJECT_INSTANCE *obj);

	//�l�b�g�ΐ펞�V�[�����`�揈��
	void UpdateSceneNetMatch(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneNetCountDown(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneNetGame(GAME_OBJECT_INSTANCE *obj);

	//�`��O���[�v����
	void UpdateMap(GAME_OBJECT_INSTANCE *obj);
	void Update3D(GAME_OBJECT_INSTANCE *obj);
	void Update2D(GAME_OBJECT_INSTANCE *obj);

};
