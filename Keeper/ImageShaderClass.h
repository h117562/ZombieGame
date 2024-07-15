////////////////////////////////////////////////////////////////////////////////
// Filename: ImageClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _IMAGE_SHADER_CLASS_H_
#define _IMAGE_SHADER_CLASS_H_


#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

using namespace DirectX;
using namespace std;


class ImageShaderClass {
private:
	struct MatrixBuffer {
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
	};

	struct DiffuseBuffer
	{
		XMFLOAT4 diffuse_color;
	};

public:
	ImageShaderClass();
	~ImageShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	bool InitializeData(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 color);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4);

	void Shutdown();

private:
	ID3D11ShaderResourceView* m_texture = nullptr;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer = 0;
	ID3D11Buffer* m_diffuseBuffer = 0;
	ID3D11SamplerState* TexSamplerState;
};

#endif