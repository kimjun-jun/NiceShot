/**
* @file collision.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/

#include "../../h/main.h"
#include "../../h/Collision/collision.h"


//=============================================================================
// �����蔻��BB2D�@xz��
//=============================================================================
bool CollisionBB2D(D3DXVECTOR3 pos1, float h1, float w1, D3DXVECTOR3 pos2, float h2, float w2)
{
	if (pos1.x + w1 >= pos2.x - w2
		&& pos1.x - w1 <= pos2.x + w2
		&& pos1.z + h1 >= pos2.z - h2
		&& pos1.z - h1 <= pos2.z + h2)
	{
		//�������Ă���
		return true;
	}
	//�������Ă��Ȃ�
	return false;
}

//=============================================================================
// �����蔻��BB3D
//=============================================================================
bool CollisionBB3D(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{
	if (pos1.x + size1.x >= pos2.x - size2.x && pos1.x - size1.x <= pos2.x + size2.x
		&& pos1.y + size1.y >= pos2.y - size2.y  && pos1.y - size1.y <= pos2.y + size2.y
		&& pos1.z + size1.z >= pos2.z - size2.z  && pos1.z - size1.z <= pos2.y + size2.z)
	{
		//�������Ă���
		return true;
	}
	//�������Ă��Ȃ�
	return false;
}

//=============================================================================
// �����蔻��BC
//=============================================================================
bool CollisionBC(D3DXVECTOR3 pos1, float r1, D3DXVECTOR3 pos2, float r2)
{
	//��1 �v���C���[�̒��S����G�l�~�[�̒��S�܂ł̋�����player_ballett�ɑ��
	D3DXVECTOR3 distance_pos = pos1 - pos2;

	//��2 �v���C���[�T�[�N��(�{�f�B)�̔��a
	//player��pos(D3DXVECTOR3�Ȃ̂�3����)��x���W���瓖���蔻��p�e�N�X�`���T�C�Y(�v���C���[���a)�������������l��player_r(�v���C���[���a)�ɑ��
	float wr1 = r1;
	//��2 �G�l�~�[�T�[�N��(�{�f�B)�̔��a
	float wr2 = r2;

	//D3DXVec2LengthSq()�֐���distance_pos(3����)��2�悵�Ă���
	float dista = D3DXVec3LengthSq(&distance_pos);

	float wr3 = (wr1 + wr2)*(wr1 + wr2);

	//�����蔻��
	if (dista <= wr3)
	{
		//�������Ă���
		return true;
	}
	return false;
}



