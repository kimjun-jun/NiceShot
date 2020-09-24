/**
* @file item.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/map/field.h"
#include "../../h/other/sound.h"
#include "../../h/object/item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.01f)			// 回転速度
#define	VALUE_FALLSPEED_ITEM	(2.0f)						// 落下速度
#define	ITEM_RADIUS				(20.0f)						// 半径
#define DROP_ITEM_MAX						(20)																		//!< フィールドに落ちてるアイテムの数
#define DROP_ITEM_CHARGE_ADDTIME			(1.0f)																		//!< アイテムをリスポーンさせる時の加算タイム
#define DROP_ITEM_CHARGE_CNT				(60.0f)																		//!< アイテムをリスポーンさせる時の所要タイム

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureItem[ITEMTYPE_MAX];	// テクスチャ読み込み場所
LPD3DXMESH			g_pMeshItem[ITEMTYPE_MAX];			// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		g_pD3DXMatBuffItem[ITEMTYPE_MAX];	// メッシュのマテリアル情報を格納
DWORD				g_aNumMatItem[ITEMTYPE_MAX];			// 属性情報の総数

const char *c_aFileNameItemModel[ITEMTYPE_MAX] =
{
	"../data/MODEL/tikeiItem.x",		// 地形変形アイテム
	"../data/MODEL/lifeItem.x",		// ライフ回復
	"../data/MODEL/sensyaItem.x",		// 戦車変形アイテム
	"../data/MODEL/bulletItem.x",		// バレットアイテム
	"../data/MODEL/speedItem.x",		// スピードアップアイテム
	"../data/MODEL/cameraItem.x",		// 強制バックカメラアイテム
	"../data/MODEL/kiriItem.x",		// 霧アイテム
};

const char *c_aFileNameItemTex[ITEMTYPE_MAX] =
{
	"../data/MODEL/landmark_aogashima.png",		// 地形変形アイテム
	"../data/MODEL/life000.png",					// ライフ回復
	"../data/MODEL/war_sensya_noman.png",			// 戦車変形アイテム
	"../data/MODEL/bullettex.png",					// バレットアイテム
	"../data/MODEL/1213810.png",					// スピードアップアイテム
	"../data/MODEL/mark_camera_satsuei_ok.png",	// 強制バックカメラアイテム
	"../data/MODEL/yama_kiri.png",					// 霧アイテム
};

//=============================================================================
// 初期化処理
//=============================================================================
void ITEM::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		g_pD3DTextureItem[nCntItemType] = NULL;
		g_pMeshItem[nCntItemType] = NULL;
		g_pD3DXMatBuffItem[nCntItemType] = NULL;

		// Xファイルの読み込み
		D3DXLoadMeshFromX(c_aFileNameItemModel[nCntItemType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pD3DXMatBuffItem[nCntItemType],
			NULL,
			&g_aNumMatItem[nCntItemType],
			&g_pMeshItem[nCntItemType]);

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			c_aFileNameItemTex[nCntItemType],			// ファイルの名前
			&g_pD3DTextureItem[nCntItemType]);			// 読み込むメモリー

	}

	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		this[nCntItem].nIdxShadow = -1;
		this[nCntItem].nType = -1;
	}
	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X/4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
		int x = rand() % 2;
		int z = rand() % 2;
		if (x == 1) pos.x *= -1;
		if (z == 1) pos.z *= -1;

		/*
		enum
		{
			ITEMTYPE_TIKEI = 0,		// 地形
			ITEMTYPE_LIFE,			// ライフ
			ITEMTYPE_SENSYA,		// 戦車
			ITEMTYPE_BULLET,		// バレット
			ITEMTYPE_SPEEDUP,		// スピードアップ
			ITEMTYPE_CAMERA,		// お邪魔アイテム　強制バックカメラ
			ITEMTYPE_KIRI,			// お邪魔アイテム　霧
			ITEMTYPE_MAX
		};
		*/
		int ItemNum = rand() % ITEMTYPE_MAX;
		//ライフ、カメラ、霧アイテムの時はもう一度抽選
		if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
		SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
		//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);

	}
	this[0].GoukeiDrop = DROP_ITEM_MAX;

}

