//=============================================================================
//
// ���f���ǂݍ��ݏ��� [load.cpp]
// Author : �ؑ���(�L�����W����)
//
//=============================================================================
#include "main.h"
#include "load.h"

//=============================================================================
// ���b�V�����[�h�֐�
//=============================================================================
HRESULT LoadMesh(char *FileName, LPD3DXBUFFER *pD3DXBuffMat, DWORD *nNumMat, LPD3DXMESH *pD3DXMesh,
	LPDIRECT3DVERTEXBUFFER9 *pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 *pD3DIdxBuff, 
	DWORD *nNumVertex, DWORD *nNumPolygon, DWORD *nNumVertexIndex, LPDIRECT3DTEXTURE9 *pD3DTexture)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXMESH pMesh;
	DWORD Vertex, Polygon, VertexIndex,NumMat;
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(FileName,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
		pDevice,									// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		pD3DXBuffMat,								// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		nNumMat,									// D3DXMATERIAL�\���̂̐�
		pD3DXMesh)))								// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
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

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,													// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,														// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,													// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		pD3DVtxBuff,														// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))																// NULL�ɐݒ�
	{
		return E_FAIL;
	}
	pMesh->GetVertexBuffer(pD3DVtxBuff);


	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateIndexBuffer(VertexIndex,						// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,													// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,														// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,													// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		pD3DIdxBuff,														// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))																// NULL�ɐݒ�
	{
		return E_FAIL;
	}
	pMesh->GetIndexBuffer(pD3DIdxBuff);

	if (pMesh != NULL)
	{// ���b�V���̊J��
		pMesh->Release();
		pMesh = NULL;
	}

	return S_OK;

}
