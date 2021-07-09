/**
* @file item.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/map/field.h"
#include "../../h/other/sound.h"
#include "../../h/object/player.h"
#include "../../h/library.h"
#include "../../h/object/item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_ROTATE_ITEM_LO	(D3DX_PI * 0.01f)			// 回転速度　低速
#define	VALUE_ROTATE_ITEM_HI	(D3DX_PI * 0.1f)			// 回転速度　高速
#define	APPROADHING_RATE		(0.25f)						// プレイヤーに近づいていく割合 (距離 * APPROADHING_RATE)
#define	VALUE_FALLSPEED_ITEM	(2.0f)						// 落下速度
#define	ITEM_RADIUS				(20.0f)						// 半径
#define DROP_ITEM_MAX						(20)			//!< フィールドに落ちてるアイテムの数
#define DROP_ITEM_CHARGE_ADDTIME			(1.0f)			//!< アイテムをリスポーンさせる時の加算タイム
#define DROP_ITEM_CHARGE_CNT				(60.0f)			//!< アイテムをリスポーンさせる時の所要タイム

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
ITEM::ITEM(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//カウントループ(アイテムの種類)　
	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++)
	{
		// テクスチャの読み込み
		this->tex[nCntItemType].LoadTexture(this->c_aFileNameTex[nCntItemType]);
	}

	// Xファイルの読み込みデータ群
	LPD3DXMESH	Mesh;
	DWORD nv, nvi, nm, np;
	LPD3DXBUFFER BuffMat;

	// Xファイルの読み込み
	// モデルは複数種類あるが、テクスチャ以外のモデルデータは全て同じため、モデルの読み込みは一度だけにしている
	LoadMesh(this->c_aFileNameModel[0], &BuffMat,
		&nm, &Mesh, &nv, &np, &nvi, NULL);

	//カウントループ(アイテムのインスタンス数)　
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		//頂点の作成
		LPDIRECT3DVERTEXBUFFER9 *VtxBuff;
		LPDIRECT3DINDEXBUFFER9	*IdxBuff;
		this->vtx[CntItem].MakeVertex3D(nv,FVF_VERTEX_3D);
		this->vtx[CntItem].MakeIdxVertex(nvi);

		//バッファの取得から反映
		VtxBuff = this->vtx[CntItem].VtxBuff();
		IdxBuff = this->vtx[CntItem].IdxBuff();
		Mesh->GetVertexBuffer(VtxBuff);
		Mesh->GetIndexBuffer(IdxBuff);
		this->vtx[CntItem].VtxBuff(*VtxBuff);
		this->vtx[CntItem].IdxBuff(*IdxBuff);

		//使用の設定
		this->iUseType[CntItem].ChangeUse(NoUse);
	}

	//モデルデータ反映
	this->model.SetNumMat(nm);
	this->model.SetMat(BuffMat);
	this->model.SetNumVertex(nv);
	this->model.SetNumPolygon(np);
	this->model.SetNumVertexIndex(nvi);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
ITEM::~ITEM(void)
{
	//カウントループ(アイテムの種類)
	for (int nCntItemType = 0; nCntItemType < ITEMTYPE_MAX; nCntItemType++) {
		//テクスチャ解放
		this->tex[nCntItemType].~TEXTURE();
		//モデル解放
		this->model.~ModelAttribute();
	}

	//カウントループ(アイテムのインスタンス数)　
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		//頂点解放
		this->vtx[CntItem].~VTXBuffer();
	}
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void ITEM::Init(void)
{
	//カウントループ(アイテムのインスタンス数)
	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		//初期化設定
		this->Transform[nCntItem].Scl(this->ItemParaOne.MaxSize);
		this->ItemParaAll[nCntItem].nIdxShadow = -1;
		this->ItemParaAll[nCntItem].eType = ITEMTYPE_NONE;
		this->ItemParaAll[nCntItem].NetUse = false;
		this->ItemParaAll[nCntItem].NetGetItemFlag = false;
		this->ItemParaAll[nCntItem].NetGetItemFlagOld = false;
		this->iUseType[nCntItem].Use(NoUse);
	}

	//!< 現在ドロップしている数　ドロップさせるまでのタイマー
	this->ItemParaOne.GoukeiDrop = 0;
	this->ItemParaOne.Droptime = 0;
}


////=============================================================================
//// 再初期化処理
////=============================================================================
//void ITEM::Reinit(void)
//{
//	//初期化 全てセットしない
//	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
//	{
//		this[nCntItem].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
//		this[nCntItem].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetQrot(0.0f);
//		this[nCntItem].NetUse = false;
//		this[nCntItem].Use(false);
//		this[nCntItem].Droptime = 0.0f;
//		this[nCntItem].nIdxShadow = -1;
//		this[nCntItem].nType = -1;
//		this[nCntItem].GettingSignal = false;
//		this[nCntItem].GettingSignalEnd = false;
//		this[nCntItem].CollisionFieldEnd = false;
//		this[nCntItem].NetGetItemFlag = false;
//		this[nCntItem].NetGetItemFlagOld = false;
//	}
//
//	//初期化2　すべてセット
//	/*
//	for (int nCntItem = 0; nCntItem < DROP_ITEM_MAX; nCntItem++)
//	{
//		D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
//		int x = rand() % 2;
//		int z = rand() % 2;
//		if (x == 1) pos.x *= -1;
//		if (z == 1) pos.z *= -1;
//		int ItemTypeNum = rand() % ITEMTYPE_MAX;
//		//ライフ、カメラ、霧アイテムの時はもう一度抽選
//		if (ItemTypeNum == ITEMTYPE_LIFE && ItemTypeNum == ITEMTYPE_CAMERA && ItemTypeNum == ITEMTYPE_KIRI) ItemTypeNum = rand() % ITEMTYPE_MAX;
//		this[0].SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ItemTypeNum);
//		//this[0].SetItem(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
//		this[nCntItem].Use(true);
//	}
//	*/
//
//	//this[0].GoukeiDrop = DROP_ITEM_MAX;
//	this[0].GoukeiDrop = 0;
//}
//


