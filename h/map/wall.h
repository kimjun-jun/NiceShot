/**
* @file wall.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

struct WALL
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff;			// インデックスバッファインターフェースへのポインタ

	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 pos;							// ポリゴン表示位置の中心座標
	D3DXVECTOR3 rot;							// ポリゴンの回転角
	int nNumBlockX, nNumBlockY;					// ブロック数
	int nNumVertex;								// 総頂点数	
	int nNumVertexIndex;						// 総インデックス数
	int nNumPolygon;							// 総ポリゴン数
	float fBlockSizeX, fBlockSizeY;				// ブロックサイズ
} ;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
WALL *GetWall(void);
int GetWallNum(void);

