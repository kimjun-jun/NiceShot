/**
* @file Posture.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
 * @class POSTURE
 * @brief �N�H�[�^�j�I���Ŏg���n�`�@���⋅�ʉ�]�N���X
 */
class POSTURE
{
public:
	POSTURE() { FNVec = VEC3_ALL0; FNUNCross = VEC3_ALL0; Qrot = 0.0f; }
	~POSTURE() {}
	//------------------------get�֐�
	inline D3DXVECTOR3 FNVecFunc() const { return this->FNVec; };				//!< �N�H�[�^�j�I���Ŏg���n�`�̖@���x�N�g��
	inline D3DXVECTOR3 FNUNCrossFunc() const { return this->FNUNCross; };		//!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	inline float QrotFunc() const { return this->Qrot; };						//!< Up�x�N�g������n�`�@���ւ̉�]�p�x

	//------------------------set�֐�
	inline void FNVecFunc(const D3DXVECTOR3 POSTUREVec) { this->FNVec = POSTUREVec; };						//!< �N�H�[�^�j�I���Ŏg���n�`�̖@���x�N�g��
	inline void FNUNCrossFunc(const D3DXVECTOR3 POSTUREUpNorCross) { this->FNUNCross = POSTUREUpNorCross; }  //!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	inline void QrotFunc(const float Qrot) { this->Qrot = Qrot; };												//!< Up�x�N�g������n�`�@���ւ̉�]�p�x

	//------�p������
	void Quaternion(void);		//!< �N�H�[�^�j�I������@�n�`�@����UP�x�N�g���ƊO��

private:
	D3DXVECTOR3					FNVec;				//!< �����̂���n�`�@���x�N�g��
	D3DXVECTOR3					FNUNCross;			//!< �n�`�@���Ǝ�����Up�x�N�g���̊O�ϒl
	float						Qrot;				//!< Up�x�N�g������n�`�@���ւ̊p�x

};

