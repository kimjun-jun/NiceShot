/**
* @file bulletprediction.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../../h/object/objectclass.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class BULLETPREDICTION
*　@brief GAMEOBJECT派生クラス
*/
class BULLETPREDICTION : public GAME_OBJECT
{
public:
	BULLETPREDICTION();		//!< データ読み込み　初期化
	~BULLETPREDICTION();	//!< 削除

private:
	void		Init(void);						//!< 初期化
	void		Update(PLAYER_HONTAI *player);	//!< 更新
	void		Draw(PLAYER_HONTAI *player, int CntPlayer);		//!< 描画

	void		UpdateInstance(D3DXVECTOR3 pos, int PlayerType, D3DXCOLOR col, float fSizeX, float fSizeY, int CntBulletPrediction);

	TEXTURE		tex;				//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx[OBJECT_PLAYER_MAX];				//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_PLAYER_MAX][OBJECT_BULLETPREDICTION_MAX];		//!< トランスフォーム情報

};