////=============================================================================
//// 再初期化処理
////=============================================================================
//void ITEM::ReinitNet(void)
//{
//	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
//	{
//		this[nCntItem].Pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
//		this[nCntItem].SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetFieldNorUpNorCross(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//		this[nCntItem].SetQrot(0.0f);
//		this[nCntItem].Use(false);
//		this[nCntItem].Droptime = 0.0f;
//		this[nCntItem].nIdxShadow = -1;
//		this[nCntItem].nType = -1;
//		this[nCntItem].GettingSignal = false;
//		this[nCntItem].GettingSignalEnd = false;
//		this[nCntItem].CollisionFieldEnd = false;
//		this[nCntItem].NetGetItemFlag = false;
//		this[nCntItem].NetGetItemFlagOld = false;
//	}
//}
//


//=============================================================================
// 更新処理
//=============================================================================
void ITEM::Update(PLAYER_HONTAI *p, bool NetGameStartFlag)
{
	//カウントループ(アイテムのインスタンス数)
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		this->ItemParaAll[nCntItem].NetGetItemFlagOld = this->ItemParaAll[nCntItem].NetGetItemFlag;
		this->ItemParaAll[nCntItem].NetGetItemFlag = false;
		bool use = this->iUseType[nCntItem].Use();
		if (use == true)
		{
			//-------------------------------------------オブジェクトの値読み込み
			D3DXVECTOR3 pos = this->Transform[nCntItem].Pos();
			D3DXVECTOR3 rot = this->Transform[nCntItem].Rot();
			D3DXVECTOR3 FieldNorVec = this->PostureVec[nCntItem].FNVec();
			D3DXVECTOR3 FieldNorUpNorCross = this->PostureVec[nCntItem].FNUNCross();
			float Qrot = this->PostureVec[nCntItem].Qrot();

			//フィールドに落ちてるときはくるくる回転させる
			rot.y += VALUE_ROTATE_ITEM_LO;

			//徐々に落ちてくる
			if (this->ItemParaAll[nCntItem].CollisionFieldEnd != true)
			{
				pos.y -= VALUE_FALLSPEED_ITEM;
			}

			//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
			D3DXVECTOR3 Upvec = D3DXVECTOR3(0.0, 1.0f, 0.0f);
			D3DXVec3Cross(&FieldNorUpNorCross, &FieldNorVec, &Upvec);
			float kakezan = D3DXVec3Dot(&FieldNorVec, &Upvec);
			if (kakezan != 0.0f)
			{
				float cossita = kakezan /
					sqrtf(FieldNorVec.x*FieldNorVec.x +
						FieldNorVec.y *FieldNorVec.y +
						FieldNorVec.z * FieldNorVec.z);
				Qrot = acosf(cossita);
			}
			else Qrot = 0.0f;

			//-------------------------------------------オブジェクトの値書き込み
			this->Transform[nCntItem].Pos(pos);
			this->Transform[nCntItem].Rot(rot);
			this->PostureVec[nCntItem].FNUNCross(FieldNorUpNorCross);
			this->PostureVec[nCntItem].Qrot(Qrot);
		}

		//だれかがアイテムを取得したらそのプレイヤーに近づく処理
		if (this->ItemParaAll[nCntItem].GettingSignal == true)
		{
			GettingItem(nCntItem,p);
		}

	}

	//ローカル対戦ならここでリスポーン処理
	if (NetGameStartFlag == false)
	{
		//アイテムを復活させる制御。
		for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
		{
			//アイテムを最大数まで使用していたらスルー
			if (this->ItemParaOne.GoukeiDrop > DROP_ITEM_MAX) break;
			bool use = this->iUseType[nCntItem].Use();
			if (use == false)
			{
				//タイマーを加算して、一定数以上になるとアイテムを出現させる
				this->ItemParaOne.Droptime += DROP_ITEM_CHARGE_ADDTIME;
				if (this->ItemParaOne.Droptime >= DROP_ITEM_CHARGE_CNT)
				{
					//出現処理
					//座標指定
					D3DXVECTOR3 pos = D3DXVECTOR3(float(rand() % int(WALL_SIZE_X / 4)) + 100.0f, ITEM_INIT_POSY, float(rand() % int(WALL_SIZE_X / 4)) + 100.0f);
					//ランダムで正負を反転させる
					int x = rand() % 2;
					int z = rand() % 2;
					if (x == 1) pos.x *= -1;
					if (z == 1) pos.z *= -1;
					//ランダムでアイテムの種類選択
					int ItemNum = rand() % ITEMTYPE_MAX;
					//確立設定のため　ライフ、カメラ、霧アイテムの時はもう一度抽選
					if (ItemNum == ITEMTYPE_LIFE && ItemNum == ITEMTYPE_CAMERA && ItemNum == ITEMTYPE_KIRI) ItemNum = rand() % ITEMTYPE_MAX;
					//インスタンスセット　使用する
					this[0].SetInstance(pos, D3DXVECTOR3(ITEM_BIG_SCL, ITEM_BIG_SCL, ITEM_BIG_SCL), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_TIKEI);
					this->ItemParaAll[nCntItem].CollisionFieldEnd = false;
					this->ItemParaOne.Droptime = 0.0f;
					this->ItemParaOne.GoukeiDrop++;
					PlaySound(SOUND_LABEL_SE_nyu);
				}
				break;
			}
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

	//カウントループ(アイテムのインスタンス数)　使用の設定
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		//使用していれば描画
		bool use = this->iUseType[nCntItem].Use();
		if (use == true)
		{
			//ワールド変換に必要なデータ群とマテリアル情報
			D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
			D3DXMATERIAL *pD3DXMat;
			D3DMATERIAL9 matDef;

			D3DXQUATERNION q(0, 0, 0, 1);
			D3DXMATRIX mtxQ;
			D3DXMatrixIdentity(&mtxQ);

			D3DXMATRIX mtxWorldItem;
			//-------------------------------------------オブジェクトの値読み込み
			D3DXVECTOR3 pos = this->Transform[nCntItem].Pos();
			D3DXVECTOR3 rot = this->Transform[nCntItem].Rot();
			D3DXVECTOR3 scl = this->Transform[nCntItem].Scl();
			D3DXVECTOR3 PlayerUpToFieldNorVec = this->PostureVec[nCntItem].FNUNCross();
			float Qrot = this->PostureVec[nCntItem].Qrot();

			//q=(rotVecAxis法線)*(g_Player.rot回転)  -がキモ？
			D3DXQuaternionRotationAxis(&q, &PlayerUpToFieldNorVec, -Qrot);
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

			// マテリアル設定
			pDevice->GetMaterial(&matDef);
			LPD3DXBUFFER mat;
			mat = *this->model.GetMat();
			pD3DXMat = (D3DXMATERIAL*)mat->GetBufferPointer();

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
			for (int nCntMat = 0; nCntMat < (int)this->model.GetNumMat(); nCntMat++)
			{
				// デバイスにマテリアルの設定
				pDevice->SetMaterial(&pD3DXMat->MatD3D);

				// デバイスにテクスチャの設定
				pDevice->SetTexture(0, this->tex[this->ItemParaAll[nCntItem].eType].Texture());
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);
				// 頂点バッファをレンダリングパイプラインに設定
				pDevice->SetStreamSource(0, *this->vtx[nCntItem].VtxBuff(), 0, sizeof(VERTEX_3D));
				// インデックスバッファをレンダリングパイプラインに設定
				pDevice->SetIndices(*this->vtx[nCntItem].IdxBuff());
				//描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this->model.GetNumVertex(), 0, this->model.GetNumPolygon());
			}
		}
	}
	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// アイテムの設定
