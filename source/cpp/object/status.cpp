/**
* @file this.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../h/main.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/player.h"
#include "../../h/object/status.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	STATUS_SIZE_X			(40.0f)							// ステータスの幅
#define	STATUS_SIZE_Y			(40.0f)							// ステータスの高さ
#define	STATUS_SIZE_X_OFFSET	(5.0f)							// ステータスの幅
#define	STATUS_POS_X_OFFSET		(240.0f)						// ステータスの表示位置オフセット
#define	STATUS_POS_Y_OFFSET		(100.0f)						// ステータスの表示位置オフセット
#define	STATUS_POS_X_OFFSETBUFF	(1.0f)							// ステータスの表示位置オフセット
#define	STATUS_POS_X			(SCREEN_CENTER_X)				// ステータスの表示位置
#define	STATUS_POS_Y			(SCREEN_CENTER_Y)				// ステータスの表示位置

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
	this->Transform[PLAYER01].Pos(D3DXVECTOR3(SCREEN_CENTER_X - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER02].Pos(D3DXVECTOR3(SCREEN_CENTER_X * 2 - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER03].Pos(D3DXVECTOR3(SCREEN_CENTER_X - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));
	this->Transform[PLAYER04].Pos(D3DXVECTOR3(SCREEN_CENTER_X * 2 - STATUS_POS_X_OFFSET, SCREEN_CENTER_Y * 2 - STATUS_POS_Y_OFFSET, 0.0f));

	//カウントループ　ステータス数とプレイヤー数の二重ループ
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			//描画位置反映
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos();
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
				vtx[0] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET), pos.y, 0.0f);
				vtx[1] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y, 0.0f);
				vtx[2] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET), pos.y + STATUS_SIZE_Y, 0.0f);
				vtx[3] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X+ STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y + STATUS_SIZE_Y, 0.0f);

			//描画位置反映
			this->vtx[CntPlayer].Vertex2D(CntStatus, vtx);

			//テクスチャ描画トリミング初期値　表示非表示もここで設定できる
			this->PtternV[CntPlayer][CntStatus] = 0.0f;

			//RHW設定
			this->vtx[CntPlayer].RHW2D(CntStatus);

			//UV設定
			this->vtx[CntPlayer].UV2D(CntStatus);

			//カラー設定
			this->vtx[CntPlayer].Color2D(CntStatus, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
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
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
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

}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::InitNet(int MyNumber)
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
void STATUS::Update(PLAYER *p)
{
	//カウントループ　プレイヤー数の二重ループ
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//speed
		if (p->PlayerPara[CntPlayer].ItemPara.SpeedBuffSignal == true)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SPEED].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SPEED, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SPEED].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SPEED, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
		//戦車
		if (p->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SENSYA].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SENSYA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_SENSYA].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_SENSYA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
		//カメラ
		if (p->PlayerPara[CntPlayer].ItemPara.BackCameraItemSignal == true)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_CAMERA].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_CAMERA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_CAMERA].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_CAMERA, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
		//霧
		if (p->PlayerPara[CntPlayer].ItemPara.KiriSignal == true)
		{
			this->iUseType[CntPlayer][STATUS_TYPE_KIRI].Use(YesUseType1);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_KIRI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->iUseType[CntPlayer][STATUS_TYPE_KIRI].Use(NoUse);
			this->vtx[CntPlayer].Color2D(STATUS_TYPE_KIRI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
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
		for (int CntStatus = 0; CntStatus < STATUS_TYPE_MAX; CntStatus++)
		{
			//if (this->iUseType[CntPlayer][CntStatus].Use() == YesUseType1)
			{
				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_2D));
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_2D);
				// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
				pDevice->SetTexture(0, this->tex[CntStatus].Texture());
				// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4) , POLYGON_2D_NUM);
			}
		}
	}

	//ネット対戦
	else
	{
		//カウントループ
		for (int CntStatus = 0; CntStatus < STATUS_TYPE_MAX; CntStatus++)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this->vtx[NetMyNumber].VtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[CntStatus].Texture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4), POLYGON_2D_NUM);
		}
	}
}

