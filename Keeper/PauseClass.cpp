#include "PauseClass.h"

PauseClass::PauseClass()
{
	m_D3DClass = 0;
	diffuse_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_ContinueButton = 0;
	m_QuitButton = 0;
	m_ContinueLighted = 0;
	m_QuitLighted = 0;

	m_Camera = 0;


	m_ContinueImage = 0;
	m_QuitImage = 0;
	m_ContinueImage_S = 0; //선택된 경우 사용하는 이미지
	m_QuitImage_S = 0;//선택된 경우 사용하는 이미지
	m_PauseImage = 0;
	m_BackGround = 0;

	boxtouch = false;
	boxtouch2 = false;
}


PauseClass::~PauseClass()
{
}


bool PauseClass::Initialize(D3DClass* Direct3D, HWND hwnd, string directory, int screenWidth, int screenHeight)
{
	bool result;
	this->m_D3DClass = Direct3D;
	this->m_hwnd = hwnd;
	this->ScreenW = screenWidth;
	this->ScreenH = screenHeight;


	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render(false);
	m_Camera->RenderBaseViewMatrix();

	m_ContinueButton = new ButtonClass;
	if (!m_ContinueButton)
	{
		return false;
	}

	result = m_ContinueButton->Initialize(Direct3D->GetDevice(), 0, -0.3f, 2.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f));
	if (!result)
	{
		return false;
	}


	m_QuitButton = new ButtonClass;
	if (!m_QuitButton)
	{
		return false;
	}

	result = m_QuitButton->Initialize(Direct3D->GetDevice(), 0, -2.0, 2.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f));
	if (!result)
	{
		return false;
	}

	
	m_ContinueImage = new SetImageClass;
	if (!m_ContinueImage)
	{
		return false;
	}

	result = m_ContinueImage->Initialize(Direct3D->GetDevice(), hwnd, directory, "data\\Model\\Button\\Continue.png", XMFLOAT3(0.0f, -50, 1.0f), 4.0f);
	if (!result)
	{
		
		return false;
	}

	m_ContinueImage_S = new SetImageClass;
	if (!m_ContinueImage_S)
	{
		return false;
	}

	result = m_ContinueImage_S->Initialize(Direct3D->GetDevice(), hwnd, directory, "data\\Model\\Button\\Continues.png", XMFLOAT3(0.0f, -50, 1.0f), 4.0f);
	if (!result)
	{

		return false;
	}


	m_QuitImage = new SetImageClass;
	if (!m_QuitImage)
	{
		return false;
	}

	result = m_QuitImage->Initialize(Direct3D->GetDevice(), hwnd, directory, "data\\Model\\Button\\Giveup.png", XMFLOAT3(0, -200, 0), 4);
	if (!result)
	{
		return false;
	}

	m_QuitImage_S = new SetImageClass;
	if (!m_QuitImage_S)
	{
		return false;
	}

	result = m_QuitImage_S->Initialize(Direct3D->GetDevice(), hwnd, directory, "data\\Model\\Button\\Giveups.png", XMFLOAT3(0, -200, 0), 4);
	if (!result)
	{
		return false;
	}


	m_PauseImage = new SetImageClass;
	if (!m_PauseImage)
	{
		return false;
	}

	result = m_PauseImage->Initialize(Direct3D->GetDevice(), hwnd, directory, "data\\Model\\Button\\Pause.png", XMFLOAT3(0, 150, 0), 4);
	if (!result)
	{
		return false;
	}

	m_BackGround = new SetImageClass;
	if (!m_BackGround)
	{
		return false;
	}

	result = m_BackGround->Initialize(Direct3D->GetDevice(), hwnd, directory, "data\\Model\\Button\\BackGround.png", XMFLOAT3(0, 0, 0), 50);
	if (!result)
	{
		return false;
	}
	
	return true;
}


void PauseClass::Shutdown()
{

	if (m_ContinueImage)
	{
		m_ContinueImage->Close();
		delete m_ContinueImage;
		m_ContinueImage = 0;
	}

	if (m_QuitImage)
	{
		m_QuitImage->Close();
		delete m_QuitImage;
		m_QuitImage = 0;
	}

	if (m_PauseImage)
	{
		m_PauseImage->Close();
		delete m_PauseImage;
		m_PauseImage = 0;
	}

	if (m_BackGround)
	{
		m_BackGround->Close();
		delete m_BackGround;
		m_BackGround = 0;
	}


	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_ContinueButton)
	{
		m_ContinueButton->Shutdown();
		delete m_ContinueButton;
		m_ContinueButton = 0;
	}

	if (m_QuitButton)
	{
		m_QuitButton->Shutdown();
		delete m_QuitButton;
		m_QuitButton = 0;
	}

	if (m_ContinueLighted)
	{
		m_ContinueLighted->Shutdown();
		delete m_ContinueLighted;
		m_ContinueLighted = 0;
	}

	if (m_QuitLighted)
	{
		m_QuitLighted->Shutdown();
		delete m_QuitLighted;
		m_QuitLighted = 0;
	}



	return;
}


