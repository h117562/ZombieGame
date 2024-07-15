#include "gameclass.h"

GameClass::GameClass()
{
	m_UserInterface = 0;
	m_PauseClass = 0;
	m_GGClass = 0;
	m_BattleUi = 0;
	m_Camera = 0;
	m_Position = 0;
	m_Terrain = 0;
	m_Player = 0;
	m_Effect = 0;
	diffuse_color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Enemy[0] = 0;
	m_Enemy[1] = 0;
	m_Enemy[2] = 0;

	m_thirdperson = true;
	m_displaySet = false;
	m_displayUI = true;
	m_end = false;
	m_pause = false;
	m_GameReset = false;
	before = m_pause;
	m_directory = "";
	m_D3D = 0;
	m_map = 0;
	m_hitbox = 0;
}


GameClass::GameClass(const GameClass& other)
{
}


GameClass::~GameClass()
{
}


bool GameClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	this->m_hwnd = hwnd;
	this->m_screenWidth = screenWidth;
	this->m_screenHeight = screenHeight;
	this->m_D3D = Direct3D;

	char thisPath[100];
	_getcwd(thisPath, 100);
	m_directory = thisPath;
	

	// Create the user interface object.
	m_UserInterface = new UserInterfaceClass;
	if(!m_UserInterface)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_UserInterface->Initialize(Direct3D, screenHeight, screenWidth);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}


	m_BattleUi = new BattleUiClass;
	if (!m_BattleUi)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_BattleUi->Initialize(Direct3D, hwnd, m_directory);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the BattleUiClass", L"Error", MB_OK);
		return false;
	}

	m_PauseClass = new PauseClass;
	if (!m_PauseClass)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_PauseClass->Initialize(Direct3D, hwnd, m_directory, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the PauseClass", L"Error", MB_OK);
		return false;
	}

	m_GGClass = new GameOverClass;
	if (!m_GGClass)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_GGClass->Initialize(Direct3D, hwnd, m_directory, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the PauseClass", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0, 0.0f, -10.0f);
	m_Camera->Render(m_thirdperson);
	m_Camera->RenderBaseViewMatrix();

	// Create the position object.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation.
	m_Position->SetPosition(0, 0, 0);
	m_Position->SetRotation(0.0f, 180, 0.0f);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}
	

	m_Player = new Loader;
	if (!m_Player) {
		return false;
	}

	if (!m_Player->Load(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), m_directory, "data\\model\\Pistol\\PlayerStay.fbx", 0.2f , 0.8f, true))
	{
		MessageBox(hwnd, L"Loader Failed", L"Error", MB_OK);
	}
	// Set the UI to display by default.
	
	m_Player->AddAni(hwnd, "PlayerWalk.FBX", 1.0f, true);
	m_Player->AddAni(hwnd, "PlayerBackward.FBX", 1.0f, true);
	m_Player->AddAni(hwnd, "PlayerLeft.FBX", 1.0f, true);
	m_Player->AddAni(hwnd, "PlayerRight.FBX", 1.0f, true);


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_Enemy[0] = new Loader;
	if (!m_Enemy[0]) {
		return false;
	}

	if (!m_Enemy[0]->Load(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), m_directory, "data\\model\\Zombie\\ZombieStay.FBX", 0.2f ,0.5f, true))
	{
		MessageBox(hwnd, L"Loader Failed", L"Error", MB_OK);
	}
	
	m_Enemy[1] = new Loader;
	if (!m_Enemy[1]) {
		return false;
	}

	if (!m_Enemy[1]->Load(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), m_directory, "data\\model\\Zombie1\\ZombieStay1.FBX", 0.2f, 0.5f, true))
	{
		MessageBox(hwnd, L"Loader Failed", L"Error", MB_OK);
	}

	m_Enemy[2] = new Loader;
	if (!m_Enemy[2]) {
		return false;
	}

	if (!m_Enemy[2]->Load(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), m_directory, "data\\model\\Zombie2\\ZombieStay2.FBX", 0.2f, 0.5f, true))
	{
		MessageBox(hwnd, L"Loader Failed", L"Error", MB_OK);
	}

	m_Enemy[0]->AddAni(hwnd, "ZombieRun.FBX", 0.7f, true);
	m_Enemy[0]->AddAni(hwnd, "ZombieAttack.FBX", 0.3f, false);
	m_Enemy[0]->AddAni(hwnd, "ZombieDying.FBX", 0.5f, false);
	
	m_Enemy[1]->AddAni(hwnd, "ZombieRun.FBX", 0.7f, true);
	m_Enemy[1]->AddAni(hwnd, "ZombieAttack.FBX", 0.5f, false);
	m_Enemy[1]->AddAni(hwnd, "ZombieDying.FBX", 0.5f, false);

	m_Enemy[2]->AddAni(hwnd, "ZombieRun.FBX", 0.5f, true);
	m_Enemy[2]->AddAni(hwnd, "ZombieAttack.FBX", 0.5f, false);
	m_Enemy[2]->AddAni(hwnd, "ZombieDying.FBX", 0.5f, false);
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_map = new ObjectLoad;
	result = m_map->Load(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), m_directory, "data\\model\\map\\9.fbx", 0.1);
	if (!result)
	{
		MessageBox(hwnd, L"Test Failed", L"Error", MB_OK);
		return false;
	}

	m_map->SetRotation(1.57);



	m_hitbox = new ObjectLoad;
	result = m_hitbox->Load(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), m_directory, "data\\model\\hit_test.fbx", 1.0f);
	if (!result)
	{
		MessageBox(hwnd, L"Test Failed", L"Error", MB_OK);
		return false;
	}

	m_hitbox->SetSkeletonRender(true);

	m_Effect = new EffectImageClass;
	if (!m_Effect)
	{
		return false;
	}

	string e_list[15];
	
	e_list[0] = "data\\Model\\effect\\Blood1.png";
	e_list[1] = "data\\Model\\effect\\Blood2.png";
	e_list[2] = "data\\Model\\effect\\Blood3.png";
	e_list[3] = "data\\Model\\effect\\Blood4.png";
	e_list[4] = "data\\Model\\effect\\Blood5.png";
	e_list[5] = "data\\Model\\effect\\Blood6.png";
	e_list[6] = "data\\Model\\effect\\Blood7.png";
	e_list[7] = "data\\Model\\effect\\Blood8.png";
	e_list[8] = "data\\Model\\effect\\Blood9.png";
	e_list[9] = "data\\Model\\effect\\Blood10.png";
	e_list[10] = "data\\Model\\effect\\Blood11.png";
	e_list[11] = "data\\Model\\effect\\Blood12.png";
	e_list[12] = "data\\Model\\effect\\Blood13.png";
	e_list[13] = "data\\Model\\effect\\Blood14.png";
	e_list[14] = "data\\Model\\effect\\Blood15.png";


	result = m_Effect->Initialize(Direct3D->GetDevice(), hwnd, m_directory, e_list, 15, 0.1);
	if (!result)
	{
		MessageBox(hwnd, L"Effect Load Failed", L"Error", MB_OK);
		return false;
	}

	/////////////////////////////ENTITY SET//////////////////////////////
	EntityClass tty;
	tty.SetKind(0);
	tty.SetSize(0.2);
	tty.SetPos(XMFLOAT3(120.0f, 1.0f, -20.0f));
	Entitys.push_back(tty);

	tty.SetKind(1);
	tty.SetSize(0.1);
	tty.SetPos(XMFLOAT3(120.0f, 1.0f, -30.0f));
	Entitys.push_back(tty);

	tty.SetKind(2);
	tty.SetSize(0.1);
	tty.SetPos(XMFLOAT3(120.0f, 1.0f, -40.0f));
	Entitys.push_back(tty);
	////////////////////////////////////////////////////////////////////


	return true;
}


