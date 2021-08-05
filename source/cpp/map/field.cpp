/**
* @file field.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#include "../../h/main.h"
#include "../../h/other/input.h"
#include "../../h/map/field.h"
#include "../../h/object/item.h"
#include "../../h/object/bullet/bullet.h"
#include "../../h/other/sound.h"
#include "../../h/effect/explosion.h"
#include "../../h/object/player.h"
#include "../../h/library.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME				"../data/TEXTURE/map/white.png"		// 読み込むテクスチャファイル名
//#define	TEXTURE_NOISE					"../data/TEXTURE/noise.bmp"		// 読み込むテクスチャファイル名
#define SPHERELINEAR_LEN				(10.0f)							//!< 球面線形補間で使う、プレイヤーの座標と3角ポリゴン辺までの距離(プレイヤーの座標がこの値より辺に近ければ球面線形補間開始する)

#define FIELD_Y_HIGH					(120.0f)						//!< フィールドの高さで色を変える。高い
#define FIELD_Y_LOW						(20.0f)							//!< フィールドの高さで色を変える。低い

static D3DXCOLOR PLAYER_COLOR[] = {
	D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),//p1カラー
	D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f),//p2カラー
	D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),//p3カラー
	D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f),//p4カラー
};

//=============================================================================
// コンストラクタ　「読み込み」「初期化」
//=============================================================================
FIELD::FIELD(void)
{
	//オブジェクトカウントアップ
	this->CreateInstanceOBJ();

	// テクスチャの読み込み
	this->tex.LoadTexture(TEXTURE_FILENAME);


	// ブロック数の設定
	this->FieldPara.nNumBlockXField = 32;
	this->FieldPara.nNumBlockZField = 32;

	// 頂点数の設定　頂点をブロックの数＋１分用意
	this->Attribute.NumVertex((this->FieldPara.nNumBlockXField + 1) * (this->FieldPara.nNumBlockZField + 1));

	// インデックス数の設定　TriangleList描画
	this->Attribute.NumVertexIndex((this->FieldPara.nNumBlockXField + 1) * 2 * this->FieldPara.nNumBlockZField + (this->FieldPara.nNumBlockZField - 1) * 2);

	// ポリゴン数の設定
	this->Attribute.NumPolygon(this->FieldPara.nNumBlockXField * this->FieldPara.nNumBlockZField * 2 + (this->FieldPara.nNumBlockZField - 1) * 4);

	// ブロックサイズの設定
	this->FieldPara.fBlockSizeXField = 250.0f;
	this->FieldPara.fBlockSizeZField = 250.0f;

	//　辺サイズの設定
	this->FieldPara.fSideSizeXMax = this->FieldPara.nNumBlockXField * this->FieldPara.fBlockSizeXField;
	this->FieldPara.fSideSizeZMax = this->FieldPara.nNumBlockZField * this->FieldPara.fBlockSizeZField;
	this->FieldPara.fSideSizeXHalf = this->FieldPara.fSideSizeXMax / 2.0f;
	this->FieldPara.fSideSizeZHalf = this->FieldPara.fSideSizeZMax / 2.0f;
	this->FieldPara.fSideSizeXQuarter = this->FieldPara.fSideSizeXHalf / 2.0f;
	this->FieldPara.fSideSizeZQuarter = this->FieldPara.fSideSizeZHalf / 2.0f;
	this->FieldPara.fSideSizeXEighth = this->FieldPara.fSideSizeXQuarter / 2.0f;
	this->FieldPara.fSideSizeZEighth = this->FieldPara.fSideSizeZQuarter / 2.0f;



	for (int CntField = 0; CntField < FIELD_VTX_MAX; CntField++)
	{
		//頂点作成
		this->vtx[CntField].MakeVertex3D(this->Attribute.NumVertex(), FVF_VERTEX_3D);
		this->vtx[CntField].MakeIdxVertex(sizeof(WORD) *this->Attribute.NumVertex());
	}








	//標準初期化
	this->FieldPara.InterPolationFieldSignal = true;
	this->FieldPara.InterPolationFieldSignalEnd = false;
	this->FieldPara.InterPolationFieldType = -1;
	this->FieldPara.InterPolationFieldPlayerNum = -1;


	//頂点設定
}

//=============================================================================
// デストラクタ　削除
//=============================================================================
FIELD::~FIELD(void)
{
	//テクスチャ解放
	this->tex.~TEXTURE();

	for (int CntField = 0; CntField < FIELD_VTX_MAX; CntField++)
	{
		//頂点解放
		this->vtx[CntField].~VTXBuffer();
	}
	//オブジェクトカウントダウン
	this->DeleteInstanceOBJ();
}

//=============================================================================
// 初期化処理
//=============================================================================
void FIELD::Init(void)
{
	this->FieldPara.InterPolationFieldSignal = true;
	this->FieldPara.InterPolationFieldSignalEnd = false;
	this->FieldPara.InterPolationFieldType = -1;
	this->FieldPara.InterPolationFieldPlayerNum = -1;
	this->FieldPara.TikeiSeed = 0;
	this->FieldPara.OldTikeiSeed = 0;
	this->FieldPara.ItemGetPlayerNum = -1;
	this->FieldPara.NetTikei = false;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//初期セット
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;
		VERTEX_3D *pVtxE;
#if 0
		const float texSizeX = 1.0f / nNumBlockXField;
		const float texSizeZ = 1.0f / nNumBlockZField;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		//頂点バッファのセット
		LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
		VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();
		LPDIRECT3DVERTEXBUFFER9 VtxBuffS;
		VtxBuffS = this->vtx[FIELD_VTX_START].VtxBuff();
		LPDIRECT3DVERTEXBUFFER9 VtxBuffE;
		VtxBuffE = this->vtx[FIELD_VTX_END].VtxBuff();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);
		VtxBuffS->Lock(0, 0, (void**)&pVtxS, 0);
		VtxBuffE->Lock(0, 0, (void**)&pVtxE, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				// 頂点座標の設定　描画用
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeXField + nCntVtxX * this->FieldPara.fBlockSizeXField;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.z = (this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeZField - nCntVtxZ * this->FieldPara.fBlockSizeZField;

				// 頂点座標の設定　補間用
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeXField + nCntVtxX * this->FieldPara.fBlockSizeXField;
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.z = (this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeZField - nCntVtxZ * this->FieldPara.fBlockSizeZField;
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.x = -(this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeXField + nCntVtxX * this->FieldPara.fBlockSizeXField;
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.z = (this->FieldPara.nNumBlockZField / 2.0f) * this->FieldPara.fBlockSizeZField - nCntVtxZ * this->FieldPara.fBlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// 頂点データをアンロックする
		VtxBuffS->Unlock();
		VtxBuffD->Unlock();
		VtxBuffE->Unlock();

		//セットアップしたデータを反映
		this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
		this->vtx[FIELD_VTX_START].VtxBuff(VtxBuffS);
		this->vtx[FIELD_VTX_END].VtxBuff(VtxBuffE);
	}

	//初期セット
	{//インデックスバッファの中身を埋める
		//頂点インデックスのセット
		LPDIRECT3DINDEXBUFFER9	IdxBuff;
		IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
		WORD *pIdx;
		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		for (int nCntVtxZ = 0; nCntVtxZ < this->FieldPara.nNumBlockZField; nCntVtxZ++)
		{
			if (nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1);
				nCntIdx++;
			}

			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if (nCntVtxZ < (this->FieldPara.nNumBlockZField - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + this->FieldPara.nNumBlockZField;
				nCntIdx++;
			}
		}
		// インデックスデータをアンロックする
		IdxBuff->Unlock();

		//セットアップしたデータを反映
		this->vtx[FIELD_VTX_DRAW].IdxBuff(IdxBuff);
	}

	//地形生成　Y値とカラーセット
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		VERTEX_3D *pVtxS;

		//頂点バッファのセット
		LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
		VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();
		LPDIRECT3DVERTEXBUFFER9 VtxBuffS;
		VtxBuffS = this->vtx[FIELD_VTX_START].VtxBuff();

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);
		VtxBuffS->Lock(0, 0, (void**)&pVtxS, 0);

		//上限
		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
				//pVtx[(rand() % this[0].model.nNumVertex)].vtx.y = float((rand() % 200));
				pVtx[(rand() % this->Attribute.NumVertex())].vtx.y = 0.0f;
			}
		}

		//縮退ポリゴンの座標を調整
		//SetDegenerationPoly();

		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				//縮退ポリゴンよけなさい
				if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
				else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

				// 頂点座標の設定
				//上側
				if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
					//pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
				}
				//中側
				else
				{
					float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
						pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 20.0f;
				}

				// 反射光の設定
				if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y >= 60.0f)
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
				}
				else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y <= 59.9f && pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y > 20.0f)
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
				}
				else
				{
					pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
				}
			}
		}

		// 頂点データをアンロックする
		VtxBuffS->Unlock();
		VtxBuffD->Unlock();

		//セットアップしたデータを反映
		this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
		this->vtx[FIELD_VTX_START].VtxBuff(VtxBuffS);
	}

	//当たり判定高速化用。一応親子関係も登録　未使用
	//InitCntPartition();

}

//=============================================================================
// 更新処理
//=============================================================================
void FIELD::Update(PLAYER *player,ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow)
{
	this->FieldPara.OldTikeiSeed = this->FieldPara.TikeiSeed;

	//アイテムを拾うとInterPolationFieldTypeが変わる。このスイッチ文SetFieldType関数最後に-1を入れる
	switch (this->FieldPara.InterPolationFieldType)
	{
		//地形変形するときはまず地形クリア
		this->vtx[FIELD_VTX_DRAW].VtxBuff(this->ClearField(this->vtx[FIELD_VTX_DRAW].VtxBuff()));
		//地形の種類によってアルゴリズムを分ける
	case FIELD_TYPE_BOKOBOKO:
		this->vtx[FIELD_VTX_END].VtxBuff(this->SetFieldType01(this->vtx[FIELD_VTX_END].VtxBuff()));
		break;
	case FIELD_TYPE_NADARAKA:
		this->vtx[FIELD_VTX_END].VtxBuff(this->SetFieldType02(this->vtx[FIELD_VTX_END].VtxBuff()));
		break;
	case FIELD_TYPE_PLAYERADVANTAGE:
		this->vtx[FIELD_VTX_END].VtxBuff(this->SetFieldType03(&player[0], this->vtx[FIELD_VTX_END].VtxBuff(), this->vtx[FIELD_VTX_DRAW].IdxBuff()));
		break;

		//処理後にフラグセット
		this->FieldPara.InterPolationFieldType = -1;
		this->FieldPara.InterPolationFieldSignal = false;
	}

	//SetFieldTypeでInterPolationFieldSignalをfalseにしている。InterPolationField()の返り値で地形の補間が終わったか判定
	if (this->FieldPara.InterPolationFieldSignal == false)
	{
		this->FieldPara.InterPolationFieldSignal = this->InterPolationField();
	}

	this->FieldHitCheck(player,item,bullet,explosion,shadow);
}

//=============================================================================
// 描画処理
//=============================================================================
void FIELD::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate,mtxWorld;
	D3DXVECTOR3 pos = this->Transform.Pos();
	D3DXVECTOR3 rot = this->Transform.Rot();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, this->vtx[FIELD_VTX_DRAW].VtxBuff(), 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(this->vtx[FIELD_VTX_DRAW].IdxBuff());

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, this->tex.Texture());

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, this->Attribute.NumVertex(), 0, this->Attribute.NumPolygon());
}

//=============================================================================
// 地形との当たり判定
//=============================================================================
void FIELD::FieldHitCheck(PLAYER *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow)
{
	//プレイヤーと地面の当たり判定
	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		//-------------------オブジェクト値読み込み
		D3DXVECTOR3 rayS = player->modelDraw[PLAYER_PARTS_TYPE_HOUDAI].Transform[CntPlayer].Pos();
		D3DXVECTOR3 rayG = rayS;
		D3DXVECTOR3 ReturnPos = rayS;
		D3DXVECTOR3 FieldNor = player->PostureVec[CntPlayer].FNVecFunc();
		rayS.y += 1000.0f;
		rayG.y -= 1000.0f;

		//判定
		this->FieldHitGetSphereVec(rayS, rayG, &FieldNor, &ReturnPos.y);

		//-------------------オブジェクト値書き込み
		player->modelDraw[PLAYER_PARTS_TYPE_HOUDAI].Transform[CntPlayer].Pos(ReturnPos);
		player->PostureVec[CntPlayer].FNVecFunc(FieldNor);
	}

	//アイテムと地面の当たり判定
	//-----------------------------------オブジェクト先頭アドレスを読み込み
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		bool use = item->iUseType[CntItem].Use();
		if (use == true)
		{
			//-------------------オブジェクト値読み込み
			D3DXVECTOR3 rayS = item->Transform[CntItem].Pos();
			D3DXVECTOR3 rayG = rayS;
			D3DXVECTOR3 ReturnPos = rayS;
			D3DXVECTOR3 FieldNor = item->PostureVec[CntItem].FNVecFunc();
			if (item->ItemParaAll[CntItem].CollisionFieldEnd != true || this->FieldPara.InterPolationFieldSignal == false)
			{
				//レイ調整
				rayS.y += 5.0f;
				rayG.y -= 5.0f;

				//判定
				item->ItemParaAll[CntItem].CollisionFieldEnd = this->FieldHitItem(rayS, rayG, &FieldNor, &ReturnPos.y);
			}
			else if (this->FieldPara.InterPolationFieldSignal == true && item->ItemParaAll[CntItem].CollisionFieldEnd == true)
			{
				//レイ調整
				rayS.y -= 1000.0f;
				rayG.y += 1000.0f;

				//判定
				this->FieldHit(rayS, rayG, &FieldNor, &ReturnPos.y);
			}
			//-------------------オブジェクト値書き込み
			item->Transform[CntItem].Pos(ReturnPos);
			item->PostureVec[CntItem].FNVecFunc(FieldNor);
		}
	}

	//バレットと地面の当たり判定
	for (int Cntbullet = 0; Cntbullet < OBJECT_BULLET_MAX; Cntbullet++)
	{
		bool buse = bullet->iUseType[Cntbullet].Use();
		if (buse == true)
		{
			D3DXVECTOR3 kari = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//-------------------オブジェクト値読み込み
			D3DXVECTOR3 rayS = bullet->Transform[Cntbullet].Pos();
			D3DXVECTOR3 rayG = rayS;
			rayS.y += 1000.0f;
			rayG.y -= 1000.0f;

			//判定
			this->FieldHit(rayS, rayG, &kari, &bullet->BulletPara[Cntbullet].FieldPosY);

			//地形との当たり判定を緩く(調整)するためのbullet[Cntbullet].FieldPosY-3.0f
			D3DXVECTOR3 bpos = bullet->Transform[Cntbullet].Pos();
			if (bullet->BulletPara[Cntbullet].FieldPosY - 3.0f > bpos.y)
			{
				// 爆発の生成
				D3DXVECTOR3 ExploPos = D3DXVECTOR3(bpos.x, bullet->BulletPara[Cntbullet].FieldPosY, bpos.z);
				explosion->SetInstance(ExploPos, 40.0f, 40.0f, eEXPLOSION_TYPE(bullet->BulletPara[Cntbullet].UsePlayerType), PLAYER_COLOR[bullet->BulletPara[Cntbullet].UsePlayerType]);
				// バレット破棄
				bullet->ReleaseInstance(Cntbullet, shadow);
				// SE再生
				PlaySound(SOUND_LABEL_SE_damage);
			}
		}
	}
}

//=============================================================================
// 地形変形タイプ
//=============================================================================
void FIELD::SetFieldInterPolationFieldType(int type,int CntPlayer,ITEM *item)
{
	//フラグセット
	this->FieldPara.InterPolationFieldType = type;
	this->FieldPara.InterPolationFieldPlayerNum = CntPlayer;
	//-----------------------------------オブジェクト先頭アドレスを読み込み
	for (int CntItem = 0; CntItem < OBJECT_ITEM_MAX; CntItem++)
	{
		item->ItemParaAll[CntItem].CollisionFieldEnd = false;
	}
}

//=============================================================================
// 地形のクリア
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::ClearField(LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	VERTEX_3D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			// 頂点座標の設定
			pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 0.0f;
		}
	}
	VtxBuff->Unlock();
	return VtxBuff;
}

//=============================================================================
// 地形の自動生成01 ぼこぼこぎみ地形　ブロック数32*32　ブロックサイズ60*60～100*100
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::SetFieldType01(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd)
{
	VERTEX_3D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	//上限　高さを設定
	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertex() / 4); nCntVtx++)
	{
		//高さを決める頂点を決定
		int YTXrandNum(rand() % this->Attribute.NumVertex());

		//高さを決め代入 縮退ポリゴンも同時に動かす
		int VTXrandY(rand() % 200);
		if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
		{
			pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
		{
			pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		pVtx[YTXrandNum].vtx.y = float((VTXrandY / 1.0f));
	}

	//縮退ポリゴンの座標を調整
	//SetDegenerationPoly();

	//高さ調整　中点変位処理
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//縮退ポリゴンよけなさい
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

			// 頂点座標の設定
			//頂点最端の高さは固定。壁際の頂点のこと。
			if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 50.0f;
			//中側
			//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
			else
			{
				float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
					pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = y;
			}
		}
	}
	// 頂点データをアンロックする
	VtxBuffEnd->Unlock();
	return VtxBuffEnd;
}

//=============================================================================
// 地形の自動生成02　見通しのいい地形　ブロック数32*32　ブロックサイズ60*60～100*100
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::SetFieldType02(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd)
{
	VERTEX_3D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	//上限　高さを設定
	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertex() / 4); nCntVtx++)
	{
		//高さを決める頂点を決定
		int YTXrandNum(rand() % this->Attribute.NumVertex());

		//高さを決め代入 縮退ポリゴンも同時に動かす
		int VTXrandY(rand() % 200);
		if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
		{
			pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
		{
			pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float((VTXrandY / 1.0f));
			continue;
		}
		pVtx[YTXrandNum].vtx.y = float((VTXrandY / 1.0f));
	}

	//縮退ポリゴンの座標を調整
	//SetDegenerationPoly();

	//高さ調整　中点変位処理
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//縮退ポリゴンよけなさい
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

			// 頂点座標の設定
			//頂点最端の高さは固定。壁際の頂点のこと。
			//上側
			if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 100.0f;
			//中側　上下左右の平均値を算出
			//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
			else
			{
				float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
					pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = float(fabs(y));
			}
		}
	}
	// 頂点データをアンロックする
	VtxBuffEnd->Unlock();
	return VtxBuffEnd;
}

//=============================================================================
// 地形の自動生成03　取得プレイヤーが有利になる地形(相手プレイヤー付近を盆地)　　ブロック数32*32　ブロックサイズ250*250
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 FIELD::SetFieldType03(PLAYER *player, LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd, LPDIRECT3DINDEXBUFFER9 IdxBuff)
{
	//高さを決める
	VERTEX_3D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);

	//上限　高さを設定
	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertex()); nCntVtx++)
	{
		//高さを決める頂点を決定　あえて引数を中途半端な数にして出力に幅を持たせる
		int YTXrandNum = MyRandFunc(&this->FieldPara.TikeiSeed, this->Attribute.NumVertex() - 103);
		//高さを決め代入
		int VTXrandY = MyRandFunc(&this->FieldPara.TikeiSeed, 200);

		VTXrandY += 20;//オフセット
		if (pVtx[YTXrandNum].vtx == pVtx[YTXrandNum + 1].vtx)
		{
			pVtx[YTXrandNum].vtx.y = pVtx[YTXrandNum + 1].vtx.y = float(VTXrandY);
			continue;
		}
		else if (pVtx[YTXrandNum + 1].vtx == pVtx[YTXrandNum + 2].vtx)
		{
			pVtx[YTXrandNum + 1].vtx.y = pVtx[YTXrandNum + 2].vtx.y = float(VTXrandY);
			continue;
		}
		pVtx[YTXrandNum].vtx.y = float(VTXrandY);
	}

	//縮退ポリゴンの座標を調整
	//SetDegenerationPoly();

	//隣接頂点の高さ平均値を(ダイアモンドスクエア、フラクタルを参考)
	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//縮退ポリゴンよけなさい
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)	continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;
			// 頂点座標の設定
			//頂点最端の高さは固定。壁際の頂点のこと。
			if (nCntVtxZ == 0 || nCntVtxX == 0 || nCntVtxZ == this->FieldPara.nNumBlockZField || nCntVtxX == this->FieldPara.nNumBlockZField)
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = 200.0f;
			//中側　上下左右の平均値を算出
			//隣接頂点の高さの平均値を求め、中心の頂点の高さとする。
			else
			{
				float y = (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX - 1].vtx.y + pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx.y +
					pVtx[(nCntVtxZ - 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y + pVtx[(nCntVtxZ + 1) * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y) / 4.0f;
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y = fabsf(y);
			}
		}
	}
	// 頂点データをアンロックする
	VtxBuffEnd->Unlock();

	//相手プレイヤー付近の地形を盆地化
	VERTEX_3D *pVtxP;
	WORD *pIdx;
	VtxBuffEnd->Lock(0, 0, (void**)&pVtxP, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);		//頂点インデックスバッファは共通で1つしかないのでDrawのやつを使っている

	for (int CntPlayer = 0; CntPlayer < OBJECT_PLAYER_MAX; CntPlayer++)
	{
		if (CntPlayer != this->FieldPara.InterPolationFieldPlayerNum)
		{
			//最小単位(高速化判定)の座標
			float HitPosUp;
			float HitPosDown;
			float HitPosLeft;
			float HitPosRight;
			HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

			//プレイヤーの乗っているエリアを特定。4分木で範囲を絞る。
			//-------------------オブジェクト値読み込み
			D3DXVECTOR3 pos = player->modelDraw[PLAYER_PARTS_TYPE_HOUDAI].Transform[CntPlayer].Pos();
			//高速化当たり判定
			SpeedUpFieldHitPoly(pos, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
				this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth,
				this->FieldPara.fSideSizeXEighth / 2, this->FieldPara.fSideSizeZEighth / 2);

			for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
			{
				//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
				if (nCntVtx == this->Attribute.NumVertexIndex() - 2) break;
				else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])continue;
				else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])continue;
				//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
				if (pVtxP[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtxP[pIdx[nCntVtx]].vtx.x <= HitPosRight)
				{
					if (pVtxP[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtxP[pIdx[nCntVtx]].vtx.z >= HitPosDown)
					{
						// 頂点座標の設定
						pVtxP[pIdx[nCntVtx]].vtx.y = 20.0f;
					}
				}
			}
		}
	}

	// 頂点データをアンロックする
	VtxBuffEnd->Unlock();
	// インデックスデータをアンロックする
	IdxBuff->Unlock();
	return VtxBuffEnd;
}

//=============================================================================
// 地形の変形処理　生成されたフィールドに変形する
//=============================================================================
bool FIELD::InterPolationField(void)
{
	VERTEX_3D *pVtx;
	VERTEX_3D *pVtxS;
	VERTEX_3D *pVtxE;

	//頂点バッファのセット
	LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
	VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DVERTEXBUFFER9 VtxBuffS;
	VtxBuffS = this->vtx[FIELD_VTX_START].VtxBuff();
	LPDIRECT3DVERTEXBUFFER9 VtxBuffE;
	VtxBuffE = this->vtx[FIELD_VTX_END].VtxBuff();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);
	VtxBuffS->Lock(0, 0, (void**)&pVtxS, 0);
	VtxBuffE->Lock(0, 0, (void**)&pVtxE, 0);

	float dt = 1.0f / 60.0f;	// 1フレームで進める時間
	this->FieldPara.time += dt;

	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//縮退ポリゴンよけなさい
			if (nCntVtxZ*nCntVtxX == this->Attribute.NumVertexIndex() - 2) break;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx) continue;
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx) continue;

			//補間開始
			pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx = MyInterpolation(
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx,
				pVtxE[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx,
				0.0, 1.0f, dt, this->FieldPara.time, LerpEaseInEaseOut);

			// 反射光の設定
			if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y >= FIELD_Y_HIGH)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);//山
			}
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y < FIELD_Y_HIGH && pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx.y > FIELD_Y_LOW)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);//緑
			}
			else
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].diffuse = D3DXCOLOR(0.78f, 0.76f, 0.63f, 1.0f);//砂
			}
		}
	}
	if (this->FieldPara.time >= 1.0f)
	{
		//変形が終了したら現在描画している地形をpD3DVtxBuffFieldStartへ保存。次の変形開始時の地形にする
		for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
			{
				pVtxS[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx = pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx;
			}
		}
		this->FieldPara.time = 0.0f;
		this->FieldPara.InterPolationFieldSignalEnd = true;
	}
	// 頂点データをアンロックする
	VtxBuffS->Unlock();
	VtxBuffD->Unlock();
	VtxBuffE->Unlock();

	//セットアップしたデータを反映
	this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
	this->vtx[FIELD_VTX_START].VtxBuff(VtxBuffS);
	this->vtx[FIELD_VTX_END].VtxBuff(VtxBuffE);

	if (this->FieldPara.InterPolationFieldSignalEnd == true)
	{
		this->FieldPara.InterPolationFieldSignalEnd = false;
		StopSound(SOUND_LABEL_SE_quake);
		this->FieldPara.NetTikei = false;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// 縮退ポリゴンの座標を再計算する関数　地形変形後、縮退ポリゴンの座標を設定する
//=============================================================================
void FIELD::SetDegenerationPoly(void)
{
	VERTEX_3D *pVtx;
	//頂点バッファのセット
	LPDIRECT3DVERTEXBUFFER9 VtxBuffD;
	VtxBuffD = this->vtx[FIELD_VTX_DRAW].VtxBuff();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VtxBuffD->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (this->FieldPara.nNumBlockZField + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (this->FieldPara.nNumBlockZField + 1); nCntVtxX++)
		{
			//縮退ポリゴンの
			if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX].vtx = pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx;
			}
			else if (pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx == pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx)
			{
				pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 1].vtx = pVtx[nCntVtxZ * (this->FieldPara.nNumBlockZField + 1) + nCntVtxX + 2].vtx;
			}
		}
	}
	// 頂点データをアンロックする
	VtxBuffD->Unlock();
	//セットアップしたデータを反映
	this->vtx[FIELD_VTX_DRAW].VtxBuff(VtxBuffD);
}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値void バレットの当たり判定で使用
//=============================================================================
void FIELD::FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//当たり判定高速化用の座標四隅
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//高速化処理。4分木で当たり判定をする範囲を絞る。
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight, 
		this->FieldPara.fSideSizeXHalf, this->FieldPara.fSideSizeZHalf,
		this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DINDEXBUFFER9 IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
	{
		//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		if (nCntVtx == this->Attribute.NumVertexIndex() - 2)break;
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])continue;
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])continue;
		//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
		else if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
		{
			if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
			{
				// 頂点座標の設定
				D3DXVECTOR3 vtx[3];
				D3DXVECTOR3 vtxHen[2];
				D3DXVECTOR3 NaibunPos;

				//-----------------------オブジェクト読み込み
				D3DXVECTOR3 pos = this->Transform.Pos();
				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

				//レイキャストで調べる。trueで座標yを変更
				if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
				{
					vtxHen[0] = vtx[1] - vtx[0];
					vtxHen[1] = vtx[2] - vtx[0];

					//プレイやの傾く軸(地形の法線)
					D3DXVECTOR3 vtxHOUSEN;
					D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
					if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
					*vtxNor = vtxHOUSEN;
					*posY = NaibunPos.y + 1.0f;
					break;
				}
			}
		}
	}

	// 頂点データをアンロックする
	VtxBuffEnd->Unlock();
	// インデックスデータをアンロックする
	IdxBuff->Unlock();

}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値void　プレイヤーの当たり判定で使用。球面補間で使用するため。
//=============================================================================
void FIELD::FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//当たり判定高速化用の座標四隅
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//高速化処理。4分木で当たり判定をする範囲を絞る。
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
		this->FieldPara.fSideSizeXHalf, this->FieldPara.fSideSizeZHalf,
		this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DINDEXBUFFER9 IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
	{
		//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		if (nCntVtx == this->Attribute.NumVertexIndex() - 2)break;
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])continue;
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])continue;
		//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
		else if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
		{
			if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
			{
				// 頂点座標の設定
				D3DXVECTOR3 vtx[3];
				D3DXVECTOR3 vtxHen[2];
				D3DXVECTOR3 NaibunPos;

				//-----------------------オブジェクト読み込み
				D3DXVECTOR3 pos = this->Transform.Pos();
				vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
				vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
				vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

				//レイキャストで調べる。trueで座標yを変更
				if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
				{
					//プレイヤーの傾く軸(プレイヤーが乗っている地形の法線)
					vtxHen[0] = vtx[1] - vtx[0];
					vtxHen[1] = vtx[2] - vtx[0];
					D3DXVECTOR3 vtxHOUSEN;
					D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
					if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
					*vtxNor = vtxHOUSEN;
					*posY = NaibunPos.y + 1.0f;

					//pVtx[pIdx[nCntVtx]]=今の頂点
					//乗ってるポリゴンがわかったらそこから上下左右のポリゴン法線を計算する
					//ポリゴンは三角形なので隣接辺は3つ(隣接ポリゴンは3つ)　
					//ポリゴンの最端値(ブロックx,zの切り返し番号)だったら、最端値側のポリゴン法線はプレイヤーが乗ってるポリゴン法線と同じ
					//上　-1
					//下　+1
					//左右　同じ座標にある頂点

					//改良ポイント
					//今の頂点z座標と比べ、一個次の頂点z座標が大きければ左下直角三角形に乗っている
					if (pVtx[pIdx[nCntVtx]].vtx.z < pVtx[pIdx[nCntVtx + 1]].vtx.z)
					{
						//特別な頂点
						/*
						if (nCntVtx == this[0].model.nNumVertexIndex - this->FieldPara.nNumBlockZField * 2)
						{
							//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
							float MinDistance = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							if (SPHERELINEAR_LEN > MinDistance)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}

						//　左　左下直角三角形で今の頂点番号がthis->FieldPara.nNumBlockZField以下ならば、地面の最左端にいる
						//特別な頂点
						//else if (nCntVtx >= this->FieldPara.nNumBlockZField)
						//{
						//	int a = 0;
						//}
						//　上　左下直角三角形で今の頂点座標と一個前の頂点座標が同じもしくは、一個前の頂点座標と二個前の頂点座標が同じならば、地面の最上端にいる
						//特別な頂点
						//特別な頂点
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx - 1]].vtx || pVtx[pIdx[nCntVtx - 1]].vtx == pVtx[pIdx[nCntVtx - 2]].vtx)
						{
							int a = 0;

						}
						//　下　左下直角三角形で今の頂点座標と一個次の頂点座標が同じもしくは、一個次の頂点座標と二個次の頂点座標が同じならば、地面の最下端にいる(縮退ポリゴンなのでtrueにならないはず)
						//特別な頂点
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx + 1]].vtx || pVtx[pIdx[nCntVtx + 1]].vtx == pVtx[pIdx[nCntVtx + 2]].vtx)
						{
							int a = 0;
						}
						*/

						//通常の頂点
						//else
						{
							//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
							//プレイヤーの座標から乗っているポリゴンの各3辺までの最短距離MinDistanceを計算する
							float MinDistanceRightUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							float MinDistanceDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							float MinDistanceLeft = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
							//プレイヤー座標から2辺までの距離がSPHERELINEAR_LENより短いと、その先に続く二つの地形の法線平均値をとり、その値で補間させる
							//プレイヤーが3角ポリゴン境界線付近にいるときにいずれかのif()に入る。if(&&)の条件が二つあるものはポリゴン境界線が2辺に対して近い時に実行される
							if (SPHERELINEAR_LEN > MinDistanceRightUp && SPHERELINEAR_LEN > MinDistanceDown)
							{
								// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRightUp
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceDown
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//二つの法線平均値を求める
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//競合している二つの距離の平均を求める
								float MinRtionDistance = (MinDistanceRightUp + MinDistanceDown) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
							else if (SPHERELINEAR_LEN > MinDistanceRightUp && SPHERELINEAR_LEN > MinDistanceLeft)
							{
								// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRightUp
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeft
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//二つの法線平均値を求める
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//競合している二つの距離の平均を求める
								float MinRtionDistance = (MinDistanceRightUp + MinDistanceLeft) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}
							else if (SPHERELINEAR_LEN > MinDistanceDown && SPHERELINEAR_LEN > MinDistanceLeft)
							{
								// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceDown
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeft
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//二つの法線平均値を求める
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//競合している二つの距離の平均を求める
								float MinRtionDistance = (MinDistanceDown + MinDistanceLeft) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}
							//1辺しか干渉していない
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceRightUp)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx + 1]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceRightUp) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//
							else if (SPHERELINEAR_LEN > MinDistanceDown)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + (this->FieldPara.nNumBlockZField * 2) + 6]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceDown) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceLeft)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceLeft) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}
					}
					//それ以外は右上直角三角形に乗っている
					else
					{
						//特別な頂点番号
						/*
						if (nCntVtx == this[0].model.nNumVertexIndex - this->FieldPara.nNumBlockZField * 2)
						{
							//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
							float MinDistance = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
							if (SPHERELINEAR_LEN > MinDistance)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx]].vtx - pVtx[pIdx[nCntVtx - 1]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx - 1]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}
						//　上　右上直角三角形で今の頂点座標と一個前の頂点座標が同じもしくは、一個前の頂点座標と二個前の頂点座標が同じならば、地面の最上端にいる
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx - 1]].vtx || pVtx[pIdx[nCntVtx - 1]].vtx == pVtx[pIdx[nCntVtx - 2]].vtx)
						{
							//縮退ポリゴン
						}
						//　下　右上直角三角形今の頂点座標と一個次の頂点座標が同じもしくは、一個次の頂点座標と二個次の頂点座標が同じならば、地面の最下端にいる(縮退ポリゴンなのでtrueにならないはず)
						else if (pVtx[pIdx[nCntVtx]].vtx == pVtx[pIdx[nCntVtx + 1]].vtx || pVtx[pIdx[nCntVtx + 1]].vtx == pVtx[pIdx[nCntVtx + 2]].vtx)
						{

						}
						//　右　右上直角三角形で今の頂点番号がthis->FieldPara.nNumBlockZField以上ならば、地面の最右端にいる
						//else if (nCntVtx >= this->FieldPara.nNumBlockZField * this->FieldPara.nNumBlockZField)
						//{

						//}
						*/

						//else
						{
							//頂点境界辺に近づくと球面線形補間で姿勢ベクトルを制御
							float MinDistanceRight = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 2]].vtx, pVtx[pIdx[nCntVtx + 3]].vtx);
							float MinDistanceUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 3]].vtx);
							float MinDistanceLeftDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							//float MinDistanceRight = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx + 1]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							//float MinDistanceUp = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 2]].vtx);
							//float MinDistanceLeftDown = PointAndLineMinDistance(InrayS, pVtx[pIdx[nCntVtx]].vtx, pVtx[pIdx[nCntVtx + 1]].vtx);
							//プレイヤー座標から2辺までの距離がSPHERELINEAR_LENより短いと、その先に続く二つの地形の法線平均値をとり、その値で補間させる
							if (SPHERELINEAR_LEN > MinDistanceRight && SPHERELINEAR_LEN > MinDistanceUp)
							{
								// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRight
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceUp
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//二つの法線平均値を求める
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//競合している二つの距離の平均を求める
								float MinRtionDistance = (MinDistanceRight + MinDistanceUp) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;
							}
							else if (SPHERELINEAR_LEN > MinDistanceRight && SPHERELINEAR_LEN > MinDistanceLeftDown)
							{
								// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceRight
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeftDown
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//二つの法線平均値を求める
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//競合している二つの距離の平均を求める
								float MinRtionDistance = (MinDistanceRight + MinDistanceLeftDown) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}
							else if (SPHERELINEAR_LEN > MinDistanceUp && SPHERELINEAR_LEN > MinDistanceLeftDown)
							{
								// 1つめ　辺先にある地面ポリゴンの法線を求める MinDistanceUp
								D3DXVECTOR3 NextPolyNorVec1;
								D3DXVec3Cross(&NextPolyNorVec1, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec1.y <= 0) NextPolyNorVec1 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec1, &NextPolyNorVec1);

								// 2つめ　辺先にある地面ポリゴンの法線を求める MinDistanceLeftDown
								D3DXVECTOR3 NextPolyNorVec2;
								D3DXVec3Cross(&NextPolyNorVec2, &D3DXVECTOR3(pVtx[pIdx[nCntVtx - (this->FieldPara.nNumBlockZField * 2 + 6)]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec2.y <= 0) NextPolyNorVec2 *= -1;
								D3DXVec3Normalize(&NextPolyNorVec2, &NextPolyNorVec2);

								//二つの法線平均値を求める
								D3DXVECTOR3 AveNextPolyNorVec = (NextPolyNorVec1 + NextPolyNorVec2) / 2.0f;

								//競合している二つの距離の平均を求める
								float MinRtionDistance = (MinDistanceUp + MinDistanceLeftDown) / 2.0f;

								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &AveNextPolyNorVec, (SPHERELINEAR_LEN - MinRtionDistance) / 10);
								*vtxNor = NewPolyNorVec;

							}

							//1辺しか干渉していない
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceRight)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceRight) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceUp)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 2]].vtx - pVtx[pIdx[nCntVtx]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceUp) / 10);
								*vtxNor = NewPolyNorVec;
							}
							//OK
							else if (SPHERELINEAR_LEN > MinDistanceLeftDown)
							{
								//辺先にある地面ポリゴンの法線を求める
								D3DXVECTOR3 NextPolyNorVec;
								D3DXVec3Cross(&NextPolyNorVec, &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 1]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx), &D3DXVECTOR3(pVtx[pIdx[nCntVtx + 3]].vtx - pVtx[pIdx[nCntVtx + 2]].vtx));
								if (NextPolyNorVec.y <= 0) NextPolyNorVec *= -1;
								D3DXVec3Normalize(&NextPolyNorVec, &NextPolyNorVec);
								D3DXVECTOR3 NowPolyNorVec;
								D3DXVec3Normalize(&NowPolyNorVec, &vtxHOUSEN);
								D3DXVECTOR3 NewPolyNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								//SPHERELINEAR_LEN(5)-MinDistance(0～5)*10 = 補間倍率(0～0.5の範囲)
								SphereLinear(&NewPolyNorVec, &NowPolyNorVec, &NextPolyNorVec, (SPHERELINEAR_LEN - MinDistanceLeftDown) / 10);
								*vtxNor = NewPolyNorVec;
							}
						}
					}

					//プレイヤーの座標がポリゴンのつなぎ目付近にくると、そのポリゴン法線同士で球面線形補間
					//補間で求めた中間ベクトルがプレイヤーの姿勢ベクトルになる

					break;
				}
			}
		}
	}

	// 頂点データをアンロックする
	VtxBuffEnd->Unlock();
	// インデックスデータをアンロックする
	IdxBuff->Unlock();

}

