/**
* @file library.cpp
* @brief ライブラリ関数群
* @author キムラジュン
*/
#include "main.h"
#include "library.h"


//=============================================================================
// モデル読み込み処理
//=============================================================================
HRESULT LoadMesh(char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff,
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXMESH pMesh;
	DWORD Vertex, Polygon, VertexIndex, NumMat;
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(FileName,			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,							// メッシュの作成オプションを指定
		pDevice,									// IDirect3DDevice9インターフェイスへのポインタ
		NULL,										// 隣接性データを含むバッファへのポインタ
		pD3DXBuffMat,								// マテリアルデータを含むバッファへのポインタ
		NULL,										// エフェクトインスタンスの配列を含むバッファへのポインタ
		nNumMat,									// D3DXMATERIAL構造体の数
		pD3DXMesh)))								// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}
	pMesh = *pD3DXMesh;
	Vertex = pMesh->GetNumVertices();
	*nNumVertex = Vertex;
	Polygon = pMesh->GetNumFaces();
	*nNumPolygon = Polygon;
	VertexIndex = Polygon * 3;
	*nNumVertexIndex = VertexIndex;
	NumMat = *nNumMat;
	//pD3DTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9) * NumMat);

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		FVF_VERTEX_3D,														// 使用する頂点フォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		pD3DVtxBuff,														// 頂点バッファインターフェースへのポインタ
		NULL)))																// NULLに設定
	{
		return E_FAIL;
	}
	pMesh->GetVertexBuffer(pD3DVtxBuff);


	// オブジェクトのインデックスバッファを生成
	if (FAILED(pDevice->CreateIndexBuffer(VertexIndex,						// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// 頂点バッファの使用法　
		D3DFMT_INDEX16,														// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		pD3DIdxBuff,														// インデックスバッファインターフェースへのポインタ
		NULL)))																// NULLに設定
	{
		return E_FAIL;
	}
	pMesh->GetIndexBuffer(pD3DIdxBuff);

	if (pMesh != NULL)
	{// メッシュの開放
		pMesh->Release();
		pMesh = NULL;
	}

	return S_OK;

}

//=============================================================================
// ゲーム中モーフィング実行関数
//=============================================================================
void DoMorphing(GPUMODEL *FromModel, GPUMODEL *ToModel)
{
	//線形補間でモーフィング
	{
		float dt = 0.01f;	// 1フレームで進める時間
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VERTEX_3D *pVtx;
		WORD *pIdx;
		VERTEX_3D *pVtxG;
		FromModel->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		FromModel->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
		ToModel->pD3DVtxBuff->Lock(0, 0, (void**)&pVtxG, 0);
		FromModel->time += dt;		// アニメーションの合計時間に足す
		for (int CntPoly = 0; CntPoly < int(FromModel->nNumPolygon); CntPoly++, pIdx += 3)
		{
			// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
			D3DXVECTOR3 vtxvec1 = pVtxG[pIdx[0]].vtx - pVtx[pIdx[0]].vtx;
			D3DXVECTOR3 vtxvec2 = pVtxG[pIdx[1]].vtx - pVtx[pIdx[1]].vtx;
			D3DXVECTOR3 vtxvec3 = pVtxG[pIdx[2]].vtx - pVtx[pIdx[2]].vtx;

			pVtx[pIdx[0]].vtx = pVtx[pIdx[0]].vtx + vtxvec1 * FromModel->time;
			pVtx[pIdx[1]].vtx = pVtx[pIdx[1]].vtx + vtxvec2 * FromModel->time;
			pVtx[pIdx[2]].vtx = pVtx[pIdx[2]].vtx + vtxvec3 * FromModel->time;
		}
		// 頂点データをアンロックする
		ToModel->pD3DVtxBuff->Unlock();
		FromModel->pD3DVtxBuff->Unlock();
		FromModel->pD3DIdxBuff->Unlock();

		if (FromModel->time >= 1.0f)
		{
			FromModel->MorphingSignal = EndMorphing;
			FromModel->time = 0.0f;
		}
	}
}

