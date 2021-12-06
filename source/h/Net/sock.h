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

class SOCK_UPDATE;
class SOCK_SUBTHREAD_RECEIVE;
class SOCK_SUBTHREAD_SEND;

#define BUFFER_SIZE 500
#define BUFFER_SIZE_STRING 500					//!< 送信データバッファサイズ 送信受信共にサイズを合わせないと、メモリ読み取りエラーになる　メモリストリームで徐々にずれていく

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

	//-----------重要度＊＊＊
	void Packet(void);	//マルチスレッド環境で実行　アプリ起動時にマルチスレッドで永久ループ
	int GetNetMyNumber(void) { return NetMyNumber; }	//マイナンバー取得
	void SetNetMyNumber(int num) { NetMyNumber = num; }
	//--------------メンバ　ゲッターセッター
	//マルチスレッドフラグ　アプリループフラグ
	void MultThreadFlagFunc(bool flag) { this->MultThreadFlag = flag; }
	bool MultThreadFlagFunc(void) { return this->MultThreadFlag; }
	void GameSceneFlagFunc(bool flag) { this->GameSceneFlag = flag; }
	bool GameSceneFlagFunc(void) { return this->GameSceneFlag; }
	//ネット対戦　共有データアクセス許可フラグ
	void SetNetShareDateFlag(bool flag) { NetShareDateFlag = flag; }
	bool GetNetShareDateFlag(void) { return NetShareDateFlag; }

	//-----------重要度＊＊
	//--------------メンバ　ゲッターセッター
	//ソケット情報
	void DestinationFunc(char* InIP) { Destination = InIP; }
	char* DestinationFunc(void) { return Destination; }
	void PortFunc(unsigned short InPort) { Port = InPort; }
	unsigned short PortFunc(void) { return Port; }
	void DstSocketFunc(int InDstSocket) { DstSocket = InDstSocket; }
	int DstSocketFunc(void) { return DstSocket; }
	void DstAddrFunc(sockaddr_in InDstAddr) { DstAddr = InDstAddr; }
	sockaddr_in DstAddrFunc(void) { return DstAddr; }
	sockaddr_in* pDstAddrFunc(void) { return &DstAddr; }					
	//コネクト判定用フラグ　tcp coccect処理が必要
	void SetNetConnectFlag(bool flag) { NetConnectFlag = flag; }
	bool GetNetConnectFlag(void) { return NetConnectFlag; }
	//マッチフラグ
	void SetNetMatchFlag(bool flag) { NetMatchFlag = flag; }
	bool GetNetMatchFlag(void) { return NetMatchFlag; }
	//マイナンバー
	void SetNetMyNumberFlag(bool flag) { NetMyNumberFlag = flag; }
	bool GetNetMyNumberFlag(void) { return NetMyNumberFlag; }
	//アイテムフラグ　trueでアイテム情報取得完了、カウントダウン信号を受け取り始める
	void SetNetItemFlag(bool flag) { NetItemFlag = flag; }
	bool GetNetItemFlag(void) { return NetItemFlag; }
	//カウントダウンフラグ　trueでネット対戦カウントダウン始める
	void SetNetCountDownFlag(bool flag) { NetCountDownFlag = flag; }
	bool GetNetCountDownFlag(void) { return NetCountDownFlag; }
	//対戦開始フラグ　trueでネット対戦開始
	void SetNetGameStartFlag(bool flag) { NetGameStartFlag = flag; }
	bool GetNetGameStartFlag(void) { return NetGameStartFlag; }


	//--------------メンバ　同期オブジェクトのセッター	
	void NetSetItem(D3DXVECTOR3 buff, int index, int type);		//アイテム反映
	void NetSetTikeiSeed(int Seed, int PlayerNum);				//地形反映	
	void NetSetPos(D3DXVECTOR3 buff, int PlayerNum);			//プレイヤー反映群
	void NetSetHoudaiRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoutouRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHousinRot(D3DXVECTOR3 buff, int PlayerNum);
	void NetSetVital(int buff, int PlayerNum);
	void NetSetMorphing(int PlayerNum);
	void NetSetBulletType1(D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum);
	void NetSetBulletType3(D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum);
	void NetSetGameEnd(void);	//ネット対戦終了フラグ反映

	//-----------重要度＊＊＊＊＊＊＊＊＊＊＊＊　好きになれない
	//------マネージャへのポインター
	SOCK_UPDATE	*pSockUpdate = NULL;
	SOCK_SUBTHREAD_SEND *pSockSubThreadSend = NULL;
	SOCK_SUBTHREAD_RECEIVE *pSockSubThreadReceive = NULL;
	//------他クラスのアドレス
	PLAYER *pplayer;
	ITEM *pitem;
	BULLET *pbullet;
	FIELD *pfield;
	SCENE *pscene;
	GAME_OBJECT_INSTANCE *pGameObjInstance; //親クラスのアドレスなのであんまり良くない

private:
	/* IP アドレス、ポート番号、ソケット */
	char* Destination;
	unsigned short Port;
	int DstSocket;
	/* sockaddr_in 構造体 */
	sockaddr_in DstAddr;

	//-----------重要度＊＊＊
	bool MultThreadFlag = false;	//マルチスレッド判定用　オンでネット対戦 オフでネット対戦していない
	bool GameSceneFlag = false;		//ネット対戦中ループ　オンでアプリ実行中　オフでアプリ終了
	bool NetShareDateFlag = false;	//ネット対戦　共有データアクセス許可フラグ
	int NetMyNumber = -1;			//ネット対戦時のマイプレイヤーナンバー	

	//-----------重要度＊
	bool NetConnectFlag = false;		//conect判定用
	bool NetMatchFlag = false;		//マッチフラグ
	bool NetMyNumberFlag = false;	//マイナンバー
	bool NetItemFlag = false;		//アイテムフラグ　trueでアイテム情報取得完了、カウントダウン信号を受け取り始める
	bool NetCountDownFlag = false;	//countdown判定用 trueでネットカウントダウン開始
	bool NetGameStartFlag = false;	//ネットフラグ　trueでネット対戦開始

};




