/**
* @file collision.h
* @brief NiceShot(3D)êÌé‘ÉQÅ[ÉÄ
* @author ÉLÉÄÉâÉWÉÖÉì
* @date 2020/01/15
*/
#pragma once
bool CollisionBB2D(D3DXVECTOR3 pos1, float h1, float w1, D3DXVECTOR3 pos2, float h2, float w2);
bool CollisionBB3D(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);
bool CollisionBC(D3DXVECTOR3 pos1, float r1, D3DXVECTOR3 pos2, float r2);
void CheakHit(int scene, GAME_OBJECT* GameObj);