//=============================================================================
// リセット時のモーフィング実行関数
//=============================================================================
void ResetModel(GPUMODEL *FromModel, GPUMODEL *ToModel)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VERTEX_3D *pVtx;
	WORD *pIdx;
	VERTEX_3D *pVtxG;
	FromModel->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	FromModel->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	ToModel->pD3DVtxBuff->Lock(0, 0, (void**)&pVtxG, 0);
	for (int CntPoly = 0; CntPoly < int(FromModel->nNumPolygon); CntPoly++, pIdx += 3)
	{
		pVtx[pIdx[0]].vtx = pVtxG[pIdx[0]].vtx;
		pVtx[pIdx[1]].vtx = pVtxG[pIdx[1]].vtx;
		pVtx[pIdx[2]].vtx = pVtxG[pIdx[2]].vtx;
	}
	// 頂点データをアンロックする
	ToModel->pD3DVtxBuff->Unlock();
	FromModel->pD3DVtxBuff->Unlock();
	FromModel->pD3DIdxBuff->Unlock();
}

//=============================================================================
// レイキャスト関数
// 戻り値：trueならレイとポリゴンの内分点をポインターに数値入力
//=============================================================================
bool RayCast(D3DXVECTOR3 rayS, D3DXVECTOR3 rayG, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 *ReturnVtx)
{
	//if(rayS >vtx)
	//外内内内分比外外外内内
	D3DXVECTOR3 cr1, cr2, cr3, cr4;
	float do1, do2, n1, n2, n3;

	//step1	レイが貫通してるかどうか判定
	D3DXVec3Cross(&cr1, &(vtx1 - vtx0), &(vtx2 - vtx0));
	float cr1Nor = D3DXVec3Length(&cr1);
	do1 = D3DXVec3Dot(&(cr1), &(rayS - vtx0));
	do2 = D3DXVec3Dot(&(cr1), &(rayG - vtx0));
	if (do1*do2 > 0) return false;

	//step2	内分比を求める
	//d1,d2=1 raySYとrayGY交点までの距離
	// n = a = d1/(d1+d2) 
	// m = 1-a 使ってp0から交点までのベクトルが出せる
	// a:1-a = d1:d2
	D3DXVECTOR3 v1 = (rayS - vtx0);
	D3DXVECTOR3 v2 = (rayG - vtx0);
	float d1 = fabsf(D3DXVec3Dot(&(cr1), &(v1)));
	float d2 = fabsf(D3DXVec3Dot(&(cr1), &(v2)));
	d1 /= cr1Nor;
	d2 /= cr1Nor;

	float naibunhi = d1 / (d1 + d2);
	D3DXVECTOR3 v3 = ((1 - naibunhi)*v1) + (naibunhi * v2);
	D3DXVECTOR3 kouten = vtx0 + v3;

	//step3	ポリゴンの内側に入ってるかの判定
	D3DXVec3Cross(&cr2, &(vtx0 - vtx1), &(kouten - vtx1));
	D3DXVec3Cross(&cr3, &(vtx1 - vtx2), &(kouten - vtx2));
	D3DXVec3Cross(&cr4, &(vtx2 - vtx0), &(kouten - vtx0));
	n1 = D3DXVec3Dot(&cr2, &cr3);
	n2 = D3DXVec3Dot(&cr3, &cr4);
	n3 = D3DXVec3Dot(&cr2, &cr4);

	if (n1 >= 0 && n2 >= 0 && n3 >= 0)
	{
		*ReturnVtx = kouten;
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// D3DXVECTOR3からfloatに変換。移動距離の値を計算する関数
// 戻り値：移動距離の値
//=============================================================================
float SpdCal(D3DXVECTOR3 move)
{
	float spd;
	float crossvec;
	crossvec = float(sqrt((move.x*move.x) + (move.z*move.z)));
	return spd = float(sqrt((move.y*move.y) + (crossvec*crossvec)));
}

//=============================================================================
// 扇と点の判定
// 戻り値：true:範囲内　false:範囲外
//=============================================================================
bool IsCollisionFanAndPoint(D3DXVECTOR3 CenterPos, D3DXVECTOR3 TargetPos, float StartRot, float EndRot, float RangeLen)
{
	float CenterToTargetx = -(TargetPos.x - CenterPos.x);
	float CenterToTargety = TargetPos.z - CenterPos.z;

	StartRot += 1.57f;
	EndRot += 1.57f;

	float sx = cosf(StartRot);
	float sy = sinf(StartRot);
	float ex = cosf(EndRot);
	float ey = sinf(EndRot);

	// 円の内外判定
	if (CenterToTargetx * CenterToTargetx + CenterToTargety * CenterToTargety > RangeLen * RangeLen)
	{
		return false;
	}
	// 扇型の角度が180を超えているか
	if (sx * ey - ex * sy > 0)
	{
		//2Dの符号付き角度＝角度の大きさ × 外積の符号
		// 開始角に対して左にあるか
		if (sx * CenterToTargety - CenterToTargetx * sy < 0)
		{
			return false;
		}
		// 終了角に対して右にあるか
		if (ex * CenterToTargety - CenterToTargetx * ey > 0)
		{
			return false;
		}
		//if ((sx * CenterToTargety - CenterToTargetx * sy) * (ex * CenterToTargety - CenterToTargetx * ey) > 0)
		//{
		//	return false;
		//}
		// 扇型の内部にあることがわかった
		return true;
	}
	else
	{
		// 開始角に対して左にあるか
		if (sx * CenterToTargety - CenterToTargetx * sy >= 0)
		{
			return true;
		}
		// 終了角に対して右にあるか
		if (ex * CenterToTargety - CenterToTargetx * ey <= 0)
		{
			return true;
		}
	}
	// 扇型の外部にあることがわかった
	return false;
}

//=============================================================================
// ホーミングタイプ01処理　常時ホーミング Y座標は変化させない
// 戻り値：なし
//=============================================================================
void HormingType01(D3DXVECTOR3 *StartPos, D3DXVECTOR3 EndPos, float MoveValue)
{
	D3DXVECTOR3 distance = EndPos - *(StartPos);
	float XZrot = atan2f(distance.z, distance.x);
	StartPos->x += (cosf(XZrot) * MoveValue);
	StartPos->z += (sinf(XZrot) * MoveValue);
}

//=============================================================================
// 補間関数(Lerp = Linear interpolation)
// 戻り値：補間計算後のD3DXVECTOR3
//=============================================================================
D3DXVECTOR3 MyInterpolation(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos, float min, float max, float dt, float ElapsedTime, float(*func)(float))
{
	ElapsedTime += dt;									// 経過時間に偏差時間を代入

	float t = (ElapsedTime - min) / (max - min);		// 経過時間をもとに全体からみての時間位置を求める

	if (t < 0.0f) t = 0.0f;
	else if (t > 1.0f) t = 1.0f;

	float rate = (*func)(t);							// 引数の補間値に変換

	return StartPos * (1.0f - rate) + EndPos * rate;	//補間値を使い
}

//=============================================================================
// 線形補間　計算関数	MyInterpolation使用時に利用
// 戻り値：float　計算結果
//=============================================================================
float Lerp(float t)
{
	return t;
}

//=============================================================================
// 二次関数下巻き補間　計算関数	MyInterpolation使用時に利用
// 戻り値：float　計算結果
//=============================================================================
float LerpEaseIn(float t)
{
	return t * t;
}

//=============================================================================
// 二次関数上巻き補間　計算関数	MyInterpolation使用時に利用
// 戻り値：float　計算結果
//=============================================================================
float LerpEaseOut(float t)
{
	return 2.0f * t - t * t;
}

//=============================================================================
// 三次関数補間　計算関数	MyInterpolation使用時に利用
// 戻り値：float　計算結果
//=============================================================================
float LerpEaseInEaseOut(float t)
{
	return t * t * (3.0f - 2.0f * t);
}



bool CheckHitColumnLB(D3DXVECTOR3 yarnvec, D3DXVECTOR3 vec, D3DXVECTOR3 yarnpos, D3DXVECTOR3 bottompos, float yarnradius, float radius)
{
	//yarnpos 中心座標 
	//pos 円2中心座標		//pos 円2中心座標
	//yarnradius 糸の半径		//yarnradius 糸の半径
	//radius 円の半径		//radius 円の半径
	//yarnlength 糸の長さ		//yarnlength 糸の長さ
	//length 円柱の高さ		//length 円柱の高さ
	D3DXVECTOR3 normalvec;
	D3DXVec3Cross(&normalvec, &yarnvec, &vec);
	D3DXVECTOR3 vOneToTwo = yarnpos - bottompos; //円１と円２の間のベクトル
	float inner_product = D3DXVec3Dot(&normalvec, &vOneToTwo);
	//float DistSq = D3DXVec3LengthSq(&naiseki); //ベクトルの長さの二乗
	inner_product *= inner_product;		inner_product *= inner_product;
	if (inner_product < (yarnradius + radius)*(yarnradius + radius)) //半径の合計の二乗と比較
	{
		{
			return true;
		}
	}
	return false;
}


//--------------------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory for a string to hold the name of a frame or mesh
//--------------------------------------------------------------------------------------
HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}



//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateFrame()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED* pFrame;

	*ppNewFrame = NULL;

	pFrame = new D3DXFRAME_DERIVED;
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
		goto e_Exit;

	// initialize other data members of the frame
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

e_Exit:
	delete pFrame;
	return hr;
}



