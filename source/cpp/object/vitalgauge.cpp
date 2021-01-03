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
#define	TEXTURE_VITALGAUGE_BASE		"../data/TEXTURE/UI/UI_HP_Base.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_VITALGAUGE_GREEN	"../data/TEXTURE/UI/UI_HP_Green.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_VITALGAUGE_ORANGE	"../data/TEXTURE/UI/UI_HP_Orange.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_VITALGAUGE_RED		"../data/TEXTURE/UI/UI_HP_Red.png"		// 読み込むテクスチャファイル名

#define	VITALGAUGE_SIZE_X			(500.0f)							// アタックゲージの数字の幅
#define	VITALGAUGE_SIZE_Y			(20.0f)							// アタックゲージの数字の高さ
#define	VITALGAUGE_SIZE_X_OFFSET	(0.8f)							// アタックゲージの数字の幅
#define	VITALGAUGE_SIZE_Y_OFFSET	(0.8f)							// アタックゲージの数字の高さ
#define	VITALGAUGE_BASE_POS_X		(570.0f)							// アタックゲージの表示基準位置Ｘ座標
#define	VITALGAUGE_BASE_POS_Y		(SCREEN_CENTER_Y/10)							// アタックゲージの表示基準位置Ｙ座標

#define VITALGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X
#define VITALGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y
#define VITALGAUGE_NET_POS_X	SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_X+55.0f
#define VITALGAUGE_NET_POS_Y	SCREEN_H - SCREEN_SEPARATE_BUFF - VITALGAUGE_BASE_POS_Y

#define	VITAL_GREEN_DIAMETER			(0.5f)
#define	VITAL_ORANGE_DIAMETER			(0.2f)
#define	VITAL_RED_DIAMETER				(0.1f)

//=============================================================================
// 初期化処理
//=============================================================================
void VITALGAUGE::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//座標セット
	this[0].SetPos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));


	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					
			TEXTURE_VITALGAUGE_BASE,			
			&this[CntPlayer].Tex[VITALTEX_BASE].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,					
			TEXTURE_VITALGAUGE_GREEN,
			&this[CntPlayer].Tex[VITALTEX_GREEN].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,					
			TEXTURE_VITALGAUGE_ORANGE,
			&this[CntPlayer].Tex[VITALTEX_ORANGE].pD3DTexture);
		D3DXCreateTextureFromFile(pDevice,					
			TEXTURE_VITALGAUGE_RED,
			&this[CntPlayer].Tex[VITALTEX_RED].pD3DTexture);
		// ライフの初期化
		this[CntPlayer].VitalPower = PLAYER_VITAL_MAX;

		// 頂点情報の作成
		MakeVertexVitalGauge(pDevice, CntPlayer);
	}
}

//=============================================================================
// 再初期化処理
//=============================================================================
void VITALGAUGE::Reinit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//座標セット
	this[0].SetPos(D3DXVECTOR3(VITALGAUGE_P1_POS_X, VITALGAUGE_P1_POS_Y, 0.0f));
	this[1].SetPos(D3DXVECTOR3(VITALGAUGE_P2_POS_X, VITALGAUGE_P2_POS_Y, 0.0f));
	this[2].SetPos(D3DXVECTOR3(VITALGAUGE_P3_POS_X, VITALGAUGE_P3_POS_Y, 0.0f));
	this[3].SetPos(D3DXVECTOR3(VITALGAUGE_P4_POS_X, VITALGAUGE_P4_POS_Y, 0.0f));

	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		// ライフの初期化
		this[CntPlayer].VitalPower = PLAYER_VITAL_MAX;

		// 頂点情報の作成
		MakeVertexVitalGauge(pDevice, CntPlayer);
	}
}

//=============================================================================
// 再初期化処理　ネット対戦前
//=============================================================================
void VITALGAUGE::ReinitNet(int MyNumber)
{

	this[MyNumber].SetPos(D3DXVECTOR3(VITALGAUGE_NET_POS_X, VITALGAUGE_NET_POS_Y, 0.0f));
	//座標取得
	D3DXVECTOR3 pos = this[MyNumber].GetPos();

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

}

