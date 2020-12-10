/**
* @file title.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/net/sock.h"
#include "../../h/scene/title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE_LOGO				"../data/TEXTURE/other/Title.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_BG				"../data/TEXTURE/other/TItle_Background.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_PRESS				"../data/TEXTURE/UI/UI_Press.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_TUTORIAL	"../data/TEXTURE/UI/UI_Tutorial.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_GAME_LOCAL	"../data/TEXTURE/UI/UI_LocalBattle.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_GAME_NET	"../data/TEXTURE/UI/UI_NetorkBattle.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_GAME_QUIT	"../data/TEXTURE/UI/UI_Quit.png"	// 読み込むテクスチャファイル名


#define	TITLE_BG_WIDTH						(SCREEN_W/2)				// タイトルBGの幅
#define	TITLE_BG_HEIGHT						(SCREEN_H/2)				// タイトルBGの高さ
#define	TITLE_BG_POS_X						(SCREEN_CENTER_X)			// タイトルBGの位置(X座標)
#define	TITLE_BG_POS_Y						(SCREEN_CENTER_Y)			// タイトルBGの位置(Y座標)

#define	TITLE_LOGO_POS_X					(SCREEN_CENTER_X)			// タイトルロゴの位置(X座標)
#define	TITLE_LOGO_POS_Y					(SCREEN_CENTER_Y-200.0f)	// タイトルロゴの位置(Y座標)
#define	TITLE_LOGO_WIDTH					(400.0f)					// タイトルロゴの幅
#define	TITLE_LOGO_HEIGHT					(250.0f)					// タイトルロゴの高さ

#define	TITLE_PRESS_LOGO_POS_X				(SCREEN_CENTER_X)			// PRESSロゴの位置(X座標)
#define	TITLE_PRESS_LOGO_POS_Y				(SCREEN_CENTER_Y+200.0f)	// PRESSロゴの位置(Y座標)
#define	TITLE_PRESS_LOGO_WIDTH				(200.0f)					// PRESSロゴの幅
#define	TITLE_PRESS_LOGO_HEIGHT				(50.0f)					// PRESSロゴの高さ

#define	TITLE_SELECT_POS_X					(SCREEN_CENTER_X)			// セレクトテクスチャの位置(X座標)
#define	TITLE_SELECT_POS_Y					(SCREEN_CENTER_Y)			// セレクトテクスチャの位置(Y座標)
#define	TITLE_SELECT_WIDTH					(150.0f)					// セレクトテクスチャの幅
#define	TITLE_SELECT_HEIGHT					(100.0f)					// セレクトテクスチャの高さ
#define	TITLE_SELECT_OFFSET_X				(200.0f)					// オフセット値
#define	TITLE_SELECT_OFFSET_Y				(200.0f)					// オフセット値

#define	COUNT_APPERA_START					(10)						// スタートボタン出現までの時間
#define	INTERVAL_DISP_START					(30)						// スタートボタン点滅の時間
#define	COUNT_WAIT_DEMO						(60 * 5)					// デモまでの待ち時間


//=============================================================================
// 初期化処理
//=============================================================================
void TITLE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexTitle();

	// テクスチャの読み込み
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
// 再初期化処理
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
// 終了処理
//=============================================================================
void TITLE::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void TITLE::Update(GAME_OBJECT* obj, FADE *fade)
{
	switch (this[0].HierarchyNum)
	{
		//タイトルアニメ階層
	case TITLE_HIERARCHY_NUM_ANIM:
	{
		//シーンセレクト階層へ切り替え
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
	//シーンセレクト階層
	case TITLE_HIERARCHY_NUM_SELECT:
	{
		//チュートリアル対決階層へ切り替え
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
		//カーソル移動
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

		//カーソルに合わせているやつを光らせる
		//チュートリアル
		if (this[0].SceneNumX == 0 && this[0].SceneNumY == 0)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.4f);
			//シーンをチュートリアルへ
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				fade->SetFade(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
			}
		}
		//ローカル対戦
		else if (this[0].SceneNumX == 1 && this[0].SceneNumY == 0)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.4f);
			//シーンをゲームへ
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				fade->SetFade(FADE_OUT, SCENE_GAMECOUNTDOWN, SOUND_LABEL_BGM_normal01);
			}
		}
		//ネット対戦
		else if (this[0].SceneNumX == 0 && this[0].SceneNumY == 1)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 0.4f);
			//シーンをゲームへ
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				fade->SetFade(FADE_OUT, SCENE_NETMATCH, SOUND_LABEL_BGM_select01);
			}
		}
		//アプリ終了へ
		else if (this[0].SceneNumX == 1 && this[0].SceneNumY == 1)
		{
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.4f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_QUIT, 1.0f);
			//シーンをゲームへ
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
// 描画処理
//=============================================================================
void TITLE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_OBJECT_NAME_MAX; CntTitleSelectScene++)
	{
		if (this[CntTitleSelectScene].bDisp == true)
		{

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, this[CntTitleSelectScene].tex2DVB.pD3DTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT TITLE::MakeVertexTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//BG生成
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&this[0].tex2DVB.pD3DVtxBuff,		// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			this[0].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_H, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロックする
			this[0].tex2DVB.pD3DVtxBuff->Unlock();
		}
	}

	//タイトルロゴ生成
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&this[1].tex2DVB.pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			this[1].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X - TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y - TITLE_LOGO_HEIGHT, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y - TITLE_LOGO_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X - TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロックする
			this[1].tex2DVB.pD3DVtxBuff->Unlock();
		}
	}

	//PRESS生成
	{

		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&this[2].tex2DVB.pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			this[2].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X - TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y - TITLE_PRESS_LOGO_HEIGHT, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X + TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y - TITLE_PRESS_LOGO_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X - TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y + TITLE_PRESS_LOGO_HEIGHT, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X + TITLE_PRESS_LOGO_WIDTH, TITLE_PRESS_LOGO_POS_Y + TITLE_PRESS_LOGO_HEIGHT, 0.0f);

			// テクスチャのパースペクティブコレクト用
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロックする
			this[2].tex2DVB.pD3DVtxBuff->Unlock();
		}
	}

	//セレクトUI生成
	{
		for (int CntTitleSelectScene = TITLE_OBJECT_NAME_TUTO; CntTitleSelectScene < TITLE_OBJECT_NAME_MAX; CntTitleSelectScene++)
		{

			// オブジェクトの頂点バッファを生成
			if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * POLYGON_2D_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
				D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
				FVF_VERTEX_2D,				// 使用する頂点フォーマット
				D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
				&this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
				NULL)))						// NULLに設定
			{
				return E_FAIL;
			}

			{//頂点バッファの中身を埋める
				VERTEX_2D *pVtx;

				// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
				this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
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
				// テクスチャのパースペクティブコレクト用
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				// 頂点データをアンロックする
				this[CntTitleSelectScene].tex2DVB.pD3DVtxBuff->Unlock();
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 頂点色変更
//=============================================================================
void TITLE::SetColorTitle(int CntTitle, float alpha)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff = this[CntTitle].tex2DVB.pD3DVtxBuff;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

		// 頂点データをアンロックする
		pD3DVtxBuff->Unlock();
	}

}

