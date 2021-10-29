/**
* @file player.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../h/object/objectclass.h"




//class PLAYER_HONTAI;

/**
 * @enum ePLAYER_PARTS_TYPE
 * @brief 
 */
enum ePLAYER_PARTS_TYPE
{
	PLAYER_PARTS_TYPE_HOUDAI = 0,		//!< �C��
	PLAYER_PARTS_TYPE_HOUTOU,			//!< �C��
	PLAYER_PARTS_TYPE_HOUSIN,			//!< �C�g
	PLAYER_PARTS_TYPE_MAX,				//!< ���v��
};

/**
 * @enum ePLAYER_PARTS_TYPE
 * @brief
 */
enum ePLAYER_MODEL_ORIGINAL_TYPE
{
	PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN = 0,			//!< �C�g�ʏ�
	PLAYER_MODEL_ORIGINAL_TYPE_HOUSIN_MORPHING,		//!< �C�g�A�^�b�N
	PLAYER_MODEL_ORIGINAL_TYPE_MAX,					//!< ���v��
};

/**
 * @enum ePLAYER_MODEL_ALL_TYPE
 * @brief
 */
enum ePLAYER_MODEL_ALL_TYPE
{
	PLAYER_MODEL_DRAW_ALL_HOUDAI = 0,		//!< �C��
	PLAYER_MODEL_DRAW_ALL_HOUTOU,			//!< �C��
	PLAYER_MODEL_DRAW_ALL_HOUSIN,			//!< �C�g
	PLAYER_MODEL_DRAW_ALL_HOUSIN_MORPHING,	//!< �C�g�A�^�b�N
	PLAYER_MODEL_DRAW_ALL_MAX,				//!< ���v��
};

/**
*�@@class PLAYER_PRATS
*�@@brief 
*/
class PLAYER_PRATS
{
public:
	//PLAYER_HONTAI		*ParentHontai;		//!< �e����@�e�K�w�̃A�h���X
	//PLAYER_PRATS		*ParentParts;		//!< �q����@�q�K�w�̃A�h���X
};

/**
*�@@class PLAYER_PARAMETER_BULLET
*�@@brief �v���C���[�o���b�g�p�p�����[�^
*/
class PLAYER_PARAMETER_BULLET
{
public:
	PLAYER_PARAMETER_BULLET() {
		BulletStartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	for (int i = 0; i < 3; i++) BulletMove[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		BulletRotY = 0.0f; BulletBornTime = 0.0f; BulletStock = 0; NetBulletShotFlagOneFrame = 0;
	};
	~PLAYER_PARAMETER_BULLET() {}

	D3DXVECTOR3		BulletStartPos;				//!< �o���b�g�̔��˒n�_
	D3DXVECTOR3		BulletMove[3];				//!< �o���b�g�̈ړ��ʁ@�p���[�A�b�v���͎O�����ɔ���
	float			BulletRotY;					//!< �o���b�g��Front�x�N�g������n�`�@���ւ̉�]�p�x
	float			BulletBornTime;				//!< �c�e�񕜃J�E���g�B���Ԃŉ� BULLET_BORN_MAXTIME
	int				BulletStock;				//!< �c�e��
	int				NetBulletShotFlagOneFrame;	//!< �l�b�g�ΐ�p�@�o���b�g�𔭎˂��Ă��Ȃ�0,�ʏ탂�f���͔���1,�A�^�b�N���f���͔���3�@���t���[���m�F���āA�o���b�g���˂���ƃt���O�𗧂Ă�
};

/**
*�@@class PLAYER_PARAMETER_STANDARD
*�@@brief �v���C���[�W���p�����[�^�@�@���@�p���x�N�g���@�̗͂Ȃ�
*/
class PLAYER_PARAMETER_STANDARD
{
public:
	PLAYER_PARAMETER_STANDARD() {
		FUPFCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f); Speed = 0.0f; Vital = PLAYER_VITAL_MAX; OldVital = PLAYER_VITAL_MAX; ShadowIdx = -1;
		eModelType = PLAYER_MODEL_TYPE_NORMAL; eOldModelType = PLAYER_MODEL_TYPE_NORMAL;
	}
	~PLAYER_PARAMETER_STANDARD() {}

	D3DXVECTOR3		FUPFCross;			//!< �n�`�@���ƃv���C���[Front�x�N�g���̊O�ϒl FieldUpPlayerFrontCross
	float			Speed;				//!< �ړ��X�s�[�h
	int				Vital;				//!< �̗�
	int				OldVital;			//!< �̗�
	int				ShadowIdx;			//!< �e�̃C���f�b�N�X�ԍ�
	ePLAYER_MODEL_TYPE	eModelType;		//!< �ǂ̃��f����Ԃ�����
	ePLAYER_MODEL_TYPE	eOldModelType;	//!< �ǂ̃��f����Ԃ�����

};

