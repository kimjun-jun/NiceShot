/**
* @file objectclass.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

//�l�b�g�ΐ�̂Ƃ��Ɏg��
void SetNetMatchFlag(bool flag);
void SetNetMyNumberFlagFlag(bool flag);
void SetNetMyNumber(int num);
int GetNetMyNumber(void);
void SetNetItemFlag(bool flag);
bool GetNetItemFlag(void);
void SetNetGameStartFlag(bool flag);
void SetNetShareDateFlag(bool flag);
bool GetNetShareDateFlag(void);

class PLAYER;
class EFFECT;
class BULLET;
class SHADOW;
class COUNTDOWN;
class NETMATCH;
class TUTO;
class STATUS;
class BULLETPREDICTION;
class VITALGAUGE;
class BULLETGAUGE;
class DAMEGE;
class EXPLOSION;
class ITEM;
class RANK;
class RESULT;
class TITLE;
class FIELD;
class SKY;
class WALL;
class FADE;

/**
 * @class GAMEOBJECT
 * �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 */
class GAME_OBJECT
{
public:
	GAME_OBJECT() {
		player = NULL;
		effect = NULL;
		bullet = NULL;
		shadow = NULL;
		countdown = NULL;
		tuto = NULL;
		netmatch = NULL;
		status = NULL;
		bulletprediction = NULL;
		vitalgauge = NULL;
		bulletgauge = NULL;
		damege = NULL;
		explosion = NULL;
		item = NULL;
		rank = NULL;
		result = NULL;
		title = NULL;
		nScene = SCENE_TITLE;
		stop = 0;
	}
	~GAME_OBJECT() {}

	virtual void Init();		//������(���l�Ȃ�)
	virtual void Update();		//�X�V
	virtual void Draw();		//�`��

	void Generate();			//�������m�ہ@�f�[�^�ǂݍ��݁@������
	void Delete();				//���������

	//scene�ύX�֐�
	inline void SetScene(int Scene){ nScene = Scene;}
	inline int GetScene(void){	return nScene;}

	//instance�����v��
	inline void CreateInstanceOBJ() { AllOBJCnt++; }
	inline void DeleteInstanceOBJ() { AllOBJCnt--; }


private:

	//�����̈ꎞ��~�t���O
	inline void SetStop(int Flag) { stop = Flag; }
	inline int GetStop(void) { return stop; }

	//�����蔻��
	void CheakHit(int scene);

	PLAYER *player;
	EFFECT *effect;
	BULLET *bullet;
	SHADOW *shadow;
	COUNTDOWN *countdown;
	TUTO *tuto;
	NETMATCH *netmatch;
	STATUS *status;
	BULLETPREDICTION *bulletprediction;
	VITALGAUGE *vitalgauge;
	BULLETGAUGE *bulletgauge;
	DAMEGE *damege;
	EXPLOSION *explosion;
	ITEM *item;
	RANK *rank;
	RESULT *result;
	TITLE *title;
	FIELD *field;
	SKY *sky;
	WALL *wall;
	FADE *fade;

	int	nScene = SCENE_TITLE;		//!< �X�e�[�W�ԍ�
	bool stop = 0;					//!< �f�o�b�O���̈ꎞ��~�p�ϐ� true=��~�@false=��~���Ȃ�
	static int AllOBJCnt;				//!< �h���N���X�ŃR���X�g���N�g�E�f�X�g���N�g���Ă΂��Ƃ��ɕK���v������BCreateInstanceOBJ()�EDeleteInstanceOBJ()���Ă�
};

/**
 * @struct VERTEX_2D
 * @brief 2D�|���S�����_�t�H�[�}�b�g
 */
struct VERTEX_2D
{
	VERTEX_2D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f); rhw = 0.0f; diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
	~VERTEX_2D() {}
	D3DXVECTOR3 vtx;		//!< ���_���W
	float		rhw;		//!< �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
 * @struct VERTEX_3D
 * @brief 3D�|���S�����_�t�H�[�}�b�g
 */
