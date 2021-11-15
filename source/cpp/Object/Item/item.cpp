/**
* @file item.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#include "../../../h/main.h"
#include "../../../h/Map/field.h"
#include "../../../h/Other/sound.h"
#include "../../../h/Object/Player/player.h"
#include "../../../h/Object/Shadow/shadow.h"
#include "../../../h/Library/library.h"
#include "../../../h/Object/Item/item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
constexpr float	VALUE_ROTATE_ITEM_LO{ D3DX_PI * 0.01f };	//!< 回転速度　低速
constexpr float	VALUE_ROTATE_ITEM_HI{ D3DX_PI * 0.1f };		//!< 回転速度　高速
constexpr float	APPROADHING_RATE{ 0.25f };					//!< プレイヤーに近づいていく割合 (距離 * APPROADHING_RATE)
constexpr float	VALUE_FALLSPEED_ITEM{ 2.0f };				//!< 落下速度
constexpr float	ITEM_RADIUS{ 20.0f };						//!< 半径
constexpr float DROP_ITEM_CHARGE_CNT{ 60.0f };				//!< アイテムをリスポーンさせる時の所要タイム
constexpr int DROP_ITEM_MAX{ 20 };							//!< フィールドに落ちてるアイテムの数
constexpr int DROP_ITEM_CHARGE_ADDTIME{ 1 };				//!< アイテムをリスポーンさせる時の加算タイム

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
ITEM::ITEM(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	//カウントループ(アイテムの種類)　
	for (int nCntItemType = 0; nCntItemType < ITEM_TYPE_MAX; nCntItemType++)
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
		this->vtx[CntItem].MakeVertex3D(nv, FVF_VERTEX_3D);
		this->vtx[CntItem].MakeIdxVertex(nvi);
		LPDIRECT3DVERTEXBUFFER9 VtxBuff;
		LPDIRECT3DINDEXBUFFER9	IdxBuff;

		//バッファの取得から反映
		VtxBuff = this->vtx[CntItem].VtxBuff();
		IdxBuff = this->vtx[CntItem].IdxBuff();
		Mesh->GetVertexBuffer(&VtxBuff);
		Mesh->GetIndexBuffer(&IdxBuff);
		this->vtx[CntItem].VtxBuff(VtxBuff);
		this->vtx[CntItem].IdxBuff(IdxBuff);

		//使用の設定
		this->iUseType[CntItem].Use(NoUse);
	}

	//モデルデータ反映
	this->model.NumMat(nm);
	this->model.Mat(BuffMat);
	this->model.NumVertex(nv);
	this->model.NumPolygon(np);
	this->model.NumVertexIndex(nvi);

}

//=============================================================================
// デストラクタ　削除
//=============================================================================
ITEM::~ITEM(void)
{
	//カウントループ(アイテムの種類)
	for (int nCntItemType = 0; nCntItemType < ITEM_TYPE_MAX; nCntItemType++)
	{
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
		this->ItemParaAll[nCntItem].eType = ITEM_TYPE_NONE;
		this->ItemParaAll[nCntItem].LinkShadowPos = VEC3_ALL0;
		this->ItemParaAll[nCntItem].ShadowPosSignal = false;
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
//		this[nCntItem].Pos(VEC3_ALL0);
//		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
//		this[nCntItem].SetRot(VEC3_ALL0);
//		this[nCntItem].SetFieldNorVec(VEC3_ALL0);
//		this[nCntItem].SetFieldNorUpNorCross(VEC3_ALL0);
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
//		this[0].SetItem(pos, D3DXVECTOR3(2.0f, 2.0f, 2.0f), VEC3_ALL0, ItemTypeNum);
//		//this[0].SetItem(pos, VEC3_ALL1, VEC3_ALL0, ITEMTYPE_TIKEI);
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
//		this[nCntItem].Pos(VEC3_ALL0);
//		this[nCntItem].SetScl(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
//		this[nCntItem].SetRot(VEC3_ALL0);
//		this[nCntItem].SetFieldNorVec(VEC3_ALL0);
//		this[nCntItem].SetFieldNorUpNorCross(VEC3_ALL0);
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
void ITEM::Update(PLAYER *Player, SHADOW *Shadow, bool NetGameStartFlag)
{
	//カウントループ(アイテムのインスタンス数)
	for (int nCntItem = 0; nCntItem < OBJECT_ITEM_MAX; nCntItem++)
	{
		this->ItemParaAll[nCntItem].NetGetItemFlagOld = this->ItemParaAll[nCntItem].NetGetItemFlag;
		this->ItemParaAll[nCntItem].NetGetItemFlag = false;
		if (this->iUseType[nCntItem].Use() == YesUseType1)
		{
			//-------------------------------------------オブジェクトの値読み込み
			D3DXVECTOR3 pos = this->Transform[nCntItem].Pos();
			D3DXVECTOR3 rot = this->Transform[nCntItem].Rot();
			D3DXVECTOR3 scl = this->Transform[nCntItem].Scl();
			D3DXVECTOR3 FieldNorVec = this->PostureVec[nCntItem].FNVecFunc();
			D3DXVECTOR3 FieldNorUpNorCross = this->PostureVec[nCntItem].FNUNCrossFunc();
			float Qrot = this->PostureVec[nCntItem].QrotFunc();

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

			// 影の位置設定
			Shadow->UpdateInstance(this->ItemParaAll[nCntItem].nIdxShadow,
				this->ItemParaAll[nCntItem].LinkShadowPos,rot, scl);

			//-------------------------------------------オブジェクトの値書き込み
			this->Transform[nCntItem].Pos(pos);
			this->Transform[nCntItem].Rot(rot);
			this->PostureVec[nCntItem].FNUNCrossFunc(FieldNorUpNorCross);
			this->PostureVec[nCntItem].QrotFunc(Qrot);
		}

		//だれかがアイテムを取得したらそのプレイヤーに近づく処理
		if (this->ItemParaAll[nCntItem].GettingSignal == true)
		{
			GettingItem(nCntItem, Player, Shadow);
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
					int ItemNum = rand() % ITEM_TYPE_MAX;
					//確立設定のため　ライフ、カメラ、霧アイテムの時はもう一度抽選
					if (ItemNum == ITEM_TYPE_LIFE && ItemNum == ITEM_TYPE_CAMERA && ItemNum == ITEM_TYPE_KIRI) ItemNum = rand() % ITEM_TYPE_MAX;
					this->ItemParaAll[nCntItem].eType = eITEM_TYPE(ItemNum);
					//インスタンスセット　使用する
					this->SetInstance(pos, D3DXVECTOR3(ITEM_BIG_SCL, ITEM_BIG_SCL, ITEM_BIG_SCL), VEC3_ALL0, this->ItemParaAll[nCntItem].eType, Shadow);
					//this->SetInstance(pos, D3DXVECTOR3(ITEM_BIG_SCL, ITEM_BIG_SCL, ITEM_BIG_SCL), VEC3_ALL0, ITEM_TYPE_TIKEI);
					//this->SetInstance(pos, D3DXVECTOR3(ITEM_BIG_SCL, ITEM_BIG_SCL, ITEM_BIG_SCL), VEC3_ALL0, ITEM_TYPE_SENSYA);
					this->ItemParaAll[nCntItem].CollisionFieldEnd = false;
					this->ItemParaOne.Droptime = 0;
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
			D3DXVECTOR3 PlayerUpToFieldNorVec = this->PostureVec[nCntItem].FNUNCrossFunc();
			float Qrot = this->PostureVec[nCntItem].QrotFunc();

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
			mat = *this->model.Mat();
			pD3DXMat = (D3DXMATERIAL*)mat->GetBufferPointer();

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
			for (int nCntMat = 0; nCntMat < (int)this->model.NumMat(); nCntMat++)
			{
				// デバイスにマテリアルの設定
				pDevice->SetMaterial(&pD3DXMat->MatD3D);
				// デバイスにテクスチャの設定
				pDevice->SetTexture(0, this->tex[this->ItemParaAll[nCntItem].eType].Texture());
				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);
				// 頂点バッファをレンダリングパイプラインに設定
				pDevice->SetStreamSource(0, this->vtx[nCntItem].VtxBuff(), 0, sizeof(VERTEX_3D));
				// インデックスバッファをレンダリングパイプラインに設定
				pDevice->SetIndices(this->vtx[nCntItem].IdxBuff());
				//描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this->model.NumVertex(), 0, this->model.NumPolygon());
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// アイテムの設定
//=============================================================================
void ITEM::SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType, SHADOW *s)
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
			this->iUseType[nCntItem].Use(YesUseType1);
			this->ItemParaAll[nCntItem].eType = eType;

			// 影の設定
			this->ItemParaAll[nCntItem].LinkShadowPos = VEC3_ALL0;
			this->ItemParaAll[nCntItem].ShadowPosSignal = false;
			this->ItemParaAll[nCntItem].nIdxShadow = s->SetInstance(pos, VEC3_ALL1);

			break;
		}
	}
}

//未使用
void ITEM::SetInstance(int Index, D3DXVECTOR3 pos, D3DXVECTOR3 scl, D3DXVECTOR3 rot, eITEM_TYPE eType)
{
	//使用されていないインスタンスが存在すれば一個だけ使用(出現)する
	if (this->iUseType[Index].Use() == NoUse)
	{
		//基本パラメータの設定
		this->Transform[Index].Pos(pos);
		this->Transform[Index].Scl(scl);
		this->Transform[Index].Rot(rot);
		this->iUseType[Index].Use(YesUseType1);
		this->ItemParaAll[Index].eType = eType;
	}
}

//=============================================================================
// アイテムの削除
//=============================================================================
void ITEM::ReleaseInstance(int nIdxItem, SHADOW *s)
{
	//指定範囲外の数値ならスルー　存在する数値なら使用(描画)しない
	if(nIdxItem >= 0 && nIdxItem < OBJECT_ITEM_MAX)
	{
		//基本パラメータの設定
		this->iUseType[nIdxItem].Use(NoUse);
		this->ItemParaAll[nIdxItem].CollisionFieldEnd = false;
		s->ReleaseInstance(this->ItemParaAll[nIdxItem].nIdxShadow);
	}
}

//=============================================================================
// アイテムを取得したプレイヤーへ近づける関数
//=============================================================================
void ITEM::GettingItem(int nIdxItem, PLAYER *Player, SHADOW *s)
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
		D3DXVECTOR3 ppos = Player->modelDraw[this->ItemParaAll[nIdxItem].GetPlayerType].Transform[PLAYER_PARTS_TYPE_HOUDAI].Pos();

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
		this->ReleaseInstance(nIdxItem,s);
		this->ItemParaAll[nIdxItem].GettingSignal = false;
		this->ItemParaAll[nIdxItem].GettingSignalEnd = false;
		this->ItemParaOne.GoukeiDrop--;
	}
}
