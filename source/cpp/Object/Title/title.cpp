/**
* @file title.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../../h/main.h"
#include "../../../h/Other/input.h"
#include "../../../h/Object/Scene/Scene.h"
#include "../../../h/Other/sound.h"
#include "../../../h/net/sock.h"
#include "../../../h/Object/Title/title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr int	TITLE_BG_POS_X{ SCREEN_CENTER_X };				// タイトルBGの位置(X座標)
constexpr int	TITLE_BG_POS_Y{ SCREEN_CENTER_Y };				// タイトルBGの位置(Y座標)
constexpr int	TITLE_BG_WIDTH{ SCREEN_W };						// タイトルBGの幅
constexpr int	TITLE_BG_HEIGHT{ SCREEN_H };					// タイトルBGの高さ
constexpr int	TITLE_LOGO_POS_X{ SCREEN_CENTER_X };			// タイトルロゴの位置(X座標)
constexpr int	TITLE_LOGO_POS_Y{ SCREEN_CENTER_Y - 200 };		// タイトルロゴの位置(Y座標)
constexpr int	TITLE_LOGO_HEIGHT{ 500 };						// タイトルロゴの高さ
constexpr int	TITLE_LOGO_WIDTH{ 800 };						// タイトルロゴの幅
constexpr int	TITLE_PRESS_LOGO_POS_X{ SCREEN_CENTER_X };		// PRESSロゴの位置(X座標)
constexpr int	TITLE_PRESS_LOGO_POS_Y{ SCREEN_CENTER_Y + 200 };// PRESSロゴの位置(Y座標)
constexpr int	TITLE_PRESS_LOGO_WIDTH{ 400 };					// PRESSロゴの幅
constexpr int	TITLE_PRESS_LOGO_HEIGHT{ 100 };					// PRESSロゴの高さ
constexpr int	TITLE_SELECT_POS_X{ SCREEN_CENTER_X };			// セレクトテクスチャの位置(X座標)
constexpr int	TITLE_SELECT_POS_Y{ SCREEN_CENTER_Y };			// セレクトテクスチャの位置(Y座標)
constexpr int	TITLE_SELECT_WIDTH{ 300 };						// セレクトテクスチャの幅
constexpr int	TITLE_SELECT_HEIGHT{ 200 };						// セレクトテクスチャの高さ
constexpr int	TITLE_SELECT_OFFSET_X{ 200 };					// オフセット値
constexpr int	TITLE_SELECT_OFFSET_Y{ 200 };					// オフセット値
constexpr int	COUNT_APPERA_START{ 10 };						// スタートボタン出現までの時間
constexpr int	INTERVAL_DISP_START{ 30 };						// スタートボタン点滅の時間
constexpr int	COUNT_WAIT_DEMO{ 60 * 5 };						// デモまでの待ち時間

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
TITLE::TITLE(void)
{
	//頂点作成
	this->vtx.MakeVertex2D(OBJECT_TITLE_MAX, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform[NAME_BG].Pos(D3DXVECTOR3(TITLE_BG_POS_X, TITLE_BG_POS_Y, 0.0f));
	this->Transform[NAME_LOGO].Pos(D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y, 0.0f));
	this->Transform[NAME_PRESS].Pos(D3DXVECTOR3(TITLE_PRESS_LOGO_POS_X, TITLE_PRESS_LOGO_POS_Y, 0.0f));
	this->Transform[NAME_TUTO].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_OFFSET_Y, 0.0f));
	this->Transform[NAME_LOCAL].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y - TITLE_SELECT_OFFSET_Y, 0.0f));
	this->Transform[NAME_NET].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X - TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_OFFSET_Y, 0.0f));
	this->Transform[NAME_QUIT].Pos(D3DXVECTOR3(TITLE_SELECT_POS_X + TITLE_SELECT_OFFSET_X, TITLE_SELECT_POS_Y + TITLE_SELECT_OFFSET_Y, 0.0f));

	//描画位置反映
	this->vtx.Vertex2D(NAME_BG, TITLE_BG_WIDTH / 2, TITLE_BG_HEIGHT / 2, this->Transform[NAME_BG].Pos());
	this->vtx.Vertex2D(NAME_LOGO, TITLE_LOGO_WIDTH / 2, TITLE_LOGO_HEIGHT / 2, this->Transform[NAME_LOGO].Pos());
	this->vtx.Vertex2D(NAME_PRESS, TITLE_PRESS_LOGO_WIDTH / 2, TITLE_PRESS_LOGO_HEIGHT / 2, this->Transform[NAME_PRESS].Pos());
	this->vtx.Vertex2D(NAME_TUTO, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_TUTO].Pos());
	this->vtx.Vertex2D(NAME_LOCAL, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_LOCAL].Pos());
	this->vtx.Vertex2D(NAME_NET, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_NET].Pos());
	this->vtx.Vertex2D(NAME_QUIT, TITLE_SELECT_WIDTH / 2, TITLE_SELECT_HEIGHT / 2, this->Transform[NAME_QUIT].Pos());

	//カウントループ
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//RHW設定
		this->vtx.RHW2D(CntTitle);

		//UV設定
		this->vtx.UV2D(CntTitle);

		// テクスチャ読み込み
		this->tex[CntTitle].LoadTexture(this->c_FileNameTex[CntTitle]);

	}
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
TITLE::~TITLE(void)
{
	//カウントループ
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//テクスチャ解放
		this->tex[CntTitle].~TEXTURE();
	}
	//頂点解放
	this->vtx.~VTXBUFFER();
}

//=============================================================================
// 他クラスのアドレス取得
//=============================================================================
void TITLE::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pscene = obj->GetScene();
}

//=============================================================================
// 初期化処理
//=============================================================================
void TITLE::Init(void)
{
	//カウントループ
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//カラーの設定と使用設定
		{
			D3DXCOLOR col;
			switch (CntTitle)
			{
				//背景とロゴとPRESS画像だけ初期状態から使用
			case NAME_BG:
			case NAME_LOGO:
			case NAME_PRESS:
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				this->iUseType[CntTitle].Use(YesUseType1);
				break;

				//それ以外不使用
			default:
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				this->iUseType[CntTitle].Use(NoUse);
				break;
			}
			this->vtx.Color2D(CntTitle, col);
		}
	}

	//モード選択中のデータ初期化
	this->TitlePara.HierarchyNum = TITLE_HIERARCHY_NUM_ANIM;
	this->TitlePara.SceneNumX = this->TitlePara.SceneNumY = this->TitlePara.OldSceneNumX = this->TitlePara.OldSceneNumY = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void TITLE::Update(void)
{
	//カーソルを保存
	this->TitlePara.OldSceneNumX = this->TitlePara.SceneNumX;
	this->TitlePara.OldSceneNumY = this->TitlePara.SceneNumY;

	//階層確認
	switch (this->TitlePara.HierarchyNum)
	{
		//タイトルアニメ階層中
	case TITLE_HIERARCHY_NUM_ANIM:
		//AボタンかENTERが入力されたら階層切り替え
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//ヒエラルキー変更
			ChangeHierarchy(TITLE_HIERARCHY_NUM_SELECT);//目的のヒエラルキーを入れる
		}
		break;
		//シーンセレクト階層中
	case TITLE_HIERARCHY_NUM_SELECT:
		//BボタンかバックSpaceが入力されたら階層切り替え
		if (GetKeyboardTrigger(DIK_BACK) || IsButtonTriggered(0, BUTTON_B))
		{
			//ヒエラルキー変更
			ChangeHierarchy(TITLE_HIERARCHY_NUM_ANIM);
		}
		//カーソル移動処理
		else if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_ANALOG_L_UP) || IsButtonTriggered(0, BUTTON_DIGITAL_UP))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter03);
			//カーソル設定
			this->TitlePara.SceneNumY++;
			if (this->TitlePara.SceneNumY >= 2)this->TitlePara.SceneNumY = 0;
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_ANALOG_L_DOWN) || IsButtonTriggered(0, BUTTON_DIGITAL_DOWN))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter03);
			//カーソル設定
			this->TitlePara.SceneNumY--;
			if (this->TitlePara.SceneNumY <= -1)this->TitlePara.SceneNumY = 1;
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_ANALOG_L_RIGHT) || IsButtonTriggered(0, BUTTON_DIGITAL_RIGHT))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter03);
			//カーソル設定
			this->TitlePara.SceneNumX++;
			if (this->TitlePara.SceneNumX >= 2)this->TitlePara.SceneNumX = 0;
		}
		else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_ANALOG_L_LEFT) || IsButtonTriggered(0, BUTTON_DIGITAL_LEFT))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter03);
			//カーソル設定
			this->TitlePara.SceneNumX--;
			if (this->TitlePara.SceneNumX <= -1)this->TitlePara.SceneNumX = 1;
		}
		//scene選択処理
		this->CheckScene();
		break;
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

	//カウントループ
	for (int CntTitle = 0; CntTitle < OBJECT_TITLE_MAX; CntTitle++)
	{
		//描画判定　
		if (this->iUseType[CntTitle].Use() == YesUseType1)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0,this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[CntTitle].Texture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntTitle * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// 階層切り替え処理
//=============================================================================
void TITLE::ChangeHierarchy(int NextHierarchyType)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ヒエラルキー選択
	switch (NextHierarchyType)
	{
		//タイトルアニメに変更
	case TITLE_HIERARCHY_NUM_ANIM:
		//選択音再生
		PlaySound(SOUND_LABEL_SE_enter01);
		//ヒエラルキー変更
		this->TitlePara.HierarchyNum = TITLE_HIERARCHY_NUM_ANIM;
		//α値変更
		this->vtx.Color2D(NAME_LOGO, col);
		this->vtx.Color2D(NAME_PRESS, col);
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		this->vtx.Color2D(NAME_TUTO, col);
		this->vtx.Color2D(NAME_LOCAL, col);
		this->vtx.Color2D(NAME_NET, col);
		this->vtx.Color2D(NAME_QUIT, col);
		//使用変更
		this->iUseType[NAME_TUTO].Use(NoUse);
		this->iUseType[NAME_LOCAL].Use(NoUse);
		this->iUseType[NAME_NET].Use(NoUse);
		this->iUseType[NAME_QUIT].Use(NoUse);
		break;
		//シーン選択に変更
	case TITLE_HIERARCHY_NUM_SELECT:
		//選択音再生
		PlaySound(SOUND_LABEL_SE_enter01);
		//ヒエラルキー変更
		this->TitlePara.HierarchyNum = TITLE_HIERARCHY_NUM_SELECT;
		//選択シーンリセット
		this->TitlePara.SceneNumX = this->TitlePara.SceneNumY = 0;
		//α値変更
		this->vtx.Color2D(NAME_TUTO, col);
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		this->vtx.Color2D(NAME_LOCAL, col);
		this->vtx.Color2D(NAME_NET, col);
		this->vtx.Color2D(NAME_QUIT, col);
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		this->vtx.Color2D(NAME_LOGO, col);
		this->vtx.Color2D(NAME_PRESS, col);
		//使用変更
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
// シーン切り替え処理
//=============================================================================
void TITLE::CheckScene(void)
{
	//チュートリアル
	if (this->TitlePara.SceneNumX == 0 && this->TitlePara.SceneNumY == 0)
	{
		//カーソルが移動したらα値設定
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//α値変更
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_TUTO, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_LOCAL, col);
			this->vtx.Color2D(NAME_NET, col);
			this->vtx.Color2D(NAME_QUIT, col);
		}
		//シーンをチュートリアルへ
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter01);
			pscene->NextScene(FADE_OUT, SCENE_TUTORIAL, SOUND_LABEL_BGM_tutorial01);
		}
	}
	//ローカル対戦
	else if (this->TitlePara.SceneNumX == 1 && this->TitlePara.SceneNumY == 0)
	{
		//カーソルが移動したらα値設定
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//α値変更
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_LOCAL, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_TUTO, col);
			this->vtx.Color2D(NAME_NET, col);
			this->vtx.Color2D(NAME_QUIT, col);
		}
		//シーンをゲームへ
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter01);
			pscene->NextScene(FADE_OUT, SCENE_GAMECOUNTDOWN, SOUND_LABEL_BGM_normal01);
			SourceVolumeChange(0,SOUND_LABEL_BGM_normal01);
		}
	}
	//ネット対戦
	else if (this->TitlePara.SceneNumX == 0 && this->TitlePara.SceneNumY == 1)
	{
		//カーソルが移動したらα値設定
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//α値変更
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_NET, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_TUTO, col);
			this->vtx.Color2D(NAME_LOCAL, col);
			this->vtx.Color2D(NAME_QUIT, col);
		}
		//シーンをゲームへ
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter01);
			pscene->NextScene(FADE_OUT, SCENE_NETMATCH, SOUND_LABEL_BGM_select01);
		}
	}
	//アプリ終了へ
	else if (this->TitlePara.SceneNumX == 1 && this->TitlePara.SceneNumY == 1)
	{
		//カーソルが移動したらα値設定
		if (this->TitlePara.OldSceneNumX != this->TitlePara.SceneNumX || this->TitlePara.OldSceneNumY != this->TitlePara.SceneNumY)
		{
			//α値変更
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			this->vtx.Color2D(NAME_QUIT, col);
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			this->vtx.Color2D(NAME_LOCAL, col);
			this->vtx.Color2D(NAME_NET, col);
			this->vtx.Color2D(NAME_TUTO, col);
		}
		//アプリ終了
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_A))
		{
			//選択音再生
			PlaySound(SOUND_LABEL_SE_enter01);
			SetMsg(WM_QUIT);
		}
	}
}


