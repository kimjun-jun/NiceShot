/**
* @file objectclass.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once


/**
 * @class GAMEOBJECT
 * �Q�[���I�u�W�F�N�g�W���֐��QCLASS���`�@(�S�I�u�W�F�N�g����)
 */
class GAME_OBJECT
{
public:
	GAME_OBJECT() { cnt++; };
	~GAME_OBJECT() { cnt--, player = NULL, effect = NULL, bullet = NULL; };
	virtual void Init() = 0;		//!< ������
	virtual void Reinit() = 0;		//!< �ď�����
	virtual void Uninit() = 0;		//!< �f���[�g
	virtual void Update() = 0;		//!< �X�V
	virtual void Draw() = 0;		//!< �`��
	int GetCnt() { return cnt; };	//!< �I�u�W�F�N�g�ԍ��擾

	PLAYER_HONTAI *GetPlayer() { return &player[0]; };	//!< �v���C���[�擪�A�h���X�擾
	void *SetPlayer(int cnt) { player[cnt]; };	//!< �I�u�W�F�N�g�ԍ��擾

	EFFECT *GetEffect() { return &effect[0]; };	//!< �v���C���[�擪�A�h���X�擾
	void *SetEffect(int cnt) { effect[cnt]; };	//!< �I�u�W�F�N�g�ԍ��擾

	BULLET *GetBullet() { return &bullet[0]; };	//!< �v���C���[�擪�A�h���X�擾
	void *SetBullet(int cnt) { bullet[cnt]; };	//!< �I�u�W�F�N�g�ԍ��擾

private:
	static int cnt;
	PLAYER_HONTAI *player = NULL;
	EFFECT *effect = NULL;
	BULLET *bullet = NULL;
};

/**
 * @struct VERTEX_2D
 * �|���S�����_�t�H�[�}�b�g�ɍ��킹��CLASS���`
 */
struct VERTEX_2D
{
	VERTEX_2D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rhw = 0.0f, diffuse = D3DCOLOR(0), tex = D3DXVECTOR2(0.0f, 0.0f); };
	D3DXVECTOR3 vtx;		//!< ���_���W
	float		rhw;		//!< �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
 * @struct VERTEX_3D
 * �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹��CLASS���`
 */
struct VERTEX_3D
{
	VERTEX_3D() { vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f), nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f), diffuse = D3DCOLOR(0), tex = D3DXVECTOR2(0.0f, 0.0f); };
	D3DXVECTOR3 vtx;		//!< ���_���W
	D3DXVECTOR3 nor;		//!< �@���x�N�g��
	D3DCOLOR	diffuse;	//!< ���ˌ�
	D3DXVECTOR2 tex;		//!< �e�N�X�`�����W
};

/**
 * @struct INTERPOLATION_DATA
 * ��ԗp�̃f�[�^CLASS���`
 */
struct INTERPOLATION_DATA
{
	INTERPOLATION_DATA() { pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scl = D3DXVECTOR3(0.0f, 0.0f, 0.0f), frame = 0.0f; };
	D3DXVECTOR3 pos;		//!< ���_���W
	D3DXVECTOR3 rot;		//!< ��]
	D3DXVECTOR3 scl;		//!< �g��k��
	float		frame;		//!< ���s�t���[���� ( dt = 1.0f/frame )
};

/**
 * @class ALL_OBJECT_PARAMETER
 * �L�����N�^�[�̕W���I�ȕϐ����܂Ƃ߂�CLASS
 */
struct ALL_OBJECT_PARAMETER
{
	ALL_OBJECT_PARAMETER() {
		D3DXMatrixIdentity(&mtxWorld),
			pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			move = D3DXVECTOR3(0.0f, 0.0f, 0.0f), FieldNorVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			FieldNorUpNorCross = D3DXVECTOR3(0.0f, 0.0f, 0.0f), Qrot = 0.0f, col = D3DXCOLOR(DWORD(0)), use = false;
	};

