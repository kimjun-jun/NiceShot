/**
* @file player.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

class OBJECT_3D;
class PLAYER_PRATS;
struct GPUMODEL;

/**
 * @enum ITEMTYPE
 * �A�C�e���萔
 */
enum PARTSTYPE
{
	PARTSTYPE_HOUTOU = 0,		//!< �C��
	PARTSTYPE_HOUSIN,			//!< �C�g
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
		AmmoCnt = 0;
		ModelType = 0;
		shadowIdx = 0;
		Morphing = false;
		MorphingEnd = false;
		speedbuffsignal = false;
		BackCameraItemSignal = false;
		KiriSignal = false;
		dashFlag = false;
	}


	//PLAYER_HONTAI *GetPlayerPointer() { return &this[0]; }	//!< �擪�A�h���X�擾
	//void *SetPlayerPointer(int cnt) { this[cnt]; }			//!< �A�h���X�Z�b�g



	PLAYER_PRATS				parts[2];					//!< ���f���̃p�[�c�K�w�@�e�{��(�C��)�A�q�p�[�c[0](�C��)�A���p�[�c[1](�C�g)
	PLAYER_HONTAI				*Parent;					//!< �e�q����
	GPUMODEL					ModelDate[2];				//!< ���f���f�[�^�@0�m�[�}���A1�U���A2�X�s�[�h
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	void						SetPlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff,DWORD nNumPolygon, int CntPlayer);
	void						SetMoveABL(int CntPlayer);				//!< �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
	void						SetCamera(int CntPlayer);				//!< �J�����X�V
	void						SetMoveL(int CntPlayer);				//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	void						SetCameraR(int CntPlayer);				//!< �J��������(LR�X�e�B�b�N�ňړ�����)
	void						SetMoveL2R2(int CntPlayer);				//!< �ړ�����(L2R2�ňړ�����)
	void						SetQ(int CntPlayer);					//!< �N�H�[�^�j�I������
	void						SetBulletALL(int CntPlayer);			//!< �o���b�g�֘A����
	void						SetBulletALLMoveL2R2Ver(int CntPlayer);	//!< �o���b�g�֘A���� ���˃{�^�����\���J�[�\��
	void						SetKiri(int CntPlayer);					//!< �t�H�O����
	void						SetMorphing(int CntPlayer);				//!< ���[�t�B���O����


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
	int							AmmoCnt;								//!< �c�e���B
	int							ModelType;								//!< �ǂ̃��f����Ԃ�����
	int							shadowIdx;								//!< �e�̃C���f�b�N�X�ԍ�
	bool						Morphing;								//!< �ό`����
	bool						MorphingEnd;							//!< �ό`�I������
	bool						speedbuffsignal;						//!< �X�s�[�h�A�b�v�o�t����
	bool						BackCameraItemSignal;					//!< �o�b�N�J�����A�C�e�����g�p���Ă��邩�ǂ���
	bool						KiriSignal;								//!< ���A�C�e�����g�p���Ă��邩�ǂ���
	bool						dashFlag;								//!< �_�b�V������@���͂������true.����ȊO��false

};

class PLAYER_PRATS : public OBJECT_3D
{
public:
	PLAYER_HONTAI				*ParentHontai;		// �e�q����
	PLAYER_PRATS				*ParentParts;		// �e�q����
};
