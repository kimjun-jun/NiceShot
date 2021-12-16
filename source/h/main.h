/**
* @file main.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS			//!< scanf ��warning�h�~

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <windows.h>
#include <d3dx9.h>
#include <tchar.h>
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

#define DIRECTINPUT_VERSION (0x0800)	//!< �x���΍�
#include <dinput.h>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	FVF_VERTEX_2D				(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)							//!< �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//!< �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
constexpr int SCREEN_W{ 1280 };																				//!< �E�C���h�E�̕�
constexpr int SCREEN_H{ 960 };																				//!< �E�C���h�E�̍���
constexpr int SCREEN_CENTER_X{ SCREEN_W / 2 };																//!< �E�C���h�E�̒��S�w���W
constexpr int SCREEN_CENTER_Y{ SCREEN_H / 2 };																//!< �E�C���h�E�̒��S�x���W
constexpr float SCREEN_SEPARATE_BUFF{ 2.5f };																//!< ��ʕ������̕`��͈͂ƕ`��͈͂̌���

//�J���}�N��
/**
 * @def SafeDelete
 * @brief �e���v���[�g�^�f���[�g�}�N��
 */
template <typename T>
inline void SafeDelete(T*& p) {
	if (p != NULL) {
		delete (p);
		(p) = NULL;
	}
}

/**
 * @def SafeDeleteArray
 * @brief SafeDeleteArray�@�e���v���[�g�^�f���[�g�}�N���@�z��
 */
template <typename T>
inline void SafeDeleteArray(T*& p) {
	if (p != NULL) {
		delete[](p);
		(p) = NULL;
	}
}

/**
 * @def SafeRelease
 * @brief SafeRelease�@�e���v���[�g�^�f���[�g�}�N���@�|�C���^�[
 */
template <typename T>
inline void SafeRelease(T*& p) {
	if (p != NULL) {
		(p)->Release();
		(p) = NULL;
	}
}

#define VEC3_ALL0	D3DXVECTOR3(0.0f,0.0f,0.0f)		//!< �p�o�������Z���@���W�������Ȃ�
#define VEC3_ALL1	D3DXVECTOR3(1.0f,1.0f,1.0f)		//!< �p�o�������Z���@�X�P�[���������Ȃ�

//2D�|���S���Ɋւ���p�����[�^																							
constexpr int	POLYGON_2D_VERTEX{ 4 };												//!< ���_��
constexpr int	POLYGON_2D_NUM{ 2 };												//!< �|���S����
																					
//�L�����N�^�[�Ȃǂ̃p�����[�^ �����t�@�C���Ŏg�p�������
constexpr int PLAYER_VITAL_MAX{ 200 };												//!< �v���C���[�̗�
constexpr int PLAYER_ATTACK_NORMAL{ 20 };											//!< �v���C���[�U���͒�
constexpr int BULLETPREDICTION_MAX{ 100 };											//!< �e���\�z�G�t�F�N�g�@100 * 4
constexpr int BULLET_MAX_STOCK{ 10 };												//!< 1�e�򕜊�����̂ɕK�v�ȃ^�C��
constexpr float MORPHING_TIME{ 300.0f };											//!< ���[�t�B���O�A�C�e���L������

//���W��T�C�Y �����t�@�C���Ŏg�p�������
constexpr float	ITEM_INIT_POSY{ 500.0f };											//!< �������W�A�C�e��
constexpr float	WALL_INIT_POSX{ 2600.0f };											//!< �������W��
constexpr float	WALL_INIT_POSZ{ 2600.0f };											//!< �������W��
constexpr float	BULLET_EFFECT_SIZE{ 4.0f };											//!< �v���C���[�Ŏg�p
constexpr int	BULLET_EFFECT_TIME{ 240 };											//!< �v���C���[�Ŏg�p
constexpr float	WALL_SIZE_X{ WALL_INIT_POSX * 2 };									//!< �ǂ̃T�C�YX
constexpr float	WALL_SIZE_Y{ 800.0f };												//!< �ǂ̃T�C�YY

