/**
* @file field.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct FIELD_COLLISION
{
	FIELD_COLLISION				*Parent;				// 4������4������4�����̍��v3���(�eNULL�A�ePARENT�A�eCHILD)�@��4*4*4=64�^�C�v
	int							Num;					// �ԍ�
	bool						Cheak;					// ����ɓ����蔻�肵�Ă��邩�ǂ����̔���
};



//*****************************************************************************
// �v���g�^�C�v�錾
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
