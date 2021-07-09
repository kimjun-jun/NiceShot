/**
* @file vitalgauge.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../../h/scene/rank.h"
#include "../../h/object/vitalgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VITALGAUGE_SIZE_X			(500.0f)						//!< アタックゲージの数字の幅
#define	VITALGAUGE_SIZE_Y			(20.0f)							//!< アタックゲージの数字の高さ
#define	VITALGAUGE_SIZE_X_OFFSET	(0.8f)							//!< アタックゲージの数字の幅
#define	VITALGAUGE_SIZE_Y_OFFSET	(0.8f)							//!< アタックゲージの数字の高さ
#define	VITALGAUGE_BASE_POS_X		(570.0f)						//!< アタックゲージの表示基準位置Ｘ座標
#define	VITALGAUGE_BASE_POS_Y		(SCREEN_CENTER_Y/10)			//!< アタックゲージの表示基準位置Ｙ座標

#define VITALGAUGE_P1_POS_X			SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P1_POS_Y			SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P2_POS_X			SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P2_POS_Y			SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P3_POS_X			SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P3_POS_Y			SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P4_POS_X			SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P4_POS_Y			SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_NET_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X+55.0f
#define VITALGAUGE_NET_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y

#define	VITAL_GREEN_DIAMETER		(0.5f)	//!< 
#define	VITAL_ORANGE_DIAMETER		(0.2f)	//!< 
#define	VITAL_RED_DIAMETER			(0.1f)	//!< 

#define	VITAL_DRAW_TEX_NUM			(2)		//!< 各プレイヤーが描画するテクスチャの数(枠、本体)

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
VITALGAUGE::VITALGAUGE(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx[PLAYER01].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);
	this->vtx[PLAYER02].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);
	this->vtx[PLAYER03].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);
	this->vtx[PLAYER04].MakeVertex2D(VITAL_DRAW_TEX_NUM, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));

	//カウントループ プレイヤー数(ゲージ数)
	for (int CntVital = 0; CntVital < OBJECT_VITAL_MAX; CntVital++)
	{
		//ベース座標取得
		D3DXVECTOR3 pos = this->Transform[CntVital].Pos;
		//カウントループ 弾数
		for (int CntVitalTex = 0; CntVitalTex < VITAL_DRAW_TEX_NUM; CntVitalTex++)
		{
			//「枠」頂点と「中身」頂点で別の処理をする
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			if (CntVitalTex==0)//枠 0
			{
				vtx[0] = D3DXVECTOR3(pos.x, pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
			}
			else//中身 1	 
			{
				float vitallen = float(VITALGAUGE_SIZE_X * (this->VitalGaugePara[CntVital].VitalPower / PLAYER_VITAL_MAX));//バイタルの長さ
				vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			}
			//描画位置反映
			this->vtx[CntVital].Vertex2D(CntVitalTex, vtx);

			//RHW設定
			this->vtx[CntVital].RHW2D(CntVitalTex);

			//UVの設定
			this->vtx[CntVital].UV2D(CntVitalTex);

			//カラー設定
			this->vtx[CntVital].Color2D(CntVitalTex, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	//カウントループ VitalGaugeのテクスチャ数
	for (int CntVitalTex = 0; CntVitalTex < VITAL_TEX_MAX; CntVitalTex++)
	{
		// テクスチャの読み込み
		this->tex[CntVitalTex].LoadTexture(this->c_aFileNameTex[CntVitalTex]);
	}
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
VITALGAUGE::~VITALGAUGE(void)
{
	//カウントループ VitalGaugeのテクスチャ数
	for (int CntVitalTex = 0; CntVitalTex < VITAL_TEX_MAX; CntVitalTex++)
	{
		//テクスチャ解放
		this->tex[CntVitalTex].~TEXTURE();
	}
	//カウントループ プレイヤー数(ゲージ数)
	for (int CntVital = 0; CntVital < OBJECT_BULLETGAUGE_MAX; CntVital++)
	{
		//頂点解放
		this->vtx[CntVital].~VTXBuffer();
	}
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void VITALGAUGE::Init(void)
{
	//描画位置設定
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));

	//カウントループ プレイヤー数(ゲージ数)
	for (int CntVital = 0; CntVital < OBJECT_VITAL_MAX; CntVital++)
	{
		//体力満タン
		this->VitalGaugePara[CntVital].VitalPower = PLAYER_VITAL_MAX;

		//ベース座標取得
		D3DXVECTOR3 pos = this->Transform[CntVital].Pos;
		//カウントループ 弾数
		for (int CntVitalTex = 0; CntVitalTex < VITAL_DRAW_TEX_NUM; CntVitalTex++)
		{
			//「枠」頂点と「中身」頂点で別の処理をする
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			if (CntVitalTex == 0)//枠
			{
				vtx[0] = D3DXVECTOR3(pos.x, pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
			}
			else//中身		
			{
				float vitallen = float(VITALGAUGE_SIZE_X * (this->VitalGaugePara[CntVital].VitalPower / PLAYER_VITAL_MAX));//バイタルの長さ
				vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			}
			//描画位置反映
			this->vtx[CntVital].Vertex2D(CntVitalTex, vtx);
		}
	}
}

//=============================================================================
// 再初期化処理　ネット対戦前
//=============================================================================
void VITALGAUGE::ReinitNet(int MyNumber)
{
	/*
	this[MyNumber].Pos(D3DXVECTOR3(VITALGAUGE_NET_POS_X, VITALGAUGE_NET_POS_Y, 0.0f));
	//座標取得
	D3DXVECTOR3 pos = this[MyNumber].Pos();

	//頂点座標の設定
	//BASE
	this[MyNumber].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	this[MyNumber].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X*2, pos.y, 0.0f);
	this[MyNumber].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x, pos.y + VITALGAUGE_SIZE_Y*2, 0.0f);
	this[MyNumber].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X*2, pos.y + VITALGAUGE_SIZE_Y*2, 0.0f);


	//中身
	//バイタルの長さ
	float vitallen = float(VITALGAUGE_SIZE_X*2 * (this[MyNumber].VitalPower / PLAYER_VITAL_MAX));
	// 頂点座標の設定
	this[MyNumber].Tex[0].textureVTX[0].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[MyNumber].Tex[0].textureVTX[1].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[MyNumber].Tex[0].textureVTX[2].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y*2 - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[MyNumber].Tex[0].textureVTX[3].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y*2 - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	*/
}

