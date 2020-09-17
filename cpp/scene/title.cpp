/**
* @file title.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/scene/title.h"
#include "../../h/other/input.h"
#include "../../h/scene/fade.h"
#include "../../h/other/sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TITLE_BG				"../data/TEXTURE/title.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LOGO_START				"../data/TEXTURE/press_enter.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_TUTORIAL	"../data/TEXTURE/rensyurogo.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_GAME		"../data/TEXTURE/taisenrogo.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����


#define	TITLE_BG_WIDTH		(SCREEN_W/2)						// �^�C�g��BG�̕�
#define	TITLE_BG_HEIGHT		(SCREEN_H/2)						// �^�C�g��BG�̍���
#define	TITLE_BG_POS_X		(SCREEN_CENTER_X)					// �^�C�g��BG�̈ʒu(X���W)
#define	TITLE_BG_POS_Y		(SCREEN_CENTER_Y)					// �^�C�g��BG�̈ʒu(Y���W)

#define	TITLE_START_LOGO_POS_X				(TITLE_BG_POS_X)			// �X�^�[�g���S�̈ʒu(X���W)
#define	TITLE_START_LOGO_POS_Y				(TITLE_BG_POS_Y+200)		// �X�^�[�g���S�̈ʒu(Y���W)
#define	TITLE_START_LOGO_WIDTH				(100)						// �X�^�[�g���S�̕�
#define	TITLE_START_LOGO_HEIGHT				(100)						// �X�^�[�g���S�̍���

#define	TITLE_SELECT_POS_X				(TITLE_BG_POS_X+200.0f)		// �Z���N�g�e�N�X�`���̈ʒu(X���W)
#define	TITLE_SELECT_POS_Y				(TITLE_BG_POS_Y-100.0f)		// �Z���N�g�e�N�X�`���̈ʒu(Y���W)
#define	TITLE_SELECT_WIDTH				(150)						// �Z���N�g�e�N�X�`���̕�
#define	TITLE_SELECT_HEIGHT				(150)						// �Z���N�g�e�N�X�`���̍���

#define	COUNT_APPERA_START		(10)						// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_START		(30)						// �X�^�[�g�{�^���_�ł̎���
#define	COUNT_WAIT_DEMO			(60 * 5)					// �f���܂ł̑҂�����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
void SetColorTitle(TITLECLASS *SetTitle);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
TITLECLASS g_TitleBG;
TITLECLASS g_TitleStartLogo;
TITLECLASS g_TitleSelectScene[TITLE_SELECT_SCENE_MAX];

int			TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;	//!<
bool		TitleSelectTime = false;								//!< true=�^�C�g���V�[���ŃZ���N�g��ʂɂȂ��Ă���@false=�^�C�g���V�[���œ_�ŉ�ʂɂȂ��Ă���
bool		TitleSelectTimeCHK = false;								//!< true=���̃V�[���֍s���ėǂ��@false=�^�C�g���V�[���œ_�ŉ�ʂɂȂ��Ă���@�����gamescne�ŃV�[���`�F�b�N�̎��ɔ��肵�Ă���
//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
	TitleSelectTime = false;
	TitleSelectTimeCHK = false;

	g_TitleStartLogo.nCountAppearStart = 0;
	g_TitleStartLogo.fAlpha = 0.0f;
	g_TitleStartLogo.nCountDisp = 0;
	g_TitleStartLogo.bDisp = false;
	g_TitleStartLogo.nConutDemo = 0;

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		g_TitleSelectScene[CntTitleSelectScene].bDisp = false;
		g_TitleSelectScene[CntTitleSelectScene].fAlpha = 0.0f;
	}
	// ���_���̍쐬
	MakeVertexTitle(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_TITLE_BG,			// �t�@�C���̖��O
		&g_TitleBG.pD3DTexture);	// �ǂݍ��ރ������[


	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_LOGO_START,			// �t�@�C���̖��O
		&g_TitleStartLogo.pD3DTexture);		// �ǂݍ��ރ������[


	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_TITLE_SELECT_TUTORIAL,			// �t�@�C���̖��O
		&g_TitleSelectScene[TITLE_SELECT_SCENE_TUTORIAL].pD3DTexture);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_TITLE_SELECT_GAME,			// �t�@�C���̖��O
		&g_TitleSelectScene[TITLE_SELECT_SCENE_GAME].pD3DTexture);		// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �ď���������
//=============================================================================
HRESULT ReInitTitle(void)
{
	TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
	TitleSelectTime = false;
	TitleSelectTimeCHK = false;
	g_TitleBG.fAlpha = 1.0f;

	g_TitleStartLogo.nCountAppearStart = 0;
	g_TitleStartLogo.fAlpha = 0.0f;
	g_TitleStartLogo.nCountDisp = 0;
	g_TitleStartLogo.bDisp = false;
	g_TitleStartLogo.nConutDemo = 0;

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		g_TitleSelectScene[CntTitleSelectScene].bDisp = false;
		g_TitleSelectScene[CntTitleSelectScene].fAlpha = 0.0f;
	}
	SetColorTitle(&g_TitleBG);
	SetColorTitle(&g_TitleStartLogo);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	if (g_TitleBG.pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		g_TitleBG.pD3DTexture->Release();
		g_TitleBG.pD3DTexture = NULL;
	}

	if (g_TitleBG.pD3DVtxBuff != NULL)
	{// ���_�o�b�t�@�̊J��
		g_TitleBG.pD3DVtxBuff->Release();
		g_TitleBG.pD3DVtxBuff = NULL;
	}

	if (g_TitleStartLogo.pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		g_TitleStartLogo.pD3DTexture->Release();
		g_TitleStartLogo.pD3DTexture = NULL;
	}

	if (g_TitleStartLogo.pD3DVtxBuff != NULL)
	{// ���_�o�b�t�@�̊J��
		g_TitleStartLogo.pD3DVtxBuff->Release();
		g_TitleStartLogo.pD3DVtxBuff = NULL;
	}

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (g_TitleSelectScene[CntTitleSelectScene].pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			g_TitleSelectScene[CntTitleSelectScene].pD3DTexture->Release();
			g_TitleSelectScene[CntTitleSelectScene].pD3DTexture = NULL;
		}

		if (g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff->Release();
			g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	//�^�C�g�����(�Z���N�g��)
	if (TitleSelectTime == true)
	{
		//���菈��
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			if(TitleSelectSceneNum == TITLE_SELECT_SCENE_TUTORIAL) SetFade(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
			else if (TitleSelectSceneNum == TITLE_SELECT_SCENE_GAME) SetFade(FADE_OUT, SCENE_GAMECOUNTDOWN, SOUND_LABEL_BGM_normal01);
		}
		//�Z���N�g�؂�ւ�
		else if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			g_TitleBG.fAlpha = 1.0f;
			SetColorTitle(&g_TitleBG);
			g_TitleStartLogo.fAlpha = 0.0f;
			SetColorTitle(&g_TitleStartLogo);
			TitleSelectTime = false;
			g_TitleSelectScene[0].bDisp = false;
			g_TitleSelectScene[1].bDisp = false;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DIGITAL_DOWN))
		{
			////�I����������Ƃ�����
			//{
			//	TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
			//}
			//3�ȏ�I����������Ƃ������g��
			{
				if (TitleSelectSceneNum != TITLE_SELECT_SCENE_GAME) PlaySound(SOUND_LABEL_SE_enter02);
				TitleSelectSceneNum++;
				if (TitleSelectSceneNum > TITLE_SELECT_SCENE_GAME) TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
			}
		}
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_DIGITAL_UP))
		{
			////�I����������Ƃ�����
			//{
			//	TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			//}
			//3�ȏ�I����������Ƃ������g��
			{
				if (TitleSelectSceneNum != TITLE_SELECT_SCENE_TUTORIAL) PlaySound(SOUND_LABEL_SE_enter02);
				TitleSelectSceneNum--;
				if (TitleSelectSceneNum <= TITLE_SELECT_SCENE_TUTORIAL) TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
		}
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			if (TitleSelectSceneNum == CntTitleSelectScene)
			{
				g_TitleSelectScene[CntTitleSelectScene].fAlpha = 1.0f;
			}
			else
			{
				g_TitleSelectScene[CntTitleSelectScene].fAlpha = 0.2f;
			}
			SetColorTitle(&g_TitleSelectScene[CntTitleSelectScene]);
		}
	}

	//�^�C�g�����(���S�_��)
	else if (TitleSelectTime == false)
	{
		//�Z���N�g�؂�ւ�
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			g_TitleBG.fAlpha = 0.4f;
			SetColorTitle(&g_TitleBG);
			g_TitleStartLogo.fAlpha = 0.0f;
			SetColorTitle(&g_TitleStartLogo);
			TitleSelectTime = true;
			g_TitleSelectScene[0].bDisp = true;
			g_TitleSelectScene[1].bDisp = true;
		}
		//���߂͓����e�N�X�`���̃A���t�@�l���グ�Ă���������悤�ɂ���
		if (g_TitleStartLogo.fAlpha < 1.0f)
		{
			g_TitleStartLogo.fAlpha += 0.05f;
			if (g_TitleStartLogo.fAlpha >= 1.0f)
			{
				g_TitleStartLogo.fAlpha = 1.0f;
			}
			SetColorTitle(&g_TitleStartLogo);
		}
		//�A���t�@�l���ő�܂ŗ�����J�E���g���Ƃ�A�_�ł��J��Ԃ�
		else
		{
			g_TitleStartLogo.nCountAppearStart++;
			if (g_TitleStartLogo.nCountAppearStart > COUNT_APPERA_START)
			{
				g_TitleStartLogo.nCountDisp = (g_TitleStartLogo.nCountDisp + 1) % 80;
				if (g_TitleStartLogo.nCountDisp > INTERVAL_DISP_START)
				{
					g_TitleStartLogo.bDisp = false;
				}
				else
				{
					g_TitleStartLogo.bDisp = true;
				}
			}
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_TitleBG.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_TitleBG.pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);

	if (g_TitleStartLogo.bDisp == true)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_TitleStartLogo.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_TitleStartLogo.pD3DTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}


	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (g_TitleSelectScene[CntTitleSelectScene].bDisp == true)
		{

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_TitleSelectScene[CntTitleSelectScene].pD3DTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	//�^�C�g��BG����
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&g_TitleBG.pD3DVtxBuff,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_TitleBG.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
			g_TitleBG.pD3DVtxBuff->Unlock();
		}
	}

	//�^�C�g���X�^�[�g���S����
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&g_TitleStartLogo.pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_TitleStartLogo.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X - TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y - TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X + TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y - TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X - TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y + TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X + TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y + TITLE_START_LOGO_HEIGHT, 0.0f);

			// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleStartLogo.fAlpha);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			g_TitleStartLogo.pD3DVtxBuff->Unlock();
		}
	}

	//�^�C�g���Z���N�g���S����
	{
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
			if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
				D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
				FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
				D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
				&g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
				NULL)))						// NULL�ɐݒ�
			{
				return E_FAIL;
			}

			{//���_�o�b�t�@�̒��g�𖄂߂�
				VERTEX_2D *pVtx;

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
				g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W�̐ݒ�
				pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (CntTitleSelectScene*TITLE_SELECT_HEIGHT * 2), 0.0f);

				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TitleSelectScene[CntTitleSelectScene].fAlpha);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				// ���_�f�[�^���A�����b�N����
				g_TitleSelectScene[CntTitleSelectScene].pD3DVtxBuff->Unlock();
			}
		}
	}


	return S_OK;
}

//=============================================================================
// ���_�F�ύX
//=============================================================================
void SetColorTitle(TITLECLASS *SetTitle)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		SetTitle->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, SetTitle->fAlpha);

		// ���_�f�[�^���A�����b�N����
		SetTitle->pD3DVtxBuff->Unlock();
	}

}


void SetTitleSelectTimeCHK(bool b)
{
	TitleSelectTimeCHK = b;
}

bool GetTitleSelectTimeCHK(void)
{
	return TitleSelectTimeCHK;
}

int GetSelectTitleScene(void)
{
	return TitleSelectSceneNum;
}
