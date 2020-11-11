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
#define	TEXTURE_TITLE_BG				"../data/TEXTURE/title.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LOGO_START				"../data/TEXTURE/press_enter.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_TUTORIAL	"../data/TEXTURE/rensyurogo.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_GAME		"../data/TEXTURE/taisenrogo.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_GAME_LOCAL	"../data/TEXTURE/local.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_SELECT_GAME_NET	"../data/TEXTURE/network.png"	// �ǂݍ��ރe�N�X�`���t�@�C����


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
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_BG, &this[0].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_LOGO_START, &this[1].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_TUTORIAL, &this[2].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME, &this[3].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME_LOCAL, &this[4].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME_NET, &this[5].tex2DVB.pD3DTexture);

}

//=============================================================================
// �ď���������
//=============================================================================
void TITLE::Reinit(void)
{
	this[0].HierarchyNum = 0;
	this[0].SceneNum = TITLE_SELECT_SCENE_LOCAL;
	this[0].fAlpha = 1.0f;

	this[TITLE_OBJECT_NAME_TUTO].bDisp = false;
	this[TITLE_OBJECT_NAME_BATTLE].bDisp = false;
	this[TITLE_OBJECT_NAME_LOCAL].bDisp = false;
	this[TITLE_OBJECT_NAME_NET].bDisp = false;

	this[0].SetColorTitle(TITLE_OBJECT_NAME_TITLELOGO, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_START, 1.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
	this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
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
void TITLE::Update(FADE *fade)
{
		switch (this[0].HierarchyNum)
		{
		//�^�C�g���K�w
		case TITLE_HIERARCHY_NUM_TITLE:
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TITLELOGO, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_START, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
			//�`���[�g���A���Ό��K�w�֐؂�ւ�
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				this[0].HierarchyNum = TITLE_HIERARCHY_NUM_TUTO_BATTLE;
				this[TITLE_OBJECT_NAME_TUTO].bDisp = true;
				this[TITLE_OBJECT_NAME_BATTLE].bDisp = true;
				this[0].SceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
			//�؂�ւ�
			else if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SOUND_LABEL_SE_attack02);
			}
			break;
			//�`���[�g���A���Ό��K�w
		case TITLE_HIERARCHY_NUM_TUTO_BATTLE:
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TITLELOGO, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_START, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.2f);
			//�^�C�g���K�w�֐؂�ւ�
			if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				this[0].HierarchyNum = TITLE_HIERARCHY_NUM_TITLE;
				this[TITLE_OBJECT_NAME_TUTO].bDisp = false;
				this[TITLE_OBJECT_NAME_BATTLE].bDisp = false;

			}
			//����͂Ń`���[�g���A���ɃJ�[�\�����킹��
			else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_ANALOG_L_UP))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
			//�����͂őΌ��ɃJ�[�\�����킹��
			else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_ANALOG_L_DOWN))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_BATTLE;
			}

			if (this[0].SceneNum == TITLE_SELECT_SCENE_TUTORIAL)
			{
				this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 1.0f);
				this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.2f);
				//�Q�[���V�[�����`���[�g���A����
				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
				{
					PlaySound(SOUND_LABEL_SE_enter01);
					fade->SetFade(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
				}
			}
			else if (this[0].SceneNum == TITLE_SELECT_SCENE_BATTLE)
			{
				this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.2f);
				this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 1.0f);
				//���[�J���l�b�g�K�w�֐؂�ւ�
				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
				{
					PlaySound(SOUND_LABEL_SE_enter01);
					this[0].HierarchyNum = TITLE_HIERARCHY_NUM_LOCAL_NET;
					this[TITLE_OBJECT_NAME_TUTO].bDisp = false;
					this[TITLE_OBJECT_NAME_BATTLE].bDisp = false;
					this[TITLE_OBJECT_NAME_LOCAL].bDisp = true;
					this[TITLE_OBJECT_NAME_NET].bDisp = true;
					this[0].SceneNum = TITLE_SELECT_SCENE_LOCAL;

				}
			}
			break;
			//���[�J���l�b�g�K�w
		case TITLE_HIERARCHY_NUM_LOCAL_NET:
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TITLELOGO, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_START, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.2f);
			//�`���[�g���A���Ό��K�w�֐؂�ւ�
			if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				this[0].HierarchyNum = TITLE_HIERARCHY_NUM_TUTO_BATTLE;
				this[TITLE_OBJECT_NAME_TUTO].bDisp = true;
				this[TITLE_OBJECT_NAME_BATTLE].bDisp = true;
				this[TITLE_OBJECT_NAME_LOCAL].bDisp = false;
				this[TITLE_OBJECT_NAME_NET].bDisp = false;

			}
			//����͂Ń��[�J���ɃJ�[�\�����킹��
			else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_ANALOG_L_UP))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_LOCAL;
			}
			//�����͂Ńl�b�g�ɃJ�[�\�����킹��
			else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_ANALOG_L_DOWN))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_NET;
			}

			if (this[0].SceneNum == TITLE_SELECT_SCENE_LOCAL)
			{
				this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 1.0f);
				this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.2f);
				//�V�[�����Q�[����
				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
				{
					PlaySound(SOUND_LABEL_SE_enter01);
					fade->SetFade(FADE_OUT, SCENE_GAMECOUNTDOWN, SOUND_LABEL_BGM_normal01);
				}
			}
			else if (this[0].SceneNum == TITLE_SELECT_SCENE_NET)
			{
				this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.2f);
				this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 1.0f);
				//�V�[�����l�b�g�}�b�`��
				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
				{
					PlaySound(SOUND_LABEL_SE_enter01);
					fade->SetFade(FADE_OUT, SCENE_NETMATCH, SOUND_LABEL_BGM_boss01);
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

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this[0].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this[0].tex2DVB.pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this[1].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this[1].tex2DVB.pD3DTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);


	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (this[CntTitleSelectScene + 2].bDisp == true)
		{

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, this[CntTitleSelectScene + 2].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, this[CntTitleSelectScene + 2].tex2DVB.pD3DTexture);

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

	//�^�C�g���X�^�[�g���S����
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

	//�^�C�g���Z���N�g���S����
	{
		for (int CntTitleSelectScene = TITLE_OBJECT_NAME_TUTO; CntTitleSelectScene < TITLE_OBJECT_NAME_LOCAL; CntTitleSelectScene++)
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
				int cnt = CntTitleSelectScene - TITLE_OBJECT_NAME_TUTO;
				pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);

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

	//�^�C�g���Z���N�g���S����
	{
		for (int CntTitleSelectScene = TITLE_OBJECT_NAME_LOCAL; CntTitleSelectScene < TITLE_OBJECT_NAME_MAX; CntTitleSelectScene++)
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
				int cnt = CntTitleSelectScene- TITLE_OBJECT_NAME_LOCAL;
				pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);

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