/**
*�@@class PLAYER_PARAMETER_ITEM
*�@@brief �v���C���[�A�C�e���p�p�����[�^�@���ʎ��ԁ@�����t���O�@�Ȃ�
*/
class PLAYER_PARAMETER_ITEM
{
public:
	PLAYER_PARAMETER_ITEM() {
		SpeedBuff = 0.0f; SpeedBuffTime = 0.0f; BackCameraTime = 0.0f; KiriTime = 0.0f;
		SpeedBuffSignal = false; BackCameraItemSignal = false; KiriSignal = false; DashSignal = false;
	};
	~PLAYER_PARAMETER_ITEM() {}

	float	SpeedBuff;					//!< �ړ��X�s�[�h�A�C�e���o�t MAX_SPEEDBUFF
	float	SpeedBuffTime;				//!< �ړ��X�s�[�h�A�C�e�����ʎ���
	float	BackCameraTime;				//!< �o�b�N�J�����A�C�e���̌��ʎ���
	float	KiriTime;					//!< ���A�C�e���̌��ʎ���
	bool	SpeedBuffSignal;			//!< �X�s�[�h�A�b�v�o�t����
	bool	BackCameraItemSignal;		//!< �o�b�N�J�����A�C�e�����g�p���Ă��邩�ǂ���
	bool	KiriSignal;					//!< ���A�C�e�����g�p���Ă��邩�ǂ���
	bool	DashSignal;					//!< �_�b�V������@���͂������true.����ȊO��false
};

/**
*�@@class PLAYER_PARAMETER_MORPHING
*�@@brief �v���C���[���[�t�B���O�p�����[�^�@���ԁ@�M���Ȃ�
*/
class PLAYER_PARAMETER_MORPHING
{
public:
	PLAYER_PARAMETER_MORPHING() {
		MorphingTime = 0.0f; MorphingDTtime = 0.0f; MorphingSignal = NoMorphing;
		MorphingStart = false; MorphingEnd = false; NetGetMorphingOneFrame = false;
	}
	~PLAYER_PARAMETER_MORPHING() {}

	float			MorphingTime;				//!< ���[�t�B���O�ł��鎞��
	float			MorphingDTtime;				//!< ���[�t�B���O���ɕ�ԃ^�C���Ƃ��Ďg�p
	eMORPHING_TYPE	MorphingSignal;				//!< 1,�Ȃɂ����Ȃ��@2,���[�t�B���O���@3,���[�t�B���O����
	bool			MorphingStart;				//!< �ό`����@true�A�^�b�N���f��(�A�C�e���擾��true)�@false�ʏ탂�f��(ture�ɂȂ��Ď��Ԍo�ߌ�false)
	bool			MorphingEnd;				//!< �ό`�I������@
	bool			NetGetMorphingOneFrame;		//!< �l�b�g�ΐ�p�@true�Ń��[�t�B���O���@���[�t�B���O�̓����Ŏg�p
};

/**
*�@@class PLAYER_PARAMETER_SUMMARY
*�@@brief �v���C���[�p�����[�^�܂Ƃ�
*/
class PLAYER_PARAMETER_SUMMARY
{
public:
	PLAYER_PARAMETER_SUMMARY() {}
	~PLAYER_PARAMETER_SUMMARY() {}

	PLAYER_PARAMETER_BULLET		BulletPara;
	PLAYER_PARAMETER_STANDARD	StandardPara;
	PLAYER_PARAMETER_ITEM		ItemPara;
	PLAYER_PARAMETER_MORPHING	MorphingPara;
};

/**
*�@@class PLAYER_MODEL_DRAW
*�@@brief �v���C���[���f���@�`��p
*/
class PLAYER_MODEL_DRAW
{
public:
	PLAYER_MODEL_DRAW() {}
	~PLAYER_MODEL_DRAW() {}

	ModelAttribute		ModelAttribute[PLAYER_PARTS_TYPE_MAX];		//!< ���f���f�[�^�@0�m�[�}���A1�U���@���f���f�[�^�̎�ށ@�u�v���C���[�̐��v=4�A�u���f���p�[�c��(�C��A�C���A�C�g�A�C�g�ό`��)�v=2
	VTXBuffer			Vtx[PLAYER_PARTS_TYPE_MAX];					//!< ���_���@�`��p
	TransForm			Transform[PLAYER_PARTS_TYPE_MAX];			//!< �g�����X�t�H�[�����

};

