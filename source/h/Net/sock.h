/**
* @file sock.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#include "../../../h/Object/ObjectClass/Interface/interface.h"
#include "../../../h/Object/ObjectClass/Instance/instance.h"
#include "../../../h/Object/ObjectClass/StandardComponent/Model/Model.h"
#include "../../../h/Object/ObjectClass/StandardComponent/TRANSFORM/TransForm.h"
#include "../../../h/Object/ObjectClass/StandardComponent/UseCheck/UseCheck.h"

#define BUFFER_SIZE 500
#define BUFFER_SIZE_STRING 500					//!< 送信データバッファサイズ 送信受信共にサイズを合わせないと、メモリ読み取りエラーになる　メモリストリームで徐々にずれていく

/**
 * @enum e_VariableAnalysType
 * 解析用定数　対応した解析処理を判別する
 */
enum e_VariableAnalysType
{
	VariableAnalysType_Pos,					//!< 0　座標
	VariableAnalysType_HoudaiRot,			//!< 1　砲台rot
	VariableAnalysType_HoutouRot,			//!< 2　砲塔rot
	VariableAnalysType_HousinRot,			//!< 3　砲身rot
	VariableAnalysType_Vital,				//!< 4　体力
	VariableAnalysType_Morphing,			//!< 5　変形
	VariableAnalysType_BulletA,				//!< 6　通常発射
	VariableAnalysType_BulletB,				//!< 7　拡散発射
	VariableAnalysType_MAX
};

/**
*　@class MySOCKET
*　@brief GAMEOBJECT派生クラス
*/
class MySOCKET : private GAME_OBJECT_INTERFACE_SUMMRY
{
public:
	MySOCKET();		//!< ソケット設定
	~MySOCKET();	//!< ソケット削除

	void Addressor(GAME_OBJECT_INSTANCE *obj) override;	//!< アドレッサー
	void Init(void) override;			//!< 初期化
	void InitNet(void)override {};		//!< 初期化ネット対戦用に変更が必要なとこで使用
	void Update(void)override;			//!< 更新
	void Draw(void)override {};			//!< 描画

	void MultThreadFlagFunc(bool flag) { this->MultThreadFlag = flag; }
	bool MultThreadFlagFunc(void) { return this->MultThreadFlag; }
	void GameSceneFlagFunc(bool flag) { this->GameSceneFlag = flag; }
	bool GameSceneFlagFunc(void) { return this->GameSceneFlag; }
					
	//ネット対戦開始処理
	void NetMatch(void);
	void NetMyNumberGet(void);
	void NetItemGet(void);
	void NetCountdown(void);

	//---------マルチスレッド環境で実行　アプリ起動時にマルチスレッドで永久ループ
	void Packet(void);

	//ネット対戦中実行処理
	void SendPacket(void);						//送信処理
	void ReceivePacket(void);					//受信処理



	void ObjectAnalys(char* argRMsg);			//解析処理　上位処理　オブジェクト判別
	void VariableAnalys(char* argVariableMsg, int PlayerNum, char* PlayerType);		//解析処理　中位処理　対応変数判別
	void XYZValue1Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//解析処理　下位処理　数値データ判別　float *3
	void XYZValue2Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//解析処理　下位処理　数値データ判別　float *3 *2
	void XYZValue4Analys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//解析処理　下位処理　数値データ判別　float *3 *4
	void INTValueAnalys(char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);			//解析処理　下位処理　数値データ判別　int *1

	//アイテム反映
	void NetSetItem(D3DXVECTOR3 buff, int index, int type);

	//地形反映
	void NetSetTikeiSeed(int Seed, int PlayerNum);
	
	//プレイヤー反映群
	void NetSetPos(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetVital(int buff, int PlayerNum);
	void NetSetMorphing(int PlayerNum);
	void NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum);
	void NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum);
	void NetSetGameEnd(void);



	//マッチフラグ
	void SetNetMatchFlag(bool flag) { NetMatchFlag = flag; }
	//マイナンバー
	void SetNetMyNumberFlag(bool flag) { NetMyNumberFlag = flag; }
	void SetNetMyNumber(int num) { NetMyNumber = num; }
	int GetNetMyNumber(void) { return NetMyNumber; }
	//アイテムフラグ　trueでアイテム情報取得完了、カウントダウン信号を受け取り始める
	void SetNetItemFlag(bool flag) { NetItemFlag = flag; }
	bool GetNetItemFlag(void) { return NetItemFlag; }
	//ネットフラグ　trueでネット対戦始める
	void SetNetGameStartFlag(bool flag) { NetGameStartFlag = flag; }
	bool GetNetGameStartFlag(void) { return NetGameStartFlag; }
	//ネット対戦　共有データアクセス許可フラグ
	void SetNetShareDateFlag(bool flag) { NetShareDateFlag = flag; }
	bool GetNetShareDateFlag(void) { return NetShareDateFlag; }

private:
	/* IP アドレス、ポート番号、ソケット */
	char* Destination;
	unsigned short Port;
	int DstSocket;
	/* sockaddr_in 構造体 */
	sockaddr_in DstAddr;
	//conect判定用
	bool ConnectFlag = false;
	//countdown判定用
	bool CountDownFlag = false;
	//マルチスレッド判定用
	bool MultThreadFlag = false;
	//ネット対戦中ループ　trueでネット対戦中　falseで対戦していない  これで同期するか判定している
	bool GameSceneFlag = false;

	//メンバ関数　ゲッターセッター
	void DestinationFunc(char* InIP) { Destination = InIP; }
	char* DestinationFunc(void) { return Destination; }
	void PortFunc(unsigned short InPort) { Port = InPort; }
	unsigned short PortFunc(void) { return Port; }
	void DstSocketFunc(int InDstSocket) { DstSocket = InDstSocket; }
	int DstSocketFunc(void) { return DstSocket; }
	void DstAddrFunc(sockaddr_in InDstAddr) { DstAddr = InDstAddr; }
	sockaddr_in DstAddrFunc(void) { return DstAddr; }
	sockaddr_in* pDstAddrFunc(void) { return &DstAddr; }


	//マッチフラグ
	bool NetMatchFlag = false;
	//マイナンバー
	bool NetMyNumberFlag = false;
	int NetMyNumber = -1;
	//アイテムフラグ　trueでアイテム情報取得完了、カウントダウン信号を受け取り始める
	bool NetItemFlag = false;
	//ネットフラグ　trueでネット対戦始める
	bool NetGameStartFlag = false;
	//ネット対戦　共有データアクセス許可フラグ
	bool NetShareDateFlag = false;

	//------他クラスのアドレス
	PLAYER *pplayer;
	ITEM *pitem;
	BULLET *pbullet;
	FIELD *pfield;
	SCENE *pscene;
	GAME_OBJECT_INSTANCE *pGameObjInstance; //親クラスのアドレスなのであんまり良くない

};




