/**
* @file bullet.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../../h/main.h"
#include "../../../h/other/input.h"
#include "../../../h/Object/Camera/camera.h"
//#include "../../../h/Collision/"
#include "../../../h/Other/sound.h"
#include "../../../h/Object/Shadow/shadow.h"
#include "../../../h/Effect/effect.h"
#include "../../../h/Object/Bullet/bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr float	EFFECT_BULLET_SIZE_X{ 16.0f };		//!< �G�t�F�N�g�o���b�g�̕�
constexpr float	EFFECT_BULLET_SIZE_Y{ 16.0f };		//!< �G�t�F�N�g�o���b�g�̍���
constexpr int	EFFECT_BULLET_TIME{ 12 };			//!< �G�t�F�N�g�o���b�g�̐�������

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
BULLET::BULLET(void)
{
	//�I�u�W�F�N�g�J�E���g�A�b�v
	this->CreateInstanceOBJ();

	//�J�E���g���[�v
	for (int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		//�g�p�ݒ�
		this->iUseType[CntBullet].Use(NoUse);
	}

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
BULLET::~BULLET(void)
{
	//�I�u�W�F�N�g�J�E���g�_�E��
	this->DeleteInstanceOBJ();
}

//=============================================================================
// ����������
//=============================================================================
void BULLET::Init(void)
{
	//�J�E���g���[�v
	for(int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		//�g�p�ݒ�
		this->iUseType[CntBullet].Use(NoUse);
		this->Transform[CntBullet].Pos(VEC3_ALL0);
		this->move[CntBullet].Move(VEC3_ALL0);
		this->BulletPara[CntBullet].Gravity = 0.0f;
		this->BulletPara[CntBullet].Timer = 0;
		this->BulletPara[CntBullet].UsePlayerType = PLAYER_NONE;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void BULLET::Update(SHADOW *shadow, EFFECT *effect)
{
	for(int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		bool use = this->iUseType[CntBullet].Use();
		//�g�p���Ă�����e������A�e�A�G�t�F�N�g��ݒ肷��
		if (use)
		{
			//---------------------------------------------�I�u�W�F�N�g�l�ǂݍ���
			D3DXVECTOR3 pos = this->Transform[CntBullet].Pos();
			D3DXVECTOR3 rot = this->Transform[CntBullet].Rot();
			D3DXVECTOR3 scl = this->Transform[CntBullet].Scl();
			D3DXVECTOR3 move = this->move[CntBullet].Move();

			//�o���b�g���v���C���[�̈��͈͓��Ȃ�኱�̃z�[�~���O�@���g�p
			/*
			PLAYER_HONTAI *p = GetPlayerHoudai();

			for (int CntPlayer = 0; CntPlayer < PLAYER_MAX; CntPlayer++)
			{
				if (this[CntBullet].UsePlayerType == CntPlayer) continue;
				if (p[CntPlayer].use == false) continue;

				if (CollisionBC(p[CntPlayer].pos, PLAYER_MODEL_SIZE*5.0f, pos, BULLET_MODEL_SIZE))
				{
					if (this[CntBullet].HormingSignal == false)
					{
						this[CntBullet].HormingSignal = true;
						this[CntBullet].HormingPlayerType = CntPlayer;
					}
				}
				else
				{
					this[CntBullet].HormingSignal = false;
				}
			}
			if (this[CntBullet].HormingSignal == true)
			{
				this[CntBullet].Hormingmove = p[this[CntBullet].HormingPlayerType].pos - pos;
			}
			*/

			//�z�[�~���O�͖��g�p
			//pos.x += move.x + (this[CntBullet].Hormingmove.x / 30.0f);
			//pos.y -= this[CntBullet].Gravity + move.y - (this[CntBullet].Hormingmove.y / 10.0f);
			//pos.z += move.z + (this[CntBullet].Hormingmove.z / 30.0f);

			//�e������
			pos.x += move.x;
			pos.y -= this->BulletPara[CntBullet].Gravity + move.y;
			pos.z += move.z;

			this->BulletPara[CntBullet].Gravity += VALUE_GRAVITYADD_BULLET;
			if (this->BulletPara[CntBullet].Gravity > VALUE_GRAVITYMAX_BULLET) this->BulletPara[CntBullet].Gravity = VALUE_GRAVITYMAX_BULLET;

			//�^�C�}�[(��������)�����炷
			this->BulletPara[CntBullet].Timer--;

			//��莞�Ԍo�߂Ŗ��g�p�ɂ���
			if (this->BulletPara[CntBullet].Timer <= 0)
			{
				shadow->ReleaseInstance(this->BulletPara[CntBullet].IdxShadow);
				this->ReleaseInstance(CntBullet, &shadow[0]);
			}
			else
			{
				// �e�̈ʒu�ݒ�
				shadow->UpdateInstance(this->BulletPara[CntBullet].IdxShadow,
					D3DXVECTOR3(pos.x, this->BulletPara[CntBullet].FieldPosY, pos.z), 
					rot,scl);

				// �G�t�F�N�g�̐ݒ�
				effect->SetInstance(pos, VEC3_ALL0,
					PLAYER_COLOR[this->BulletPara[CntBullet].UsePlayerType], 
					EFFECT_BULLET_SIZE_X, EFFECT_BULLET_SIZE_Y, EFFECT_BULLET_TIME);
			}
			this->Transform[CntBullet].Pos(pos);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void BULLET::Draw(void)
{

}

//=============================================================================
// �e�̐ݒ�
//=============================================================================
int BULLET::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeX, float fSizeY, int nTimer, ePLAYER_TYPE type,SHADOW *s)
{
	int nIdxBullet = -1;

	for (int CntBullet = 0; CntBullet < OBJECT_BULLET_MAX; CntBullet++)
	{
		bool use = this->iUseType[CntBullet].Use();
		if (use != true)
		{
			//�����ŏ����l��ݒ� rot��scl�͎g�p���Ȃ�
   			this->Transform[CntBullet].Pos(pos);
			this->Transform[CntBullet].Rot(VEC3_ALL0);
			this->Transform[CntBullet].Scl(VEC3_ALL1);
			this->move[CntBullet].Move(move);
			this->iUseType[CntBullet].Use(YesUseType1);
			this->BulletPara[CntBullet].Timer = nTimer;
			this->BulletPara[CntBullet].UsePlayerType = type;
			this->BulletPara[CntBullet].Gravity = 0.0f;

			// �e�̐ݒ�
			this->BulletPara[CntBullet].IdxShadow = s->SetInstance(pos, VEC3_ALL1);

			//���^�[������C���f�b�N�X���X�V
			nIdxBullet = CntBullet;

			break;
		}
	}

	return nIdxBullet;
}

//=============================================================================
// �e�̍폜
//=============================================================================
void BULLET::ReleaseInstance(int nIdxBullet, SHADOW *s)
{
	//�C���f�b�N�X������Ȃ�Ή�����o���b�g�𖢎g�p�ɂ���
	if(nIdxBullet >= 0 && nIdxBullet < OBJECT_BULLET_MAX)
	{
		s->ReleaseInstance(this->BulletPara[nIdxBullet].IdxShadow);
		this->iUseType[nIdxBullet].Use(NoUse);
	}
}
