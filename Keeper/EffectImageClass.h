#ifndef _EFFECT_IMAGE_CLASS_
#define _EFFECT_IMAGE_CLASS_

#include <string>
#include <vector>
#include <d3dx11.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

class EffectImageClass
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
	EffectImageClass();
	~EffectImageClass();

	bool Initialize(ID3D11Device* device, HWND hwnd, string directory, string path[], int array_length, float size);
	bool InitializeBuffer(ID3D11Device* dev);

	bool Render(ID3D11DeviceContext* deviceContext, float PlayTime);
	XMMATRIX GetPosMatrix();
	void Close();

private:
	ID3D11Buffer* VertexBuffer = 0;
	ID3D11Buffer* IndexBuffer = 0;
	Vertex vertices[6];
	UINT indices[6];
	vector<ID3D11ShaderResourceView*> m_texture;
	XMFLOAT3* m_target;
	float size;
	float speed;
	
};


#endif