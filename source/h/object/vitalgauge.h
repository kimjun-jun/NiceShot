/**
* @file vitalgauge.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

enum VITAL_TEX_TYPE
{
	VITAL_TEX_BASE,
	VITAL_TEX_GREEN,
	VITAL_TEX_ORANGE,
	VITAL_TEX_RED,
	VITAL_TEX_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class VITALGAUGE
*　@brief GAMEOBJECT派生クラス
*/
class VITALGAUGE_PARAMETER
{
public:
	VITALGAUGE_PARAMETER() { VitalPower = PLAYER_VITAL_MAX; }
	~VITALGAUGE_PARAMETER() {}

	int			VitalPower;
};

/**
*　@class VITALGAUGE
*　@brief GAMEOBJECT派生クラス
*/
class VITALGAUGE : public GAME_OBJECT
{
public:
	VITALGAUGE();	//!< データ読み込み　初期化
	~VITALGAUGE();	//!< 削除

private:
	void		Init(void);								//!< 初期化
	void		Update(PLAYER_HONTAI *p, RANK *rank, bool Netflag, int NetMyNumber);	//!< 更新
	void		Draw(bool Netflag, int NetMyNumber, int CntPlayer);						//!< 描画

	void		ReinitNet(int MyNumber);		//!< 再初期化ネット対戦前	

	TEXTURE			tex[VITAL_TEX_MAX];				//!< テクスチャ情報　複数使用するならここを配列化　0:枠　1,2,3:中身(カラーバリエーション)
	VTXBuffer		vtx[OBJECT_VITAL_MAX];			//!< 頂点情報　複数使用するならここを配列化 
	TransForm		Transform[OBJECT_VITAL_MAX];	//!< トランスフォーム情報
	VITALGAUGE_PARAMETER	VitalGaugePara[OBJECT_VITAL_MAX];//!< インスタンスに必要なデータ群

	const char *c_aFileNameTex[VITAL_TEX_MAX] =
	{
		"../data/TEXTURE/UI/UI_HP_Base.png",	
		"../data/TEXTURE/UI/UI_HP_Green.png",	
		"../data/TEXTURE/UI/UI_HP_Orange.png",	
		"../data/TEXTURE/UI/UI_HP_Red.png",		
	};

};

