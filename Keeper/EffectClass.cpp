#include "EffectClass.h"


EffectClass::EffectClass()
{
	GenTime = 0;
	speed = 1;
	EffectKind = 0;
	EffectSize = 1;
	EffectPos = XMFLOAT3(0.0f, 0.0f, 0.0f); //위치
	EffectRot = 0; //회전
}

EffectClass::~EffectClass()
{

}

void EffectClass::TracePlayer(XMFLOAT3 position)
{
	EffectRot = atan2f(EffectPos.x - position.x, EffectPos.z - position.z);

	return;
}

void EffectClass::SetKind(UINT k)
{
	EffectKind = k;

	return;
}


void EffectClass::SetSize(float s)
{
	EffectSize = s;

	return;
}

void EffectClass::SetGenTime(float t)
{
	GenTime = t;

	return;
}

void EffectClass::SetPos(XMFLOAT3 pos)
{
	EffectPos = pos;

	return;
}

void EffectClass::SetSpeed(float t)
{
	speed = t;

	return;
}

void EffectClass::ProgressTime(float frameTime)
{
	GenTime += frameTime * speed;

	return;
}

UINT EffectClass::GetKind()
{
	return EffectKind;
}

float& EffectClass::GetGenTime()
{
	return GenTime;
}

XMFLOAT3 EffectClass::GetPos()
{
	return EffectPos;
}

XMMATRIX EffectClass::GetMatrix()
{
	return XMMatrixRotationY(EffectRot) * XMMatrixScaling(EffectSize, EffectSize, EffectSize) * XMMatrixTranslation(EffectPos.x, EffectPos.y, EffectPos.z);
}