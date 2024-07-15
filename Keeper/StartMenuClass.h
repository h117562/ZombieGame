#ifndef _START_MENU_CLASS_H_
#define _START_MENU_CLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
#include "shadermanagerclass.h"
#include "cameraclass.h"
#include "fontclass.h"
#include "textclass.h"
#include "ButtonClass.h"
#include "gameclass.h"
#include <DirectXCollision.h>

class MenuClass
{
public:
	MenuClass();
	~MenuClass();

	bool Initialize(D3DClass*, HWND , InputClass*, ShaderManagerClass*, TextureShaderClass* ,ImageShaderClass*, ObjectShader*, int, int);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, float, int);

	bool GetSkip();

private:
	bool HandleMovementInput(InputClass*, float, int);
	bool Render(D3DClass*, ShaderManagerClass*);
	bool RenderLogo(D3DClass*);
	bool ButtonEvent();
	void EntryGame();
private:
	HWND m_hwnd;
	D3DClass* m_D3D;
	ButtonClass* m_StartButton, * m_StartLighted;
	ButtonClass* m_QuitButton, * m_QuitLighted;
	CameraClass* m_Camera;
	FontClass* m_Font1;
	TextClass* m_Start;
	TextClass* m_Out;
	ObjectShader* m_ObjectShader;
	ShaderManagerClass* m_ShaderManager;
	TextureShaderClass* m_TextureShader;
	ImageShaderClass* m_ImageShader;
	GameClass* m_GameClass;
	int ScreenW, ScreenH;

	string m_directory;
	SetImageClass* m_LogoImage;
	POINT pp;
	bool skip;
	bool logo;
	float color;
	float t_counter;
	XMFLOAT4 diffusecolor;
	bool boxtouch, boxtouch2;
};

#endif