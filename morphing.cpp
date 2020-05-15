//=============================================================================
//
// モーフィング処理 [morphing.cpp]
// Author : 木村純(キムラジュン)
//
//=============================================================================
#include "main.h"
#include "morphing.h"

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
void ResetMorphing(GPUMODEL *FromModel, GPUMODEL *ToModel)
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

