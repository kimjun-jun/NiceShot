///**
//* @file time.cpp
//* @brief NiceShot(3D)��ԃQ�[��
//* @author �L�����W����
//*/
//#include "../../h/main.h"
//#include "../../h/scene/time.h"
//#include "../../h/scene/fade.h"
//#include "../../h/other/sound.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
///**
//* @brief ���_�����֐� MakeVertexTime
//* @return HRESULT
//*/
//HRESULT MakeVertexTime(void);
//
///**
//* @brief �e�N�X�`���ݒ�֐� SetTextureTime
//*/
//void SetTextureTime(void);
//
///**
//* @brief ���_�ݒ�֐� SetVertexTime
//*/
//void SetVertexTime(void);
//
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//static TIME  g_time[TIME_DIGIT];		//!< �^�C���̃J�E���g�\���̕ϐ�
//static DOT g_dot;						//!< �����_�\���̕ϐ�
//static TIMELOGO g_timelogo;				//!< �^�C�����S�\���̕ϐ�
//static int g_time_maneger;				//!< �c�莞��
//
////=============================================================================
//// ����������
////=============================================================================
//HRESULT InitTime(int type)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	for (int i = 0; i < TIME_DIGIT; i++)
//	{
//		TIME *time = &g_time[i];
//		time->pos = D3DXVECTOR3(TEXTURE_TIME_POS_X - (NUM_INTERVAL_X*i), TEXTURE_TIME_POS_Y, 0.0f);
//		time->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		time->nCountAnim = 0;
//		time->nPatternAnim = 0;
//		time->use = true;
//		// �e�N�X�`���̓ǂݍ���  
//		if (type == 0)	// ����̂ݓǂݍ���
//		{
//			D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
//				TEXTURE_GAME_SCORE,				// �t�@�C���̖��O
//				&time->pD3DTexture);			// �ǂݍ��ރ������̃|�C���^
//		}
//	}
//
//	g_dot.pos = D3DXVECTOR3(g_time[0].pos.x - TEXTURE_TIME_SIZE_X / 2, TEXTURE_TIMELOGO_POS_Y+ (TEXTURE_TIME_SIZE_Y/2)+ TEXTURE_DOT_SIZE_Y, 0.0f);
//	g_timelogo.pos = D3DXVECTOR3(TEXTURE_TIMELOGO_POS_X, TEXTURE_TIMELOGO_POS_Y, 0.0f);
//	// �e�N�X�`���̓ǂݍ���  
//	if (type == 0)	// ����̂ݓǂݍ���
//	{
//		D3DXCreateTextureFromFile(pDevice,
//			TEXTURE_GAME_DOT,
//			&g_dot.pD3DTexture);
//
//		D3DXCreateTextureFromFile(pDevice,
//			TEXTURE_GAME_TIMELOGO,
//			&g_timelogo.pD3DTexture);
//
//	}
//	g_time_maneger = FPS_TIME_COUNT;
//	MakeVertexTime();
//	SetTextureTime();
//	return S_OK;
//}
//
////=============================================================================
//// �ď���������
////=============================================================================
//void ReInitTime(void)
//{
//	for (int i = 0; i < TIME_DIGIT; i++)
//	{
//		TIME *time = &g_time[i];
//		time->nCountAnim = 0;
//		time->nPatternAnim = 0;
//		time->use = true;
//	}
//
//	g_time_maneger = FPS_TIME_COUNT;
//	MakeVertexTime();
//	SetTextureTime();
//	UpdateTime();
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void UninitTime(void)
//{
//	for (int i = 0; i < TIME_DIGIT; i++)
//	{
//		TIME *time = &g_time[i];
//		if (time->pD3DTexture != NULL)
//		{	// �e�N�X�`���̊J��
//			time->pD3DTexture->Release();
//			time->pD3DTexture = NULL;
//		}
//	}
//	if (g_dot.pD3DTexture != NULL)
//	{	// �e�N�X�`���̊J��
//		g_dot.pD3DTexture->Release();
//		g_dot.pD3DTexture = NULL;
//	}
//	if (g_timelogo.pD3DTexture != NULL)
//	{	// �e�N�X�`���̊J��
//		g_timelogo.pD3DTexture->Release();
//		g_timelogo.pD3DTexture = NULL;
//	}
//}
//
////=============================================================================
//// �X�V����
////=============================================================================
//void UpdateTime(void)
//{
//	SetTextureTime();
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void DrawTime(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//	for (int i = 0; i < TIME_DIGIT; i++)
//	{
//		TIME *time = &g_time[i];
//		if (time->use == true)
//		{
//			pDevice->SetTexture(0, time->pD3DTexture);
//			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, time->texture, sizeof(VERTEX_2D));
//		}
//	}
//	DOT *dot = &g_dot;
//	pDevice->SetTexture(0, dot->pD3DTexture);
//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, dot->texture, sizeof(VERTEX_2D));
//	TIMELOGO *tlogo = &g_timelogo;
//	pDevice->SetTexture(0, tlogo->pD3DTexture);
//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, tlogo->texture, sizeof(VERTEX_2D));
//
//}
//
////=============================================================================
//// ���_�̍쐬
////=============================================================================
//HRESULT MakeVertexTime(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	// ���_���W�̐ݒ�
//	SetVertexTime();
//	for (int i = 0; i < TIME_DIGIT; i++)
//	{
//		g_time[i].texture[0].rhw =
//			g_time[i].texture[1].rhw =
//			g_time[i].texture[2].rhw =
//			g_time[i].texture[3].rhw = 1.0f;
//		g_time[i].texture[0].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//		g_time[i].texture[1].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//		g_time[i].texture[2].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//		g_time[i].texture[3].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//		g_time[i].texture[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		g_time[i].texture[1].tex = D3DXVECTOR2(1.0f / SCORE_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
//		g_time[i].texture[2].tex = D3DXVECTOR2(0.0f, 1.0f / SCORE_TEXTURE_PATTERN_DIVIDE_Y);
//		g_time[i].texture[3].tex = D3DXVECTOR2(1.0f / SCORE_TEXTURE_PATTERN_DIVIDE_X, 1.0f / SCORE_TEXTURE_PATTERN_DIVIDE_Y);
//	}
//
//	g_dot.texture[0].rhw =
//		g_dot.texture[1].rhw =
//		g_dot.texture[2].rhw =
//		g_dot.texture[3].rhw = 1.0f;
//	g_dot.texture[0].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//	g_dot.texture[1].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//	g_dot.texture[2].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//	g_dot.texture[3].diffuse = D3DCOLOR_RGBA(127, 255, 0, 255);
//	g_dot.texture[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	g_dot.texture[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	g_dot.texture[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	g_dot.texture[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	g_timelogo.texture[0].rhw =
//		g_timelogo.texture[1].rhw =
//		g_timelogo.texture[2].rhw =
//		g_timelogo.texture[3].rhw = 1.0f;
//	g_timelogo.texture[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
//	g_timelogo.texture[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
//	g_timelogo.texture[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
//	g_timelogo.texture[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
//	g_timelogo.texture[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	g_timelogo.texture[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	g_timelogo.texture[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	g_timelogo.texture[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	return S_OK;
//}
//
////=============================================================================
//// �e�N�X�`�����W�̐ݒ�
////=============================================================================
//void SetTextureTime(void)
//{
//	int timeval = g_time_maneger;
//	timeval /= 7;
//	if (timeval > 0)
//	{
//		if (timeval <= 100)
//		{
//			g_time[0].texture[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[0].texture[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[0].texture[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[0].texture[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[1].texture[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[1].texture[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[1].texture[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[1].texture[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_time[2].use = false;
//
//			g_dot.texture[0].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_dot.texture[1].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_dot.texture[2].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//			g_dot.texture[3].diffuse = D3DCOLOR_RGBA(255, 0, 0, 255);
//		}
//		for (int i = 0; i < TIME_DIGIT; i++)
//		{
//			TIME *time = &g_time[i];
//			float tx = (float)(timeval % 10);
//			// �e�N�X�`�����W�̐ݒ�
//			time->texture[0].tex = D3DXVECTOR2(tx*0.1f, 0.0f);
//			time->texture[1].tex = D3DXVECTOR2((tx + 1)*0.1f, 0.0f);
//			time->texture[2].tex = D3DXVECTOR2(tx*0.1f, 1.0f);
//			time->texture[3].tex = D3DXVECTOR2((tx + 1)*0.1f, 1.0f);
//			timeval /= 10;
//		}
//	}
//}
//
////=============================================================================
//// ���_���W�̐ݒ�
////=============================================================================
//void SetVertexTime(void)
//{
//	for (int i = 0; i < TIME_DIGIT; i++)
//	{
//		TIME *time = &g_time[i];
//		// ���_���W�̐ݒ� 
//		time->texture[0].vtx.x = time->pos.x - TEXTURE_TIME_SIZE_X;
//		time->texture[0].vtx.y = time->pos.y - TEXTURE_TIME_SIZE_Y;
//		time->texture[0].vtx.z = 0.0f;
//
//		time->texture[1].vtx.x = time->pos.x + TEXTURE_TIME_SIZE_X;
//		time->texture[1].vtx.y = time->pos.y - TEXTURE_TIME_SIZE_Y;
//		time->texture[1].vtx.z = 0.0f;
//
//		time->texture[2].vtx.x = time->pos.x - TEXTURE_TIME_SIZE_X;
//		time->texture[2].vtx.y = time->pos.y + TEXTURE_TIME_SIZE_Y;
//		time->texture[2].vtx.z = 0.0f;
//
//		time->texture[3].vtx.x = time->pos.x + TEXTURE_TIME_SIZE_X;
//		time->texture[3].vtx.y = time->pos.y + TEXTURE_TIME_SIZE_Y;
//		time->texture[3].vtx.z = 0.0f;
//	}
//
//	DOT *dot = &g_dot;
//	// ���_���W�̐ݒ� 
//	dot->texture[0].vtx.x = dot->pos.x - TEXTURE_DOT_SIZE_X;
//	dot->texture[0].vtx.y = dot->pos.y - TEXTURE_DOT_SIZE_Y;
//	dot->texture[0].vtx.z = 0.0f;
//	dot->texture[1].vtx.x = dot->pos.x + TEXTURE_DOT_SIZE_X;
//	dot->texture[1].vtx.y = dot->pos.y - TEXTURE_DOT_SIZE_Y;
//	dot->texture[1].vtx.z = 0.0f;
//	dot->texture[2].vtx.x = dot->pos.x - TEXTURE_DOT_SIZE_X;
//	dot->texture[2].vtx.y = dot->pos.y + TEXTURE_DOT_SIZE_Y;
//	dot->texture[2].vtx.z = 0.0f;
//	dot->texture[3].vtx.x = dot->pos.x + TEXTURE_DOT_SIZE_X;
//	dot->texture[3].vtx.y = dot->pos.y + TEXTURE_DOT_SIZE_Y;
//	dot->texture[3].vtx.z = 0.0f;
//
//	TIMELOGO *tlogo = &g_timelogo;
//	tlogo->texture[0].vtx.x = tlogo->pos.x - TEXTURE_TIMELOGO_SIZE_X;
//	tlogo->texture[0].vtx.y = tlogo->pos.y - TEXTURE_TIMELOGO_SIZE_Y;
//	tlogo->texture[0].vtx.z = 0.0f;
//	tlogo->texture[1].vtx.x = tlogo->pos.x + TEXTURE_TIMELOGO_SIZE_X;
//	tlogo->texture[1].vtx.y = tlogo->pos.y - TEXTURE_TIMELOGO_SIZE_Y;
//	tlogo->texture[1].vtx.z = 0.0f;
//	tlogo->texture[2].vtx.x = tlogo->pos.x - TEXTURE_TIMELOGO_SIZE_X;
//	tlogo->texture[2].vtx.y = tlogo->pos.y + TEXTURE_TIMELOGO_SIZE_Y;
//	tlogo->texture[2].vtx.z = 0.0f;
//	tlogo->texture[3].vtx.x = tlogo->pos.x + TEXTURE_TIMELOGO_SIZE_X;
//	tlogo->texture[3].vtx.y = tlogo->pos.y + TEXTURE_TIMELOGO_SIZE_Y;
//	tlogo->texture[3].vtx.z = 0.0f;
//}
//
////=============================================================================
//// �^�C���̃A�h���X��Ԃ�
////=============================================================================
//int *GetTimemaneger(void)
//{
//	return &g_time_maneger;
//}
//
////=============================================================================
//// �X�R�A���Z����
////=============================================================================
//void AddTime(int val)
//{
//	g_time_maneger += val;
//	if (g_time_maneger == 0)
//	{
//		SetFade(FADE_OUT, SCENE_RESULT, SOUND_LABEL_BGM_gameover01);
//	}
//}