void GameClass::Shutdown()
{
	Entitys.clear();
	vector<EntityClass>().swap(Entitys);

	Effects.clear();
	vector<EffectClass>().swap(Effects);


	if (m_Effect) {
		m_Effect->Close();
		delete m_Effect;
		m_Effect = 0;
	}

	// Release the terrain object.
	if (m_Player) {
		m_Player->Close();
		delete m_Player;
		m_Player = 0;
	}

	if (m_Enemy) {
		for (int i = 0; i < 3; i++)
		{
			m_Enemy[i]->Close();
			delete m_Enemy[i];
			m_Enemy[i] = 0;
		}
	}

	if(m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	if (m_hitbox)
	{
		m_hitbox->Close();
		delete m_hitbox;
		m_hitbox = 0;
	}

	if (m_map)
	{
		m_map->Close();
		delete m_map;
		m_map = 0;
	}

	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the user interface object.
	if(m_UserInterface)
	{
		m_UserInterface->Shutdown();
		delete m_UserInterface;
		m_UserInterface = 0;
	}

	if (m_BattleUi)
	{
		m_BattleUi->Close();
		delete m_BattleUi;
		m_BattleUi = 0;
	}

	if (m_GGClass)
	{
		m_GGClass->Shutdown();
		delete m_GGClass;
		m_GGClass = 0;
	}

	if (m_PauseClass)
	{
		m_PauseClass->Shutdown();
		delete m_PauseClass;
		m_PauseClass = 0;
	}

	return;
}



bool GameClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, TextureShaderClass* m_TS, ImageShaderClass* m_IS, ObjectShader* m_OS, float frameTime, int fps)
{
	bool result;
	float posX, posY, posZ, rotX, rotY, rotZ;
	

	if (!before == m_pause)//m_pause에 변화가 있었다면 3인칭 움직임 고정
	{
		Input->ToggleMove();

		before = m_pause;
	}

	if (!(m_pause||m_end))
	{
		MouseLock();
		HandleMovementInput(Input, frameTime);
		
		CollisionDetect(frameTime);//물체끼리 서로 밀어낸다

		for (EntityClass& entity : Entitys)
		{
			entity.UpdatePos(frameTime);
		}
	}
	
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);
	

	result = m_UserInterface->Frame(Direct3D->GetDeviceContext(), fps, posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render(Direct3D, ShaderManager, m_TS, m_IS, m_OS, frameTime);
	if(!result)
	{
		return false;
	}


	if (m_pause)
	{
		result = m_PauseClass->Frame(Direct3D, Input, ShaderManager, m_IS, frameTime, fps, m_pause);
		if (!result)
		{
			return false;
		}
	}
	
	if (m_end)
	{
		if (once)
		{
			Input->ToggleMove();

			once = false;
		}

		result = m_GGClass->Frame(Direct3D, Input, ShaderManager, m_IS, frameTime, fps, m_GameReset);
		if (!result)
		{
			return false;
		}
	}

	Direct3D->EndScene();

	GameProgress(frameTime);

	//일정시간이 지난 이펙트들은 삭제
	if (Effects.size() > 0)
	{

		for (EffectClass& list : Effects)
		{
			list.ProgressTime(frameTime);
			list.TracePlayer(XMFLOAT3(posX, 0, posZ));
		}

		for (int i = 0; i < Effects.size(); i++)
		{
			if (Effects[i].GetGenTime() > 15)
			{
				Effects.erase(Effects.begin() + i);
			}
		}
	}
	
	return true;
}

