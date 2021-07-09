/**
* @file this.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../h/object/status.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	STATUS_SIZE_X			(20.0f)							// ステータスの幅
#define	STATUS_SIZE_Xhalf		(STATUS_SIZE_X/2)				// ステータスの幅 半分
#define	STATUS_SIZE_Y			(20.0f)							// ステータスの高さ
#define	STATUS_SIZE_Yhalf		(STATUS_SIZE_Y/2)				// ステータスの高さ 半分
#define	STATUS_SIZE_X_OFFSET	(0.8f)							// ステータスの幅
#define	STATUS_SIZE_Y_OFFSET	(0.8f)							// ステータスの高さ
#define	STATUS_POS_X			(SCREEN_CENTER_X)				// ステータスの表示位置
#define	STATUS_POS_Y			(SCREEN_CENTER_Y)				// ステータスの表示位置
#define	STATUS_POS_X_OFFSET		(220.0f)						// ステータスの表示位置オフセット
#define	STATUS_POS_Y_OFFSET		(80.0f)							// ステータスの表示位置オフセット
#define	STATUS_POS_X_OFFSETBUFF	(1.0f)							// ステータスの表示位置オフセット

#define STATUS_NET_POS_X_OFFSET		(260.0f)
#define STATUS_NET_POS_Y_OFFSET		(48.0f)

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
STATUS::STATUS(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成
	this->vtx[PLAYER01].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);
	this->vtx[PLAYER02].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);
	this->vtx[PLAYER03].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);
	this->vtx[PLAYER04].MakeVertex2D(OBJECT_STATUS_MAX, FVF_VERTEX_2D);

	//描画位置設定
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(STATUS_POS_X - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_POS_X_OFFSET, STATUS_POS_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	//カウントループ　ステータス数とプレイヤー数の二重ループ
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			//描画位置反映
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos;
			//Halfサイズ+(オフセット値*ステータスカウント)
			this->vtx[CntPlayer].SetVertex(CntStatus, STATUS_SIZE_Xhalf + CntStatus*(STATUS_SIZE_Xhalf + STATUS_POS_X_OFFSETBUFF), STATUS_SIZE_Yhalf, pos);

			//テクスチャ描画トリミング初期値　表示非表示もここで設定できる
			this->PtternV[CntPlayer][CntStatus] = 0.0f;
		}
		// テクスチャの読み込み
		this->tex[CntStatus].LoadTexture(this->c_FileNameTex[CntStatus]);
	}
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
STATUS::~STATUS(void)
{
	//カウントループ
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX*OBJECT_PLAYER_MAX; CntStatus++)
	{
		//テクスチャ解放
		this->tex[CntStatus].~TEXTURE();
	}
	//カウントループ
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//頂点解放
		this->vtx[CntPlayer].~VTXBuffer();
	}
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void STATUS::Init(void)
{
	//描画UVを設定する
}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::ReinitNet(int MyNumber)
{
	/*
		float buffsize = 48.0f;
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));
		this[MyNumber].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET * 2, 0.0f));

		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			//-----------------------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = this[MyNumber].Pos();

			//Base
			// 頂点座標の設定
			this[MyNumber].TexEmpty[CntStatus].textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2, 0.0f);
			this[MyNumber].TexEmpty[CntStatus].textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2, 0.0f);
			this[MyNumber].TexEmpty[CntStatus].textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2, 0.0f);
			this[MyNumber].TexEmpty[CntStatus].textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2, 0.0f);

			//中身
			// 頂点座標の設定
			this[MyNumber].Tex[CntStatus].textureVTX[0].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
			this[MyNumber].Tex[CntStatus].textureVTX[1].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y - STATUS_SIZE_Y * 2.0f, 0.0f);
			this[MyNumber].Tex[CntStatus].textureVTX[2].vtx = D3DXVECTOR3(pos.x - STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
			this[MyNumber].Tex[CntStatus].textureVTX[3].vtx = D3DXVECTOR3(pos.x + STATUS_SIZE_X * 2.0f + (CntStatus*(STATUS_SIZE_X + STATUS_POS_X_OFFSETBUFF) * 2.0f * 2), pos.y + STATUS_SIZE_Y * 2.0f, 0.0f);
		}
		*/

}

//=============================================================================
// 更新処理
//=============================================================================
void STATUS::Update(PLAYER_HONTAI *p)
{
	//描画UVを設定する
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//スピード
		if (p[CntPlayer].PlayerPara.speedbuffsignal == true) this[CntPlayer].use[STATUSTYPE_SPEED] = true;
		else this[CntPlayer].use[STATUSTYPE_SPEED] = false;
		//戦車
		if (p[CntPlayer].ModelType == PLAYER_MODEL_ATTACK) this[CntPlayer].use[STATUSTYPE_SENSYA] = true;
		else this[CntPlayer].use[STATUSTYPE_SENSYA] = false;
		//カメラ
		if (p[CntPlayer].PlayerPara.BackCameraItemSignal == true) this[CntPlayer].use[STATUSTYPE_CAMERA] = true;
		else this[CntPlayer].use[STATUSTYPE_CAMERA] = false;
		//霧
		if (p[CntPlayer].PlayerPara.KiriSignal == true) this[CntPlayer].use[STATUSTYPE_KIRI] = true;
		else this[CntPlayer].use[STATUSTYPE_KIRI] = false;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void STATUS::Draw(bool Netflag, int NetMyNumber,int CntPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル対戦
	if (Netflag == false)
	{
		//カウントループ
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, *this->vtx[CntPlayer].GetVtxBuff(), 0, sizeof(VERTEX_2D));
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);
				// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
				pDevice->SetTexture(0, this->tex[CntStatus].GetTexture());
				// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4), POLYGON_2D_NUM);
		}
	}

	//ネット対戦
	else
	{
		//カウントループ
		for (int CntStatus = 0; CntStatus < STATUSTYPE_MAX; CntStatus++)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, *this->vtx[NetMyNumber].GetVtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[CntStatus].GetTexture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4), POLYGON_2D_NUM);
		}
	}
}

