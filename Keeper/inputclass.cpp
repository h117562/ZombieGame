////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	m_mouseX = 0;
	m_mouseY = 0;

	m_pause = true;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;



	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;



	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	m_F1_released = true;
	m_F2_released = true;
	m_PD_released = true;

	return true;
}


void InputClass::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}


bool InputClass::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	if (!m_pause)
	{
		ProcessInput();
	}
	

	return true;
}


bool InputClass::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void InputClass::ProcessInput()
{

	m_mouseX += m_mouseState.lX *0.1;
	m_mouseY += m_mouseState.lY *0.1;


	return;
}


bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		if (m_Escape_released)
		{
			m_Escape_released = false;
			return true;
		}
	}
	else
	{
		m_Escape_released = true;
	}

	return false;
}


void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}


bool InputClass::IsLeftPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_A] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsRightPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_D] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsUpPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_W] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsDownPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_S] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsSpacePressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_SPACE] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsShiftPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_LSHIFT] & 0x80)
	{
		return true;
	}

	return false;
}



bool InputClass::Is_F_Pressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_F] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::Is_NUM1_Pressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_NUMPAD1] & 0x80)
	{
		if (m_N1_released)
		{
			m_N1_released = false;
			return true;
		}
	}
	else
	{
		m_N1_released = true;
	}

	return false;
}

bool InputClass::Is_NUM2_Pressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_NUMPAD2] & 0x80)
	{
		if (m_N2_released)
		{
			m_N2_released = false;
			return true;
		}
	}
	else
	{
		m_N2_released = true;
	}

	return false;
}

bool InputClass::Is_NUM3_Pressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_NUMPAD3] & 0x80)
	{
		if (m_N3_released)
		{
			m_N3_released = false;
			return true;
		}
	}
	else
	{
		m_N3_released = true;
	}

	return false;
}

bool InputClass::IsPgUpPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_PGUP] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsPgDownPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_PGDN] & 0x80)
	{
		if (m_PD_released)
		{
			m_PD_released = false;
			return true;
		}
	}
	else
	{
		m_PD_released = true;
	}

	return false;
}


bool InputClass::IsLeftMouseButtonDown()
{
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}



bool InputClass::IsRightMouseButtonDown()
{

	if (m_mouseState.rgbButtons[1] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftMouseButtonDownOnce()
{
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		if (m_M1_released)
		{
			m_M1_released = false;
			return true;
		}
	}
	else
	{
		m_M1_released = true;
	}

	return false;
}

bool InputClass::IsRightMouseButtonDownOnce()
{

	if (m_mouseState.rgbButtons[1] & 0x80)
	{
		if (m_M2_released)
		{
			m_M2_released = false;
			return true;
		}
	}
	else
	{
		m_M2_released = true;
	}

	return false;
}


bool InputClass::IsF1Toggled()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_F1] & 0x80)
	{
		if(m_F1_released)
		{
			m_F1_released = false;
			return true;
		}
	}
	else
	{
		m_F1_released = true;
	}

	return false;
}


bool InputClass::IsF2Toggled()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if(m_keyboardState[DIK_F2] & 0x80)
	{
		if(m_F2_released)
		{
			m_F2_released = false;
			return true;
		}
	}
	else
	{
		m_F2_released = true;
	}

	return false;
}

bool InputClass::IsF5Toggled()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (m_keyboardState[DIK_F5] & 0x80)
	{
		if (m_F5_released)
		{
			m_F5_released = false;
			return true;
		}
	}
	else
	{
		m_F5_released = true;
	}

	return false;
}

void InputClass::ToggleMove()
{
	m_pause = !m_pause;

	if (m_pause)
	{
		ShowCursor(true);
	}
	else
	{
		ShowCursor(false);
	}

}