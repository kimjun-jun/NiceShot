/**
* @file main.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
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
#define	FVF_VERTEX_2D				(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)										//!< �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)							//!< �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define SCREEN_W					(1280)																				//!< �E�C���h�E�̕�
#define SCREEN_H					(960)																				//!< �E�C���h�E�̍���
#define SCREEN_CENTER_X				(SCREEN_W / 2)																		//!< �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y				(SCREEN_H / 2)																		//!< �E�C���h�E�̒��S�x���W
#define SCREEN_SEPARATE_BUFF		(2.5f)																				//!< ��ʕ������̕`��͈͂ƕ`��͈͂̌���

//�J���}�N��
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif


//2D�|���S���Ɋւ���p�����[�^																							
#define	POLYGON_2D_VERTEX			(4)																					//!< ���_��
#define	POLYGON_2D_NUM				(2)																					//!< �|���S����
																														
																													

//�L�����N�^�[�Ȃǂ̃p�����[�^
#define PLAYER_VITAL_MAX					(200)																		//!< �v���C���[�̗�
#define PLAYER_DEFENCE_STRONG				(15)																		//!< �v���C���[�h��͋�
#define PLAYER_DEFENCE_NORMAL				(10)																		//!< �v���C���[�h��͒�
#define PLAYER_DEFENCE_WEAK					(5)																			//!< �v���C���[�h��͎�
#define PLAYER_ATTACK_STRONG				(30)																		//!< �v���C���[�U���͋�
#define PLAYER_ATTACK_NORMAL				(20)																		//!< �v���C���[�U���͒�
#define PLAYER_ATTACK_WEAK					(10)																		//!< �v���C���[�U���͎�
#define PLAYER_SPEED_STRONG					(1.5f)																		//!< �v���C���[�X�s�[�h��
#define PLAYER_SPEED_NORMAL					(1.0f)																		//!< �v���C���[�X�s�[�h��
#define PLAYER_SPEED_WEAK					(0.7f)																		//!< �v���C���[�X�s�[�h��

#define	BULLETPREDICTION_MAX			(100)		// �e���\�z�G�t�F�N�g�@100 * 4

//�I�u�W�F�N�g�̍��v(�C���X�^���X����)
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
	OBJECT_EFFECT_MAX = 8000,
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
	OBJECT_FADE_MAX = 1,
};


//���f�����̏������W
#define	PLAYER_INIT_POSX					(700.0f)																	//!< �������W�v���C���[
#define	PLAYER_INIT_POSY					(300.0f)																	//!< �������W�v���C���[
#define	PLAYER_INIT_POSZ					(700.0f)																	//!< �������W�v���C���[
#define	ITEM_INIT_POSX						(700.0f)																	//!< �������W�A�C�e��
#define	ITEM_INIT_POSY						(500.0f)																	//!< �������W�A�C�e��
#define	ITEM_INIT_POSZ						(700.0f)																	//!< �������W�A�C�e��
#define	WALL_INIT_POSX						(2600.0f)																	//!< �������W��
#define	WALL_INIT_POSY						(0.0f)																		//!< �������W��
#define	WALL_INIT_POSZ						(2600.0f)																	//!< �������W��

//���f�����̃T�C�Y
#define	BULLET_EFFECT_SIZE					(4.0f)		//!< �v���C���[�Ŏg�p
#define	BULLET_EFFECT_TIME					(240)		//!< �v���C���[�Ŏg�p
#define	WALL_SIZE_X							(WALL_INIT_POSX*2)															//!< �ǂ̃T�C�YX
#define	WALL_SIZE_Y							(800.0f)																	//!< �ǂ̃T�C�YY


//�ړ��ʃ}�N��
#define	PLAYER_VALUE_DASHRATE				(2.0f)																		//!< �����ɔ�ׂẴ_�b�V���̑��x�{��
#define	PLAYER_MOVE_RATE_X					(1.0f/40000.0f)																//!< ���E�����̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
#define	PLAYER_MOVE_RATE_Y					(1.0f/200.0f)																//!< �O������̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
#define	PLAYER_MOVE_RATE_LR2				(1.0f/20000.0f)																//!< �L���^�s���̈ړ��ʔ{��(0�`65000�����͔͈͂Ȃ̂ł���ɏ�Z����)
#define VALUE_MOVE_BULLET					(15.0f)																		//!< �o���b�g�̑��x
#define VALUE_LEN_BULLET					(10.0f)																		//!< �v���C���[���S�Ƃ����o���b�g�̔��ˈʒu�܂ł̋���
#define VALUE_LENTIMES_BULLET				(2.5f)																		//!< �v���C���[���S�Ƃ����o���b�g�̔��ˈʒu�ɂ�������{��
#define VALUE_GRAVITYMAX_BULLET				(10.0f)																		//!< �o���b�g�̏d�͍ő�l
#define VALUE_GRAVITYADD_BULLET				(0.05f)																		//!< �o���b�g�̏d�͉����x
#define	VALUE_MOVE							(0.1f)																		//!< �ړ������x
#define	VALUE_MOVE_MAX						(4.0f)																		//!< �ړ������x�̍ő�l
#define	VALUE_ROTATE_PLAYER_HOUTOU			(0.03f)																		//!< �v���C���[�C����]��
#define	VALUE_ROTATE_PLAYER_HOUTOU_MAX		(1.57f)																		//!< �v���C���[�C����]�ʍő�l
#define	VALUE_ROTATE_PLAYER_HOUSIN			(0.01f)																		//!< �v���C���[�C�g��]��
#define	VALUE_ROTATE_PLAYER_HOUSIN_MAX		(0.2f)																		//!< �v���C���[�C����]�ʍő�l
#define MAX_SPEEDBUFF						(1.5f)																		//!< �X�s�[�h�o�t�ړ��ʍő�l(��)
#define MAX_SPEEDBUFFTIME					(120.0f*3)																	//!< �X�s�[�h�o�t�L������
#define	FRONT_VEC							(1)																			//!< �O�x�N�g��
#define	BACK_VEC							(-1)																		//!< ���x�N�g��
#define	MOVE_INERTIA_MOMENT					(0.8f)																		//!< �ړ��ʂ̊������[�����g�A�����W���Ƃ��Ďg�p
#define	VALUE_SPEEDBUFF_SUB					(1.0f)																		//!< �X�s�[�g�A�b�v���Ԃ̃f�N�������g����l

//2D�e�N�X�`���̃T�C�Y�|�W�V�����Ȃ�																			
#define	LIFETEX_SIZE_X						(30.0f)																		//!< ���C�t�̐����̕�
#define	LIFETEX_SIZE_Y						(40.0f)																		//!< ���C�t�̐����̍���
#define	LIFETEX_POS_X						(SCREEN_CENTER_X-200.0f)													//!< ���C�t�̕\����ʒu�w���W
#define	LIFETEX_POS_Y						(SCREEN_CENTER_Y-100.0f)													//!< ���C�t�̕\����ʒu�x���W
#define	BULLETTEX_SIZE_X					(LIFETEX_SIZE_X)															//!< ���C�t�̐����̕�
#define	BULLETTEX_SIZE_Y					(LIFETEX_SIZE_Y)															//!< ���C�t�̐����̍���
#define	BULLETTEX_POS_X						(LIFETEX_POS_X)																//!< ���C�t�̕\����ʒu�w���W
#define	BULLETTEX_POS_Y						(LIFETEX_POS_Y+LIFETEX_SIZE_Y)												//!< ���C�t�̕\����ʒu�x���W
#define	EXPLOSION_COLLISIONPOS_BUFFSIZE		(5.0f)																		//!< 2D���j��ǂɓ����������̕`�悷����W�𒲐�

#define MORPHING_TIME						(300.0f)																	//!< ���[�t�B���O�A�C�e���L������
#define BACKCAMERA_TIME						(150.0f)																	//!< �o�b�N�J�����A�C�e���L������
#define KIRI_TIME							(150.0f)																	//!< �t�H�O���A�C�e���̗L������
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< ���f���T�C�Y
#define BULLET_MODEL_SIZE					(15.0f)																		//!< ���f���T�C�Y
#define ITEM_MODEL_SIZE						(15.0f)																		//!< ���f���T�C�Y


#define VEC3_ALL0						D3DXVECTOR3(0.0f,0.0f,0.0f)		//!< �p�o�������Z��
#define VEC3_ALL1						D3DXVECTOR3(1.0f,1.0f,1.0f)		//!< �p�o�������Z��


#define BULLET_MAX_STOCK			(10)				//!< 1�e�򕜊�����̂ɕK�v�ȃ^�C��


/**
 * @enum E_STAGE
 * ��ʑJ�ڒ萔
 */
