//=============================================================================
//
// ���[�t�B���O���� [morphing.cpp]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#include "main.h"
#include "morphing.h"

//=============================================================================
// �Q�[�������[�t�B���O���s�֐�
//=============================================================================
void DoMorphing(GPUMODEL *FromModel, GPUMODEL *ToModel)
{
	//���`��ԂŃ��[�t�B���O
	{
		float dt = 0.01f;	// 1�t���[���Ői�߂鎞��
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VERTEX_3D *pVtx;
		WORD *pIdx;
		VERTEX_3D *pVtxG;
		FromModel->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		FromModel->pD3DIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
		ToModel->pD3DVtxBuff->Lock(0, 0, (void**)&pVtxG, 0);
		FromModel->time += dt;		// �A�j���[�V�����̍��v���Ԃɑ���
		for (int CntPoly = 0; CntPoly < int(FromModel->nNumPolygon); CntPoly++, pIdx += 3)
		{
			// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
			D3DXVECTOR3 vtxvec1 = pVtxG[pIdx[0]].vtx - pVtx[pIdx[0]].vtx;
			D3DXVECTOR3 vtxvec2 = pVtxG[pIdx[1]].vtx - pVtx[pIdx[1]].vtx;
			D3DXVECTOR3 vtxvec3 = pVtxG[pIdx[2]].vtx - pVtx[pIdx[2]].vtx;

			pVtx[pIdx[0]].vtx = pVtx[pIdx[0]].vtx + vtxvec1 * FromModel->time;
			pVtx[pIdx[1]].vtx = pVtx[pIdx[1]].vtx + vtxvec2 * FromModel->time;
			pVtx[pIdx[2]].vtx = pVtx[pIdx[2]].vtx + vtxvec3 * FromModel->time;
		}
		// ���_�f�[�^���A�����b�N����
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
// ���Z�b�g���̃��[�t�B���O���s�֐�
//=============================================================================
void ResetMorphing(GPUMODEL *FromModel, GPUMODEL *ToModel)
{
	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
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
	// ���_�f�[�^���A�����b�N����
	ToModel->pD3DVtxBuff->Unlock();
	FromModel->pD3DVtxBuff->Unlock();
	FromModel->pD3DIdxBuff->Unlock();
}

