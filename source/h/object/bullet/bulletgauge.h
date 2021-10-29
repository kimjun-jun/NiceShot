/**
* @file bulletgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once
#include "../../../h/object/objectclass.h"

enum eBULLETGAUGE_TEX_TYPE
{
	BULLETGAUGE_TEX_BASE,
	BULLETGAUGE_TEX_SHELL,
	BULLETGAUGE_TEX_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class BULLETGAUGE_PARAMETER
*　@brief 
*/
class BULLETGAUGE_PARAMETER
{
public:
	BULLETGAUGE_PARAMETER() { AmmoStock = 0; }
	~BULLETGAUGE_PARAMETER() { }
	int	AmmoStock;
};

/**
*　@class BULLETGAUGE
*　@brief GAMEOBJECT派生クラス
*/
class BULLETGAUGE : public GAME_OBJECT
{
public:
	BULLETGAUGE();	//!< データ読み込み　初期化
	~BULLETGAUGE();	//!< 削除

	void		Init(void);						//!< 初期化
	void		InitNet(int MyNumber);	//!< 初期化
	void		Update(PLAYER *player);	//!< 更新
	void		Draw(bool Netflag, int NetMyNumber, int CntPlayer);		//!< 描画

private:

	TEXTURE			tex[BULLETGAUGE_TEX_MAX];					//!< テクスチャ情報　複数使用するならここを配列化　0:枠　1:中身
	VTXBuffer		vtx[OBJECT_BULLETGAUGE_MAX];				//!< 頂点情報　複数使用するならここを配列化
	TransForm		Transform[OBJECT_BULLETGAUGE_MAX];			//!< トランスフォーム情報
	BULLETGAUGE_PARAMETER	BulletGaugePara[OBJECT_BULLETGAUGE_MAX];//!< インスタンスに必要なデータ群

	const char *c_aFileNameTex[BULLETGAUGE_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_Shell.png" ,
		"../data/TEXTURE/UI/UI_Shell_Empty.png",
	};
};
