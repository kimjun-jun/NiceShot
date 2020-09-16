/**
* @file field.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

/**
 * @enum RAND
 * ランダムの種類
 */
enum FIELD_TYPE
{
	FIELD_TYPE_BOKOBOKO,
	FIELD_TYPE_NADARAKA,
	FIELD_TYPE_PLAYERADVANTAGE,
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
void SetFieldInterPolationFieldType(int type, int CntPlayer);
float GetFieldBlockXSize(void);
float GetFieldBlockZSize(void);

void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);

