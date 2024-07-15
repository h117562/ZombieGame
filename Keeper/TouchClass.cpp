#include "TouchClass.h"

TouchClass::TouchClass()
{

}

TouchClass::~TouchClass()
{

}

bool TouchClass::Initialize(float x, float y, float w, float h)
{


	vertpos[0] = XMVectorSet((x + w / 2), (y + h / 2), 0.0f, 0.0f);
	vertpos[1] = XMVectorSet((x + w / 2), (y - h / 2), 0.0f, 0.0f);
	vertpos[2] = XMVectorSet((x - w / 2), (y + h / 2), 0.0f, 0.0f);
	vertpos[3] = XMVectorSet((x - w / 2), (y - h / 2), 0.0f, 0.0f);
	vertpos[4] = XMVectorSet((x - w / 2), (y + h / 2), 0.0f, 0.0f);
	vertpos[5] = XMVectorSet((x + w / 2), (y - h / 2), 0.0f, 0.0f);


	return true;
}

void TouchClass::Shutdown()
{

}



bool TouchClass::Intersect_11(XMVECTOR ray_o, XMVECTOR ray_d)
{
	float a;
	bool inter_1, inter_2;
	inter_1 = TriangleTests::Intersects(ray_o, ray_d, vertpos[0], vertpos[1], vertpos[2], a);
	inter_2 = TriangleTests::Intersects(ray_o, ray_d, vertpos[3], vertpos[4], vertpos[5], a);



	if (inter_1 || inter_2) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
