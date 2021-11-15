/**
* @file input.h
* @brief NiceShot(3D)戦車ゲーム
* @author キムラジュン
*/
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************

// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる


/* game pad情報 */
//#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
//#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
//#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
//#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)
//#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
//#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
//#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
//#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
//#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
//#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
//#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
//#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
//#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
//#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)

//F310コントローラー環境
#define BUTTON_A							0x00000001l	// Ａボタン					(.rgbButtons[0] & 0x80)
#define BUTTON_B							0x00000002l	// Ｂボタン					(.rgbButtons[1] & 0x80)
#define BUTTON_X							0x00000004l	// Ⅹボタン					(.rgbButtons[2] & 0x80)
#define BUTTON_Y							0x00000008l	// Ｙボタン					(.rgbButtons[3] & 0x80)
#define BUTTON_L1							0x00000010l	// Ｌ１ボタン				(.rgbButtons[4] & 0x80)
#define BUTTON_R1							0x00000020l	// Ｒ１ボタン				(.rgbButtons[5] & 0x80)
#define BUTTON_SELECT						0x00000040l	// ＳＥＬＥＣＴボタン		(.rgbButtons[6] & 0x80)
#define BUTTON_START						0x00000080l	// ＳＴＡＲＴボタン			(.rgbButtons[7] & 0x80)
#define BUTTON_L3							0x00000100l	// Ｌ３ボタン				(.rgbButtons[8] & 0x80)
#define BUTTON_R3							0x00000200l	// Ｒ３ボタン				(.rgbButtons[9] & 0x80)
#define BUTTON_ANALOG_L_UP					0x00000400l	// 左アナログスティック上							(.IY>0)
#define BUTTON_ANALOG_L_RIGHT				0x00001000l	// 左アナログスティック右		(.IX>0)
#define BUTTON_ANALOG_L_DOWN				0x00002000l	// 左アナログスティック下							(.IY<0)
#define BUTTON_ANALOG_L_LEFT				0x00004000l	// 左アナログスティック左		(.IX<0)
#define BUTTON_ANALOG_R_UP					0x00008000l	// 右アナログスティック上							(.IRY>0)
#define BUTTON_ANALOG_R_RIGHT				0x00010000l	// 右アナログスティック右		(.IRX>0)
#define BUTTON_ANALOG_R_DOWN				0x00020000l	// 右アナログスティック下							(.IRY<0)
#define BUTTON_ANALOG_R_LEFT				0x00040000l	// 右アナログスティック左		(.IX<0)
#define BUTTON_DIGITAL_UP					0x00080000l	// 方向キー上				dijs.rgdwPOV[0] == 0
#define BUTTON_DIGITAL_RIGHTUP				0x00100000l	// 方向キー右上				dijs.rgdwPOV[0] == 4500
#define BUTTON_DIGITAL_RIGHT				0x00200000l	// 方向キー右				dijs.rgdwPOV[0] == 9000
#define BUTTON_DIGITAL_RIGHTDOWN			0x00400000l	// 方向キー右下				dijs.rgdwPOV[0] == 13500
#define BUTTON_DIGITAL_DOWN					0x00800000l	// 方向キー下				dijs.rgdwPOV[0] == 18000
#define BUTTON_DIGITAL_LEFTDOWN				0x01000000l	// 方向キー左下				dijs.rgdwPOV[0] == 22500
#define BUTTON_DIGITAL_LEFT					0x02000000l	// 方向キー左				dijs.rgdwPOV[0] == 27000
#define BUTTON_DIGITAL_LEFTUP				0x04000000l	// 方向キー左上				dijs.rgdwPOV[0] == 31500
#define BUTTON_L2							0x08000000l	// L2						(.IZ>32767)
#define BUTTON_R2							0x10000000l	// R2						(.IZ<32767)


#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット


//*****************************************************************************
// プロトタイプ宣言
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
BOOL IsMouseLeftPressed(void);      // 左クリックした状態

BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
BOOL IsMouseRightPressed(void);     // 右クリックした状態
BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
BOOL IsMouseCenterPressed(void);    // 中クリックした状態
BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
long GetMouseX(void);               // マウスがX方向に動いた相対値
long GetMouseY(void);               // マウスがY方向に動いた相対値
long GetMouseZ(void);               // マウスホイールが動いた相対値

//---------------------------- game pad
int GetPadData(int PlayerType, int no);
BOOL IsButtonPressed(int padNo, DWORD button);
DIJOYSTATE2 *GetIsButton(int PlayerType);
BOOL IsButtonTriggered(int padNo, DWORD button);


