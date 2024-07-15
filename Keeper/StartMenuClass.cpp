#include "StartMenuClass.h"

MenuClass::MenuClass()
{
	m_StartButton = 0;
	m_QuitButton = 0;

	m_StartLighted = 0;
	m_QuitLighted = 0;

	m_Camera = 0;
	m_Font1 = 0;

	m_Start = 0;
	m_Out = 0;

	m_directory = "";
	m_LogoImage = 0;
	boxtouch = false;
	boxtouch2 = false;
	skip = false;
	logo = true;
	color = 1.0f;
	t_counter = 0.0f;
	diffusecolor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}


MenuClass::~MenuClass()
{
}


bool MenuClass::Initialize(D3DClass* Direct3D, HWND hwnd, InputClass* Input, ShaderManagerClass* ShaderManager, TextureShaderClass* textureShader, 
	ImageShaderClass* ImageShader, ObjectShader* objectshader, int screenWidth, int screenHeight)
{
	bool result;

	this->ScreenW = screenWidth;
	this->ScreenH = screenHeight;
	this->m_D3D = Direct3D;
	this->m_hwnd = hwnd;
	this->m_ShaderManager = ShaderManager;
	this->m_ImageShader = ImageShader;
	this->m_TextureShader = textureShader;
	this->m_ObjectShader = objectshader;

	char thisPath[100];
	_getcwd(thisPath, 100);
	m_directory = thisPath;

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render(false);
	m_Camera->RenderBaseViewMatrix();

	m_Font1 = new FontClass;
	if (!m_Font1)
	{
		return false;
	}

	// Initialize the first font object.
	result = m_Font1->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "../data/font/font01.txt",
		"../data/font/font01.tga", 32.0f, 3);
	if (!result)
	{
		return false;
	}

	m_Start = new TextClass;
	if (!m_Start)
	{
		return false;
	}

	result = m_Start->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 10, false, m_Font1,
		"GameStart", screenWidth / 2 - 45, screenHeight / 2, 0.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_Out = new TextClass;
	if (!m_Out)
	{
		return false;
	}

	result = m_Out->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 10, false, m_Font1,
		"Out", screenWidth / 2 - 15, (screenHeight / 2) + 140, 1.0f, 1.0f, 0.2f);
	if (!result)
	{
		return false;
	}

	m_LogoImage = new SetImageClass;
	if (!m_LogoImage)
	{
		return false;
	}

	result = m_LogoImage->Initialize(Direct3D->GetDevice(), hwnd, m_directory, "data\\Model\\Logo.png", XMFLOAT3(0.0f, 0.0f, 0.0f), 4.0f);
	if (!result)
	{
		return false;
	}

	m_StartButton = new ButtonClass;
	if (!m_StartButton)
	{
		return false;
	}

	result = m_StartButton->Initialize(Direct3D->GetDevice(), 0, 0, 4.0f, 1.0f, XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
	{
		return false;
	}

	m_StartLighted = new ButtonClass;
	if (!m_StartLighted)
	{
		return false;
	}

	result = m_StartLighted->Initialize(Direct3D->GetDevice(), 0, 0, 4.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
	{
		return false;
	}
	
	m_QuitButton = new ButtonClass;
	if (!m_QuitButton)
	{
		return false;
	}

	result = m_QuitButton->Initialize(Direct3D->GetDevice(), 0, -1.5, 4.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f));
	if (!result)
	{
		return false;
	}


	m_QuitLighted = new ButtonClass;
	if (!m_QuitLighted)
	{
		return false;
	}

	result = m_QuitLighted->Initialize(Direct3D->GetDevice(), 0, -1.5, 4.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
	{
		return false;
	}
	
	
	return true;
}


void MenuClass::Shutdown()
{

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_LogoImage)
	{
		m_LogoImage->Close();
		delete m_LogoImage;
		m_LogoImage = 0;
	}

	if (m_StartButton)
	{
		m_StartButton->Shutdown();
		delete m_StartButton;
		m_StartButton = 0;
	}

	if (m_QuitButton)
	{
		m_QuitButton->Shutdown();
		delete m_QuitButton;
		m_QuitButton = 0;
	}

	if (m_StartLighted)
	{
		m_StartLighted->Shutdown();
		delete m_StartLighted;
		m_StartLighted = 0;
	}

	if (m_QuitLighted)
	{
		m_QuitLighted->Shutdown();
		delete m_QuitLighted;
		m_QuitLighted = 0;
	}

	if (m_Start)
	{
		m_Start->Shutdown();
		delete m_Start;
		m_Start = 0;
	}

	if (m_Out)
	{
		m_Out->Shutdown();
		delete m_Out;
		m_Out = 0;
	}

	if (m_Font1)
	{
		m_Font1->Shutdown();
		delete m_Font1;
		m_Font1 = 0;
	}


	return;
}


