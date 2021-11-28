/**
* @file Scene.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/

#include "../../../h/main.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Object/Scene/Scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
constexpr float	SCENE_RATE{ 0.02f };									//!< �t�F�[�h�W��

//=============================================================================
// �R���X�g���N�^�@�u�ǂݍ��݁v�u�������v
//=============================================================================
SCENE::SCENE(void)
{
	//���_�̍쐬
	this->vtx.MakeVertex2D(OBJECT_SCENE_MAX, FVF_VERTEX_2D);

	//�`��ʒu���f�@�X�N���[���T�C�Y�ō쐬
	D3DXVECTOR3 VTX[POLYGON_2D_VERTEX] = {
	VEC3_ALL0,
	D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, SCREEN_H, 0.0f),
	D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f),
	};
	this->vtx.Vertex2D(0, VTX);

	//RHW�ݒ�
	this->vtx.RHW2D(0);

	//UV�ݒ�
	this->vtx.UV2D(0);

}

//=============================================================================
// �f�X�g���N�^�@�폜
//=============================================================================
SCENE::~SCENE(void)
{
	//���_���
	this->vtx.~VTXBUFFER();
}

//=============================================================================
// ����������
//=============================================================================
void SCENE::Init(void)
{
	//�����l�ݒ�
	this->ScenePara.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->ScenePara.eFade = FADE_IN;
	this->ScenePara.eNowScene = SCENE_TITLE;
	this->ScenePara.eNextScene = SCENE_TITLE;
	this->ScenePara.sno = -1;
}

//=============================================================================
// �X�V����
//=============================================================================
void SCENE::Update(void)
{
	if (this->ScenePara.eFade != FADE_NONE)
	{// �t�F�[�h������
		if (this->ScenePara.eFade == FADE_OUT)
		{// �t�F�[�h�A�E�g����
			this->ScenePara.color.a += SCENE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���
			if (this->ScenePara.color.a >= 1.0f)
			{
				// ��Ԃ�؂�ւ�
				SetScene(this->ScenePara.eNextScene);

				// �t�F�[�h�C�������ɐ؂�ւ�
				this->ScenePara.color.a = 1.0f;
				this->ScenePara.eFade = FADE_MUSIC_STOP;

				// BGM��~
				StopSound();
			}

			// �F��ݒ�
			this->vtx.Color2D(0, this->ScenePara.color);
		}
		else if (this->ScenePara.eFade == FADE_MUSIC_STOP)
		{
			// BGM�Đ�
			if (this->ScenePara.sno > -1)
			{
				PlaySound(this->ScenePara.sno);
			}

			this->ScenePara.eFade = FADE_IN;
		}
		else if (this->ScenePara.eFade == FADE_IN)
		{// �t�F�[�h�C������
			this->ScenePara.color.a -= SCENE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
			if (this->ScenePara.color.a <= 0.0f)
			{
				// �t�F�[�h�����I��
				this->ScenePara.color.a = 0.0f;
				this->ScenePara.eFade = FADE_NONE;

			}

			// �F��ݒ�
			this->vtx.Color2D(0, this->ScenePara.color);
		}
	}
}

//=============================================================================
// �t�F�[�h���
//=============================================================================
void SCENE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	// �|���S���̕`��@������ڂ̕`��J�n���_��ݒ肷�邱�Ƃ��厖
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
}

//=============================================================================
// �V�[����؂�ւ���
//=============================================================================
void SCENE::SetScene(e_SCENE next)
{
	this->ScenePara.eNowScene = next;

}

//=============================================================================
// �V�[���؂�ւ��󂯎��
//=============================================================================
void SCENE::NextScene(eFADE_TYPE eFade, e_SCENE next, int sno)
{
	this->ScenePara.eFade = eFade;
	this->ScenePara.eNextScene = next;
	this->ScenePara.sno = sno;

}

