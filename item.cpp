/**
* @file item.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "main.h"
#include "item.h"
#include "field.h"
#include "player.h"
#include "shadow.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_ROTATE_ITEM		(D3DX_PI * 0.01f)			// 回転速度
#define	ITEM_RADIUS				(20.0f)						// 半径

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

D3DXMATRIX			g_mtxWorldItem;				// ワールドマトリックス

ITEM				g_aItem[MAX_ITEM];			// アイテムワーク

const char *c_aFileNameItemModel[ITEMTYPE_MAX] =
{
	"data/MODEL/tikeiItem.x",		// 地形変形アイテム
	"data/MODEL/lifeItem.x",		// ライフ回復
	"data/MODEL/sensyaItem.x",		// 戦車変形アイテム
	"data/MODEL/bulletItem.x",		// バレットアイテム
	"data/MODEL/speedItem.x",		// スピードアップアイテム
	"data/MODEL/cameraItem.x",		// 強制バックカメラアイテム
	"data/MODEL/kiriItem.x",		// 霧アイテム
};

const char *c_aFileNameItemTex[ITEMTYPE_MAX] =
{
	"data/MODEL/landmark_aogashima.png",		// 地形変形アイテム
	"data/MODEL/life000.png",					// ライフ回復
	"data/MODEL/war_sensya_noman.png",			// 戦車変形アイテム
	"data/MODEL/bullettex.png",					// バレットアイテム
	"data/MODEL/1213810.png",					// スピードアップアイテム
	"data/MODEL/mark_camera_satsuei_ok.png",	// 強制バックカメラアイテム
	"data/MODEL/yama_kiri.png",					// 霧アイテム
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		g_pD3DTextureItem[nCntItemType] = NULL;
		g_pMeshItem[nCntItemType] = NULL;
		g_pD3DXMatBuffItem[nCntItemType] = NULL;

		// Xファイルの読み込み
		if(FAILED(D3DXLoadMeshFromX(c_aFileNameItemModel[nCntItemType],
									D3DXMESH_SYSTEMMEM,
									pDevice,
									NULL,
									&g_pD3DXMatBuffItem[nCntItemType],
									NULL,
									&g_aNumMatItem[nCntItemType],
									&g_pMeshItem[nCntItemType])))
		{
			return E_FAIL;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
			c_aFileNameItemTex[nCntItemType],			// ファイルの名前
			&g_pD3DTextureItem[nCntItemType]);			// 読み込むメモリー

	}

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].Upvec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_aItem[nCntItem].rotVecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].Qrot = 0.0f;
		g_aItem[nCntItem].fRadius = 0.0f;
		g_aItem[nCntItem].Droptime = 0.0f;
		g_aItem[nCntItem].nIdxShadow = -1;
		g_aItem[nCntItem].nType = -1;
		g_aItem[nCntItem].bUse = false;
		g_aItem[nCntItem].GettingSignal = false;
		g_aItem[nCntItem].GettingSignalEnd = false;
		g_aItem[nCntItem].fCollisionEnd = false;
	}
	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % 500) + 100.0f, 1000.0f, float(rand() % 500) + 100.0f);
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
		SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
		//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);

	}

	g_aItem[0].GoukeiDrop = DROP_ITEM_MAX;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
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
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//フィールドに落ちてるときはくるくる回転させる
			g_aItem[nCntItem].rot.y += VALUE_ROTATE_ITEM;

			//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
			D3DXVec3Cross(&g_aItem[nCntItem].rotTOaxis, &g_aItem[nCntItem].rotVecAxis, &g_aItem[nCntItem].Upvec);
			float kakezan = D3DXVec3Dot(&g_aItem[nCntItem].rotVecAxis, &g_aItem[nCntItem].Upvec);
			if (kakezan != 0)
			{
				float cossita = kakezan /
					sqrtf(g_aItem[nCntItem].rotVecAxis.x*g_aItem[nCntItem].rotVecAxis.x +
						g_aItem[nCntItem].rotVecAxis.y *g_aItem[nCntItem].rotVecAxis.y +
						g_aItem[nCntItem].rotVecAxis.z * g_aItem[nCntItem].rotVecAxis.z);
				g_aItem[nCntItem].Qrot = acosf(cossita);
			}
			else g_aItem[nCntItem].Qrot = 0.0f;
		}
		if (g_aItem[nCntItem].GettingSignal == true)
		{
			GettingItem(nCntItem);
		}
	}
	//アイテムを復活させる制御。
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[0].GoukeiDrop > DROP_ITEM_MAX) break;
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].Droptime += DROP_ITEM_CHARGE_ADDTIME;
			if (g_aItem[nCntItem].Droptime >= DROP_ITEM_CHARGE_CNT)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % 500) + 100.0f, 1000.0f, float(rand() % 500) + 100.0f);
				int x = rand() % 2;
				int z = rand() % 2;
				if (x == 1) pos.x *= -1;
				if (z == 1) pos.z *= -1;
				int ItemNum = rand() % ITEMTYPE_MAX;
				//ライフ、カメラ、霧アイテムの時はもう一度抽選
				if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
				SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemNum);
				//SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
				g_aItem[nCntItem].fCollisionEnd = false;
				g_aItem[nCntItem].Droptime = 0.0f;
				g_aItem[0].GoukeiDrop++;
				PlaySound(SOUND_LABEL_SE_nyu);
			}
			break;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse==true)
		{
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			D3DXQUATERNION q(0, 0, 0, 1);
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);
			//q=(rotVecAxis法線)*(g_Player.rot回転)  -がキモ？
			D3DXQuaternionRotationAxis(&q, &g_aItem[nCntItem].rotTOaxis, -g_aItem[nCntItem].Qrot);
			D3DXMatrixRotationQuaternion(&mtxQ, &q);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldItem);

			// スケールを反映
			D3DXMatrixScaling(&mtxScl, g_aItem[nCntItem].scl.x, g_aItem[nCntItem].scl.y, g_aItem[nCntItem].scl.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxScl);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxQ);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffItem[g_aItem[nCntItem].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aNumMatItem[g_aItem[nCntItem].nType]; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
				if (pD3DXMat[nCntMat].pTextureFilename != NULL)
				{
					// テクスチャの設定
					pDevice->SetTexture(0, g_pD3DTextureItem[g_aItem[nCntItem].nType]);
				}

				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureItem[g_aItem[nCntItem].nType]);

				// 描画
				g_pMeshItem[g_aItem[nCntItem].nType]->DrawSubset(nCntMat);
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
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, int nType)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(!g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].scl = scl;
			g_aItem[nCntItem].rot = rot;
			g_aItem[nCntItem].fRadius = ITEM_RADIUS;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;

			// 影の設定
			//g_aItem[nCntItem].nIdxShadow = CreateShadow(g_aItem[nCntItem].pos, D3DXVECTOR3(g_aItem[nCntItem].fRadius * 2.0f, g_aItem[nCntItem].fRadius * 2.0f, g_aItem[nCntItem].fRadius * 2.0f));

			break;
		}
	}
}

//=============================================================================
// アイテムの削除
//=============================================================================
void DeleteItem(int nIdxItem)
{
	if(nIdxItem >= 0 && nIdxItem < MAX_ITEM)
	{
		ReleaseShadow(g_aItem[nIdxItem].nIdxShadow);
		g_aItem[nIdxItem].bUse = false;
		g_aItem[nIdxItem].fCollisionEnd = false;
	}
}

//=============================================================================
// アイテムの取得
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}

//=============================================================================
// アイテムを取得したプレイヤーへ近づける関数
//=============================================================================
void GettingItem(int nIdxItem)
{
	if (g_aItem[nIdxItem].GettingSignalEnd == false)
	{
		//くるくる回転を加速
		g_aItem[nIdxItem].rot.y += VALUE_ROTATE_ITEM*10;
		
		//プレイヤーとアイテムの距離を計算し/5分づつ近づける
		PLAYER_HONTAI *p = GetPlayerHoudai();
		D3DXVECTOR3 distance = p[g_aItem[nIdxItem].GetPlayerType].pos - g_aItem[nIdxItem].pos;
		distance /= 5.0f;
		g_aItem[nIdxItem].pos += distance;
		g_aItem[nIdxItem].scl -= D3DXVECTOR3(ITEM_SMALL_SCL, ITEM_SMALL_SCL, ITEM_SMALL_SCL);
		if (g_aItem[nIdxItem].scl.x <= ITEM_DELETE_SCL)
		{
			g_aItem[nIdxItem].GettingSignalEnd = true;
		}
	}

	else
	{
		DeleteItem(nIdxItem);
		g_aItem[nIdxItem].GettingSignal = false;
		g_aItem[nIdxItem].GettingSignalEnd = false;
		g_aItem[0].GoukeiDrop--;
	}
}