//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateMeshContainer()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	// this sample does not handle patch meshes, so fail when one is found
	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// get the pMesh interface pointer out of the mesh data structure
	pMesh = pMeshData->pMesh;

	// this sample does not FVF compatible meshes, so fail when one is found
	if (pMesh->GetFVF() == 0)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// allocate the overloaded structure to return as a D3DXMESHCONTAINER
	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	if (pMeshContainer == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

	// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
		goto e_Exit;

	pMesh->GetDevice(&pd3dDevice);
	NumFaces = pMesh->GetNumFaces();

	// if no normals are in the mesh, add them
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// clone the mesh to make room for the normals
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// now generate the normals for the pmesh
		D3DXComputeNormals(pMesh, NULL);
	}
	else  // if no normals, just add a reference to the mesh for the mesh container
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// allocate memory to contain the material information.  This sample uses
	//   the D3D9 materials and texture names instead of the EffectInstance style materials
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	// if materials provided, copy them
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				/*
				WCHAR wszBuf[MAX_PATH] = {NULL};
				MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[iMaterial].pTextureFilename, -1, wszBuf, MAX_PATH);

				//DXUTFindDXSDKMediaFileCch(strTexturePath, MAX_PATH, wszBuf);
				LPSTR texname = "data/MODEL/";
				int texnamelen=0;
				texnamelen=strlen(texname);
				LPSTR texname2 = {NULL};
				int itex = 0;
				for (int i = texnamelen+1; i < MAX_PATH; i+=1)
				{
					if (pMeshContainer->pMaterials[iMaterial].pTextureFilename[i] != NULL)
					{
						texname[i] = char(pMeshContainer->pMaterials[iMaterial].pTextureFilename[itex]);
						itex++;
					}
				}

				strcat(texname, texname2);

				if (FAILED(D3DXCreateTextureFromFile(pd3dDevice,
					texname,
					&pMeshContainer->ppTextures[iMaterial])))
				{
					pMeshContainer->ppTextures[iMaterial] = NULL;
				}

				// don't remember a pointer into the dynamic memory, just forget the name after loading
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
				*/
			}
		}
	}
	else // if no materials provided, use a default one
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// if there is skinning information, save off the required data and then setup for HW skinning
	if (pSkinInfo != NULL)
	{
		// first save off the SkinInfo and original mesh data
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		// get each of the bone offset matrices so that we don't need to get them later
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}

		// GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
		hr = GenerateSkinnedMesh(pMeshContainer, SkinningMethod, pBoneMatrices, NumBoneMatricesMax, &bUseSoftwareVP);
		if (FAILED(hr))
			goto e_Exit;
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

