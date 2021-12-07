/**
* @file TRANSFORM.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
 * @class TRANSFORM
 * @brief ���W�A��]�A�X�P�[���N���X
 */
class TRANSFORM
{
public:
	TRANSFORM() {
		pos = VEC3_ALL0; oldpos = VEC3_ALL0;
		rot = VEC3_ALL0; oldrot = VEC3_ALL0;
		scl = VEC3_ALL1;
	}
	~TRANSFORM() {}

	//------------------------get�֐�
	inline D3DXVECTOR3 Pos() const { return this->pos; };
	inline D3DXVECTOR3 OldPos() const { return this->oldpos; };
	inline D3DXVECTOR3 Rot() const { return this->rot; };
	inline D3DXVECTOR3 OldRot() const { return this->oldrot; };
	inline D3DXVECTOR3 Scl() const { return this->scl; };

	//------------------------set�֐�
	inline void Pos(const D3DXVECTOR3 pos) { this->oldpos = this->pos; this->pos = pos; };
	inline void Rot(const D3DXVECTOR3 rot) { this->oldrot = this->rot; this->rot = rot; };
	inline void Scl(const D3DXVECTOR3 scl) { this->scl = scl; };

private:
	D3DXVECTOR3		pos;				//!< �ʒu
	D3DXVECTOR3		oldpos;				//!< �O�t���[���̈ʒu
	D3DXVECTOR3		rot;				//!< ����(��])
	D3DXVECTOR3		oldrot;				//!< �O�t���[���̌���(��])
	D3DXVECTOR3		scl;				//!< �傫��(�X�P�[��)

};

