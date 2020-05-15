//=============================================================================
//
// ���C������ [main.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#pragma once
#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~

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

#define DIRECTINPUT_VERSION (0x0800)	// �x���΍�
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
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_W		(1280)				// �E�C���h�E�̕�
#define SCREEN_H		(960)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X				(SCREEN_W / 2)		// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y				(SCREEN_H / 2)		// �E�C���h�E�̒��S�x���W

//2D�|���S���Ɋւ���p�����[�^
#define	POLYGON_2D_VERTEX			(4)					// ���_��
#define	POLYGON_2D_NUM				(2)					// �|���S����

//�摜�̎Q�Ɩ�
#define TEXTURE_GAME_SCORE		"data/TEXTURE/BG/0-9.png"						// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_MEISAI			"data/MODEL/28513607_p4_master1200.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TEXTURE_TIKEI_ITEM			"data/MODEL/landmark_aogashima.png"			// �n�`�ό`�A�C�e��
#define	TEXTURE_LIFE_ITEM			"data/MODEL/life000.png"					// ���C�t��
#define	TEXTURE_SENSYA_ITEM			"data/MODEL/war_sensya_noman.png"			// ��ԕό`�A�C�e��
#define	TEXTURE_BULLET_ITEM			"data/MODEL/bullettex.png"					// �o���b�g�A�C�e��
#define	TEXTURE_SPEED_ITEM			"data/MODEL/1213810.png"					// �X�s�[�h�A�b�v�A�C�e��
#define	TEXTURE_CAMERA_ITEM			"data/MODEL/mark_camera_satsuei_ok.png"		// �����o�b�N�J�����A�C�e��
#define	TEXTURE_KIRI_ITEM			"data/MODEL/yama_kiri.png"					// ���A�C�e��


//���f���̎Q��
#define	MODEL_HOUDAI		"data/MODEL/PlayerSensyaHoudai.x"		// �ǂݍ��ރ��f����
#define	MODEL_HOUTOU		"data/MODEL/PlayerSensyaHoutou.x"		// �ǂݍ��ރ��f����
#define	MODEL_HOUSIN		"data/MODEL/PlayerSensyaHousin.x"		// �ǂݍ��ރ��f����
#define	MODEL_HOUSINMO		"data/MODEL/PlayerSensyaHousinMo.x"		// �ǂݍ��ރ��f����


//�L�����N�^�[�Ȃǂ̍��v
#define PLAYER_MAX					(4)
#define BULLETPOINT_MAX				(PLAYER_MAX)

//�ړ��ʃ}�N��
#define VALUE_MOVE_BULLET			(6.0f)
#define	VALUE_MOVE					(0.1f)							// �ړ������x
#define	VALUE_MOVE_MAX				(4.0f)							// �ړ��ʂ�MAX
#define	VALUE_ROTATE				(D3DX_PI * 0.02f)				// ��]��
#define	VALUE_ROTATE_PLAYER			(0.03f)							// �v���C���[��]��
#define MAX_SPEEDBUFF				(2.0f)
#define MAX_SPEEDBUFFTIME			(120.0f*3)

//2D�e�N�X�`���̃T�C�Y�|�W�V�����Ȃ�
#define	LIFETEX_SIZE_X				(30.0f)										// ���C�t�̐����̕�
#define	LIFETEX_SIZE_Y				(40.0f)										// ���C�t�̐����̍���
#define	LIFETEX_POS_X				(SCREEN_CENTER_X-200.0f)					// ���C�t�̕\����ʒu�w���W
#define	LIFETEX_POS_Y				(SCREEN_CENTER_Y-100.0f)					// ���C�t�̕\����ʒu�x���W
#define	BULLETTEX_SIZE_X			(LIFETEX_SIZE_X)							// ���C�t�̐����̕�
#define	BULLETTEX_SIZE_Y			(LIFETEX_SIZE_Y)							// ���C�t�̐����̍���
#define	BULLETTEX_POS_X				(LIFETEX_POS_X)								// ���C�t�̕\����ʒu�w���W
#define	BULLETTEX_POS_Y				(LIFETEX_POS_Y+LIFETEX_SIZE_Y)				// ���C�t�̕\����ʒu�x���W

