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
#define	FVF_VERTEX_2D				(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)								//!< �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//!< �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define SCREEN_W					(1280)																		//!< �E�C���h�E�̕�
#define SCREEN_H					(960)																		//!< �E�C���h�E�̍���
#define SCREEN_CENTER_X				(SCREEN_W / 2)																//!< �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y				(SCREEN_H / 2)																//!< �E�C���h�E�̒��S�x���W
#define SCREEN_SEPARATE_BUFF		(2.5f)																		//!< ��ʕ������̕`��͈͂ƕ`��͈͂̌���

//2D�|���S���Ɋւ���p�����[�^																					
#define	POLYGON_2D_VERTEX			(4)																			//!< ���_��
#define	POLYGON_2D_NUM				(2)																			//!< �|���S����
																												
//�摜�̎Q�Ɩ�																									
#define TEXTURE_GAME_SCORE			"../data/TEXTURE/BG/0-9.png"													//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_MEISAI				"../data/MODEL/28513607_p4_master1200.jpg"										//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_STATUS_TIKEI_ITEM	"../data/MODEL/landmark_aogashima.png"											//!< �n�`�ό`�A�C�e��
#define	TEXTURE_STATUS_LIFE_ITEM	"../data/MODEL/life000.png"													//!< ���C�t��
#define	TEXTURE_STATUS_SENSYA_ITEM	"../data/MODEL/war_sensya_noman.png"											//!< ��ԕό`�A�C�e��
#define	TEXTURE_STATUS_BULLET_ITEM	"../data/MODEL/bullettex.png"													//!< �o���b�g�A�C�e��
#define	TEXTURE_STATUS_SPEED_ITEM	"../data/MODEL/1213810.png"													//!< �X�s�[�h�A�b�v�A�C�e��
#define	TEXTURE_STATUS_CAMERA_ITEM	"../data/MODEL/mark_camera_satsuei_ok.png"										//!< �����o�b�N�J�����A�C�e��
#define	TEXTURE_STATUS_KIRI_ITEM	"../data/MODEL/yama_kiri.png"													//!< ���A�C�e��
																												
//���f���̎Q��																									
#define	MODEL_HOUDAI				"../data/MODEL/PlayerSensyaHoudai.x"											//!< �ǂݍ��ރ��f����
#define	MODEL_HOUTOU				"../data/MODEL/PlayerSensyaHoutou.x"											//!< �ǂݍ��ރ��f����
#define	MODEL_HOUSIN				"../data/MODEL/PlayerSensyaHousin.x"											//!< �ǂݍ��ރ��f����
#define	MODEL_HOUSINMO				"../data/MODEL/PlayerSensyaHousinMo.x"											//!< �ǂݍ��ރ��f����
#define	MODEL_BULLETPOS				"../data/MODEL/PlayerSensyaBulletPos.x"										//!< �ǂݍ��ރ��f����

//�L�����N�^�[�Ȃǂ̍��v
#define PLAYER_MAX							(4)																			//!< �v���C���[�ő�l��
#define BULLETPOINT_MAX						(PLAYER_MAX)																//!< ���n�_�̍ő吔

//�ړ��ʃ}�N��
#define	PLAYER_VALUE_DASHRATE			(2.0f)											//�����ɔ�ׂẴ_�b�V���̑��x�{��
#define	PLAYER_MOVE_RATE_X				(1.0f/40000.0f)									//���E�����̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
#define	PLAYER_MOVE_RATE_Y				(1.0f/300.0f)									//�O������̈ړ��ʔ{��(-1000�`1000�����͔͈͂Ȃ̂ł���ɏ�Z����)
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

