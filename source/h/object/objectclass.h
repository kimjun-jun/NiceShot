/**
* @file objectclass.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//ネット対戦のときに使う
void SetNetMatchFlag(bool flag);
void SetNetMyNumberFlagFlag(bool flag);
void SetNetMyNumber(int num);
int GetNetMyNumber(void);
void SetNetItemFlag(bool flag);
bool GetNetItemFlag(void);
void SetNetGameStartFlag(bool flag);
void SetNetShareDateFlag(bool flag);
bool GetNetShareDateFlag(void);

class PLAYER;
class EFFECT;
class BULLET;
class SHADOW;
class COUNTDOWN;
class NETMATCH;
class TUTO;
class STATUS;
class BULLETPREDICTION;
class VITALGAUGE;
class BULLETGAUGE;
class DAMEGE;
class EXPLOSION;
class ITEM;
class RANK;
class RESULT;
class TITLE;
class FIELD;
class SKY;
class WALL;
class FADE;

/**
 * @class GAMEOBJECT
 * ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 */
class GAME_OBJECT
{
public:
	GAME_OBJECT() {
		player = NULL;
		effect = NULL;
		bullet = NULL;
		shadow = NULL;
		countdown = NULL;
		tuto = NULL;
		netmatch = NULL;
		status = NULL;
		bulletprediction = NULL;
		vitalgauge = NULL;
		bulletgauge = NULL;
		damege = NULL;
		explosion = NULL;
		item = NULL;
		rank = NULL;
		result = NULL;
		title = NULL;
		nScene = SCENE_TITLE;
		stop = 0;
	}
	~GAME_OBJECT() {}

	virtual void Init();		//初期化(数値など)
	virtual void Update();		//更新
	virtual void Draw();		//描画

	void Generate();			//メモリ確保　データ読み込み　初期化
	void Delete();				//メモリ解放

	//scene変更関数
	inline void SetScene(int Scene){ nScene = Scene;}
	inline int GetScene(void){	return nScene;}

	//instance総数計測
	inline void CreateInstanceOBJ() { AllOBJCnt++; }
	inline void DeleteInstanceOBJ() { AllOBJCnt--; }


private:

	//処理の一時停止フラグ
	inline void SetStop(int Flag) { stop = Flag; }
	inline int GetStop(void) { return stop; }

	//当たり判定
	void CheakHit(int scene);

	PLAYER *player;
	EFFECT *effect;
	BULLET *bullet;
	SHADOW *shadow;
	COUNTDOWN *countdown;
	TUTO *tuto;
	NETMATCH *netmatch;
	STATUS *status;
	BULLETPREDICTION *bulletprediction;
	VITALGAUGE *vitalgauge;
	BULLETGAUGE *bulletgauge;
	DAMEGE *damege;
	EXPLOSION *explosion;
	ITEM *item;
	RANK *rank;
	RESULT *result;
	TITLE *title;
	FIELD *field;
	SKY *sky;
	WALL *wall;
	FADE *fade;

	int	nScene = SCENE_TITLE;		//!< ステージ番号
	bool stop = 0;					//!< デバッグ時の一時停止用変数 true=停止　false=停止しない
	static int AllOBJCnt;				//!< 派生クラスでコンストラクト・デストラクトが呼ばれるときに必ず計測する。CreateInstanceOBJ()・DeleteInstanceOBJ()を呼ぶ
};

/**
 * @struct VERTEX_2D
 * @brief 2Dポリゴン頂点フォーマット
 */
struct VERTEX_2D
{
	VERTEX_2D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f); rhw = 0.0f; diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
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
	VERTEX_3D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f); nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f); diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
	~VERTEX_3D() {}
	D3DXVECTOR3 vtx;		//!< 頂点座標
	D3DXVECTOR3 nor;		//!< 法線ベクトル
	D3DCOLOR	diffuse;	//!< 反射光
	D3DXVECTOR2 tex;		//!< テクスチャ座標
};

/**
 * @class Mtx
 * @brief 行列クラス
 */
class Mtx
{
public:
	Mtx() { D3DXMatrixIdentity(&mtxWorld); }
	~Mtx() {}
	//------------------------get関数
	inline D3DXMATRIX Matrix() const { return this->mtxWorld; };

	//------------------------set関数
	inline void Matrix(const D3DXMATRIX mtxWorld) { this->mtxWorld = mtxWorld; };

private:
	D3DXMATRIX					mtxWorld;			//!< ワールドマトリックス

};

/**
 * @class Move
 * @brief 移動クラス
 */
class Movement
{
public:
	Movement() { move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	~Movement() {}
	//------------------------get関数
	inline D3DXVECTOR3 Move() const { return this->move; };

	//------------------------set関数
	inline void Move(const D3DXVECTOR3 move) { this->move = move; };

private:
	D3DXVECTOR3					move;				//!< 移動量

};

/**
 * @class Color
 * @brief 色クラス
 */
class Color
{
public:
	Color() { col = D3DXCOLOR(DWORD(0)); }
	~Color() {}
	//------------------------get関数
	inline D3DXCOLOR Col() const { return this->col; };

	//------------------------set関数
	inline void Col(const D3DXCOLOR col) { this->col = col; };

private:
	D3DXCOLOR					col;				//!< 色

};

/**
 * @class bUseCheak
 * @brief 使用判定クラス　bool型
 */
class bUseCheak
{
public:
	bUseCheak() { use = false; }
	~bUseCheak() {}
	//------------------------get関数
	inline bool Use() const { return this->use; };