e_Exit:
	SAFE_RELEASE(pd3dDevice);

	// call Destroy function to properly clean up the memory allocated 
	if (pMeshContainer != NULL)
	{
		DestroyMeshContainer(pMeshContainer);
	}

	return hr;
}




//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}




//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	// release all the allocated textures
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_DELETE(pMeshContainer);
	return S_OK;
}



//--------------------------------------------------------------------------------------
// Called either by CreateMeshContainer when loading a skin mesh, or when 
// changing methods.  This function uses the pSkinInfo of the mesh 
// container to generate the desired drawable mesh and bone combination 
// table.
//--------------------------------------------------------------------------------------
HRESULT GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED* pMeshContainer, METHOD SkinningMethod, D3DXMATRIXA16* pBoneMatrices, UINT NumBoneMatricesMax, bool *bUseSoftwareVP)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	HRESULT hr = S_OK;
	D3DCAPS9 d3dCaps;
	pDevice->GetDeviceCaps(&d3dCaps);

	if (pMeshContainer->pSkinInfo == NULL)
		return hr;

	*bUseSoftwareVP = false;

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

	// if non-indexed skinning mode selected, use ConvertToBlendedMesh to generate drawable mesh
	if (SkinningMethod == D3DNONINDEXED)
	{

		hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh
		(
			pMeshContainer->pOrigMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh
		);
		if (FAILED(hr))
			goto e_Exit;


		// If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
		// Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
		// drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing.
		LPD3DXBONECOMBINATION rgBoneCombinations = reinterpret_cast<LPD3DXBONECOMBINATION>(
			pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// look for any set of bone combinations that do not fit the caps
		for (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups;
			pMeshContainer->iAttributeSW++)
		{
			DWORD cInfl = 0;

			for (DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl++)
			{
				if (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX)
				{
					++cInfl;
				}
			}

			if (cInfl > d3dCaps.MaxVertexBlendMatrices)
			{
				break;
			}
		}

		// if there is both HW and SW, add the Software Processing flag
		if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
		{
			LPD3DXMESH pMeshTmp;

			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING |
				pMeshContainer->MeshData.pMesh->GetOptions(),
				pMeshContainer->MeshData.pMesh->GetFVF(),
				pDevice, &pMeshTmp);
			if (FAILED(hr))
			{
				goto e_Exit;
			}

			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMeshTmp;
			pMeshTmp = NULL;
		}
	}
	// if indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
	else if (SkinningMethod == D3DINDEXED)
	{
		DWORD NumMaxFaceInfl;
		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

		LPDIRECT3DINDEXBUFFER9 pIB;
		hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
		if (FAILED(hr))
			goto e_Exit;

		hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB,
			pMeshContainer->pOrigMesh->GetNumFaces(),
			&NumMaxFaceInfl);
		pIB->Release();
		if (FAILED(hr))
			goto e_Exit;

		// 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
		// can be handled
		NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

		if (d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl)
		{
			// HW does not support indexed vertex blending. Use SW instead
			pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
			pMeshContainer->UseSoftwareVP = true;
			*bUseSoftwareVP = true;
			Flags |= D3DXMESH_SYSTEMMEM;
		}
		else
		{
			// using hardware - determine palette size from caps and number of bones
			// If normals are present in the vertex data that needs to be blended for lighting, then 
			// the number of matrices is half the number specified by MaxVertexBlendMatrixIndex.
			pMeshContainer->NumPaletteEntries = min((d3dCaps.MaxVertexBlendMatrixIndex + 1) / 2,
				pMeshContainer->pSkinInfo->GetNumBones());
			pMeshContainer->UseSoftwareVP = false;
			Flags |= D3DXMESH_MANAGED;
		}

		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
		(
			pMeshContainer->pOrigMesh,
			Flags,
			pMeshContainer->NumPaletteEntries,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;
	}
	// if vertex shader indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
	else if ((SkinningMethod == D3DINDEXEDVS) || (SkinningMethod == D3DINDEXEDHLSLVS))
	{
		// Get palette size
		// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
		// (96 - 9) /3 i.e. Maximum constant count - used constants 
		UINT MaxMatrices = 26;
		pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
		if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
			pMeshContainer->UseSoftwareVP = false;
			Flags |= D3DXMESH_MANAGED;
		}
		else
		{
			pMeshContainer->UseSoftwareVP = true;
			*bUseSoftwareVP = true;
			Flags |= D3DXMESH_SYSTEMMEM;
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
		(
			pMeshContainer->pOrigMesh,
			Flags,
			pMeshContainer->NumPaletteEntries,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;


		// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
		DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
			D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
		{
			LPD3DXMESH pMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF,
				pDevice, &pMesh);
			if (!FAILED(hr))
			{
				pMeshContainer->MeshData.pMesh->Release();
				pMeshContainer->MeshData.pMesh = pMesh;
				pMesh = NULL;
			}
		}

		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		LPD3DVERTEXELEMENT9 pDeclCur;
		hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
		//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
		//          this is more of a "cast" operation
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
		if (NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
		{
			NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

			// Allocate space for blend matrices
			delete[] pBoneMatrices;
			pBoneMatrices = new D3DXMATRIXA16[NumBoneMatricesMax];
			if (pBoneMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}
		}

	}
	// if software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
	else if (SkinningMethod == SOFTWARE)
	{
		hr = pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
			pDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;

		hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
		if (FAILED(hr))
			goto e_Exit;

		delete[] pMeshContainer->pAttributeTable;
		pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
		if (pMeshContainer->pAttributeTable == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
		if (FAILED(hr))
			goto e_Exit;

		// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
		if (NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
		{
			NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

			// Allocate space for blend matrices
			delete[] pBoneMatrices;
			pBoneMatrices = new D3DXMATRIXA16[NumBoneMatricesMax];
			if (pBoneMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}
		}
	}
	else  // invalid g_SkinningMethod value
	{
		// return failure due to invalid skinning method value
		hr = E_INVALIDARG;
		goto e_Exit;
	}

e_Exit:
	return hr;
}




//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void OnFrameMoveAnime(double fTime, ID3DXAnimationController* AnimController)
{
	if (AnimController != NULL)
		AnimController->AdvanceTime(fTime, NULL);
}


//--------------------------------------------------------------------------------------
// Called to render a mesh in the hierarchy
//--------------------------------------------------------------------------------------
void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, METHOD SkinningMethod, LPDIRECT3DTEXTURE9 pD3DTexture)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pDevice->GetDeviceCaps(&d3dCaps);

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		if (SkinningMethod == D3DNONINDEXED)
		{
			AttribIdPrev = UNUSED32;
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer
			());

			// Draw using default vtx processing of the device (typically HW)
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				NumBlend = 0;
				for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
				{
					if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
					{
						NumBlend = i;
					}
				}

				if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
				{
					// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
					for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
					{
						iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
						if (iMatrixIndex != UINT_MAX)
						{
							D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
								pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
							pDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
						}
					}

					pDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

					// lookup the material used for this subset of faces
					if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
					{
						pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
						pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
						pDevice->SetTexture(0, pD3DTexture);
						AttribIdPrev = pBoneComb[iAttrib].AttribId;
					}

					// draw the subset now that the correct material and matrices are loaded
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
				}
			}

			// If necessary, draw parts that HW could not handle using SW
			if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
			{
				AttribIdPrev = UNUSED32;
				pDevice->SetSoftwareVertexProcessing(TRUE);
				for (iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
				{
					NumBlend = 0;
					for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
					{
						if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
						{
							NumBlend = i;
						}
					}

					if (d3dCaps.MaxVertexBlendMatrices < NumBlend + 1)
					{
						// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
						for (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
						{
							iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
							if (iMatrixIndex != UINT_MAX)
							{
								D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
									pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
								pDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
							}
						}

						pDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

						// lookup the material used for this subset of faces
						if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
						{
							pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
							pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
							pDevice->SetTexture(0, pD3DTexture);
							AttribIdPrev = pBoneComb[iAttrib].AttribId;
						}

						// draw the subset now that the correct material and matrices are loaded
						pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
					}
				}
				pDevice->SetSoftwareVertexProcessing(FALSE);
			}

			pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		}
		else // bug out as unsupported mode
		{
			return;
		}
	}
	else  // standard mesh, just draw it after setting material properties
	{
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			D3DMATERIAL9 matDef;
			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);


			//pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pDevice->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
			pDevice->SetTexture(0, pD3DTexture);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);

			// マテリアルをデフォルトに戻す
			//pDevice->SetMaterial(&matDef);
		}
	}
}