struct VERTEX_3D
{
	VERTEX_3D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f); nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f); diffuse = D3DCOLOR(0); tex = D3DXVECTOR2(0.0f, 0.0f); }
	~VERTEX_3D() {}
	D3DXVECTOR3 vtx;		//!< ���_���W
	D3DXVECTOR3 nor;		//!< �@���x�N�g��
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
 * @class Mtx
 * @brief �s��N���X
 */
class Mtx
{
public:
	Mtx() { D3DXMatrixIdentity(&mtxWorld); }
	~Mtx() {}
	//------------------------get�֐�
	inline D3DXMATRIX Matrix() const { return this->mtxWorld; };

	//------------------------set�֐�
	inline void Matrix(const D3DXMATRIX mtxWorld) { this->mtxWorld = mtxWorld; };

private:
	D3DXMATRIX					mtxWorld;			//!< ���[���h�}�g���b�N�X

};

/**
 * @class Move
 * @brief �ړ��N���X
 */
class Movement
{
public:
	Movement() { move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	~Movement() {}
	//------------------------get�֐�
	inline D3DXVECTOR3 Move() const { return this->move; };

	//------------------------set�֐�
	inline void Move(const D3DXVECTOR3 move) { this->move = move; };

private:
	D3DXVECTOR3					move;				//!< �ړ���

};

/**
 * @class Color
 * @brief �F�N���X
 */
class Color
{
public:
	Color() { col = D3DXCOLOR(DWORD(0)); }
	~Color() {}
	//------------------------get�֐�
	inline D3DXCOLOR Col() const { return this->col; };

	//------------------------set�֐�
	inline void Col(const D3DXCOLOR col) { this->col = col; };

private:
	D3DXCOLOR					col;				//!< �F

};

/**
 * @class bUseCheak
 * @brief �g�p����N���X�@bool�^
 */
class bUseCheak
{
public:
	bUseCheak() { use = false; }
	~bUseCheak() {}
	//------------------------get�֐�
	inline bool Use() const { return this->use; };

	//------------------------set�֐�
	inline void Use(const bool use) { this->use = use; };

private:
	bool		use;		//!< �g�p����

};

/**
 * @class iUseCheak
 * @brief �g�p����N���X�@int�^(0�Ŗ��g�p�A����ȊO�ŊY���e�N�X�`���g�p�AUseType�Q��)
 */
class iUseCheak
{
public:
	iUseCheak() { use = -1; }
	~iUseCheak() {}
	//------------------------get�֐�
	inline int Use() const { return this->use; };

	//------------------------set�֐�
	inline void Use(const int usetype) { this->use = usetype; };

	//�g�p����^�C�v��ύX����
	void ChangeUse(int NowUseType);
private:
	int		use;		//!< �g�p����

};

/**
 * @class FieldNor
 * @brief �N�H�[�^�j�I���Ŏg���n�`�@���⋅�ʉ�]�N���X
 */
class FieldNor
{
private:
	D3DXVECTOR3					FNVec;				//!< �����̂���n�`�@���x�N�g��
	D3DXVECTOR3					FNUNCross;			//!< �n�`�@���Ǝ�����Up�x�N�g���̊O�ϒl
	float						Qrot;				//!< Up�x�N�g������n�`�@���ւ̊p�x

public:
	FieldNor() { FNVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f); FNUNCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f); Qrot = 0.0f; }
	~FieldNor() {}
	//------------------------get�֐�
	inline D3DXVECTOR3 FNVecFunc() const { return this->FNVec; };				//!< �N�H�[�^�j�I���Ŏg���n�`�̖@���x�N�g��
	inline D3DXVECTOR3 FNUNCrossFunc() const { return this->FNUNCross; };		//!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	inline float QrotFunc() const { return this->Qrot; };						//!< Up�x�N�g������n�`�@���ւ̉�]�p�x

	//------------------------set�֐�
	inline void FNVecFunc(const D3DXVECTOR3 FieldNorVec) { this->FNVec = FieldNorVec; };						//!< �N�H�[�^�j�I���Ŏg���n�`�̖@���x�N�g��
	inline void FNUNCrossFunc(const D3DXVECTOR3 FieldNorUpNorCross) { this->FNUNCross = FieldNorUpNorCross; }  //!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	inline void QrotFunc(const float Qrot) { this->Qrot = Qrot; };												//!< Up�x�N�g������n�`�@���ւ̉�]�p�x
};

