/**
* @file player.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

class OBJECT_3D;
class PLAYER_PRATS;
struct GPUMODEL;

/**
 * @enum ITEMTYPE
 * アイテム定数
 */
enum PARTSTYPE
{
	PARTSTYPE_HOUTOU = 0,		//!< 砲塔
	PARTSTYPE_HOUSIN,			//!< 砲身
};


class PLAYER_HONTAI : public OBJECT_3D
{
public:
	PLAYER_HONTAI()
	{
		FrontRotTOaxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f), Brot = 0.0f;
		speed = 0.0f;
		speedbuff = 0.0f;
		speedbufftime = 0.0f;
		MorphingTime = 0.0f;
		BackCameraItemTime = 0.0f;
		KiriItemTime = 0.0f;
		AmmoBornTime = 0.0f;
		time = 0.0f;
		MorphingSignal = 0;
		vital = 0;
		AmmoCnt = 0;
		ModelType = 0;
		shadowIdx = 0;
		Morphing = false;
		MorphingEnd = false;
		speedbuffsignal = false;
		BackCameraItemSignal = false;
		KiriSignal = false;
		dashFlag = false;
	}


	//PLAYER_HONTAI *GetPlayerPointer() { return &this[0]; }	//!< 先頭アドレス取得
	//void *SetPlayerPointer(int cnt) { this[cnt]; }			//!< アドレスセット



	PLAYER_PRATS				parts[2];					//!< モデルのパーツ階層　親本体(砲台)、子パーツ[0](砲塔)、孫パーツ[1](砲身)
	PLAYER_HONTAI				*Parent;					//!< 親子判定
	GPUMODEL					ModelDate[2];				//!< モデルデータ　0ノーマル、1攻撃、2スピード
	void						Init(void);					//!< 初期化
	void						Reinit(void);				//!< 再初期化
	void						Uninit(void);				//!< 終了
	void						Update(void);				//!< 更新
	void						Draw(void);					//!< 描画
	void						SetPlayerMeshColor(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff, LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff,DWORD nNumPolygon, int CntPlayer);
	void						SetMoveABL(int CntPlayer);				//!< 移動制御(ABボタンLスティックで移動制御)
	void						SetCamera(int CntPlayer);				//!< カメラ更新
	void						SetMoveL(int CntPlayer);				//!< 移動制御(LRスティックで移動制御)
	void						SetCameraR(int CntPlayer);				//!< カメラ制御(LRスティックで移動制御)
	void						SetMoveL2R2(int CntPlayer);				//!< 移動制御(L2R2で移動制御)
	void						SetQ(int CntPlayer);					//!< クォータニオン制御
	void						SetBulletALL(int CntPlayer);			//!< バレット関連制御
	void						SetBulletALLMoveL2R2Ver(int CntPlayer);	//!< バレット関連制御 発射ボタンを十字カーソル
	void						SetKiri(int CntPlayer);					//!< フォグ制御
	void						SetMorphing(int CntPlayer);				//!< モーフィング制御


	D3DXVECTOR3					FrontRotTOaxis;							//!< 地形法線とプレイヤーFrontベクトルの外積値
	float						Brot;									//!< Frontベクトルから地形法線への回転角度
	float						speed;									//!< 移動スピード
	float						speedbuff;								//!< 移動スピードアイテムバフ
	float						speedbufftime;							//!< 移動スピードアイテム効果時間
	float						MorphingTime;							//!< モーフィングできる時間
	float						BackCameraItemTime;						//!< バックカメラアイテムの効果時間
	float						KiriItemTime;							//!< 霧アイテムの効果時間
	float						AmmoBornTime;							//!< 残弾回復カウント。時間で回復
	float						time;									//!< モーフィング時に補間タイムとして使用
	int							MorphingSignal;							//!< 1,なにもしない　2,モーフィング中　3,モーフィング完了
	int							vital;									//!< 体力
	int							AmmoCnt;								//!< 残弾数。
	int							ModelType;								//!< どのモデル状態か判定
	int							shadowIdx;								//!< 影のインデックス番号
	bool						Morphing;								//!< 変形判定
	bool						MorphingEnd;							//!< 変形終了判定
	bool						speedbuffsignal;						//!< スピードアップバフ判定
	bool						BackCameraItemSignal;					//!< バックカメラアイテムを使用しているかどうか
	bool						KiriSignal;								//!< 霧アイテムを使用しているかどうか
	bool						dashFlag;								//!< ダッシュ判定　入力がある間true.それ以外はfalse

};

class PLAYER_PRATS : public OBJECT_3D
{
public:
	PLAYER_HONTAI				*ParentHontai;		// 親子判定
	PLAYER_PRATS				*ParentParts;		// 親子判定
};
