/**
* @file TRANSFORM.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once

/**
 * @enum UseType
 * 使用タイプの種類
 */
enum eUse_Type
{
	NullUse = -1,
	NoUse = 0,
	YesUseType1 = 1,
	YesUseType2,
	YesUseType3,
	YesUseType4,
};

/**
 * @class bUseCheck
 * @brief 使用判定クラス　bool型
 */
class bUseCheck
{
public:
	bUseCheck() { use = false; }
	~bUseCheck() {}
	//------------------------get関数
	inline bool Use() const { return this->use; };

	//------------------------set関数
	inline void Use(const bool use) { this->use = use; };

private:
	bool		use;		//!< 使用判定

};

/**
 * @class iUseCheck
 * @brief 使用判定クラス　int型(0で未使用、それ以外で該当テクスチャ使用、UseType参照)
 */
class iUseCheck
{
public:
	iUseCheck() { use = NullUse; }
	~iUseCheck() {}
	//------------------------get関数
	inline eUse_Type Use() const { return this->use; };

	//------------------------set関数
	inline void Use(const eUse_Type UseType) { this->use = UseType; };

private:
	eUse_Type		use;		//!< 使用判定

};
