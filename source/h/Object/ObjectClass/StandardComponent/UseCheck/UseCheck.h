/**
* @file TRANSFORM.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
 * @enum UseType
 * �g�p�^�C�v�̎��
 */
enum eUse_Type
{
	NullUse = -1,
	NoUse = 0,
	YesUseType1 = 1,
	YesUseType2,
	YesUseType3,
	YesUseType4,
};

/**
 * @class bUseCheck
 * @brief �g�p����N���X�@bool�^
 */
class bUseCheck
{
public:
	bUseCheck() { use = false; }
	~bUseCheck() {}
	//------------------------get�֐�
	inline bool Use() const { return this->use; };

	//------------------------set�֐�
	inline void Use(const bool use) { this->use = use; };

private:
	bool		use;		//!< �g�p����

};

/**
 * @class iUseCheck
 * @brief �g�p����N���X�@int�^(0�Ŗ��g�p�A����ȊO�ŊY���e�N�X�`���g�p�AUseType�Q��)
 */
class iUseCheck
{
public:
	iUseCheck() { use = NullUse; }
	~iUseCheck() {}
	//------------------------get�֐�
	inline eUse_Type Use() const { return this->use; };

	//------------------------set�֐�
	inline void Use(const eUse_Type UseType) { this->use = UseType; };

private:
	eUse_Type		use;		//!< �g�p����

};
