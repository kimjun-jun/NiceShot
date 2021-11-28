/**
* @file library.h
* @brief ���C�u�����֐��Q
* @author �L�����W����
*/
#pragma once

#include "../../h/Object/ObjectClass/StandardComponent/Model/Model.h"

/**
* ���d�v��{�ǂݍ���X�t�@�C���͕K���O�p�ʉ����Ă�������]
* @brief X�t�@�C���ǂݍ��݊֐�	LoadMesh
* @param[in] FileName			�t�@�C����(�p�X�܂ŕK���w�肷�邱�ƁA�܂���define���Ŏw�肷�邱��)
* @param[in] pD3DXBuffMat		�o�b�t�@�}�e���A���̃|�C���^
* @param[in] nNumMat			���f���̃}�e���A����
* @param[in] pD3DXMesh			���b�V���|�C���^
* @param[in] pD3DVtxBuff		���_�o�b�t�@�|�C���^
* @param[in] pD3DIdxBuff		�C���f�b�N�X�o�b�t�@�|�C���^
* @param[in] nNumVertex			���_����
* @param[in] nNumPolygon		�|���S������
* @param[in] nNumVertexIndex	���_�C���f�b�N�X����
* @param[in] pD3DTexture		�e�N�X�`���|�C���^(���ݖ��g�p,���f���Ƀe�N�X�`����񂪂���Ƃ��Ɏg�p����\��)
* @details X�t�@�C���ǂݍ��ݗp
* @author : �ؑ���(�L�����W����)
*/
HRESULT LoadMesh(const char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff,
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture);

HRESULT LoadMesh(const char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture);

/**
* @brief ���[�t�B���O���s�֐� DoMorphing
* @param[in] FromModel �Q�Ƃ���錳���f���f�[�^
* @param[in] ToModel �ύX�������`�惂�f���f�[�^
* @details ���[�t�B���O�p ���݂͒��_������`��Ԃŕω������Ă���
* @author : �ؑ���(�L�����W����)
*/
void DoMorphing(LPDIRECT3DVERTEXBUFFER9 FromModelVB, LPDIRECT3DVERTEXBUFFER9 ToModelVB, LPDIRECT3DINDEXBUFFER9 ModelIdx, MODELATTRIBUTE *Attribute,float dt, float *time, eMORPHING_TYPE *SignalType);

/**
* @brief ���f�����Z�b�g�֐� ResetModel
* @param[in] FromModel �Q�Ƃ���錳(�I���W�i��)���f���f�[�^
* @param[in] ToModel �ύX�������`�惂�f���f�[�^
* @details ���f�����ω�������̃��Z�b�g�p
* @author : �ؑ���(�L�����W����)
*/
void ResetModel(LPDIRECT3DVERTEXBUFFER9 FromModelVB, LPDIRECT3DVERTEXBUFFER9 ToModelVB, LPDIRECT3DINDEXBUFFER9 ModelIdx, MODELATTRIBUTE *Attribute);

/**
* @brief ���C�L���X�g�֐� RayCast
* @param[in] D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2
* @param[out] D3DXVECTOR3 *ReturnVtx
* @return bool �������Ă��邩�̐M��
* @details �C�ӂ̃��C�n�I�_�����ߓ����蔻��������C�ӂ�3���_��p���Čv�Z����B���^�[����false�Ȃ�ReturnVtx�͍X�V����Ȃ��Bture�Ȃ璸�_�ƃ��C��������Ă����_��ReturnVtx�ɓ����
* @author : �ؑ���(�L�����W����)
*/
bool RayCast(D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 *ReturnVtx);

/**
* @brief D3DXVECTOR3����float�ɕϊ����Ĉړ������̒l���v�Z����֐� SpdCal
* @param[in] D3DXVECTOR3 move �O�����ړ��l
* @return float �ꎟ���ňړ������̒l��Ԃ�
* @details D3DXVECTOR3�^�Ō��߂��ړ��l����float�ړ����������߂�
* @author : �ؑ���(�L�����W����)
*/
float SpdCal(D3DXVECTOR3 move);

