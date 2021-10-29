/**
* @file shadow.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
/**
*　@class SHADOW
*　@brief GAMEOBJECT派生クラス
*/
class SHADOW : public GAME_OBJECT
{
public:
	SHADOW();	//!< データ読み込み　初期化
	~SHADOW();	//!< 削除

	void		Init(void);		//!< 初期化
	void		Update(void);	//!< 更新
	void		Draw(void);		//!< 描画

	int			SetInstance(D3DXVECTOR3 pos, D3DXVECTOR3 scl);	//!< インスタンスセット
	void		ReleaseInstance(int Idx);				//!< インスタンス解放
	void		UpdateInstance(int Idx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl);	//インスタンスアップデート　各インスタンス毎に任意呼び出し

private:

	TEXTURE		tex;			//!< テクスチャ情報　複数使用するならここを配列化
	VTXBuffer	vtx;			//!< 頂点情報　複数使用するならここを配列化
	TransForm	Transform[OBJECT_SHADOW_MAX];		//!< トランスフォーム情報
	iUseCheak	iUseType[OBJECT_SHADOW_MAX];		//!< 使用情報

} ;

