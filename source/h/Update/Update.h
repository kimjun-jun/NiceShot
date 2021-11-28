/**
* @file Update.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
*　@class UPDATE_MANAGER
*　@brief オブジェクトの更新管理クラス
*/
class UPDATE_MANAGER
{
public:
	UPDATE_MANAGER() {};	//!< コンストラクタ　特に使用しない
	~UPDATE_MANAGER() {};	//!< デストラクタ　　特に使用しない

	void UpdateFunc(GAME_OBJECT_INSTANCE *obj);
private:
	//当たり判定処理
	void CollisionCheck(GAME_OBJECT_INSTANCE *obj);			//kurasuwakerubeki?

	//シーン毎描画処理
	void UpdateSceneTitle(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneTutorial(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneCountDown(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneGame(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneResult(GAME_OBJECT_INSTANCE *obj);

	//ネット対戦時シーン毎描画処理
	void UpdateSceneNetMatch(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneNetCountDown(GAME_OBJECT_INSTANCE *obj);
	void UpdateSceneNetGame(GAME_OBJECT_INSTANCE *obj);

	//描画グループ分け
	void UpdateMap(GAME_OBJECT_INSTANCE *obj);
	void Update3D(GAME_OBJECT_INSTANCE *obj);
	void Update2D(GAME_OBJECT_INSTANCE *obj);

};
