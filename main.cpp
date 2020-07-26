/**
* @file main.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "field.h"
#include "wall.h"
#include "shadow.h"
#include "collision.h"
#include "time.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "tutorial.h"
#include "effect.h"
#include "bullet.h"
#include "bulletpoint.h"
#include "explosion.h"
#include "rank.h"
#include "countdown.h"
#include "item.h"
#include "life.h"
#include "bullettex.h"
#include "damege.h"
#include "status.h"
#include "sound.h"
#include "bulletprediction.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"				//!< �E�C���h�E�̃N���X��
#define WINDOW_NAME		_T("�i�C�X�V���b�g�I")	//!< �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
* @brief �������֐�
* @param[in]�@HWND hWnd, BOOL bWindow
* @return HRESULT
* @details �N�����̏�����
*/
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

/**
* @brief �������J���֐�
* @details �Q�[���I�����Ƀ������J������
*/
void Uninit(void);

/**
* @brief �A�b�v�f�[�g�֐�
* @details �I�u�W�F�N�g���X�V����
*/
void Update(void);

/**
* @brief �`��֐�
* @details �I�u�W�F�N�g��`�悷��
*/
void Draw(void);

#ifdef _DEBUG
/**
* @brief FPS�\���֐�
* @details �f�o�b�O���s��FPS��\������
*/
void DrawFPS(void);
#endif

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;				//!< Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		//!< Device�I�u�W�F�N�g(�`��ɕK�v)
int					g_nScene = SCENE_TITLE;		//!< �X�e�[�W�ԍ�
int					stop = 0;					//!< �f�o�b�O���̈ꎞ��~�p�ϐ�

