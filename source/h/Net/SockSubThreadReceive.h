/**
* @file SockSubThreadReceive.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

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
*　@class SOCK_SUBTHREAD_RECEIVE
*　@brief ソケットクラスのマルチスレッドReceive管理クラス
*/
class SOCK_SUBTHREAD_RECEIVE
{
public:
	SOCK_SUBTHREAD_RECEIVE() {};	//!< コンストラクタ　特に使用しない
	~SOCK_SUBTHREAD_RECEIVE() {};	//!< デストラクタ　　特に使用しない

	void ReceiveFunc(MySOCKET *MySockObj);
private:
	void ReceivePacket(MySOCKET *MySockObj);	//受信処理
	void ObjectAnalys(MySOCKET *MySockObj,char* argRMsg);			//解析処理　上位処理　オブジェクト判別
	void VariableAnalys(MySOCKET *MySockObj, char* argVariableMsg, int PlayerNum, char* PlayerType);		//解析処理　中位処理　対応変数判別
	void XYZValue1Analys(MySOCKET *MySockObj,char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//解析処理　下位処理　数値データ判別　float *3
	void XYZValue2Analys(MySOCKET *MySockObj,char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//解析処理　下位処理　数値データ判別　float *3 *2
	void XYZValue4Analys(MySOCKET *MySockObj,char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);		//解析処理　下位処理　数値データ判別　float *3 *4
	void INTValueAnalys(MySOCKET *MySockObj, char* argValueMsg, int PlayerNum, char* PlayerType, e_VariableAnalysType VariableType);			//解析処理　下位処理　数値データ判別　int *1
};
