/**
* @file bulletgauge.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../../h/object/bullet/bulletgauge.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETGAUGE			"../data/TEXTURE/UI/UI_Shell.png"			// 読み込むテクスチャファイル名
#define	TEXTURE_BULLETGAUGE_EMPTY	"../data/TEXTURE/UI/UI_Shell_Empty.png"		// 読み込むテクスチャファイル名
#define	BULLETGAUGE_SIZE_X			(15.0f)						// 数字の幅
#define	BULLETGAUGE_SIZE_Y			(20.0f)						// 数字の高さ
#define	BULLETGAUGE_SIZE_X_OFFSET	(0.8f)						// アタックゲージの数字の幅
#define	BULLETGAUGE_SIZE_Y_OFFSET	(0.8f)						// アタックゲージの数字の高さ
#define	BULLETGAUGE_POS_X			(570.0f)					// 表示基準位置Ｘ座標
#define	BULLETGAUGE_POS_Y			(SCREEN_CENTER_Y/5)			// 表示基準位置Ｙ座標
#define	BULLETGAUGE_POS_X_OFFSET	(20.0f)						// 表示基準位置Ｘ座標

#define BULLETGAUGE_P1_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P1_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P2_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P2_POS_Y		SCREEN_CENTER_Y - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P3_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P3_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_P4_POS_X		SCREEN_W - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X
#define BULLETGAUGE_P4_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y
#define BULLETGAUGE_NET_POS_X		SCREEN_CENTER_X - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_X+55.0f
#define BULLETGAUGE_NET_POS_Y		SCREEN_H - SCREEN_SEPARATE_BUFF - BULLETGAUGE_POS_Y

#define PLAYER_AMMOSTOCK		(10)	//!< 弾のストック　

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
BULLETGAUGE::BULLETGAUGE(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx[PLAYER01].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);	//!< *2は枠テクスチャと本体テクスチャの2種類必要だから
	this->vtx[PLAYER02].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);
	this->vtx[PLAYER03].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);
	this->vtx[PLAYER04].MakeVertex2D(PLAYER_AMMOSTOCK*2, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(BULLETGAUGE_P1_POS_X, BULLETGAUGE_P1_POS_Y, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(BULLETGAUGE_P2_POS_X, BULLETGAUGE_P2_POS_Y, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(BULLETGAUGE_P3_POS_X, BULLETGAUGE_P3_POS_Y, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(BULLETGAUGE_P4_POS_X, BULLETGAUGE_P4_POS_Y, 0.0f));

	//カウントループ プレイヤー数(ゲージ数)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//ベース座標取得
		D3DXVECTOR3 pos = this->Transform[CntBulletGauge].Pos;
		//カウントループ 弾数
		for (int CntAmmoStock = 0; CntAmmoStock < PLAYER_AMMOSTOCK*2; CntAmmoStock++)
		{
			//「枠」頂点と「中身」頂点で別の処理をする
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			//枠
			if (CntAmmoStock < PLAYER_AMMOSTOCK)//0-9の範囲
			{
				vtx[0] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y_OFFSET, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmoStock*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
			}
			//中身
			else
			{
				//オフセット用ループカウント
				int cnt = CntAmmoStock - PLAYER_AMMOSTOCK;
				vtx[0] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET), pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET), pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + (cnt*BULLETGAUGE_POS_X_OFFSET) + BULLETGAUGE_SIZE_X, pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
			}
			//描画位置反映
			this->vtx[CntBulletGauge].Vertex2D(CntAmmoStock, vtx);

			//RHW設定
			this->vtx[CntBulletGauge].RHW2D(CntAmmoStock);

			//UVの設定
			this->vtx[CntBulletGauge].UV2D(CntAmmoStock);

			//カラー設定
			this->vtx[CntBulletGauge].Color2D(CntAmmoStock, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	// テクスチャの読み込み
	this->tex[BULLETGAUGE_TEX_BASE].LoadTexture(TEXTURE_BULLETGAUGE_EMPTY);
	this->tex[BULLETGAUGE_TEX_SHELL].LoadTexture(TEXTURE_BULLETGAUGE);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
BULLETGAUGE::~BULLETGAUGE(void)
{
	//テクスチャ解放
	this->tex[0].~TEXTURE();
	this->tex[1].~TEXTURE();
	//カウントループ プレイヤー数(バレットゲージ数)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//頂点解放
		this->vtx[CntBulletGauge].~VTXBuffer();
	}
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void BULLETGAUGE::Init(void)
{
	//カウントループ プレイヤー数(バレットゲージ数)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//初期設定は弾薬満タンからスタート
		this->BulletGaugePara[CntBulletGauge].AmmoStock = PLAYER_AMMOSTOCK;
	}
}

//=============================================================================
// 再初期化処理　ネット対戦前
//=============================================================================
void BULLETGAUGE::ReinitNet(int MyNumber)
{
	/*
	this[MyNumber].Pos(D3DXVECTOR3(BULLETGAUGE_NET_POS_X, BULLETGAUGE_NET_POS_Y, 0.0f));

	//座標取得
	D3DXVECTOR3 pos = this[MyNumber].Pos();

	for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOPOWER_NORMAL; CntAmmo++)
	{
		//BASE設定
		// 頂点座標の設定
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[0].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[1].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X*2, pos.y - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[2].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].TexEmpty[CntAmmo].textureVTX[3].vtx = D3DXVECTOR3(pos.x + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y + BULLETGAUGE_SIZE_Y, 0.0f);

		//中身設定
		// 頂点座標の設定
		this[MyNumber].Tex[CntAmmo].textureVTX[0].vtx = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y_OFFSET - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].Tex[CntAmmo].textureVTX[1].vtx = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y_OFFSET - BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].Tex[CntAmmo].textureVTX[2].vtx = D3DXVECTOR3(pos.x + BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2), pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
		this[MyNumber].Tex[CntAmmo].textureVTX[3].vtx = D3DXVECTOR3(pos.x - BULLETGAUGE_SIZE_X_OFFSET + (CntAmmo*BULLETGAUGE_POS_X_OFFSET * 2) + BULLETGAUGE_SIZE_X * 2, pos.y + BULLETGAUGE_SIZE_Y - BULLETGAUGE_SIZE_Y_OFFSET + BULLETGAUGE_SIZE_Y, 0.0f);
	}
	*/
}

