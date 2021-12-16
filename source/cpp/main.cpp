/**
* @file main.cpp
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#include "../h/main.h"
#include "../h/Debug/debugproc.h"
#include "../h/Other/input.h"
#include "../h/Object/Camera/camera.h"
#include "../h/Object/Player/player.h"
#include "../h/Map/field.h"
#include "../h/Map/wall.h"
#include "../h/Map/sky.h"
#include "../h/Object/Shadow/shadow.h"
#include "../h/Object/Title/title.h"
#include "../h/Object/Result/result.h"
#include "../h/Object/Scene/Scene.h"
#include "../h/Object/Tutorial/tutorial.h"
#include "../h/Effect/effect.h"
#include "../h/Object/Bullet/bullet.h"
#include "../h/Effect/explosion.h"
#include "../h/Object/Rank/rank.h"
#include "../h/Object/CountDown/countdown.h"
#include "../h/Object/Item/item.h"
#include "../h/Effect/damege.h"
#include "../h/Object/Player/Status/status.h"
#include "../h/Other/sound.h"
#include "../h/Object/Bullet/bulletprediction.h"
#include "../h/Object/Bullet/bulletgauge.h"
#include "../h/Object/Player/VitalGauge/vitalgauge.h"
#include "../h/Object/ObjectClass/Instance/instance.h"
#include "../h/net/sock.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			//!< �E�C���h�E�̃N���X��
#define WINDOW_NAME		_T("NiceShot�I")	//!< �E�C���h�E�̃L���v�V������

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
HWND				edit;						//!< �n���h��
MSG					msg;						//!< �A�v���s�[�N���b�Z�[�W
bool				EndGame = false;			//!< �Q�[���I���t���O

#ifdef _DEBUG
static LPD3DXFONT	g_pD3DXFont = NULL;			//!< �t�H���g�ւ̃|�C���^
char				g_text[256] = { 0 };		//!< �\��������e�L�X�g
int					g_nCountFPS;				//!< FPS�J�E���^
char				g_textSo[256] = { "aa" };	//!< �\��������e�L�X�g
DWORD				dwFrameCount;				//!< ���Ԍv���p
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));
	
	//UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	//UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

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

	//���Ӂ@���C���ŏ������ޕϐ��ƃ}���`�ϐ��@�Ԃ���Ƃ�΂��@�����̋��L�@�^�C�~���O�Ԃ���Ȃ��悤�ɂ���
	//��������Ƃ����֐���ł���������@����������\���̂��鎞�ɃA�N�Z�X�����Ƃ�
	//����������ϐ������M����҂�(�������肵���֐��̃A�N�Z�X����)�@�V���N�����C�Y?�@

	if (FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	//-----------------------------------------------------�I�u�W�F�N�g����
	GAME_OBJECT_INSTANCE ObjectInstance;
	ObjectInstance.Generate();

	//-----------------------------------------------------�I�u�W�F�N�g�A�h���X�擾
	ObjectInstance.Addressor(&ObjectInstance);

	//-----------------------------------------------------�I�u�W�F�N�g������
	ObjectInstance.Init();

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	//�}���`�X���b�h�J�n
	std::thread SubThread([&ObjectInstance]() {ObjectInstance.MySubThreadFunc(); });
	
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
	while (1)
	{
		if (msg.message == WM_QUIT)
		{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
			break;
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (EndGame == true)
			{
				break;
			}
			if (msg.message == WM_QUIT)
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
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				// FPS�\��
				PrintDebugProc("FPS: %d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				ObjectInstance.Update();

				// �`�揈��
				ObjectInstance.Draw();

				dwFrameCount++;
			}
		}
	}

	//-----------------------------------------------------�I�u�W�F�N�g�I��
	//�V�[�P���X�I���t���O
	EndGame = true;

	ObjectInstance.GetMySocket()->MultThreadFlagFunc(false);
	ObjectInstance.GetMySocket()->GameSceneFlagFunc(false);

	//�X���b�h�j��
	SubThread.join();

	//delete
	ObjectInstance.Delete();

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
			//DestroyWindow(hWnd);
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
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
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
	InitCamera();

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
// �f�o�b�O���ɂǂ̐���ɂȂ������Q�[����ʂŔ��f���镶���`��֐�
//=============================================================================
void DrawTextType(void)
{
	TCHAR str[256];
	RECT rect = { 10, 90, SCREEN_W, SCREEN_H };
	wsprintf(str, _T("%s\n"), g_text);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0xff));
}

//=============================================================================
// �ǂ̐���ɂȂ������Q�[����ʂŔ��f���镶���`��֐�
//=============================================================================
void DrawTextTypeSo(void)
{
	TCHAR str[256];
	RECT rect = { 10, 90, SCREEN_W, SCREEN_H };
	wsprintf(str, _T("%s\n"), g_textSo);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0xff));
}

//=============================================================================
// �\�������镶�����g_text�ɏ�������
//=============================================================================
void SetTextSo(char *moji)
{
	strcpy(g_textSo, moji);
}

//=============================================================================
// �f�o�b�O���ɕ\�������镶�����g_text�ɏ�������
//=============================================================================
void SetText(char *moji)
{
	strcpy(g_text, moji);
}

//=============================================================================
// FPS�\������
//=============================================================================
void DrawFPS(void)
{
	TCHAR str[256];

	//�����̃Z�b�g�������邱�ƂŃf�o�b�O���̕\�����ڂ𑝂₹��
	{
		RECT rect = { 0, 0, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("FPS:%d\n"), g_nCountFPS);
		// �e�L�X�g�`��
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));//ARGB�ŐF�ς����
	}

	{
		RECT rect = { 0, 50, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("%d %d %d %d %d %d %d %d \n"), GetPadData(PLAYER01,0), GetPadData(PLAYER01, 1), GetPadData(PLAYER01, 2), GetPadData(PLAYER01, 3), 
			GetPadData(PLAYER01, 4), GetPadData(PLAYER01, 5), GetPadData(PLAYER01, 6), GetPadData(PLAYER01, 7));
		// �e�L�X�g�`��
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}

	{
		RECT rect = { 0, 70, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("%d %d %d %d %d %d %d %d \n"), GetPadData(PLAYER01, 8), GetPadData(PLAYER01, 9), GetPadData(PLAYER01, 10), GetPadData(PLAYER01, 11), 
			GetPadData(PLAYER01, 12), GetPadData(PLAYER01, 13), GetPadData(PLAYER01, 14), GetPadData(PLAYER01, 15));
		// �e�L�X�g�`��
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}

}
#endif

//=============================================================================
// ���에���@���`�����@
//=============================================================================
int MyRandFunc(int *X, int Max)
{
	//X�̃o�b�t�@
	int buff = *X;

	//�C�Ӑ�AB
	int A = 1664525;
	int B = 1013904223;
	int M = 2147483647;
	//���`�����@�Ōv�Z�@���ʂ����̃V�[�h�l�Ƃ���
	int ret = abs((A*buff + B) % M);
	*X = ret;
	//���r���[�Ȑ��̂��܂���g�p����@%(Max)�͏���l�ł�����
	ret = abs(ret % Max);
	return ret;
}

//=============================================================================
// �s�[�N���b�Z�[�W�̃Z�b�g �I������Ƃ���Quit��������
//=============================================================================
void SetMsg(UINT Setmsg)
{
	msg.message = Setmsg;
}

//=============================================================================
// �s�[�N���b�Z�[�W�̃Z�b�g �I������Ƃ���Quit��������
//=============================================================================
MSG GetMsg(void)
{
	return msg;
}

//=============================================================================
// �Q�[���I���t���O�Z�b�^�[
//=============================================================================
void SetEndGame(bool flag)
{
	EndGame = flag;
}

//=============================================================================
// �Q�[���I���t���O�Q�b�^�[
//=============================================================================
bool GetEndGame(void)
{
	return EndGame;
}
