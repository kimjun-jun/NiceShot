/**
* @file SockSubThreadSend.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
*�@@class SOCK_SUBTHREAD_SEND
*�@@brief �\�P�b�g�N���X�̃}���`�X���b�hsend�Ǘ��N���X
*/
class SOCK_SUBTHREAD_SEND
{
public:
	SOCK_SUBTHREAD_SEND() {};	//!< �R���X�g���N�^�@���Ɏg�p���Ȃ�
	~SOCK_SUBTHREAD_SEND() {};	//!< �f�X�g���N�^�@�@���Ɏg�p���Ȃ�

	void SendFunc(MySOCKET *MySockObj);
private:
	//�l�b�g�ΐ풆���s����
	void SendPacket(MySOCKET *MySockObj);						//���M����
};
