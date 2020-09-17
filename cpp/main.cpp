/**
* @file main.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../h/main.h"
#include "../h/other/debugproc.h"
#include "../h/other/input.h"
#include "../h/object/camera.h"
#include "../h/other/light.h"
#include "../h/object/player.h"
#include "../h/map/field.h"
#include "../h/map/wall.h"
#include "../h/map/sky.h"
#include "../h/other/shadow.h"
#include "../h/collision.h"
#include "../h/scene/time.h"
#include "../h/scene/title.h"
#include "../h/scene/result.h"
#include "../h/scene/fade.h"
#include "../h/scene/tutorial.h"
#include "../h/effect/effect.h"
#include "../h/object/bullet/bullet.h"
#include "../h/object/bullet/bulletpoint.h"
#include "../h/effect/explosion.h"
#include "../h/scene/rank.h"
#include "../h/scene/countdown.h"
#include "../h/object/item.h"
#include "../h/object/life.h"
#include "../h/object/bullet/bullettex.h"
#include "../h/effect/damege.h"
#include "../h/object/status.h"
#include "../h/other/sound.h"
#include "../h/object/bullet/bulletprediction.h"
#include "../h/object/bullet/bulletgauge.h"
#include "../h/object/vitalgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"				//!< ウインドウのクラス名
#define WINDOW_NAME		_T("NiceShot！")	//!< ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/**
* @brief 初期化関数
* @param[in]　HWND hWnd, BOOL bWindow
* @return HRESULT
* @details 起動時の初期化
*/
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

/**
* @brief メモリ開放関数
* @details ゲーム終了時にメモリ開放する
*/
void Uninit(void);

/**
* @brief アップデート関数
* @details オブジェクトを更新する
*/
void Update(void);

/**
* @brief 描画関数
* @details オブジェクトを描画する
*/
void Draw(void);

#ifdef _DEBUG
/**
* @brief FPS表示関数
* @details デバッグ実行時FPSを表示する
*/
void DrawFPS(void);
#endif

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;				//!< Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		//!< Deviceオブジェクト(描画に必要)
int					g_nScene = SCENE_TITLE;		//!< ステージ番号
int					stop = 0;					//!< デバッグ時の一時停止用変数

