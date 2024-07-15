
#ifndef _PAUSECLASS_H_
#define _PAUSECLASS_H_

#include "d3dclass.h"
#include "inputclass.h"
#include "ImageShaderClass.h"
#include "cameraclass.h"
#include "ButtonClass.h"
#include "SetImageClass.h"

#include <DirectXCollision.h>

class PauseClass
{
public:
	PauseClass();
	~PauseClass();

	bool Initialize(D3DClass*, HWND, string, int, int);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, ImageShaderClass*, float, int, bool&);

private:
	bool HandleMovementInput(InputClass*, float, int, bool&);
	bool Render(D3DClass*, ShaderManagerClass*, ImageShaderClass*);
	bool ButtonEvent();

private:
	HWND m_hwnd;
	D3DClass* m_D3DClass;
	ButtonClass* m_ContinueButton, * m_ContinueLighted;
	ButtonClass* m_QuitButton, * m_QuitLighted;
	CameraClass* m_Camera;

	SetImageClass* m_ContinueImage;
	SetImageClass* m_QuitImage;

	SetImageClass* m_ContinueImage_S; //선택된 경우 사용하는 이미지
	SetImageClass* m_QuitImage_S; //선택된 경우 사용하는 이미지

	SetImageClass* m_PauseImage;
	SetImageClass* m_BackGround;

	XMFLOAT4 diffuse_color;
	int ScreenW, ScreenH;
	POINT pp;
	bool boxtouch, boxtouch2;
};

#endif