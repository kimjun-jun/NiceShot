/**
* @file library.h
* @brief ライブラリ関数群
* @author キムラジュン
*/
#pragma once

#include "../../h/Object/ObjectClass/StandardComponent/Model/Model.h"

/**
* ＊重要＊{読み込むXファイルは必ず三角面化してください]
* @brief Xファイル読み込み関数	LoadMesh
* @param[in] FileName			ファイル名(パスまで必ず指定すること、またはdefine等で指定すること)
* @param[in] pD3DXBuffMat		バッファマテリアルのポインタ
* @param[in] nNumMat			モデルのマテリアル数
* @param[in] pD3DXMesh			メッシュポインタ
* @param[in] pD3DVtxBuff		頂点バッファポインタ
* @param[in] pD3DIdxBuff		インデックスバッファポインタ
* @param[in] nNumVertex			頂点総数
* @param[in] nNumPolygon		ポリゴン総数
* @param[in] nNumVertexIndex	頂点インデックス総数
* @param[in] pD3DTexture		テクスチャポインタ(現在未使用,モデルにテクスチャ情報があるときに使用する予定)
* @details Xファイル読み込み用
* @author : 木村純(キムラジュン)
*/
HRESULT LoadMesh(const char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff,
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture);

HRESULT LoadMesh(const char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture);

/**
* @brief モーフィング実行関数 DoMorphing
* @param[in] FromModel 参照される元モデルデータ
* @param[in] ToModel 変更したい描画モデルデータ
* @details モーフィング用 現在は頂点情報を線形補間で変化させている
* @author : 木村純(キムラジュン)
*/
void DoMorphing(LPDIRECT3DVERTEXBUFFER9 FromModelVB, LPDIRECT3DVERTEXBUFFER9 ToModelVB, LPDIRECT3DINDEXBUFFER9 ModelIdx, MODELATTRIBUTE *Attribute,float dt, float *time, eMORPHING_TYPE *SignalType);

/**
* @brief モデルリセット関数 ResetModel
* @param[in] FromModel 参照される元(オリジナル)モデルデータ
* @param[in] ToModel 変更したい描画モデルデータ
* @details モデルが変化した後のリセット用
* @author : 木村純(キムラジュン)
*/
void ResetModel(LPDIRECT3DVERTEXBUFFER9 FromModelVB, LPDIRECT3DVERTEXBUFFER9 ToModelVB, LPDIRECT3DINDEXBUFFER9 ModelIdx, MODELATTRIBUTE *Attribute);

/**
* @brief レイキャスト関数 RayCast
* @param[in] D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2
* @param[out] D3DXVECTOR3 *ReturnVtx
* @return bool 当たっているかの信号
* @details 任意のレイ始終点を決め当たり判定をした任意の3頂点を用いて計算する。リターンがfalseならReturnVtxは更新されない。tureなら頂点とレイが交わっている交点をReturnVtxに入れる
* @author : 木村純(キムラジュン)
*/
bool RayCast(D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 *ReturnVtx);

/**
* @brief D3DXVECTOR3からfloatに変換して移動距離の値を計算する関数 SpdCal
* @param[in] D3DXVECTOR3 move 三次元移動値
* @return float 一次元で移動距離の値を返す
* @details D3DXVECTOR3型で決めた移動値からfloat移動距離を求める
* @author : 木村純(キムラジュン)
*/
float SpdCal(D3DXVECTOR3 move);

/**
* @brief 扇形当たり判定を計算する関数 IsCollisionFanAndPoint
* @param[in] D3DXVECTOR3	CenterPos	扇の中心となる座標			pos
* @param[in] D3DXVECTOR3	TargetPos	判定をしたい検査対象座標	pos
* @param[in] float			StartRot	扇の左側の角度				rot
* @param[in] float			EndRot		扇の右側の角度				rot
* @param[in] float			RangeLen	扇中から延びる射程距離		len
* @return bool true:範囲内　false:範囲外
* @details 点と扇の当たり判定をしている。2D外積をしているので二つの直線を比べ左右どちらかにいるかで判定している
* @author : 木村純(キムラジュン)
*/
bool IsCollisionFanAndPoint(D3DXVECTOR3 CenterPos, D3DXVECTOR3 TargetPos, float StartRot, float EndRot, float RangeLen);

//扇形当たり判定を計算する関数　使用例
/*
	bool collisionFan = IsCollisionFanAndPoint
	(プレイヤーpos, エネミーpos, プレイヤーrot.y, プレイヤーrot.y-1.57f, プレイヤーrot.y+1.57f, 30.0f);
	 if(collisionFan == true)
	 {
		//範囲内
	 }
	 else
	 {
		//範囲外
	 }
*/