void GameClass::HandleMovementInput(InputClass* Input, float frameTime)
{
	bool WkeyDown, AkeyDown, SkeyDown, DkeyDown, SpacekeyDown, keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;
	float size = 0.2f;

	m_Position->SetFrameTime(frameTime);

	AkeyDown = Input->IsLeftPressed();
	m_Position->MoveLeft(AkeyDown);

	DkeyDown = Input->IsRightPressed();
	m_Position->MoveRight(DkeyDown);

	WkeyDown = Input->IsUpPressed();
	m_Position->MoveForward(WkeyDown);

	SkeyDown = Input->IsDownPressed();
	m_Position->MoveBackward(SkeyDown);

	//SpacekeyDown = Input->IsSpacePressed();
	//m_Position->MoveUpward(SpacekeyDown);

	//keyDown = Input->IsShiftPressed();
	//m_Position->MoveDownward(keyDown);

	if (Input->IsEscapePressed())
	{
		m_pause = true;
	}

	if (bullet_count > 0) //총알이 남아있을 경우에만 클릭 가능
	{
		keyDown = Input->IsLeftMouseButtonDownOnce();
		if (keyDown)
		{
			InterSex();
		
			bullet_count--;
		}
	}
	

////////////////////////////////////////////////////////////////

	if (Input->Is_NUM1_Pressed())
	{
		EntityClass tty;
		XMFLOAT3 fmp;

		
		fmp = XMFLOAT3(120.5f, 5.0f, -30.0f);
		

		tty.SetPos(fmp);
		tty.SetKind(0);
		tty.SetSize(0.2);

		Entitys.push_back(tty);
	}

	if (Input->Is_NUM2_Pressed())
	{
		EntityClass tty;
		
		XMFLOAT3 fmp;


		fmp = XMFLOAT3(120.0f, 5.0f, -30.0f);

		tty.SetPos(fmp);
		tty.SetKind(2);
		tty.SetSize(0.1);

		Entitys.push_back(tty);
	}



//////////////////////////////////////////////////////////////////////////////
	if (Input->IsF1Toggled())
	{
		m_displaySet = !m_displaySet;

	}

	if (Input->IsF2Toggled())
	{
		m_displayUI = !m_displayUI;

	}

	if (Input->IsF5Toggled())
	{
		m_thirdperson = !m_thirdperson;
	}

	aniflag = 0;

	if (WkeyDown)
	{
		if (!(WkeyDown && SkeyDown)) {
			aniflag = 1;
		}
		else
		{
			aniflag = 0;
		}
	}

	if (SkeyDown)
	{
		if (!(WkeyDown && SkeyDown)) {
			aniflag = 2;
		}
		else {
			aniflag = 0;
		}
	}
	
	if (!(AkeyDown && DkeyDown))
	{
		if (AkeyDown) {
			aniflag = 3;
		}else if(DkeyDown){
			aniflag = 4;
		}
	}
		
	m_Position->SpaceMove(Input->m_mouseX, Input->m_mouseY);
	m_Camera->SetRound(Input->m_mouseX, Input->m_mouseY);
	
	Input->m_mouseX = 0;
	Input->m_mouseY = 0;

	
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	m_Player->SetPosition(posX, posY, posZ);
	m_Player->SetRotation(rotX, rotY, rotZ);

	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);
	
	if (Input->Is_NUM3_Pressed())
	{
		Entitys[0].AddForce(XMFLOAT3(100, 0, 0));
	}

	keyDown = Input->IsRightMouseButtonDownOnce();
	if (keyDown)
	{
		Entitys[0].AddForce(XMFLOAT3(-1000, 0, 0));
	}
	return;
}



