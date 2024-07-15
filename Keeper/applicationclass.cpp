////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Timer = 0;
	m_Fps = 0;
	m_ShaderManager = 0;
	m_ObjectShader = 0;
	m_TextureShader = 0;
	m_ImageShader = 0;
	m_Menu = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;


	// Create the input object.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	m_ObjectShader = new ObjectShader;
	if (!m_ObjectShader)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ObjectShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}


	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!m_TextureShader) {
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	m_ImageShader = new ImageShaderClass;
	if (!m_ImageShader)
	{
		return false;
	}

	m_ImageShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!m_ImageShader) {
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}


	m_Menu = new MenuClass;
	if (!m_Menu)
	{
		return false;
	}


	result = m_Menu->Initialize(m_Direct3D, hwnd, m_Input, m_ShaderManager, m_TextureShader, m_ImageShader, m_ObjectShader, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the MenuClass.", L"Error", MB_OK);
		return false;
	}


	return true;
}


void ApplicationClass::Shutdown()
{


	if (m_Menu)
	{
		m_Menu->Shutdown();
		delete m_Menu;
		m_Menu = 0;
	}


	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}


	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_ObjectShader)
	{
		m_ObjectShader->Shutdown();
		delete m_ObjectShader;
		m_ObjectShader = 0;
	}

	if (m_ImageShader)
	{
		m_ImageShader->Shutdown();
		delete m_ImageShader;
		m_ImageShader = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool ApplicationClass::Frame()
{
	bool result;


	// Update the system stats.
	m_Fps->Frame();
	m_Timer->Frame();

	// Do the input frame processing.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}
	
	
	result = m_Menu->Frame(m_Direct3D, m_Input, m_ShaderManager, m_Timer->GetTime(), m_Fps->GetFps());
	if (!result)
	{
		return false;
	}
	
	
	return result;
}