#ifdef _DEBUG
static LPD3DXFONT	g_pD3DXFont = NULL;			//!< �t�H���g�ւ̃|�C���^
int					g_nCountFPS;				//!< FPS�J�E���^
char				g_text[256] = { 0 };		//!< �\��������e�L�X�g
DWORD				dwFrameCount;				//!< ���Ԍv���p
#endif


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));

	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_W + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_H + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	bool mode;
	//�߂�l��int�^�Ŗ߂��Ă���
	//������MessageBox���ĂԂ�window���o����B
	//����1��NULL�B����2�͖{���B
	//����3��windou���B
	//����4��MB_YESNOCANCEL����ŃC�G�X�m�[�L�����Z���̎O�����o����B�����͂����Ȏ�ނ����邩�玩���Œ��ׂ�MB_OK��OK����
	//����4�̌��Ɂu| MB_ICONQUESTION�v�ŃA�C�R�����o�����Ƃ��ł���B
	int id = MessageBox(NULL, "Window���[�h�Ńv���C���܂����H", "�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);

	//�����ꂽ�I�����ŃX�C�b�`����
	switch (id)
	{
	case IDYES:		// Yes�Ȃ�Window���[�h�ŋN��
		mode = true;
		break;
	case IDNO:		// No�Ȃ�t���X�N���[�����[�h�ŋN��
		mode = false;
		break;
	case IDCANCEL:	// CANCEL�Ȃ�I��
	default:
		return -1;
		break;
	}

	if (FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	PlaySound(SOUND_LABEL_BGM_title01);

	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				// FPS�\��
				PrintDebugProc("FPS: %d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_W;							// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_H;							// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if (bWindow)
	{// �E�B���h�E���[�h
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;			// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h

		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;							// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// �C���^�[�o��

	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

#ifdef _DEBUG
// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);
#endif

	// FOG�̐ݒ�
	FLOAT StartPos = 50;  //�J�n�ʒu
	FLOAT EndPos = 300; //�I���ʒu
	g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);								//�t�H�O�FOFF
	g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	//�J���[
	g_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);						//���_���[�h
	g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);					//�e�[�u�����[�h
	g_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));					//�J�n�ʒu
	g_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));						//�I���ʒu	

	//����������
	InitSound(hWnd);
	InitDebugProc();
	InitFade();
	InitTitle();
	InitTutorial(0);
	InitTime(0);
	InitCountdown(0);
	InitResult(0);
	InitBullet(0);
	InitEffect(0);
	InitBulletprediction(0);
	InitExplosion(0);
	InitLifeTex(0);
	InitBulletTex(0);
	InitDamege(0);
	InitStatus(0);
	InitRank(0);
	InitInput(hInstance, hWnd);

	// �t�B�[���h�̏�����
	{
		// �n�ʂ̏�����
		InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32, 32, 60.0f,60.0f);

		// �ǂ̏�����
		InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, 960.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), 1, 1, 1920.0f, 1920.0f);
		InitMeshWall(D3DXVECTOR3(-960.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), 1, 1, 1920.0f, 1920.0f);
		InitMeshWall(D3DXVECTOR3(960.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), 1, 1, 1920.0f, 1920.0f);
		InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -960.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), 1, 1, 1920.0f, 1920.0f);
		/*
		// ��(�����p�̔�����)
		InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, 875.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 1750.0f, 1750.0f);
		InitMeshWall(D3DXVECTOR3(-875.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 1750.0f, 1750.0f);
		InitMeshWall(D3DXVECTOR3(875.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 1750.0f, 1750.0f);
		InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -875.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 1, 1, 1750.0f, 1750.0f);
			*/

	}
	InitCamera();
	InitLight();
	InitShadow(0);
	//InitBulletPoint();
	InitPlayer();
	InitItem();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	if (g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	UninitPlayer();
	UninitInput();
	UninitTime();
	UninitTitle();
	UninitResult();
	UninitFade();
	UninitTutorial();
	UninitCountdown();
	UninitSound();
	UninitBullet();
	UninitEffect();
	UninitBulletprediction();
	UninitExplosion();
	UninitItem();
	UninitLifeTex();
	UninitBulletTex();
	UninitDamege();
	UninitStatus();
	UninitRank();
	//UninitBulletPoint();
	UninitMeshField();
	UninitMeshWall();
	UninitPlayer();
	UninitShadow();
	UninitCamera();
	UninitDebugProc();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͍X�V
	UpdateInput();
#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_F12) || IsButtonTriggered(0, BUTTON_SELECT)) stop++;
	if (GetKeyboardTrigger(DIK_F11) || IsButtonTriggered(0, BUTTON_L3))
	{
		MasterVolumeChange(1);
		InitGame();
		SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
#endif

	if (stop % 2 == 0)
	{
		// �J�����̍X�V����
		UpdateCamera();

		switch (g_nScene)
		{
		case SCENE_TITLE:
			UpdateTitle();
			break;
		case SCENE_TUTORIAL:
			UpdateTutorial();
			UpdateMeshField();
			UpdatePlayer();
			UpdateBullet();
			UpdateBulletprediction();
			UpdateEffect();
			UpdateExplosion();
			UpdateItem();
			UpdateShadow();
			CheakHit(0);
			UpdateBulletTex();
			UpdateDamege();
			UpdateStatus();
			break;
		case SCENE_GAMECOUNTDOWN:
			UpdateCountdown();
			AddCountdown(-1);
			break;
		case SCENE_GAME:
			// �n�ʏ����̍X�V
			UpdateMeshField();

			// �Ǐ����̍X�V
			UpdateMeshWall();

			// �L��������̍X�V����
			UpdatePlayer();
			UpdateBullet();
			UpdateBulletprediction();
			UpdateEffect();
			UpdateExplosion();
			UpdateItem();
			UpdateTime();
			
			// �e�̍X�V����
			UpdateShadow();

			//UpdateBulletPoint();

			// �����蔻��
			CheakHit(1);
			
			//2D�̍X�V����
			UpdateBulletTex();
			UpdateDamege();
			UpdateStatus();
			UpdateLifeTex();

			//���Ԑ����p
			//AddTime(-1);
			break;
		case SCENE_RESULT:
			UpdateResult();
			break;
		}
		// �t�F�[�h����
		UpdateFade();
	}	
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	//�l�l���̉�ʕ����ݒ�
	D3DVIEWPORT9 vp[]
	{
		{DWORD(0),DWORD(0),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H/2- SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2+ SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H/2- SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(0),DWORD(SCREEN_H / 2+ SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2+ SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
	};
	D3DVIEWPORT9 VpMaster{ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		// Direct3D�ɂ��`��̊J�n
		if (SUCCEEDED(g_pD3DDevice->BeginScene()))
		{
			// ��ʑJ��
			switch (g_nScene)
			{
			case SCENE_TITLE:
				DrawTitle();
				break;
			case SCENE_TUTORIAL:
				for (int i = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); i < vpCnt; i++)
				{
					g_pD3DDevice->SetViewport(&vp[i]);
					g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
					PLAYER_HONTAI *p = GetPlayerHoudai();
					if (p[i].use == false) continue;
					if (p[i].KiriSignal == true) g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
					else g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

					// �J�����̐ݒ�
					SetCamera(i);

					//DrawBullet();
					//DrawBulletPoint(i);

					//�t�B�[���h
					DrawMeshField();
					DrawMeshWall();

					// 3d���f���̕`�揈��
					DrawPlayer();
					DrawItem();

					DrawBulletprediction();
					DrawExplosion(i);
					DrawEffect(i);

					//�e
					DrawShadow();

					//2d��ʏ�
					DrawDamege();
					DrawStatus();
					DrawLifeTex();
					DrawBulletTex();
					DrawTutorial();
				}
				g_pD3DDevice->SetViewport(&VpMaster);
				break;
			case SCENE_GAMECOUNTDOWN:
				for (int i = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); i < vpCnt; i++)
				{
					g_pD3DDevice->SetViewport(&vp[i]);
					g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
					PLAYER_HONTAI *p = GetPlayerHoudai();
					if (p[i].use == true)
					{
						// �J�����̐ݒ�
						SetCamera(i);

						//DrawBullet();
						//DrawBulletPoint(i);

						//�t�B�[���h
						DrawMeshField();
						DrawMeshWall();

						// 3d���f���̕`�揈��
						DrawPlayer();
						DrawItem();

						DrawBulletprediction();
						DrawExplosion(i);
						DrawEffect(i);

						//�e
						DrawShadow();

						//2d��ʏ�
						DrawDamege();
						DrawStatus();
						DrawLifeTex();
						DrawBulletTex();
					}
				}
				g_pD3DDevice->SetViewport(&VpMaster);
				//DrawTime();
				DrawCountdown();
				break;
			case SCENE_GAME:
				for (int i = 0, vpCnt = sizeof(vp) / sizeof(vp[0]); i < vpCnt; i++)
				{
					g_pD3DDevice->SetViewport(&vp[i]);
					g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
					PLAYER_HONTAI *p = GetPlayerHoudai();

					if (p[i].use == true)
					{

						if (p[i].KiriSignal == true) g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
						else g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

						// �J�����̐ݒ�
						SetCamera(i);

						//DrawBullet();
						//DrawBulletPoint(i);

						//�t�B�[���h
						DrawMeshField();
						DrawMeshWall();

						// 3d���f���̕`�揈��
						DrawPlayer();
						DrawItem();

						DrawBulletprediction();
						DrawExplosion(i);
						DrawEffect(i);
						//�e
						DrawShadow();

						//2d��ʏ�
						DrawDamege();
						DrawStatus();
						DrawLifeTex();
						DrawBulletTex();
					}
					else
					{
						g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF

						// �J�����̐ݒ�
						SetCamera(i);

						//�e
						DrawShadow();


						// 3d���f���̕`�揈��
						//DrawBullet();
						//DrawBulletPoint(i);

						DrawPlayer();
						DrawItem();

						//�t�B�[���h
						DrawMeshField();
						DrawMeshWall();

						DrawEffect(i);
						DrawExplosion(i);


						//2d��ʏ�
						DrawDamege();
						DrawStatus();
						DrawLifeTex();
						DrawBulletTex();
						DrawRank();
					}
				}

				g_pD3DDevice->SetViewport(&VpMaster);
				//DrawTime();
				break;
			case SCENE_RESULT:
				DrawResult();
				break;
			}
			// �t�F�[�h�`��
			DrawFade();

			// �f�o�b�O�\��
#ifdef _DEBUG
			DrawTextType();
			DrawDebugProc();
#endif
		}
		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O���ɕ\�������镶�����g_text�ɏ�������
//=============================================================================
void SetText(char *moji)
{
	strcpy(g_text, moji);
}

//=============================================================================
// �f�o�b�O���ɂǂ̐���ɂȂ������Q�[����ʂŔ��f���镶���`��֐�
//=============================================================================
void DrawTextType(void)
{
	TCHAR str[256];
	RECT rect = { 10, 90, SCREEN_W, SCREEN_H };
	wsprintf(str, _T("%s\n"), g_text);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
}

//=============================================================================
// FPS�\������
//=============================================================================
void DrawFPS(void)
{
	TCHAR str[256];
	int GetPadData(int no);

	//�����̃Z�b�g�������邱�ƂŃf�o�b�O���̕\�����ڂ𑝂₹��
	{
		RECT rect = { 0, 0, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("FPS:%d\n"), g_nCountFPS);
		// �e�L�X�g�`��
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));//ARGB�ŐF�ς����
	}

	{
		RECT rect = { 0, 50, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("%d %d %d %d %d %d %d %d \n"), GetPadData(0), GetPadData(1), GetPadData(2), GetPadData(3), GetPadData(4), GetPadData(5), GetPadData(6), GetPadData(7));
		// �e�L�X�g�`��
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}

	{
		RECT rect = { 0, 70, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("%d %d %d %d %d %d %d %d \n"), GetPadData(8), GetPadData(9), GetPadData(10), GetPadData(11), GetPadData(12), GetPadData(13), GetPadData(14), GetPadData(15));
		// �e�L�X�g�`��
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}

}
#endif

//=============================================================================
// Scene�J��
//=============================================================================
void SetScene(int Scene)
{
	g_nScene = Scene;
}

//=============================================================================
// GetScene�J��
//=============================================================================
int GetScene(void)
{
	return g_nScene;
}

//=============================================================================
// �Q�[�����[�v���̍ď�������������
// �߂�l�F����
//=============================================================================
void InitGame(void)
{
	//�ď�����
	InitTime(1);
	InitTutorial(1);
	InitCountdown(1);
	InitBullet(1);
	InitEffect(1);
	InitExplosion(1);
	InitItem();
	InitLifeTex(1);
	InitBulletTex(1);
	InitDamege(1);
	InitStatus(1);
	InitRank(1);
	InitCamera();
	InitLight();
	InitShadow(1);
	ReInitPlayer();
}

//=============================================================================
// D3DXVECTOR3����float�ɕϊ��B�ړ������̒l���v�Z����֐�
// �߂�l�F�ړ������̒l
//=============================================================================
float SpdCal(D3DXVECTOR3 move)
{
	float spd;
	float crossvec;
	crossvec = float(sqrt((move.x*move.x) + (move.z*move.z)));
	return spd = float(sqrt((move.y*move.y) + (crossvec*crossvec)));
}

//=============================================================================
// ���C�L���X�g�֐�
// �߂�l�Ftrue�Ȃ烌�C�ƃ|���S���̓����_���|�C���^�[�ɐ��l����
//=============================================================================
bool RayCast(D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 *ReturnVtx)
{
	//if(rayS >vtx)
	//�O����������O�O�O����
	D3DXVECTOR3 cr1, cr2, cr3, cr4;
	float do1, do2, n1, n2, n3;

	//step1	���C���ђʂ��Ă邩�ǂ�������
	D3DXVec3Cross(&cr1, &(vtx1 - vtx0), &(vtx2 - vtx0));
	float cr1Nor = D3DXVec3Length(&cr1);
	do1 = D3DXVec3Dot(&(cr1), &(rayS - vtx0));
	do2 = D3DXVec3Dot(&(cr1), &(rayG - vtx0));
	if (do1*do2 > 0) return false;
	
	//step2	����������߂�
	//d1,d2=1 raySY��rayGY��_�܂ł̋���
	// n = a = d1/(d1+d2) 
	// m = 1-a �g����p0�����_�܂ł̃x�N�g�����o����
	// a:1-a = d1:d2
	D3DXVECTOR3 v1 = (rayS - vtx0);
	D3DXVECTOR3 v2 = (rayG - vtx0);
	float d1 = fabsf(D3DXVec3Dot(&(cr1), &(v1)));
	float d2 = fabsf(D3DXVec3Dot(&(cr1), &(v2)));
	d1 /= cr1Nor;
	d2 /= cr1Nor;

	float naibunhi = d1 / (d1 + d2);
	D3DXVECTOR3 v3 = ((1 - naibunhi)*v1) + (naibunhi * v2);
	D3DXVECTOR3 kouten = vtx0 + v3;

	//step3	�|���S���̓����ɓ����Ă邩�̔���
	D3DXVec3Cross(&cr2, &(vtx0 - vtx1), &(kouten - vtx1));
	D3DXVec3Cross(&cr3, &(vtx1 - vtx2), &(kouten - vtx2));
	D3DXVec3Cross(&cr4, &(vtx2 - vtx0), &(kouten - vtx0));
	n1 = D3DXVec3Dot(&cr2, &cr3);
	n2 = D3DXVec3Dot(&cr3, &cr4);
	n3 = D3DXVec3Dot(&cr2, &cr4);

	if (n1 >= 0 && n2 >= 0 && n3 >= 0)
	{
		*ReturnVtx = kouten;
		return true;
	}
	else
	{
		return false;
	}
}

