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

class FIELD;

/**
*　@class TITLE
*　@brief GAMEOBJECT派生クラス
*/
class FIELD_PARAMETER
{
	FIELD_PARAMETER() {
		pD3DVtxBuffFieldDraw = NULL; pD3DVtxBuffFieldEnd = NULL; pD3DVtxBuffFieldStart = NULL;
		fBlockSizeXField = fBlockSizeZField = 0.0f; fSideSizeXMax = fSideSizeZMax = 0.0f; fSideSizeXHalf = fSideSizeZHalf = 0.0f;
		fSideSizeXQuarter = fSideSizeZQuarter = 0.0f; fSideSizeXEighth = fSideSizeZEighth = 0.0f;
		time = 0.0f; nNumBlockXField = nNumBlockZField = 0; InterPolationFieldType = 0;
		InterPolationFieldPlayerNum = 0; TikeiSeed = OldTikeiSeed = 0; GetPlayerNum = -1;
		InterPolationFieldSignal = false; InterPolationFieldSignalEnd = false; NetTikei = false;
	};
	~FIELD_PARAMETER() { SAFE_RELEASE(pD3DVtxBuffFieldDraw); SAFE_RELEASE(pD3DVtxBuffFieldEnd); SAFE_RELEASE(pD3DVtxBuffFieldStart); }

	//フィールド生成用
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffFieldDraw;		// 頂点バッファインターフェースへのポインタ描画用
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffFieldEnd;		// 頂点バッファインターフェースへのポインタ補間データエンドバッファ
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffFieldStart;		// 頂点バッファインターフェースへのポインタ補間データスタートバッファ
	float fBlockSizeXField, fBlockSizeZField;			// ブロックサイズ
	float fSideSizeXMax, fSideSizeZMax;					// 辺サイズ最大 1920
	float fSideSizeXHalf, fSideSizeZHalf;				// 辺サイズ1/2   960
	float fSideSizeXQuarter, fSideSizeZQuarter;			// 辺サイズ1/4   480
	float fSideSizeXEighth, fSideSizeZEighth;			// 辺サイズ1/8   240

	//地形変形変数群
	float time;											// 地形の変形時間
	int nNumBlockXField, nNumBlockZField;				// ブロック数
	int InterPolationFieldType;							// 地形変形タイプ
	int InterPolationFieldPlayerNum;					// 地形変形アイテムを取得したプレイヤーナンバー
	bool InterPolationFieldSignal;						// 地形変形信号
	bool InterPolationFieldSignalEnd;					// 地形変形信号終了したかどうか
	FIELD *Next;										// 今後使用するかも
	int TikeiSeed;									// 地形変形ランダム値のseed値　通信の時に同期させる
	int OldTikeiSeed;								// 地形変形ランダム値のseed値　通信の時に同期させる
	int GetPlayerNum;								// 地形アイテムを取得したプレイヤー番号
	bool NetTikei;									// 通信対戦中に地形変更をしているかどうか

};


/**
*　@class TITLE
*　@brief GAMEOBJECT派生クラス
*/
class FIELD : public GAME_OBJECT
{
public:
	FIELD();	//!< データ読み込み
	~FIELD();	//!< 削除
	void Init(void);
	void Update(PLAYER_HONTAI *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow);
	void Draw(void);

	void ClearField(void);				//初期化
	void SetFieldType01(void);			//オリジナル1 
	void SetFieldType02(void);			//オリジナル2 
	void SetFieldType03(PLAYER_HONTAI *player);			//オリジナル3 
	bool InterPolationField(void);		//地形を徐々に変化させる
	void SetDegenerationPoly(void);		//縮退ポリゴンの座標を再計算する関数　地形変形後、縮退ポリゴンの座標を設定する
	void SetFieldInterPolationFieldType(int type, int CntPlayer,ITEM *item);
	void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	bool FieldHitItem(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);


private:
	FIELD_PARAMETER FieldPara;

};

