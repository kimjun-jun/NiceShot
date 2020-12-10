/**
* @file title.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/net/sock.h"
#include "../../h/scene/title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TITLE_LOGO				"../data/TEXTURE/other/Title.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_BG				"../data/TEXTURE/other/TItle_Background.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_PRESS				"../data/TEXTURE/UI/UI_Press.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_TUTORIAL	"../data/TEXTURE/UI/UI_Tutorial.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_GAME_LOCAL	"../data/TEXTURE/UI/UI_LocalBattle.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_GAME_NET	"../data/TEXTURE/UI/UI_NetorkBattle.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_GAME_QUIT	"../data/TEXTURE/UI/UI_Quit.png"	// �ǂݍ��ރe�N�X�`���t�@�C����


#define	TITLE_BG_WIDTH						(SCREEN_W/2)				// �^�C�g��BG�̕�
#define	TITLE_BG_HEIGHT						(SCREEN_H/2)				// �^�C�g��BG�̍���
#define	TITLE_BG_POS_X						(SCREEN_CENTER_X)			// �^�C�g��BG�̈ʒu(X���W)
#define	TITLE_BG_POS_Y						(SCREEN_CENTER_Y)			// �^�C�g��BG�̈ʒu(Y���W)

#define	TITLE_LOGO_POS_X					(SCREEN_CENTER_X)			// �^�C�g�����S�̈ʒu(X���W)
#define	TITLE_LOGO_POS_Y					(SCREEN_CENTER_Y-200.0f)	// �^�C�g�����S�̈ʒu(Y���W)
#define	TITLE_LOGO_WIDTH					(400.0f)					// �^�C�g�����S�̕�
#define	TITLE_LOGO_HEIGHT					(250.0f)					// �^�C�g�����S�̍���

#define	TITLE_PRESS_LOGO_POS_X				(SCREEN_CENTER_X)			// PRESS���S�̈ʒu(X���W)
#define	TITLE_PRESS_LOGO_POS_Y				(SCREEN_CENTER_Y+200.0f)	// PRESS���S�̈ʒu(Y���W)
#define	TITLE_PRESS_LOGO_WIDTH				(200.0f)					// PRESS���S�̕�
#define	TITLE_PRESS_LOGO_HEIGHT				(50.0f)					// PRESS���S�̍���

#define	TITLE_SELECT_POS_X					(SCREEN_CENTER_X)			// �Z���N�g�e�N�X�`���̈ʒu(X���W)
#define	TITLE_SELECT_POS_Y					(SCREEN_CENTER_Y)			// �Z���N�g�e�N�X�`���̈ʒu(Y���W)
#define	TITLE_SELECT_WIDTH					(150.0f)					// �Z���N�g�e�N�X�`���̕�
#define	TITLE_SELECT_HEIGHT					(100.0f)					// �Z���N�g�e�N�X�`���̍���
#define	TITLE_SELECT_OFFSET_X				(200.0f)					// �I�t�Z�b�g�l
#define	TITLE_SELECT_OFFSET_Y				(200.0f)					// �I�t�Z�b�g�l

#define	COUNT_APPERA_START					(10)						// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_START					(30)						// �X�^�[�g�{�^���_�ł̎���
#define	COUNT_WAIT_DEMO						(60 * 5)					// �f���܂ł̑҂�����


//=============================================================================
// ����������
//=============================================================================
void TITLE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexTitle();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_LOGO, &this[TITLE_OBJECT_NAME_LOGO].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_BG, &this[TITLE_OBJECT_NAME_BG].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_PRESS, &this[TITLE_OBJECT_NAME_PRESS].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_TUTORIAL, &this[TITLE_OBJECT_NAME_TUTO].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME_LOCAL, &this[TITLE_OBJECT_NAME_LOCAL].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME_NET, &this[TITLE_OBJECT_NAME_NET].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME_QUIT, &this[TITLE_OBJECT_NAME_QUIT].tex2DVB.pD3DTexture);

	this[TITLE_OBJECT_NAME_BG].bDisp = true;
	this[TITLE_OBJECT_NAME_LOGO].bDisp = true;
	this[TITLE_OBJECT_NAME_PRESS].bDisp = true;
	this[TITLE_OBJECT_NAME_TUTO].bDisp = false;
	this[TITLE_OBJECT_NAME_LOCAL].bDisp = false;
	this[TITLE_OBJECT_NAME_NET].bDisp = false;
	this[TITLE_OBJECT_NAME_QUIT].bDisp = false;

	this[0].SetColorTitle(TITLE_OBJECT_NAME_BG, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_LOGO, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_PRESS, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.0f);
}

//=============================================================================
// �ď���������
//=============================================================================
void TITLE::Reinit(void)
{
	this[0].HierarchyNum = TITLE_HIERARCHY_NUM_ANIM;
	this[0].SceneNumX = 0;
	this[0].SceneNumY = 0;
	this[0].fAlpha = 1.0f;

	this[TITLE_OBJECT_NAME_BG].bDisp = true;
	this[TITLE_OBJECT_NAME_LOGO].bDisp = true;
	this[TITLE_OBJECT_NAME_PRESS].bDisp = true;
	this[TITLE_OBJECT_NAME_TUTO].bDisp = false;
	this[TITLE_OBJECT_NAME_LOCAL].bDisp = false;
	this[TITLE_OBJECT_NAME_NET].bDisp = false;
	this[TITLE_OBJECT_NAME_QUIT].bDisp = false;

	this[0].SetColorTitle(TITLE_OBJECT_NAME_BG, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_LOGO, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_PRESS, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.0f);
}

//=============================================================================
// �I������
//=============================================================================
void TITLE::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void TITLE::Update(GAME_OBJECT* obj, FADE *fade)
{
	switch (this[0].HierarchyNum)
	{
		//�^�C�g���A�j���K�w
	case TITLE_HIERARCHY_NUM_ANIM:
	{
		//�V�[���Z���N�g�K�w�֐؂�ւ�
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			this[0].HierarchyNum = TITLE_HIERARCHY_NUM_SELECT;
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOGO, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_PRESS, 0.0f);
			this[TITLE_OBJECT_NAME_TUTO].bDisp = true;
			this[TITLE_OBJECT_NAME_LOCAL].bDisp = true;
			this[TITLE_OBJECT_NAME_NET].bDisp = true;
			this[TITLE_OBJECT_NAME_QUIT].bDisp = true;
		}
		break;
	}
	//�V�[���Z���N�g�K�w
	case TITLE_HIERARCHY_NUM_SELECT:
	{
		//�`���[�g���A���Ό��K�w�֐؂�ւ�
		if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			this[0].HierarchyNum = TITLE_HIERARCHY_NUM_ANIM;
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOGO, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_PRESS, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.0f);
			this[TITLE_OBJECT_NAME_TUTO].bDisp = false;
			this[TITLE_OBJECT_NAME_LOCAL].bDisp = false;
			this[TITLE_OBJECT_NAME_NET].bDisp = false;
			this[TITLE_OBJECT_NAME_QUIT].bDisp = false;

		}
		//�J�[�\���ړ�
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_ANALOG_L_UP) || IsButtonTriggered(0, BUTTON_DIGITAL_UP))
		{
			PlaySound(SOUND_LABEL_SE_enter03);
			this[0].SceneNumY++;
			if (this[0].SceneNumY >= 2)this[0].SceneNumY = 0;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_ANALOG_L_DOWN) || IsButtonTriggered(0, BUTTON_DIGITAL_DOWN))
		{
			PlaySound(SOUND_LABEL_SE_enter03);
			this[0].SceneNumY--;
			if (this[0].SceneNumY <= -1)this[0].SceneNumY = 1;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_ANALOG_L_RIGHT) || IsButtonTriggered(0, BUTTON_DIGITAL_RIGHT))
		{
			PlaySound(SOUND_LABEL_SE_enter03);
			this[0].SceneNumX++;
			if (this[0].SceneNumX >= 2)this[0].SceneNumX = 0;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_ANALOG_L_LEFT) || IsButtonTriggered(0, BUTTON_DIGITAL_LEFT))
		{
			PlaySound(SOUND_LABEL_SE_enter03);
			this[0].SceneNumX--;
			if (this[0].SceneNumX <= -1)this[0].SceneNumX = 1;
		}

		//�J�[�\���ɍ��킹�Ă��������点��
		//�`���[�g���A��
		if (this[0].SceneNumX == 0 && this[0].SceneNumY == 0)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.4f);
			//�V�[�����`���[�g���A����
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				fade->SetFade(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
			}
		}
		//���[�J���ΐ�
		else if (this[0].SceneNumX == 1 && this[0].SceneNumY == 0)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.4f);
			//�V�[�����Q�[����
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				fade->SetFade(FADE_OUT, SCENE_GAMECOUNTDOWN, SOUND_LABEL_BGM_normal01);
			}
		}
		//�l�b�g�ΐ�
		else if (this[0].SceneNumX == 0 && this[0].SceneNumY == 1)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.4f);
			//�V�[�����Q�[����
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				fade->SetFade(FADE_OUT, SCENE_NETMATCH, SOUND_LABEL_BGM_select01);
			}
		}
		//�A�v���I����
		else if (this[0].SceneNumX == 1 && this[0].SceneNumY == 1)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 1.0f);
			//�V�[�����Q�[����
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				SetMsg(WM_QUIT);
			}
		}

	}
	default:
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void TITLE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_OBJECT_NAME_MAX; CntTitleSelectScene++)
	{
		if (this[CntTitleSelectScene].bDisp == true)
		{

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, this[CntTitleSelectScene].tex2DVB.pD3DTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT TITLE::MakeVertexTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//BG����
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&this[0].tex2DVB.pD3DVtxBuff,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[0].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_H, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			this[0].tex2DVB.pD3DVtxBuff->Unlock();
		}
	}

	//�^�C�g�����S����
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&this[1].tex2DVB.pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[1].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X - TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y - TITLE_LOGO_HEIGHT, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y - TITLE_LOGO_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X - TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			this[1].tex2DVB.pD3DVtxBuff->Unlock();
		}
	}

	//PRESS����
	{

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&this[2].tex2DVB.pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			this[2].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X - TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y - TITLE_PRESS_LOGO_HEIGHT, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X + TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y - TITLE_PRESS_LOGO_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X - TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y + TITLE_PRESS_LOGO_HEIGHT, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X + TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y + TITLE_PRESS_LOGO_HEIGHT, 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			this[2].tex2DVB.pD3DVtxBuff->Unlock();
		}
	}

	//�Z���N�gUI����
	{
		for (int CntTitleSelectScene = TITLE_OBJECT_NAME_TUTO; CntTitleSelectScene < TITLE_OBJECT_NAME_MAX; CntTitleSelectScene++)
		{

			// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
			if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
				D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
				FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
				D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
				&this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
				NULL)))						// NULL�ɐݒ�
			{
				return E_FAIL;
			}

			{//���_�o�b�t�@�̒��g�𖄂߂�
				VERTEX_2D *pVtx;

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
				this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W�̐ݒ�
				switch (CntTitleSelectScene)
				{
				case TITLE_OBJECT_NAME_TUTO:
					pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					break;
				case TITLE_OBJECT_NAME_LOCAL:
					pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT - TITLE_SELECT_OFFSET_Y, 0.0f);
					break;
				case TITLE_OBJECT_NAME_NET:
					pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					break;
				case TITLE_OBJECT_NAME_QUIT:
					pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + TITLE_SELECT_OFFSET_Y, 0.0f);
					break;
				}
				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				// ���_�f�[�^���A�����b�N����
				this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff->Unlock();
			}
		}
	}

	return S_OK;
}

//=============================================================================
// ���_�F�ύX
//=============================================================================
void TITLE::SetColorTitle(int CntTitle, float alpha)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntTitle].tex2DVB.pD3DVtxBuff;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

		// ���_�f�[�^���A�����b�N����
		pD3DVtxBuff->Unlock();
	}

}

