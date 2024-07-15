#ifndef _OBJECT_MESH_H_
#define _OBJECT_MESH_H_

#include <string>
#include <vector>
#include <d3dx11.h>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

struct VertexTest
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
};

struct TextureTest {
	wstring name = L"";
	ID3D11ShaderResourceView* data = NULL;
};

class ObjectMesh
{
public:
	vector<VertexTest> m_vertices;
	vector<UINT> m_indices;
	TextureTest m_texture;

	ObjectMesh(ID3D11Device* dev, vector<VertexTest> vertices, vector<UINT> indices, TextureTest texture)
	{
		this->m_vertices = vertices;
		this->m_indices = indices;
		this->m_texture = texture;

		this->InitializeBuffer(dev);
	}

	void Render(ID3D11DeviceContext* devcon)
	{
		UINT stride = sizeof(VertexTest);
		UINT offset = 0;
		ID3D11ShaderResourceView* nullview = nullptr;


		devcon->PSSetShaderResources(0, 1, &m_texture.data);

		if (!m_texture.data)
		{
			devcon->PSSetShaderResources(0, 1, &nullview);
		}//텍스쳐가 없을 경우 초기화


		devcon->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
		devcon->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		devcon->DrawIndexed(m_indices.size(), 0, 0);

	}

	void Close()
	{
		VertexBuffer->Release();
		IndexBuffer->Release();

		m_vertices.clear();
		vector<VertexTest>().swap(m_vertices);
		m_indices.clear();
		vector<UINT>().swap(m_indices);
	}

private:
	ID3D11Buffer* VertexBuffer = 0;
	ID3D11Buffer* IndexBuffer = 0;

	bool InitializeBuffer(ID3D11Device* dev)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(VertexTest) * m_vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &m_vertices[0];

		hr = dev->CreateBuffer(&vbd, &initData, &VertexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(UINT) * m_indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		initData.pSysMem = &m_indices[0];

		hr = dev->CreateBuffer(&ibd, &initData, &IndexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}
};
#endif