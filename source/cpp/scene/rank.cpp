/**
* @file rank.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../h/scene/rank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	RANK_SIZE_X				(SCREEN_W/4)				//!< やられた順位テクスチャの幅
#define	RANK_SIZE_Y				(SCREEN_H/4)				//!< やられた順位テクスチャの高さ
#define	RANK_POS_X				(SCREEN_W)					//!< やられた順位テクスチャの表示位置
#define	RANK_POS_Y				(SCREEN_H)					//!< やられた順位テクスチャの表示位置
#define	RANK_POS_OFFSET_LOW		(1.0f/4.0f)					//!< スクリーン分割数
#define	RANK_POS_OFFSET_HI		(3.0f/4.0f)					//!< スクリーン分割数

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
RANK::RANK(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx.MakeVertex2D(OBJECT_RANK_MAX, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_LOW, RANK_POS_Y *RANK_POS_OFFSET_LOW, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_HI, RANK_POS_Y *RANK_POS_OFFSET_LOW, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_LOW, RANK_POS_Y *RANK_POS_OFFSET_HI, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(RANK_POS_X*RANK_POS_OFFSET_HI, RANK_POS_Y *RANK_POS_OFFSET_HI, 0.0f));

	//カウントループ
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		//描画位置設定
		D3DXVECTOR3 pos = this->Transform[CntRank].Pos();
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
		{
		D3DXVECTOR3(pos.x - RANK_SIZE_X,pos.y - RANK_SIZE_Y,0.0f),
		D3DXVECTOR3(pos.x + RANK_SIZE_X,pos.y - RANK_SIZE_Y,0.0f),
		D3DXVECTOR3(pos.x - RANK_SIZE_X,pos.y + RANK_SIZE_Y,0.0f),
		D3DXVECTOR3(pos.x + RANK_SIZE_X,pos.y + RANK_SIZE_Y,0.0f),
		};
		this->vtx.Vertex2D(CntRank, vtx);

		//RHW設定
		this->vtx.RHW2D(CntRank);

		//UVの設定
		this->vtx.UV2D(CntRank);

		//カラー設定
		this->vtx.Color2D(CntRank, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//使用設定
		this->iUseType[CntRank].Use(NoUse);
	}

	//カウントループ
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX-1; CntRank++)
	{
		// テクスチャの読み込み
		this->tex[CntRank].LoadTexture(this->c_aFileNameTex[CntRank]);
	}
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
RANK::~RANK(void)
{
	//カウントループ
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX - 1; CntRank++)
	{
		//テクスチャ解放
		this->tex[CntRank].~TEXTURE();
	}
	//頂点解放
	this->vtx.~VTXBuffer();
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void RANK::Init(void)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		this->iUseType[CntRank].Use(NoUse);
		this->RankParaOne.NetUse = false;
		this->RankParaAll[CntRank].RankNum = RANK_COUNTDOWN_NUM;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void RANK::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void RANK::Draw(bool Netflag)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル対戦時
	if (Netflag == false)
	{
		for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
		{
			if (this->iUseType[CntRank].Use() == YesUseType1)
			{
				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);
				// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
				pDevice->SetTexture(0, this->tex[this->RankParaAll[CntRank].RankNum].Texture());
				// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntRank * 4), POLYGON_2D_NUM);
			}
		}
	}

	//ネット対戦時
	else
	{
		for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
		{
			bool use = this->iUseType[CntRank].Use();
			if (use == true)
			{
				if (this->RankParaOne.NetUse == true)
				{
					// 頂点バッファをデバイスのデータストリームにバインド
					pDevice->SetStreamSource(0, this->vtx.VtxBuff(), 0, sizeof(VERTEX_2D));
					// 頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_2D);
					// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
					pDevice->SetTexture(0, this->tex[this->RankParaAll[CntRank].RankNum].Texture());
					// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntRank * 4), POLYGON_2D_NUM);
				}
			}
		}
	}
}
//=============================================================================
// ランクをセット
//=============================================================================
void RANK::SetRank(int PlayerNum)
{
	//使用可能にする
	this->iUseType[PlayerNum].Use(YesUseType1);
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		if (this->iUseType[CntRank].Use() == NoUse)
		{
			//4スタートで一人死ぬたびにカウントダウンする　死んだ人はカウントダウンされずに順位が確定になる
			this->RankParaAll[CntRank].RankNum--;
		}
	}
}

//=============================================================================
// ランクをセット
//=============================================================================
void RANK::SetRankNet(int PlayerNum, int NetMyNumber)
{
	for (int CntRank = 0; CntRank < OBJECT_RANK_MAX; CntRank++)
	{
		if (this->iUseType[CntRank].Use() == NoUse)
		{
			//ネット対戦用に描画位置を調整
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
			{
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(SCREEN_W, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, SCREEN_H, 0.0f),
			D3DXVECTOR3(SCREEN_W, SCREEN_H, 0.0f),
			};
			this->vtx.Vertex2D(CntRank, vtx);

			//自分の順位を記録して順位を使用する
			this->RankParaAll[CntRank].RankNum = PlayerNum;
			this->iUseType[CntRank].Use(YesUseType1);

			//プレイヤーナンバーとネット固有マイナンバーが同じならネット用フラグオン
			//ここをオンにしないとネット対戦時に順位が表示されない
			if (PlayerNum == NetMyNumber)
			{
				this->RankParaOne.NetUse = true;
			}
			break;
		}
	}
}
