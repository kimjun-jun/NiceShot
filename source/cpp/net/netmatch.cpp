/**
* @file netmatch.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/other/fade.h"
#include "../../h/other/sound.h"
#include "../../h/net/netmatch.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NETMATCHRIAL		("../data/TEXTURE/old/MatchLogo.png")						//!< 読み込むテクスチャファイル名
constexpr int	NETMATCHRIAL_SIZE_X			{ SCREEN_W/4-50 };										//!< チュートリアルの幅
constexpr int	NETMATCHRIAL_SIZE_Y			{ SCREEN_H/4-50 };										//!< チュートリアルの高さ
constexpr int	NETMATCHRIAL_POS_X			{ SCREEN_CENTER_X };									//!< チュートリアルの表示位置
constexpr int	NETMATCHRIAL_POS_Y			{ SCREEN_CENTER_Y };									//!< チュートリアルの表示位置

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
NETMATCH::NETMATCH(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex2D(1, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform.Pos(D3DXVECTOR3(NETMATCHRIAL_POS_X - NETMATCHRIAL_SIZE_X, NETMATCHRIAL_POS_Y - NETMATCHRIAL_SIZE_Y, 0.0f));

	//描画位置反映
	D3DXVECTOR3 pos = this->Transform.Pos();
	this->vtx.Vertex2D(0, NETMATCHRIAL_SIZE_X / 2, NETMATCHRIAL_SIZE_Y / 2, pos);

	//RHW設定
	this->vtx.RHW2D(0);

	//UV設定
	this->vtx.UV2D(0);

	//カラー設定
	this->vtx.Color2D(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_NETMATCHRIAL);
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
NETMATCH::~NETMATCH(void)
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
void NETMATCH::Init(void)
{

}

//=============================================================================
// 更新
//=============================================================================
void NETMATCH::Update(GAME_OBJECT* obj, FADE *fade)
{

}

//=============================================================================
// 描画
//=============================================================================
void NETMATCH::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
	pDevice->SetTexture(0, this->tex.Texture());
	// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
}
