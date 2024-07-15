#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <d3dx11.h>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;


	struct NodeInfo 
	{
		string name = "none";
		NodeInfo* parent = NULL;


		XMMATRIX localTM = XMMatrixIdentity();
		XMMATRIX worldTM = XMMatrixIdentity();

		NodeInfo(NodeInfo* parent, string& name, XMMATRIX& tm)
		{
			this->parent = parent;
			this->name = name;
			this->localTM = tm;
			this->worldTM = tm;

		//부모노드가 있으면 월드 행렬 변경
		if (parent)
			this->worldTM = localTM * parent->worldTM;
		}
	};

	struct MatBones
	{
		XMMATRIX matBones[128];
	};

	struct BoneInfo 
	{
		NodeInfo* linkNode = NULL;
		XMMATRIX matOffset = XMMatrixIdentity();
	};

	struct Texture 
	{
		wstring name = L"";
		ID3D11ShaderResourceView* data = NULL;
	};

	struct Vertex 
	{
		XMFLOAT3 position;
		XMFLOAT2 texcoord;
		XMUINT4 boneid= XMUINT4(0, 0, 0, 0);
		XMFLOAT4 weight= XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	};

	class Mesh 
	{
	public:
		vector<Vertex> vertices;
		vector<UINT> indices;
		
		Texture textures;
		ID3D11Device* dev;

		Mesh(ID3D11Device* dev, vector<Vertex> vertices, vector<UINT> indices, Texture textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;
			this->dev = dev;
			
		}

		void Done()
		{
			this->InitializeBuffer(dev);
		}

		void Draw(ID3D11DeviceContext* devcon)
		{
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			ID3D11ShaderResourceView* nullview = nullptr;
		

			devcon->PSSetShaderResources(0, 1, &textures.data);

			if (!textures.data) 
			{
				devcon->PSSetShaderResources(0, 1, &nullview);
			}//텍스쳐가 없을 경우 초기화

			devcon->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
			devcon->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			devcon->DrawIndexed(indices.size(), 0, 0);

		}

		bool SetShaderParameters(ID3D11DeviceContext* devcon, vector<XMMATRIX> matlist)
		{
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			MatBones* dataPtr = 0;

			result = devcon->Map(m_BoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			dataPtr = (MatBones*)mappedResource.pData;

			for (UINT i = 0; i < matlist.size(); i++)
			{
				dataPtr->matBones[i] = matlist[i];
			}

			devcon->Unmap(m_BoneBuffer, 0);

			devcon->VSSetConstantBuffers(1, 1, &m_BoneBuffer);

			return true;
		}

		void Close()
		{
			VertexBuffer->Release();
			IndexBuffer->Release();
			m_BoneBuffer->Release();

			vertices.clear();
			vector<Vertex>().swap(vertices);
			indices.clear();
			vector<UINT>().swap(indices);
		}

	private:
		ID3D11Buffer* VertexBuffer, * IndexBuffer, *m_BoneBuffer;

		/*  Functions    */
		// Initializes all the buffer objects/arrays
		bool InitializeBuffer(ID3D11Device* dev)
		{
			HRESULT hr;

			D3D11_BUFFER_DESC vbd;
			vbd.Usage = D3D11_USAGE_DEFAULT;
			vbd.ByteWidth = sizeof(Vertex) * vertices.size();
			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags = 0;
			vbd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = &vertices[0];

			hr = dev->CreateBuffer(&vbd, &initData, &VertexBuffer);
			if (FAILED(hr))
				return false;

			D3D11_BUFFER_DESC ibd;
			ibd.Usage = D3D11_USAGE_DEFAULT;
			ibd.ByteWidth = sizeof(UINT) * indices.size();
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;

			initData.pSysMem = &indices[0];

			hr = dev->CreateBuffer(&ibd, &initData, &IndexBuffer);
			if (FAILED(hr))
				return false;

			///////////////////////////////////////////////////

			D3D11_BUFFER_DESC bbd;

			bbd.Usage = D3D11_USAGE_DYNAMIC;
			bbd.ByteWidth = sizeof(MatBones);
			bbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bbd.MiscFlags = 0;
			bbd.StructureByteStride = 0;

			hr = dev->CreateBuffer(&bbd, nullptr, &m_BoneBuffer);
			if (FAILED(hr))
			{
				return false;
			}
		}
	};


	#endif