//�ړ���  �����t�@�C���Ŏg�p�������
constexpr float VALUE_MOVE_BULLET{ 15.0f };											//!< �o���b�g�̑��x
constexpr float VALUE_LENTIMES_BULLET{ 2.5f };										//!< �v���C���[���S�Ƃ����o���b�g�̔��ˈʒu�ɂ�������{��
constexpr float VALUE_GRAVITYMAX_BULLET{ 10.0f };									//!< �o���b�g�̏d�͍ő�l
constexpr float VALUE_GRAVITYADD_BULLET{ 0.05f };									//!< �o���b�g�̏d�͉����x
constexpr float BULLET_MODEL_SIZE{ 15.0f };											//!< ���f���T�C�Y

//�L�����N�^�[�Ȃǂ̃p�����[�^ ����g���\�����������
constexpr int PLAYER_DEFENCE_STRONG{ 15 };											//!< �v���C���[�h��͋�
constexpr int PLAYER_DEFENCE_NORMAL{ 10 };											//!< �v���C���[�h��͒�
constexpr int PLAYER_DEFENCE_WEAK{ 5 };												//!< �v���C���[�h��͎�
constexpr int PLAYER_ATTACK_STRONG{ 30 };											//!< �v���C���[�U���͋�
constexpr int PLAYER_ATTACK_WEAK{ 10 };												//!< �v���C���[�U���͎�
constexpr float PLAYER_SPEED_STRONG{ 1.5f };										//!< �v���C���[�X�s�[�h��
constexpr float PLAYER_SPEED_NORMAL{ 1.0f };										//!< �v���C���[�X�s�[�h��
constexpr float PLAYER_SPEED_WEAK{ 0.7f };											//!< �v���C���[�X�s�[�h��

/**
 * @brief �ÓI�萔 PLAYER_COLOR�@�v���C���[�J���[�̋K��l�@�s�ϒ萔
 */
const static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),		//!< p1�J���[
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),		//!< p2�J���[
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),		//!< p3�J���[
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),		//!< p4�J���[
};

//
/**
 * @enum eOBJECT_COUNT
 * @brief �I�u�W�F�N�g�̍��v(�C���X�^���X����)
 */
enum eOBJECT_COUNT
{
	OBJECT_PLAYER_MAX = 4,
	OBJECT_CAMERA_MAX = OBJECT_PLAYER_MAX,
	OBJECT_TUTORIAL_MAX = 1,
	OBJECT_NETMATCH_MAX = 1,
	OBJECT_STATUS_MAX = OBJECT_PLAYER_MAX,
	OBJECT_BULLETPREDICTION_MAX = OBJECT_PLAYER_MAX * BULLETPREDICTION_MAX,
	OBJECT_VITAL_MAX = OBJECT_PLAYER_MAX,
	OBJECT_BULLETGAUGE_MAX = OBJECT_PLAYER_MAX,
	OBJECT_DAMEGE_MAX = 4,
	OBJECT_EFFECT_MAX = 4000,
	OBJECT_EXPLOSION_MAX = 240,
	OBJECT_BULLET_MAX = 240,
	OBJECT_ITEM_MAX = 20,
	OBJECT_SHADOW_MAX = 256,
	OBJECT_COUNTDOWN_MAX = 2,
	OBJECT_RANK_MAX = 4,
	OBJECT_RESULT_MAX = 2,
	OBJECT_TITLE_MAX = 7,
	OBJECT_TITLEANIM_MAX = 4,
	OBJECT_FIELD_MAX = 1,
	OBJECT_SKY_MAX = 2,
	OBJECT_WALL_MAX = 4,
	OBJECT_SCENE_MAX = 1,
};

/**
 * @enum eMASTER_VOLUMEOL_CHENGE
 * @brief �T�E���h�{�����[���̃A�b�v�_�E�����
 */
enum eMASTER_VOLUMEOL_CHENGE
{
	VOL_UP,		//!< �{�����[���A�b�v
	VOL_DOWN,	//!< �{�����[���_�E��
};

/**
 * @enum eRAND
 * @brief �����_���̎��
 */
enum eRAND
{
	X,		//!< X
	Y,		//!< Y
	Z,		//!< Z
};

/**
 * @enum eMORPHING_TYPE
 * @brief ���[�t�B���O��ޒ萔
 */
enum eMORPHING_TYPE
{
	NoMorphing,				//!< �񃂁[�t�B���O
	NowMorphing,			//!< ���[�t�B���O��
	EndMorphing				//!< ���[�t�B���O����
};

/**
 * @enum ePLAYER_TYPE
 * @brief �v���C���[No
 */
