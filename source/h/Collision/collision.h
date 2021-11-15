/**
* @file collision.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
* @brief BB2D関数	CollisionBB2D
* @param[in] pos1			対象座標 1
* @param[in] h1				対象高さ 1
* @param[in] w1				対象横幅 1
* @param[in] pos2			対象座標 2
* @param[in] h2				対象高さ 2
* @param[in] w2				対象横幅 2
* @return bool true;当たっている false:当たっていない
* @details 当たり判定用
* @author : 木村純(キムラジュン)
*/
bool CollisionBB2D(D3DXVECTOR3 pos1, float h1, float w1, D3DXVECTOR3 pos2, float h2, float w2);

/**
* @brief BB3D関数	CollisionBB3D
* @param[in] pos1			対象座標 1
* @param[in] size1			対象サイズ 1
* @param[in] pos2			対象座標 2
* @param[in] size2			対象サイズ 2
* @return bool true;当たっている false:当たっていない
* @details 当たり判定用
* @author : 木村純(キムラジュン)
*/
bool CollisionBB3D(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);

/**
* @brief BC関数	CollisionBC
* @param[in] pos1			対象座標 1
* @param[in] r1				対象半径 1
* @param[in] pos2			対象座標 2
* @param[in] r2				対象半径 2
* @return bool true;当たっている false:当たっていない
* @details 当たり判定用
* @author : 木村純(キムラジュン)
*/
bool CollisionBC(D3DXVECTOR3 pos1, float r1, D3DXVECTOR3 pos2, float r2);