bool MenuClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, float frameTime, int fps)
{
	bool result;

	if (logo)
	{
		RenderLogo(Direct3D);
		t_counter += 1.0f * frameTime;


		if (t_counter > 3.0f)
		{
			color -= 2.0f * frameTime;

			if (color < 0.0f)
			{
				color = 0;
			}

			if (t_counter > 5.0f)
			{
				logo = false;
			}
		}

		
	}
	else
	{

		if (!skip)
		{
			result = HandleMovementInput(Input, frameTime, fps);
			if (!result)
			{
				return false;
			}

			ButtonEvent();

			result = Render(Direct3D, ShaderManager);
			if (!result)
			{
				return false;
			}

		}
		else
		{
			if (!m_GameClass->GameReset())
			{
				result = m_GameClass->Frame(m_D3D, Input, m_ShaderManager, m_TextureShader, m_ImageShader, m_ObjectShader, frameTime, fps);
				if (!result)
				{
					return false;
				}
			}
			else
			{
				skip = false;
				GameClass* m_del = 0;
				m_del = m_GameClass;
				m_GameClass = new GameClass;

				m_del->Shutdown();
				delete m_del;
				m_del = 0;
			}
		}

	}
	

	return true;
}


bool MenuClass::HandleMovementInput(InputClass* Input, float frameTime, int fps)
{
	bool result;
	bool keyDown;
	bool keyDown2;

	keyDown = Input->IsLeftMouseButtonDown();
	if (keyDown && boxtouch)
	{
		m_GameClass = new GameClass;
		m_GameClass->Initialize(m_D3D, m_hwnd, ScreenW, ScreenH);
		Input->ToggleMove();
		skip = true;
	}

	keyDown2 = Input->IsLeftMouseButtonDown();
	if (keyDown2 && boxtouch2)
	{
		return false;
	}

	return true;
}


bool MenuClass::ButtonEvent() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX inverseView, inverseWorld;
	XMVECTOR direction, origin, rayOrigin, rayDirection;
	float pointX, pointY;

	GetCursorPos(&pp);
	ScreenToClient(m_hwnd, &pp);

	pointX = ((2.0f * (float)pp.x) / (float)ScreenW) - 1.0f;
	pointY = ((-2.0f * (float)pp.y) / (float)ScreenH) + 1.0f;

	m_D3D->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix.r[0].m128_f32[0];
	pointY = pointY / projectionMatrix.r[1].m128_f32[1];

	m_Camera->GetBaseViewMatrix(viewMatrix);
	inverseView = XMMatrixInverse(NULL, viewMatrix);

	direction.m128_f32[0] = (pointX * inverseView.r[0].m128_f32[0]) + (pointY * inverseView.r[1].m128_f32[0]) + inverseView.r[2].m128_f32[0];
	direction.m128_f32[1] = (pointX * inverseView.r[0].m128_f32[1]) + (pointY * inverseView.r[1].m128_f32[1]) + inverseView.r[2].m128_f32[1];
	direction.m128_f32[2] = (pointX * inverseView.r[0].m128_f32[2]) + (pointY * inverseView.r[1].m128_f32[2]) + inverseView.r[2].m128_f32[2];

	origin.m128_f32[0] = 0.0f;
	origin.m128_f32[1] = 0.0f;
	origin.m128_f32[2] = -10.0f;

	m_D3D->GetWorldMatrix(worldMatrix);

	inverseWorld = XMMatrixInverse(NULL, worldMatrix);

	rayOrigin = XMVector3TransformCoord(origin, inverseWorld);
	rayDirection = XMVector3TransformNormal(direction, inverseWorld);
	rayDirection = XMVector3Normalize(rayDirection);



	boxtouch = false;
	boxtouch = m_StartButton->TouchEvent(rayOrigin, rayDirection);

	boxtouch2 = false;
	boxtouch2 = m_QuitButton->TouchEvent(rayOrigin, rayDirection);
	


	return true;
}

bool MenuClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;


	// Generate the view matrix based on the camera's position.
	m_Camera->Render(false);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);


	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	Direct3D->TurnZBufferOff();
	Direct3D->EnableAlphaToCoverageBlending();

	m_Start->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix, m_Font1->GetTexture());
	m_Out->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix, m_Font1->GetTexture());

	result = m_StartButton->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = m_QuitButton->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	
	if (boxtouch)
	{
		result = m_StartLighted->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	if (boxtouch2)
	{
		result = m_QuitLighted->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}
	

	Direct3D->DisableAlphaBlending();
	Direct3D->TurnZBufferOn();


	Direct3D->EndScene();

	return true;
}

bool MenuClass::RenderLogo(D3DClass* Direct3D)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;
	
	diffusecolor = XMFLOAT4(color, color, color, 1.0f);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render(false);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	Direct3D->TurnZBufferOff();

	m_LogoImage->Render(Direct3D->GetDeviceContext());
	m_ImageShader->Render(Direct3D->GetDeviceContext(), m_LogoImage->GetPosMatrix(), baseViewMatrix, orthoMatrix, diffusecolor);
	

	Direct3D->TurnZBufferOn();

	Direct3D->EndScene();

	return true;
}

bool MenuClass::GetSkip()
{
	return skip;
}

void MenuClass::EntryGame()
{

}