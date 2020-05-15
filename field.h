//=============================================================================
//
// 地面の処理 [field.h]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#pragma once

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct FIELD_COLLISION
{
	FIELD_COLLISION				*Parent;				// 4分割の4分割の4分割の合計3種類(親NULL、親PARENT、親CHILD)　＝4*4*4=64タイプ
	int							Num;					// 番号
	bool						Cheak;					// 正常に当たり判定しているかどうかの判定
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
LPDIRECT3DVERTEXBUFFER9 *GetFieldBuff(void);
int GetFieldBlockXNum(void);
int GetFieldBlockZNum(void);
void SetFieldInterPolationFieldType(int type);
float GetFieldBlockXSize(void);
float GetFieldBlockZSize(void);
void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);

