#include "EntityClass.h"

EntityClass::EntityClass()
{
	GenTime = 0;
	UnitKind = 0;
	UnitState = 0;
	UnitSize = 1;
	HitSize = 1;
	UnitHealth = 100; //�⺻ ü�� 10
	UnitMass = 10; //�⺻ ���� 10 
	UnitPos = XMFLOAT3(0.0f, 0.0f, 0.0f); //��ġ
	UnitVec = XMFLOAT3(0.0f, 0.0f, 0.0f); //����
	TraceVec = XMFLOAT3(0.0f, 0.0f, 0.0f);
	UnitRot = 0; //ȸ��
	UnitSpeed = 10.0f; //�ӵ�
}

EntityClass::~EntityClass()
{

}

void EntityClass::UpdatePos(float t) //��ġ���� �ֽ�ȭ �Լ�
{
	UnitVec.y -= 9.8f * UnitMass * t; //�߷� ����

	UnitPos.x += (UnitVec.x + TraceVec.x) * t;
	UnitPos.y += UnitVec.y * t; 
	UnitPos.z += (UnitVec.z + TraceVec.z) * t;

	if (UnitPos.y < -1.0f) //Y ����
	{
		UnitPos.y = -1.0f;
	}
	
	ForceLoss(t);

	return;
}


void EntityClass::AddForce(XMFLOAT3 force) //��� �ο� �Լ�
{
	UnitVec.x += force.x / UnitMass;
	UnitVec.y += force.y / UnitMass;
	UnitVec.z += force.z / UnitMass;

	return;
}

void EntityClass::AddDamage() //��ü ������ �ο� �Լ�
{
	UnitHealth -= 10;

	return;
}

void EntityClass::CutForce() //�׽�Ʈ
{

	UnitVec.x *= 0.5f;
	UnitVec.z *= 0.5f;

	return;
}

void EntityClass::ForceLoss(float t) //��� �ս� �Լ�
{
	
	UnitVec.x *= 0.9f * t;
	UnitVec.z *= 0.9f * t;

	return;
}

void EntityClass::TracePlayer(XMFLOAT3 position) //�÷��̾� ���� �Լ�
{
	XMVECTOR t1 = XMVectorSet(position.x - UnitPos.x, 0, position.z - UnitPos.z, 0.0f);

	XMStoreFloat3(&TraceVec, XMVector3Normalize(t1) * UnitSpeed); //���� ���� ���

	UnitRot = atan2f(UnitPos.x - position.x, UnitPos.z - position.z); //ȸ�� ���� ���
	
	return;
}

void EntityClass::TracePlayer() //�÷��̾� ���� ����
{

	TraceVec = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	return;
}

bool EntityClass::EntityCollision(XMFLOAT3 position, float radius) //��ü �浹 ���� �Լ�
{
	XMFLOAT3 check = XMFLOAT3(UnitPos.x - position.x, UnitPos.y - position.y, UnitPos.z - position.z);

	if ((check.x * check.x + check.y * check.y + check.z * check.z) < radius * radius)
	{
		return true;
	}

	return false;
}

void EntityClass::SetKind(UINT k)//���� �÷��� �ο� �Լ�
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

void EntityClass::SetState(UINT s)//���� ���� ���� �Լ�
{
	UnitState = s;

	return;
}

void EntityClass::SetMass(int m)//���� ���� ���� �Լ�
{
	UnitMass = m;

	return;
}

void EntityClass::SetSize(float s)//���� ũ�� ���� �Լ�
{
	UnitSize = s;

	return;
}

void EntityClass::SetGenTime(float t)//���� �ִϸ��̼� ��� �ð�
{
	GenTime = t;

	return;
}

void EntityClass::SetPos(XMFLOAT3 pos)//���� ��ġ ����
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

bool EntityClass::IsEnd()//���� ���� Ȯ�� �Լ�
{
	if (UnitState == 3 && GenTime > 24)
	{
		return true;
	}

	return false;
}

bool EntityClass::IsDead()//���� ���� Ȯ�� �Լ�
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