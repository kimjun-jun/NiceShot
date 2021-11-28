/**
* @file title.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../../../h/main.h"
#include "../../../h/Other/input.h"
#include "../../../h/Object/Scene/Scene.h"
#include "../../../h/Other/sound.h"
#include "../../../h/net/sock.h"
#include "../../../h/Object/Title/title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr int	TITLE_BG_POS_X{ SCREEN_CENTER_X };				// �^�C�g��BG�̈ʒu(X���W)
constexpr int	TITLE_BG_POS_Y{ SCREEN_CENTER_Y };				// �^�C�g��BG�̈ʒu(Y���W)
constexpr int	TITLE_BG_WIDTH{ SCREEN_W };						// �^�C�g��BG�̕�
constexpr int	TITLE_BG_HEIGHT{ SCREEN_H };					// �^�C�g��BG�̍���
constexpr int	TITLE_LOGO_POS_X{ SCREEN_CENTER_X };			// �^�C�g�����S�̈ʒu(X���W)
constexpr int	TITLE_LOGO_POS_Y{ SCREEN_CENTER_Y - 200 };		// �^�C�g�����S�̈ʒu(Y���W)
constexpr int	TITLE_LOGO_HEIGHT{ 500 };						// �^�C�g�����S�̍���
constexpr int	TITLE_LOGO_WIDTH{ 800 };						// �^�C�g�����S�̕�
constexpr int	TITLE_PRESS_LOGO_POS_X{ SCREEN_CENTER_X };		// PRESS���S�̈ʒu(X���W)
constexpr int	TITLE_PRESS_LOGO_POS_Y{ SCREEN_CENTER_Y + 200 };// PRESS���S�̈ʒu(Y���W)
constexpr int	TITLE_PRESS_LOGO_WIDTH{ 400 };					// PRESS���S�̕�
constexpr int	TITLE_PRESS_LOGO_HEIGHT{ 100 };					// PRESS���S�̍���
constexpr int	TITLE_SELECT_POS_X{ SCREEN_CENTER_X };			// �Z���N�g�e�N�X�`���̈ʒu(X���W)
constexpr int	TITLE_SELECT_POS_Y{ SCREEN_CENTER_Y };			// �Z���N�g�e�N�X�`���̈ʒu(Y���W)
constexpr int	TITLE_SELECT_WIDTH{ 300 };						// �Z���N�g�e�N�X�`���̕�
constexpr int	TITLE_SELECT_HEIGHT{ 200 };						// �Z���N�g�e�N�X�`���̍���
constexpr int	TITLE_SELECT_OFFSET_X{ 200 };					// �I�t�Z�b�g�l
constexpr int	TITLE_SELECT_OFFSET_Y{ 200 };					// �I�t�Z�b�g�l
constexpr int	COUNT_APPERA_START{ 10 };						// �X�^�[�g�{�^���o���܂ł̎���
constexpr int	INTERVAL_DISP_START{ 30 };						// �X�^�[�g�{�^���_�ł̎���
constexpr int	COUNT_WAIT_DEMO{ 60 * 5 };						// �f���܂ł̑҂�����

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
TITLE::TITLE(void)
{
	//���_�쐬
	this->vtx.MakeVertex2D(OBJECT_TITLE_MAX, FVF_VERTEX_2D);

	//�`��ʒu�ݒ�
	this->Transform[NAME_BG].Pos(D3DXVECTOR3(TITLE_BG_POS_X, TITLE_BG_POS_Y, 0.0f));
	this->Transform[NAME_LOGO].Pos(D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y, 0.0f));
	this->Transform[NAME_PRESS].Pos(D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X, TITLE_PRESS_LOGO_POS_Y, 0.0f));
	this->Transform[NAME_TUTO].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_OFFSET_Y, 0.0f));
	this->Transform[NAME_LOCAL].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_OFFSET_Y, 0.0f));
	this->Transform[NAME_NET].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_OFFSET_Y, 0.0f));
	this->Transform[NAME_QUIT].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_OFFSET_Y, 0.0f));

	//�`��ʒu���f
	this->vtx.Vertex2D(NAME_BG, TITLE_BG_WIDTH / 2, TITLE_BG_HEIGHT / 2, this->Transform[NAME_BG].Pos());
	this->vtx.Vertex2D(NAME_LOGO, TITLE_LOGO_WIDTH / 2, TITLE_LOGO_HEIGHT / 2, this->Transform[NAME_LOGO].Pos());
	this->vtx.Vertex2D(NAME_PRESS, TITLE_PRESS_LOGO_WIDTH / 2, TITLE_PRESS_LOGO_HEIGHT / 2, this->Transform[NAME_PRESS].Pos());
	this->vtx.Vertex2D(NAME_TUTO, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_TUTO].Pos());
	this->vtx.Vertex2D(NAME_LOCAL, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_LOCAL].Pos());
	this->vtx.Vertex2D(NAME_NET, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_NET].Pos());
	this->vtx.Vertex2D(NAME_QUIT, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_QUIT].Pos());

	//�J�E���g���[�v
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//RHW�ݒ�
		this->vtx.RHW2D(CntTitle);

		//UV�ݒ�
		this->vtx.UV2D(CntTitle);

		// �e�N�X�`���ǂݍ���
		this->tex[CntTitle].LoadTexture(this->c_FileNameTex[CntTitle]);

	}
}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
TITLE::~TITLE(void)
{
	//�J�E���g���[�v
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//�e�N�X�`�����
		this->tex[CntTitle].~TEXTURE();
	}
	//���_���
	this->vtx.~VTXBUFFER();
}

//=============================================================================
// ���N���X�̃A�h���X�擾
//=============================================================================
void TITLE::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pscene = obj->GetScene();
}

//=============================================================================
// ����������
//=============================================================================
void TITLE::Init(void)
{
	//�J�E���g���[�v
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//�J���[�̐ݒ�Ǝg�p�ݒ�
		{
			D3DXCOLOR col;
			switch (CntTitle)
			{
				//�w�i�ƃ��S��PRESS�摜����������Ԃ���g�p
			case NAME_BG:
			case NAME_LOGO:
			case NAME_PRESS:
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				this->iUseType[CntTitle].Use(YesUseType1);
				break;

				//����ȊO�s�g�p
			default:
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				this->iUseType[CntTitle].Use(NoUse);
				break;
			}
			this->vtx.Color2D(CntTitle, col);
		}
	}

	//���[�h�I�𒆂̃f�[�^������
	this->TitlePara.HierarchyNum = TITLE_HIERARCHY_NUM_ANIM;
	this->TitlePara.SceneNumX = this->TitlePara.SceneNumY = this->TitlePara.OldSceneNumX = this->TitlePara.OldSceneNumY = 0;
}

//=============================================================================
// �X�V����
//=============================================================================
void TITLE::Update(void)
{
	//�J�[�\����ۑ�
	this->TitlePara.OldSceneNumX = this->TitlePara.SceneNumX;
	this->TitlePara.OldSceneNumY = this->TitlePara.SceneNumY;

	//�K�w�m�F
	switch (this->TitlePara.HierarchyNum)
	{
		//�^�C�g���A�j���K�w��
	case TITLE_HIERARCHY_NUM_ANIM:
		//A�{�^����ENTER�����͂��ꂽ��K�w�؂�ւ�
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//�q�G�����L�[�ύX
			ChangeHierarchy(TITLE_HIERARCHY_NUM_SELECT);//�ړI�̃q�G�����L�[������
		}
		break;
		//�V�[���Z���N�g�K�w��
	case TITLE_HIERARCHY_NUM_SELECT:
		//B�{�^�����o�b�NSpace�����͂��ꂽ��K�w�؂�ւ�
		if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
		{
			//�q�G�����L�[�ύX
			ChangeHierarchy(TITLE_HIERARCHY_NUM_ANIM);
		}
		//�J�[�\���ړ�����
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_ANALOG_L_UP) || IsButtonTriggered(0, BUTTON_DIGITAL_UP))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter03);
			//�J�[�\���ݒ�
			this->TitlePara.SceneNumY++;
			if (this->TitlePara.SceneNumY >= 2)this->TitlePara.SceneNumY = 0;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_ANALOG_L_DOWN) || IsButtonTriggered(0, BUTTON_DIGITAL_DOWN))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter03);
			//�J�[�\���ݒ�
			this->TitlePara.SceneNumY--;
			if (this->TitlePara.SceneNumY <= -1)this->TitlePara.SceneNumY = 1;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_ANALOG_L_RIGHT) || IsButtonTriggered(0, BUTTON_DIGITAL_RIGHT))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter03);
			//�J�[�\���ݒ�
			this->TitlePara.SceneNumX++;
			if (this->TitlePara.SceneNumX >= 2)this->TitlePara.SceneNumX = 0;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_ANALOG_L_LEFT) || IsButtonTriggered(0, BUTTON_DIGITAL_LEFT))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter03);
			//�J�[�\���ݒ�
			this->TitlePara.SceneNumX--;
			if (this->TitlePara.SceneNumX <= -1)this->TitlePara.SceneNumX = 1;
		}
		//scene�I������
		this->CheckScene();
		break;
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

	//�J�E���g���[�v
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//�`�攻��@
		if (this->iUseType[CntTitle].Use() == YesUseType1)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0,this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);
			// �e�N�X�`���̐ݒ�@�e�N�X�`���������Ȃ�tex��z�񉻂��đI��������悤��
			pDevice->SetTexture(0, this->tex[CntTitle].Texture());
			// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntTitle * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// �K�w�؂�ւ�����
//=============================================================================
void TITLE::ChangeHierarchy(int NextHierarchyType)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//�q�G�����L�[�I��
	switch (NextHierarchyType)
	{
		//�^�C�g���A�j���ɕύX
	case TITLE_HIERARCHY_NUM_ANIM:
		//�I�����Đ�
		PlaySound(SOUND_LABEL_SE_enter01);
		//�q�G�����L�[�ύX
		this->TitlePara.HierarchyNum = TITLE_HIERARCHY_NUM_ANIM;
		//���l�ύX
		this->vtx.Color2D(NAME_LOGO, col);
		this->vtx.Color2D(NAME_PRESS, col);
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		this->vtx.Color2D(NAME_TUTO, col);
		this->vtx.Color2D(NAME_LOCAL, col);
		this->vtx.Color2D(NAME_NET, col);
		this->vtx.Color2D(NAME_QUIT, col);
		//�g�p�ύX
		this->iUseType[NAME_TUTO].Use(NoUse);
		this->iUseType[NAME_LOCAL].Use(NoUse);
		this->iUseType[NAME_NET].Use(NoUse);
		this->iUseType[NAME_QUIT].Use(NoUse);
		break;
		//�V�[���I���ɕύX
	case TITLE_HIERARCHY_NUM_SELECT:
		//�I�����Đ�
		PlaySound(SOUND_LABEL_SE_enter01);
		//�q�G�����L�[�ύX
		this->TitlePara.HierarchyNum = TITLE_HIERARCHY_NUM_SELECT;
		//�I���V�[�����Z�b�g
		this->TitlePara.SceneNumX = this->TitlePara.SceneNumY = 0;
		//���l�ύX
		this->vtx.Color2D(NAME_TUTO, col);
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		this->vtx.Color2D(NAME_LOCAL, col);
		this->vtx.Color2D(NAME_NET, col);
		this->vtx.Color2D(NAME_QUIT, col);
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		this->vtx.Color2D(NAME_LOGO, col);
		this->vtx.Color2D(NAME_PRESS, col);
		//�g�p�ύX
		this->iUseType[NAME_TUTO].Use(YesUseType1);
		this->iUseType[NAME_LOCAL].Use(YesUseType1);
		this->iUseType[NAME_NET].Use(YesUseType1);
		this->iUseType[NAME_QUIT].Use(YesUseType1);
		break;
	default:
		break;
	}
}

//=============================================================================
// �V�[���؂�ւ�����
//=============================================================================
void TITLE::CheckScene(void)
{
	//�`���[�g���A��
	if (this->TitlePara.SceneNumX == 0 && this->TitlePara.SceneNumY == 0)
	{
		//�J�[�\�����ړ������烿�l�ݒ�
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//���l�ύX
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_TUTO, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_LOCAL, col);
			this->vtx.Color2D(NAME_NET, col);
			this->vtx.Color2D(NAME_QUIT, col);
		}
		//�V�[�����`���[�g���A����
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter01);
			pscene->NextScene(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
		}
	}
	//���[�J���ΐ�
	else if (this->TitlePara.SceneNumX == 1 && this->TitlePara.SceneNumY == 0)
	{
		//�J�[�\�����ړ������烿�l�ݒ�
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//���l�ύX
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_LOCAL, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_TUTO, col);
			this->vtx.Color2D(NAME_NET, col);
			this->vtx.Color2D(NAME_QUIT, col);
		}
		//�V�[�����Q�[����
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter01);
			pscene->NextScene(FADE_OUT, SCENE_GAMECOUNTDOWN, SOUND_LABEL_BGM_normal01);
			SourceVolumeChange(0,SOUND_LABEL_BGM_normal01);
		}
	}
	//�l�b�g�ΐ�
	else if (this->TitlePara.SceneNumX == 0 && this->TitlePara.SceneNumY == 1)
	{
		//�J�[�\�����ړ������烿�l�ݒ�
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//���l�ύX
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_NET, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_TUTO, col);
			this->vtx.Color2D(NAME_LOCAL, col);
			this->vtx.Color2D(NAME_QUIT, col);
		}
		//�V�[�����Q�[����
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter01);
			pscene->NextScene(FADE_OUT, SCENE_NETMATCH, SOUND_LABEL_BGM_select01);
		}
	}
	//�A�v���I����
	else if (this->TitlePara.SceneNumX == 1 && this->TitlePara.SceneNumY == 1)
	{
		//�J�[�\�����ړ������烿�l�ݒ�
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//���l�ύX
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_QUIT, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_LOCAL, col);
			this->vtx.Color2D(NAME_NET, col);
			this->vtx.Color2D(NAME_TUTO, col);
		}
		//�A�v���I��
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//�I�����Đ�
			PlaySound(SOUND_LABEL_SE_enter01);
			SetMsg(WM_QUIT);
		}
	}
}


