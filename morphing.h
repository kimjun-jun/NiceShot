/**
* @file morphing.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
#pragma once

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DoMorphing(GPUMODEL *FromModel, GPUMODEL *ToModel);
void ResetMorphing(GPUMODEL *FromModel, GPUMODEL *ToModel);