bool GameClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureShaderClass* m_TS, ImageShaderClass* m_IS, ObjectShader* m_OS, float frametime)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;
	float posX, posY, posZ;
	
	// 뷰 행렬 업데이트
	m_Camera->Render(m_thirdperson);

	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	// 화면 초기화
	Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	/*
	//바둑발판 렌더링
	m_Terrain->Render(Direct3D->GetDeviceContext());

	result = ShaderManager->RenderColorShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix);
	if (!result)
	{
		return false;
	}
	*/


	//플레이어 렌더링
	if (m_thirdperson)
	{
		m_TS->Render(Direct3D->GetDeviceContext(), m_Player->GetPosition(), viewMatrix, projectionMatrix);
		m_Player->Render(Direct3D->GetDeviceContext(), aniflag, frametime, testframe);
	}

	//적 개체 렌더링
	for (EntityClass& entity : Entitys)
	{
		m_TS->Render(Direct3D->GetDeviceContext(), entity.GetMatrix(), viewMatrix, projectionMatrix);
		m_Enemy[entity.GetKind()]->Render(Direct3D->GetDeviceContext(), entity.GetState(), frametime, entity.GetGenTime());


		if (m_displaySet)
		{
			m_OS->Render(Direct3D->GetDeviceContext(), entity.GetHit(), viewMatrix, projectionMatrix);
			m_hitbox->Render(Direct3D->GetDeviceContext());
		}

	}

	//맵 렌더링
	m_OS->Render(Direct3D->GetDeviceContext(), m_map->GetPosition(), viewMatrix, projectionMatrix);
	m_map->Render(Direct3D->GetDeviceContext());



	//이펙트 렌더링
	
	if (Effects.size() > 0)
	{
		Direct3D->EnableAlphaToCoverageBlending();
		for (EffectClass& list : Effects)
		{
			m_Effect->Render(Direct3D->GetDeviceContext(), list.GetGenTime());
			m_IS->Render(Direct3D->GetDeviceContext(), list.GetMatrix() , viewMatrix, projectionMatrix, diffuse_color);
		}
	}
	
	
	//인게임 UI 렌더링
	if (m_displayUI)
	{
		result = m_BattleUi->Render(Direct3D, m_IS, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix, false);
		if (!result)
		{
			return false;
		}
	}

	//상세 UI 렌더링
	if(m_displaySet)
	{
		result = m_UserInterface->Render(Direct3D, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if(!result)
		{
			return false;
		}
	}

	return true;
}