/**
* @brief ��`�����蔻����v�Z����֐� IsCollisionFanAndPoint
* @param[in] D3DXVECTOR3	CenterPos	��̒��S�ƂȂ���W			pos
* @param[in] D3DXVECTOR3	TargetPos	����������������Ώۍ��W	pos
* @param[in] float			StartRot	��̍����̊p�x				rot
* @param[in] float			EndRot		��̉E���̊p�x				rot
* @param[in] float			RangeLen	����牄�т�˒�����		len
* @return bool true:�͈͓��@false:�͈͊O
* @details �_�Ɛ�̓����蔻������Ă���B2D�O�ς����Ă���̂œ�̒������׍��E�ǂ��炩�ɂ��邩�Ŕ��肵�Ă���
* @author : �ؑ���(�L�����W����)
*/
bool IsCollisionFanAndPoint(D3DXVECTOR3 CenterPos, D3DXVECTOR3 TargetPos, float StartRot, float EndRot, float RangeLen);

//��`�����蔻����v�Z����֐��@�g�p��
/*
	bool collisionFan = IsCollisionFanAndPoint
	(�v���C���[pos, �G�l�~�[pos, �v���C���[rot.y, �v���C���[rot.y-1.57f, �v���C���[rot.y+1.57f, 30.0f);
	 if(collisionFan == true)
	 {
		//�͈͓�
	 }
	 else
	 {
		//�͈͊O
	 }
*/



/**
* @brief �z�[�~���O����֐�(Y���W�͌v�Z���Ȃ�2D��) HormingType01
* @param[in] D3DXVECTOR3	*StartPos	�X�^�[�g���W	pos
* @param[in] D3DXVECTOR3	EndPos		�G���h���W		pos
* @param[in] float			MoveValue	�ړ���			Value
* @details �X�^�[�g���W����G���h���W�֌�����MoveValue�������z�[�~���O����B���֏������D3DXVECTOR3�ɂ��Ă���B
* @details *StartPos�̂݃|�C���^�[�Ƃ����ڍ��W��ω������Ă���B
* @author : �ؑ���(�L�����W����)
*/
void HormingType01(D3DXVECTOR3 *StartPos, D3DXVECTOR3 EndPos, float MoveValue);


/**
* @brief ��Ԋ֐� MyInterpolation
* @param[in] D3DXVECTOR3	StartPos			�X�^�[�g���W	pos
* @param[in] D3DXVECTOR3	EndPos				�G���h���W		pos
* @param[in] float			min					�ŏ���Ԓl		num
* @param[in] float			max					�ő��Ԓl		num
* @param[in] float			dt					�΍�����		Value
* @param[in] float			ElapsedTime			�o�ߎ���		num
* @param[in] float			float(*func)(float)	��Ԓl�v�Z�֐�	�C�ӂ̕�Ԓl�v�Z�֐����w��
* @return D3DXVECTOR3 �v�Z���ʍ��W��Ԃ��B
* @details �X�^�[�g���W����G���h���W�֌����ĕ�ԏ�������B�����ɔC�ӂ̕�ԏ�����I�����邱�ƂōD���Ȏ�ނ̕�ԂŌv�Z���\�B
* @author : �ؑ���(�L�����W����)
*/
D3DXVECTOR3 MyInterpolation(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, float min, float max, float dt, float ElapsedTime, float(*func)(float));

//��Ԋ֐� MyInterpolation�@�g�p��
/*
	enemy.pos = MyInterpolation(enemy.InterpolationStartPos, enemy.InterpolationEndPos, 0.0f, 1.0f, 1.0f, 120.0f, LerpEaseIn);
*/

/**
* @brief ��Ԓl�v�Z�֐� Lerp ���`���
* @param[in] float	t	����
* @return float �v�Z���ʕ�Ԓl��Ԃ�
* @details MyInterpolation�g�p���ɗ��p�B�����ɍD���ȕ�Ԓl�v�Z�֐�(�����̊֐��Q)���w�肵�Ďg���B
* @author : �ؑ���(�L�����W����)
*/
float Lerp(float t);