//=============================================================================
// 終了処理
//=============================================================================
void VITALGAUGE::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void VITALGAUGE::Update(PLAYER_HONTAI *p, RANK *rank, bool Netflag, int NetMyNumber)
{
	for (int CntPlayer = 0; CntPlayer < OBJECT_VITAL_MAX; CntPlayer++)
	{
		//-----------------------------------オブジェクト先頭アドレスを読み込み
		bool puse = p[CntPlayer].GetUse();
		if (puse == true)
		{
			if (this[CntPlayer].VitalPower <= 0)
			{
				p[CntPlayer].SetUse(false);
				if (Netflag == false) rank->SetRank(CntPlayer);
				else if (Netflag == true) rank->SetRankNet(CntPlayer, NetMyNumber);
			}
		}

		this[CntPlayer].VitalPower = p[CntPlayer].vital;

		//中身設定
		if (Netflag == false)
		{
			//座標取得
			D3DXVECTOR3 pos = this[CntPlayer].GetPos();

			//バイタルの長さ
			float vitallen = float(VITALGAUGE_SIZE_X) * float(p[CntPlayer].vital) / float(PLAYER_VITAL_MAX);

			// 頂点座標の設定
			this[CntPlayer].Tex[0].textureVTX[0].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this[CntPlayer].Tex[0].textureVTX[1].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this[CntPlayer].Tex[0].textureVTX[2].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this[CntPlayer].Tex[0].textureVTX[3].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		}
		//中身設定
		else
		{
			//座標取得
			D3DXVECTOR3 pos = this[NetMyNumber].GetPos();

			//バイタルの長さ
			float vitallen = float(VITALGAUGE_SIZE_X)*2 * float(p[NetMyNumber].vital) / float(PLAYER_VITAL_MAX);

			// 頂点座標の設定
			this[NetMyNumber].Tex[0].textureVTX[0].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this[NetMyNumber].Tex[0].textureVTX[1].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this[NetMyNumber].Tex[0].textureVTX[2].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y*2 - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
			this[NetMyNumber].Tex[0].textureVTX[3].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y*2 - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void VITALGAUGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (Netflag==false)
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		//BASEを描画
			pDevice->SetTexture(0, this[CntPlayer].Tex[VITALTEX_BASE].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].tex2D.textureVTX, sizeof(VERTEX_2D));//ここは座標だから0
		//中身を描画
			// テクスチャの設定
			if (this[CntPlayer].VitalPower >= VITAL_GREEN_DIAMETER * PLAYER_VITAL_MAX) pDevice->SetTexture(0, this[CntPlayer].Tex[VITALTEX_GREEN].pD3DTexture);
			else if (this[CntPlayer].VitalPower >= VITAL_ORANGE_DIAMETER * PLAYER_VITAL_MAX) pDevice->SetTexture(0, this[CntPlayer].Tex[VITALTEX_ORANGE].pD3DTexture);
			else pDevice->SetTexture(0, this[CntPlayer].Tex[VITALTEX_RED].pD3DTexture);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[CntPlayer].Tex[0].textureVTX, sizeof(VERTEX_2D));//ここは座標だから0
	}
	else
	{
		//BASEを描画
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0, this[NetMyNumber].Tex[VITALTEX_BASE].pD3DTexture);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[NetMyNumber].tex2D.textureVTX, sizeof(VERTEX_2D));//ここは座標だから0

																														   //中身を描画
		// テクスチャの設定
		if (this[NetMyNumber].VitalPower >= VITAL_GREEN_DIAMETER * PLAYER_VITAL_MAX) pDevice->SetTexture(0, this[NetMyNumber].Tex[VITALTEX_GREEN].pD3DTexture);
		else if (this[NetMyNumber].VitalPower >= VITAL_ORANGE_DIAMETER * PLAYER_VITAL_MAX) pDevice->SetTexture(0, this[NetMyNumber].Tex[VITALTEX_ORANGE].pD3DTexture);
		else pDevice->SetTexture(0, this[NetMyNumber].Tex[VITALTEX_RED].pD3DTexture);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POLYGON_2D_NUM, this[NetMyNumber].Tex[0].textureVTX, sizeof(VERTEX_2D));//ここは座標だから0
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT VITALGAUGE::MakeVertexVitalGauge(LPDIRECT3DDEVICE9 pDevice, int CntPlayer)
{
	//座標取得
	D3DXVECTOR3 pos = this[CntPlayer].GetPos();
	//BASE設定
	// 頂点座標の設定
	this[CntPlayer].tex2D.textureVTX[0].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	this[CntPlayer].tex2D.textureVTX[1].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y, 0.0f);
	this[CntPlayer].tex2D.textureVTX[2].vtx = D3DXVECTOR3(pos.x, pos.y + VITALGAUGE_SIZE_Y, 0.0f);
	this[CntPlayer].tex2D.textureVTX[3].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X, pos.y + VITALGAUGE_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	this[CntPlayer].tex2D.textureVTX[0].rhw =
		this[CntPlayer].tex2D.textureVTX[1].rhw =
		this[CntPlayer].tex2D.textureVTX[2].rhw =
		this[CntPlayer].tex2D.textureVTX[3].rhw = 1.0f;

	// 反射光の設定
	this[CntPlayer].tex2D.textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	this[CntPlayer].tex2D.textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	this[CntPlayer].tex2D.textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	this[CntPlayer].tex2D.textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	this[CntPlayer].tex2D.textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	this[CntPlayer].tex2D.textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	this[CntPlayer].tex2D.textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	this[CntPlayer].tex2D.textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//中身設定
	//バイタルの長さ
	float vitallen = float(VITALGAUGE_SIZE_X * (this[CntPlayer].VitalPower / PLAYER_VITAL_MAX));
	// 頂点座標の設定
	this[CntPlayer].Tex[0].textureVTX[0].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[CntPlayer].Tex[0].textureVTX[1].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[CntPlayer].Tex[0].textureVTX[2].vtx = D3DXVECTOR3(pos.x + VITALGAUGE_SIZE_X_OFFSET, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	this[CntPlayer].Tex[0].textureVTX[3].vtx = D3DXVECTOR3(pos.x - VITALGAUGE_SIZE_X_OFFSET + vitallen, pos.y + VITALGAUGE_SIZE_Y - VITALGAUGE_SIZE_Y_OFFSET, 0.0f);
	// テクスチャのパースペクティブコレクト用
	this[CntPlayer].Tex[0].textureVTX[0].rhw =
		this[CntPlayer].Tex[0].textureVTX[1].rhw =
		this[CntPlayer].Tex[0].textureVTX[2].rhw =
		this[CntPlayer].Tex[0].textureVTX[3].rhw = 1.0f;

	// 反射光の設定
	this[CntPlayer].Tex[0].textureVTX[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	this[CntPlayer].Tex[0].textureVTX[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	this[CntPlayer].Tex[0].textureVTX[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	this[CntPlayer].Tex[0].textureVTX[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	this[CntPlayer].Tex[0].textureVTX[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	this[CntPlayer].Tex[0].textureVTX[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	this[CntPlayer].Tex[0].textureVTX[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	this[CntPlayer].Tex[0].textureVTX[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}