/**
* @file Draw.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

//前方宣言
class GAME_OBJECT_INSTANCE;

/**
*　@class DRAW_MANAGER
*　@brief オブジェクトの描画管理クラス
*/
class DRAW_MANAGER
{
public:
	DRAW_MANAGER() {
		vp[0] = { DWORD(0),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f };
		vp[1] =	{DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(0),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f};
		vp[2] = { DWORD(0),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f };
		vp[3] = { DWORD(SCREEN_W / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 + SCREEN_SEPARATE_BUFF),DWORD(SCREEN_W / 2 - SCREEN_SEPARATE_BUFF),DWORD(SCREEN_H / 2 - SCREEN_SEPARATE_BUFF),0.0f,1.0f };
		VpMaster={ 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };
		DrawManagerNum = 0;};		//!< コンストラクタ
	~DRAW_MANAGER() {};		//!< デストラクタ　　特に使用しない

	void DrawFunc(GAME_OBJECT_INSTANCE *obj);
	inline int GetDrawManagerNum() { return DrawManagerNum; };
private:

	//四人分の画面分割
	D3DVIEWPORT9 vp[OBJECT_PLAYER_MAX];
	D3DVIEWPORT9 VpMaster;
	int DrawManagerNum;	//描画ループカウント　プレイヤーの数分カウントする　描画ループさせる時は必ずカウントアップと初期化必須

	//シーン毎描画処理
	void DrawSceneTitle(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneTutorial(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneCountDown(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneGame(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneResult(GAME_OBJECT_INSTANCE *obj);

	//ネット対戦時シーン毎描画処理
	void DrawSceneNetMatch(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneNetCountDown(GAME_OBJECT_INSTANCE *obj);
	void DrawSceneNetGame(GAME_OBJECT_INSTANCE *obj);

	//描画グループ分け
	void DrawMap(GAME_OBJECT_INSTANCE *obj);
	void Draw3D(GAME_OBJECT_INSTANCE *obj);
	void Draw2D(GAME_OBJECT_INSTANCE *obj);

};