/**
* @brief ��Ԓl�v�Z�֐� LerpEaseIn �񎟊֐����������
* @param[in] float	t	����
* @return float �v�Z���ʕ�Ԓl��Ԃ�
* @details MyInterpolation�g�p���ɗ��p�B�����ɍD���ȕ�Ԓl�v�Z�֐�(�����̊֐��Q)���w�肵�Ďg���B
* @author : �ؑ���(�L�����W����)
*/
float LerpEaseIn(float t);

/**
* @brief ��Ԓl�v�Z�֐� LerpEaseOut �񎟊֐��㊪�����
* @param[in] float	t	����
* @return float �v�Z���ʕ�Ԓl��Ԃ�
* @details MyInterpolation�g�p���ɗ��p�B�����ɍD���ȕ�Ԓl�v�Z�֐�(�����̊֐��Q)���w�肵�Ďg���B
* @author : �ؑ���(�L�����W����)
*/
float LerpEaseOut(float t);

/**
* @brief ��Ԓl�v�Z�֐� LerpEaseInEaseOut �O���֐����
* @param[in] float	t	����
* @return float �v�Z���ʕ�Ԓl��Ԃ�
* @details MyInterpolation�g�p���ɗ��p�B�����ɍD���ȕ�Ԓl�v�Z�֐�(�����̊֐��Q)���w�肵�Ďg���B
* @author : �ؑ���(�L�����W����)
*/
float LerpEaseInEaseOut(float t);

/**
* @brief [2D�p]�_�ƒ���(2�_���狁�߂�)�̍ŒZ�����v�Z�֐�	 PointAndLineMinDistance
* @param[in] D3DXVECTOR3	Point		���W
* @param[in] D3DXVECTOR3	LinePoint1	���W
* @param[in] D3DXVECTOR3	LinePoint2	���W
* @return float�@�v�Z����(�ŒZ����)
* @details [2D�p]�_���璼���܂ł̍ŒZ���������߂�Ƃ��Ɏg�p
* @author : �ؑ���(�L�����W����)
*/
float PointAndLineMinDistance(D3DXVECTOR3 Point, D3DXVECTOR3 LinePoint1, D3DXVECTOR3 LinePoint2);

/**
* @brief [3D�p]�~���ƒ��܂̓����蔻��֐�	CheckHitColumnLB
* @param[in] D3DXVECTOR3	yarnvec				�����x�N�g��
* @param[in] D3DXVECTOR3	cylindervec			�����x�N�g��
* @param[in] D3DXVECTOR3	yarnpos				���W
* @param[in] D3DXVECTOR3	bottompos			���W
* @param[in] float			yarnradius			���a
* @param[in] float			cylinderradius		���a
* @return bool�@true:�������Ă���@false:�������Ă��Ȃ�
* @details �~���ƒ������������Ă��邩����Ŏg�p
*/
bool CheckHitColumnLB(D3DXVECTOR3 yarnvec, D3DXVECTOR3 cylindervec, D3DXVECTOR3 yarnpos, D3DXVECTOR3 bottompos, float yarnradius, float cylinderradius);

/**
* @brief �����蔻�荂�����̃t���O����������	InitCntPartition
* @details �����蔻�荂�����łǂ̕��ʃG���A�ɑ����Ă��邩�̔���t���O������������
* @author : �ؑ���(�L�����W����)
*/
void InitCntPartition(void);

