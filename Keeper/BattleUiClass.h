////////////////////////////////////////////////////////////////////////////////
// Filename: BattleUiClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _BATTLE_UI_CLASS_H_
#define _BATTLE_UI_CLASS_H_

#include "d3dclass.h"
#include "textclass.h"
#include "ImageShaderClass.h"
#include "SetImageClass.h"

class BattleUiClass {

public:
	BattleUiClass();
	~BattleUiClass();

	bool Initialize(D3DClass* Direct3D, HWND hwnd, string directory);
	void SetHealthPoint(int);
	void SetStageCount(int);
	void SetBulletCount(int);
	bool Render(D3DClass* ,ImageShaderClass*, ShaderManagerClass* , XMMATRIX , XMMATRIX ,XMMATRIX , bool);
	void Close();

private:
	XMFLOAT4 diffuse_color;
	SetImageClass* m_crosshair;
	FontClass* m_Font1;
	TextClass* t_bullet;
	TextClass* t_health;
	TextClass* t_stage;
	int m_Bullet;
	int m_Health;
	int m_stagecount;
};

#endif