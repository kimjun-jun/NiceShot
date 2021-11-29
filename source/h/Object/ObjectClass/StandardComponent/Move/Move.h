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

	//------�ړ�����
	//�g�p
	void MoveKeybord(int CntPlayer, PLAYER *Player);				//!< �ړ�����(ki-bo-do�ňړ�����)
	void MoveL(int CntPlayer, PLAYER *Player, bool Netflag);		//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	//���g�p
	//void MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag);	//!< �ړ�����(LR�X�e�B�b�N�ňړ�����)
	//void MoveABL(int CntPlayer, EFFECT *effect, bool Netflag);		//!< �ړ�����(AB�{�^��L�X�e�B�b�N�ňړ�����)
	//void MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag);		//!< �ړ�����(L2R2�ňړ�����)

private:
	D3DXVECTOR3 move;
};
