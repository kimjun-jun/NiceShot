/**
* @file interface.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

//各クラスの前置宣言
class GAME_OBJECT_INSTANCE;

template <typename T>
inline void NewObject(T *p)
{
	p = new T;
}

/**
 * @class GAME_OBJECT_INTERFACE_INIT
 * @brief ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 * @details 派生クラスでオーバライド必須(プレイヤーやアイテムなどの全オブジェクトが継承)
 * @details 初期化処理インターフェース
 */
class GAME_OBJECT_INTERFACE_INIT
{
public:
	GAME_OBJECT_INTERFACE_INIT() {}
	virtual ~GAME_OBJECT_INTERFACE_INIT() {}
	virtual void Init(void) = 0;		//!< 初期化(数値など)
	virtual void InitNet(void) = 0;		//!< 初期化(ネット対戦時専用の初期化)

private:
};

/**
 * @class GAME_OBJECT_INTERFACE_UPDATE
 * @brief ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 * 派生クラスでオーバライド必須(プレイヤーやアイテムなどの全オブジェクトが継承)
 * 更新処理インターフェース
 */
class GAME_OBJECT_INTERFACE_UPDATE
{
public:
	GAME_OBJECT_INTERFACE_UPDATE() {}
	virtual ~GAME_OBJECT_INTERFACE_UPDATE() {}
	virtual void Update(void) = 0;		//!< 更新
private:
};

/**
 * @class GAME_OBJECT_INTERFACE_DRAW
 * @brief ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 * 派生クラスでオーバライド必須(プレイヤーやアイテムなどの全オブジェクトが継承)
 * 描画処理インターフェース
 * 描画ループの回数を取得する　画面4分割だから4ループする
 */
class GAME_OBJECT_INTERFACE_DRAW
{
public:
	GAME_OBJECT_INTERFACE_DRAW() {}
	virtual ~GAME_OBJECT_INTERFACE_DRAW() {}
	virtual void Draw(void) = 0; //!< 描画

private:
};

/**
 * @class GAME_OBJECT_INTERFACE_ADDRESSOR
 * @brief ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 * 派生クラスでオーバライド必須(プレイヤーやアイテムなどの全オブジェクトが継承)
 * 派生クラス間アドレス取得処理インターフェース
 */
class GAME_OBJECT_INTERFACE_ADDRESSOR
{
public:
	GAME_OBJECT_INTERFACE_ADDRESSOR() {}
	virtual ~GAME_OBJECT_INTERFACE_ADDRESSOR() {}
	virtual void Addressor(GAME_OBJECT_INSTANCE *obj) = 0;		//!< アドレス取得
private:
};

/**
 * @class GAME_OBJECT_INTERFACE_SUMMRY
 * @brief ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 * 派生クラスでオーバライド必須(プレイヤーやアイテムなどの全オブジェクトが継承)
 */
class GAME_OBJECT_INTERFACE_SUMMRY :protected GAME_OBJECT_INTERFACE_INIT, protected GAME_OBJECT_INTERFACE_UPDATE, protected GAME_OBJECT_INTERFACE_DRAW, protected GAME_OBJECT_INTERFACE_ADDRESSOR
{
public:
	GAME_OBJECT_INTERFACE_SUMMRY(void) { AllInheritanceCnt++; };
	~GAME_OBJECT_INTERFACE_SUMMRY(void) { AllInheritanceCnt--; };

	virtual void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< アドレッサー
	virtual void Init(void) override {};		//!< 初期化(数値など)
	virtual void InitNet(void) override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	virtual void Update(void) override {};		//!< 更新
	virtual void Draw(void) override {};		//!< 描画

private:
	static int AllInheritanceCnt;	//!< コンストラクト・デストラクトが呼ばれるときに必ず計測する
};

