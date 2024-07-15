#include <d3d11.h>
#include <directxmath.h>


using namespace DirectX;

class EffectClass
{
public:
	EffectClass();
	~EffectClass();

	void TracePlayer(XMFLOAT3 position);

	UINT GetKind();
	XMFLOAT3 GetPos();
	XMMATRIX GetMatrix();
	float& GetGenTime();

	void ProgressTime(float);
	void SetSpeed(float);
	void SetSize(float);
	void SetGenTime(float);
	void SetKind(UINT);
	void SetPos(XMFLOAT3);

private:
	float speed;
	float EffectSize;
	float GenTime;
	UINT EffectKind;
	XMFLOAT3 EffectPos;
	float EffectRot;

};