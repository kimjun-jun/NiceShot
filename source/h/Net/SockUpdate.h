/**
* @file SockUpdate.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
*�@@class SOCK_UPDATE
*�@@brief �\�P�b�g�N���X�̍X�V�Ǘ�
*/
class SOCK_UPDATE
{
public:
	SOCK_UPDATE() {};	//!< �R���X�g���N�^�@���Ɏg�p���Ȃ�
	~SOCK_UPDATE() {};	//!< �f�X�g���N�^�@�@���Ɏg�p���Ȃ�

	void UpdateFunc(MySOCKET *MySockObj);
private:
	//�l�b�g�ΐ�J�n����
	void NetMatch(MySOCKET *MySockObj);
	void NetMyNumberGet(MySOCKET *MySockObj);
	void NetItemGet(MySOCKET *MySockObj);
	void NetCountdown(MySOCKET *MySockObj);

};