#ifdef _DEBUG
static LPD3DXFONT	g_pD3DXFont = NULL;			//!< フォントへのポインタ
int					g_nCountFPS;				//!< FPSカウンタ
char				g_text[256] = { 0 };		//!< 表示させるテキスト
DWORD				dwFrameCount;				//!< 時間計測用
#endif


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));

	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

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
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
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

	// DirectXの初期化(ウィンドウを作成してから行う)
	bool mode;
	//戻り値がint型で戻ってくる
	//ここのMessageBoxを呼ぶとwindowが出せる。
	//引数1はNULL。引数2は本文。
	//引数3はwindou名。
	//引数4はMB_YESNOCANCELこれでイエスノーキャンセルの三択が出せる。ここはいろんな種類があるから自分で調べるMB_OKでOKだけ
	//引数4の後ろに「| MB_ICONQUESTION」でアイコンを出すことができる。
	int id = MessageBox(NULL, "Windowモードでプレイしますか？", "起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);

	//押された選択肢でスイッチ分岐
	switch (id)
	{
	case IDYES:		// YesならWindowモードで起動
		mode = true;
		break;
	case IDNO:		// Noならフルスクリーンモードで起動
		mode = false;
		break;
	case IDCANCEL:	// CANCELなら終了
	default:
		return -1;
		break;
	}

	if (FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	PlaySound(SOUND_LABEL_BGM_title01);

	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
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
				// FPS表示
				PrintDebugProc("FPS: %d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_W;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_H;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;							// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;			// インターバル
	}
	else
	{// フルスクリーンモード

		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;							// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// インターバル

	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
// 情報表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);
#endif

	// FOGの設定
	FLOAT StartPos = 50;  //開始位置
	FLOAT EndPos = 300; //終了位置
	g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);								//フォグ：OFF
	g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	//カラー
	g_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);						//頂点モード
	g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);					//テーブルモード
	g_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));					//開始位置
	g_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));						//終了位置	

	//初期化処理
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
	InitVitalGauge();
	InitBulletTex(0);
	InitBulletGauge();
	InitDamege(0);
	InitStatus(0);
	InitRank(0);
	InitInput(hInstance, hWnd);

	// フィールドの初期化
	{
		// 空の初期化
		InitMeshSky(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 8, 4000.0f, true);

		// 地面の初期化
		InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 32, 32, 250.0f,250.0f);

		// 壁の初期化
		InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_INIT_POSZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, WALL_SIZE_X, WALL_SIZE_Y);
		InitMeshWall(D3DXVECTOR3(-WALL_INIT_POSX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, WALL_SIZE_X, WALL_SIZE_Y);
		InitMeshWall(D3DXVECTOR3(WALL_INIT_POSX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, WALL_SIZE_X, WALL_SIZE_Y);
		InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -WALL_INIT_POSZ), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, WALL_SIZE_X, WALL_SIZE_Y);

		/*
		// 壁(裏側用の半透明)
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
// 終了処理
//=============================================================================
void Uninit(void)
{
	if (g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
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
	UninitVitalGauge();
	UninitBulletTex();
	UninitBulletGauge();
	UninitDamege();
	UninitStatus();
	UninitRank();
	//UninitBulletPoint();
	UninitMeshSky();
	UninitMeshField();
	UninitMeshWall();
	UninitPlayer();
	UninitShadow();
	UninitCamera();
	UninitDebugProc();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力更新
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
		// カメラの更新処理
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
			//UpdateBulletTex();
			UpdateBulletGauge();
			UpdateDamege();
			UpdateStatus();
			break;
		case SCENE_GAMECOUNTDOWN:
			UpdateCountdown();
			AddCountdown(-1);
			break;
		case SCENE_GAME:
			// 地面処理の更新
			UpdateMeshField();

			// 壁処理の更新
			UpdateMeshWall();

			// キャラ周りの更新処理
			UpdatePlayer();
			UpdateBullet();
			UpdateBulletprediction();
			UpdateEffect();
			UpdateExplosion();
			UpdateItem();
			UpdateTime();
			
			// 影の更新処理
			UpdateShadow();

			//UpdateBulletPoint();

			// 当たり判定
			CheakHit(1);
			
			//2Dの更新処理
			//UpdateBulletTex();
			UpdateBulletGauge();
			UpdateDamege();
			UpdateStatus();
			//UpdateLifeTex();
			UpdateVitalGauge();

			//時間制限用
			//AddTime(-1);
			break;
		case SCENE_RESULT:
			UpdateResult();
			break;
		}
		// フェード処理
		UpdateFade();
	}	
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	//四人分の画面分割設定
	D3DVIEWPORT9 vp[]
	{
		{DWORD(0),DWORD(0),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H/2- SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2+ SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H/2- SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(0),DWORD(SCREEN_H / 2+ SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
		{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2+ SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W/2- SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f},
	};
	D3DVIEWPORT9 VpMaster{ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	// バックバッファ＆Ｚバッファのクリア
		g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		// Direct3Dによる描画の開始
		if (SUCCEEDED(g_pD3DDevice->BeginScene()))
		{
			// 画面遷移
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
					if (p[i].KiriSignal == true) g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
					else g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

					// カメラの設定
					SetCamera(i);

					//DrawBullet();
					//DrawBulletPoint(i);

					//フィールド
					DrawMeshSky();
					DrawMeshField();
					DrawMeshWall();

					// 3dモデルの描画処理
					DrawPlayer();
					DrawItem();

					DrawBulletprediction(i);
					DrawExplosion(i);
					DrawEffect(i);

					//影
					DrawShadow();

					//2d画面上
					DrawDamege();
					DrawStatus();
					//DrawLifeTex();
					DrawVitalGauge();
					//DrawBulletTex();
					DrawBulletGauge();
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
						// カメラの設定
						SetCamera(i);

						//DrawBullet();
						//DrawBulletPoint(i);

						//フィールド
						DrawMeshSky();
						DrawMeshField();
						DrawMeshWall();

						// 3dモデルの描画処理
						DrawPlayer();
						DrawItem();

						DrawBulletprediction(i);
						DrawExplosion(i);
						DrawEffect(i);

						//影
						DrawShadow();

						//2d画面上
						DrawDamege();
						DrawStatus();
						//DrawLifeTex();
						DrawVitalGauge();
						//DrawBulletTex();
						DrawBulletGauge();
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

						if (p[i].KiriSignal == true) g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
						else g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

						// カメラの設定
						SetCamera(i);

						//DrawBullet();
						//DrawBulletPoint(i);

						//フィールド
						DrawMeshSky();
						DrawMeshField();
						DrawMeshWall();

						// 3dモデルの描画処理
						DrawPlayer();
						DrawItem();

						DrawBulletprediction(i);
						DrawExplosion(i);
						DrawEffect(i);
						//影
						DrawShadow();

						//2d画面上
						DrawDamege();
						DrawStatus();
						//DrawLifeTex();
						DrawVitalGauge();
						//DrawBulletTex();
						DrawBulletGauge();
					}
					else
					{
						g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF

						// カメラの設定
						SetCamera(i);

						//影
						DrawShadow();


						// 3dモデルの描画処理
						//DrawBullet();
						//DrawBulletPoint(i);

						DrawPlayer();
						DrawItem();

						//フィールド
						DrawMeshSky();
						DrawMeshField();
						DrawMeshWall();

						DrawEffect(i);
						DrawExplosion(i);


						//2d画面上
						DrawDamege();
						DrawStatus();
						//DrawLifeTex();
						DrawVitalGauge();
						//DrawBulletTex();
						DrawBulletGauge();
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
			// フェード描画
			DrawFade();

			// デバッグ表示
#ifdef _DEBUG
			DrawTextType();
			DrawDebugProc();
#endif
		}
		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef _DEBUG
//=============================================================================
// デバッグ時に表示させる文字列をg_textに書き込む
//=============================================================================
void SetText(char *moji)
{
	strcpy(g_text, moji);
}

//=============================================================================
// デバッグ時にどの制御になったかゲーム画面で判断する文字描画関数
//=============================================================================
void DrawTextType(void)
{
	TCHAR str[256];
	RECT rect = { 10, 90, SCREEN_W, SCREEN_H };
	wsprintf(str, _T("%s\n"), g_text);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
}

//=============================================================================
// FPS表示処理
//=============================================================================
void DrawFPS(void)
{
	TCHAR str[256];

	//ここのセットを加えることでデバッグ時の表示項目を増やせる
	{
		RECT rect = { 0, 0, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("FPS:%d\n"), g_nCountFPS);
		// テキスト描画
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));//ARGBで色変えれる
	}

	{
		RECT rect = { 0, 50, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("%d %d %d %d %d %d %d %d \n"), GetPadData(PLAYER01,0), GetPadData(PLAYER01, 1), GetPadData(PLAYER01, 2), GetPadData(PLAYER01, 3), 
			GetPadData(PLAYER01, 4), GetPadData(PLAYER01, 5), GetPadData(PLAYER01, 6), GetPadData(PLAYER01, 7));
		// テキスト描画
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}

	{
		RECT rect = { 0, 70, SCREEN_W, SCREEN_H };
		wsprintf(str, _T("%d %d %d %d %d %d %d %d \n"), GetPadData(PLAYER01, 8), GetPadData(PLAYER01, 9), GetPadData(PLAYER01, 10), GetPadData(PLAYER01, 11), 
			GetPadData(PLAYER01, 12), GetPadData(PLAYER01, 13), GetPadData(PLAYER01, 14), GetPadData(PLAYER01, 15));
		// テキスト描画
		g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0x00, 0x00));
	}

}
#endif

//=============================================================================
// Scene遷移
//=============================================================================
void SetScene(int Scene)
{
	g_nScene = Scene;
}

//=============================================================================
// GetScene遷移
//=============================================================================
int GetScene(void)
{
	return g_nScene;
}

//=============================================================================
// ゲームループ時の再初期化処理処理
// 戻り値：無し
//=============================================================================
void InitGame(void)
{
	//再初期化
	InitTime(1);
	InitTutorial(1);
	InitCountdown(1);
	InitBullet(1);
	InitEffect(1);
	InitExplosion(1);
	InitLifeTex(1);
	InitBulletTex(1);
	InitDamege(1);
	InitStatus(1);
	InitRank(1);
	InitCamera();
	InitLight();
	InitShadow(1);
	ReInitTitle();
	ReInitPlayer();
	ReInitItem();
	ReInitVitalGauge();
	ReInitBulletGauge();
}