/**
* @brief ホーミングする関数(Y座標は計算しない2D上) HormingType01
* @param[in] D3DXVECTOR3	*StartPos	スタート座標	pos
* @param[in] D3DXVECTOR3	EndPos		エンド座標		pos
* @param[in] float			MoveValue	移動量			Value
* @details スタート座標からエンド座標へ向けてMoveValue分だけホーミングする。利便上引数をD3DXVECTOR3にしている。
* @details *StartPosのみポインターとし直接座標を変化させている。
* @author : 木村純(キムラジュン)
*/
void HormingType01(D3DXVECTOR3 *StartPos, D3DXVECTOR3 EndPos, float MoveValue);


/**
* @brief 補間関数 MyInterpolation
* @param[in] D3DXVECTOR3	StartPos			スタート座標	pos
* @param[in] D3DXVECTOR3	EndPos				エンド座標		pos
* @param[in] float			min					最小補間値		num
* @param[in] float			max					最大補間値		num
* @param[in] float			dt					偏差時間		Value
* @param[in] float			ElapsedTime			経過時間		num
* @param[in] float			float(*func)(float)	補間値計算関数	任意の補間値計算関数を指定
* @return D3DXVECTOR3 計算結果座標を返す。
* @details スタート座標からエンド座標へ向けて補間処理する。引数に任意の補間処理を選択することで好きな種類の補間で計算が可能。
* @author : 木村純(キムラジュン)
*/
D3DXVECTOR3 MyInterpolation(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, float min, float max, float dt, float ElapsedTime, float(*func)(float));

//補間関数 MyInterpolation　使用例
/*
	enemy.pos = MyInterpolation(enemy.InterpolationStartPos, enemy.InterpolationEndPos, 0.0f, 1.0f, 1.0f, 120.0f, LerpEaseIn);
*/

/**
* @brief 補間値計算関数 Lerp 線形補間
* @param[in] float	t	時間
* @return float 計算結果補間値を返す
* @details MyInterpolation使用時に利用。引数に好きな補間値計算関数(これらの関数群)を指定して使う。
* @author : 木村純(キムラジュン)
*/
float Lerp(float t);

/**
* @brief 補間値計算関数 LerpEaseIn 二次関数下巻き補間
* @param[in] float	t	時間
* @return float 計算結果補間値を返す
* @details MyInterpolation使用時に利用。引数に好きな補間値計算関数(これらの関数群)を指定して使う。
* @author : 木村純(キムラジュン)
*/
float LerpEaseIn(float t);

/**
* @brief 補間値計算関数 LerpEaseOut 二次関数上巻き補間
* @param[in] float	t	時間
* @return float 計算結果補間値を返す
* @details MyInterpolation使用時に利用。引数に好きな補間値計算関数(これらの関数群)を指定して使う。
* @author : 木村純(キムラジュン)
*/
float LerpEaseOut(float t);

/**
* @brief 補間値計算関数 LerpEaseInEaseOut 三次関数補間
* @param[in] float	t	時間
* @return float 計算結果補間値を返す
* @details MyInterpolation使用時に利用。引数に好きな補間値計算関数(これらの関数群)を指定して使う。
* @author : 木村純(キムラジュン)
*/
float LerpEaseInEaseOut(float t);

/**
* @brief [2D用]点と直緯(2点から求める)の最短距離計算関数	 PointAndLineMinDistance
* @param[in] D3DXVECTOR3	Point		座標
* @param[in] D3DXVECTOR3	LinePoint1	座標
* @param[in] D3DXVECTOR3	LinePoint2	座標
* @return float　計算結果(最短距離)
* @details [2D用]点から直線までの最短距離を求めるときに使用
* @author : 木村純(キムラジュン)
*/
float PointAndLineMinDistance(D3DXVECTOR3 Point, D3DXVECTOR3 LinePoint1, D3DXVECTOR3 LinePoint2);

/**
* @brief [3D用]円柱と直緯の当たり判定関数	CheckHitColumnLB
* @param[in] D3DXVECTOR3	yarnvec				方向ベクトル
* @param[in] D3DXVECTOR3	cylindervec			方向ベクトル
* @param[in] D3DXVECTOR3	yarnpos				座標
* @param[in] D3DXVECTOR3	bottompos			座標
* @param[in] float			yarnradius			半径
* @param[in] float			cylinderradius		半径
* @return bool　true:当たっている　false:当たっていない
* @details 円柱と直線が当たっているか判定で使用
*/
bool CheckHitColumnLB(D3DXVECTOR3 yarnvec, D3DXVECTOR3 cylindervec, D3DXVECTOR3 yarnpos, D3DXVECTOR3 bottompos, float yarnradius, float cylinderradius);

/**
* @brief 当たり判定高速化のフラグ初期化処理	InitCntPartition
* @details 当たり判定高速化でどの平面エリアに属しているかの判定フラグを初期化する
* @author : 木村純(キムラジュン)
*/
void InitCntPartition(void);

