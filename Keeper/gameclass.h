#ifndef _GAME_CLASS_H_
#define _GAME_CLASS_H_

#include "d3dclass.h" // directx ���� ���
#include "inputclass.h" // �Է� ���
#include "shadermanagerclass.h" // ��Ÿ ���̴� ���
#include "timerclass.h" // ������ ��� �ð� ��� ���
#include "textinterfaceclass.h" // ���� ������ ���
#include "cameraclass.h" // ī�޶� ���Ʈ���� ���� ���
#include "positionclass.h"// �÷��̾� ��ġ ���
#include "terrainclass.h" // �ٵ���
#include "ObjectShader.h" // ������Ʈ �޽� �ؽ�ó ���
#include <direct.h> // ���� ���α׷��� ��θ� �������� ���� �Լ� ��� 
#include "TextureShader.h" // �ؽ�ó ���� ���
#include "Loader.h" // �ִϸ��̼� ���� �� �޽� �ε� ���
#include "BattleUiClass.h" //���� UI ������ Ŭ����
#include "PauseClass.h" // ���� ȭ�� Ŭ����
#include "GameOverClass.h"// ���ӿ��� ȭ�� Ŭ����
#include "ObjectLoad.h" //������Ʈ �޽� �ε� ���
#include "EntityClass.h" //�� ��ü ���


//����Ʈ ���� ���
#include "EffectClass.h"
#include "EffectImageClass.h"

#include <random>
#include <vector>
#include <algorithm>

using namespace std;


class GameClass
{
public:
	GameClass();
	GameClass(const GameClass&);
	~GameClass();

	bool Initialize(D3DClass*, HWND, int, int);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureShaderClass*, ImageShaderClass*, ObjectShader*, float, int);
	bool GameReset();
private:
	void HandleMovementInput(InputClass*, float);
	bool Render(D3DClass*, ShaderManagerClass*, TextureShaderClass* , ImageShaderClass*, ObjectShader*, float);
	void MouseLock();
	void GameProgress(float);
	bool InterSex();
	void CollisionDetect(float);
	void SortByDistance();
private:
	BattleUiClass* m_BattleUi;
	PauseClass* m_PauseClass;
	GameOverClass* m_GGClass;
	UserInterfaceClass* m_UserInterface;
	CameraClass* m_Camera;
	PositionClass* m_Position;
	TerrainClass* m_Terrain;
	Loader* m_Player;
	Loader* m_Enemy[3];
	EffectImageClass* m_Effect;
	HWND m_hwnd;
	
	string m_directory;
	UINT aniflag= 0;
	D3DClass* m_D3D;
	int m_screenWidth;
	int m_screenHeight;


	bool m_displayUI;
	bool m_displaySet;
	bool m_thirdperson;
	int test_count= 0;

	bool m_pause, before;
	bool m_end, m_GameReset;
	bool once = true;

	vector <EntityClass> Entitys;
	vector <EffectClass> Effects;
	ObjectLoad* m_map;
	ObjectLoad* m_hitbox;
	random_device ran;
	XMFLOAT4 diffuse_color;
	float testframe = 0;

	int health_point = 100;
	int bullet_count = 15;
	int stage_count = 1;
	float reload_time = 0;
};

#endif