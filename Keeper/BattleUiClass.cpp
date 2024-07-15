#include "BattleUiClass.h"


BattleUiClass::BattleUiClass()
{
	diffuse_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_crosshair = 0;
	m_Font1 = 0;
	t_health = 0;
	m_Health = 0;
	t_stage = 0;
	m_stagecount = 0;
	t_bullet = 0;
	m_Bullet = 0;
}


BattleUiClass::~BattleUiClass()
{
	
}



bool BattleUiClass::Initialize(D3DClass* Direct3D, HWND hwnd, string directory)
{
	bool result;

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

	t_health = new TextClass;
	if (!t_health)
	{
		return false;
	}

	// Initialize the fps text string.
	result = t_health->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), 0, 0, 30, false, m_Font1,
		"Health : 0 / 100", 220, 130, 1.0f, 0.2f, 0.2f);
	if (!result)
	{
		return false;
	}

	t_bullet = new TextClass;
	if (!t_bullet)
	{
		return false;
	}

	// Initialize the fps text string.
	result = t_bullet->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), 0, 0, 30, false, m_Font1,
		"Bullet : 0 / 15", 220, 130, 1.0f, 0.2f, 0.2f);
	if (!result)
	{
		return false;
	}

	t_stage = new TextClass;
	if (!t_stage)
	{
		return false;
	}

	// Initialize the fps text string.
	result = t_stage->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), 0, 0, 30, false, m_Font1,
		"Stage : 0", 100, -190, 1.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	m_crosshair = new SetImageClass;
	if (!m_crosshair)
	{
		return false;
	}

	result = m_crosshair->Initialize(Direct3D->GetDevice(), hwnd, directory, "data\\Model\\Crosshair\\crosshair3.png", XMFLOAT3(0.0f,0.0f,0.0f), 2);
	if (!result)
	{
		return false;
	}

	return true;
}


bool BattleUiClass::Render(D3DClass* Direct3D, ImageShaderClass* m_Shader, ShaderManagerClass* ShaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX orthoMatrix, bool test)
{
	bool result;
	XMMATRIX m_worldMatrix;

	char HealthCount[20];
	char StageCount[20];
	char BulletCount[20];

	sprintf_s(HealthCount, "Health : %d", m_Health);
	strcat_s(HealthCount, "/ 100");

	sprintf_s(StageCount, "Stage : %d", m_stagecount);

	sprintf_s(BulletCount, "Bullet : %2d / 15", m_Bullet);

	Direct3D->TurnZBufferOff();
	Direct3D->EnableAlphaToCoverageBlending();

	result = m_crosshair->Render(Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	m_Shader->Render(Direct3D->GetDeviceContext(), m_crosshair->GetPosMatrix(), viewMatrix, orthoMatrix, diffuse_color);


	Direct3D->EnableAlphaBlending();
	

	t_health->UpdateSentence(Direct3D->GetDeviceContext(), m_Font1, HealthCount, 130, 130, 1, 0, 0);
	t_health->Render(Direct3D->GetDeviceContext(), ShaderManager, XMMatrixScaling(2, 2, 2) * worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());

	t_stage->UpdateSentence(Direct3D->GetDeviceContext(), m_Font1, StageCount, 100, -190, 1.0f, 1.0f, 0.0f);
	t_stage->Render(Direct3D->GetDeviceContext(), ShaderManager, XMMatrixScaling(2, 2, 2) * worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());

	t_bullet->UpdateSentence(Direct3D->GetDeviceContext(), m_Font1, BulletCount, 130, 160, 0.5f, 0.0f, 1.0f);
	t_bullet->Render(Direct3D->GetDeviceContext(), ShaderManager, XMMatrixScaling(2, 2, 2) * worldMatrix, viewMatrix, orthoMatrix, m_Font1->GetTexture());

	Direct3D->DisableAlphaBlending();
	Direct3D->TurnZBufferOn();

	return true;
}

void BattleUiClass::SetHealthPoint(int count)
{
	m_Health = count;

	return;
}

void BattleUiClass::SetStageCount(int count)
{
	m_stagecount = count;

	return;
}

void BattleUiClass::SetBulletCount(int count)
{
	m_Bullet = count;

	return;
}

void BattleUiClass::Close()
{
	
	if (m_crosshair) {
		m_crosshair->Close();
		delete m_crosshair;
		m_crosshair = 0;
	}

	if (t_health)
	{
		t_health->Shutdown();
		delete t_health;
		t_health = 0;
	}

	if (t_stage)
	{
		t_stage->Shutdown();
		delete t_stage;
		t_stage = 0;
	}

	if (m_Font1)
	{
		m_Font1->Shutdown();
		delete m_Font1;
		m_Font1 = 0;
	}

	return;
}