bool PauseClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, 
	ImageShaderClass* ImageShader, float frameTime, int fps, bool& b_continue)
{
	bool result;

	result = HandleMovementInput(Input, frameTime, fps, b_continue);
	if (!result)
	{
		return false;
	}

	ButtonEvent();

	result = Render(Direct3D, ShaderManager, ImageShader);
	if (!result)
	{
		return false;
	}

	return true;
}


bool PauseClass::HandleMovementInput(InputClass* Input, float frameTime, int fps, bool &b_continue)
{
	bool result;
	bool keyDown;
	bool keyDown2;

	keyDown = Input->IsLeftMouseButtonDown();
	if (keyDown && boxtouch)
	{
		b_continue = !b_continue;
	}

	keyDown2 = Input->IsLeftMouseButtonDown();
	if (keyDown2 && boxtouch2)
	{
		return false;
	}

	return true;
}

bool PauseClass::ButtonEvent() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX inverseView, inverseWorld;
	XMVECTOR direction, origin, rayOrigin, rayDirection;
	float pointX, pointY;

	GetCursorPos(&pp);
	ScreenToClient(m_hwnd, &pp);

	pointX = ((2.0f * (float)pp.x) / (float)ScreenW) - 1.0f;
	pointY = (((2.0f * (float)pp.y) / (float)ScreenH) - 1.0f) * -1.0f;


	m_D3DClass->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix.r[0].m128_f32[0];
	pointY = pointY / projectionMatrix.r[1].m128_f32[1];

	m_Camera->GetBaseViewMatrix(viewMatrix);
	inverseView = XMMatrixInverse(NULL, viewMatrix);

	direction.m128_f32[0] = (pointX * inverseView.r[0].m128_f32[0]) + (pointY * inverseView.r[1].m128_f32[0]) + inverseView.r[2].m128_f32[0];
	direction.m128_f32[1] = (pointX * inverseView.r[0].m128_f32[1]) + (pointY * inverseView.r[1].m128_f32[1]) + inverseView.r[2].m128_f32[1];
	direction.m128_f32[2] = (pointX * inverseView.r[0].m128_f32[2]) + (pointY * inverseView.r[1].m128_f32[2]) + inverseView.r[2].m128_f32[2];

	origin = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);

	m_D3DClass->GetWorldMatrix(worldMatrix);

	inverseWorld = XMMatrixInverse(NULL, worldMatrix);

	rayOrigin = XMVector3TransformCoord(origin, inverseWorld);
	rayDirection = XMVector3TransformNormal(direction, inverseWorld);
	rayDirection = XMVector3Normalize(rayDirection);

	
	
	boxtouch = false;
	boxtouch = m_ContinueButton->TouchEvent(rayOrigin, rayDirection);
	
	boxtouch2 = false;
	boxtouch2 = m_QuitButton->TouchEvent(rayOrigin, rayDirection);


	return true;
}

bool PauseClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, ImageShaderClass* ImageShader)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;
	XMMATRIX world1;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(false);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);


	Direct3D->TurnZBufferOff();
	Direct3D->EnableAlphaToCoverageBlending();

	result = m_BackGround->Render(Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	
	ImageShader->Render(Direct3D->GetDeviceContext(),m_BackGround->GetPosMatrix() , baseViewMatrix, orthoMatrix, diffuse_color);


	
	//////////////////////////////////////////////////////////////////////////////////////////////
	result = m_ContinueImage->Render(Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	
	
	ImageShader->Render(Direct3D->GetDeviceContext(), m_ContinueImage->GetPosMatrix(), baseViewMatrix, orthoMatrix, diffuse_color);
	//////////////////////////////////////////////////////////////////////////////////////////////

	result = m_QuitImage->Render(Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	
	ImageShader->Render(Direct3D->GetDeviceContext(), m_QuitImage->GetPosMatrix(), baseViewMatrix, orthoMatrix, diffuse_color);

	//////////////////////////////////////////////////////////////////////////////////////////////

	result = m_PauseImage->Render(Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	
	ImageShader->Render(Direct3D->GetDeviceContext(), m_PauseImage->GetPosMatrix(), baseViewMatrix, orthoMatrix, diffuse_color);

	//////////////////////////////////////////////////////////////////////////////////////////////


	if (boxtouch)
	{
		result = m_ContinueImage_S->Render(Direct3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		ImageShader->Render(Direct3D->GetDeviceContext(), m_ContinueImage_S->GetPosMatrix(), baseViewMatrix, orthoMatrix, diffuse_color);

		/* 버튼 마우스 감지 테두리 렌더링
		result = m_ContinueButton->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
		*/
	}

	if (boxtouch2)
	{
		result = m_QuitImage_S->Render(Direct3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}


		ImageShader->Render(Direct3D->GetDeviceContext(), m_QuitImage_S->GetPosMatrix(), baseViewMatrix, orthoMatrix, diffuse_color);

		/* 버튼 마우스 감지 테두리 렌더링
		result = m_QuitButton->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, baseViewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
		*/
	}

	Direct3D->DisableAlphaBlending();
	Direct3D->TurnZBufferOn();
	Direct3D->EndScene();

	return true;
}
