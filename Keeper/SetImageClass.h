#ifndef _SET_IMAGE_CLASS_
#define _SET_IMAGE_CLASS_

#include <string>
#include <d3dx11.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

class SetImageClass
{
private:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texcoord;
	};

	struct MatrixBuffer {
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
	};

public:
	SetImageClass();
	~SetImageClass();

	bool Initialize(ID3D11Device* device, HWND hwnd, string directory, string path, XMFLOAT3 position, float size);
	bool InitializeBuffer(ID3D11Device* dev);

	bool Render(ID3D11DeviceContext* deviceContext);
	XMMATRIX GetPosMatrix();
	void Close();

private:
	ID3D11Buffer* VertexBuffer = 0;
	ID3D11Buffer* IndexBuffer = 0;
	Vertex vertices[6];
	UINT indices[6];
	XMMATRIX m_world;
	ID3D11ShaderResourceView* m_texture = nullptr;
};


#endif