/**
* @brief 当たり判定高速化判定関数	SpeedUpFieldHitPoly
* @param[in] D3DXVECTOR3	InPos					判定する入力座標
* @param[in] float			*HitPosUp				属しているエリアの最大Z座標
* @param[in] float			*HitPosDown				属しているエリアの最小Z座標
* @param[in] float			*HitPosLeft				属しているエリアの最大X座標
* @param[in] float			*HitPosRight			属しているエリアの最小X座標
* @param[in] float			fSideSizeXQuarter		フィールドのX1/4サイズ
* @param[in] float			fSideSizeZQuarter		フィールドのZ1/4サイズ
* @param[in] float			fSideSizeXEighth		フィールドのX1/8サイズ
* @param[in] float			fSideSizeZEighth		フィールドのZ1/8サイズ
* @details 属しているHitPosのXZ最大最小をポインターとして返す。このエリア内にInPosがあるという
* @author : 木村純(キムラジュン)
*/
void SpeedUpFieldHitPoly(D3DXVECTOR3 InPos, float *HitPosUp, float *HitPosDown, float *HitPosLeft, float *HitPosRight
	,float fSideSizeXQuarter, float fSideSizeZQuarter, float fSideSizeXEighth, float fSideSizeZEighth);

/**
* @brief 球面線形補間算出関数	SphereLinear
* @param[in] D3DXVECTOR3	*out		補間ベクトル（出力）	補間されたプレイヤーの姿勢ベクトル
* @param[in] D3DXVECTOR3	*start		開始ベクトル			プレイヤーの姿勢ベクトル
* @param[in] D3DXVECTOR3	*end		終了ベクトル			目的地形の法線ベクトル
* @param[in] float			t			スタートからエンドまでの補間値の割合　
* @details ベクトルの回転を球面で処理する。姿勢制御などで使用
* @author : 木村純(キムラジュン)
*/
void SphereLinear(D3DXVECTOR3* out, D3DXVECTOR3* start, D3DXVECTOR3* end, float t);

/**
* @brief 球面線形補間による姿勢補間関数	CalcInterPause
* @param[in] D3DXVECTOR3	*out		補間ベクトル（出力）	補間されたプレイヤーの姿勢ベクトル
* @param[in] D3DXVECTOR3	*start		開始ベクトル			プレイヤーの姿勢ベクトル
* @param[in] D3DXVECTOR3	*end		終了ベクトル			目的地形の法線ベクトル
* @param[in] float			t			スタートからエンドまでの補間値の割合　
* @details ベクトルの回転を球面で処理する。姿勢制御などで使用。マトリクスから直接制御するために使用
* @author : 木村純(キムラジュン)
*/
//D3DXMATRIX* CalcInterPause(D3DXMATRIX* out, D3DXMATRIX* start, D3DXMATRIX* end, float t);


/**
* @brief /オブジェクトの破棄(Release)処理を簡略化 safe_release
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
	FIELD_COLLISION				*Parent;				// 4分割の4分割の4分割の合計3種類(親NULL、親PARENT、親CHILD)　＝4*4*4=64タイプ
	bool						Cheak;					// 正常に当たり判定しているかどうかの判定
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
	UINT						m_PreAnimID = 0;					//前のアニメセット
	UINT						m_CurAnimID = 0;					//今のアニメセット
	float						ShiftTime[10] = { 0.0f };			//アニメがシフトする時間
	float						CurWeight[10] = { 0.0f };			//現在の姿勢係数



	// ループ時間を設定
	bool SetLoopTime(ORIANIMATION *pAnim, UINT animID, FLOAT time);
	// 動作開始にかかる時間を設定
	bool SetShiftTime(ORIANIMATION *pAnim, UINT animID, FLOAT interval);
	// アニメーションを切り替え
	bool ChangeAnimation(ORIANIMATION *pAnim, UINT NextAnimID);
	// アニメーションを更新
	bool AdvanceTime(ORIANIMATION *pAnim, FLOAT time);
};
bool SetLoopTime(ORIANIMATION *pAnim, UINT animID, FLOAT time);
bool SetShiftTime(ORIANIMATION *pAnim, UINT animID, FLOAT interval);
bool ChangeAnimation(ORIANIMATION *pAnim, UINT NextAnimID);
bool AdvanceTime(ORIANIMATION *pAnim, FLOAT time);


//アニメーション上位レベル構造体
struct HLANIMATION_DESC
{
	UINT uiAnimID;                 // アニメーションID
	ID3DXAnimationSet *pAnimSet;   // アニメーションセット
	FLOAT fLoopTime;               // 1ループの時間
	FLOAT fTrackSpeed;             // トラックスピード調節値
	FLOAT fShiftTime;              // シフトするのにかかる時間
	FLOAT fCurWeightTime;          // 現在のウェイト時間
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