#define MAX_AMMO							(5)																			//!< �����e��̍ő�l
#define MAX_LIFE							(6)																			//!< �̗͍ő�l
#define BORN_AMMO_ADDTIME					(1.0f)																		//!< �e�򕜊������邽�߂̉��Z�^�C��
#define BORN_AMMO_MAXTIME					(120.0f)																	//!< 1�e�򕜊�����̂ɕK�v�ȃ^�C��
#define MORPHING_TIME						(300.0f)																	//!< ���[�t�B���O�A�C�e���L������
#define BACKCAMERA_TIME						(150.0f)																	//!< �o�b�N�J�����A�C�e���L������
#define KIRI_TIME							(150.0f)																	//!< �t�H�O���A�C�e���̗L������
#define SCREENDAMEGE_TIME					(30.0f)																		//!< ��_���[�W���̉�ʃt�F�[�h����
#define	MAX_SHADOW							(256)																		//!< �e�ő吔
#define	BULLETKAKUSAN_MAX					(3)																			//!< �o���b�g�g�U��
#define	BULLET_MAX							((MAX_AMMO*BULLETKAKUSAN_MAX*PLAYER_MAX)*2)									//!< ���v�̃o���b�g���[�N��
#define	BULLET_PREDICTION_MAX				(4)																			//!< �o���b�g�\�����̐�
#define HEADICON_MAX						(PLAYER_MAX*(PLAYER_MAX-1))													//!< �G�̓���A�C�R���ő吔
#define PLAYER_MODEL_SIZE					(15.0f)																		//!< ���f���T�C�Y
#define BULLET_MODEL_SIZE					(15.0f)																		//!< ���f���T�C�Y
#define ITEM_MODEL_SIZE						(15.0f)																		//!< ���f���T�C�Y
#define DROP_ITEM_MAX						(10)																		//!< �t�B�[���h�ɗ����Ă�A�C�e���̐�
#define DROP_ITEM_CHARGE_ADDTIME			(1.0f)																		//!< �A�C�e�������X�|�[�������鎞�̉��Z�^�C��
#define DROP_ITEM_CHARGE_CNT				(60.0f)																		//!< �A�C�e�������X�|�[�������鎞�̏��v�^�C��
#define	MAX_ITEM							(20)																		//!< �A�C�e�����[�N�ő吔


//�G�t�F�N�g�֘A�萔
#define	EFFECT_NORMALSET_SIZE_X				(16.0f)																		//!< �G�t�F�N�g�W���̕�
#define	EFFECT_NORMALSET_SIZE_Y				(16.0f)																		//!< �G�t�F�N�g�W���̍���
#define	EFFECT_BULLET_SIZE_X				(16.0f)																		//!< �G�t�F�N�g�o���b�g�̕�
#define	EFFECT_BULLET_SIZE_Y				(16.0f)																		//!< �G�t�F�N�g�o���b�g�̍���
#define	EFFECT_BULLET_TIME					(10)																		//!< �G�t�F�N�g�o���b�g�̐�������
#define	EFFECT_SPEEDUP_SIZE_X				(25.0f)																		//!< �G�t�F�N�g�X�s�[�h�A�b�v�̕�
#define	EFFECT_SPEEDUP_SIZE_Y				(10.0f)																		//!< �G�t�F�N�g�X�s�[�h�A�b�v�̍���
#define	EFFECT_SPEEDUP_TIME					(10)																			//!< �G�t�F�N�g�X�s�[�h�A�b�v�̐�������

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
	SCENE_RESULT,					//!< 4���U���g
	SCENE_MAX						
};

/**
 * @enum MASTER_VOLUMEOL_CHENGE
 * �T�E���h�{�����[���̃A�b�v�_�E�����
 */
enum MASTER_VOLUMEOL_CHENGE
{
	VOL_UP,
	VOL_DOWN,
};

/**
 * @enum RAND
 * �����_���̎��
 */
enum RAND
{
	X,
	Y,
	Z,
};

/**
 * @enum MORPHINGTYPE
 * ���[�t�B���O��ޒ萔
 */
enum MORPHINGTYPE
{
	NoMorphing,				//!< �񃂁[�t�B���O
	NowMorphing,			//!< ���[�t�B���O��
	EndMorphing				//!< ���[�t�B���O����
};

/**
 * @enum DIRECTION
 * �ړ�����
 */
