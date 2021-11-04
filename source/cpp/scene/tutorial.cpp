/**
* @file tutorial.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/object/objectclass.h"
#include "../../h/scene/tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TUTORIAL		("../data/TEXTURE/other/tutorialbg.png")			//!< 読み込むテクスチャファイル名

constexpr int	TUTORIAL_SIZE_X{ 300 };					//!< チュートリアルの幅
constexpr int	TUTORIAL_SIZE_Y{ 200 };					//!< チュートリアルの高さ
constexpr int	TUTORIAL_POS_X{ SCREEN_CENTER_X };		//!< チュートリアルの表示位置
constexpr int	TUTORIAL_POS_Y{ SCREEN_CENTER_Y };		//!< チュートリアルの表示位置
constexpr int	TUTORIAL_POS_X_OFFSET{ 170 };			//!< チュートリアルの表示位置
constexpr int	TUTORIAL_POS_Y_OFFSET{ 200 };			//!< チュートリアルの表示位置

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
TUTO::TUTO(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y - TUTORIAL_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(TUTORIAL_POS_X * 2 - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y - TUTORIAL_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(TUTORIAL_POS_X - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y * 2 - TUTORIAL_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(TUTORIAL_POS_X * 2 - TUTORIAL_POS_X_OFFSET, TUTORIAL_POS_Y * 2 - TUTORIAL_POS_Y_OFFSET, 0.0f));


	//カウントループ
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//描画位置反映
		D3DXVECTOR3 pos = this->Transform[CntTuto].Pos();
		this->vtx.Vertex2D(CntTuto, TUTORIAL_SIZE_X/2, TUTORIAL_SIZE_Y/2, pos);

		//RHW設定
		this->vtx.RHW2D(CntTuto);

		//UV設定
		this->vtx.UV2D(CntTuto);

		//カラー設定
		this->vtx.Color2D(CntTuto, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

		//使用設定
		this->iUseType[CntTuto].Use(YesUseType1);
	}

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_TUTORIAL);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
TUTO::~TUTO(void)
{
	//テクスチャ解放
	this->tex.~TEXTURE();
	//頂点解放
	this->vtx.~VTXBuffer();
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化
//=============================================================================
void TUTO::Init(void)
{	
	//カウントループ
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//初期値では全部表示させる
		this->iUseType[CntTuto].Use(YesUseType1);
	}
}

//=============================================================================
// 更新
//=============================================================================
void TUTO::Update(GAME_OBJECT* obj, FADE *fade)
{

	//カウントループ
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//スタートボタンorENTERでタイトルに戻る
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(CntTuto, BUTTON_START))
		{
			PlaySound(SOUND_LABEL_SE_enter02);
			fade->SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
			obj->Init();
		}

		//操作方法画像　表示非表示切り替え
		if (GetKeyboardTrigger(DIK_M) || IsButtonTriggered(CntTuto, BUTTON_R3))
		{
			if (this->iUseType[CntTuto].Use() == YesUseType1)
			{
				this->iUseType[CntTuto].Use(NoUse);
			}
			else if (this->iUseType[CntTuto].Use() == NoUse)
			{
				this->iUseType[CntTuto].Use(YesUseType1);
			}
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void TUTO::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//カウントループ
	for (int CntTuto = 0; CntTuto < OBJECT_PLAYER_MAX*OBJECT_TUTORIAL_MAX; CntTuto++)
	{
		//描画判定　
		if (this->iUseType[CntTuto].Use()== YesUseType1)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex.Texture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntTuto * 4), POLYGON_2D_NUM);
		}
	}
}

