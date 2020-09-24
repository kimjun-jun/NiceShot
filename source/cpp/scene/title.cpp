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
#include "../../h/scene/title.h"

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


//=============================================================================
// ����������
//=============================================================================
void TITLE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexTitle();

	// �e�N�X�`���̓ǂݍ���
	LPDIRECT3DTEXTURE9 pD3DTexture[OBJECT_TITLE_MAX];
	D3DXCreateTextureFromFile(pDevice,TEXTURE_TITLE_BG,&pD3DTexture[0]);	

	D3DXCreateTextureFromFile(pDevice,TEXTURE_LOGO_START,&pD3DTexture[1]);

	D3DXCreateTextureFromFile(pDevice,TEXTURE_TITLE_SELECT_TUTORIAL,&pD3DTexture[2]);

	D3DXCreateTextureFromFile(pDevice,TEXTURE_TITLE_SELECT_GAME,&pD3DTexture[3]);

	this[0].tex2DVB.SetpD3DTexture(pD3DTexture[0]);
	this[1].tex2DVB.SetpD3DTexture(pD3DTexture[1]);
	this[2].tex2DVB.SetpD3DTexture(pD3DTexture[2]);
	this[3].tex2DVB.SetpD3DTexture(pD3DTexture[3]);

	pD3DTexture[0]->Release();
	pD3DTexture[1]->Release();
	pD3DTexture[2]->Release();
	pD3DTexture[3]->Release();
}

//=============================================================================
// �ď���������
//=============================================================================
void TITLE::Reinit(void)
{
	this[0].TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
	this[0].fAlpha = 1.0f;
	this[0].TitleSelectTime = false;

	this[1].nCountAppearStart = 0;
	this[1].fAlpha = 0.0f;
	this[1].nCountDisp = 0;
	this[1].bDisp = false;
	this[1].nConutDemo = 0;

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		this[CntTitleSelectScene].bDisp = false;
		this[CntTitleSelectScene].fAlpha = 0.0f;
	}
	this[0].SetColorTitle(0, this[0].fAlpha);
	this[0].SetColorTitle(1, this[0].fAlpha);
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
void TITLE::Update(void)
{
	//�^�C�g�����(�Z���N�g��)
	if (this[0].TitleSelectTime == true)
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
			this[0].fAlpha = 1.0f;
			SetColorTitle(0, this[0].fAlpha);
			this[1].fAlpha = 0.0f;
			SetColorTitle(1, this[1].fAlpha);
			this[0].TitleSelectTime = false;
			this[2].bDisp = false;
			this[3].bDisp = false;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DIGITAL_DOWN))
		{
			////�I����������Ƃ�����
			//{
			//	TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
			//}
			//3�ȏ�I����������Ƃ������g��
			{
				if (this[0].TitleSelectSceneNum != TITLE_SELECT_SCENE_GAME) PlaySound(SOUND_LABEL_SE_enter02);
				this[0].TitleSelectSceneNum++;
				if (this[0].TitleSelectSceneNum > TITLE_SELECT_SCENE_GAME) this[0].TitleSelectSceneNum = TITLE_SELECT_SCENE_GAME;
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
				if (this[0].TitleSelectSceneNum != TITLE_SELECT_SCENE_TUTORIAL) PlaySound(SOUND_LABEL_SE_enter02);
				this[0].TitleSelectSceneNum--;
				if (this[0].TitleSelectSceneNum <= TITLE_SELECT_SCENE_TUTORIAL) this[0].TitleSelectSceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
		}
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			if (this[0].TitleSelectSceneNum == CntTitleSelectScene)
			{
				this[2].fAlpha = 1.0f;
			}
			else
			{
				this[3].fAlpha = 0.2f;
			}
			SetColorTitle(2, this[2].fAlpha);
			SetColorTitle(3, this[3].fAlpha);
		}
	}

	//�^�C�g�����(���S�_��)
	else if (this[0].TitleSelectTime == false)
	{
		//�Z���N�g�؂�ւ�
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			PlaySound(SOUND_LABEL_SE_enter01);
			this[0].fAlpha = 0.4f;
			SetColorTitle(0, this[0].fAlpha);
			this[1].fAlpha = 0.0f;
			SetColorTitle(1,this[1].fAlpha);
			this[0].TitleSelectTime = true;
			this[2].bDisp = true;
			this[3].bDisp = true;
		}
		//���߂͓����e�N�X�`���̃A���t�@�l���グ�Ă���������悤�ɂ���
		if (this[1].fAlpha < 1.0f)
		{
			this[1].fAlpha += 0.05f;
			if (this[1].fAlpha >= 1.0f)
			{
				this[1].fAlpha = 1.0f;
			}
			SetColorTitle(1, this[1].fAlpha);
		}
		//�A���t�@�l���ő�܂ŗ�����J�E���g���Ƃ�A�_�ł��J��Ԃ�
		else
		{
			this[1].nCountAppearStart++;
			if (this[1].nCountAppearStart > COUNT_APPERA_START)
			{
				this[1].nCountDisp = (this[1].nCountDisp + 1) % 80;
				if (this[1].nCountDisp > INTERVAL_DISP_START)
				{
					this[1].bDisp = false;
				}
				else
				{
					this[1].bDisp = true;
				}
			}
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void TITLE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this[0].tex2DVB.GetpD3DVtxBuff(), 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this[0].tex2DVB.GetpD3DTexture());

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);

	if (this[1].bDisp == true)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, this[1].tex2DVB.GetpD3DVtxBuff(), 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, this[1].tex2DVB.GetpD3DTexture());

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
	}


	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (this[CntTitleSelectScene].bDisp == true)
		{

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this[CntTitleSelectScene].tex2DVB.GetpD3DVtxBuff(), 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, this[CntTitleSelectScene].tex2DVB.GetpD3DTexture());

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
	//�^�C�g��BG����
	{
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&pD3DVtxBuff,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
			pD3DVtxBuff->Unlock();

			this[0].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
		}
	}

	//�^�C�g���X�^�[�g���S����
	{
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[1].fAlpha);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			pD3DVtxBuff->Unlock();
			this[1].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
		}
	}

	//�^�C�g���Z���N�g���S����
	{
		for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
		{
			LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;

			// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
			if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
				D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
				FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
				D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
				&pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
				NULL)))						// NULL�ɐݒ�
			{
				return E_FAIL;
			}

			{//���_�o�b�t�@�̒��g�𖄂߂�
				VERTEX_2D *pVtx;

				// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
				pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, this[CntTitleSelectScene].fAlpha);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				// ���_�f�[�^���A�����b�N����
				pD3DVtxBuff->Unlock();
				this[CntTitleSelectScene].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
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
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntTitle].tex2DVB.GetpD3DVtxBuff();
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// ���_�f�[�^���A�����b�N����
		pD3DVtxBuff->Unlock();

		this[CntTitle].tex2DVB.SetpD3DVtxBuff(pD3DVtxBuff);
	}

}

