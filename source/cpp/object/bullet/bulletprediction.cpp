/**
* @file bulletprediction.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../../h/main.h"
#include "../../../h/other/input.h"
#include "../../../h/object/camera.h"
#include "../../../h/object/player.h"
#include "../../../h/object/objectclass.h"
#include "../../../h/object/bullet/bulletprediction.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETPREDICTION		"../data/TEXTURE/effect/effect000.jpg"	// 読み込むテクスチャファイル名
#define	BULLETPREDICTION_SIZE_X			(5.0f)							// ビルボードの幅
#define	BULLETPREDICTION_SIZE_Y			(5.0f)							// ビルボードの高さ

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.1f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.1f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.1f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 0.1f),//p4カラー
};

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
BULLETPREDICTION::BULLETPREDICTION(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//頂点の作成 全体400　各100作成
	this->vtx[PLAYER01].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);
	this->vtx[PLAYER02].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);
	this->vtx[PLAYER03].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);
	this->vtx[PLAYER04].MakeVertex3DBill(OBJECT_BULLETPREDICTION_MAX/PLAYER_MAX, FVF_VERTEX_3D);

	//カウントループ　プレイヤー
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//カウントループ　弾道予測エフェクト
		for (int CntBulletprediction = 0; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			// 頂点座標の設定
			D3DXVECTOR3 vtx[POLYGON_2D_VERTEX] =
			{
			D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			D3DXVECTOR3(-BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			D3DXVECTOR3(BULLETPREDICTION_SIZE_X / 2, -BULLETPREDICTION_SIZE_Y / 2, 0.0f),
			};
			this->vtx[CntPlayer].Vertex3D(CntBulletprediction, vtx);

			//RHW設定
			this->vtx[CntPlayer].Nor3D(CntBulletprediction);

			//UVの設定
			this->vtx[CntPlayer].UV3D(CntBulletprediction);

			//カラー設定
			this->vtx[CntPlayer].Color3D(CntBulletprediction, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_BULLETPREDICTION);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
BULLETPREDICTION::~BULLETPREDICTION(void)
{
	//テクスチャ解放
	this->tex.~TEXTURE();
	//カウントループ　プレイヤー
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
void BULLETPREDICTION::Init(void)
{
	//カウントループ　プレイヤー
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//カウントループ　弾道予測エフェクト
		for (int CntBulletprediction = 0; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			this->Transform[CntPlayer][CntBulletprediction].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			this->Transform[CntPlayer][CntBulletprediction].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			this->vtx[CntPlayer].Color3D(CntBulletprediction, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void BULLETPREDICTION::Update(PLAYER *player)
{
	//プレイヤーの情報から発射位置角度移動量を利用してバレットの着弾点を算出する
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//---------------------------------オブジェクト値読み込み
		D3DXVECTOR3	BulletPredictionPos = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();
		BulletPredictionPos.y += 20.0f;
		D3DXVECTOR3 HoudaiRot = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUDAI].Rot();
		D3DXVECTOR3 HoutouRot = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUTOU].Rot();
		D3DXVECTOR3 HousinRot = player->modelDraw[CntPlayer].Transform[PLAYER_PARTS_TYPE_HOUSIN].Rot();

		//発射角度、発射座用計算
		D3DXVECTOR3 BmoveRot;
		BmoveRot.x = -sinf(HoutouRot.y + HoudaiRot.y);
		BmoveRot.y = sinf(player->PlayerPara[CntPlayer].BulletPara.BulletRotY - HousinRot.x);
		BmoveRot.z = -cosf(HoutouRot.y + HoudaiRot.y);
		D3DXVECTOR3 bulletmove;
		bulletmove.x = (BmoveRot.x) *VALUE_MOVE_BULLET;
		bulletmove.y = (BmoveRot.y) *VALUE_MOVE_BULLET;
		bulletmove.z = (BmoveRot.z) *VALUE_MOVE_BULLET;

		D3DXCOLOR	col = PLAYER_COLOR[CntPlayer];
		float Gravity = 0.0f;
		//float time = 1.0f;
		//float maxtime = 10.0f;

		for (int CntBulletprediction = 0 ; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			BulletPredictionPos.x += bulletmove.x;
			BulletPredictionPos.y -= bulletmove.y + Gravity;
			BulletPredictionPos.z += bulletmove.z;

			//重力を加速させる
			Gravity += VALUE_GRAVITYADD_BULLET;
			//時間割合を進める。飛距離が増えるほど弾道予測間隔が広くなる
			//time++;
			//重力最大値制限
			if (Gravity > VALUE_GRAVITYMAX_BULLET) Gravity = VALUE_GRAVITYMAX_BULLET;
			//徐々にアルファ値を強くして遠距離地点を見やすくする
			col.a += 0.01f;
			this->UpdateInstance(BulletPredictionPos, CntPlayer, col, BULLETPREDICTION_SIZE_X, BULLETPREDICTION_SIZE_Y, CntBulletprediction);


		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void BULLETPREDICTION::Draw(PLAYER *player, int CntPlayer)
{
	bool puse = player->iUseType[CntPlayer].Use();
	if (puse)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		for (int CntBulletprediction = 0; CntBulletprediction < BULLETPREDICTION_MAX; CntBulletprediction++)
		{
			D3DXMATRIX mtxView, mtxScale, mtxTranslate;

			//---------------------------------オブジェクト値読み込み
			D3DXVECTOR3	pos = this->Transform[CntPlayer][CntBulletprediction].Pos();
			D3DXVECTOR3	scl = this->Transform[CntPlayer][CntBulletprediction].Scl();
			D3DXMATRIX mtxWorld;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorld);

			// ビューマトリックスを取得
			CAMERA *cam = GetCamera();

			mtxWorld._11 = cam[CntPlayer].mtxView._11;
			mtxWorld._12 = cam[CntPlayer].mtxView._21;
			mtxWorld._13 = cam[CntPlayer].mtxView._31;
			mtxWorld._21 = cam[CntPlayer].mtxView._12;
			mtxWorld._22 = cam[CntPlayer].mtxView._22;
			mtxWorld._23 = cam[CntPlayer].mtxView._32;
			mtxWorld._31 = cam[CntPlayer].mtxView._13;
			mtxWorld._32 = cam[CntPlayer].mtxView._23;
			mtxWorld._33 = cam[CntPlayer].mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, this->vtx[CntPlayer].VtxBuff(), 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, this->tex.Texture());

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntBulletprediction * 4), POLYGON_2D_NUM);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//=============================================================================
// インスタンスの設定
//=============================================================================
void BULLETPREDICTION::UpdateInstance(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY,int CntBulletPrediction)
{
	this->Transform[PlayerType][CntBulletPrediction].Pos(pos);
	this->Transform[PlayerType][CntBulletPrediction].Scl(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	this->vtx[PlayerType].Color3D(CntBulletPrediction,col);
}
