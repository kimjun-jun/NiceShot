/**
* @file Move.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
 * @class MOVE
 * @brief 移動クラス
 */
class MOVE
{
public:
	MOVE() { move = VEC3_ALL0; }
	~MOVE() {}

	//------------------------get関数
	inline D3DXVECTOR3 Move(void) { return move; };
	//------------------------set関数
	inline void Move(const D3DXVECTOR3 InMove) { move = InMove; };

private:
	D3DXVECTOR3 move;
};

/**
 * @class PLAYER_PARAMETER_MOVE
 * @brief 移動クラス
 */
class PLAYER_PARAMETER_MOVE
{
public:
	PLAYER_PARAMETER_MOVE() { move = VEC3_ALL0; }
	~PLAYER_PARAMETER_MOVE() {}

	//------------------------get関数
	inline D3DXVECTOR3 Move(void) { return move; };
	//------------------------set関数
	inline void Move(const D3DXVECTOR3 InMove) { move = InMove; };

	//------移動制御
	//使用
	void MoveKeybord(PLAYER *Player, int CntPlayer);				//!< 移動制御(ki-bo-doで移動制御)
	void MoveL(PLAYER *Player, int CntPlayer, bool Netflag);		//!< 移動制御(LRスティックで移動制御)
	//未使用
	//void MoveLtype0(int CntPlayer, EFFECT *effect, bool Netflag);	//!< 移動制御(LRスティックで移動制御)
	//void MoveABL(int CntPlayer, EFFECT *effect, bool Netflag);		//!< 移動制御(ABボタンLスティックで移動制御)
	//void MoveL2R2(int CntPlayer, EFFECT *effect, bool Netflag);		//!< 移動制御(L2R2で移動制御)

private:
	D3DXVECTOR3 move;
};
