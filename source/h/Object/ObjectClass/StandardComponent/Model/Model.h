/**
* @file Model.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once

/**
 * @struct VERTEX_2D
 * @brief 2D�|���S�����_�t�H�[�}�b�g
 */
struct VERTEX_2D
{
	VERTEX_2D() { vtx = VEC3_ALL0; rhw = 0.0f; diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
	~VERTEX_2D() {}
	D3DXVECTOR3 vtx;		//!< ���_���W
	float		rhw;		//!< �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
 * @struct VERTEX_3D
 * @brief 3D�|���S�����_�t�H�[�}�b�g
 */
struct VERTEX_3D
{
	VERTEX_3D() { vtx = VEC3_ALL0; nor = VEC3_ALL0; diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
	~VERTEX_3D() {}
	D3DXVECTOR3 vtx;		//!< ���_���W
	D3DXVECTOR3 nor;		//!< �@���x�N�g��
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
*�@@class TEXTURE
*�@@brief �e�N�X�`���N���X
*/
class TEXTURE
{
public:
	TEXTURE() { pD3DTexture = NULL; }
	~TEXTURE() { SafeRelease(pD3DTexture); }

	//------------------------�ǂݍ��݊֐�
	void LoadTexture(const char *FileName);

	//------------------------get�֐�
	inline LPDIRECT3DTEXTURE9 Texture(void) { return pD3DTexture; };
	inline LPDIRECT3DTEXTURE9 *pTexture(void) { return &pD3DTexture; };
	//inline LPDIRECT3DTEXTURE9 Texture(int pHeadNum) const { return pD3DTexture[pHeadNum]; };

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;		//!< �e�N�X�`���ւ̃|�C���^
};

/**
*�@@class VTXBUFFER
*�@@brief ���_�o�b�t�@�N���X�@�C���^�[�t�F�[�X������̂�����
*/
class VTXBUFFER
{
public:
	VTXBUFFER() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~VTXBUFFER() { SafeRelease(pD3DVtxBuff); SafeRelease(pD3DIdxBuff); }

	//------------------------�ݒ�֐�
	//���_[�쐬]���ݒ�(3D)
	void MakeVertex3D(const int VTXnum, const DWORD FVF);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//���_[�쐬]���ݒ�(3D�r���{�[�h)
	void MakeVertex3DBill(const int CntMax, const DWORD FVF);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//���_[�쐬]���ݒ�(2D)
	void MakeVertex2D(const int CntMax, const DWORD FVF);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
		const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//�C���f�b�N�X[�쐬]
	void MakeIdxVertex(const int VTXIdxNum);

	//���_�T�C�Y�̐ݒ�
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);

	//���_�@���̐ݒ�
	void Nor3D(const int Indx);
	void Nor3D(const int Indx, const D3DXVECTOR3 Nor[4]);
	void RHW2D(const int Indx);

	//���_�J���[�̐ݒ�
	void Color3D(const int Indx);
	void Color3D(const int Indx, const D3DXCOLOR col);
	void Color2D(const int Indx);
	void Color2D(const int Indx, const D3DXCOLOR col);

	//UV�ݒ�
	void UV3D(const int Indx);
	void UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);
	void UV2D(const int Indx);
	void UV2D(const int Indx, const float PatternU, const float PatternV);
	void UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);

	//------------------------get�֐�
	D3DXCOLOR GetColor3D(const int Indx);
	D3DXCOLOR GetColor2D(const int Indx);


	//------------------------�o�b�t�@�Q
	//------------------------get�֐�
	inline LPDIRECT3DVERTEXBUFFER9 VtxBuff(void) { return pD3DVtxBuff; };
	inline LPDIRECT3DVERTEXBUFFER9 *pVtxBuff(void) { return &pD3DVtxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 *pIdxBuff(void) { return &pD3DIdxBuff; };

	//------------------------set�֐�
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

private:

	//�o�b�t�@�Q
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^ 3D���f���ǂݍ��ݎ��Ɏg�p

};

/**
*�@@class My3D_VTXBUFFER
*�@@brief ���_�o�b�t�@�N���X�@�C���^�[�t�F�[�X������̂�����
*/
class My3D_VTXBUFFER
{
public:
	My3D_VTXBUFFER() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~My3D_VTXBUFFER() { SafeRelease(pD3DVtxBuff); SafeRelease(pD3DIdxBuff); }

	//------------------------�ݒ�֐�
	//���_[�쐬]���ݒ�(3D)
	void MakeVertex3D(const int VTXnum, const DWORD FVF);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//���_[�쐬]���ݒ�(3D�r���{�[�h)
	void MakeVertex3DBill(const int CntMax, const DWORD FVF);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//�C���f�b�N�X[�쐬]
	void MakeIdxVertex3D(const int VTXIdxNum);

	//���_�T�C�Y�̐ݒ�
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);

