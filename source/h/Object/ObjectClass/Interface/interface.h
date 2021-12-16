/**
* @file interface.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

//�e�N���X�̑O�u�錾
class GAME_OBJECT_INSTANCE;

template <typename T>
inline void NewObject(T *p)
{
	p = new T;
}

/**
 * @class GAME_OBJECT_INTERFACE_INIT
 * @brief �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 * @details �h���N���X�ŃI�[�o���C�h�K�{(�v���C���[��A�C�e���Ȃǂ̑S�I�u�W�F�N�g���p��)
 * @details �����������C���^�[�t�F�[�X
 */
class GAME_OBJECT_INTERFACE_INIT
{
public:
	GAME_OBJECT_INTERFACE_INIT() {}
	virtual ~GAME_OBJECT_INTERFACE_INIT() {}
	virtual void Init(void) = 0;		//!< ������(���l�Ȃ�)
	virtual void InitNet(void) = 0;		//!< ������(�l�b�g�ΐ펞��p�̏�����)

private:
};

/**
 * @class GAME_OBJECT_INTERFACE_UPDATE
 * @brief �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 * �h���N���X�ŃI�[�o���C�h�K�{(�v���C���[��A�C�e���Ȃǂ̑S�I�u�W�F�N�g���p��)
 * �X�V�����C���^�[�t�F�[�X
 */
class GAME_OBJECT_INTERFACE_UPDATE
{
public:
	GAME_OBJECT_INTERFACE_UPDATE() {}
	virtual ~GAME_OBJECT_INTERFACE_UPDATE() {}
	virtual void Update(void) = 0;		//!< �X�V
private:
};

/**
 * @class GAME_OBJECT_INTERFACE_DRAW
 * @brief �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 * �h���N���X�ŃI�[�o���C�h�K�{(�v���C���[��A�C�e���Ȃǂ̑S�I�u�W�F�N�g���p��)
 * �`�揈���C���^�[�t�F�[�X
 * �`�惋�[�v�̉񐔂��擾����@���4����������4���[�v����
 */
class GAME_OBJECT_INTERFACE_DRAW
{
public:
	GAME_OBJECT_INTERFACE_DRAW() {}
	virtual ~GAME_OBJECT_INTERFACE_DRAW() {}
	virtual void Draw(void) = 0; //!< �`��

private:
};

/**
 * @class GAME_OBJECT_INTERFACE_ADDRESSOR
 * @brief �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 * �h���N���X�ŃI�[�o���C�h�K�{(�v���C���[��A�C�e���Ȃǂ̑S�I�u�W�F�N�g���p��)
 * �h���N���X�ԃA�h���X�擾�����C���^�[�t�F�[�X
 */
class GAME_OBJECT_INTERFACE_ADDRESSOR
{
public:
	GAME_OBJECT_INTERFACE_ADDRESSOR() {}
	virtual ~GAME_OBJECT_INTERFACE_ADDRESSOR() {}
	virtual void Addressor(GAME_OBJECT_INSTANCE *obj) = 0;		//!< �A�h���X�擾
private:
};

/**
 * @class GAME_OBJECT_INTERFACE_SUMMRY
 * @brief �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 * �h���N���X�ŃI�[�o���C�h�K�{(�v���C���[��A�C�e���Ȃǂ̑S�I�u�W�F�N�g���p��)
 */
class GAME_OBJECT_INTERFACE_SUMMRY :protected GAME_OBJECT_INTERFACE_INIT, protected GAME_OBJECT_INTERFACE_UPDATE, protected GAME_OBJECT_INTERFACE_DRAW, protected GAME_OBJECT_INTERFACE_ADDRESSOR
{
public:
	GAME_OBJECT_INTERFACE_SUMMRY(void) { AllInheritanceCnt++; };
	~GAME_OBJECT_INTERFACE_SUMMRY(void) { AllInheritanceCnt--; };

	virtual void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< �A�h���b�T�[
	virtual void Init(void) override {};		//!< ������(���l�Ȃ�)
	virtual void InitNet(void) override {};		//!< �������l�b�g�ΐ�p�ɕύX���K�v�ȂƂ��Ŏg�p
	virtual void Update(void) override {};		//!< �X�V
	virtual void Draw(void) override {};		//!< �`��

private:
	static int AllInheritanceCnt;	//!< �R���X�g���N�g�E�f�X�g���N�g���Ă΂��Ƃ��ɕK���v������
};

