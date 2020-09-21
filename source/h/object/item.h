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
#define ITEM_SMALL_SCL				0.03f			//�A�C�e���擾�オ�A�C�e�������񂾂񏬂����Ȃ�l
#define ITEM_DELETE_SCL				0.5f			//�A�C�e����������l

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ITEM : public OBJECT_3D
{
public:
	ITEM() {
		Droptime = 0.0f, nType, GoukeiDrop = 0, GetPlayerType = 0,
			GettingSignal = false, GettingSignalEnd = false, CollisionFieldEnd = false;
	};
	void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, int nType);
	void DeleteItem(int nIdxItem);
	void GettingItem(int nIdxItem);
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	PLAYER_HONTAI				*p;						//!<�v���C���[�擪�A�h���X
	float						Droptime;				//!< �o������܂ł̎���
	int							nIdxShadow;				//!< �eID
	int							nType;					//!< ���
	int							GoukeiDrop;				//!< �h���b�v���v
	int							GetPlayerType;			//!< �ǂ̃v���C���[���擾������
	bool						GettingSignal;			//!< �v���C���[���A�C�e�����擾�������ǂ����@�^:�擾���@�U:�擾���Ă��Ȃ�
	bool						GettingSignalEnd;		//!< �v���C���[���A�C�e�����擾�I���������ǂ����@�^:�I���@�U:�I�����Ă��Ȃ��A�������͎擾���Ă��Ȃ�
	bool						CollisionFieldEnd;		//!< �n�`�Ƃ̓����蔻�肪�I��������ǂ���
} ;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