/**
* @brief �����蔻�荂��������֐�	SpeedUpFieldHitPoly
* @param[in] D3DXVECTOR3	InPos					���肷����͍��W
* @param[in] float			*HitPosUp				�����Ă���G���A�̍ő�Z���W
* @param[in] float			*HitPosDown				�����Ă���G���A�̍ŏ�Z���W
* @param[in] float			*HitPosLeft				�����Ă���G���A�̍ő�X���W
* @param[in] float			*HitPosRight			�����Ă���G���A�̍ŏ�X���W
* @param[in] float			fSideSizeXQuarter		�t�B�[���h��X1/4�T�C�Y
* @param[in] float			fSideSizeZQuarter		�t�B�[���h��Z1/4�T�C�Y
* @param[in] float			fSideSizeXEighth		�t�B�[���h��X1/8�T�C�Y
* @param[in] float			fSideSizeZEighth		�t�B�[���h��Z1/8�T�C�Y
* @details �����Ă���HitPos��XZ�ő�ŏ����|�C���^�[�Ƃ��ĕԂ��B���̃G���A����InPos������Ƃ���
* @author : �ؑ���(�L�����W����)
*/
void SpeedUpFieldHitPoly(D3DXVECTOR3 InPos, float *HitPosUp, float *HitPosDown, float *HitPosLeft, float *HitPosRight
	,float fSideSizeXQuarter, float fSideSizeZQuarter, float fSideSizeXEighth, float fSideSizeZEighth);

/**
* @brief ���ʐ��`��ԎZ�o�֐�	SphereLinear
* @param[in] D3DXVECTOR3	*out		��ԃx�N�g���i�o�́j	��Ԃ��ꂽ�v���C���[�̎p���x�N�g��
* @param[in] D3DXVECTOR3	*start		�J�n�x�N�g��			�v���C���[�̎p���x�N�g��
* @param[in] D3DXVECTOR3	*end		�I���x�N�g��			�ړI�n�`�̖@���x�N�g��
* @param[in] float			t			�X�^�[�g����G���h�܂ł̕�Ԓl�̊����@
* @details �x�N�g���̉�]�����ʂŏ�������B�p������ȂǂŎg�p
* @author : �ؑ���(�L�����W����)
*/
void SphereLinear(D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t);

/**
* @brief ���ʐ��`��Ԃɂ��p����Ԋ֐�	CalcInterPause
* @param[in] D3DXVECTOR3	*out		��ԃx�N�g���i�o�́j	��Ԃ��ꂽ�v���C���[�̎p���x�N�g��
* @param[in] D3DXVECTOR3	*start		�J�n�x�N�g��			�v���C���[�̎p���x�N�g��
* @param[in] D3DXVECTOR3	*end		�I���x�N�g��			�ړI�n�`�̖@���x�N�g��
* @param[in] float			t			�X�^�[�g����G���h�܂ł̕�Ԓl�̊����@
* @details �x�N�g���̉�]�����ʂŏ�������B�p������ȂǂŎg�p�B�}�g���N�X���璼�ڐ��䂷�邽�߂Ɏg�p
* @author : �ؑ���(�L�����W����)
*/
//D3DXMATRIX* CalcInterPause(D3DXMATRIX* out, D3DXMATRIX* start, D3DXMATRIX* end, float t);


/**
* @brief /�I�u�W�F�N�g�̔j��(Release)�������ȗ��� safe_release
*/
template<class T>
inline void safe_release(T& p)
{
	if (p)
	{
		p->Release();
		p = NULL;
	}
}


// enum for various skinning modes possible
enum METHOD
{
	D3DNONINDEXED,
	D3DINDEXED,
	SOFTWARE,
	D3DINDEXEDVS,
	D3DINDEXEDHLSLVS,
	NONE
};

struct FIELD_COLLISION
{
	FIELD_COLLISION				*Parent;				// 4������4������4�����̍��v3���(�eNULL�A�ePARENT�A�eCHILD)�@��4*4*4=64�^�C�v
	bool						Cheak;					// ����ɓ����蔻�肵�Ă��邩�ǂ����̔���
};




//--------------------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can add some app-specific
//       info that will be stored with each frame
//--------------------------------------------------------------------------------------
struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};


