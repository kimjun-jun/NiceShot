/**
* @file player.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"
class PLAYER_HONTAI;

/**
 * @enum ITEMTYPE
 * �A�C�e���萔
 */
enum PARTSTYPE
{
	PARTSTYPE_HOUTOU = 0,		//!< �C��
	PARTSTYPE_HOUSIN,			//!< �C�g
};

class PLAYER_PRATS : public OBJECT_3D
{
public:
	PLAYER_HONTAI				*ParentHontai;		// �e�q����
	PLAYER_PRATS				*ParentParts;		// �e�q����
};

class PLAYER_HONTAI : public OBJECT_3D
{
public:
	PLAYER_HONTAI()
	{
		FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f), Brot = 0.0f;
		speed = 0.0f;
		speedbuff = 0.0f;
		speedbufftime = 0.0f;
		MorphingTime = 0.0f;
		BackCameraItemTime = 0.0f;
		KiriItemTime = 0.0f;
		AmmoBornTime = 0.0f;
		time = 0.0f;
		MorphingSignal = 0;
		vital = 0;
		oldvital = 0;
		AmmoCnt = 0;
		ModelType = 0;
		OldModelType = 0;
		shadowIdx = 0;
		GetMorphing = false;
		Morphing = false;
		MorphingEnd = false;
		speedbuffsignal = false;
		BackCameraItemSignal = false;
		KiriSignal = false;
		dashFlag = false;
	}

	PLAYER_PRATS				parts[2];					//!< ���f���̃p�[�c�K�w�@�e�{��(�C��)�A�q�p�[�c[0](�C��)�A���p�[�c[1](�C�g)
	PLAYER_HONTAI				*Parent;					//!< �e�q����
	GPUMODEL					ModelDate[2];				//!< ���f���f�[�^�@0�m�[�}���A1�U���A2�X�s�[�h
	virtual void				Init(FIELD *field);					//!< ������
	virtual void				Reinit(FIELD *field);				//!< �ď�����
	virtual void				ReinitNet(int MyNumber);				//!< �ď�����
	virtual void				Uninit(void);				//!< �I��
	virtual void				Update(EFFECT*effect, BULLET*bullet, SHADOW*shadow, FADE *fade, bool Netflag, int MyNumber);				//!< �X�V
	virtual void				Draw();					//!< �`��
	void						SetPlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff,DWORD nNumPolygon, int CntPlayer);
	void						SetMoveABL(int CntPlayer, EFFECT *effect);				//!< �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
	void						SetCamera(int CntPlayer, bool Netflag);				//!< �J�����X�V
	void						SetMoveL(int CntPlayer, EFFECT *effect, bool Netflag);				//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	void						SetCameraR(int CntPlayer, bool Netflag);				//!< �J��������(LR�X�e�B�b�N�ňړ�����)
	void						SetMoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag);				//!< �ړ�����(L2R2�ňړ�����)
	void						SetQ(int CntPlayer);					//!< �N�H�[�^�j�I������
	void						SetBulletALL(int CntPlayer, BULLET *bullet, SHADOW *shadow, bool Netflag);			//!< �o���b�g�֘A����
	void						SetBulletALLMoveL2R2Ver(int CntPlayer, BULLET *bullet, SHADOW *shadow);	//!< �o���b�g�֘A���� ���˃{�^�����\���J�[�\��
	void						SetKiri(int CntPlayer);					//!< �t�H�O����
	void						SetMorphing(int CntPlayer);				//!< ���[�t�B���O����

	D3DXVECTOR3					GetPosPlayer() { return this[0].GetPos(); };
	D3DXVECTOR3					GetRotPlayer() { return this[0].GetRot(); };

	D3DXVECTOR3					FrontRotTOaxis;							//!< �n�`�@���ƃv���C���[Front�x�N�g���̊O�ϒl
	float						Brot;									//!< Front�x�N�g������n�`�@���ւ̉�]�p�x
	float						speed;									//!< �ړ��X�s�[�h
	float						speedbuff;								//!< �ړ��X�s�[�h�A�C�e���o�t
	float						speedbufftime;							//!< �ړ��X�s�[�h�A�C�e�����ʎ���
	float						MorphingTime;							//!< ���[�t�B���O�ł��鎞��
	float						BackCameraItemTime;						//!< �o�b�N�J�����A�C�e���̌��ʎ���
	float						KiriItemTime;							//!< ���A�C�e���̌��ʎ���
	float						AmmoBornTime;							//!< �c�e�񕜃J�E���g�B���Ԃŉ�
	float						time;									//!< ���[�t�B���O���ɕ�ԃ^�C���Ƃ��Ďg�p
	int							MorphingSignal;							//!< 1,�Ȃɂ����Ȃ��@2,���[�t�B���O���@3,���[�t�B���O����
	int							vital;									//!< �̗�
	int							oldvital;								//!< �̗�
	int							AmmoCnt;								//!< �c�e���B
	int							ModelType;								//!< �ǂ̃��f����Ԃ�����
	int							OldModelType;							//!< �ǂ̃��f����Ԃ�����
	int							shadowIdx;								//!< �e�̃C���f�b�N�X�ԍ�
	bool						GetMorphing;							//!< �ό`����
	bool						Morphing;								//!< �ό`����
	bool						MorphingEnd;							//!< �ό`�I������
	bool						speedbuffsignal;						//!< �X�s�[�h�A�b�v�o�t����
	bool						BackCameraItemSignal;					//!< �o�b�N�J�����A�C�e�����g�p���Ă��邩�ǂ���
	bool						KiriSignal;								//!< ���A�C�e�����g�p���Ă��邩�ǂ���
	bool						dashFlag;								//!< �_�b�V������@���͂������true.����ȊO��false

	//�ύX�������m�F
	//�`�F�b�N���� pos,rot(�C��C���C�g),Morphing,vital
	bool NetChkPos = false;
	bool NetChkHoudaiRot = false;
	bool NetChkHoutouRot = false;
	bool NetChkHousinRot = false;
	bool NetChkvital[4] = { false };
	bool NetChkMorphing = false;

};


