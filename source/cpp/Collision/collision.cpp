/**
* @file collision.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../h/main.h"
#include "../../h/Collision/collision.h"


//=============================================================================
// 当たり判定BB2D　xz軸
//=============================================================================
bool CollisionBB2D(D3DXVECTOR3 pos1, float h1, float w1, D3DXVECTOR3 pos2, float h2, float w2)
{
	if (pos1.x + w1 >= pos2.x - w2
		&& pos1.x - w1 <= pos2.x + w2
		&& pos1.z + h1 >= pos2.z - h2
		&& pos1.z - h1 <= pos2.z + h2)
	{
		//当たっている
		return true;
	}
	//当たっていない
	return false;
}

//=============================================================================
// 当たり判定BB3D
//=============================================================================
bool CollisionBB3D(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{
	if (pos1.x + size1.x >= pos2.x - size2.x && pos1.x - size1.x <= pos2.x + size2.x
		&& pos1.y + size1.y >= pos2.y - size2.y  && pos1.y - size1.y <= pos2.y + size2.y
		&& pos1.z + size1.z >= pos2.z - size2.z  && pos1.z - size1.z <= pos2.y + size2.z)
	{
		//当たっている
		return true;
	}
	//当たっていない
	return false;
}

//=============================================================================
// 当たり判定BC
//=============================================================================
bool CollisionBC(D3DXVECTOR3 pos1, float r1, D3DXVECTOR3 pos2, float r2)
{
	//式1 プレイヤーの中心からエネミーの中心までの距離をplayer_ballettに代入
	D3DXVECTOR3 distance_pos = pos1 - pos2;

	//式2 プレイヤーサークル(ボディ)の半径
	//playerのpos(D3DXVECTOR3なので3次元)のx座標から当たり判定用テクスチャサイズ(プレイヤー半径)を差し引いた値をplayer_r(プレイヤー半径)に代入
	float wr1 = r1;
	//式2 エネミーサークル(ボディ)の半径
	float wr2 = r2;

	//D3DXVec2LengthSq()関数でdistance_pos(3次元)を2乗している
	float dista = D3DXVec3LengthSq(&distance_pos);

	float wr3 = (wr1 + wr2)*(wr1 + wr2);

	//当たり判定
	if (dista <= wr3)
	{
		//当たっている
		return true;
	}
	return false;
}



