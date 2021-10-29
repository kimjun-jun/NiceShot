/**
* @file input.h
* @brief NiceShot(3D)��ԃQ�[��
* @author �L�����W����
*/
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD										// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE											// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD												// �錾����ƃp�b�h�ő���\�ɂȂ�


/* game pad��� */
//#define BUTTON_UP		0x00000001l	// �����L�[��(.IY<0)
//#define BUTTON_DOWN		0x00000002l	// �����L�[��(.IY>0)
//#define BUTTON_LEFT		0x00000004l	// �����L�[��(.IX<0)
//#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.IX>0)
//#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
//#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
//#define BUTTON_C		0x00000040l	// �b�{�^��(.rgbButtons[2]&0x80)
//#define BUTTON_X		0x00000080l	// �w�{�^��(.rgbButtons[3]&0x80)
//#define BUTTON_Y		0x00000100l	// �x�{�^��(.rgbButtons[4]&0x80)
//#define BUTTON_Z		0x00000200l	// �y�{�^��(.rgbButtons[5]&0x80)
//#define BUTTON_L		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
//#define BUTTON_R		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
//#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
//#define BUTTON_M		0x00002000l	// �l�{�^��(.rgbButtons[9]&0x80)

//F310�R���g���[���[��
#define BUTTON_A							0x00000001l	// �`�{�^��					(.rgbButtons[0] & 0x80)
#define BUTTON_B							0x00000002l	// �a�{�^��					(.rgbButtons[1] & 0x80)
#define BUTTON_X							0x00000004l	// �]�{�^��					(.rgbButtons[2] & 0x80)
#define BUTTON_Y							0x00000008l	// �x�{�^��					(.rgbButtons[3] & 0x80)
#define BUTTON_L1							0x00000010l	// �k�P�{�^��				(.rgbButtons[4] & 0x80)
#define BUTTON_R1							0x00000020l	// �q�P�{�^��				(.rgbButtons[5] & 0x80)
#define BUTTON_SELECT						0x00000040l	// �r�d�k�d�b�s�{�^��		(.rgbButtons[6] & 0x80)
#define BUTTON_START						0x00000080l	// �r�s�`�q�s�{�^��			(.rgbButtons[7] & 0x80)
#define BUTTON_L3							0x00000100l	// �k�R�{�^��				(.rgbButtons[8] & 0x80)
#define BUTTON_R3							0x00000200l	// �q�R�{�^��				(.rgbButtons[9] & 0x80)
#define BUTTON_ANALOG_L_UP					0x00000400l	// ���A�i���O�X�e�B�b�N��							(.IY>0)
#define BUTTON_ANALOG_L_RIGHT				0x00001000l	// ���A�i���O�X�e�B�b�N�E		(.IX>0)
#define BUTTON_ANALOG_L_DOWN				0x00002000l	// ���A�i���O�X�e�B�b�N��							(.IY<0)
#define BUTTON_ANALOG_L_LEFT				0x00004000l	// ���A�i���O�X�e�B�b�N��		(.IX<0)
#define BUTTON_ANALOG_R_UP					0x00008000l	// �E�A�i���O�X�e�B�b�N��							(.IRY>0)
#define BUTTON_ANALOG_R_RIGHT				0x00010000l	// �E�A�i���O�X�e�B�b�N�E		(.IRX>0)
#define BUTTON_ANALOG_R_DOWN				0x00020000l	// �E�A�i���O�X�e�B�b�N��							(.IRY<0)
#define BUTTON_ANALOG_R_LEFT				0x00040000l	// �E�A�i���O�X�e�B�b�N��		(.IX<0)
#define BUTTON_DIGITAL_UP					0x00080000l	// �����L�[��				dijs.rgdwPOV[0] == 0
#define BUTTON_DIGITAL_RIGHTUP				0x00100000l	// �����L�[�E��				dijs.rgdwPOV[0] == 4500
#define BUTTON_DIGITAL_RIGHT				0x00200000l	// �����L�[�E				dijs.rgdwPOV[0] == 9000
#define BUTTON_DIGITAL_RIGHTDOWN			0x00400000l	// �����L�[�E��				dijs.rgdwPOV[0] == 13500
#define BUTTON_DIGITAL_DOWN					0x00800000l	// �����L�[��				dijs.rgdwPOV[0] == 18000
#define BUTTON_DIGITAL_LEFTDOWN				0x01000000l	// �����L�[����				dijs.rgdwPOV[0] == 22500
#define BUTTON_DIGITAL_LEFT					0x02000000l	// �����L�[��				dijs.rgdwPOV[0] == 27000
#define BUTTON_DIGITAL_LEFTUP				0x04000000l	// �����L�[����				dijs.rgdwPOV[0] == 31500
#define BUTTON_L2							0x08000000l	// L2						(.IZ>32767)
#define BUTTON_R2							0x10000000l	// R2						(.IZ<32767)


#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mouse
BOOL IsMouseLeftPressed(void);      // ���N���b�N�������

BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
long GetMouseX(void);               // �}�E�X��X�����ɓ��������Βl
long GetMouseY(void);               // �}�E�X��Y�����ɓ��������Βl
long GetMouseZ(void);               // �}�E�X�z�C�[�������������Βl

//---------------------------- game pad
int GetPadData(int PlayerType, int no);
BOOL IsButtonPressed(int padNo, DWORD button);
DIJOYSTATE2 *GetIsButton(int PlayerType);
BOOL IsButtonTriggered(int padNo, DWORD button);


