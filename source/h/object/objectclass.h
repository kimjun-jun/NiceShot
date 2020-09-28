/**
* @file objectclass.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once
class PLAYER_HONTAI;
class EFFECT;
class BULLET;
class SHADOW;
class COUNTDOWN;
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
	~GAME_OBJECT() {
		//SAFE_DELETE_ARRAY(player);
		//SAFE_DELETE_ARRAY(effect);
		//SAFE_DELETE_ARRAY(bullet);
		//SAFE_DELETE_ARRAY(shadow);
		//SAFE_DELETE_ARRAY(countdown);
		//SAFE_DELETE_ARRAY(tuto);
		//SAFE_DELETE_ARRAY(status);
		//SAFE_DELETE_ARRAY(bulletprediction);
		//SAFE_DELETE_ARRAY(vitalgauge);
		//SAFE_DELETE_ARRAY(bulletgauge);
		//SAFE_DELETE_ARRAY(damege);
		//SAFE_DELETE_ARRAY(explosion);
		//SAFE_DELETE_ARRAY(item);
		//SAFE_DELETE_ARRAY(rank);
		//SAFE_DELETE_ARRAY(result);
		//SAFE_DELETE_ARRAY(title);
		//SAFE_DELETE_ARRAY(field);
		//SAFE_DELETE_ARRAY(sky);
		//SAFE_DELETE_ARRAY(wall);
	}
	void Create();
	void Init();
	void Reinit();
	void Uninit();
	void Update();
	void Draw();
	void SetScene(int Scene)
	{
		nScene = Scene;
	}

	int GetScene(void)
	{
		return nScene;
	}

private:
	void CheakHit(int scene);
	PLAYER_HONTAI *player;
	EFFECT *effect;
	BULLET *bullet;
	SHADOW *shadow;
	COUNTDOWN *countdown;
	TUTO *tuto;
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
	int	stop = 0;					//!< デバッグ時の一時停止用変数
};

/**
 * @struct VERTEX_2D
 * ポリゴン頂点フォーマットに合わせたCLASSを定義
 */
struct VERTEX_2D
{
	VERTEX_2D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f); rhw = 0.0f; diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
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
	VERTEX_3D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f); nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f); diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
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
	INTERPOLATION_DATA() { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f); frame = 0.0f; }
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
		D3DXMatrixIdentity(&mtxWorld);
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		FieldNorUpNorCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f); Qrot = 0.0f; col = D3DXCOLOR(DWORD(0)); use = false;
	}

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
		pD3DTexture = NULL; pD3DXMesh = NULL; pD3DXBuffMat = NULL; nNumMat = 0; pD3DVtxBuff = NULL;
		pD3DIdxBuff = NULL; nNumVertex = 0; nNumVertexIndex = 0; nNumPolygon = 0;
	}
	virtual ~GPUMODEL() {
		SAFE_RELEASE(pD3DTexture); SAFE_RELEASE(pD3DXMesh); SAFE_RELEASE(pD3DXBuffMat); SAFE_RELEASE(pD3DVtxBuff); SAFE_RELEASE(pD3DIdxBuff);
	}

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
	TEXTURE2D_VERTEXBUFFER() { pD3DTexture = NULL; pD3DVtxBuff = NULL; }
	virtual ~TEXTURE2D_VERTEXBUFFER() { SAFE_RELEASE(pD3DTexture); SAFE_RELEASE(pD3DVtxBuff); }
	LPDIRECT3DTEXTURE9		pD3DTexture;				//!< テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;				//!< 頂点バッファインターフェースへのポインタ

private:
};

/**
*　@struct TEXTURE2D
*　@brief 2Dポリゴンを定義する構造体　頂点バッファを見仕様
*/
class TEXTURE_2D
{
public:
	TEXTURE_2D() { pD3DTexture = NULL; }
	virtual ~TEXTURE_2D() { SAFE_RELEASE(pD3DTexture); }
	LPDIRECT3DTEXTURE9		pD3DTexture;				//!< テクスチャへのポインタ
	VERTEX_2D				textureVTX[POLYGON_2D_VERTEX];					   //!< 頂点情報格納ワーク

	//------------------------get関数
	//VERTEX_2D* GettextureVTX();

	//------------------------set関数
	//void SettextureVTX(VERTEX_2D *textureVTX);

private:
};


/**
 * @class OBJECT_2D_VERTEXBUFFER
 * 2DオブジェクトCLASSを定義　
 */
class OBJECT_2D_VERTEXBUFFER
{
public:
	OBJECT_2D_VERTEXBUFFER() { Num++; }
	virtual ~OBJECT_2D_VERTEXBUFFER() { Num--; }
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
class OBJECT_2D
{
public:
	OBJECT_2D() { Num++; }
	virtual ~OBJECT_2D() { Num--; }
	virtual void Init() {}
	virtual void Reinit() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
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
class OBJECT_3D
{
public:
	OBJECT_3D() { Num++; }
	virtual ~OBJECT_3D() { Num--; }
	virtual void Init() {}
	virtual void Reinit() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
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