/**
 * @class TranceForm
 * @brief ���W�A��]�A�X�P�[���N���X
 */
class TransForm
{
public:
	TransForm() {	
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); oldrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f); 
	}
	~TransForm() {}

	//------------------------get�֐�
	inline D3DXVECTOR3 Pos() const { return this->pos; };
	inline D3DXVECTOR3 OldPos() const { return this->oldpos; };
	inline D3DXVECTOR3 Rot() const { return this->rot; };
	inline D3DXVECTOR3 OldRot() const { return this->oldrot; };
	inline D3DXVECTOR3 Scl() const { return this->scl; };

	//------------------------set�֐�
	inline void Pos(const D3DXVECTOR3 pos) { this->oldpos = this->pos; this->pos = pos; };
	inline void Rot(const D3DXVECTOR3 rot) { this->oldrot = this->rot; this->rot = rot; };
	inline void Scl(const D3DXVECTOR3 scl) { this->scl = scl; };

private:
	D3DXVECTOR3		pos;				//!< �ʒu
	D3DXVECTOR3		oldpos;				//!< �O�t���[���̈ʒu
	D3DXVECTOR3		rot;				//!< ����(��])
	D3DXVECTOR3		oldrot;				//!< �O�t���[���̌���(��])
	D3DXVECTOR3		scl;				//!< �傫��(�X�P�[��)

};

/**
*�@@class TEXTURE
*�@@brief �e�N�X�`���N���X
*/
class TEXTURE
{
public:
	TEXTURE() { pD3DTexture = NULL; }
	~TEXTURE() { SAFE_DELETE_ARRAY(pD3DTexture); }

	//------------------------�ǂݍ��݊֐�
	void LoadTexture(const char *FileName);

	//------------------------get�֐�
	inline LPDIRECT3DTEXTURE9 Texture(void) const { return pD3DTexture; };
	//inline LPDIRECT3DTEXTURE9 Texture(int pHeadNum) const { return pD3DTexture[pHeadNum]; };

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;		//!< �e�N�X�`���ւ̃|�C���^
};

/**
*�@@class VTXBuffer
*�@@brief ���_�o�b�t�@�N���X�@�C���^�[�t�F�[�X������̂�����
*/
class VTXBuffer
{
public:
	VTXBuffer() { pD3DVtxBuff = NULL; pD3DIdxBuff = NULL; }
	~VTXBuffer() { SAFE_RELEASE(pD3DVtxBuff); SAFE_RELEASE(pD3DIdxBuff); }

	//------------------------�ݒ�֐�
	//���_[�쐬]���ݒ�(3D)
	void MakeVertex3D(const int VTXnum, const DWORD FVF);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3D(const int VTXnum, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//���_[�쐬]���ݒ�(3D�r���{�[�h)
	void MakeVertex3DBill(const int CntMax, const DWORD FVF);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex3DBill(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY,
		const D3DXVECTOR3 Nor[4], const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//���_[�쐬]���ݒ�(2D)
	void MakeVertex2D(const int CntMax, const DWORD FVF);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float HalfSizeX, const float HalfSizeY, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void MakeVertex2D(const int CntMax, const DWORD FVF, const float LHalfSizeX, const float RHalfSizeX,
		const float HHalfSizeY, const float LHalfSizeY, const D3DXCOLOR col, const float uStart, const float uEnd, const float vStart, const float vEnd);
	//�C���f�b�N�X[�쐬]
	void MakeIdxVertex(const int VTXIdxNum);


	//���_�T�C�Y�̐ݒ�
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex3D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex3D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex3D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY);
	void Vertex2D(const int Indx, const float LHalfSizeX, const float RHalfSizeX, const float HHalfSizeY, const float LHalfSizeY);
	void Vertex2D(const int Indx, const D3DXVECTOR3 VTX[4]);
	void Vertex2D(const int Indx, const float HalfSizeX, const float HalfSizeY, const D3DXVECTOR3 Pos);

	//���_�@���̐ݒ�
	void Nor3D(const int Indx);
	void Nor3D(const int Indx, const D3DXVECTOR3 Nor[4]);
	void RHW2D(const int Indx);

	//���_�J���[�̐ݒ�
	void Color3D(const int Indx);
	void Color3D(const int Indx, const D3DXCOLOR col);
	void Color2D(const int Indx);
	void Color2D(const int Indx, const D3DXCOLOR col);

	//UV�ݒ�
	void UV3D(const int Indx);
	void UV3D(const int Indx, const float PatternU, const float PatternV);
	void UV3D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV3D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);
	void UV2D(const int Indx);
	void UV2D(const int Indx, const float PatternU, const float PatternV);
	void UV2D(const int Indx, const float uStart, const float uEnd, const float vStart, const float vEnd);
	void UV2D(const int Indx, const float uStart1, const float uEnd1, const float vStart1, const float vEnd1
		, const float uStart2, const float uEnd2, const float vStart2, const float vEnd2);