//--------------------------------------------------------------------------------------
// Called to render a frame in the hierarchy
//--------------------------------------------------------------------------------------
void DrawFrame(LPD3DXFRAME pFrame, METHOD SkinningMethod, LPDIRECT3DTEXTURE9 pD3DTexture)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pMeshContainer, pFrame, SkinningMethod, pD3DTexture);

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling, SkinningMethod, pD3DTexture);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild, SkinningMethod, pD3DTexture);
	}
}



//--------------------------------------------------------------------------------------
// Called to setup the pointers for a given bone to its transformation matrix
//--------------------------------------------------------------------------------------
HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameRoot)
{
	UINT iBone, cBones;
	D3DXFRAME_DERIVED* pFrame;

	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	// if there is a skinmesh, then setup the bone matrices
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFrameRoot,
				pMeshContainer->pSkinInfo->GetBoneName(iBone));
			if (pFrame == NULL)
				return E_FAIL;

			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called to setup the pointers for a given bone to its transformation matrix
//--------------------------------------------------------------------------------------
HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pFrameRoot)
{
	HRESULT hr;

	if (pFrame->pMeshContainer != NULL)
	{
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer, pFrameRoot);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling, pFrameRoot);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild, pFrameRoot);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}




//--------------------------------------------------------------------------------------
// update the frame matrices
//--------------------------------------------------------------------------------------
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

	if (pParentMatrix != NULL)
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