//=============================================================================
void ITEM::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType)
{
	//カウントループ(アイテムのインスタンス数)
	for(int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		//使用されていないインスタンスが存在すれば一個だけ使用(出現)する
		bool use = this->iUseType[nCntItem].Use();
		if (use != true)
		{
			//基本パラメータの設定
			this->Transform[nCntItem].Pos(pos);
			this->Transform[nCntItem].Scl(scl);
			this->Transform[nCntItem].Rot(rot);
			this->iUseType[nCntItem].Use(YesUse);
			this->ItemParaAll[nCntItem].eType = eType;
			break;
		}
	}
}

//=============================================================================
// アイテムの削除
//=============================================================================
void ITEM::ReleaseInstance(int nIdxItem)
{
	//指定範囲外の数値ならスルー　存在する数値なら使用(描画)しない
	if(nIdxItem >= 0 && nIdxItem < OBJECT_ITEM_MAX)
	{
		//基本パラメータの設定
		this->iUseType[nIdxItem].Use(NoUse);
		this->ItemParaAll[nIdxItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// アイテムを取得したプレイヤーへ近づける関数
//=============================================================================
void ITEM::GettingItem(int nIdxItem, PLAYER_HONTAI *p)
{
	//プレイヤーへ近づける処理
	if (this->ItemParaAll[nIdxItem].GettingSignalEnd == false)
	{
		//-------------------------------------------オブジェクトの値読み込み
		D3DXVECTOR3 ipos = this->Transform[nIdxItem].Pos();
		D3DXVECTOR3 irot = this->Transform[nIdxItem].Rot();
		D3DXVECTOR3 iscl = this->Transform[nIdxItem].Scl();

		//くるくる回転を加速
		irot.y += VALUE_ROTATE_ITEM_HI;
		
		//プレイヤーとアイテムの距離を計算し近づける
		D3DXVECTOR3 ppos = p->Transform[this->ItemParaAll[nIdxItem].GetPlayerType].Pos();

		D3DXVECTOR3 distance = ppos - ipos;
		distance *= APPROADHING_RATE;
		ipos += distance;
		//スケールも徐々に小さくしていく
		iscl -= D3DXVECTOR3(ITEM_SMALL_SCL, ITEM_SMALL_SCL, ITEM_SMALL_SCL);
		//一定値より小さくなれば近づく処理を終了させる
		if (iscl.x <= ITEM_DELETE_SCL)
		{
			this->ItemParaAll[nIdxItem].GettingSignalEnd = true;
		}

		//-------------------------------------------オブジェクトの値書き込み
		this->Transform[nIdxItem].Pos(ipos);
		this->Transform[nIdxItem].Rot(irot);
		this->Transform[nIdxItem].Scl(iscl);
	}

	//取得処理が終わったらアイテムを使用(描画)しない
	else if(this->ItemParaAll[nIdxItem].GettingSignalEnd == true)
	{
		//フラグを終了させ使用インスタンス数を減らす
		this->ReleaseInstance(nIdxItem);
		this->ItemParaAll[nIdxItem].GettingSignal = false;
		this->ItemParaAll[nIdxItem].GettingSignalEnd = false;
		this->ItemParaOne.GoukeiDrop--;
	}
}