	//------------------------get�֐�
	D3DXCOLOR GetColor3D(const int Indx);
	D3DXCOLOR GetColor2D(const int Indx);


	//------------------------�o�b�t�@�Q
	//------------------------get�֐�
	inline LPDIRECT3DVERTEXBUFFER9 VtxBuff(void) { return pD3DVtxBuff; };
	//inline LPDIRECT3DVERTEXBUFFER9 *VtxBuff(int pHeadNum) { return &pD3DVtxBuff[pHeadNum]; };
	inline LPDIRECT3DINDEXBUFFER9 IdxBuff(void) { return pD3DIdxBuff; };
	//inline LPDIRECT3DINDEXBUFFER9 *IdxBuff(int pHeadNum) { return &pD3DIdxBuff[pHeadNum]; };

	//------------------------set�֐�
	inline void VtxBuff(const LPDIRECT3DVERTEXBUFFER9 vtxbuff) { pD3DVtxBuff = vtxbuff; };
	inline void IdxBuff(const LPDIRECT3DINDEXBUFFER9 idxbuff) { pD3DIdxBuff = idxbuff; };

private:

	//�o�b�t�@�Q
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;		//!< ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;		//!< �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^ 3D���f���ǂݍ��ݎ��Ɏg�p

};

/**
 * @class ModelAttribute
 * @brief 3D���f���f�[�^ BUFFER�g�p
 */
class ModelAttribute
{
public:
	ModelAttribute() { pD3DXBuffMat = NULL; nNumMat = 0;nNumVertex = 0; nNumVertexIndex = 0; nNumPolygon = 0;}
	~ModelAttribute() { SAFE_RELEASE(pD3DXBuffMat);}

	//------------------------get�֐�
	inline LPD3DXBUFFER *Mat() { return &pD3DXBuffMat; };
	inline DWORD NumMat() const { return nNumMat; };
	inline DWORD NumVertex() const { return nNumVertex; };
	inline DWORD NumVertexIndex() const { return nNumVertexIndex; };
	inline DWORD NumPolygon() const { return nNumPolygon; };

	//------------------------set�֐�
	inline void Mat(const LPD3DXBUFFER buffmat) { pD3DXBuffMat = buffmat; };
	inline void NumMat(DWORD nummat) { nNumMat = nummat; };
	inline void NumVertex(DWORD numvtx) { nNumVertex = numvtx; };
	inline void NumVertexIndex(DWORD numidx) { nNumVertexIndex = numidx; };
	inline void NumPolygon(DWORD numpoly) { nNumPolygon = numpoly; };

private:
	LPD3DXBUFFER				pD3DXBuffMat;			//!< �}�e���A�����ւ̃|�C���^
	DWORD						nNumMat;				//!< �}�e���A�����̐�
	DWORD						nNumVertex;				//!< ���_�̐�
	DWORD						nNumVertexIndex;		//!< ���_�̃C���f�b�N�X
	DWORD						nNumPolygon;			//!< �|���S���̐�

};