//--------------------------------------------------------------------------------------
// update the skinning method
//--------------------------------------------------------------------------------------
void UpdateSkinningMethod(LPD3DXFRAME pFrameBase, METHOD SkinningMethod, D3DXMATRIXA16* pBoneMatrices, UINT NumBoneMatricesMax, bool* bUseSoftwareVP)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer;

	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

	while (pMeshContainer != NULL)
	{
		GenerateSkinnedMesh(pMeshContainer, SkinningMethod, pBoneMatrices, NumBoneMatricesMax, bUseSoftwareVP);

		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		UpdateSkinningMethod(pFrame->pFrameSibling, SkinningMethod, pBoneMatrices, NumBoneMatricesMax, bUseSoftwareVP);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateSkinningMethod(pFrame->pFrameFirstChild, SkinningMethod, pBoneMatrices, NumBoneMatricesMax, bUseSoftwareVP);
	}
}


void ReleaseAttributeTable(LPD3DXFRAME pFrameBase)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer;

	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

	while (pMeshContainer != NULL)
	{
		delete[] pMeshContainer->pAttributeTable;

		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		ReleaseAttributeTable(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		ReleaseAttributeTable(pFrame->pFrameFirstChild);
	}
}







//アニメーションセットのロード　Xファイルに複数保存したアニメーション情報を一個づつ読み込み。読み込んだアニメーションセットはSetTrackAnimationSet()にセットすると再生
//Xファイルのアニメーションセットに保存されているインデックスとGetAnimationSet()のインデックスは順番が反対になっている。
//if　Xファイル(0歩く1撃つ2止まる)→GetAnimationSet(0止まる1撃つ2歩く)
void LoadAnimSet(ORIANIMATION *pAnim)
{
	for (DWORD i = 0; i < pAnim->pAnimController->GetNumAnimationSets(); i++)
	{
		pAnim->pAnimController->GetAnimationSet(i, &pAnim->pAnimSetKeep[i]);
	}

	//とりあえず撃つアニメセットをセット。現在のアニメIDも更新
	pAnim->pAnimController->SetTrackAnimationSet(0, pAnim->pAnimSetKeep[1]);
	pAnim->m_CurAnimID = 1;


}


