/**
* @file countdown.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../../h/main.h"
#include "../../../h/Object/Scene/Scene.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Net/sock.h"
#include "../../../h/Object/CountDown/countdown.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNTDOWN_TEXTURE_NUMBER			_T("../data/TEXTURE/BG/0-9.png")	//!< 読み込むテクスチャファイル名
#define COUNTDOWN_TEXTURE_LOGO				_T("../data/TEXTURE/sozai/go.png")	//!< 読み込むテクスチャファイル名

constexpr int COUNTDOWN_TEXTURE_NUMBER_SIZE_X{ 50 };			//!< テクスチャサイズ 
constexpr int COUNTDOWN_TEXTURE_NUMBER_SIZE_Y{ 50 };			//!< 同上  
constexpr int COUNTDOWN_TEXTURE_LOGO_SIZE_X{ 200 };				//!< テクスチャロゴサイズ 
constexpr int COUNTDOWN_TEXTURE_LOGO_SIZE_Y{ 200 };				//!< 同上  
constexpr int COUNTDOWN_TEXTURE_PATTERN_X{ 10 };				//!< アニメパターンのテクスチャ内分割数（X)  
constexpr int COUNTDOWN_TEXTURE_PATTERN_Y{ 1 };					//!< アニメパターンのテクスチャ内分割数（Y)  
constexpr int COUNTDOWN_ANIM_PATTERN_NUM{ COUNTDOWN_TEXTURE_PATTERN_X*COUNTDOWN_TEXTURE_PATTERN_Y };	//!< アニメーションパターン数  
constexpr int COUNTDOWN_TIME_ANIMATION{ 4 };					//!< アニメーションの切り替わるカウント  
constexpr int COUNTDOWN_TEXTURE_POS_X{ SCREEN_CENTER_X };		//!< テクスチャ座標
constexpr int COUNTDOWN_TEXTURE_POS_Y{ SCREEN_CENTER_Y };		//!< 同上  
constexpr int COUNTDOWN_FPS_TIME{ 240 };						//!< 毎フレームカウントしてタイム計測
constexpr int COUNTDOWN_FPS_TIME_START{ 4 };					//!< カウントダウンスタート時点のタイム
constexpr int COUNTDOWN_CHANGESIZE{ 5 };						//!< カウントダウン数字のサイズが拡大していく値
constexpr float COUNTDOWN_ALPHA{ 0.05f };						//!< カウントダウンが0の時GOを描画してGOが薄くなるスピード

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
COUNTDOWN::COUNTDOWN(void)
{
	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_COUNTDOWN_MAX, FVF_VERTEX_2D);

	//カウントループ
	for (int CntCountDown = 0; CntCountDown < OBJECT_COUNTDOWN_MAX; CntCountDown++)
	{
		//描画位置設定
		this->Transform[CntCountDown].Pos(D3DXVECTOR3(COUNTDOWN_TEXTURE_POS_X, COUNTDOWN_TEXTURE_POS_Y, 0.0f));
		D3DXVECTOR3 pos = this->Transform[CntCountDown].Pos();
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
		D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
		D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
		D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
		};

		//描画位置反映
		this->vtx.Vertex2D(CntCountDown,vtx);

		//RHW設定
		this->vtx.RHW2D(CntCountDown);

		//UVの設定
		this->vtx.UV2D(CntCountDown);

		//カラー設定
		this->vtx.Color2D(CntCountDown, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	}

	//使用設定
	this->iUseType[COUNTDOWN_TEX_NUMBER].Use(YesUseType1);
	this->iUseType[COUNTDOWN_TEX_LOGO].Use(NoUse);

	// テクスチャの読み込み
	this->tex[COUNTDOWN_TEX_NUMBER].LoadTexture(COUNTDOWN_TEXTURE_NUMBER);
	this->tex[COUNTDOWN_TEX_LOGO].LoadTexture(COUNTDOWN_TEXTURE_LOGO);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
COUNTDOWN::~COUNTDOWN(void)
{
	//テクスチャ解放
	this->tex[COUNTDOWN_TEX_NUMBER].~TEXTURE();
	this->tex[COUNTDOWN_TEX_LOGO].~TEXTURE();
	//頂点解放
	this->vtx.~VTXBUFFER();
}

//=============================================================================
// 他クラスのアドレス取得
//=============================================================================
void COUNTDOWN::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pmysocket = obj->GetMySocket();
	pscene = obj->GetScene();
}

//=============================================================================
// 初期化処理
//=============================================================================
void COUNTDOWN::Init(void)
{
	//初期設定　全て規定値
	this->iUseType[COUNTDOWN_TEX_NUMBER].Use(YesUseType1);
	this->iUseType[COUNTDOWN_TEX_LOGO].Use(NoUse);
	this->CountdownPara.color = 1.0f;
	this->CountdownPara.time_maneger = COUNTDOWN_FPS_TIME;
	this->CountdownPara.fps_maneger = COUNTDOWN_FPS_TIME_START;
	this->CountdownPara.CountAnim = 0;
	this->CountdownPara.PatternAnim = 0;
	this->CountdownPara.changeval = 0;
	this->CountdownPara.CountEnd = false;

}

//=============================================================================
// 更新処理
//=============================================================================
void COUNTDOWN::Update(void)
{
	NumberCountUpdate();
	NumberSizeUpdate();
	//StopSound();
	if (this->CountdownPara.CountEnd == true)
	{
		MasterVolumeChange(1);
		//ローカルゲーム開始
		if (pmysocket->GetNetGameStartFlag() == false)
		{
			pscene->SetScene(SCENE_GAME);
			PlaySound(SOUND_LABEL_BGM_normal01);
			SourceVolumeChange(0.5f, SOUND_LABEL_BGM_normal01);
		}
		//ネットゲーム開始
		else
		{
			pscene->SetScene(SCENE_NETGAME);
			PlaySound(SOUND_LABEL_BGM_boss01);
			SourceVolumeChange(0.8f, SOUND_LABEL_BGM_boss01);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void COUNTDOWN::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int CntCountDown = 0; CntCountDown < OBJECT_COUNTDOWN_MAX; CntCountDown++)
	{
		bool use = this->iUseType[CntCountDown].Use();
		if (use == true )
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[CntCountDown].Texture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntCountDown * 4), POLYGON_2D_NUM);
		}
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void COUNTDOWN::NumberCountUpdate(void)
{
	//現在タイムを取得
	int val = this->CountdownPara.time_maneger;
	
	//サイズを徐々に大きく
	this->CountdownPara.changeval += COUNTDOWN_CHANGESIZE;

	//カウントダウンが終了したらフラグセット
	if (val <= 0)
	{
		this->iUseType[COUNTDOWN_TEX_NUMBER].Use(NoUse);
		this->iUseType[COUNTDOWN_TEX_LOGO].Use(YesUseType1);
	}
	//else this->iUseType[COUNTDOWN_TEX_LOGO].Use(NoUse);



	//240/3=80 ->80フレームに1カウントダウン ちょうどいい時間を設定
	if (val % (COUNTDOWN_FPS_TIME / (COUNTDOWN_FPS_TIME_START - 1)) == 0)
	{
		//数値を更新してサイズを0にする
		this->CountdownPara.fps_maneger -= 1;
		float uStart, uEnd, vStart, vEnd;
		uStart = 0.1f*this->CountdownPara.fps_maneger;
		uEnd = 0.1f*this->CountdownPara.fps_maneger + 0.1f;
		vStart = 0.0f;
		vEnd = 1.0f;
		this->vtx.UV2D(COUNTDOWN_TEX_NUMBER, uStart, uEnd, vStart, vEnd);
		this->CountdownPara.changeval = 0;
	}

	//カウントダウンが終わったらスタートロゴを表示　表示後じょじょにαを下げて、0になるとゲーム開始
	bool use = this->iUseType[COUNTDOWN_TEX_LOGO].Use();
	if (use == true)
	{
		this->CountdownPara.color -= COUNTDOWN_ALPHA;
		this->vtx.Color2D(COUNTDOWN_TEX_LOGO, D3DXCOLOR(1.0f, 1.0f, 1.0f, this->CountdownPara.color));
		if (this->CountdownPara.color <= 0) this->CountdownPara.CountEnd = true;
	}


}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void COUNTDOWN::NumberSizeUpdate(void)
{
	//描画位置設定
	this->Transform[COUNTDOWN_TEX_NUMBER].Pos(D3DXVECTOR3(COUNTDOWN_TEXTURE_POS_X, COUNTDOWN_TEXTURE_POS_Y, 0.0f));
	D3DXVECTOR3 pos = this->Transform[COUNTDOWN_TEX_NUMBER].Pos();
	D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
	{
	D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
	D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y - COUNTDOWN_TEXTURE_NUMBER_SIZE_Y - this->CountdownPara.changeval,0.0f),
	D3DXVECTOR3(pos.x - COUNTDOWN_TEXTURE_NUMBER_SIZE_X - this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
	D3DXVECTOR3(pos.x + COUNTDOWN_TEXTURE_NUMBER_SIZE_X + this->CountdownPara.changeval,pos.y + COUNTDOWN_TEXTURE_NUMBER_SIZE_Y + this->CountdownPara.changeval,0.0f),
	};

	//描画位置反映
	this->vtx.Vertex2D(COUNTDOWN_TEX_NUMBER, vtx);
}

//=============================================================================
// カウントダウン加算処理
//=============================================================================
void COUNTDOWN::AddCountdown(int val)
{
	this->CountdownPara.time_maneger += val;
}
