/**
* @file load.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT LoadMesh(char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff, 
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture);