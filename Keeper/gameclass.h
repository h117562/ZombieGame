#ifndef _GAME_CLASS_H_
#define _GAME_CLASS_H_

#include "d3dclass.h" // directx 설정 헤더
#include "inputclass.h" // 입력 헤더
#include "shadermanagerclass.h" // 기타 쉐이더 헤더
#include "timerclass.h" // 프레임 경과 시간 계산 헤더
#include "textinterfaceclass.h" // 글자 렌더링 헤더
#include "cameraclass.h" // 카메라 뷰매트릭스 관련 헤더
#include "positionclass.h"// 플레이어 위치 헤더
#include "terrainclass.h" // 바둑판
#include "ObjectShader.h" // 오브젝트 메쉬 텍스처 헤더
#include <direct.h> // 현재 프로그램의 경로를 가져오기 위한 함수 사용 
#include "TextureShader.h" // 텍스처 저장 헤더
#include "Loader.h" // 애니메이션 포함 된 메쉬 로드 헤더
#include "BattleUiClass.h" //전투 UI 렌더링 클래스
#include "PauseClass.h" // 정지 화면 클래스
#include "GameOverClass.h"// 게임오버 화면 클래스
#include "ObjectLoad.h" //오브젝트 메쉬 로드 헤더
#include "EntityClass.h" //적 개체 헤더


//이펙트 관련 헤더
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