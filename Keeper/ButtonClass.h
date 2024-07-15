#ifndef _BUTTON_CLASS_H_
#define _BUTTON_CLASS_H_

#include "shadermanagerclass.h"
#include "TouchClass.h"

class ButtonClass
{
private:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ButtonClass();
	~ButtonClass();

	bool Initialize(ID3D11Device*, float positionX, float positionY, float width, float height, XMFLOAT4 color);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, ShaderManagerClass*, XMMATRIX, XMMATRIX, XMMATRIX);
	bool TouchEvent(XMVECTOR ray_o, XMVECTOR ray_d);
	int GetIndexCount();
private:

	bool InitializeBuffers(ID3D11Device*, float, float, float, float, XMFLOAT4);
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	TouchClass* m_touch;
	int m_vertexCount, m_indexCount;
};




#endif