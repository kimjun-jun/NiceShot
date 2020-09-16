/**
* @file sky.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshSky(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockZ,
						float fRadius, float fRotY, int nTexType = -1, bool bReverse = false);
void UninitMeshSky(void);
void UpdateMeshSky(void);
void DrawMeshSky(void);

