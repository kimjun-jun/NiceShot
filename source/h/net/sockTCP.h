/**
* @file sockTCP.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

#define BUFFER_SIZE 500
#define BUFFER_SIZE_STRING 500					//!< 送信データバッファサイズ 送信受信共にサイズを合わせないと、メモリ読み取りエラーになる　メモリストリームで徐々にずれていく

enum
{
	SetEnumPos = 0,
	SetEnumHoudaiRot = 1,
	SetEnumHoutouRot = 2,
	SetEnumHousinRot = 3,
};


/**
*　@class MySOCKET
*　@brief GAMEOBJECT派生クラス
*/
class MySOCKET : public GAME_OBJECT
{
public:
	MySOCKET();	//!< ソケット設定
	~MySOCKET();	//!< ソケット削除

	void Init(void);	//!< 初期化

	//ネット対戦開始処理
	void NetMatch(void);
	void NetMyNumberGet(void);
	void NetItemGet(ITEM *item);
	void NetCountdown(void);

	//---------マルチスレッド環境で実行　アプリ起動時にマルチスレッドで永久ループ
	void Packet(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet,SHADOW *Shadow);
	//ネット対戦実行中処理
	void SendPacket(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet);	//送信処理
	void ReceivePacket(PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet, SHADOW *Shadow);		//受信処理
	void MsgAnalys(char* argRMsg, PLAYER *Player, ITEM *Item, FIELD *Field, BULLET *Bullet, SHADOW *Shadow);	//解析処理

	//同期処理
	void SetBuff(char* RMsgBlock, int Type, int PlayerNum);	//RMsgBlockはデータのブロック　TypeはPosやRotなどの種類

	//アイテム反映
	void NetSetItem(ITEM *Item, D3DXVECTOR3 buff, int index, int type);

	//地形反映
	void NetSetTikeiSeed(FIELD *Field, ITEM *Item, int Seed, int PlayerNum);
	
	//プレイヤー反映群
	void NetSetPos(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoudaiRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHoutouRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetHousinRot(PLAYER *Player, D3DXVECTOR3 buff, int PlayerNum);
	void NetSetVital(PLAYER *Player, int buff, int PlayerNum);
	void NetSetMorphing(PLAYER *Player, int PlayerNum);
	void NetSetBulletType1(PLAYER *Player, BULLET *Bullet, SHADOW *Shadow, D3DXVECTOR3 buffpos, D3DXVECTOR3 buffmove, int PlayerNum);
	void NetSetBulletType3(PLAYER *Player, BULLET *Bullet, SHADOW *Shadow, D3DXVECTOR3 buffpos, D3DXVECTOR3 *buffmove, int PlayerNum);

	void NetSetGameEnd(void);
	//---------マルチスレッド環境で実行　アプリ起動時にマルチスレッドで永久ループ


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

};