	D3DXMATRIX					mtxWorld;			//!< ���[���h�}�g���b�N�X
	D3DXVECTOR3					pos;				//!< �ʒu
	D3DXVECTOR3					oldpos;				//!< �O��̈ʒu
	D3DXVECTOR3					rot;				//!< ����(��])
	D3DXVECTOR3					scl;				//!< �傫��(�X�P�[��)
	D3DXVECTOR3					move;				//!< �ړ���
	D3DXVECTOR3					FieldNorVec;		//!< �����̎��Ɏg���n�`�̖@���x�N�g��
	D3DXVECTOR3					FieldNorUpNorCross;	//!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	float						Qrot;				//!< Up�x�N�g������n�`�@���ւ̉�]�p�x
	D3DXCOLOR					col;				//!< �F
	bool						use;				//!< �����Ă锻��
};

/**
 * @struct GPUMODEL
 * ���f���̒��_�f�[�^���g�p�@���[�t�B���O���s���Ɍ��f�[�^�ƕω��f�[�^�̒��_����ۑ�
 */
struct GPUMODEL
{
	GPUMODEL() {
		pD3DTexture = NULL, pD3DXMesh = NULL, pD3DXBuffMat = NULL, nNumMat = NULL, pD3DVtxBuff = NULL, pD3DIdxBuff = NULL, nNumVertex = NULL, nNumVertexIndex = NULL, nNumPolygon = NULL;
	};
	~GPUMODEL() {
		SAFE_RELEASE(pD3DTexture); SAFE_RELEASE(pD3DXMesh); SAFE_RELEASE(pD3DXBuffMat); SAFE_RELEASE(pD3DVtxBuff); SAFE_RELEASE(pD3DIdxBuff);
	};

	LPDIRECT3DTEXTURE9			pD3DTexture;			//!< �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH					pD3DXMesh;				//!< ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				pD3DXBuffMat;			//!< �}�e���A�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;			//!< ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		pD3DIdxBuff;			//!< �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	DWORD						nNumMat;				//!< �}�e���A�����̐�
	DWORD						nNumVertex;				//!< ���_�̐�
	DWORD						nNumVertexIndex;		//!< ���_�̃C���f�b�N�X
	DWORD						nNumPolygon;			//!< �|���S���̐�

};

/**
*�@@struct TEXTURE2DVERTEXBUFFER
*�@@brief 2D�|���S�����`����\���́@���_�o�b�t�@���g�p
*/
class TEXTURE2D_VERTEXBUFFER
{
public:
	TEXTURE2D_VERTEXBUFFER() { pD3DTexture = NULL, pD3DVtxBuff = NULL; };
	~TEXTURE2D_VERTEXBUFFER() { SAFE_RELEASE(pD3DTexture); SAFE_RELEASE(pD3DVtxBuff); };

	//------------------------get�֐�
	LPDIRECT3DTEXTURE9 GetpD3DTexture();
	LPDIRECT3DVERTEXBUFFER9 GetpD3DVtxBuff();

	//------------------------set�֐�
	void SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture);
	void SetpD3DVtxBuff(LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff);

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;				//!< �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;				//!< ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
};

/**
*�@@struct TEXTURE2D
*�@@brief 2D�|���S�����`����\���́@���_�o�b�t�@�����d�l
*/
class TEXTURE_2D
{
public:
	TEXTURE_2D() { pD3DTexture = NULL; };
	~TEXTURE_2D() { SAFE_RELEASE(pD3DTexture); };

	//------------------------get�֐�
	LPDIRECT3DTEXTURE9 GetpD3DTexture();
	VERTEX_2D GettextureVTX();

	//------------------------set�֐�
	void SetpD3DTexture(LPDIRECT3DTEXTURE9 pD3DTexture);
	void SettextureVTX(VERTEX_2D *textureVTX);

private:
	LPDIRECT3DTEXTURE9		pD3DTexture;							   //!< �e�N�X�`���ւ̃|�C���^ 
	VERTEX_2D				textureVTX[POLYGON_2D_VERTEX];					   //!< ���_���i�[���[�N
};


