/**
* @file Scene.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../../h/main.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Object/Scene/Scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float	SCENE_RATE{ 0.02f };									//!< フェード係数

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
SCENE::SCENE(void)
{
	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_SCENE_MAX, FVF_VERTEX_2D);

	//描画位置反映　スクリーンサイズで作成
	D3DXVECTOR3 VTX[POLYGON_2D_VERTEX] = {
	VEC3_ALL0,
	D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, SCREEN_H, 0.0f),
	D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f),
	};
	this->vtx.Vertex2D(0, VTX);

	//RHW設定
	this->vtx.RHW2D(0);

	//UV設定
	this->vtx.UV2D(0);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
SCENE::~SCENE(void)
{
	//頂点解放
	this->vtx.~VTXBUFFER();
}

//=============================================================================
// 初期化処理
//=============================================================================
void SCENE::Init(void)
{
	//初期値設定
	this->ScenePara.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->ScenePara.eFade = FADE_IN;
	this->ScenePara.eNowScene = SCENE_TITLE;
	this->ScenePara.eNextScene = SCENE_TITLE;
	this->ScenePara.sno = -1;
}

//=============================================================================
// 更新処理
//=============================================================================
void SCENE::Update(void)
{
	if (this->ScenePara.eFade != FADE_NONE)
	{// フェード処理中
		if (this->ScenePara.eFade == FADE_OUT)
		{// フェードアウト処理
			this->ScenePara.color.a += SCENE_RATE;		// α値を加算して画面を消していく
			if (this->ScenePara.color.a >= 1.0f)
			{
				// 状態を切り替え
				SetScene(this->ScenePara.eNextScene);

				// フェードイン処理に切り替え
				this->ScenePara.color.a = 1.0f;
				this->ScenePara.eFade = FADE_MUSIC_STOP;

				// BGM停止
				StopSound();
			}

			// 色を設定
			this->vtx.Color2D(0, this->ScenePara.color);
		}
		else if (this->ScenePara.eFade == FADE_MUSIC_STOP)
		{
			// BGM再生
			if (this->ScenePara.sno > -1)
			{
				PlaySound(this->ScenePara.sno);
			}

			this->ScenePara.eFade = FADE_IN;
		}
		else if (this->ScenePara.eFade == FADE_IN)
		{// フェードイン処理
			this->ScenePara.color.a -= SCENE_RATE;		// α値を減算して画面を浮き上がらせる
			if (this->ScenePara.color.a <= 0.0f)
			{
				// フェード処理終了
				this->ScenePara.color.a = 0.0f;
				this->ScenePara.eFade = FADE_NONE;

			}

			// 色を設定
			this->vtx.Color2D(0, this->ScenePara.color);
		}
	}
}

//=============================================================================
// フェード画面
//=============================================================================
void SCENE::Draw(void)
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
// シーンを切り替える
//=============================================================================
void SCENE::SetScene(e_SCENE next)
{
	this->ScenePara.eNowScene = next;

}

//=============================================================================
// シーン切り替え受け取る
//=============================================================================
void SCENE::NextScene(eFADE_TYPE eFade, e_SCENE next, int sno)
{
	this->ScenePara.eFade = eFade;
	this->ScenePara.eNextScene = next;
	this->ScenePara.sno = sno;

}