enum E_STAGE//�񋓌^�Bdefine�̔ԍ��������Ŋ��蓖�ĂĂ����B
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
 * @enum UseType
 * �g�p�^�C�v�̎��
 */
enum eUse_Type
{
	NoUse=0,
	YesUse,
	YesUseType1=1,
	YesUseType2,
	YesUseType3,
	YesUseType4,
};

/**
 * @enum MASTER_VOLUMEOL_CHENGE
 * �T�E���h�{�����[���̃A�b�v�_�E�����
 */
enum eMASTER_VOLUMEOL_CHENGE
{
	VOL_UP,
	VOL_DOWN,
};

/**
 * @enum RAND
 * �����_���̎��
 */
enum eRAND
{
	X,
	Y,
	Z,
};

/**
 * @enum MORPHINGTYPE
 * ���[�t�B���O��ޒ萔
 */
enum eMORPHING_TYPE
{
	NoMorphing,				//!< �񃂁[�t�B���O
	NowMorphing,			//!< ���[�t�B���O��
	EndMorphing				//!< ���[�t�B���O����
};

/**
 * @enum PLAYER_TYPE
 * �v���C���[No�萔
 */
enum ePLAYER_TYPE
{
	PLAYER_NONE = -1,
	PLAYER01,
	PLAYER02,
	PLAYER03,
	PLAYER04,
	PLAYER_MAX,
};