void GameClass::MouseLock()
{
	POINT pp= {0,0};

	if (GetFocus() == m_hwnd)
	{	
		ScreenToClient(m_hwnd, &pp);
		SetCursorPos(m_screenWidth/2 - pp.x, m_screenHeight / 2 - pp.y);
	}

	return;
}

bool GameClass::GameReset()
{
	return m_GameReset;
}

void GameClass::CollisionDetect(float frameTime)
{

	for (int i = 0; i < Entitys.size(); i++)
	{
		for (int j = 0; j < Entitys.size(); j++)
		{
			if (Entitys[i].EntityCollision(Entitys[j].GetPos(), 5) && i != j)
			{
				
				XMVECTOR f1 = XMVectorSet( //엔티티끼리 서로 밀어내는 방향벡터 생성
					Entitys[i].GetPos().x - Entitys[j].GetPos().x,
					Entitys[i].GetPos().y - Entitys[j].GetPos().y,
					Entitys[i].GetPos().z - Entitys[j].GetPos().z,
					0.0f
					);

				XMVECTOR f2 = XMVectorSet( //엔티티끼리 서로 밀어내는 방향벡터 생성
					Entitys[j].GetPos().x - Entitys[i].GetPos().x,
					Entitys[j].GetPos().y - Entitys[i].GetPos().y,
					Entitys[j].GetPos().z - Entitys[i].GetPos().z,
					0.0f
					);

				XMFLOAT3 a1;
				XMFLOAT3 a2;

				XMStoreFloat3(&a1, XMVector3Normalize(f1) * 10);
				XMStoreFloat3(&a2, XMVector3Normalize(f2) * 10);

				Entitys[i].AddForce(a1);
				Entitys[j].AddForce(a2);
			}
		}
	}
}