//=============================================================================
// 再初期化処理
//=============================================================================
void ITEM::Reinit(void)
{

	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		this[nCntItem].SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		this[nCntItem].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		this[nCntItem].SetQrot(0.0f);
		this[nCntItem].SetUse(false);
		this[nCntItem].Droptime = 0.0f;
		this[nCntItem].nIdxShadow = -1;
		this[nCntItem].nType = -1;
		this[nCntItem].GettingSignal = false;
		this[nCntItem].GettingSignalEnd = false;
		this[nCntItem].CollisionFieldEnd = false;
	}
	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
		int x = rand() % 2;
		int z = rand() % 2;
		if (x == 1) pos.x *= -1;
		if (z == 1) pos.z *= -1;
		/*
		enum
		{
			ITEMTYPE_TIKEI = 0,		// 地形
			ITEMTYPE_LIFE,			// ライフ
			ITEMTYPE_SENSYA,		// 戦車
			ITEMTYPE_BULLET,		// バレット
			ITEMTYPE_SPEEDUP,		// スピードアップ
			ITEMTYPE_CAMERA,		// お邪魔アイテム　強制バックカメラ
			ITEMTYPE_KIRI,			// お邪魔アイテム　霧
			ITEMTYPE_MAX
		};
		*/
		int ItemNum = rand() % ITEMTYPE_MAX;
		//ライフ、カメラ、霧アイテムの時はもう一度抽選
		if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
		SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
		//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);

	}
	this[0].GoukeiDrop = DROP_ITEM_MAX;
}

//=============================================================================
// 終了処理
//=============================================================================
void ITEM::Uninit(void)
{
	for(int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		if(g_pD3DTextureItem[nCntItemType] != NULL)
		{// テクスチャの開放
			g_pD3DTextureItem[nCntItemType]->Release();
			g_pD3DTextureItem[nCntItemType] = NULL;
		}

		if(g_pMeshItem[nCntItemType] != NULL)
		{// メッシュの開放
			g_pMeshItem[nCntItemType]->Release();
			g_pMeshItem[nCntItemType] = NULL;
		}

		if(g_pD3DXMatBuffItem[nCntItemType] != NULL)
		{// マテリアルの開放
			g_pD3DXMatBuffItem[nCntItemType]->Release();
			g_pD3DXMatBuffItem[nCntItemType] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void ITEM::Update(void)
{
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			//-------------------------------------------オブジェクトの値読み込み
			D3DXVECTOR3 pos = this[nCntItem].GetPos();
			D3DXVECTOR3 rot = this[nCntItem].GetRot();
			D3DXVECTOR3 FieldNorVec = this[nCntItem].GetFieldNorVec();
			D3DXVECTOR3 FieldNorUpNorCross = this[nCntItem].GetFieldNorUpNorCross();
			float Qrot = this[nCntItem].GetQrot();

			//フィールドに落ちてるときはくるくる回転させる
			rot.y += VALUE_ROTATE_ITEM;

			//徐々に落ちてくる
			if (this[nCntItem].CollisionFieldEnd != true)
			{
				pos.y -= VALUE_FALLSPEED_ITEM;
			}

			//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
			D3DXVECTOR3 Upvec = D3DXVECTOR3(0.0, 1.0f, 0.0f);
			D3DXVec3Cross(&FieldNorVec, &FieldNorUpNorCross, &Upvec);
			float kakezan = D3DXVec3Dot(&FieldNorUpNorCross, &Upvec);
			if (kakezan != 0)
			{
				float cossita = kakezan /
					sqrtf(FieldNorUpNorCross.x*FieldNorUpNorCross.x +
						FieldNorUpNorCross.y *FieldNorUpNorCross.y +
						FieldNorUpNorCross.z * FieldNorUpNorCross.z);
				Qrot = acosf(cossita);
			}
			else Qrot = 0.0f;

			//-------------------------------------------オブジェクトの値書き込み
			this[nCntItem].SetPos(pos);
			this[nCntItem].SetRot(rot);
			this[nCntItem].SetFieldNorVec(FieldNorVec);
//			this[nCntItem].SetFieldNorUpNorCross(FieldNorUpNorCross);
			this[nCntItem].SetQrot(Qrot);
		}


		if (this[nCntItem].GettingSignal == true)
		{
			GettingItem(nCntItem);
		}


	}


	//アイテムを復活させる制御。
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		if (this[0].GoukeiDrop > DROP_ITEM_MAX) break;
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			this[nCntItem].Droptime += DROP_ITEM_CHARGE_ADDTIME;
			if (this[nCntItem].Droptime >= DROP_ITEM_CHARGE_CNT)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
				int x = rand() % 2;
				int z = rand() % 2;
				if (x == 1) pos.x *= -1;
				if (z == 1) pos.z *= -1;
				int ItemNum = rand() % ITEMTYPE_MAX;
				//ライフ、カメラ、霧アイテムの時はもう一度抽選
				if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
				SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
				//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
				this[nCntItem].CollisionFieldEnd = false;
				this[nCntItem].Droptime = 0.0f;
				this[0].GoukeiDrop++;
				PlaySound(SOUND_LABEL_SE_nyu);
			}
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void ITEM::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			D3DXQUATERNION q(0, 0, 0, 1);
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);

			//-------------------------------------------オブジェクトの値読み込み
			D3DXMATRIX mtxWorldItem = this[nCntItem].GetMatrix();
			D3DXVECTOR3 pos = this[nCntItem].GetPos();
			D3DXVECTOR3 rot = this[nCntItem].GetRot();
			D3DXVECTOR3 scl = this[nCntItem].GetScl();
			D3DXVECTOR3 FieldNorVec = this[nCntItem].GetFieldNorVec();
			float Qrot = this[nCntItem].GetQrot();


			//q=(rotVecAxis法線)*(g_Player.rot回転)  -がキモ？
			D3DXQuaternionRotationAxis(&q, &FieldNorVec, -Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &q);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldItem);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxRot);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxQ);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(&mtxWorldItem, &mtxWorldItem, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldItem);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffItem[this[nCntItem].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatItem[this[nCntItem].nType]; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, g_pD3DTextureItem[this[nCntItem].nType]);
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureItem[this[nCntItem].nType]);

				// 描画
				g_pMeshItem[this[nCntItem].nType]->DrawSubset(nCntMat);
			}
			pDevice->SetMaterial(&matDef);
		}
	}
	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// アイテムの設定
