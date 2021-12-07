/**
* @file Attack.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
*�@@class PLAYER_PARAMETER_BULLET
*�@@brief �v���C���[�o���b�g�p�p�����[�^
*/
class PLAYER_PARAMETER_BULLET
{
public:
	PLAYER_PARAMETER_BULLET() {
		BulletStartPos = VEC3_ALL0;	for (int i = 0; i < 3; i++) BulletMove[i] = VEC3_ALL0;
		BulletRotY = 0.0f; BulletBornTime = 0.0f; BulletStock = 0; NetBulletShotFlagOneFrame = 0;
	};
	~PLAYER_PARAMETER_BULLET() {}

	//------�o���b�g����
	//�g�p
	void BulletALL(PLAYER *Player, int CntPlayer, bool Netflag);	//!< �o���b�g�֘A����
	//���g�p
	//void BulletALLMoveL2R2Ver(PLAYER *Player, int CntPlayer);		//!< �o���b�g�֘A���� ���˃{�^�����\���J�[�\��

	//------------------------get�֐�
	inline D3DXVECTOR3 GetBulletStartPos(void) { return BulletStartPos; };
	inline D3DXVECTOR3 *GetBulletMove(void) { return &BulletMove[0]; };
	inline float GetBulletRotY(void) { return BulletRotY; };
	inline float GetBulletBornTime(void) { return BulletBornTime; };
	inline int GetBulletStock(void) { return BulletStock; };
	inline int GetNetBulletShotFlagOneFrame(void) { return NetBulletShotFlagOneFrame; };
	//------------------------set�֐�
	inline void SetBulletStartPos(const D3DXVECTOR3 InPos) { BulletStartPos = InPos; };
	inline void SetBulletMove(const D3DXVECTOR3 InMove,int Indx) { BulletMove[Indx] = InMove; };
	inline void SetBulletRotY(const float InRotY) { BulletRotY = InRotY; };
	inline void SetBulletBornTime(const float InTime) { BulletBornTime = InTime; };
	inline void SetBulletStock(const int InStockNum) { BulletStock = InStockNum; };
	inline void SetNetBulletShotFlagOneFrame(const int InFlag) { NetBulletShotFlagOneFrame = InFlag; };

private:
	D3DXVECTOR3		BulletStartPos;				//!< �o���b�g�̔��˒n�_
	D3DXVECTOR3		BulletMove[3];				//!< �o���b�g�̈ړ��ʁ@�p���[�A�b�v���͎O�����ɔ���
	float			BulletRotY;					//!< �o���b�g��Front�x�N�g������n�`�@���ւ̉�]�p�x
	float			BulletBornTime;				//!< �c�e�񕜃J�E���g�B���Ԃŉ� BULLET_BORN_MAXTIME
	int				BulletStock;				//!< �c�e��
	int				NetBulletShotFlagOneFrame;	//!< �l�b�g�ΐ�p�@�o���b�g�𔭎˂��Ă��Ȃ�0,�ʏ탂�f���͔���1,�A�^�b�N���f���͔���3�@���t���[���m�F���āA�o���b�g���˂���ƃt���O�𗧂Ă�
};