/**
 * @enum PLAYER_MODEL_TYPE
 * ���f���^�C�v�萔
 */
enum ePLAYER_MODEL_TYPE
{
	PLAYER_MODEL_TYPE_NORMAL,
	PLAYER_MODEL_TYPE_ATTACK,
	PLAYER_MODEL_TYPE_MAX,
};


/**
 * @enum PLAYER_MODEL_NUM
 * ���f���^�C�v�萔
 */
//num PLAYER_MODEL_NUM
//
//	PLAYER_MODEL_NUM_HOUDAI,
//	PLAYER_MODEL_NUM_HOUTOU,
//	PLAYER_MODEL_NUM_HOUSIN,
//	PLAYER_MODEL_NUM_HOUSIN_MORPHING,
//	PLAYER_MODEL_NUM_MAX,
//;


/**
 * @enum ITEMTYPE
 * �A�C�e���萔
 */
enum eITEM_TYPE
{
	ITEM_TYPE_NONE = -1,		//!< ���ݒ�
	ITEM_TYPE_TIKEI,			//!< �n�`
	ITEM_TYPE_LIFE,			//!< ���C�t
	ITEM_TYPE_SENSYA,		//!< ���
	ITEM_TYPE_BULLET,		//!< �o���b�g
	ITEM_TYPE_SPEEDUP,		//!< �X�s�[�h�A�b�v
	ITEM_TYPE_CAMERA,		//!< ���ז��A�C�e���@�����o�b�N�J����
	ITEM_TYPE_KIRI,			//!< ���ז��A�C�e���@��
	ITEM_TYPE_MAX
};

/**
 * @enum STATUSTYPE
 * �X�e�[�^�X�萔
 */
enum eSTATUS_TYPE
{
	STATUS_TYPE_SPEED = 0,		//!< �X�s�[�h�A�b�v���
	STATUS_TYPE_SENSYA,			//!< ��ԋ������
	STATUS_TYPE_CAMERA,			//!< �o�b�N�J�������
	STATUS_TYPE_KIRI,			//!< ���������
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

void SetTextSo(char *moji);
void DrawTextTypeSo(void);

LRESULT CALLBACK IDD_DIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK IP_DIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


bool GetEndGame(void);

class GAME_OBJECT;
GAME_OBJECT *GetSendObjectP(void);


//���`�����@�֐�
int MyRandFunc(int *X, int M);

//�s�[�N���b�Z�[�W�̃Z�b�g
void SetMsg(UINT msg);