//=============================================================================
// 更新処理
//=============================================================================
void VITALGAUGE::Update(PLAYER_HONTAI *p, RANK *rank, bool Netflag, int NetMyNumber)
{
	//中身更新
	//ローカル対戦時
	if (Netflag == false)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
		{
			//バイタル記録
			this->VitalGaugePara[CntPlayer].VitalPower = p->PlayerPara[CntPlayer].vital;

			//プレイヤーが生きているときにバイタルが0以下で未使用にする（死亡フラグ）　負けた時点での順位もセットする
			bool puse = p->iUseType[CntPlayer].Use();
			if (puse == true)
			{
				if (this->VitalGaugePara[CntPlayer].VitalPower <= 0)
				{
					p->iUseType[CntPlayer].Use(false);
					rank->SetRank(CntPlayer);
				}
			}

			//座標取得
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos();

			//バイタルの長さ
			float vitallen = float(VITALGAUGE_SIZE_X) * float(this->VitalGaugePara[CntPlayer].VitalPower) / float(PLAYER_VITAL_MAX);

			// 頂点座標の設定
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this->vtx[CntPlayer].Vertex2D(1, vtx);//中身は1
		}
	}

	//ネット対戦時
	else
	{
		//バイタル記録
		this->VitalGaugePara[NetMyNumber].VitalPower = p->PlayerPara[NetMyNumber].vital;

		//プレイヤーが生きているときにバイタルが0以下で未使用にする（死亡フラグ）　負けた時点での順位もセットする
		for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
		{
			bool puse = p->iUseType[CntPlayer].Use();
			if (puse == true)
			{
				if (this->VitalGaugePara[CntPlayer].VitalPower <= 0)
				{
					p->iUseType[CntPlayer].Use(false);
					rank->SetRankNet(CntPlayer, NetMyNumber);
				}
			}
		}
		//座標取得
		D3DXVECTOR3 pos = this->Transform[NetMyNumber].Pos();

		//バイタルの長さ
		float vitallen = float(VITALGAUGE_SIZE_X) * 2 * float(this->VitalGaugePara[NetMyNumber].VitalPower) / float(PLAYER_VITAL_MAX);

		// 頂点座標の設定
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
		vtx[0] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		vtx[1] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		vtx[2] = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		vtx[3] = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		this->vtx[NetMyNumber].Vertex2D(1, vtx);//中身は1
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void VITALGAUGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//ローカル対戦時
	if (Netflag == false)
	{
		//枠描画ー＞中身描画
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, *this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
		pDevice->SetTexture(0, this->tex[0].Texture());
		//BASEを描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		// テクスチャの設定
		if (this->VitalGaugePara[CntPlayer].VitalPower >= VITAL_GREEN_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_GREEN].Texture());
		else if (this->VitalGaugePara[CntPlayer].VitalPower >= VITAL_ORANGE_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_ORANGE].Texture());
		else pDevice->SetTexture(0, this->tex[VITAL_TEX_RED].Texture());
		// 中身を描画　引数二個目の描画開始頂点を設定することが大事
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, POLYGON_2D_NUM);
	}
	//ネット対戦時
	else
	{
		//枠描画ー＞中身描画
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, *this->vtx[NetMyNumber].VtxBuff(), 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
		pDevice->SetTexture(0, this->tex[0].Texture());
		//BASEを描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, POLYGON_2D_NUM);
		// テクスチャの設定
		if (this->VitalGaugePara[NetMyNumber].VitalPower >= VITAL_GREEN_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_GREEN].Texture());
		else if (this->VitalGaugePara[NetMyNumber].VitalPower >= VITAL_ORANGE_DIAMETER * PLAYER_VITAL_MAX)
			pDevice->SetTexture(0, this->tex[VITAL_TEX_ORANGE].Texture());
		else pDevice->SetTexture(0, this->tex[VITAL_TEX_RED].Texture());
		// 中身を描画　引数二個目の描画開始頂点を設定することが大事
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, POLYGON_2D_NUM);
	}
}

