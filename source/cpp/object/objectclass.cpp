/**
* @file objectclass.cpp
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
* @date 2020/01/15
*/
using namespace std;

#include "../../h/main.h"
#include "../../h/object/objectclass.h"




//-------------------------------------------------------------------2DVBテクスチャゲット関数
LPDIRECT3DTEXTURE9 TEXTURE2D_VERTEXBUFFER::GetpD3DTexture() { return this->pD3DTexture; };
LPDIRECT3DVERTEXBUFFER9 TEXTURE2D_VERTEXBUFFER::GetpD3DVtxBuff() { return this->pD3DVtxBuff; };

//-------------------------------------------------------------------2DVBテクスチャセット関数
void TEXTURE2D_VERTEXBUFFER::SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture) { this->pD3DTexture = pD3DTexture; };
void TEXTURE2D_VERTEXBUFFER::SetpD3DVtxBuff(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff) { this->pD3DVtxBuff = pD3DVtxBuff; };


//-------------------------------------------------------------------2Dテクスチャゲット関数
LPDIRECT3DTEXTURE9 TEXTURE_2D::GetpD3DTexture() { return this->pD3DTexture; };
VERTEX_2D* TEXTURE_2D::GettextureVTX() { return &this->textureVTX[0]; };

//-------------------------------------------------------------------2Dテクスチャセット関数
void TEXTURE_2D::SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture) {this->pD3DTexture = pD3DTexture;};
void TEXTURE_2D::SettextureVTX(VERTEX_2D *textureVTX) { this->textureVTX[0] = textureVTX[0], 
this->textureVTX[1] = textureVTX[1], this->textureVTX[2] = textureVTX[2], this->textureVTX[3] = textureVTX[3]; };


//-------------------------------------------------------------------2DオブジェクトVBテクスチャゲット関数
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetPos() { return this->para.pos; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetOldPos() { return this->para.oldpos; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetRot() { return this->para.rot; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetScl() { return this->para.scl; }
D3DXVECTOR3 OBJECT_2D_VERTEXBUFFER::GetMove() { return this->para.move; }
D3DXCOLOR OBJECT_2D_VERTEXBUFFER::GetCol() { return this->para.col; }
bool OBJECT_2D_VERTEXBUFFER::GetUse() { return this->para.use; }

//-------------------------------------------------------------------2DオブジェクトVBテクスチャセット関数
void OBJECT_2D_VERTEXBUFFER::SetPos(D3DXVECTOR3 pos) { this->para.pos = pos; }
void OBJECT_2D_VERTEXBUFFER::SetOldPos(D3DXVECTOR3 oldpos) { this->para.oldpos = oldpos; }
void OBJECT_2D_VERTEXBUFFER::SetRot(D3DXVECTOR3 rot) { this->para.rot = rot; }
void OBJECT_2D_VERTEXBUFFER::SetScl(D3DXVECTOR3 scl) { this->para.scl = scl; }
void OBJECT_2D_VERTEXBUFFER::SetMove(D3DXVECTOR3 move) { this->para.move = move; }
void OBJECT_2D_VERTEXBUFFER::SetCol(D3DXCOLOR col) { this->para.col = col; }
void OBJECT_2D_VERTEXBUFFER::SetUse(bool use) { this->para.use=use; }



//-------------------------------------------------------------------2Dオブジェクトゲット関数
D3DXVECTOR3 OBJECT_2D::GetPos() { return this->para.pos; }
D3DXVECTOR3 OBJECT_2D::GetOldPos() { return this->para.oldpos; }
D3DXVECTOR3 OBJECT_2D::GetRot() { return this->para.rot; }
D3DXVECTOR3 OBJECT_2D::GetScl() { return this->para.scl; }
D3DXVECTOR3 OBJECT_2D::GetMove() { return this->para.move; }
D3DXCOLOR OBJECT_2D::GetCol() { return this->para.col; }
bool OBJECT_2D::GetUse() { return this->para.use; }

//-------------------------------------------------------------------2Dオブジェクトセット関数
void OBJECT_2D::SetPos(D3DXVECTOR3 pos) { this->para.pos = pos; }
void OBJECT_2D::SetOldPos(D3DXVECTOR3 oldpos) { this->para.oldpos = oldpos; }
void OBJECT_2D::SetRot(D3DXVECTOR3 rot) { this->para.rot = rot; }
void OBJECT_2D::SetScl(D3DXVECTOR3 scl) { this->para.scl = scl; }
void OBJECT_2D::SetMove(D3DXVECTOR3 move) { this->para.move = move; }
void OBJECT_2D::SetCol(D3DXCOLOR col) { this->para.col = col; }
void OBJECT_2D::SetUse(bool use) { this->para.use = use; }


//-------------------------------------------------------------------3Dオブジェクトゲット関数
D3DXMATRIX OBJECT_3D::GetMatrix() { return this->para.mtxWorld; }
D3DXVECTOR3 OBJECT_3D::GetPos() { return this->para.pos; }
D3DXVECTOR3 OBJECT_3D::GetOldPos() { return this->para.oldpos; }
D3DXVECTOR3 OBJECT_3D::GetRot() { return this->para.rot; }
D3DXVECTOR3 OBJECT_3D::GetScl() { return this->para.scl; }
D3DXVECTOR3 OBJECT_3D::GetMove() { return this->para.move; }
D3DXVECTOR3 OBJECT_3D::GetFieldNorVec() { return this->para.FieldNorVec; }
D3DXVECTOR3 OBJECT_3D::GetFieldNorUpNorCross() { return this->para.FieldNorUpNorCross; }
float OBJECT_3D::GetQrot() { return this->para.Qrot; }
D3DXCOLOR OBJECT_3D::GetCol() { return this->para.col; }
bool OBJECT_3D::GetUse() { return this->para.use; }

//-------------------------------------------------------------------3Dオブジェクトセット関数
void OBJECT_3D::SetMatrix(D3DXMATRIX mtxWorld) { this->para.mtxWorld = mtxWorld; }
void OBJECT_3D::SetPos(D3DXVECTOR3 pos) { this->para.pos = pos; }
void OBJECT_3D::SetOldPos(D3DXVECTOR3 oldpos) { this->para.oldpos = oldpos; }
void OBJECT_3D::SetRot(D3DXVECTOR3 rot) { this->para.rot = rot; }
void OBJECT_3D::SetScl(D3DXVECTOR3 scl) { this->para.scl = scl; }
void OBJECT_3D::SetMove(D3DXVECTOR3 move) { this->para.move = move; }
void OBJECT_3D::SetFieldNorVec(D3DXVECTOR3 FieldNorVec) { this->para.FieldNorVec = FieldNorVec; }
void OBJECT_3D::SetFieldNorUpNorCross(D3DXVECTOR3 FieldNorUpNorCross) { this->para.FieldNorUpNorCross = FieldNorUpNorCross; }
void OBJECT_3D::SetQrot(float Qrot) { this->para.Qrot = Qrot; }
void OBJECT_3D::SetCol(D3DXCOLOR col) { this->para.col = col; }
void OBJECT_3D::SetUse(bool use) { this->para.use = use; }





