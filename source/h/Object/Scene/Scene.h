/**
* @file Scene.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

/**
 * @enum e_SCENE
 * 画面遷移定数
 */
enum e_SCENE//列挙型。defineの番号を自動で割り当ててくれる。
{
	SCENE_TITLE,					//!< 0。1つめだから0。列挙型の列挙定数名。定義した名前をシンボルっていうのが一般らしい。
	SCENE_TUTORIAL,					//!< 1チュートリアル
	SCENE_GAMECOUNTDOWN,			//!< 2カウントダウン
	SCENE_GAME,						//!< 3ゲーム
	SCENE_NETMATCH,					//!< 4ネットマッチ
	SCENE_NETGAMECOUNTDOWN,			//!< 5ネットカウントダウン
	SCENE_NETGAME,					//!< 6ネットゲーム
	SCENE_RESULT,					//!< 7リザルト
	SCENE_MAX
};

/**
 * @enum FADE_TYPE
 * フェードの状態
 */
enum eFADE_TYPE
{
	FADE_NONE = 0,		//!< 何もない状態
	FADE_IN,			//!< フェードイン処理
	FADE_MUSIC_STOP,	//!< 音楽ストップ
	FADE_OUT,			//!< フェードアウト処理
	FADE_MAX			//!< マックス
};

/**
*　@class SCENE_PARAMETER
*　@brief
*/
class SCENE_PARAMETER
{
public:
	SCENE_PARAMETER() {	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); eFade = FADE_IN; eNowScene = SCENE_TITLE; eNextScene = SCENE_TITLE; sno = 0; stop = false;}
	~SCENE_PARAMETER() { }

	D3DXCOLOR	color;							//!< カラー情報
	eFADE_TYPE	eFade;							//!< フェード番号
	int			sno;							//!< サウンドナンバー
	e_SCENE		eNowScene;							//!< 現在のScene
	e_SCENE		eNextScene;						//!< 次に飛ぶ予定のScene
	bool		stop;							//!< デバッグ時の一時停止用変数 true=停止　false=停止しない
};

/**
*　@class SCENE
*　@brief GAMEOBJECT派生クラス　シーン切り替え時のフェード効果　一定周期タイム後にシーンを切り替える
*/
class SCENE : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	SCENE();		//!< データ読み込み　初期化
	~SCENE();	//!< 削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override {};	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override;			//!< 描画
		 
	void NextScene(eFADE_TYPE eFade, e_SCENE next, int sno);		//!< シーン切り替え　シーン切り替えを受け取ったらフェードさせながらシーンを切り替え　基本的にシーン切り替えはこの関数
	void SetScene(e_SCENE next);									//!< シーンセット シーン切り替え時に使用　 例外的にフェード処理せずに切り替えることも可能(countdownで使用)

	SCENE_PARAMETER	ScenePara;			//!< パラメーターに必要なデータ群

private:
	VTXBUFFER	vtx;												//!< 頂点情報　複数使用するならここを配列化
};
