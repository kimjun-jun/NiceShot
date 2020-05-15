//=============================================================================
//
// �v���C���[���� [player.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************


struct PLAYER_HONTAI
{
	ALLCHARACTER				AllParameter;				// �W���p�����[�^
	ONECHARACTER				OneParameter;				// �σp�����[�^
	PLAYER_HONTAI				*Parent;					// �e�q����
	GPUMODEL					Model;						// ���f���\����
	bool						BackCameraItemSignal;		// �o�b�N�J�����A�C�e�����g�p���Ă��邩�ǂ���
	bool						KiriSignal;					// ���A�C�e�����g�p���Ă��邩�ǂ���
	float						BackCameraItemTime;			// �o�b�N�J�����A�C�e���̌��ʎ���
	float						KiriItemTime;				// ���A�C�e���̌��ʎ���
	float						AmmoBornCnt;				// �c�e�񕜃J�E���g�B���Ԃŉ�
	int							AmmoNum;					// �c�e���B�ő�5��
	int							ModelType;					// �ǂ̃��f����Ԃ�����
};

struct PLAYER_PRATS
{
	ALLCHARACTER				AllParameter;		// �W���p�����[�^
	PLAYER_HONTAI				*ParentHontai;		// �e�q����
	PLAYER_PRATS				*ParentParts;		// �e�q����
	GPUMODEL					Model;				// ���f���\����
	bool						CameraKey;			// �J��������𔻒�,true�v���C���[�Ɏ����ǔ������邩����,false�J�����̉�]�̓v���C���[���s��
	float						AmmoBornCnt;		// �c�e�񕜃J�E���g�B���Ԃŉ�
	int							AmmoNum;			// �c�e���B�ő�5��
	int							ModelType;			// �ǂ̃��f����Ԃ�����
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
void SetPlayerMeshColor(GPUMODEL *model, int type);

