/**
* @file field.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
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
void SpeedUpFieldHitPoly(D3DXVECTOR3 InrayS, float *HitPosUp, float *HitPosDown, float *HitPosLeft, float *HitPosRight);

void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);

void SphereLinear(D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t);
D3DXMATRIX* CalcInterPause(D3DXMATRIX* out, D3DXMATRIX* start, D3DXMATRIX* end, float t);