//=============================================================================
void ITEM::SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, int nType)
{
	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		bool use = this[nCntItem].GetUse();
		if (use == true)
		{
			this[nCntItem].SetPos(pos);
			this[nCntItem].SetScl(scl);
			this[nCntItem].SetRot(rot);
			this[nCntItem].SetUse(true);
			this[nCntItem].nType = nType;
			break;
		}
	}
}

//=============================================================================
// アイテムの削除
//=============================================================================
void ITEM::DeleteItem(int nIdxItem)
{
	if(nIdxItem >= 0 && nIdxItem < OBJECT_ITEM_MAX)
	{
		this[nIdxItem].SetUse(false);
		this[nIdxItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// アイテムを取得したプレイヤーへ近づける関数
//=============================================================================
void ITEM::GettingItem(int nIdxItem)
{
	if (this[nIdxItem].GettingSignalEnd == false)
	{
		//-------------------------------------------オブジェクトの値読み込み
		D3DXVECTOR3 pos = this[nIdxItem].GetPos();
		D3DXVECTOR3 rot = this[nIdxItem].GetRot();
		D3DXVECTOR3 scl = this[nIdxItem].GetScl();

		//くるくる回転を加速
		rot.y += VALUE_ROTATE_ITEM*10;
		
		//プレイヤーとアイテムの距離を計算し/5分づつ近づける
		//PLAYER_HONTAI *p = this[nIdxItem].GetPlayer();
		PLAYER_HONTAI *p = p->GetPlayerPointer();
		D3DXVECTOR3 PlayerPos = this[nIdxItem].p[this[nIdxItem].GetPlayerType].GetPos();

		D3DXVECTOR3 distance = PlayerPos - pos;
		distance /= 4.0f;
		pos += distance;
		scl -= D3DXVECTOR3(ITEM_SMALL_SCL, ITEM_SMALL_SCL, ITEM_SMALL_SCL);
		if (scl.x <= ITEM_DELETE_SCL)
		{
			this[nIdxItem].GettingSignalEnd = true;
		}

		//-------------------------------------------オブジェクトの値書き込み
		this[nIdxItem].SetPos(pos);
		this[nIdxItem].SetRot(rot);
	}

	else
	{
		DeleteItem(nIdxItem);
		this[nIdxItem].GettingSignal = false;
		this[nIdxItem].GettingSignalEnd = false;
		this[0].GoukeiDrop--;
	}
}
