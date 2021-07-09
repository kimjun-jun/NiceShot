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

class FIELD;

/**
*�@@class TITLE
*�@@brief GAMEOBJECT�h���N���X
*/
class FIELD_PARAMETER
{
	FIELD_PARAMETER() {
		pD3DVtxBuffFieldDraw = NULL; pD3DVtxBuffFieldEnd = NULL; pD3DVtxBuffFieldStart = NULL;
		fBlockSizeXField = fBlockSizeZField = 0.0f; fSideSizeXMax = fSideSizeZMax = 0.0f; fSideSizeXHalf = fSideSizeZHalf = 0.0f;
		fSideSizeXQuarter = fSideSizeZQuarter = 0.0f; fSideSizeXEighth = fSideSizeZEighth = 0.0f;
		time = 0.0f; nNumBlockXField = nNumBlockZField = 0; InterPolationFieldType = 0;
		InterPolationFieldPlayerNum = 0; TikeiSeed = OldTikeiSeed = 0; GetPlayerNum = -1;
		InterPolationFieldSignal = false; InterPolationFieldSignalEnd = false; NetTikei = false;
	};
	~FIELD_PARAMETER() { SAFE_RELEASE(pD3DVtxBuffFieldDraw); SAFE_RELEASE(pD3DVtxBuffFieldEnd); SAFE_RELEASE(pD3DVtxBuffFieldStart); }

	//�t�B�[���h�����p
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffFieldDraw;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�`��p
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffFieldEnd;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^��ԃf�[�^�G���h�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffFieldStart;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^��ԃf�[�^�X�^�[�g�o�b�t�@
	float fBlockSizeXField, fBlockSizeZField;			// �u���b�N�T�C�Y
	float fSideSizeXMax, fSideSizeZMax;					// �ӃT�C�Y�ő� 1920
	float fSideSizeXHalf, fSideSizeZHalf;				// �ӃT�C�Y1/2   960
	float fSideSizeXQuarter, fSideSizeZQuarter;			// �ӃT�C�Y1/4   480
	float fSideSizeXEighth, fSideSizeZEighth;			// �ӃT�C�Y1/8   240

	//�n�`�ό`�ϐ��Q
	float time;											// �n�`�̕ό`����
	int nNumBlockXField, nNumBlockZField;				// �u���b�N��
	int InterPolationFieldType;							// �n�`�ό`�^�C�v
	int InterPolationFieldPlayerNum;					// �n�`�ό`�A�C�e�����擾�����v���C���[�i���o�[
	bool InterPolationFieldSignal;						// �n�`�ό`�M��
	bool InterPolationFieldSignalEnd;					// �n�`�ό`�M���I���������ǂ���
	FIELD *Next;										// ����g�p���邩��
	int TikeiSeed;									// �n�`�ό`�����_���l��seed�l�@�ʐM�̎��ɓ���������
	int OldTikeiSeed;								// �n�`�ό`�����_���l��seed�l�@�ʐM�̎��ɓ���������
	int GetPlayerNum;								// �n�`�A�C�e�����擾�����v���C���[�ԍ�
	bool NetTikei;									// �ʐM�ΐ풆�ɒn�`�ύX�����Ă��邩�ǂ���

};


/**
*�@@class TITLE
*�@@brief GAMEOBJECT�h���N���X
*/
class FIELD : public GAME_OBJECT
{
public:
	FIELD();	//!< �f�[�^�ǂݍ���
	~FIELD();	//!< �폜
	void Init(void);
	void Update(PLAYER_HONTAI *player, ITEM *item, BULLET *bullet, EXPLOSION *explosion, SHADOW *shadow);
	void Draw(void);

	void ClearField(void);				//������
	void SetFieldType01(void);			//�I���W�i��1 
	void SetFieldType02(void);			//�I���W�i��2 
	void SetFieldType03(PLAYER_HONTAI *player);			//�I���W�i��3 
	bool InterPolationField(void);		//�n�`�����X�ɕω�������
	void SetDegenerationPoly(void);		//�k�ރ|���S���̍��W���Čv�Z����֐��@�n�`�ό`��A�k�ރ|���S���̍��W��ݒ肷��
	void SetFieldInterPolationFieldType(int type, int CntPlayer,ITEM *item);
	void FieldHit(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	void FieldHitGetSphereVec(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);
	bool FieldHitItem(D3DXVECTOR3 InrayS, D3DXVECTOR3 InrayG, D3DXVECTOR3 *vtxNor, float *posY);


private:
	FIELD_PARAMETER FieldPara;

};