//ループ時間を設定（SetLoopTimeメソッド）
bool ORIANIMATION::SetLoopTime(ORIANIMATION *pAnim, UINT animID, FLOAT time)
{
	// 指定のアニメーションIDの存在をチェック
	if (pAnim->pAnimController->GetNumAnimationSets() <= animID)
		return false;

	// トラックスピード調節値を算出
	FLOAT DefTime = FLOAT(pAnim->pAnimSetKeep[animID]->GetPeriod());
	pAnim->pAnimController->SetTrackSpeed(animID, DefTime / time);

	return true;
}

bool SetLoopTime(ORIANIMATION *pAnim, UINT animID, FLOAT time)
{
	// 指定のアニメーションIDの存在をチェック
	if (pAnim->pAnimController->GetNumAnimationSets() <= animID)
		return false;

	// トラックスピード調節値を算出
	FLOAT DefTime = FLOAT(pAnim->pAnimSetKeep[animID]->GetPeriod());
	pAnim->pAnimController->SetTrackSpeed(animID, DefTime / time);

	return true;
}


//動作開始にかかる時間を設定（SetShiftTimeメソッド）
bool ORIANIMATION::SetShiftTime(ORIANIMATION *pAnim, UINT animID, FLOAT interval)
{
	//// 指定のアニメーションIDの存在をチェック
	//if (pAnim->pAnimController->GetNumAnimationSets() <= animID)
	//	return false;

	//// シフト時間を登録
	//pAnim->pAnimController.
	//m_Anim[animID].ShiftTime = interval;

	return true;
}

bool SetShiftTime(ORIANIMATION *pAnim, UINT animID, FLOAT interval)
{
	// 指定のアニメーションIDの存在をチェック
	if (pAnim->pAnimController->GetNumAnimationSets() <= animID)
		return false;

	// シフト時間を登録
	pAnim->ShiftTime[animID] = interval;

	return true;
}