/**
 * @class OBJECT_2D_VERTEXBUFFER
 * 2D�I�u�W�F�N�gCLASS���`�@
 */
class OBJECT_2D_VERTEXBUFFER :public GAME_OBJECT
{
public:
	OBJECT_2D_VERTEXBUFFER() { Num++; };
	~OBJECT_2D_VERTEXBUFFER() { Num--; };
	virtual void Init() {};
	virtual void Reinit() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
	//------------------------get�֐�
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetOldPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetScl();
	D3DXVECTOR3 GetMove();
	D3DXCOLOR GetCol();
	bool GetUse();

	//------------------------set�֐�
	void SetPos(D3DXVECTOR3 pos);
	void SetOldPos(D3DXVECTOR3 oldpos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScl(D3DXVECTOR3 scl);
	void SetMove(D3DXVECTOR3 move);
	void SetCol(D3DXCOLOR col);
	void SetUse(bool use);
	TEXTURE2D_VERTEXBUFFER tex2DVB;

private:
	static int Num;
	ALL_OBJECT_PARAMETER para;
};

/**
 * @class OBJECT_2D
 * 2D�I�u�W�F�N�gCLASS���`�@
 */
class OBJECT_2D :public GAME_OBJECT
{
public:
	OBJECT_2D() { Num++; };
	~OBJECT_2D() { Num--; };
	virtual void Init() {};
	virtual void Reinit() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
	//------------------------get�֐�
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetOldPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetScl();
	D3DXVECTOR3 GetMove();
	D3DXCOLOR GetCol();
	bool GetUse();

	//------------------------set�֐�
	void SetPos(D3DXVECTOR3 pos);
	void SetOldPos(D3DXVECTOR3 oldpos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScl(D3DXVECTOR3 scl);
	void SetMove(D3DXVECTOR3 move);
	void SetCol(D3DXCOLOR col);
	void SetUse(bool use);
	TEXTURE_2D tex2D;

private:
	ALL_OBJECT_PARAMETER para;
	static int Num;
};

/**
 * @class OBJECT_3D
 * 3D�I�u�W�F�N�gCLASS���`�@�v���C���[�Ŏg�p
 */
class OBJECT_3D :public GAME_OBJECT
{
public:
	OBJECT_3D() { Num++; };
	~OBJECT_3D() { Num--; };
	virtual void Init() {};
	virtual void Reinit() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
	GPUMODEL model;

	//------------------------get�֐�
	D3DXMATRIX GetMatrix();
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetOldPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetScl();
	D3DXVECTOR3 GetMove();
	D3DXVECTOR3 GetFieldNorVec();//!< �����̎��Ɏg���n�`�̖@���x�N�g��
	D3DXVECTOR3 GetFieldNorUpNorCross();	//!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	float GetQrot();			//!< Up�x�N�g������n�`�@���ւ̉�]�p�x
	D3DXCOLOR GetCol();
	bool GetUse();
	//------------------------�擪�A�h���Xget
//	PLAYER_HONTAI* GetPlayer() { return PLAYER_HONTAI(this[0]); };

	//------------------------set�֐�
	void SetMatrix(D3DXMATRIX mtxWorld);
	void SetPos(D3DXVECTOR3 pos);
	void SetOldPos(D3DXVECTOR3 oldpos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScl(D3DXVECTOR3 scl);
	void SetMove(D3DXVECTOR3 move);
	void SetFieldNorVec(D3DXVECTOR3 FieldNorVec);//!< �����̎��Ɏg���n�`�̖@���x�N�g��
	void SetFieldNorUpNorCross(D3DXVECTOR3 FieldNorUpNorCross);  //!< �n�`�@���ƃv���C���[Up�x�N�g���̊O�ϒl
	void SetQrot(float Qrot);				   //!< Up�x�N�g������n�`�@���ւ̉�]�p�x
	void SetCol(D3DXCOLOR col);
	void SetUse(bool use);

private:
	static int Num;
	ALL_OBJECT_PARAMETER para;
};
