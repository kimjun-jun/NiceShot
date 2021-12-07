/**
* @file Move.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
 * @class MOVE
 * @brief �ړ��N���X
 */
class MOVE
{
public:
	MOVE() { move = VEC3_ALL0; }
	~MOVE() {}

	//------------------------get�֐�
	inline D3DXVECTOR3 Move(void) { return move; };
	//------------------------set�֐�
	inline void Move(const D3DXVECTOR3 InMove) { move = InMove; };

private:
	D3DXVECTOR3 move;
};

/**
 * @class PLAYER_PARAMETER_MOVE
 * @brief �ړ��N���X
 */
class PLAYER_PARAMETER_MOVE
{
public:
	PLAYER_PARAMETER_MOVE() { move = VEC3_ALL0; }
	~PLAYER_PARAMETER_MOVE() {}

	//------------------------get�֐�
	inline D3DXVECTOR3 Move(void) { return move; };
	//------------------------set�֐�
	inline void Move(const D3DXVECTOR3 InMove) { move = InMove; };

	//------�ړ�����
	//�g�p
	void MoveKeybord(PLAYER *Player, int CntPlayer);				//!< �ړ�����(ki-bo-do�ňړ�����)
	void MoveL(PLAYER *Player, int CntPlayer, bool Netflag);		//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	//���g�p
	//void MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag);	//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	//void MoveABL(int CntPlayer, EFFECT *effect, bool Netflag);		//!< �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
	//void MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag);		//!< �ړ�����(L2R2�ňړ�����)

private:
	D3DXVECTOR3 move;
};
