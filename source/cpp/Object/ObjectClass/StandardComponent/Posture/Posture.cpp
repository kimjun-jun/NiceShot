/**
* @file Posture.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/

#include "../../../../../h/main.h"
#include "../../../../../h/Object/ObjectClass/StandardComponent/Posture/Posture.h"


//=============================================================================
// クォータニオン制御
//=============================================================================
void POSTURE::Quaternion(void)
{
	//---------------------------------------------------------オブジェクト値呼び出し
	D3DXVECTOR3 POSTUREVec = this->FNVecFunc();
	D3DXVECTOR3 POSTUREUpNorCross = this->FNUNCrossFunc();
	float Qrot = this->QrotFunc();

	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//地形の角度とプレイヤーの角度を計算。drawでクオータニオンで使う
	D3DXVec3Cross(&POSTUREUpNorCross, &POSTUREVec, &UpVec);

	//地形法線とUpベクトルで内積をとる(軸に対する回転角度を求める)
	float Upkakezan = D3DXVec3Dot(&POSTUREVec, &UpVec);

	//もし回転角度が0以外(平行でない)ならば回転角度θを求める
	//θはクォータニオンの任意軸を回転させる時の回転角度になる
	if (Upkakezan != 0)
	{
		float cossita = Upkakezan /
			sqrtf(POSTUREVec.x*POSTUREVec.x +
				POSTUREVec.y *POSTUREVec.y +
				POSTUREVec.z * POSTUREVec.z);
		Qrot = acosf(cossita);
	}
	else Qrot = 0.0f;

	//---------------------------------------------------------オブジェクト値セット
	this->QrotFunc(Qrot);
	this->FNUNCrossFunc(POSTUREUpNorCross);

}

