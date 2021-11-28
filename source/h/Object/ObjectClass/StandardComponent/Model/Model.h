/**
* @file Model.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
 * @struct VERTEX_2D
 * @brief 2Dポリゴン頂点フォーマット
 */
struct VERTEX_2D
{
	VERTEX_2D() { vtx = VEC3_ALL0; rhw = 0.0f; diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
	~VERTEX_2D() {}
	D3DXVECTOR3 vtx;		//!< 頂点座標
	float		rhw;		//!< テクスチャのパースペクティブコレクト用
	D3DCOLOR	diffuse;	//!< 反射光
	D3DXVECTOR2 tex;		//!< テクスチャ座標
};

/**
 * @struct VERTEX_3D
 * @brief 3Dポリゴン頂点フォーマット
 */
struct VERTEX_3D
{
	VERTEX_3D() { vtx = VEC3_ALL0; nor = VEC3_ALL0; diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
	~VERTEX_3D() {}
	D3DXVECTOR3 vtx;		//!< 頂点座標
	D3DXVECTOR3 nor;		//!< 法線ベクトル
	D3DCOLOR	diffuse;	//!< 反射光
	D3DXVECTOR2 tex;		//!< テクスチャ座標
};

/**
*　@class TEXTURE
*　@brief テクスチャクラス
*/
class TEXTURE
{
public:
	TEXTURE() { pD3DTexture = NULL; }
	~TEXTURE() { SafeRelease(pD3DTexture); }

	//------------------------読み込み関数
	void LoadTexture(const char *FileName);

	//------------------------get関数
	inline LPDIRECT3DTEXTURE9 Texture(void) { return pD3DTexture; };
	inline LPDIRECT3DTEXTURE9 *pTexture(void) { return &pD3DTexture; };
	//inline LPDIRECT3DTEXTURE9 Texture(int pHeadNum) const { return pD3DTexture[pHeadNum]; };

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;		//!< テクスチャへのポインタ
};

/**
*　@class VTXBUFFER
*　@brief 頂点バッファクラス　インターフェース化するのもあり
*/
class VTXBUFFER
{
public:
	VTXBUFFER() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~VTXBUFFER() { SafeRelease(pD3DVtxBuff); SafeRelease(pD3DIdxBuff); }

	//------------------------設定関数
	//頂点[作成]時設定(3D)
	void MakeVertex3D(const int VTXnum, const DWORD FVF);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//頂点[作成]時設定(3Dビルボード)
	void MakeVertex3DBill(const int CntMax, const DWORD FVF);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//頂点[作成]時設定(2D)
	void MakeVertex2D(const int CntMax, const DWORD FVF);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
		const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//インデックス[作成]
	void MakeIdxVertex(const int VTXIdxNum);

	//頂点サイズの設定
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);

	//頂点法線の設定
	void Nor3D(const int Indx);
	void Nor3D(const int Indx, const D3DXVECTOR3 Nor[4]);
	void RHW2D(const int Indx);

	//頂点カラーの設定
	void Color3D(const int Indx);
	void Color3D(const int Indx, const D3DXCOLOR col);
	void Color2D(const int Indx);
	void Color2D(const int Indx, const D3DXCOLOR col);

	//UV設定
	void UV3D(const int Indx);
	void UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);
	void UV2D(const int Indx);
	void UV2D(const int Indx, const float PatternU, const float PatternV);
	void UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);

	//------------------------get関数
	D3DXCOLOR GetColor3D(const int Indx);
	D3DXCOLOR GetColor2D(const int Indx);


	//------------------------バッファ群
	//------------------------get関数
	inline LPDIRECT3DVERTEXBUFFER9 VtxBuff(void) { return pD3DVtxBuff; };
	inline LPDIRECT3DVERTEXBUFFER9 *pVtxBuff(void) { return &pD3DVtxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 *pIdxBuff(void) { return &pD3DIdxBuff; };

	//------------------------set関数
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

private:

	//バッファ群
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< インデックスバッファインターフェースへのポインタ 3Dモデル読み込み時に使用

};

/**
*　@class My3D_VTXBUFFER
*　@brief 頂点バッファクラス　インターフェース化するのもあり
*/
class My3D_VTXBUFFER
{
public:
	My3D_VTXBUFFER() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~My3D_VTXBUFFER() { SafeRelease(pD3DVtxBuff); SafeRelease(pD3DIdxBuff); }

	//------------------------設定関数
	//頂点[作成]時設定(3D)
	void MakeVertex3D(const int VTXnum, const DWORD FVF);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//頂点[作成]時設定(3Dビルボード)
	void MakeVertex3DBill(const int CntMax, const DWORD FVF);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//インデックス[作成]
	void MakeIdxVertex3D(const int VTXIdxNum);

