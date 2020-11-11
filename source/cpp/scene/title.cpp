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
#define	TEXTURE_TITLE_BG				"../data/TEXTURE/title.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START				"../data/TEXTURE/press_enter.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_TUTORIAL	"../data/TEXTURE/rensyurogo.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_GAME		"../data/TEXTURE/taisenrogo.jpg"	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_GAME_LOCAL	"../data/TEXTURE/local.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_SELECT_GAME_NET	"../data/TEXTURE/network.png"	// 読み込むテクスチャファイル名


#define	TITLE_BG_WIDTH		(SCREEN_W/2)						// タイトルBGの幅
#define	TITLE_BG_HEIGHT		(SCREEN_H/2)						// タイトルBGの高さ
#define	TITLE_BG_POS_X		(SCREEN_CENTER_X)					// タイトルBGの位置(X座標)
#define	TITLE_BG_POS_Y		(SCREEN_CENTER_Y)					// タイトルBGの位置(Y座標)

#define	TITLE_START_LOGO_POS_X				(TITLE_BG_POS_X)			// スタートロゴの位置(X座標)
#define	TITLE_START_LOGO_POS_Y				(TITLE_BG_POS_Y+200)		// スタートロゴの位置(Y座標)
#define	TITLE_START_LOGO_WIDTH				(100)						// スタートロゴの幅
#define	TITLE_START_LOGO_HEIGHT				(100)						// スタートロゴの高さ

#define	TITLE_SELECT_POS_X				(TITLE_BG_POS_X+200.0f)		// セレクトテクスチャの位置(X座標)
#define	TITLE_SELECT_POS_Y				(TITLE_BG_POS_Y-100.0f)		// セレクトテクスチャの位置(Y座標)
#define	TITLE_SELECT_WIDTH				(150)						// セレクトテクスチャの幅
#define	TITLE_SELECT_HEIGHT				(150)						// セレクトテクスチャの高さ

#define	COUNT_APPERA_START		(10)						// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(30)						// スタートボタン点滅の時間
#define	COUNT_WAIT_DEMO			(60 * 5)					// デモまでの待ち時間


//=============================================================================
// 初期化処理
//=============================================================================
void TITLE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexTitle();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_BG, &this[0].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_LOGO_START, &this[1].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_TUTORIAL, &this[2].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME, &this[3].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME_LOCAL, &this[4].tex2DVB.pD3DTexture);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE_SELECT_GAME_NET, &this[5].tex2DVB.pD3DTexture);

}

//=============================================================================
// 再初期化処理
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
// 終了処理
//=============================================================================
void TITLE::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void TITLE::Update(FADE *fade)
{
		switch (this[0].HierarchyNum)
		{
		//タイトル階層
		case TITLE_HIERARCHY_NUM_TITLE:
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TITLELOGO, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_START, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
			//チュートリアル対決階層へ切り替え
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				this[0].HierarchyNum = TITLE_HIERARCHY_NUM_TUTO_BATTLE;
				this[TITLE_OBJECT_NAME_TUTO].bDisp = true;
				this[TITLE_OBJECT_NAME_BATTLE].bDisp = true;
				this[0].SceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
			//切り替え
			else if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SOUND_LABEL_SE_attack02);
			}
			break;
			//チュートリアル対決階層
		case TITLE_HIERARCHY_NUM_TUTO_BATTLE:
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TITLELOGO, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_START, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.2f);
			//タイトル階層へ切り替え
			if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				this[0].HierarchyNum = TITLE_HIERARCHY_NUM_TITLE;
				this[TITLE_OBJECT_NAME_TUTO].bDisp = false;
				this[TITLE_OBJECT_NAME_BATTLE].bDisp = false;

			}
			//上入力でチュートリアルにカーソル合わせる
			else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_ANALOG_L_UP))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_TUTORIAL;
			}
			//下入力で対決にカーソル合わせる
			else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_ANALOG_L_DOWN))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_BATTLE;
			}

			if (this[0].SceneNum == TITLE_SELECT_SCENE_TUTORIAL)
			{
				this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 1.0f);
				this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.2f);
				//ゲームシーンをチュートリアルへ
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
				//ローカルネット階層へ切り替え
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
			//ローカルネット階層
		case TITLE_HIERARCHY_NUM_LOCAL_NET:
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TITLELOGO, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_START, 0.2f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_TUTO, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_BATTLE, 0.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 1.0f);
			this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.2f);
			//チュートリアル対決階層へ切り替え
			if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
			{
				PlaySound(SOUND_LABEL_SE_enter01);
				this[0].HierarchyNum = TITLE_HIERARCHY_NUM_TUTO_BATTLE;
				this[TITLE_OBJECT_NAME_TUTO].bDisp = true;
				this[TITLE_OBJECT_NAME_BATTLE].bDisp = true;
				this[TITLE_OBJECT_NAME_LOCAL].bDisp = false;
				this[TITLE_OBJECT_NAME_NET].bDisp = false;

			}
			//上入力でローカルにカーソル合わせる
			else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_ANALOG_L_UP))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_LOCAL;
			}
			//下入力でネットにカーソル合わせる
			else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_ANALOG_L_DOWN))
			{
				PlaySound(SOUND_LABEL_SE_enter03);
				this[0].SceneNum = TITLE_SELECT_SCENE_NET;
			}

			if (this[0].SceneNum == TITLE_SELECT_SCENE_LOCAL)
			{
				this[0].SetColorTitle(TITLE_OBJECT_NAME_LOCAL, 1.0f);
				this[0].SetColorTitle(TITLE_OBJECT_NAME_NET, 0.2f);
				//シーンをゲームへ
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
				//シーンをネットマッチへ
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
// 描画処理
//=============================================================================
void TITLE::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this[0].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, this[0].tex2DVB.pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this[1].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, this[1].tex2DVB.pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);


	for (int CntTitleSelectScene = 0; CntTitleSelectScene < TITLE_SELECT_SCENE_MAX; CntTitleSelectScene++)
	{
		if (this[CntTitleSelectScene + 2].bDisp == true)
		{

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this[CntTitleSelectScene + 2].tex2DVB.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, this[CntTitleSelectScene + 2].tex2DVB.pD3DTexture);

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
	//タイトルBG生成
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

	//タイトルスタートロゴ生成
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
			pVtx[0].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X - TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y - TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X + TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y - TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X - TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y + TITLE_START_LOGO_HEIGHT, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TITLE_START_LOGO_POS_X + TITLE_START_LOGO_WIDTH, TITLE_START_LOGO_POS_Y + TITLE_START_LOGO_HEIGHT, 0.0f);

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

	//タイトルセレクトロゴ生成
	{
		for (int CntTitleSelectScene = TITLE_OBJECT_NAME_TUTO; CntTitleSelectScene < TITLE_OBJECT_NAME_LOCAL; CntTitleSelectScene++)
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
				int cnt = CntTitleSelectScene - TITLE_OBJECT_NAME_TUTO;
				pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);

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

	//タイトルセレクトロゴ生成
	{
		for (int CntTitleSelectScene = TITLE_OBJECT_NAME_LOCAL; CntTitleSelectScene < TITLE_OBJECT_NAME_MAX; CntTitleSelectScene++)
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
				int cnt = CntTitleSelectScene- TITLE_OBJECT_NAME_LOCAL;
				pVtx[0].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y - TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_WIDTH, TITLE_SELECT_POS_Y + TITLE_SELECT_HEIGHT + (cnt*TITLE_SELECT_HEIGHT * 2), 0.0f);

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

