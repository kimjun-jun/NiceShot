/**
* @file Posture.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/

#include "../../../../../h/main.h"
#include "../../../../../h/Object/ObjectClass/StandardComponent/Posture/Posture.h"


//=============================================================================
// �N�H�[�^�j�I������
//=============================================================================
void POSTURE::Quaternion(void)
{
	//---------------------------------------------------------�I�u�W�F�N�g�l�Ăяo��
	D3DXVECTOR3 POSTUREVec = this->FNVecFunc();
	D3DXVECTOR3 POSTUREUpNorCross = this->FNUNCrossFunc();
	float Qrot = this->QrotFunc();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�n�`�̊p�x�ƃv���C���[�̊p�x���v�Z�Bdraw�ŃN�I�[�^�j�I���Ŏg��
	D3DXVec3Cross(&POSTUREUpNorCross, &POSTUREVec, &UpVec);

	//�n�`�@����Up�x�N�g���œ��ς��Ƃ�(���ɑ΂����]�p�x�����߂�)
	float Upkakezan = D3DXVec3Dot(&POSTUREVec, &UpVec);

	//������]�p�x��0�ȊO(���s�łȂ�)�Ȃ�Ή�]�p�x�Ƃ����߂�
	//�Ƃ̓N�H�[�^�j�I���̔C�ӎ�����]�����鎞�̉�]�p�x�ɂȂ�
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(POSTUREVec.x*POSTUREVec.x +
				POSTUREVec.y *POSTUREVec.y +
				POSTUREVec.z * POSTUREVec.z);
		Qrot = acosf(cossita);
	}
	else Qrot = 0.0f;

	//---------------------------------------------------------�I�u�W�F�N�g�l�Z�b�g
	this->QrotFunc(Qrot);
	this->FNUNCrossFunc(POSTUREUpNorCross);

}

