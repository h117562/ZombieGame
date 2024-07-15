#ifndef _TOUCHCLASS_H_
#define _TOUCHCLASS_H_

#include <directxmath.h>
#include <DirectXCollision.h>
using namespace DirectX;

class TouchClass
{
public:
	TouchClass();
	~TouchClass();

	bool Initialize(float, float, float, float);
	void Shutdown();

	bool Intersect_11(XMVECTOR, XMVECTOR);
private:

	XMVECTOR vertpos[6];
	int ScreenW, ScreenH;

};
#endif