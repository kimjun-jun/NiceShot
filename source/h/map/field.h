/**
* @file field.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
 * @enum FIELD_TYPE
 * FIELDの種類
 */
enum FIELD_TYPE
{
	FIELD_TYPE_BOKOBOKO,
	FIELD_TYPE_NADARAKA,
	FIELD_TYPE_PLAYERADVANTAGE,
};

/**
 * @enum FIELD_VTX_TYPE
 * FIELD頂点の種類　「描画用」変形時の「開始」と「終了」頂点
 */
enum FIELD_VTX_TYPE
{
	FIELD_VTX_DRAW = 0,
	FIELD_VTX_START,
	FIELD_VTX_END,
	FIELD_VTX_MAX,
};

/**
*　@class FIELD_PARAMETER
*　@brief
*/
class FIELD_PARAMETER
{
public:
	FIELD_PARAMETER() {
		fBlockSizeXField = fBlockSizeZField = 0.0f; fSideSizeXMax = fSideSizeZMax = 0.0f; fSideSizeXHalf = fSideSizeZHalf = 0.0f;
		fSideSizeXQuarter = fSideSizeZQuarter = 0.0f; fSideSizeXEighth = fSideSizeZEighth = 0.0f;
		time = 0.0f; nNumBlockXField = nNumBlockZField = 0; InterPolationFieldType = 0;
		InterPolationFieldPlayerNum = 0; TikeiSeed = OldTikeiSeed = 0; ItemGetPlayerNum = -1;
		InterPolationFieldSignal = false; InterPolationFieldSignalEnd = false; NetTikei = false;
	}
	~FIELD_PARAMETER() { }

	float fBlockSizeXField, fBlockSizeZField;		//!< ブロックサイズ
	float fSideSizeXMax, fSideSizeZMax;				//!< 辺サイズ最大 1920
	float fSideSizeXHalf, fSideSizeZHalf;			//!< 辺サイズ1/2   960
	float fSideSizeXQuarter, fSideSizeZQuarter;		//!< 辺サイズ1/4   480
	float fSideSizeXEighth, fSideSizeZEighth;		//!< 辺サイズ1/8   240
													  
	//地形変形変数群								  
	float time;										//!< 地形の変形時間
	int nNumBlockXField, nNumBlockZField;			//!< ブロック数
	int InterPolationFieldType;						//!< 地形変形タイプ
	int InterPolationFieldPlayerNum;				//!< 地形変形アイテムを取得したプレイヤーナンバー
	int TikeiSeed;									//!< 地形変形ランダム値のseed値　通信の時に同期させる
	int OldTikeiSeed;								//!< 地形変形ランダム値のseed値　通信の時に同期させる
	int ItemGetPlayerNum;							//!< 地形アイテムを取得したプレイヤー番号
	bool InterPolationFieldSignal;					//!< 地形変形信号
	bool InterPolationFieldSignalEnd;				//!< 地形変形信号終了したかどうか
	bool NetTikei;									//!< 通信対戦中に地形変更をしているかどうか

};


/**
*　@class TITLE
*　@brief GAMEOBJECT派生クラス
*/
class FIELD : public GAME_OBJECT
{
public:
	FIELD();	//!< データ読み込み　初期化
	~FIELD();	//!< 削除

	void Init(void);	//!< 初期化
	void Update(PLAYER *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow);//!< 更新
	void Draw(void);	//!< 描画

	void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	bool FieldHitItem(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	void SetFieldInterPolationFieldType(int type, int CntPlayer, ITEM *item);

	FIELD_PARAMETER		FieldPara;						//!< 

private:
	ModelAttribute		Attribute;						//!< モデル情報　マテリアルや頂点数など　複数使用するならここを配列化
	TEXTURE				tex;							//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX
	VTXBuffer			vtx[FIELD_VTX_MAX];				//!< 頂点情報　複数使用するならここを配列化
	TransForm			Transform;						//!< トランスフォーム情報


	LPDIRECT3DVERTEXBUFFER9 ClearField(LPDIRECT3DVERTEXBUFFER9 VtxBuff);		//!< 頂点設定初期化
	LPDIRECT3DVERTEXBUFFER9 SetFieldType01(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd);	//!< ぼこぼこ地形
	LPDIRECT3DVERTEXBUFFER9 SetFieldType02(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd);							//!< なだらか地形
	LPDIRECT3DVERTEXBUFFER9 SetFieldType03(PLAYER *player, LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd, LPDIRECT3DINDEXBUFFER9 IdxBuff);			//!< プレイヤー有利 
	void FieldHitCheck(PLAYER *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow);//!< 地形との当たり判定　高さ姿勢を計算している

	bool InterPolationField(void);						//!< 地形を徐々に補間変化させる
	void SetDegenerationPoly(void);						//!< 縮退ポリゴンの座標を再計算する関数　地形変形後、縮退ポリゴンの座標を設定する
};

