/**
* @file title.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

enum TITLE_HIERARCHY_NUM
{
	TITLE_HIERARCHY_NUM_ANIM,						//!< 0�A�j����
	TITLE_HIERARCHY_NUM_SELECT,				//!< 1�V�[���I��
	TITLE_HIERARCHY_NUM_MAX
};

enum TITLE_OBJECT_NAME
{
	TITLE_OBJECT_NAME_BG,					//!< 0�w�i
	TITLE_OBJECT_NAME_LOGO,					//!< 1���S
	TITLE_OBJECT_NAME_PRESS,				//!< 2PRESS
	TITLE_OBJECT_NAME_TUTO,					//!< 3�`���[�g���A��
	TITLE_OBJECT_NAME_LOCAL,				//!< 4���[�J��
	TITLE_OBJECT_NAME_NET,					//!< 5�l�b�g
	TITLE_OBJECT_NAME_QUIT,					//!< 6�I��
	TITLE_OBJECT_NAME_MAX,					//!< �ő�
};

/**
*�@@class TITLECLASS
*�@@brief 2D�|���S�����`����\����
*/
class TITLE :public OBJECT_2D_VERTEXBUFFER
{
public:
	TITLE() {
		fAlpha = 0.0f;
		HierarchyNum = 0;
		SceneNumX = 0;
		SceneNumY = 0;
		bDisp = false;
	}
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(GAME_OBJECT* obj, FADE *fade);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexTitle(void);
	void						SetColorTitle(int CntTitle, float alpha);

	float			fAlpha;							//!<
	int				HierarchyNum;					//!<
	int				SceneNumX;						//!<
	int				SceneNumY;						//!<
	bool			bDisp;							//!<
};
