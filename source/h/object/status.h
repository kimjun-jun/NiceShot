/**
* @file status.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
#include "../../h/object/objectclass.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
/**
*　@class STATUS
*　@brief GAMEOBJECT派生クラス　
*/
class STATUS : public GAME_OBJECT
{
public:
	STATUS();		//!< データ読み込み　初期化
	~STATUS();		//!< 削除

private:
	void		Init(void);						//!< 初期化
	void		Update(PLAYER_HONTAI *p);		//!< 更新
	void		Draw(bool Netflag, int NetMyNumber, int CntPlayer);	//!< 描画

	void		ReinitNet(int MyNumber);		//!< 再初期化ネット対戦前	

	TEXTURE		tex[OBJECT_STATUS_MAX];			//!< テクスチャ情報　複数使用するならここを配列化　
	VTXBuffer	vtx[OBJECT_PLAYER_MAX];			//!< 頂点情報　配列構造は[P1tex1,P1tex2,P1tex3,P1tex4,P2tex1,P2tex2,,,]構造
	TransForm	Transform[OBJECT_PLAYER_MAX];	//!< トランスフォーム情報　各プレイヤー用のスクリーンに描画
	float		PtternV[OBJECT_PLAYER_MAX][OBJECT_STATUS_MAX];	//!< 取得直後は全部描画するが、時間が経過するにつれてテクスチャが徐々に上から消えていく　表示非表示もここで設定できる
	eSTATUS_TYPE;
	const char *c_FileNameTex[OBJECT_STATUS_MAX] =
	{
		"../data/TEXTURE/UI/UI_Skill_Power.png"		// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Skill_Speed.png"		// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Skill_Flash.png"		// 読み込むテクスチャファイル名
		"../data/TEXTURE/UI/UI_Skill_Blind.png"		// 読み込むテクスチャファイル名
	};

};
