/**
* @file SockUpdate.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
*　@class SOCK_UPDATE
*　@brief ソケットクラスの更新管理
*/
class SOCK_UPDATE
{
public:
	SOCK_UPDATE() {};	//!< コンストラクタ　特に使用しない
	~SOCK_UPDATE() {};	//!< デストラクタ　　特に使用しない

	void UpdateFunc(MySOCKET *MySockObj);
private:
	//ネット対戦開始処理
	void NetMatch(MySOCKET *MySockObj);
	void NetMyNumberGet(MySOCKET *MySockObj);
	void NetItemGet(MySOCKET *MySockObj);
	void NetCountdown(MySOCKET *MySockObj);

};
