/**
* @file tutorial.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

/**
*�@@class TUTO
*�@@brief 2D�|���S�����`����\����
*/
class TUTO : public OBJECT_2D
{
public:
	TUTO() { cnt = 0; };
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	HRESULT						MakeVertexTutorial(void);
	int							cnt;						//!< �`�攻��p�@2�Ŋ������]�蔻��@0�\���@1��\���@����̃e�N�X�`����񂪑����邱�Ƃ���������int��`
};

