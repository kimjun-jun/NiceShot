/**
* @file fade.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/sound.h"
#include "../../h/other/fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	FADE_RATE		(0.02f)									//!< フェード係数

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
FADE::FADE(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_FADE_MAX, FVF_VERTEX_2D);

	//描画位置反映　スクリーンサイズで作成
	D3DXVECTOR3 VTX[POLYGON_2D_VERTEX] = {
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, SCREEN_H, 0.0f),
	D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f),
	};
	this->vtx.Vertex2D(0, VTX);
}


//=============================================================================
// デストラクタ　削除
//=============================================================================
FADE::~FADE(void)
{
	//頂点解放
	this->vtx.~VTXBuffer();
}

//=============================================================================
// 初期化処理
//=============================================================================
void FADE::Init(void)
{
	//初期値設定
	this->FadePara.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->FadePara.eFade = FADE_IN; 
	this->FadePara.eScene = SCENE_TITLE;
	this->FadePara.sno = -1;
}

//=============================================================================
// 更新処理
//=============================================================================
void FADE::Update(GAME_OBJECT*obj)
{
	if (this->FadePara.eFade != FADE_NONE)
	{// フェード処理中
		if (this->FadePara.eFade == FADE_OUT)
		{// フェードアウト処理
			this->FadePara.color.a += FADE_RATE;		// α値を加算して画面を消していく
			if (this->FadePara.color.a >= 1.0f)
			{
				// 状態を切り替え
				obj->SetScene(this->FadePara.eScene);
				
				// フェードイン処理に切り替え
				this->FadePara.color.a = 1.0f;
				this->FadePara.eFade = FADE_MUSIC_STOP;

				// BGM停止
				StopSound();
			}

			// 色を設定
			this->vtx.Color2D(0,this->FadePara.color);
		}
		else if (this->FadePara.eFade == FADE_MUSIC_STOP)
		{
			// BGM再生
			if (this->FadePara.sno > -1)
			{
				PlaySound(this->FadePara.sno);
			}

			this->FadePara.eFade = FADE_IN;
		}
		else if (this->FadePara.eFade == FADE_IN)
		{// フェードイン処理
			this->FadePara.color.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
			if (this->FadePara.color.a <= 0.0f)
			{
				// フェード処理終了
				this->FadePara.color.a = 0.0f;
				this->FadePara.eFade = FADE_NONE;

			}

			// 色を設定
			this->vtx.Color2D(0,this->FadePara.color);
		}
	}
}

//=============================================================================
// フェード画面
//=============================================================================
void FADE::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
	// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
}

//=============================================================================
// フェードの状態設定
//=============================================================================
void FADE::SetFade(FADE_TYPE fade, E_STAGE next, int sno)
{
	this->FadePara.eFade = fade;
	this->FadePara.eScene = next;
	this->FadePara.sno = sno;

}

