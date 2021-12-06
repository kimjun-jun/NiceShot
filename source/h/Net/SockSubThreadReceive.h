/**
* @file SockSubThreadReceive.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
 * @enum e_VariableAnalysType
 * ��͗p�萔�@�Ή�������͏����𔻕ʂ���
 */
enum e_VariableAnalysType
{
	VariableAnalysType_Pos,					//!< 0�@���W
	VariableAnalysType_HoudaiRot,			//!< 1�@�C��rot
	VariableAnalysType_HoutouRot,			//!< 2�@�C��rot
	VariableAnalysType_HousinRot,			//!< 3�@�C�grot
	VariableAnalysType_Vital,				//!< 4�@�̗�
	VariableAnalysType_Morphing,			//!< 5�@�ό`
	VariableAnalysType_BulletA,				//!< 6�@�ʏ픭��
	VariableAnalysType_BulletB,				//!< 7�@�g�U����
	VariableAnalysType_MAX
};

/**
*�@@class SOCK_SUBTHREAD_RECEIVE
*�@@brief �\�P�b�g�N���X�̃}���`�X���b�hReceive�Ǘ��N���X
*/
class SOCK_SUBTHREAD_RECEIVE
{
public:
	SOCK_SUBTHREAD_RECEIVE() {};	//!< �R���X�g���N�^�@���Ɏg�p���Ȃ�
	~SOCK_SUBTHREAD_RECEIVE() {};	//!< �f�X�g���N�^�@�@���Ɏg�p���Ȃ�

	void ReceiveFunc(MySOCKET *MySockObj);
private:
	void ReceivePacket(MySOCKET *MySockObj);	//��M����
	void ObjectAnalys(MySOCKET *MySockObj,char* argRMsg);			//��͏����@��ʏ����@�I�u�W�F�N�g����
	void VariableAnalys(MySOCKET *MySockObj, char* argVariableMsg, int PlayerNum, char* PlayerType);		//��͏����@���ʏ����@�Ή��ϐ�����
	void XYZValue1Analys(MySOCKET *MySockObj,char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3
	void XYZValue2Analys(MySOCKET *MySockObj,char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3 *2
	void XYZValue4Analys(MySOCKET *MySockObj,char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//��͏����@���ʏ����@���l�f�[�^���ʁ@float *3 *4
	void INTValueAnalys(MySOCKET *MySockObj, char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);			//��͏����@���ʏ����@���l�f�[�^���ʁ@int *1
};