/**
*�@@class PLAYER_MODEL_ORIGINAL
*�@@brief �v���C���[���f���@�I���W�i���p
*/
class PLAYER_MODEL_ORIGINAL
{
public:
	PLAYER_MODEL_ORIGINAL() {}
	~PLAYER_MODEL_ORIGINAL() {}

	ModelAttribute		ModelAttribute[PLAYER_MODEL_ORIGINAL_TYPE_MAX];		//!< ���f���f�[�^�@0�m�[�}���A1�U���@���f���f�[�^�̎�ށ@�u�C�g�A�C�g�ό`��v=2
	VTXBuffer			Vtx[PLAYER_MODEL_ORIGINAL_TYPE_MAX];				//!< ���_���@���[�t�B���O��p
};

/**
*�@@class PLAYER
*�@@brief GAMEOBJECT�h���N���X
*/
class PLAYER : public GAME_OBJECT
{
public:
	PLAYER();	//!< �f�[�^�ǂݍ��݁@������
	~PLAYER();	//!< �폜

	void				Init(FIELD *field);			//!< ������
	void				Update(EFFECT*effect, BULLET*bullet, SHADOW*shadow, FADE *fade, bool Netflag, int MyNumber); //!< �X�V
	void				Draw(void);						//!< �`��

	//PLAYER_PRATS				parts[OBJECT_PLAYER_MAX];			//!< ���f���̃p�[�c�K�w�@�e�{��(�C��)�A�q�p�[�c[0](�C��)�A���p�[�c[1](�C�g)
	//PLAYER_HONTAI				*Parent[OBJECT_PLAYER_MAX];			//!< �e�q���� ���[���h�}�g���N�X�Őe���q�����̂悤�Ɏg��

	PLAYER_PARAMETER_SUMMARY	PlayerPara[OBJECT_PLAYER_MAX];		//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q	���������̂ŕ�����܂��Ă���							

	TEXTURE						tex;								//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	PLAYER_MODEL_DRAW			modelDraw[OBJECT_PLAYER_MAX];		//!< �`��p���f���f�[�^
	PLAYER_MODEL_ORIGINAL		modelOri;							//!< �I���W�i�����f���f�[�^�@���[�t�B���O���̊�p

	iUseCheak					iUseType[OBJECT_PLAYER_MAX];		//!< �g�p���
	FieldNor					PostureVec[OBJECT_PLAYER_MAX];		//!< �p���x�N�g��
	Movement					Move[OBJECT_PLAYER_MAX];			//!< �ړ���
private:

	//------�J���[
	void	PlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff, DWORD nNumPolygon, int CntPlayer);

	//------�ړ�����
	void	MoveABL(int CntPlayer, EFFECT *effect, bool Netflag);		//!< �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
	void	MoveL(int CntPlayer, EFFECT *effect, bool Netflag);			//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	void	MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag);	//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	void	MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag);		//!< �ړ�����(L2R2�ňړ�����)
	void	MoveKeybord(int CntPlayer, EFFECT *effect, bool Netflag);	//!< �ړ�����(ki-bo-do�ňړ�����)

	//------�J��������
	void	CameraRevers(int CntPlayer, bool Netflag);		//!< �J��������
	void	CameraRotControl(int CntPlayer, bool Netflag);	//!< �J��������(LR�X�e�B�b�N�ňړ�����)

	//------�p������
	void	Quaternion(int CntPlayer);					//!< �N�H�[�^�j�I������@�n�`�@����UP�x�N�g���ƊO��

	//------�o���b�g����
	void	BulletALL(int CntPlayer, BULLET *bullet, SHADOW *shadow, bool Netflag);			//!< �o���b�g�֘A����
	void	BulletALLMoveL2R2Ver(int CntPlayer, BULLET *bullet, SHADOW *shadow);			//!< �o���b�g�֘A���� ���˃{�^�����\���J�[�\��

	//------�A�C�e������
	void	ItemTimeKiri(int CntPlayer);				//!< �t�H�O����
	void	ItemTimeMorphing(int CntPlayer);			//!< ���[�t�B���O����

	//�ǂݍ��ރ��f���@�`��p
	const char *c_aFileNameModelDraw[PLAYER_MODEL_DRAW_ALL_MAX] =
	{
		"../data/MODEL/PlayerSensyaHoudai.x",
		"../data/MODEL/PlayerSensyaHoutou.x",
		"../data/MODEL/PlayerSensyaHousin.x",
	};

	//�ǂݍ��ރ��f���@�I���W�i���p�@���[�t�B���O���̊�f�[�^
	const char *c_aFileNameModelOriginal[PLAYER_MODEL_ORIGINAL_TYPE_MAX] =
	{
		"../data/MODEL/PlayerSensyaHousin.x",
		"../data/MODEL/PlayerSensyaHousinMo.x"
	};


};


