/**
* @file player.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Move/Move.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Attack/Attack.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Posture/Posture.h"

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
*�@@class PLAYER_MODEL_DRAW
*�@@brief �v���C���[���f���@�`��p
*/
class PLAYER_MODEL_DRAW
{
public:
	PLAYER_MODEL_DRAW() {}
	~PLAYER_MODEL_DRAW() {}

	MODELATTRIBUTE		Attribute[PLAYER_PARTS_TYPE_MAX];		//!< ���f���f�[�^�@0�m�[�}���A1�U���@���f���f�[�^�̎�ށ@�u�v���C���[�̐��v=4�A�u���f���p�[�c��(�C��A�C���A�C�g�A�C�g�ό`��)�v=2
	VTXBUFFER			Vtx[PLAYER_PARTS_TYPE_MAX];					//!< ���_���@�`��p
	TRANSFORM			Transform[PLAYER_PARTS_TYPE_MAX];			//!< �g�����X�t�H�[�����

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

	MODELATTRIBUTE		Attribute[PLAYER_MODEL_ORIGINAL_TYPE_MAX];		//!< ���f���f�[�^�@0�m�[�}���A1�U���@���f���f�[�^�̎�ށ@�u�C�g�A�C�g�ό`��v=2
	VTXBUFFER			Vtx[PLAYER_MODEL_ORIGINAL_TYPE_MAX];				//!< ���_���@���[�t�B���O��p
};

/**
*�@@class PLAYER_PARAMETER_STANDARD
*�@@brief �v���C���[�W���p�����[�^�@�@���@�p���x�N�g���@�̗͂Ȃ�
*/
class PLAYER_PARAMETER_STANDARD
{
public:
	PLAYER_PARAMETER_STANDARD() {
		FUPFCross = VEC3_ALL0; Speed = 0.0f; Vital = PLAYER_VITAL_MAX; OldVital = PLAYER_VITAL_MAX; ShadowIdx = -1;
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

	//------�A�C�e������
	void ItemTimeSpeed(PLAYER *Player, int CntPlayer);	//!< �X�s�[�h�A�b�v����
	void ItemTimeKiri(int CntPlayer);				//!< �t�H�O����

	//------------------------get�֐�
	inline float GetSpeedBuff(void) { return SpeedBuff; };
	inline float GetSpeedBuffTime(void) { return SpeedBuffTime; };
	inline float GetBackCameraTime(void) { return BackCameraTime; };
	inline float GetKiriTime(void) { return KiriTime; };
	inline bool GetSpeedBuffSignal(void) { return SpeedBuffSignal; };
	inline bool GetBackCameraItemSignal(void) { return BackCameraItemSignal; };
	inline bool GetKiriSignal(void) { return KiriSignal; };
	inline bool GetDashSignal(void) { return DashSignal; };
	//------------------------set�֐�
	inline void SetSpeedBuff(const float InBuff) { SpeedBuff = InBuff; };
	inline void SetSpeedBuffTime(const float InBuffTime) { SpeedBuffTime = InBuffTime; };
	inline void SetBackCameraTime(const float InTime) { BackCameraTime = InTime; };
	inline void SetKiriTime(const float InTime) { KiriTime = InTime; };
	inline void SetSpeedBuffSignal(const bool InFlag) { SpeedBuffSignal = InFlag; };
	inline void SetBackCameraItemSignal(const bool InFlag) { BackCameraItemSignal = InFlag; };
	inline void SetKiriSignal(const bool InFlag) { KiriSignal = InFlag; };
	inline void SetDashSignal(const bool InFlag) { DashSignal = InFlag; };

private:
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
		MorphingStart = false; MorphingEnd = false; NetMorphingOneFrame = false;
	}
	~PLAYER_PARAMETER_MORPHING() {}

	//------�A�C�e������
	void ItemTimeMorphing(PLAYER *Player, int CntPlayer);		//!< ���[�t�B���O����

	//------------------------get�֐�
	inline float GetMorphingTime(void) { return MorphingTime; };
	inline float GetMorphingDTtime(void) { return MorphingDTtime; };
	inline bool GetMorphingStart(void) { return MorphingStart; };
	inline bool GetMorphingEnd(void) { return MorphingEnd; };
	inline bool GetNetMorphingOneFrame(void) { return NetMorphingOneFrame; };
	inline eMORPHING_TYPE GetMorphingSignal(void) { return MorphingSignal; };
	//------------------------set�֐�
	inline void SetMorphingTime(const float InTime) { MorphingTime = InTime; };
	inline void SetMorphingDTtime(const float InDtTime) { MorphingDTtime = InDtTime; };
	inline void SetMorphingStart(const bool InFlag) { MorphingStart = InFlag; };
	inline void SetMorphingEnd(const bool InFlag) { MorphingEnd = InFlag; };
	inline void SetNetMorphingOneFrame(const bool InFlag) { NetMorphingOneFrame = InFlag; };
	inline void SetMorphingSignal(const eMORPHING_TYPE InType) { MorphingSignal = InType; };

private:
	float			MorphingTime;				//!< ���[�t�B���O�ł��鎞��
	float			MorphingDTtime;				//!< ���[�t�B���O���ɕ�ԃ^�C���Ƃ��Ďg�p
	bool			MorphingStart;				//!< �ό`����@true�A�^�b�N���f��(�A�C�e���擾��true)�@false�ʏ탂�f��(ture�ɂȂ��Ď��Ԍo�ߌ�false)
	bool			MorphingEnd;				//!< �ό`�I������@
	bool			NetMorphingOneFrame;		//!< �l�b�g�ΐ�p�@true�Ń��[�t�B���O���@���[�t�B���O�̓����Ŏg�p
	eMORPHING_TYPE	MorphingSignal;				//!< 1,�Ȃɂ����Ȃ��@2,���[�t�B���O���@3,���[�t�B���O����
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

	PLAYER_PARAMETER_MOVE		MovePara;
	PLAYER_PARAMETER_BULLET		BulletPara;
	PLAYER_PARAMETER_ITEM		ItemPara;
	PLAYER_PARAMETER_MORPHING	MorphingPara;
	PLAYER_PARAMETER_STANDARD	StandardPara;
	POSTURE	PostureVec;					//!< �p���x�N�g��
};

/**
*�@@class PLAYER
*�@@brief GAMEOBJECT�h���N���X
*/
class PLAYER : public GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	PLAYER();	//!< �f�[�^�ǂݍ��݁@������
	~PLAYER();	//!< �폜

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;		//!< �A�h���b�T�[
	void Init(void) override;			//!< ������
	void InitNet(void)override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	void Update(void)override;			//!< �X�V
	void Draw(void)override;			//!< �`��

	PLAYER_PARAMETER_SUMMARY PlayerPara[OBJECT_PLAYER_MAX];	//!< �C���X�^���X�ɕK�v�ȃf�[�^�Q	���������̂ŕ�����܂��Ă���							
	PLAYER_MODEL_DRAW modelDraw[OBJECT_PLAYER_MAX];			//!< �`��p���f���f�[�^
	PLAYER_MODEL_ORIGINAL modelOri;							//!< �I���W�i�����f���f�[�^�@���[�t�B���O���̊�p
	iUseCheck iUseType[OBJECT_PLAYER_MAX];					//!< �g�p���

	//------���N���X�̃A�h���X ���ʂɃp�u���b�N
	EFFECT *peffect;
	BULLET *pbullet;

private:
	TEXTURE	tex;						//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX

	//------���N���X�̃A�h���X
	FIELD *pfield;
	MySOCKET *pmysocket;
	SCENE *pscene;

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