bool GameClass::InterSex()
{
	bool result;
	XMMATRIX viewMatrix;
	XMMATRIX inverseView;
	XMVECTOR direction, origin;
	float pointX= 0, pointY= 0;
	float dist;

	m_Camera->GetViewMatrix(viewMatrix);
	inverseView = XMMatrixInverse(NULL, viewMatrix);

	direction = XMVectorSet(
		inverseView.r[2].m128_f32[0],
		inverseView.r[2].m128_f32[1],
		inverseView.r[2].m128_f32[2],
		0.0f);


	for (EntityClass& entity : Entitys)
	{
		XMFLOAT3 xf3p = entity.GetPos();
		XMFLOAT3 xf3i;

		origin = XMVectorSet(
			inverseView.r[3].m128_f32[0] - xf3p.x,
			inverseView.r[3].m128_f32[1] - xf3p.y,
			inverseView.r[3].m128_f32[2] - xf3p.z,
			0.0f
			);
		
		if (m_hitbox->ObjectIntersect(xf3i, origin, direction, entity.GetSize()))
		{
			EffectClass ec;
			xf3i.x += xf3p.x;
			xf3i.y += xf3p.y;
			xf3i.z += xf3p.z;

			ec.SetPos(xf3i);
			ec.SetSpeed(30);
			Effects.push_back(ec);

			entity.AddDamage();

			return true;
		}

	}

	
	return false;
}

void GameClass::GameProgress(float frametime)
{
	XMFLOAT3 m_pos;
	m_Position->GetPosition(m_pos.x, m_pos.y, m_pos.z);
	m_BattleUi->SetHealthPoint(health_point);
	m_BattleUi->SetStageCount(stage_count);
	m_BattleUi->SetBulletCount(bullet_count);

	for (int i = 0; i < Entitys.size(); i++) //플레이어와 너무 가까우면 따라가기를 멈춤
	{
		
		if (!Entitys[i].IsEnd()) //살아있는 모든 개체들
		{
			UINT k = Entitys[i].GetState();
			
			
			if (Entitys[i].IsDead() && k != 3) //체력이 0이하인 개체들을 죽임
			{
				Entitys[i].Kill();
			}
			else
			{

			if (k == 2 && Entitys[i].GetGenTime() > 24)
			{
				Entitys[i].SetGenTime(0);
				Entitys[i].SetState(0);
			}

			if (k == 0 || k == 1) //개체 상태가 대기 또는 달리는 경우
			{
				if (Entitys[i].EntityCollision(m_pos, 9)) //범위 안에 있는 경우
				{

					Entitys[i].SetState(2);
					Entitys[i].SetGenTime(0);
					Entitys[i].TracePlayer();

					health_point -= 10; // 플레이어 체력 깎음
				}
				else // 범위 안에 없는 경우
				{
					Entitys[i].SetState(1);
					Entitys[i].TracePlayer(m_pos);
				}
			}

			}
		}
		else
		{
			Entitys.erase(Entitys.begin() + i);//개체가 죽은 후 일정시간 지났을 경우 개체 삭제
		}
		
	}

	if (Entitys.size() < 1)//개체수가 0이면 게임단계 증가 및 적 추가
	{
		stage_count++;

		mt19937 mt(ran());
		uniform_int_distribution<int> dis1(0, 2);
		uniform_int_distribution<int> dis2(100, 140);
		uniform_int_distribution<int> dis3(10, 50);

		for (int i = 0; i < stage_count * 3; i++)
		{
			EntityClass etc;
			XMFLOAT3 xf3(dis2(mt), 3, -dis3(mt));
			UINT k = dis1(mt);

			etc.SetKind(k);
			etc.SetPos(xf3);

			if (k == 0)
			{
				etc.SetSize(0.2);
			}
			else
			{
				etc.SetSize(0.1);
			}
			

			Entitys.push_back(etc);
		}
	}

	if (bullet_count < 1)
	{
		if (reload_time > 1)
		{
			bullet_count = 15;
			reload_time = 0;
		}

		reload_time += frametime;
	}

	if (health_point <= 0)//체력 다 떨어졌을때 게임오버
	{
		m_end = true;
	}
}

void GameClass::SortByDistance()
{
	
		
}