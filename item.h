/**
* @file item.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ITEM_SMALL_SCL				0.01f			//�A�C�e���擾�オ�A�C�e�������񂾂񏬂����Ȃ�l
#define ITEM_DELETE_SCL				0.5f			//�A�C�e����������l

//**************************************
// ���
//**************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3					pos;				// ���݂̈ʒu
	D3DXVECTOR3					scl;				//!< ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3					rot;				// ���݂̌���
	D3DXVECTOR3					Upvec;				// ���f���̏����
	D3DXVECTOR3					rotVecAxis;			//�����̎��Ɏg���n�`�̖@���x�N�g��
	D3DXVECTOR3					rotTOaxis;			// �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	float						Qrot;				// Up�x�N�g������n�`�@���ւ̉�]�p�x
	float						fRadius;			// ���a
	float						Droptime;			// �o������܂ł̎���
	int							nIdxShadow;			// �eID
	int							nType;				// ���
	int							GoukeiDrop;			// �h���b�v���v
	int							GetPlayerType;		// �ǂ̃v���C���[���擾������
	bool						bUse;				// �g�p���Ă��邩�ǂ���
	bool						GettingSignal;		// �v���C���[���A�C�e�����擾�������ǂ����@�^:�擾���@�U:�擾���Ă��Ȃ�
	bool						GettingSignalEnd;	// �v���C���[���A�C�e�����擾�I���������ǂ����@�^:�I���@�U:�I�����Ă��Ȃ��A�������͎擾���Ă��Ȃ�
	bool						fCollisionEnd;		// �n�`�Ƃ̓����蔻�肪�I��������ǂ���
} ITEM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, int nType);
void DeleteItem(int nIdxItem);
ITEM *GetItem(void);
void GettingItem(int nIdxItem);

