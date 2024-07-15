#include <d3d11.h>
#include <directxmath.h>


using namespace DirectX;

class EntityClass
{
public:
	EntityClass();
	~EntityClass();

	void UpdatePos(float frametime);
	void AddForce(XMFLOAT3 force);
	void TracePlayer(XMFLOAT3 position);
	void TracePlayer();
	void AddDamage();
	void CutForce();

	UINT GetKind();
	UINT GetState();
	XMFLOAT3 GetPos();
	XMFLOAT3 GetVec();
	XMMATRIX GetMatrix();
	XMMATRIX GetHit();
	float& GetGenTime();
	float GetSize();

	void SetSize(float);
	void SetGenTime(float);
	void SetKind(UINT);
	void SetState(UINT);
	void SetMass(int);
	void SetPos(XMFLOAT3);
	bool EntityCollision(XMFLOAT3 position, float distance);
	bool IsDead();
	bool IsEnd();
	void Kill();


private:
	void ForceLoss(float);

private:
	int UnitHealth;
	float UnitSize;
	float HitSize;
	float GenTime;
	UINT UnitKind;
	int UnitMass;
	XMFLOAT3 UnitVec;
	XMFLOAT3 UnitPos;
	XMFLOAT3 TraceVec;
	float UnitRot;
	float UnitSpeed;
	UINT UnitState;
};