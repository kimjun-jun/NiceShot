/**
* @file explosion.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/objectclass.h"

//爆発種類
enum eEXPLOSION_TYPE
{
	EXPLOSIONTYPE_BULLET_NONE = -1,		//!< 未割当
	EXPLOSIONTYPE_BULLET_PLAYER1 = 0,	//!< プレイヤー1
	EXPLOSIONTYPE_BULLET_PLAYER2,		//!< プレイヤー2
	EXPLOSIONTYPE_BULLET_PLAYER3,		//!< プレイヤー3
	EXPLOSIONTYPE_BULLET_PLAYER4,		//!< プレイヤー4
	EXPLOSIONTYPE_BULLET_ENEMY1,		//!< エネミー1
	EXPLOSIONTYPE_BULLET_ENEMY2,		//!< エネミー2
	EXPLOSIONTYPE_BULLET_ENEMY3,		//!< エネミー3
	EXPLOSIONTYPE_BULLET_ENEMY4,		//!< エネミー4
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class EXPLOSION_PARAMETER
*　@brief 
*/
class EXPLOSION_PARAMETER
{
public:
	EXPLOSION_PARAMETER() { nCounter = nPatternX = nPatternY = 0; fSizeX = fSizeY = 0.0f; 
	eType = EXPLOSIONTYPE_BULLET_NONE;}
	~EXPLOSION_PARAMETER() {}

	int			nCounter;			//!< カウンター
	int			nPatternX;			//!< パターンNo.X
	int			nPatternY;			//!< パターンNo.Y
	float		fSizeX;				//!< 頂点サイズX
	float		fSizeY;				//!< 頂点サイズY
	eEXPLOSION_TYPE	eType;			//!< 種類
};

/**
*　@class EXPLOSION
*　@brief GAMEOBJECT派生クラス
*/
class EXPLOSION : public GAME_OBJECT
{
public:
	EXPLOSION();	//!< データ読み込み　初期化
	~EXPLOSION();	//!< 削除

	int	SetInstance(D3DXVECTOR3 pos, float fSizeX, float fSizeY, eEXPLOSION_TYPE eType, D3DXCOLOR col);	//!< インスタンスセット

	void		Init(void);					//!< 初期化
	void		Update(void);				//!< 更新
	void		Draw(int CntPlayer);		//!< 描画

private:


	TEXTURE		tex;	//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;	//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_EXPLOSION_MAX];			//!< トランスフォーム情報
	iUseCheak	iUseType[OBJECT_EXPLOSION_MAX];			//!< 使用情報

	EXPLOSION_PARAMETER	ExploPara[OBJECT_EXPLOSION_MAX];

} ;



