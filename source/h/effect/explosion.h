/**
* @file explosion.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


//**************************************
// ���
//**************************************
enum
{
	EXPLOSIONTYPE_BULLET_PLAYER = 0,	// �v���C���[�̒e�̔���
	EXPLOSIONTYPE_BULLET_ENEMY,			// �G�̒e�̔���
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class EXPLOSION : public OBJECT_3D
{
public:
	EXPLOSION() { nCounter = 0, nPatternX = 0, nPatternY = 0, nType = 0, fSizeX = 0.0f, fSizeY = 0.0f; };
	void						Init(void);					//!< ������
	void						Reinit(void);				//!< �ď�����
	void						Uninit(void);				//!< �I��
	void						Update(void);				//!< �X�V
	void						Draw(void);					//!< �`��
	HRESULT						MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
	void						SetVertexExplosion(int nIdxBullet, float fSizeX, float fSizeY);
	void						SetColorExplosion(int nIdxExplosion, D3DXCOLOR col);
	void						SetTextureExplosion(int nIdxExplosion, int nPatternX, int nPatternY);
	int							SetExplosion(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType, D3DXCOLOR col);

	int							nCounter;			//!< �J�E���^�[
	int							nPatternX;			//!< �p�^�[��No.X
	int							nPatternY;			//!< �p�^�[��No.Y
	int							nType;				//!< ���
	float						fSizeX;				//!< �T�C�YX
	float						fSizeY;				//!< �T�C�YY
} ;



