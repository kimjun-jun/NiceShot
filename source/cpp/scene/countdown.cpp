/**
* @file countdown.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/scene/countdown.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_COUNTDOWNGO			_T("../data/TEXTURE/sozai/go.png")													//!< �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_COUNTDOWN_SIZE_X			(50)																			//!< �e�N�X�`���T�C�Y 
#define TEXTURE_COUNTDOWN_SIZE_Y			(50)																			//!< ����  
#define TEXTURE_COUNTDOWNGO_SIZE_X			(200)																			//!< �e�N�X�`�����S�T�C�Y 
#define TEXTURE_COUNTDOWNGO_SIZE_Y			(200)																			//!< ����  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X	(10)																			//!< �A�j���p�^�[���̃e�N�X�`�����������iX)  
#define COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y	(1)																				//!< �A�j���p�^�[���̃e�N�X�`�����������iY)  
#define COUNTDOWN_ANIM_PATTERN_NUM			(COUNTDOWN_TEXTURE_PATTERN_DIVIDE_X*COUNTDOWN_TEXTURE_PATTERN_DIVIDE_Y)			//!< �A�j���[�V�����p�^�[����  
#define COUNTDOWN_TIME_ANIMATION			(4)																				//!< �A�j���[�V�����̐؂�ւ��J�E���g  
#define TEXTURE_COUNTDOWN_POS_X				(SCREEN_CENTER_X)																//!< �e�N�X�`�����W
#define TEXTURE_COUNTDOWN_POS_Y				(SCREEN_CENTER_Y)																//!< ����  
#define FPS_TIME_COUNTDOWN					(240)																			//!< ���t���[���J�E���g���ă^�C���v��
#define COUNTDOWN_A							(10)																			//!< �J�E���g�_�E����0�̎�GO��`�悵��GO�������Ȃ�X�s�[�h
#define COUNTDOWN_CHANGESIZE				(5)																				//!< �J�E���g�_�E�������̃T�C�Y���g�債�Ă����l

//=============================================================================
// ����������
//=============================================================================
void COUNTDOWN::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPDIRECT3DTEXTURE9 pD3DTexture[OBJECT_COUNTDOWN_MAX];
	// �e�N�X�`���̓ǂݍ���  
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME_SCORE, &pD3DTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME_COUNTDOWNGO, &pD3DTexture[1]);
	this[0].tex2D.SetpD3DTexture(pD3DTexture[0]);
	this[1].tex2D.SetpD3DTexture(pD3DTexture[1]);

	pD3DTexture[0]->Release();
	pD3DTexture[1]->Release();
	for (int i = 0; i < OBJECT_COUNTDOWN_MAX; i++)
	{
		this[i].SetPos(D3DXVECTOR3(TEXTURE_COUNTDOWN_POS_X, TEXTURE_COUNTDOWN_POS_Y, 0.0f));
	}
	this[0].SetUse(true);
	this[1].color = 255;
	this[0].time_maneger = FPS_TIME_COUNTDOWN;
	this[0].fps_maneger = 4;	//�J�E���g��4����X�^�[�g(�\����3)
	MakeVertexCountdown();

}

//=============================================================================
// �ď���������
//=============================================================================
void COUNTDOWN::Reinit(void)
{
	for (int i = 0; i < OBJECT_COUNTDOWN_MAX; i++)
	{
		this[i].nCountAnim = 0;
		this[i].nPatternAnim = 0;
		this[i].changeval = 0;
	}
	this[0].SetUse(true);
	this[1].SetUse(false);
	this[0].signal = false;
	this[1].color = 255;
	this[0].time_maneger = FPS_TIME_COUNTDOWN;
	this[0].fps_maneger = 4;
}

//=============================================================================
// �I������
//=============================================================================
void COUNTDOWN::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void COUNTDOWN::Update(GAME_OBJECT*obj)
{
	SetTextureCountdown();
	SetVertexCountdown();
	StopSound();
	MasterVolumeChange(1);
	if (this[0].signal == true)
	{
		obj->SetScene(SCENE_GAME);
		PlaySound(SOUND_LABEL_BGM_normal01);
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
	for (int i = 0; i < OBJECT_COUNTDOWN_MAX; i++)
	{
		bool use = this[i].GetUse();
		if (use == true )
		{
			// �e�N�X�`���̐ݒ�  
			pDevice->SetTexture(0, LPDIRECT3DTEXTURE9(this->tex2D.GetpD3DTexture()));
			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this->tex2D.GettextureVTX(), sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT COUNTDOWN::MakeVertexCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���_���W�̐ݒ�
	SetVertexCountdown();

	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		// rhw�̐ݒ�
		vtx2d[0].rhw =
			vtx2d[1].rhw =
			vtx2d[2].rhw =
			vtx2d[3].rhw = 1.0f;
		// ���ːݒ�  [i]
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		// �e�N�����W[i]�̐ݒ�  
		vtx2d[0].tex = D3DXVECTOR2(0.3f, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(0.4f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.3f, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(0.4f, 1.0f);
		this[0].tex2D.SettextureVTX(vtx2d);
	}
	{
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		// rhw�̐ݒ�
		vtx2d[0].rhw =
			vtx2d[1].rhw =
			vtx2d[2].rhw =
			vtx2d[3].rhw = 1.0f;
		// ���ːݒ�  [i]
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		// �e�N�����W[i]�̐ݒ�  
		vtx2d[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		this[0].tex2D.SettextureVTX(vtx2d);
	}
	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void COUNTDOWN::SetTextureCountdown(void)
{
	int val = this[0].time_maneger;
	if (val <= 0)
	{
		this[0].SetUse(false);
		this[1].SetUse(true);
	}
	else this[1].SetUse(false);
	if (val % (FPS_TIME_COUNTDOWN/3) == 0)
	{
		this[0].fps_maneger -= 1;
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		vtx2d[0].tex = D3DXVECTOR2(0.1f*this[0].fps_maneger, 0.0f);
		vtx2d[1].tex = D3DXVECTOR2(0.1f*this[0].fps_maneger+0.1f, 0.0f);
		vtx2d[2].tex = D3DXVECTOR2(0.1f*this[0].fps_maneger, 1.0f);
		vtx2d[3].tex = D3DXVECTOR2(0.1f*this[0].fps_maneger+0.1f, 1.0f);
		this[0].tex2D.SettextureVTX(vtx2d);
		this[0].changeval = 0;
	}
	bool use = this[1].GetUse();
	if (use == true)
	{
		this[1].color -= COUNTDOWN_A;
		VERTEX_2D vtx2d[POLYGON_2D_VERTEX];
		vtx2d[0].diffuse = D3DCOLOR_RGBA(255, 255, 255,this[1].color);
		vtx2d[1].diffuse = D3DCOLOR_RGBA(255, 255, 255,this[1].color);
		vtx2d[2].diffuse = D3DCOLOR_RGBA(255, 255, 255,this[1].color);
		vtx2d[3].diffuse = D3DCOLOR_RGBA(255, 255, 255,this[1].color);
		this[1].tex2D.SettextureVTX(vtx2d);
		if (this[1].color <= 0) this[0].signal = true;
	}
	this[0].changeval += COUNTDOWN_CHANGESIZE;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void COUNTDOWN::SetVertexCountdown(void)
{
	// ���_���W�̐ݒ� 
	{
		VERTEX_2D vtx2d1[POLYGON_2D_VERTEX];
		//--------------------------------------�I�u�W�F�N�g�l�ǂݍ���
		D3DXVECTOR3 pos1 = this[0].GetPos();
		vtx2d1[0].vtx.x = pos1.x - TEXTURE_COUNTDOWN_SIZE_X - this[0].changeval;
		vtx2d1[0].vtx.y = pos1.y - TEXTURE_COUNTDOWN_SIZE_Y - this[0].changeval;
		vtx2d1[0].vtx.z = 0.0f;
		vtx2d1[1].vtx.x = pos1.x + TEXTURE_COUNTDOWN_SIZE_X + this[0].changeval;
		vtx2d1[1].vtx.y = pos1.y - TEXTURE_COUNTDOWN_SIZE_Y - this[0].changeval;
		vtx2d1[1].vtx.z = 0.0f;
		vtx2d1[2].vtx.x = pos1.x - TEXTURE_COUNTDOWN_SIZE_X - this[0].changeval;
		vtx2d1[2].vtx.y = pos1.y + TEXTURE_COUNTDOWN_SIZE_Y + this[0].changeval;
		vtx2d1[2].vtx.z = 0.0f;
		vtx2d1[3].vtx.x = pos1.x + TEXTURE_COUNTDOWN_SIZE_X + this[0].changeval;
		vtx2d1[3].vtx.y = pos1.y + TEXTURE_COUNTDOWN_SIZE_Y + this[0].changeval;
		vtx2d1[3].vtx.z = 0.0f;
		//--------------------------------------�I�u�W�F�N�g�l��������
		this[0].tex2D.SettextureVTX(vtx2d1);

	}

	{
		VERTEX_2D vtx2d2[POLYGON_2D_VERTEX];
		//--------------------------------------�I�u�W�F�N�g�l�ǂݍ���
		D3DXVECTOR3 pos2 = this[0].GetPos();

		vtx2d2[0].vtx.x = pos2.x - TEXTURE_COUNTDOWNGO_SIZE_X;
		vtx2d2[0].vtx.y = pos2.y - TEXTURE_COUNTDOWNGO_SIZE_Y;
		vtx2d2[0].vtx.z = 0.0f;
		vtx2d2[1].vtx.x = pos2.x + TEXTURE_COUNTDOWNGO_SIZE_X;
		vtx2d2[1].vtx.y = pos2.y - TEXTURE_COUNTDOWNGO_SIZE_Y;
		vtx2d2[1].vtx.z = 0.0f;
		vtx2d2[2].vtx.x = pos2.x - TEXTURE_COUNTDOWNGO_SIZE_X;
		vtx2d2[2].vtx.y = pos2.y + TEXTURE_COUNTDOWNGO_SIZE_Y;
		vtx2d2[2].vtx.z = 0.0f;
		vtx2d2[3].vtx.x = pos2.x + TEXTURE_COUNTDOWNGO_SIZE_X;
		vtx2d2[3].vtx.y = pos2.y + TEXTURE_COUNTDOWNGO_SIZE_Y;
		vtx2d2[3].vtx.z = 0.0f;
		//--------------------------------------�I�u�W�F�N�g�l��������
		this[1].tex2D.SettextureVTX(vtx2d2);

	}
}

//=============================================================================
// �J�E���g�_�E�����Z����
//=============================================================================
void COUNTDOWN::AddCountdown(int val)
{
	this[0].time_maneger += val;
}
