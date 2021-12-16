/**
* @file this.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../../h/main.h"
#include "../../../../h/Object/Player/player.h"
#include "../../../../h/Net/sock.h"
#include "../../../../h/Draw/Draw.h"
#include "../../../../h/Object/Player/Status/status.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float	STATUS_SIZE_X{ 40.0f };						// ステータスの幅
constexpr float	STATUS_SIZE_Y{ 40.0f };						// ステータスの高さ
constexpr float	STATUS_SIZE_X_NET{ STATUS_SIZE_X * 2 };		// ステータスの幅
constexpr float	STATUS_SIZE_Y_NET{ STATUS_SIZE_X * 2 };		// ステータスの高さ
constexpr float	STATUS_SIZE_X_OFFSET{ 5.0f };				// ステータスの幅
constexpr float	STATUS_POS_X_OFFSET{ 240.0f };				// ステータスの表示位置オフセット
constexpr float	STATUS_POS_Y_OFFSET{ 100.0f };				// ステータスの表示位置オフセット
constexpr float	STATUS_POS_X_OFFSETBUFF{ 1.0f };			// ステータスの表示位置オフセット
constexpr int	STATUS_POS_X{ SCREEN_CENTER_X };			// ステータスの表示位置
constexpr int	STATUS_POS_Y{ SCREEN_CENTER_Y };			// ステータスの表示位置

constexpr float STATUS_NET_POS_X_OFFSET{ 260.0f };			// ステータスの表示位置オフセット　ネット用
constexpr float STATUS_NET_POS_Y_OFFSET{ 150.0f };			// ステータスの表示位置オフセット　ネット用

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
STATUS::STATUS(void)
{
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
		this->vtx[CntPlayer].~VTXBUFFER();
	}
}

//=============================================================================
// 他クラスのアドレス取得
//=============================================================================
void STATUS::Addressor(GAME_OBJECT_INSTANCE *obj)
{
	pplayer = obj->GetPlayer();
	pmysocket = obj->GetMySocket();
	pDrawManager = obj->GetDrawManager();
}

//=============================================================================
// 初期化処理
//=============================================================================
void STATUS::Init(void)
{
	//カウントループ　ステータス数とプレイヤー数の二重ループ
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
		{
			//描画位置反映
			D3DXVECTOR3 pos = this->Transform[CntPlayer].Pos();
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
			vtx[0] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET), pos.y, 0.0f);
			vtx[1] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y, 0.0f);
			vtx[2] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET), pos.y + STATUS_SIZE_Y, 0.0f);
			vtx[3] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X, pos.y + STATUS_SIZE_Y, 0.0f);

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
	}

}

//=============================================================================
// 再初期化処理
//=============================================================================
void STATUS::InitNet(void)
{
	//サイズ調整用
	float buffsize = 48.0f;
	//描画位置設定
	this->Transform[pmysocket->GetNetMyNumber()].Pos(D3DXVECTOR3(STATUS_POS_X * 2 - STATUS_NET_POS_X_OFFSET * 2 + buffsize, STATUS_POS_Y * 2 - STATUS_NET_POS_Y_OFFSET, 0.0f));

	//カウントループ　ステータス数
	for (int CntStatus = 0; CntStatus < OBJECT_STATUS_MAX; CntStatus++)
	{
		//描画位置反映
		D3DXVECTOR3 pos = this->Transform[pmysocket->GetNetMyNumber()].Pos();
		D3DXVECTOR3 vtx[POLYGON_2D_VERTEX];
		vtx[0] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET), pos.y, 0.0f);
		vtx[1] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X_NET, pos.y, 0.0f);
		vtx[2] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET), pos.y + STATUS_SIZE_Y_NET, 0.0f);
		vtx[3] = D3DXVECTOR3(pos.x + (CntStatus*STATUS_SIZE_X_NET + STATUS_SIZE_X_OFFSET) + STATUS_SIZE_X_NET, pos.y + STATUS_SIZE_Y_NET, 0.0f);

		//描画位置反映
		this->vtx[pmysocket->GetNetMyNumber()].Vertex2D(CntStatus, vtx);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void STATUS::Update(void)
{
	//カウントループ　プレイヤー数の二重ループ
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//speed
		if (pplayer->PlayerPara[CntPlayer].ItemPara.GetSpeedBuffSignal() == true)
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
		if (pplayer->PlayerPara[CntPlayer].StandardPara.eModelType == PLAYER_MODEL_TYPE_ATTACK)
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
		if (pplayer->PlayerPara[CntPlayer].ItemPara.GetBackCameraItemSignal() == true)
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
		if (pplayer->PlayerPara[CntPlayer].ItemPara.GetKiriSignal() == true)
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
void STATUS::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル対戦
	if (pmysocket->GetNetGameStartFlag() == false)
	{
		//カウントループ
		for (int CntStatus = 0; CntStatus < STATUS_TYPE_MAX; CntStatus++)
		{
			//if (this->iUseType[CntPlayer][CntStatus].Use() == YesUseType1)
			{
				// 頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, this->vtx[pDrawManager->GetDrawManagerNum()].VtxBuff(), 0, sizeof(VERTEX_2D));
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
			pDevice->SetStreamSource(0, this->vtx[pmysocket->GetNetMyNumber()].VtxBuff(), 0, sizeof(VERTEX_2D));
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);
			// テクスチャの設定　テクスチャが複数ならtexを配列化して選択させるように
			pDevice->SetTexture(0, this->tex[CntStatus].Texture());
			// ポリゴンの描画　引数二個目の描画開始頂点を設定することが大事
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntStatus * 4), POLYGON_2D_NUM);
		}
	}
}