	//���_�@���̐ݒ�
	void Nor3D(const int Indx);
	void Nor3D(const int Indx, const D3DXVECTOR3 Nor[4]);

	//���_�J���[�̐ݒ�
	void Color3D(const int Indx);
	void Color3D(const int Indx, const D3DXCOLOR col);

	//UV�ݒ�
	void UV3D(const int Indx);
	void UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);

	//------------------------get�֐�
	D3DXCOLOR GetColor3D(const int Indx);

	//------------------------�o�b�t�@�Q
	//------------------------get�֐�
	inline LPDIRECT3DVERTEXBUFFER9 VtxBuff(void) { return pD3DVtxBuff; };
	inline LPDIRECT3DVERTEXBUFFER9 *pVtxBuff(void) { return &pD3DVtxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 *pIdxBuff(void) { return &pD3DIdxBuff; };

	//------------------------set�֐�
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

private:

	//�o�b�t�@�Q
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^ 3D���f���ǂݍ��ݎ��Ɏg�p

};

/**
*�@@class My2D_VTXBUFFER
*�@@brief ���_�o�b�t�@�N���X�@�C���^�[�t�F�[�X������̂�����
*/
class My2D_VTXBUFFER
{
public:
	My2D_VTXBUFFER() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~My2D_VTXBUFFER() { SafeRelease(pD3DVtxBuff); SafeRelease(pD3DIdxBuff); }

	//------------------------�ݒ�֐�
	//���_[�쐬]���ݒ�(2D)
	void MakeVertex2D(const int CntMax, const DWORD FVF);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
		const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//�C���f�b�N�X[�쐬]
	void MakeIdxVertex2D(const int VTXIdxNum);

	//���_�T�C�Y�̐ݒ�
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);

	//���_�@���̐ݒ�
	void RHW2D(const int Indx);

	//���_�J���[�̐ݒ�
	void Color2D(const int Indx);
	void Color2D(const int Indx, const D3DXCOLOR col);

	//UV�ݒ�
	void UV2D(const int Indx);
	void UV2D(const int Indx, const float PatternU, const float PatternV);
	void UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);

	//------------------------get�֐�
	D3DXCOLOR GetColor2D(const int Indx);

	//------------------------�o�b�t�@�Q
	//------------------------get�֐�
	inline LPDIRECT3DVERTEXBUFFER9 VtxBuff(void) { return pD3DVtxBuff; };
	inline LPDIRECT3DVERTEXBUFFER9 *pVtxBuff(void) { return &pD3DVtxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 *pIdxBuff(void) { return &pD3DIdxBuff; };

	//------------------------set�֐�
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

	//------------------------�`��A�V�X�g�֐��@�g��Ȃ��ق����������ׂ��y���ł���̂Ŏg�p���Ȃ�
	void VtxBuffDraw2D(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex);
	void VtxBuffDraw3D(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex);
	void VtxBuffDraw3DIdx(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DINDEXBUFFER9 *idxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex);

private:

	//�o�b�t�@�Q
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^ 3D���f���ǂݍ��ݎ��Ɏg�p

};

/**
 * @class MODELATTRIBUTE
 * @brief 3D���f���f�[�^ BUFFER�g�p
 */
class MODELATTRIBUTE
{
public:
	MODELATTRIBUTE() { pD3DXBuffMat = NULL; nNumMat = 0; nNumVertex = 0; nNumVertexIndex = 0; nNumPolygon = 0; }
	~MODELATTRIBUTE() { SafeRelease(pD3DXBuffMat); }

	//------------------------get�֐�
	inline LPD3DXBUFFER *Mat() { return &pD3DXBuffMat; };
	inline DWORD NumMat() const { return nNumMat; };
	inline DWORD NumVertex() const { return nNumVertex; };
	inline DWORD NumVertexIndex() const { return nNumVertexIndex; };
	inline DWORD NumPolygon() const { return nNumPolygon; };

	//------------------------set�֐�
	inline void Mat(const LPD3DXBUFFER buffmat) { pD3DXBuffMat = buffmat; };
	inline void NumMat(DWORD nummat) { nNumMat = nummat; };
	inline void NumVertex(DWORD numvtx) { nNumVertex = numvtx; };
	inline void NumVertexIndex(DWORD numidx) { nNumVertexIndex = numidx; };
	inline void NumPolygon(DWORD numpoly) { nNumPolygon = numpoly; };

private:
	LPD3DXBUFFER				pD3DXBuffMat;			//!< �}�e���A�����ւ̃|�C���^
	DWORD						nNumMat;				//!< �}�e���A�����̐�
	DWORD						nNumVertex;				//!< ���_�̐�
	DWORD						nNumVertexIndex;		//!< ���_�̃C���f�b�N�X
	DWORD						nNumPolygon;			//!< �|���S���̐�

};

