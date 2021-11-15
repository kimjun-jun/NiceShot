/**
* @file collision.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
* @brief BB2D�֐�	CollisionBB2D
* @param[in] pos1			�Ώۍ��W 1
* @param[in] h1				�Ώۍ��� 1
* @param[in] w1				�Ώۉ��� 1
* @param[in] pos2			�Ώۍ��W 2
* @param[in] h2				�Ώۍ��� 2
* @param[in] w2				�Ώۉ��� 2
* @return bool true;�������Ă��� false:�������Ă��Ȃ�
* @details �����蔻��p
* @author : �ؑ���(�L�����W����)
*/
bool CollisionBB2D(D3DXVECTOR3 pos1, float h1, float w1, D3DXVECTOR3 pos2, float h2, float w2);

/**
* @brief BB3D�֐�	CollisionBB3D
* @param[in] pos1			�Ώۍ��W 1
* @param[in] size1			�ΏۃT�C�Y 1
* @param[in] pos2			�Ώۍ��W 2
* @param[in] size2			�ΏۃT�C�Y 2
* @return bool true;�������Ă��� false:�������Ă��Ȃ�
* @details �����蔻��p
* @author : �ؑ���(�L�����W����)
*/
bool CollisionBB3D(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);

/**
* @brief BC�֐�	CollisionBC
* @param[in] pos1			�Ώۍ��W 1
* @param[in] r1				�Ώ۔��a 1
* @param[in] pos2			�Ώۍ��W 2
* @param[in] r2				�Ώ۔��a 2
* @return bool true;�������Ă��� false:�������Ă��Ȃ�
* @details �����蔻��p
* @author : �ؑ���(�L�����W����)
*/
bool CollisionBC(D3DXVECTOR3 pos1, float r1, D3DXVECTOR3 pos2, float r2);
