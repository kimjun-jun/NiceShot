/**
* @file player.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************


class PLAYER_HONTAI : public ALLCHARACTER , public ONECHARACTER, public GPUMODEL
{
public:
	PLAYER_HONTAI				*Parent;								//!< �e�q����
	D3DXVECTOR3					bulletmove;								//!< �o���b�g�i�s����
	D3DXVECTOR3					BposStart;								//!< �o���b�g�̔��ˈʒu
	D3DXVECTOR3					BmoveRot;								//!< �e���i�s����p�x
	D3DXVECTOR3					movepos;								//!< ���f���̈ړ�����(��)
	D3DXVECTOR3					moverot;								//!< ���f���̉�]����(��)
	bool						BackCameraItemSignal;					//!< �o�b�N�J�����A�C�e�����g�p���Ă��邩�ǂ���
	bool						KiriSignal;								//!< ���A�C�e�����g�p���Ă��邩�ǂ���
	bool						dashFlag;								//!< �_�b�V������@���͂������true.����ȊO��false
	float						BackCameraItemTime;						//!< �o�b�N�J�����A�C�e���̌��ʎ���
	float						KiriItemTime;							//!< ���A�C�e���̌��ʎ���
	float						AmmoBornCnt;							//!< �c�e�񕜃J�E���g�B���Ԃŉ�
	int							vital;									//!< �̗�
	int							AmmoNum;								//!< �c�e���B
	int							ModelType;								//!< �ǂ̃��f����Ԃ�����
	int							RidePolygonNum;							//!< �ǂ̒n�`�|���S���ɏ���Ă��邩����
	void						SetMoveABL(int CntPlayer);				//!< �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
	void						SetCamera(int CntPlayer);				//!< �J�����X�V
	void						SetMoveL(int CntPlayer);				//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	void						SetCameraR(int CntPlayer);				//!< �J��������(LR�X�e�B�b�N�ňړ�����)
	void						SetQ(int CntPlayer);					//!< �N�H�[�^�j�I������
	void						SetBulletALL(int CntPlayer);			//!< �o���b�g�֘A����
	void						SetKiri(int CntPlayer);					//!< �t�H�O����
	void						SetMorphing(int CntPlayer);				//!< ���[�t�B���O����
};

class PLAYER_PRATS : public ALLCHARACTER, public GPUMODEL
{
public:
	PLAYER_HONTAI				*ParentHontai;		// �e�q����
	PLAYER_PRATS				*ParentParts;		// �e�q����
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
HRESULT ReInitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void UpdateEnemy(void);
void DrawPlayer(void);
PLAYER_HONTAI *GetPlayerHoudai(void);
PLAYER_PRATS *GetPlayerHoutou(void);
PLAYER_PRATS *GetPlayerHousin(void);
D3DXVECTOR3 GetPlayerBulletStartPos(int PlayerType);
void SetPlayerMeshColor(GPUMODEL *model, int type);

