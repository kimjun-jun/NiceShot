/**
* @file SockSubThreadSend.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
*　@class SOCK_SUBTHREAD_SEND
*　@brief ソケットクラスのマルチスレッドsend管理クラス
*/
class SOCK_SUBTHREAD_SEND
{
public:
	SOCK_SUBTHREAD_SEND() {};	//!< コンストラクタ　特に使用しない
	~SOCK_SUBTHREAD_SEND() {};	//!< デストラクタ　　特に使用しない

	void SendFunc(MySOCKET *MySockObj);
private:
	//ネット対戦中実行処理
	void SendPacket(MySOCKET *MySockObj);						//送信処理
};
