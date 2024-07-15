////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsSpacePressed();
	bool IsShiftPressed();
	bool Is_F_Pressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();

	bool Is_NUM1_Pressed();
	bool Is_NUM2_Pressed();
	bool Is_NUM3_Pressed();

	bool IsLeftMouseButtonDown();
	bool IsRightMouseButtonDown();
	bool IsLeftMouseButtonDownOnce();
	bool IsRightMouseButtonDownOnce();

	bool IsF1Toggled();
	bool IsF2Toggled();
	bool IsF5Toggled();

	void ToggleMove();

	float m_mouseX, m_mouseY;

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	
	bool m_Escape_released;
	bool m_F1_released;
	bool m_F2_released;
	bool m_F5_released;
	bool m_PD_released;

	bool m_N1_released;
	bool m_N2_released;
	bool m_N3_released;

	bool m_M1_released;
	bool m_M2_released;

	bool m_pause;
};

#endif