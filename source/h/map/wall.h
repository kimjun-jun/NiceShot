/**
* @file wall.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once
#include "../../../h/Object/ObjectClass/objectclass.h"


/**
*　@class WALL
*　@brief GAMEOBJECT派生クラス
*/
class WALL : public GAME_OBJECT
{
public:
	WALL();		//!< データ読み込み　初期化
	~WALL();	//!< 削除
	void		Init(void);		//!< 初期化
	void		Update(void);	//!< 更新
	void		Draw(void);		//!< 描画
	TransForm				Transform[OBJECT_WALL_MAX];			//!< トランスフォーム情報

private:
	TEXTURE					tex;								//!< テクスチャ情報　複数使用するならここを配列化　ITEMTYPE_MAX
	VTXBuffer				vtx[OBJECT_WALL_MAX];				//!< 頂点情報　複数使用するならここを配列化

	void SetUpMesh(void);			// WALLセットアップ処理

} ;
