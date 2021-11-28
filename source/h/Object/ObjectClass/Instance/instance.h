/**
* @file instance.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../../h/Object/ObjectClass/Interface/interface.h"

//各クラスの前置宣言
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
class SCENE;
class MySOCKET;

class UPDATE_MANAGER;
class DRAW_MANAGER;

/**
 * @class GAME_OBJECT_INSTANCE
 * ゲームオブジェクト標準関数群CLASSを定義　(全オブジェクト共通)
 * 派生クラスでオーバライド必須(プレイヤーやアイテムなどの全オブジェクトが継承)
 * GAME_OBJECT_INTERFACE_SUMMRYの派生クラス兄弟(？)へのアクセッサー
 */
class GAME_OBJECT_INSTANCE :public GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	GAME_OBJECT_INSTANCE() {}
	virtual ~GAME_OBJECT_INSTANCE() {}

	virtual void Addressor(GAME_OBJECT_INSTANCE *obj) override;		//アドレッサー
	virtual void Init(void) override;			//初期化(数値など)
	virtual void InitNet(void) override;		//初期化ネット対戦用に変更が必要なとこで使用
	virtual void Update(void) override;			//更新
	virtual void Draw(void) override;			//描画

	//生成と破棄
	void Generate(void);			//メモリ確保　データ読み込み　初期化
	void Delete(void);				//メモリ解放

	//マルチスレッド処理
	void MySubThreadFunc(void);		//サブスレッド関数　アプリ終了信号を受けるまでループ処理

	PLAYER *GetPlayer(void) const { return Player; };
	EFFECT *GetEffect(void) const { return Effect; };
	BULLET *GetBullet(void) const { return Bullet; };
	SHADOW *GetShadow(void) const { return Shadow; };
	COUNTDOWN *GetCountdown(void) const { return CountDown; };
	TUTO *GetTuto(void) const { return Tuto; };
	NETMATCH *GetNetMatch(void) const { return Netmatch; };
	STATUS *GetStatus(void) const { return Status; };
	BULLETPREDICTION *GetBulletPrediction(void) const { return BulletPrediction; };
	VITALGAUGE *GetVitalGauge(void) const { return VitalGauge; };
	BULLETGAUGE *GetBulletGauge(void) const { return BulletGauge; };
	DAMEGE *GetDamege(void) const { return Damege; };
	EXPLOSION *GetExplosion(void) const { return Explosion; };
	ITEM *GetItem(void) const { return Item; };
	RANK *GetRank(void) const { return Rank; };
	RESULT *GetResult(void) const { return Result; };
	TITLE *GetTitle(void) const { return Title; };
	FIELD *GetField(void) const { return Field; };
	SKY *GetSky(void) const { return Sky; };
	WALL *GetWall(void) const { return Wall; };
	SCENE *GetScene(void) const { return Scene; };
	MySOCKET *GetMySocket(void) const { return MySocket; };

	DRAW_MANAGER *GetDrawManager(void) { return this->DrawManager; };

private:
	//オブジェクトへのポインター
	PLAYER *Player = NULL;
	EFFECT *Effect = NULL;
	BULLET *Bullet = NULL;
	SHADOW *Shadow = NULL;
	COUNTDOWN *CountDown = NULL;
	TUTO *Tuto = NULL;
	NETMATCH *Netmatch = NULL;
	STATUS *Status = NULL;
	BULLETPREDICTION *BulletPrediction = NULL;
	VITALGAUGE *VitalGauge = NULL;
	BULLETGAUGE *BulletGauge = NULL;
	DAMEGE *Damege = NULL;
	EXPLOSION *Explosion = NULL;
	ITEM *Item = NULL;
	RANK *Rank = NULL;
	RESULT *Result = NULL;
	TITLE *Title = NULL;
	FIELD *Field = NULL;
	SKY *Sky = NULL;
	WALL *Wall = NULL;
	SCENE *Scene = NULL;
	MySOCKET *MySocket = NULL;

	//マネージャへのポインター
	UPDATE_MANAGER *UpdateManager = NULL;
	DRAW_MANAGER *DrawManager = NULL;
};
