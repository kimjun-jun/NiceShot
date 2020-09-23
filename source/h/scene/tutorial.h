/**
* @file tutorial.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

/**
*　@class TUTO
*　@brief 2Dポリゴンを定義する構造体
*/
class TUTO : public OBJECT_2D
{
public:
	TUTO() { cnt = 0; };
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	HRESULT						MakeVertexTutorial(void);
	int							cnt;						//!< 描画判定用　2で割った余り判定　0表示　1非表示　今後のテクスチャ情報が増えることを加味してint定義
};