//=============================================================================
// 更新処理
//=============================================================================
void BULLETGAUGE::Update(PLAYER_HONTAI *player)
{
	//カウントループ プレイヤー数(バレットゲージ数)
	for (int CntBulletGauge = 0; CntBulletGauge < OBJECT_BULLETGAUGE_MAX; CntBulletGauge++)
	{
		//プレイヤーの弾数を記録
		this->BulletGaugePara[CntBulletGauge].AmmoStock = player->PlayerPara[CntBulletGauge].AmmoStock;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void BULLETGAUGE::Draw(bool Netflag, int NetMyNumber, int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル対戦時
	if (Netflag == false)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, *this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		//カウントループ　枠の数
		for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOSTOCK; CntAmmo++)
		{
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_BASE].Texture());//枠なので[0]
			// 枠を描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4), POLYGON_2D_NUM);
		}
		//カウントループ　中身の数
		for (int CntAmmo = 0; CntAmmo < this->BulletGaugePara[CntPlayer].AmmoStock; CntAmmo++)
		{
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_SHELL].Texture());		//枠なので[1]
			// 中身を描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4)+ PLAYER_AMMOSTOCK, POLYGON_2D_NUM);
		}
	}
	//ネット対戦時
	else
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, *this->vtx[NetMyNumber].VtxBuff(), 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		//カウントループ　枠の数
		for (int CntAmmo = 0; CntAmmo < PLAYER_AMMOSTOCK; CntAmmo++)
		{
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_BASE].Texture());//枠なので[0]
			// 枠を描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4), POLYGON_2D_NUM);
		}
		//カウントループ　中身の数
		for (int CntAmmo = 0; CntAmmo < this->BulletGaugePara[NetMyNumber].AmmoStock; CntAmmo++)
		{
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[BULLETGAUGE_TEX_SHELL].Texture());		//枠なので[1]
			// 中身を描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntAmmo * 4) + PLAYER_AMMOSTOCK, POLYGON_2D_NUM);
		}
	}
}

