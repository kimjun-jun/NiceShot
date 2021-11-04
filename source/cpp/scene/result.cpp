/**
* @file result.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/net/sock.h"
#include "../../h/scene/result.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr int	RESULT_LOGO_POS_X{ SCREEN_CENTER_X };			//!< ロゴの座標
constexpr int	RESULT_LOGO_POS_Y{ SCREEN_CENTER_Y - 200 };		//!< ロゴの座標
constexpr int	RESULT_BG_SIZE_X{ SCREEN_W };					//!< 背景の幅
constexpr int	RESULT_BG_SIZE_Y{ SCREEN_H };					//!< 背景の高さ
constexpr float	RESULT_LOGO_SIZE_X{ 300.0f };					//!< ロゴの幅
constexpr float	RESULT_LOGO_SIZE_Y{200.0f};						//!< ロゴの高さ

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
RESULT::RESULT(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_RESULT_MAX, FVF_VERTEX_2D);

	//描画位置反映　スクリーンサイズで作成 BGとLOGO
	D3DXVECTOR3 vtxBG[POLYGON_2D_VERTEX] = {
	VEC3_ALL0,
	D3DXVECTOR3(RESULT_BG_SIZE_X, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, RESULT_BG_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_BG_SIZE_X, RESULT_BG_SIZE_Y, 0.0f),
	};
	this->vtx.Vertex2D(RESULT_TEX_BG, vtxBG);

	D3DXVECTOR3 vtxLOGO[POLYGON_2D_VERTEX] = {
	D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y - RESULT_LOGO_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_LOGO_POS_X - RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f),
	D3DXVECTOR3(RESULT_LOGO_POS_X + RESULT_LOGO_SIZE_X, RESULT_LOGO_POS_Y + RESULT_LOGO_SIZE_Y, 0.0f),
	};
	this->vtx.Vertex2D(RESULT_TEX_LOGO, vtxLOGO);

	for (int CntResult = 0; CntResult < OBJECT_RESULT_MAX; CntResult++)
	{
		//RHW設定
		this->vtx.RHW2D(CntResult);
		//UVの設定
		this->vtx.UV2D(CntResult);
		//カラー設定
		this->vtx.Color2D(CntResult, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		this->tex[CntResult].LoadTexture(this->c_aFileNameTex[CntResult]);
	}
}


//=============================================================================
// デストラクタ　削除
//=============================================================================
RESULT::~RESULT(void)
{
	for (int CntResult = 0; CntResult < OBJECT_RESULT_MAX; CntResult++)
	{
		this->tex[CntResult].~TEXTURE();
	}
	//頂点解放
	this->vtx.~VTXBuffer();
}

//=============================================================================
// 初期化処理
//=============================================================================
void RESULT::Init(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void RESULT::Update(GAME_OBJECT* obj, FADE *fade)
{
	//リザルト画面中に入力があれば初期化してタイトルシーンに戻る
	if (IsButtonTriggered(0, BUTTON_A)|| GetKeyboardTrigger(DIK_RETURN))
	{
		obj->Init();	// ゲームの初期化処理
		fade->SetFade(FADE_OUT, SCENE_TITLE, SOUND_LABEL_BGM_title01);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void RESULT::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int CntResult = 0; CntResult < OBJECT_RESULT_MAX; CntResult++)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
		pDevice->SetTexture(0, this->tex[CntResult].Texture());
		// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntResult * 4), POLYGON_2D_NUM);
	}
}