enum DIRECTION
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFTUP,
	DIRECTION_RIGHTUP,
	DIRECTION_LEFTDOWN,
	DIRECTION_RIGHTDOWN,
};

/**
 * @enum PLAYER_TYPE
 * �v���C���[No�萔
 */
enum PLAYER_TYPE
{
	PLAYER01,
	PLAYER02,
	PLAYER03,
	PLAYER04
};

/**
 * @enum PLAYER_MODEL_TYPE
 * ���f���^�C�v�萔
 */
enum PLAYER_MODEL_TYPE
{
	PLAYER_MODEL_NORMAL,
	PLAYER_MODEL_ATTACK
};

/**
 * @enum ITEMTYPE
 * �A�C�e���萔
 */
enum ITEMTYPE
{
	ITEMTYPE_TIKEI = 0,		//!< �n�`
	ITEMTYPE_LIFE,			//!< ���C�t
	ITEMTYPE_SENSYA,		//!< ���
	ITEMTYPE_BULLET,		//!< �o���b�g
	ITEMTYPE_SPEEDUP,		//!< �X�s�[�h�A�b�v
	ITEMTYPE_CAMERA,		//!< ���ז��A�C�e���@�����o�b�N�J����
	ITEMTYPE_KIRI,			//!< ���ז��A�C�e���@��
	ITEMTYPE_MAX
};

/**
 * @enum STATUSTYPE
 * �X�e�[�^�X�萔
 */
enum STATUSTYPE
{
	STATUSTYPE_SPEED = 0,		//!< �X�s�[�h�A�b�v���
	STATUSTYPE_SENSYA,			//!< ��ԋ������
	STATUSTYPE_CAMERA,			//!< �o�b�N�J�������
	STATUSTYPE_KIRI,			//!< ���������
	STATUSTYPE_MAX
};

/**
 * @class VERTEX_2D
 * �|���S�����_�t�H�[�}�b�g�ɍ��킹��CLASS���`
 */
