/**
* @file objectclass.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once


/**
 * @class GAMEOBJECT
 * ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 */
class GAME_OBJECT
{
public:
	GAME_OBJECT() { cnt++; };
	~GAME_OBJECT() { cnt--, player = NULL, effect = NULL, bullet = NULL; };
	virtual void Init() = 0;		//!< 初期化
	virtual void Reinit() = 0;		//!< 再初期化
	virtual void Uninit() = 0;		//!< デリート
	virtual void Update() = 0;		//!< 更新
	virtual void Draw() = 0;		//!< 描画
	int GetCnt() { return cnt; };	//!< オブジェクト番号取得

	PLAYER_HONTAI *GetPlayer() { return &player[0]; };	//!< プレイヤー先頭アドレス取得
	void *SetPlayer(int cnt) { player[cnt]; };	//!< オブジェクト番号取得

	EFFECT *GetEffect() { return &effect[0]; };	//!< プレイヤー先頭アドレス取得
	void *SetEffect(int cnt) { effect[cnt]; };	//!< オブジェクト番号取得

	BULLET *GetBullet() { return &bullet[0]; };	//!< プレイヤー先頭アドレス取得
	void *SetBullet(int cnt) { bullet[cnt]; };	//!< オブジェクト番号取得

private:
	static int cnt;
	PLAYER_HONTAI *player = NULL;
	EFFECT *effect = NULL;
	BULLET *bullet = NULL;
};

/**
 * @struct VERTEX_2D
 * ポリゴン頂点フォーマットに合わせたCLASSを定義
 */
struct VERTEX_2D
{
	VERTEX_2D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rhw = 0.0f, diffuse = D3DCOLOR(0), tex = D3DXVECTOR2(0.0f, 0.0f); };
	D3DXVECTOR3 vtx;		//!< 頂点座標
	float		rhw;		//!< テクスチャのパースペクティブコレクト用
	D3DCOLOR	diffuse;	//!< 反射光
	D3DXVECTOR2 tex;		//!< テクスチャ座標
};

/**
 * @struct VERTEX_3D
 * ３Ｄポリゴン頂点フォーマットに合わせたCLASSを定義
 */
struct VERTEX_3D
{
	VERTEX_3D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f), nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f), diffuse = D3DCOLOR(0), tex = D3DXVECTOR2(0.0f, 0.0f); };
	D3DXVECTOR3 vtx;		//!< 頂点座標
	D3DXVECTOR3 nor;		//!< 法線ベクトル
	D3DCOLOR	diffuse;	//!< 反射光
	D3DXVECTOR2 tex;		//!< テクスチャ座標
};

/**
 * @struct INTERPOLATION_DATA
 * 補間用のデータCLASSを定義
 */
struct INTERPOLATION_DATA
{
	INTERPOLATION_DATA() { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f), frame = 0.0f; };
	D3DXVECTOR3 pos;		//!< 頂点座標
	D3DXVECTOR3 rot;		//!< 回転
	D3DXVECTOR3 scl;		//!< 拡大縮小
	float		frame;		//!< 実行フレーム数 ( dt = 1.0f/frame )
};

/**
 * @class ALL_OBJECT_PARAMETER
 * キャラクターの標準的な変数をまとめたCLASS
 */
struct ALL_OBJECT_PARAMETER
{
	ALL_OBJECT_PARAMETER() {
		D3DXMatrixIdentity(&mtxWorld),
			pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			move = D3DXVECTOR3(0.0f, 0.0f, 0.0f), FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			FieldNorUpNorCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f), Qrot = 0.0f, col = D3DXCOLOR(DWORD(0)), use = false;
	};

	D3DXMATRIX					mtxWorld;			//!< ワールドマトリックス
	D3DXVECTOR3					pos;				//!< 位置
	D3DXVECTOR3					oldpos;				//!< 前回の位置
	D3DXVECTOR3					rot;				//!< 向き(回転)
	D3DXVECTOR3					scl;				//!< 大きさ(スケール)
	D3DXVECTOR3					move;				//!< 移動量
	D3DXVECTOR3					FieldNorVec;		//!< くおの時に使う地形の法線ベクトル
	D3DXVECTOR3					FieldNorUpNorCross;	//!< 地形法線とプレイヤーUpベクトルの外積値
	float						Qrot;				//!< Upベクトルから地形法線への回転角度
	D3DXCOLOR					col;				//!< 色
	bool						use;				//!< 生きてる判定
};

/**
 * @struct GPUMODEL
 * モデルの頂点データを使用　モーフィング実行時に元データと変化データの頂点情報を保存
 */
struct GPUMODEL
{
	GPUMODEL() {
		pD3DTexture = NULL, pD3DXMesh = NULL, pD3DXBuffMat = NULL, nNumMat = NULL, pD3DVtxBuff = NULL, pD3DIdxBuff = NULL, nNumVertex = NULL, nNumVertexIndex = NULL, nNumPolygon = NULL;
	};
	~GPUMODEL() {
		SAFE_RELEASE(pD3DTexture); SAFE_RELEASE(pD3DXMesh); SAFE_RELEASE(pD3DXBuffMat); SAFE_RELEASE(pD3DVtxBuff); SAFE_RELEASE(pD3DIdxBuff);
	};

	LPDIRECT3DTEXTURE9			pD3DTexture;			//!< テクスチャへのポインタ
	LPD3DXMESH					pD3DXMesh;				//!< メッシュ情報へのポインタ
	LPD3DXBUFFER				pD3DXBuffMat;			//!< マテリアル情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;			//!< 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9		pD3DIdxBuff;			//!< インデックスバッファインターフェースへのポインタ
	DWORD						nNumMat;				//!< マテリアル情報の数
	DWORD						nNumVertex;				//!< 頂点の数
	DWORD						nNumVertexIndex;		//!< 頂点のインデックス
	DWORD						nNumPolygon;			//!< ポリゴンの数

};