//=============================================================================
// 地形との当たり判定 rayS,rayG,vtxHOUSEN,posy 返り値bool アイテムの当たり判定で使用
//=============================================================================
bool FIELD::FieldHitItem(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY)
{
	bool Cheack = false;
	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;
	WORD *pIdx;
	//当たり判定高速化用の座標四隅
	float HitPosUp;
	float HitPosDown;
	float HitPosLeft;
	float HitPosRight;
	HitPosUp = HitPosDown = HitPosLeft = HitPosRight = 0.0f;

	//高速化処理。4分木で当たり判定をする範囲を絞る。
	SpeedUpFieldHitPoly(InrayS, &HitPosUp, &HitPosDown, &HitPosLeft, &HitPosRight,
		this->FieldPara.fSideSizeXHalf, this->FieldPara.fSideSizeZHalf, 
		this->FieldPara.fSideSizeXEighth, this->FieldPara.fSideSizeZEighth);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd = this->vtx[FIELD_VTX_DRAW].VtxBuff();
	LPDIRECT3DINDEXBUFFER9 IdxBuff = this->vtx[FIELD_VTX_DRAW].IdxBuff();
	VtxBuffEnd->Lock(0, 0, (void**)&pVtx, 0);
	IdxBuff->Lock(0, 0, (void**)&pIdx, 0);


	for (int nCntVtx = 0; nCntVtx < int(this->Attribute.NumVertexIndex()); nCntVtx++)
	{
		//縮退ポリゴンのときはコンティニュー。最終ポリゴンの時はbreak;
		if (nCntVtx == this->Attribute.NumVertexIndex() - 2)
		{
			break;
		}
		else if (pIdx[nCntVtx] == pIdx[nCntVtx + 1])
		{
			continue;
		}
		else if (pIdx[nCntVtx + 1] == pIdx[nCntVtx + 2])
		{
			continue;
		}
		else if (InrayS.y <= 400.0f)
		{
			//高速当たり判定用ポリゴンの座標内なら当たり判定実行　XチェックからZチェック。ともにtrueだと判定
			if (pVtx[pIdx[nCntVtx]].vtx.x >= HitPosLeft && pVtx[pIdx[nCntVtx]].vtx.x <= HitPosRight)
			{
				if (pVtx[pIdx[nCntVtx]].vtx.z <= HitPosUp && pVtx[pIdx[nCntVtx]].vtx.z >= HitPosDown)
				{
					// 頂点座標の設定
					D3DXVECTOR3 vtx[3];
					D3DXVECTOR3 vtxHen[2];
					D3DXVECTOR3 NaibunPos;

					//-----------------------オブジェクト読み込み
					D3DXVECTOR3 pos = this->Transform.Pos();
					vtx[0] = pVtx[pIdx[nCntVtx]].vtx + pos;
					vtx[1] = pVtx[pIdx[nCntVtx + 1]].vtx + pos;
					vtx[2] = pVtx[pIdx[nCntVtx + 2]].vtx + pos;

					//レイキャストで調べる。trueで座標yを変更
					if (RayCast(InrayS, InrayG, vtx[0], vtx[1], vtx[2], &NaibunPos))
					{
						vtxHen[0] = vtx[1] - vtx[0];
						vtxHen[1] = vtx[2] - vtx[0];

						//プレイやの傾く軸(地形の法線)
						D3DXVECTOR3 vtxHOUSEN;
						D3DXVec3Cross(&vtxHOUSEN, &vtxHen[0], &vtxHen[1]);
						if (vtxHOUSEN.y <= 0) vtxHOUSEN *= -1;
						*vtxNor = vtxHOUSEN;
						*posY = NaibunPos.y + 1.0f;
						Cheack = true;
					}
				}
			}
		}
	}

	// 頂点データをアンロックする
	VtxBuffEnd->Unlock();
	// インデックスデータをアンロックする
	IdxBuff->Unlock();
	if (Cheack == true) return true;

	return false;
}