class VERTEX_2D
{
public:
	D3DXVECTOR3 vtx;		//!< ���_���W
	float		rhw;		//!< �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
 * @class VERTEX_3D
 * �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹��CLASS���`
 */

class VERTEX_3D
{
public:
	D3DXVECTOR3 vtx;		//!< ���_���W
	D3DXVECTOR3 nor;		//!< �@���x�N�g��
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
 * @class INTERPOLATION_DATA
 * ��ԗp�̃f�[�^CLASS���`
 */
class INTERPOLATION_DATA
{
public:
	D3DXVECTOR3 pos;		//!< ���_���W
	D3DXVECTOR3 rot;		//!< ��]
	D3DXVECTOR3 scl;		//!< �g��k��
	float		frame;		//!< ���s�t���[���� ( dt = 1.0f/frame )
};

/**
 * @class ALLCHARACTER
 * �L�����N�^�[�̕W���I�ȕϐ����܂Ƃ߂�CLASS
 */
class ALLCHARACTER
{
public:
	D3DXMATRIX					mtxWorld;			//!< ���[���h�}�g���b�N�X
	D3DXVECTOR3					pos;				//!< ���f���̈ʒu
	D3DXVECTOR3					oldpos;				//!< ���f���̈ʒu
	D3DXVECTOR3					rot;				//!< ���f���̌���(��])
	D3DXVECTOR3					scl;				//!< ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3					move;				//!< ���f���̈ړ���
};

/**
 * @class ONECHARACTER
 * �L�����N�^�[�̕W���I�ȕϐ����܂Ƃ߂�CLASS(�K�w�֌W��1���������Ă�Ηǂ��ϐ�)
 */
class ONECHARACTER
{
public:
	D3DXMATRIX					mtxWorldRotBuff;	//!< ���[���h�}�g���b�N�X��]�p�x�ۑ�
	D3DXVECTOR3					Upvec;				//!< ���f���̏����
	D3DXVECTOR3					Frontvec;			//!< ���f���̑O����
	D3DXVECTOR3					RotVecAxis;			//!< �N�I�[�^�j�I���̎��Ɏg���n�`�̖@���x�N�g��
	D3DXVECTOR3					UpFieldNorVec;		//!< �p���̋��ʐ��`��Ԏ��Ɏg���n�`�̖@���x�N�g���@�v���C���[������Ă���|���S���̈��̃|���S��
	D3DXVECTOR3					RightFieldNorVec;	//!< �p���̋��ʐ��`��Ԏ��Ɏg���n�`�̖@���x�N�g���@�v���C���[������Ă���|���S���̈�E�̃|���S��
	D3DXVECTOR3					LeftFieldNorVec;	//!< �p���̋��ʐ��`��Ԏ��Ɏg���n�`�̖@���x�N�g���@�v���C���[������Ă���|���S���̈���̃|���S��
	D3DXVECTOR3					DownFieldNorVec;	//!< �p���̋��ʐ��`��Ԏ��Ɏg���n�`�̖@���x�N�g���@�v���C���[������Ă���|���S���̈���̃|���S��
	D3DXVECTOR3					UpRotTOaxis;		//!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	D3DXVECTOR3					FrontRotTOaxis;		//!< �n�`�@���ƃv���C���[Front�x�N�g���̊O�ϒl
	D3DXQUATERNION				q;					//!< �v���C���[�p�N�I�[�^�j�I��
	float						speed;				//!< �ړ��X�s�[�h
	float						speedbuff;			//!< �ړ��X�s�[�h�A�C�e���o�t
	float						speedbufftime;		//!< �ړ��X�s�[�h�A�C�e�����ʎ���
	float						Qrot;				//!< Up�x�N�g������n�`�@���ւ̉�]�p�x
	float						Brot;				//!< Front�x�N�g������n�`�@���ւ̉�]�p�x
	float						MorphingTime;		//!< ���[�t�B���O�ł��鎞��
	int							shadowIdx;			//!< �e�̃C���f�b�N�X�ԍ�
	int							dir;				//!< �i�s����
	bool						use;				//!< �����Ă锻��
	bool						Morphing;			//!< �ό`����
	bool						MorphingEnd;		//!< �ό`�I������
	bool						speedbuffsignal;	//!< �X�s�[�h�A�b�v�o�t����
};

/**
 * @class GPUMODEL
 * ���f���̒��_�f�[�^���g�p�@���[�t�B���O���s���Ɍ��f�[�^�ƕω��f�[�^�̒��_����ۑ�
 */
class GPUMODEL
{
public:
	LPDIRECT3DTEXTURE9			pD3DTexture;			//!< �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH					pD3DXMesh;				//!< ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				pD3DXBuffMat;			//!< �}�e���A�����ւ̃|�C���^
	DWORD						nNumMat;				//!< �}�e���A�����̐�
	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;			//!< ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		pD3DIdxBuff;			//!< �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	DWORD						nNumVertex;				//!< ���_�̐�
	DWORD						nNumVertexIndex;		//!< ���_�̃C���f�b�N�X
	DWORD						nNumPolygon;			//!< �|���S���̐�
	float						time;					//!< ���[�t�B���O���ɕ�ԃ^�C���Ƃ��Ďg�p
	int							MorphingSignal;			//!< 1,�Ȃɂ����Ȃ��@2,���[�t�B���O���@3,���[�t�B���O����
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
* @brief �Q�[���V�[�����Z�b�g����֐� SetScene
* @param[in] Scene �ړ���V�[�������
* @details �Q�[���V�[����E_STAGE���Q��
*/
void SetScene(int Scene);

/**
* @brief ���݂̃Q�[���V�[������肷��֐� GetScene
* @return int ���݂̃Q�[���V�[����Ԃ�
* @details �Q�[���V�[����E_STAGE���Q��
*/
int GetScene(void);

/**
* @brief �Q�[��������������֐� InitGame
* @details �Q�[���N�����Ɏg�p�Q�[�����[�v���Ɏg�p
*/
void InitGame(void);