	//頂点サイズの設定
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);

	//頂点法線の設定
	void Nor3D(const int Indx);
	void Nor3D(const int Indx, const D3DXVECTOR3 Nor[4]);

	//頂点カラーの設定
	void Color3D(const int Indx);
	void Color3D(const int Indx, const D3DXCOLOR col);

	//UV設定
	void UV3D(const int Indx);
	void UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);

	//------------------------get関数
	D3DXCOLOR GetColor3D(const int Indx);

	//------------------------バッファ群
	//------------------------get関数
	inline LPDIRECT3DVERTEXBUFFER9 VtxBuff(void) { return pD3DVtxBuff; };
	inline LPDIRECT3DVERTEXBUFFER9 *pVtxBuff(void) { return &pD3DVtxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 *pIdxBuff(void) { return &pD3DIdxBuff; };

	//------------------------set関数
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

private:

	//バッファ群
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< インデックスバッファインターフェースへのポインタ 3Dモデル読み込み時に使用

};

/**
*　@class My2D_VTXBUFFER
*　@brief 頂点バッファクラス　インターフェース化するのもあり
*/
class My2D_VTXBUFFER
{
public:
	My2D_VTXBUFFER() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~My2D_VTXBUFFER() { SafeRelease(pD3DVtxBuff); SafeRelease(pD3DIdxBuff); }

	//------------------------設定関数
	//頂点[作成]時設定(2D)
	void MakeVertex2D(const int CntMax, const DWORD FVF);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
		const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//インデックス[作成]
	void MakeIdxVertex2D(const int VTXIdxNum);

	//頂点サイズの設定
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);

	//頂点法線の設定
	void RHW2D(const int Indx);

	//頂点カラーの設定
	void Color2D(const int Indx);
	void Color2D(const int Indx, const D3DXCOLOR col);

	//UV設定
	void UV2D(const int Indx);
	void UV2D(const int Indx, const float PatternU, const float PatternV);
	void UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);

	//------------------------get関数
	D3DXCOLOR GetColor2D(const int Indx);

	//------------------------バッファ群
	//------------------------get関数
	inline LPDIRECT3DVERTEXBUFFER9 VtxBuff(void) { return pD3DVtxBuff; };
	inline LPDIRECT3DVERTEXBUFFER9 *pVtxBuff(void) { return &pD3DVtxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	inline LPDIRECT3DINDEXBUFFER9 *pIdxBuff(void) { return &pD3DIdxBuff; };

	//------------------------set関数
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

	//------------------------描画アシスト関数　使わないほうが処理負荷を軽くできるので使用しない
	void VtxBuffDraw2D(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex);
	void VtxBuffDraw3D(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex);
	void VtxBuffDraw3DIdx(const LPDIRECT3DVERTEXBUFFER9 *vtxbuff, const LPDIRECT3DINDEXBUFFER9 *idxbuff, const LPDIRECT3DTEXTURE9 *texture, const int StartVertex);

private:

	//バッファ群
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< インデックスバッファインターフェースへのポインタ 3Dモデル読み込み時に使用

};

/**
 * @class MODELATTRIBUTE
 * @brief 3Dモデルデータ BUFFER使用
 */
class MODELATTRIBUTE
{
public:
	MODELATTRIBUTE() { pD3DXBuffMat = NULL; nNumMat = 0; nNumVertex = 0; nNumVertexIndex = 0; nNumPolygon = 0; }
	~MODELATTRIBUTE() { SafeRelease(pD3DXBuffMat); }

	//------------------------get関数
	inline LPD3DXBUFFER *Mat() { return &pD3DXBuffMat; };
	inline DWORD NumMat() const { return nNumMat; };
	inline DWORD NumVertex() const { return nNumVertex; };
	inline DWORD NumVertexIndex() const { return nNumVertexIndex; };
	inline DWORD NumPolygon() const { return nNumPolygon; };

	//------------------------set関数
	inline void Mat(const LPD3DXBUFFER buffmat) { pD3DXBuffMat = buffmat; };
	inline void NumMat(DWORD nummat) { nNumMat = nummat; };
	inline void NumVertex(DWORD numvtx) { nNumVertex = numvtx; };
	inline void NumVertexIndex(DWORD numidx) { nNumVertexIndex = numidx; };
	inline void NumPolygon(DWORD numpoly) { nNumPolygon = numpoly; };

private:
	LPD3DXBUFFER				pD3DXBuffMat;			//!< マテリアル情報へのポインタ
	DWORD						nNumMat;				//!< マテリアル情報の数
	DWORD						nNumVertex;				//!< 頂点の数
	DWORD						nNumVertexIndex;		//!< 頂点のインデックス
	DWORD						nNumPolygon;			//!< ポリゴンの数

};

