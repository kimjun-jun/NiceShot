/**
* @file rank.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RANK2		("../data/TEXTURE/2i.png")							//!< 読み込むテクスチャファイル名
#define	TEXTURE_RANK3		("../data/TEXTURE/3i.png")							//!< 読み込むテクスチャファイル名
#define	TEXTURE_RANK4		("../data/TEXTURE/4i.png")							//!< 読み込むテクスチャファイル名
#define	RANK_SIZE_X			(SCREEN_W/4)									//!< やられた順位テクスチャの幅
#define	RANK_SIZE_Y			(SCREEN_H/4)									//!< やられた順位テクスチャの高さ
#define	RANK_POS_X			(SCREEN_W)										//!< やられた順位テクスチャの表示位置
#define	RANK_POS_Y			(SCREEN_H)										//!< やられた順位テクスチャの表示位置

#define RANK_GOUKEI			(3)

typedef struct
{
	LPDIRECT3DTEXTURE9		pD3DTexture = NULL;								//!< テクスチャへのポインタ 
	VERTEX_2D				vertexWk[POLYGON_2D_VERTEX];					//!< 頂点情報格納ワーク
	D3DXVECTOR3				pos;											//!< ポリゴンの座標
	bool					use;											//!< 使用しているかどうか判定
}RANK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
/**
* @brief Rank初期化関数 InitRank
* @param[in] type 初期化タイプ　InitRank利用。0:初回　0以外: 2回目以降
* @return HRESULT
*/
HRESULT InitRank(int type);

/**
* @brief Rank開放関数 UninitRank
*/
void UninitRank(void);

/**
* @brief Rank更新関数 UpdateRank
*/
void UpdateRank(void);

/**
* @brief Rank描画関数 DrawRank
*/
void DrawRank(void);

/**
* @brief Rank取得関数 GetRank
* @return RANK
*/
RANK *GetRank(void);

/**
* @brief 負けた人からランクをセットする関数 SetRank
* @param[in] PlayerNum プレイヤーナンバー PLAYER_TYPE参照
*/
void SetRank(int PlayerNum);

