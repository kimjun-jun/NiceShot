/**
* @file field.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
* @date 2020/01/15
*/
#pragma once

#include "../../h/object/objectclass.h"

/**
 * @enum FIELD_TYPE
 * FIELD�̎��
 */
enum FIELD_TYPE
{
	FIELD_TYPE_BOKOBOKO,
	FIELD_TYPE_NADARAKA,
	FIELD_TYPE_PLAYERADVANTAGE,
};

/**
 * @enum FIELD_VTX_TYPE
 * FIELD���_�̎�ށ@�u�`��p�v�ό`���́u�J�n�v�Ɓu�I���v���_
 */
enum FIELD_VTX_TYPE
{
	FIELD_VTX_DRAW = 0,
	FIELD_VTX_START,
	FIELD_VTX_END,
	FIELD_VTX_MAX,
};

/**
*�@@class FIELD_PARAMETER
*�@@brief
*/
class FIELD_PARAMETER
{
public:
	FIELD_PARAMETER() {
		fBlockSizeXField = fBlockSizeZField = 0.0f; fSideSizeXMax = fSideSizeZMax = 0.0f; fSideSizeXHalf = fSideSizeZHalf = 0.0f;
		fSideSizeXQuarter = fSideSizeZQuarter = 0.0f; fSideSizeXEighth = fSideSizeZEighth = 0.0f;
		time = 0.0f; nNumBlockXField = nNumBlockZField = 0; InterPolationFieldType = 0;
		InterPolationFieldPlayerNum = 0; TikeiSeed = OldTikeiSeed = 0; ItemGetPlayerNum = -1;
		InterPolationFieldSignal = false; InterPolationFieldSignalEnd = false; NetTikei = false;
	}
	~FIELD_PARAMETER() { }

	float fBlockSizeXField, fBlockSizeZField;		//!< �u���b�N�T�C�Y
	float fSideSizeXMax, fSideSizeZMax;				//!< �ӃT�C�Y�ő� 1920
	float fSideSizeXHalf, fSideSizeZHalf;			//!< �ӃT�C�Y1/2   960
	float fSideSizeXQuarter, fSideSizeZQuarter;		//!< �ӃT�C�Y1/4   480
	float fSideSizeXEighth, fSideSizeZEighth;		//!< �ӃT�C�Y1/8   240
													  
	//�n�`�ό`�ϐ��Q								  
	float time;										//!< �n�`�̕ό`����
	int nNumBlockXField, nNumBlockZField;			//!< �u���b�N��
	int InterPolationFieldType;						//!< �n�`�ό`�^�C�v
	int InterPolationFieldPlayerNum;				//!< �n�`�ό`�A�C�e�����擾�����v���C���[�i���o�[
	int TikeiSeed;									//!< �n�`�ό`�����_���l��seed�l�@�ʐM�̎��ɓ���������
	int OldTikeiSeed;								//!< �n�`�ό`�����_���l��seed�l�@�ʐM�̎��ɓ���������
	int ItemGetPlayerNum;							//!< �n�`�A�C�e�����擾�����v���C���[�ԍ�
	bool InterPolationFieldSignal;					//!< �n�`�ό`�M��
	bool InterPolationFieldSignalEnd;				//!< �n�`�ό`�M���I���������ǂ���
	bool NetTikei;									//!< �ʐM�ΐ풆�ɒn�`�ύX�����Ă��邩�ǂ���

};


/**
*�@@class TITLE
*�@@brief GAMEOBJECT�h���N���X
*/
class FIELD : public GAME_OBJECT
{
public:
	FIELD();	//!< �f�[�^�ǂݍ��݁@������
	~FIELD();	//!< �폜

	void Init(void);	//!< ������
	void Update(PLAYER *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow);//!< �X�V
	void Draw(void);	//!< �`��

	void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	bool FieldHitItem(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	void SetFieldInterPolationFieldType(int type, int CntPlayer, ITEM *item);

	FIELD_PARAMETER		FieldPara;						//!< 

private:
	ModelAttribute		Attribute;						//!< ���f�����@�}�e���A���Ⓒ�_���Ȃǁ@�����g�p����Ȃ炱����z��
	TEXTURE				tex;							//!< �e�N�X�`�����@�����g�p����Ȃ炱����z�񉻁@ITEMTYPE_MAX
	VTXBuffer			vtx[FIELD_VTX_MAX];				//!< ���_���@�����g�p����Ȃ炱����z��
	TransForm			Transform;						//!< �g�����X�t�H�[�����


	LPDIRECT3DVERTEXBUFFER9 ClearField(LPDIRECT3DVERTEXBUFFER9 VtxBuff);		//!< ���_�ݒ菉����
	LPDIRECT3DVERTEXBUFFER9 SetFieldType01(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd);	//!< �ڂ��ڂ��n�`
	LPDIRECT3DVERTEXBUFFER9 SetFieldType02(LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd);							//!< �Ȃ��炩�n�`
	LPDIRECT3DVERTEXBUFFER9 SetFieldType03(PLAYER *player, LPDIRECT3DVERTEXBUFFER9 VtxBuffEnd, LPDIRECT3DINDEXBUFFER9 IdxBuff);			//!< �v���C���[�L�� 
	void FieldHitCheck(PLAYER *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow);//!< �n�`�Ƃ̓����蔻��@�����p�����v�Z���Ă���

	bool InterPolationField(void);						//!< �n�`�����X�ɕ�ԕω�������
	void SetDegenerationPoly(void);						//!< �k�ރ|���S���̍��W���Čv�Z����֐��@�n�`�ό`��A�k�ރ|���S���̍��W��ݒ肷��
};

