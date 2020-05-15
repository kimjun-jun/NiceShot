//=============================================================================
//
// �n�ʂ̏��� [field.h]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
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