//--------------------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//--------------------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures;       // array of textures, entries are NULL if no texture specified    

	// SkinMesh info             
	LPD3DXMESH pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD NumAttributeGroups;
	DWORD NumInfl;
	LPD3DXBUFFER pBoneCombinationBuf;
	D3DXMATRIX** ppBoneMatrixPtrs;
	D3DXMATRIX* pBoneOffsetMatrices;
	DWORD NumPaletteEntries;
	bool UseSoftwareVP;
	DWORD iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
};

//--------------------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//--------------------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	CAllocateHierarchy()
	{
	}

	METHOD                      SkinningMethod = D3DNONINDEXED; // Current skinning method
	D3DXMATRIXA16*              pBoneMatrices = NULL;
	UINT                        NumBoneMatricesMax = 0;
	bool                        bUseSoftwareVP;       // Flag to indicate whether software vp is
};

class ORIANIMATION
{
public:
	CAllocateHierarchy			AH;
	LPD3DXFRAME                 pFrameRoot = NULL;
	ID3DXAnimationController*   pAnimController = NULL;
	LPD3DXANIMATIONSET			pAnimSetKeep[10] = { NULL };
	float						AnimTime;
	UINT						m_PreAnimID = 0;					//�O�̃A�j���Z�b�g
	UINT						m_CurAnimID = 0;					//���̃A�j���Z�b�g
	float						ShiftTime[10] = { 0.0f };			//�A�j�����V�t�g���鎞��
	float						CurWeight[10] = { 0.0f };			//���݂̎p���W��



	// ���[�v���Ԃ�ݒ�
	bool SetLoopTime(ORIANIMATION *pAnim, UINT animID, FLOAT time);
	// ����J�n�ɂ����鎞�Ԃ�ݒ�
	bool SetShiftTime(ORIANIMATION *pAnim, UINT animID, FLOAT interval);
	// �A�j���[�V������؂�ւ�
	bool ChangeAnimation(ORIANIMATION *pAnim, UINT NextAnimID);
	// �A�j���[�V�������X�V
	bool AdvanceTime(ORIANIMATION *pAnim, FLOAT time);
};
bool SetLoopTime(ORIANIMATION *pAnim, UINT animID, FLOAT time);
bool SetShiftTime(ORIANIMATION *pAnim, UINT animID, FLOAT interval);
bool ChangeAnimation(ORIANIMATION *pAnim, UINT NextAnimID);
bool AdvanceTime(ORIANIMATION *pAnim, FLOAT time);


//�A�j���[�V������ʃ��x���\����
struct HLANIMATION_DESC
{
	UINT uiAnimID;                 // �A�j���[�V����ID
	ID3DXAnimationSet *pAnimSet;   // �A�j���[�V�����Z�b�g
	FLOAT fLoopTime;               // 1���[�v�̎���
	FLOAT fTrackSpeed;             // �g���b�N�X�s�[�h���ߒl
	FLOAT fShiftTime;              // �V�t�g����̂ɂ����鎞��
	FLOAT fCurWeightTime;          // ���݂̃E�F�C�g����
};


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
void OnFrameMoveAnime(double fTime, ID3DXAnimationController* AnimController);
void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, METHOD SkinningMethod, LPDIRECT3DTEXTURE9 pD3DTexture);
void DrawFrame(LPD3DXFRAME pFrame, METHOD SkinningMethod, LPDIRECT3DTEXTURE9 pD3DTexture);
HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameRoot);
HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pFrameRoot);
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
void UpdateSkinningMethod(LPD3DXFRAME pFrameBase, METHOD SkinningMethod, D3DXMATRIXA16* pBoneMatrices, UINT NumBoneMatricesMax, bool* bUseSoftwareVP);
HRESULT GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED* pMeshContainer, METHOD SkinningMethod, D3DXMATRIXA16* pBoneMatrices, UINT NumBoneMatricesMax, bool* bUseSoftwareVP);
void ReleaseAttributeTable(LPD3DXFRAME pFrameBase);
void LoadAnimSet(ORIANIMATION *pAnim);