#define	NUM_PLACE			(8)													// ���C�t�̐�
#define MAX_AMMO			(5)
#define MAX_LIFE			(6)
#define BORN_AMMO_ADDTIME	(1.0f)							
#define BORN_AMMO_MAXTIME	(120.0f)							
#define MORPHING_TIME		(300.0f)							
#define BACKCAMERA_TIME		(300.0f)
#define KIRI_TIME			(300.0f)							
#define SCREENDAMEGE_TIME	(30.0f)							
#define	MAX_SHADOW			(256)							// �e�ő吔

#define	BULLETNORMAL_MAX			(5)							// �o���b�g�ʏ펝����
#define	BULLETKAKUSAN_MAX			(3)							// �o���b�g�g�U��
#define	BULLET_MAX					((BULLETNORMAL_MAX*BULLETKAKUSAN_MAX*PLAYER_MAX)*2)				// �o���b�g��

#define HEADICON_MAX				(PLAYER_MAX*(PLAYER_MAX-1))

#define PLAYER_MODEL_SIZE			(15.0f)
#define BULLET_MODEL_SIZE			(15.0f)
#define ITEM_MODEL_SIZE				(15.0f)
#define DROP_ITEM_MAX				(10)
#define DROP_ITEM_CHARGE_ADDTIME	(0.1f)
#define DROP_ITEM_CHARGE_CNT		(300.0f)
#define	MAX_ITEM					(20)						// �A�C�e���ő吔

// ��ʑJ�ڒ萔
enum E_STAGE//�񋓌^�Bdefine�̔ԍ��������Ŋ��蓖�ĂĂ����B
{
	SCENE_TITLE,					//0�B1�߂�����0�B�񋓌^�̗񋓒萔���B��`�������O���V���{�����Ă����̂���ʂ炵���B
	SCENE_SELECT,					//1�l������
	SCENE_TUTORIAL,					//2�`���[�g���A��
	SCENE_GAMECOUNTDOWN,			//3�J�E���g�_�E��
	SCENE_GAME,						//4�Q�[��
	SCENE_RESULT,					//5���U���g
	SCENE_MAX						
};

//�T�E���h�{�����[���̃A�b�v�_�E�����
enum MASTER_VOLUMEOL_CHENGE
{
	VOL_UP,
	VOL_DOWN,
};

//�����_���̎��
enum RAND
{
	X,
	Y,
	Z,
};

//�X�R�A�̎��
enum SCORETYPE
{
	TIMEV,
	SCOREVAL,
};

//���[�t�B���O�̎��
enum MORPHINGTYPE
{
	NoMorphing,
	NowMorphing,
	EndMorphing
};

//�ړ�����
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

enum PLAYER_TYPE
{
	PLAYER01,
	PLAYER02,
	PLAYER03,
	PLAYER04
};

//���f���^�C�v
enum PLAYER_MODEL_TYPE
{
	PLAYER_MODEL_NORMAL,
	PLAYER_MODEL_ATTACK
};

enum
{
	ITEMTYPE_TIKEI = 0,		// �n�`
	ITEMTYPE_LIFE,			// ���C�t
	ITEMTYPE_SENSYA,		// ���
	ITEMTYPE_BULLET,		// �o���b�g
	ITEMTYPE_SPEEDUP,		// �X�s�[�h�A�b�v
	ITEMTYPE_CAMERA,		// ���ז��A�C�e���@�����o�b�N�J����
	ITEMTYPE_KIRI,			// ���ז��A�C�e���@��
	ITEMTYPE_MAX
};

