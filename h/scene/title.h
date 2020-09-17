/**
* @file title.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


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
*�@@class TITLECLASS
*�@@brief 2D�|���S�����`����\����
*/
class TITLECLASS :public TEXTURECLASS2D
{
public:
	int				nCountAppearStart = 0;				//!<
	float			fAlpha = 0.0f;						//!<
	int				nCountDisp = 0;						//!<
	bool			bDisp = false;						//!<
	int				nConutDemo = 0;						//!<
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(void);
HRESULT ReInitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void SetTitleSelectTimeCHK(bool b);
bool GetTitleSelectTimeCHK(void);

/**
* @brief �^�C�g�����̑I���V�[���擾�֐� GetSelectTitleScene
* @return int�@E_STAGE���	SCENE_TUTORIAL��1�`���[�g���A���@SCENE_GAME��3�Q�[��
* @details �^�C�g��cpp�ɂ���ϐ�SelectTitleScene���^�C�g���A�b�v�f�[�g���ɐ؂�ւ����őI���\
*/
int GetSelectTitleScene(void);