//アニメーションの切り替え（ChangeAnimationメソッド）
bool ORIANIMATION::ChangeAnimation(ORIANIMATION *pAnim, UINT NextAnimID)
{
	// 指定のアニメーションIDの存在をチェック
	if (pAnim->pAnimController->GetNumAnimationSets() <= NextAnimID)
		return false;

	// 異なるアニメーションであるかをチェック
	if (pAnim->pAnimController->GetNumAnimationSets() == NextAnimID)
		return false;

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	pAnim->pAnimController->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	pAnim->pAnimController->SetTrackAnimationSet(1, pAnim->pAnimSetKeep[m_CurAnimID]);
	pAnim->pAnimController->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	pAnim->pAnimController->SetTrackAnimationSet(0, pAnim->pAnimSetKeep[NextAnimID]);

	// トラック0のスピードの設定
	pAnim->pAnimController->SetTrackSpeed(0, FLOAT(pAnim->pAnimSetKeep[NextAnimID]->GetPeriod()));

	// トラックの合成を許可
	pAnim->pAnimController->SetTrackEnable(0, true);
	pAnim->pAnimController->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	pAnim->CurWeight[NextAnimID] = 0.0f;

	// 現在のアニメーション番号を切り替え
	m_PreAnimID = m_CurAnimID;
	m_CurAnimID = NextAnimID;

	return true;
}


bool ChangeAnimation(ORIANIMATION *pAnim, UINT NextAnimID)
{
	// 指定のアニメーションIDの存在をチェック
	if (pAnim->pAnimController->GetNumAnimationSets() <= NextAnimID)
		return false;

	// 異なるアニメーションであるかをチェック
	if (pAnim->m_CurAnimID == NextAnimID)
		return false;

	// 現在のアニメーションセットの設定値を取得
	D3DXTRACK_DESC TD;   // トラックの能力
	pAnim->pAnimController->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	pAnim->pAnimController->SetTrackAnimationSet(1, pAnim->pAnimSetKeep[pAnim->m_CurAnimID]);
	pAnim->pAnimController->SetTrackDesc(1, &TD);

	// 新しいアニメーションセットをトラック0に設定
	pAnim->pAnimController->SetTrackAnimationSet(0, pAnim->pAnimSetKeep[NextAnimID]);

	// トラック0のスピードの設定
	pAnim->pAnimController->SetTrackSpeed(0, FLOAT(pAnim->pAnimSetKeep[NextAnimID]->GetPeriod()));

	// トラックの合成を許可
	pAnim->pAnimController->SetTrackEnable(0, true);
	pAnim->pAnimController->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	pAnim->CurWeight[NextAnimID] = 0.0f;

	// 現在のアニメーション番号を切り替え
	pAnim->m_PreAnimID = pAnim->m_CurAnimID;
	pAnim->m_CurAnimID = NextAnimID;

	return true;
}

//アニメーションを更新（AdvanceTimeメソッド）
bool ORIANIMATION::AdvanceTime(ORIANIMATION *pAnim, FLOAT time)
{
	// 合成中か否かを判定
	pAnim->CurWeight[pAnim->m_CurAnimID] += time;


	if (pAnim->CurWeight[pAnim->m_CurAnimID] < pAnim->ShiftTime[pAnim->m_CurAnimID])
	{
		// 合成中。ウェイトを算出
		FLOAT Weight = pAnim->CurWeight[pAnim->m_CurAnimID] / pAnim->ShiftTime[pAnim->m_CurAnimID];
		// ウェイトを登録
		pAnim->pAnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
		pAnim->pAnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
	}
	else
	{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		pAnim->pAnimController->SetTrackWeight(0, 1.0f);       // 現在のアニメーション
		pAnim->pAnimController->SetTrackEnable(1, false);      // 前のアニメーションを無効にする
	}


	// 時間を更新
	pAnim->pAnimController->AdvanceTime(time, NULL);

	return true;
}


bool AdvanceTime(ORIANIMATION *pAnim, FLOAT time)
{
	// 合成中か否かを判定
	pAnim->CurWeight[pAnim->m_CurAnimID] += time;

	if (pAnim->CurWeight[pAnim->m_CurAnimID] < pAnim->ShiftTime[pAnim->m_CurAnimID])
	{
		// 合成中。ウェイトを算出
		FLOAT Weight = pAnim->CurWeight[pAnim->m_CurAnimID] / pAnim->ShiftTime[pAnim->m_CurAnimID];
		// ウェイトを登録
		pAnim->pAnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
		pAnim->pAnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
	}
	else
	{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		pAnim->pAnimController->SetTrackWeight(0, 1.0f);       // 現在のアニメーション
		pAnim->pAnimController->SetTrackEnable(1, false);      // 前のアニメーションを無効にする
	}

	// 時間を更新
	pAnim->pAnimController->AdvanceTime(time, NULL);

	return true;
}