enum
{
	STATUSTYPE_SPEED=0,
	STATUSTYPE_SENSYA,
	STATUSTYPE_CAMERA,
	STATUSTYPE_KIRI,
	STATUSTYPE_MAX
};

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float		rhw;		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR	diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

// ��ԗp�̃f�[�^�\���̂��`
struct INTERPOLATION_DATA
{
	D3DXVECTOR3 pos;		// ���_���W
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scl;		// �g��k��
	float		frame;		// ���s�t���[���� ( dt = 1.0f/frame )
};

// �L�����N�^�[�̕W���I�ȕϐ����܂Ƃ߂��\����
struct ALLCHARACTER
{
	D3DXMATRIX					mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3					pos;				// ���f���̈ʒu
	D3DXVECTOR3					oldpos;				// ���f���̈ʒu
	D3DXVECTOR3					rot;				// ���f���̌���(��])
	D3DXVECTOR3					scl;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3					move;				// ���f���̈ړ���
};

// �L�����N�^�[�̕W���I�ȕϐ����܂Ƃ߂��\����(�K�w�֌W�ł�1���������Ă�Ηǂ��ϐ�)
struct ONECHARACTER
{
	D3DXMATRIX					mtxWorldRotBuff;	// ���[���h�}�g���b�N�X��]�p�x�ۑ�
	D3DXVECTOR3					Upvec;				// ���f���̏����
	D3DXVECTOR3					Frontvec;			// ���f���̑O����
	D3DXVECTOR3					RotVecAxis;			// �����̎��Ɏg���n�`�̖@���x�N�g��
	D3DXVECTOR3					UpRotTOaxis;		// �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	D3DXVECTOR3					FrontRotTOaxis;		// �n�`�@���ƃv���C���[Front�x�N�g���̊O�ϒl
	D3DXQUATERNION				q;					// �N�I�[�^�j�I��
	D3DXQUATERNION				BrotQ;				// �N�I�[�^�j�I��
	D3DXQUATERNION				kyouyakuQ;			// ��������
	float						speed;				// �ړ��X�s�[�h
	float						speedbuff;			// �ړ��X�s�[�h�A�C�e���o�t
	float						speedbufftime;		// �ړ��X�s�[�h�A�C�e�����ʎ���
	float						Qrot;				// Up�x�N�g������n�`�@���ւ̉�]�p�x
	float						Brot;				// Front�x�N�g������n�`�@���ւ̉�]�p�x
	float						MorphingTime;		// ���[�t�B���O�ł��鎞��
	int							shadowIdx;			// �e�̃C���f�b�N�X�ԍ�
	int							dir;				// �i�s����
	bool						use;				// �����Ă锻��
	bool						Morphing;			// �ό`����
	bool						MorphingEnd;		// �ό`�I������
	bool						speedbuffsignal;	// �X�s�[�h�A�b�v�o�t����

};

//GPU���g�p���ĕ`�悷�郂�f���̍\����
struct GPUMODEL
{
	LPDIRECT3DTEXTURE9			pD3DTexture;			// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH					pD3DXMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				pD3DXBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD						nNumMat;				// �}�e���A�����̐�
	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		pD3DIdxBuff;			// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	DWORD						nNumVertex;				// ���_�̐�
	DWORD						nNumVertexIndex;		// ���_�̃C���f�b�N�X
	DWORD						nNumPolygon;			// �|���S���̐�
	float						time;					// ���[�t�B���O���ɕ�ԃ^�C���Ƃ��Ďg�p
	int							MorphingSignal;			// 1,�Ȃɂ����Ȃ��@2,���[�t�B���O���@3,���[�t�B���O����
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);		// �f�o�C�X�擾�֐�
void DrawTextType(void);
void SetScene(int Scene);
void InitGame(void);
int GetScene(void);
float Random(int type);
DWORD GetFPS(void);
float SpdCal(D3DXVECTOR3 move);
bool RayCast(D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 *ReturnVtx);








