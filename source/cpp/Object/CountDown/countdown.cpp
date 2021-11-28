/**
* @file countdown.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../../h/main.h"
#include "../../../h/Object/Scene/Scene.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Net/sock.h"
#include "../../../h/Object/CountDown/countdown.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COUNTDOWN_TEXTURE_NUMBER			_T("../data/TEXTURE/BG/0-9.png")	//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define COUNTDOWN_TEXTURE_LOGO				_T("../data/TEXTURE/sozai/go.png")	//!< �ǂݍ��ރe�N�X�`���t�@�C����

constexpr int COUNTDOWN_TEXTURE_NUMBER_SIZE_X{ 50 };			//!< �e�N�X�`���T�C�Y 
constexpr int COUNTDOWN_TEXTURE_NUMBER_SIZE_Y{ 50 };			//!< ����  
constexpr int COUNTDOWN_TEXTURE_LOGO_SIZE_X{ 200 };				//!< �e�N�X�`�����S�T�C�Y 
constexpr int COUNTDOWN_TEXTURE_LOGO_SIZE_Y{ 200 };				//!< ����  
constexpr int COUNTDOWN_TEXTURE_PATTERN_X{ 10 };				//!< �A�j���p�^�[���̃e�N�X�`�����������iX)  
constexpr int COUNTDOWN_TEXTURE_PATTERN_Y{ 1 };					//!< �A�j���p�^�[���̃e�N�X�`�����������iY)  
constexpr int COUNTDOWN_ANIM_PATTERN_NUM{ COUNTDOWN_TEXTURE_PATTERN_X*COUNTDOWN_TEXTURE_PATTERN_Y };	//!< �A�j���[�V�����p�^�[����  
constexpr int COUNTDOWN_TIME_ANIMATION{ 4 };					//!< �A�j���[�V�����̐؂�ւ��J�E���g  
constexpr int COUNTDOWN_TEXTURE_POS_X{ SCREEN_CENTER_X };		//!< �e�N�X�`�����W
constexpr int COUNTDOWN_TEXTURE_POS_Y{ SCREEN_CENTER_Y };		//!< ����  
constexpr int COUNTDOWN_FPS_TIME{ 240 };						//!< ���t���[���J�E���g���ă^�C���v��
constexpr int COUNTDOWN_FPS_TIME_START{ 4 };					//!< �J�E���g�_�E���X�^�[�g���_�̃^�C��
constexpr int COUNTDOWN_CHANGESIZE{ 5 };						//!< �J�E���g�_�E�������̃T�C�Y���g�債�Ă����l
constexpr float COUNTDOWN_ALPHA{ 0.05f };						//!< �J�E���g�_�E����0�̎�GO��`�悵��GO�������Ȃ�X�s�[�h

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
COUNTDOWN::COUNTDOWN(void)
{
	//���_�̍쐬
	this->vtx.MakeVertex2D(OBJECT_COUNTDOWN_MAX, FVF_VERTEX_2D);

	//�J�E���g���[�v
	for (int CntCountDown = 0; CntCountDown < OBJECT_COUNTDOWN_MAX; CntCountDown++)
	{
		//�`��ʒu�ݒ�
		this->Transform[CntCountDown].Pos(D3DXVECTOR3(COUNTDOWN_TEXTURE_POS_X, COUNTDOWN_TEXTURE_POS_Y, 0.0f));
		D3DXVECTOR3 pos = this->Transform[CntCountDown].Pos();
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
		D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
		D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
		D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
		};

		//�`��ʒu���f
		this->vtx.Vertex2D(CntCountDown,vtx);

		//RHW�ݒ�
		this->vtx.RHW2D(CntCountDown);

		//UV�̐ݒ�
		this->vtx.UV2D(CntCountDown);

		//�J���[�ݒ�
		this->vtx.Color2D(CntCountDown, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	}

	//�g�p�ݒ�
	this->iUseType[COUNTDOWN_TEX_NUMBER].Use(YesUseType1);
	this->iUseType[COUNTDOWN_TEX_LOGO].Use(NoUse);

	// �e�N�X�`���̓ǂݍ���
	this->tex[COUNTDOWN_TEX_NUMBER].LoadTexture(COUNTDOWN_TEXTURE_NUMBER);
	this->tex[COUNTDOWN_TEX_LOGO].LoadTexture(COUNTDOWN_TEXTURE_LOGO);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
COUNTDOWN::~COUNTDOWN(void)
{
	//�e�N�X�`�����
	this->tex[COUNTDOWN_TEX_NUMBER].~TEXTURE();
	this->tex[COUNTDOWN_TEX_LOGO].~TEXTURE();
	//���_���
	this->vtx.~VTXBUFFER();
}

//=============================================================================
// ���N���X�̃A�h���X�擾
//=============================================================================
void COUNTDOWN::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pmysocket = obj->GetMySocket();
	pscene = obj->GetScene();
}

//=============================================================================
// ����������
//=============================================================================
void COUNTDOWN::Init(void)
{
	//�����ݒ�@�S�ċK��l
	this->iUseType[COUNTDOWN_TEX_NUMBER].Use(YesUseType1);
	this->iUseType[COUNTDOWN_TEX_LOGO].Use(NoUse);
	this->CountdownPara.color = 1.0f;
	this->CountdownPara.time_maneger = COUNTDOWN_FPS_TIME;
	this->CountdownPara.fps_maneger = COUNTDOWN_FPS_TIME_START;
	this->CountdownPara.CountAnim = 0;
	this->CountdownPara.PatternAnim = 0;
	this->CountdownPara.changeval = 0;
	this->CountdownPara.CountEnd = false;

}

//=============================================================================
// �X�V����
//=============================================================================
void COUNTDOWN::Update(void)
{
	NumberCountUpdate();
	NumberSizeUpdate();
	//StopSound();
	if (this->CountdownPara.CountEnd == true)
	{
		MasterVolumeChange(1);
		//���[�J���Q�[���J�n
		if (pmysocket->GetNetGameStartFlag() == false)
		{
			pscene->SetScene(SCENE_GAME);
			PlaySound(SOUND_LABEL_BGM_normal01);
			SourceVolumeChange(0.5f, SOUND_LABEL_BGM_normal01);
		}
		//�l�b�g�Q�[���J�n
		else
		{
			pscene->SetScene(SCENE_NETGAME);
			PlaySound(SOUND_LABEL_BGM_boss01);
			SourceVolumeChange(0.8f, SOUND_LABEL_BGM_boss01);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void COUNTDOWN::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int CntCountDown = 0; CntCountDown < OBJECT_COUNTDOWN_MAX; CntCountDown++)
	{
		bool use = this->iUseType[CntCountDown].Use();
		if (use == true )
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[CntCountDown].Texture());
			// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntCountDown * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void COUNTDOWN::NumberCountUpdate(void)
{
	//���݃^�C�����擾
	int val = this->CountdownPara.time_maneger;
	
	//�T�C�Y�����X�ɑ傫��
	this->CountdownPara.changeval += COUNTDOWN_CHANGESIZE;

	//�J�E���g�_�E�����I��������t���O�Z�b�g
	if (val <= 0)
	{
		this->iUseType[COUNTDOWN_TEX_NUMBER].Use(NoUse);
		this->iUseType[COUNTDOWN_TEX_LOGO].Use(YesUseType1);
	}
	//else this->iUseType[COUNTDOWN_TEX_LOGO].Use(NoUse);



	//240/3=80 ->80�t���[����1�J�E���g�_�E�� ���傤�ǂ������Ԃ�ݒ�
	if (val % (COUNTDOWN_FPS_TIME / (COUNTDOWN_FPS_TIME_START - 1)) == 0)
	{
		//���l���X�V���ăT�C�Y��0�ɂ���
		this->CountdownPara.fps_maneger -= 1;
		float uStart, uEnd, vStart, vEnd;
		uStart = 0.1f*this->CountdownPara.fps_maneger;
		uEnd = 0.1f*this->CountdownPara.fps_maneger + 0.1f;
		vStart = 0.0f;
		vEnd = 1.0f;
		this->vtx.UV2D(COUNTDOWN_TEX_NUMBER, uStart, uEnd, vStart, vEnd);
		this->CountdownPara.changeval = 0;
	}

	//�J�E���g�_�E�����I�������X�^�[�g���S��\���@�\���ザ�傶��Ƀ��������āA0�ɂȂ�ƃQ�[���J�n
	bool use = this->iUseType[COUNTDOWN_TEX_LOGO].Use();
	if (use == true)
	{
		this->CountdownPara.color -= COUNTDOWN_ALPHA;
		this->vtx.Color2D(COUNTDOWN_TEX_LOGO, D3DXCOLOR(1.0f, 1.0f, 1.0f, this->CountdownPara.color));
		if (this->CountdownPara.color <= 0) this->CountdownPara.CountEnd = true;
	}


}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void COUNTDOWN::NumberSizeUpdate(void)
{
	//�`��ʒu�ݒ�
	this->Transform[COUNTDOWN_TEX_NUMBER].Pos(D3DXVECTOR3(COUNTDOWN_TEXTURE_POS_X, COUNTDOWN_TEXTURE_POS_Y, 0.0f));
	D3DXVECTOR3 pos = this->Transform[COUNTDOWN_TEX_NUMBER].Pos();
	D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
	{
	D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
	D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
	D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
	D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
	};

	//�`��ʒu���f
	this->vtx.Vertex2D(COUNTDOWN_TEX_NUMBER, vtx);
}

//=============================================================================
// �J�E���g�_�E�����Z����
//=============================================================================
void COUNTDOWN::AddCountdown(int val)
{
	this->CountdownPara.time_maneger += val;
}