/**
*　@struct TEXTURE2DVERTEXBUFFER
*　@brief 2Dポリゴンを定義する構造体　頂点バッファを使用
*/
class TEXTURE2D_VERTEXBUFFER
{
public:
	TEXTURE2D_VERTEXBUFFER() { pD3DTexture = NULL, pD3DVtxBuff = NULL; };
	~TEXTURE2D_VERTEXBUFFER() { SAFE_RELEASE(pD3DTexture); SAFE_RELEASE(pD3DVtxBuff); };

	//------------------------get関数
	LPDIRECT3DTEXTURE9 GetpD3DTexture();
	LPDIRECT3DVERTEXBUFFER9 GetpD3DVtxBuff();

	//------------------------set関数
	void SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture);
	void SetpD3DVtxBuff(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff);

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;				//!< テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;				//!< 頂点バッファインターフェースへのポインタ
};

/**
*　@struct TEXTURE2D
*　@brief 2Dポリゴンを定義する構造体　頂点バッファを見仕様
*/
class TEXTURE_2D
{
public:
	TEXTURE_2D() { pD3DTexture = NULL; };
	~TEXTURE_2D() { SAFE_RELEASE(pD3DTexture); };

	//------------------------get関数
	LPDIRECT3DTEXTURE9 GetpD3DTexture();
	VERTEX_2D GettextureVTX();

	//------------------------set関数
	void SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture);
	void SettextureVTX(VERTEX_2D *textureVTX);

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;							   //!< テクスチャへのポインタ 
	VERTEX_2D				textureVTX[POLYGON_2D_VERTEX];					   //!< 頂点情報格納ワーク
};


/**
 * @class OBJECT_2D_VERTEXBUFFER
 * 2DオブジェクトCLASSを定義　
 */
class OBJECT_2D_VERTEXBUFFER :public GAME_OBJECT
{
public:
	OBJECT_2D_VERTEXBUFFER() { Num++; };
	~OBJECT_2D_VERTEXBUFFER() { Num--; };
	virtual void Init() {};
	virtual void Reinit() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
	//------------------------get関数
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetOldPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetScl();
	D3DXVECTOR3 GetMove();
	D3DXCOLOR GetCol();
	bool GetUse();

	//------------------------set関数
	void SetPos(D3DXVECTOR3 pos);
	void SetOldPos(D3DXVECTOR3 oldpos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScl(D3DXVECTOR3 scl);
	void SetMove(D3DXVECTOR3 move);
	void SetCol(D3DXCOLOR col);
	void SetUse(bool use);
	TEXTURE2D_VERTEXBUFFER tex2DVB;

private:
	static int Num;
	ALL_OBJECT_PARAMETER para;
};

/**
 * @class OBJECT_2D
 * 2DオブジェクトCLASSを定義　
 */
class OBJECT_2D :public GAME_OBJECT
{
public:
	OBJECT_2D() { Num++; };
	~OBJECT_2D() { Num--; };
	virtual void Init() {};
	virtual void Reinit() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
	//------------------------get関数
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetOldPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetScl();
	D3DXVECTOR3 GetMove();
	D3DXCOLOR GetCol();
	bool GetUse();

	//------------------------set関数
	void SetPos(D3DXVECTOR3 pos);
	void SetOldPos(D3DXVECTOR3 oldpos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScl(D3DXVECTOR3 scl);
	void SetMove(D3DXVECTOR3 move);
	void SetCol(D3DXCOLOR col);
	void SetUse(bool use);
	TEXTURE_2D tex2D;

private:
	ALL_OBJECT_PARAMETER para;
	static int Num;
};

/**
 * @class OBJECT_3D
 * 3DオブジェクトCLASSを定義　プレイヤーで使用
 */
class OBJECT_3D :public GAME_OBJECT
{
public:
	OBJECT_3D() { Num++; };
	~OBJECT_3D() { Num--; };
	virtual void Init() {};
	virtual void Reinit() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
	GPUMODEL model;

	//------------------------get関数
	D3DXMATRIX GetMatrix();
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetOldPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetScl();
	D3DXVECTOR3 GetMove();
	D3DXVECTOR3 GetFieldNorVec();//!< くおの時に使う地形の法線ベクトル
	D3DXVECTOR3 GetFieldNorUpNorCross();	//!< 地形法線とプレイヤーUpベクトルの外積値
	float GetQrot();			//!< Upベクトルから地形法線への回転角度
	D3DXCOLOR GetCol();
	bool GetUse();
	//------------------------先頭アドレスget
//	PLAYER_HONTAI* GetPlayer() { return PLAYER_HONTAI(this[0]); };

	//------------------------set関数
	void SetMatrix(D3DXMATRIX mtxWorld);
	void SetPos(D3DXVECTOR3 pos);
	void SetOldPos(D3DXVECTOR3 oldpos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScl(D3DXVECTOR3 scl);
	void SetMove(D3DXVECTOR3 move);
	void SetFieldNorVec(D3DXVECTOR3 FieldNorVec);//!< くおの時に使う地形の法線ベクトル
	void SetFieldNorUpNorCross(D3DXVECTOR3 FieldNorUpNorCross);  //!< 地形法線とプレイヤーUpベクトルの外積値
	void SetQrot(float Qrot);				   //!< Upベクトルから地形法線への回転角度
	void SetCol(D3DXCOLOR col);
	void SetUse(bool use);

private:
	static int Num;
	ALL_OBJECT_PARAMETER para;
};