	//------------------------set関数
	inline void Use(const bool use) { this->use = use; };

private:
	bool		use;		//!< 使用判定

};

/**
 * @class iUseCheak
 * @brief 使用判定クラス　int型(0で未使用、それ以外で該当テクスチャ使用、UseType参照)
 */
class iUseCheak
{
public:
	iUseCheak() { use = -1; }
	~iUseCheak() {}
	//------------------------get関数
	inline int Use() const { return this->use; };

	//------------------------set関数
	inline void Use(const int usetype) { this->use = usetype; };

	//使用するタイプを変更する
	void ChangeUse(int NowUseType);
private:
	int		use;		//!< 使用判定

};

/**
 * @class FieldNor
 * @brief クォータニオンで使う地形法線や球面回転クラス
 */
class FieldNor
{
private:
	D3DXVECTOR3					FNVec;				//!< 自分のいる地形法線ベクトル
	D3DXVECTOR3					FNUNCross;			//!< 地形法線と自分のUpベクトルの外積値
	float						Qrot;				//!< Upベクトルから地形法線への角度

public:
	FieldNor() { FNVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f); FNUNCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f); Qrot = 0.0f; }
	~FieldNor() {}
	//------------------------get関数
	inline D3DXVECTOR3 FNVecFunc() const { return this->FNVec; };				//!< クォータニオンで使う地形の法線ベクトル
	inline D3DXVECTOR3 FNUNCrossFunc() const { return this->FNUNCross; };		//!< 地形法線とプレイヤーUpベクトルの外積値
	inline float QrotFunc() const { return this->Qrot; };						//!< Upベクトルから地形法線への回転角度

	//------------------------set関数
	inline void FNVecFunc(const D3DXVECTOR3 FieldNorVec) { this->FNVec = FieldNorVec; };						//!< クォータニオンで使う地形の法線ベクトル
	inline void FNUNCrossFunc(const D3DXVECTOR3 FieldNorUpNorCross) { this->FNUNCross = FieldNorUpNorCross; }  //!< 地形法線とプレイヤーUpベクトルの外積値
	inline void QrotFunc(const float Qrot) { this->Qrot = Qrot; };												//!< Upベクトルから地形法線への回転角度
};

/**
 * @class TranceForm
 * @brief 座標、回転、スケールクラス
 */
class TransForm
{
public:
	TransForm() {	
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); oldrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f); 
	}
	~TransForm() {}

	//------------------------get関数
	inline D3DXVECTOR3 Pos() const { return this->pos; };
	inline D3DXVECTOR3 OldPos() const { return this->oldpos; };
	inline D3DXVECTOR3 Rot() const { return this->rot; };
	inline D3DXVECTOR3 OldRot() const { return this->oldrot; };
	inline D3DXVECTOR3 Scl() const { return this->scl; };

	//------------------------set関数
	inline void Pos(const D3DXVECTOR3 pos) { this->oldpos = this->pos; this->pos = pos; };
	inline void Rot(const D3DXVECTOR3 rot) { this->oldrot = this->rot; this->rot = rot; };
	inline void Scl(const D3DXVECTOR3 scl) { this->scl = scl; };

private:
	D3DXVECTOR3		pos;				//!< 位置
	D3DXVECTOR3		oldpos;				//!< 前フレームの位置
	D3DXVECTOR3		rot;				//!< 向き(回転)
	D3DXVECTOR3		oldrot;				//!< 前フレームの向き(回転)
	D3DXVECTOR3		scl;				//!< 大きさ(スケール)

};

/**
*　@class TEXTURE
*　@brief テクスチャクラス
*/
class TEXTURE
{
public:
	TEXTURE() { pD3DTexture = NULL; }
	~TEXTURE() { SAFE_DELETE_ARRAY(pD3DTexture); }

	//------------------------読み込み関数
	void LoadTexture(const char *FileName);

	//------------------------get関数
	inline LPDIRECT3DTEXTURE9 Texture(void) const { return pD3DTexture; };
	//inline LPDIRECT3DTEXTURE9 Texture(int pHeadNum) const { return pD3DTexture[pHeadNum]; };

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;		//!< テクスチャへのポインタ
};

/**
*　@class VTXBuffer
*　@brief 頂点バッファクラス　インターフェース化するのもあり
*/
class VTXBuffer
{
public:
	VTXBuffer() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~VTXBuffer() { SAFE_RELEASE(pD3DVtxBuff); SAFE_RELEASE(pD3DIdxBuff); }

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
	void UV3D(const int Indx, const float PatternU, const float PatternV);
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
	//inline LPDIRECT3DVERTEXBUFFER9 *VtxBuff(int pHeadNum) { return &pD3DVtxBuff[pHeadNum]; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	//inline LPDIRECT3DINDEXBUFFER9 *IdxBuff(int pHeadNum) { return &pD3DIdxBuff[pHeadNum]; };

	//------------------------set関数
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

private:

	//バッファ群
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< インデックスバッファインターフェースへのポインタ 3Dモデル読み込み時に使用

};

/**
 * @class ModelAttribute
 * @brief 3Dモデルデータ BUFFER使用
 */
class ModelAttribute
{
public:
	ModelAttribute() { pD3DXBuffMat = NULL; nNumMat = 0;nNumVertex = 0; nNumVertexIndex = 0; nNumPolygon = 0;}
	~ModelAttribute() { SAFE_RELEASE(pD3DXBuffMat);}

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

