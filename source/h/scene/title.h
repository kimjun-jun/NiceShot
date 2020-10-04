/**
* @file title.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

enum TITLE_SELECT_SCENE//�񋓌^�Bdefine�̔ԍ��������Ŋ��蓖�ĂĂ����B
{
	TITLE_SELECT_SCENE_TUTORIAL=2,					//!< 0�`���[�g���A��
	TITLE_SELECT_SCENE_BATTLE,						//!< 1�Ό�
	TITLE_SELECT_SCENE_LOCAL,						//!< 2���[�J���Q�[��
	TITLE_SELECT_SCENE_NET,							//!< 3�l�b�g�Q�[��
	TITLE_SELECT_SCENE_MAX
};

enum TITLE_HIERARCHY_NUM
{
	TITLE_HIERARCHY_NUM_TITLE,						//!< 0�^�C�g�����S
	TITLE_HIERARCHY_NUM_TUTO_BATTLE,				//!< 1�`���[�g���A���A�Ό�
	TITLE_HIERARCHY_NUM_LOCAL_NET,					//!< 2���[�J���A�l�b�g�I��
	TITLE_HIERARCHY_NUM_MAX
};

enum TITLE_OBJECT_NAME
{
	TITLE_OBJECT_NAME_TITLELOGO,					//!< 0�^�C�g�����S
	TITLE_OBJECT_NAME_START,						//!< 1�X�^�[�g���S
	TITLE_OBJECT_NAME_TUTO,							//!< 2�`���[�g���A��
	TITLE_OBJECT_NAME_BATTLE,						//!< 3�Ό�
	TITLE_OBJECT_NAME_LOCAL,						//!< 4���[�J��
	TITLE_OBJECT_NAME_NET,							//!< 5�l�b�g
	TITLE_OBJECT_NAME_MAX,							//!< 5�l�b�g
};

/**
*�@@class TITLECLASS 0�w�i 1�X�^�[�g���S 2���K 3����
*�@@brief 2D�|���S�����`����\����
*/
class TITLE :public OBJECT_2D_VERTEXBUFFER
{
public:
	TITLE() {
		fAlpha = 0.0f; HierarchyNum = 0; SceneNum = 0; bDisp = false;
	}
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(FADE *fade);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexTitle(void);
	void						SetColorTitle(int CntTitle, float alpha);

	float			fAlpha;							//!<
	int				HierarchyNum;					//!<
	int				SceneNum;						//!<
	bool			bDisp;							//!<
};
