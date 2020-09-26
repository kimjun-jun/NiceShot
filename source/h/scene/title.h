/**
* @file title.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
 * @enum E_STAGE
 * ��ʑJ�ڒ萔
 */
enum TITLE_SELECT_SCENE//�񋓌^�Bdefine�̔ԍ��������Ŋ��蓖�ĂĂ����B
{
	TITLE_SELECT_SCENE_TUTORIAL,					//!< 0�`���[�g���A��
	TITLE_SELECT_SCENE_GAME,						//!< 1�Q�[��
	TITLE_SELECT_SCENE_MAX
};


/**
*�@@class TITLECLASS 0�w�i 1�X�^�[�g���S 2���K 3����
*�@@brief 2D�|���S�����`����\����
*/
class TITLE :public OBJECT_2D_VERTEXBUFFER
{
public:
	TITLE() { fAlpha = 0.0f, nCountAppearStart = 0, nCountDisp = 0, nConutDemo = 0,
		TitleSelectSceneNum = 0, bDisp = false, TitleSelectTime = false; };
	virtual void						Init(void);					//!< ������
	virtual void						Reinit(void);				//!< �ď�����
	virtual void						Uninit(void);				//!< �I��
	virtual void						Update(void);				//!< �X�V
	virtual void						Draw(void);					//!< �`��
	HRESULT						MakeVertexTitle(void);
	void						SetColorTitle(int CntTitle, float alpha);

	float			fAlpha;							//!<
	int				nCountAppearStart;				//!<
	int				nCountDisp;						//!<
	int				nConutDemo;						//!<
	int				TitleSelectSceneNum;			//!<
	bool			bDisp;							//!<
	bool			TitleSelectTime;				//!<
};
