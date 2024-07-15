#include "EntityClass.h"

EntityClass::EntityClass()
{
	GenTime = 0;
	UnitKind = 0;
	UnitState = 0;
	UnitSize = 1;
	HitSize = 1;
	UnitHealth = 100; //기본 체력 10
	UnitMass = 10; //기본 질량 10 
	UnitPos = XMFLOAT3(0.0f, 0.0f, 0.0f); //위치
	UnitVec = XMFLOAT3(0.0f, 0.0f, 0.0f); //벡터
	TraceVec = XMFLOAT3(0.0f, 0.0f, 0.0f);
	UnitRot = 0; //회전
	UnitSpeed = 10.0f; //속도
}

EntityClass::~EntityClass()
{

}

void EntityClass::UpdatePos(float t) //위치정보 최신화 함수
{
	UnitVec.y -= 9.8f * UnitMass * t; //중력 적용

	UnitPos.x += (UnitVec.x + TraceVec.x) * t;
	UnitPos.y += UnitVec.y * t; 
	UnitPos.z += (UnitVec.z + TraceVec.z) * t;

	if (UnitPos.y < -1.0f) //Y 고정
	{
		UnitPos.y = -1.0f;
	}
	
	ForceLoss(t);

	return;
}


void EntityClass::AddForce(XMFLOAT3 force) //운동량 부여 함수
{
	UnitVec.x += force.x / UnitMass;
	UnitVec.y += force.y / UnitMass;
	UnitVec.z += force.z / UnitMass;

	return;
}

void EntityClass::AddDamage() //개체 데미지 부여 함수
{
	UnitHealth -= 10;

	return;
}

void EntityClass::CutForce() //테스트
{

	UnitVec.x *= 0.5f;
	UnitVec.z *= 0.5f;

	return;
}

void EntityClass::ForceLoss(float t) //운동량 손실 함수
{
	
	UnitVec.x *= 0.9f * t;
	UnitVec.z *= 0.9f * t;

	return;
}

void EntityClass::TracePlayer(XMFLOAT3 position) //플레이어 추적 함수
{
	XMVECTOR t1 = XMVectorSet(position.x - UnitPos.x, 0, position.z - UnitPos.z, 0.0f);

	XMStoreFloat3(&TraceVec, XMVector3Normalize(t1) * UnitSpeed); //추적 벡터 계산

	UnitRot = atan2f(UnitPos.x - position.x, UnitPos.z - position.z); //회전 각도 계산
	
	return;
}

void EntityClass::TracePlayer() //플레이어 추적 중지
{

	TraceVec = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	return;
}

bool EntityClass::EntityCollision(XMFLOAT3 position, float radius) //개체 충돌 감지 함수
{
	XMFLOAT3 check = XMFLOAT3(UnitPos.x - position.x, UnitPos.y - position.y, UnitPos.z - position.z);

	if ((check.x * check.x + check.y * check.y + check.z * check.z) < radius * radius)
	{
		return true;
	}

	return false;
}

void EntityClass::SetKind(UINT k)//유닛 플래그 부여 함수
{
	UnitKind = k;

	switch (k)
	{
		case 0:
		
			HitSize = 0.8f;
			break;
		case 1:
		
			HitSize = 0.9f;
			break;
		default:
			HitSize = 1.0f;

	}

	return;
}

void EntityClass::SetState(UINT s)//유닛 상태 지정 함수
{
	UnitState = s;

	return;
}

void EntityClass::SetMass(int m)//유닛 질량 지정 함수
{
	UnitMass = m;

	return;
}

void EntityClass::SetSize(float s)//유닛 크기 지정 함수
{
	UnitSize = s;

	return;
}

void EntityClass::SetGenTime(float t)//유닛 애니메이션 재생 시간
{
	GenTime = t;

	return;
}

void EntityClass::SetPos(XMFLOAT3 pos)//유닛 위치 지정
{
	UnitPos = pos;

	return;
}

UINT EntityClass::GetKind()
{
	return UnitKind;
}

UINT EntityClass::GetState()
{
	return UnitState;
}

float& EntityClass::GetGenTime()
{
	return GenTime;
}

XMFLOAT3 EntityClass::GetPos()
{
	return UnitPos;
}

float EntityClass::GetSize()
{
	return HitSize;
}

XMFLOAT3 EntityClass::GetVec()
{
	return UnitVec;
}

XMMATRIX EntityClass::GetMatrix()
{
	return XMMatrixRotationY(UnitRot) * XMMatrixScaling(UnitSize, UnitSize, UnitSize) * XMMatrixTranslation(UnitPos.x, UnitPos.y, UnitPos.z);
}

XMMATRIX EntityClass::GetHit()
{
	return XMMatrixScaling(HitSize, HitSize, HitSize) * XMMatrixTranslation(UnitPos.x, UnitPos.y, UnitPos.z);
}

bool EntityClass::IsEnd()//유닛 삭제 확인 함수
{
	if (UnitState == 3 && GenTime > 24)
	{
		return true;
	}

	return false;
}

bool EntityClass::IsDead()//유닛 생존 확인 함수
{

	if (UnitHealth <= 0)
	{
		return true;
	}

	return false;
}

void EntityClass::Kill()
{
	UnitState = 3;
	GenTime = 0;
	TraceVec = XMFLOAT3(0.0f, 0.0f, 0.0f);
	HitSize = 0;
		 
	return;
}