enum ePLAYER_TYPE
{
	PLAYER_NONE = -1,		//!< �v���C���[����
	PLAYER01,				//!< �v���C���[1
	PLAYER02,				//!< �v���C���[2
	PLAYER03,				//!< �v���C���[3
	PLAYER04,				//!< �v���C���[4
	PLAYER_MAX,				//!< �v���C���[�}�b�N�X
};

/**
 * @enum ePLAYER_MODEL_TYPE
 * @brief ���f���^�C�v�̎��
 */
enum ePLAYER_MODEL_TYPE
{
	PLAYER_MODEL_TYPE_NORMAL,	//!< �m�[�}���^�C�v�@�ʏ�
	PLAYER_MODEL_TYPE_ATTACK,	//!< attack�^�C�v�@�C��3��
	PLAYER_MODEL_TYPE_MAX,		//!< �^�C�v�}�b�N�X
};

/**
 * @enum eITEM_TYPE
 * @brief �A�C�e���̎��
 */
enum eITEM_TYPE
{
	ITEM_TYPE_NONE = -1,	//!< ���ݒ�
	ITEM_TYPE_TIKEI,		//!< �n�`
	ITEM_TYPE_LIFE,			//!< ���C�t
	ITEM_TYPE_SENSYA,		//!< ���
	ITEM_TYPE_BULLET,		//!< �o���b�g
	ITEM_TYPE_SPEEDUP,		//!< �X�s�[�h�A�b�v
	ITEM_TYPE_CAMERA,		//!< ���ז��A�C�e���@�����o�b�N�J����
	ITEM_TYPE_KIRI,			//!< ���ז��A�C�e���@��
	ITEM_TYPE_MAX
};

/**
 * @enum eSTATUS_TYPE
 * @brief �X�e�[�^�X�̎��
 */
enum eSTATUS_TYPE
{
	STATUS_TYPE_SENSYA = 0,	//!< ��ԋ������
	STATUS_TYPE_SPEED,		//!< �X�s�[�h�A�b�v���
	STATUS_TYPE_CAMERA,		//!< �o�b�N�J�������
	STATUS_TYPE_KIRI,		//!< ���������
	STATUS_TYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
/**
* @brief �f�o�C�X�擾�֐� GetDevice
* @return LPDIRECT3DDEVICE9
*/
LPDIRECT3DDEVICE9 GetDevice(void);

/**
* @brief �����Z�b�g����֐� SelectText
* @param[in] moji �\�����������������
* @details ���g�p Debug�݂̂Ŏg�p�\
*/
void SetText(char *moji);

/**
* @brief ������\������֐� DrawTextType
* @details ���g�p Debug�݂̂Ŏg�p�\
*/
void DrawTextType(void);

/**
* @brief �����Z�b�g����֐� SetTextSo
* @param[in] moji �\�����������������
* @details ���g�p Debug�݂̂Ŏg�p�\
*/
void SetTextSo(char *moji);

/**
* @brief ������\������֐� DrawTextTypeSo
* @details ���g�p Debug�݂̂Ŏg�p�\
*/
void DrawTextTypeSo(void);

/**
* @brief ���`�����@�֐� MyRandFunc
* @param[in] *X �V�[�h�l�̃A�h���X�@�C�ӂ̒l
* @param[in] Max �o�͂̍ő�l
* @return int
* @details ���색���_���֐��@���`�����@
*/
int MyRandFunc(int *X, int Max);

/**
* @brief �s�[�N���b�Z�[�W�Z�b�g�֐� SetMsg
* @param[in] msg �s�[�N���b�Z�[�WUINT
* @details �A�v���̉E��X�{�^���Ȃǂ̑��상�b�Z�[�W
*/
void SetMsg(UINT msg);

/**
* @brief �s�[�N���b�Z�[�W�Q�b�g�֐� GetMsg
* @return MSG
* @details �A�v���̉E��X�{�^���Ȃǂ̑��상�b�Z�[�W
*/
MSG GetMsg(void);

/**
* @brief �Q�[���I���t���O�Z�b�g�֐� SetEndGame
* @details true:�I���@false:�p��
*/
void SetEndGame(bool flag);

/**
* @brief �Q�[���I���t���O�Q�b�g�֐� GetEndGame
* @return bool
* @details true:�I���@false:�p��
*/
bool GetEndGame(void);

