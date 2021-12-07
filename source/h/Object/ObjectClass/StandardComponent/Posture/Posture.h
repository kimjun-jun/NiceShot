/**
* @file Posture.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
 * @class POSTURE
 * @brief クォータニオンで使う地形法線や球面回転クラス
 */
class POSTURE
{
public:
	POSTURE() { FNVec = VEC3_ALL0; FNUNCross = VEC3_ALL0; Qrot = 0.0f; }
	~POSTURE() {}
	//------------------------get関数
	inline D3DXVECTOR3 FNVecFunc() const { return this->FNVec; };				//!< クォータニオンで使う地形の法線ベクトル
	inline D3DXVECTOR3 FNUNCrossFunc() const { return this->FNUNCross; };		//!< 地形法線とプレイヤーUpベクトルの外積値
	inline float QrotFunc() const { return this->Qrot; };						//!< Upベクトルから地形法線への回転角度

	//------------------------set関数
	inline void FNVecFunc(const D3DXVECTOR3 POSTUREVec) { this->FNVec = POSTUREVec; };						//!< クォータニオンで使う地形の法線ベクトル
	inline void FNUNCrossFunc(const D3DXVECTOR3 POSTUREUpNorCross) { this->FNUNCross = POSTUREUpNorCross; }  //!< 地形法線とプレイヤーUpベクトルの外積値
	inline void QrotFunc(const float Qrot) { this->Qrot = Qrot; };												//!< Upベクトルから地形法線への回転角度

	//------姿勢制御
	void Quaternion(void);		//!< クォータニオン制御　地形法線とUPベクトルと外積

private:
	D3DXVECTOR3					FNVec;				//!< 自分のいる地形法線ベクトル
	D3DXVECTOR3					FNUNCross;			//!< 地形法線と自分のUpベクトルの外積値
	float						Qrot;				//!< Upベクトルから地形法線への角度

};

