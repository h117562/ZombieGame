#include "EffectImageClass.h"


EffectImageClass::EffectImageClass()
{
	vertices[0].position = XMFLOAT3(-10.0f, 10.0f, 0.0f);
	vertices[1].position = XMFLOAT3(10.0f, -10.0f, 0.0f);
	vertices[2].position = XMFLOAT3(-10.0f, -10.0f, 0.0f);
	vertices[3].position = XMFLOAT3(-10.0f, 10.0f, 0.0f);
	vertices[4].position = XMFLOAT3(10.0f, 10.0f, 0.0f);
	vertices[5].position = XMFLOAT3(10.0f, -10.0f, 0.0f);

	vertices[0].texcoord = XMFLOAT2(0.0f, 0.0f);
	vertices[1].texcoord = XMFLOAT2(1.0f, 1.0f);
	vertices[2].texcoord = XMFLOAT2(0.0f, 1.0f);
	vertices[3].texcoord = XMFLOAT2(0.0f, 0.0f);
	vertices[4].texcoord = XMFLOAT2(1.0f, 0.0f);
	vertices[5].texcoord = XMFLOAT2(1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

	m_target = 0;
	size = 1;
	speed = 1;
}


EffectImageClass::~EffectImageClass()
{

}


bool EffectImageClass::Initialize(ID3D11Device* dev, HWND hwnd, string directory, string path[], int count, float size)
{
	bool result;
	ID3D11ShaderResourceView* temp_texture = 0;
	

	for (int i = 0; i < count; i++)
	{
		if (path[i] != "")
		{
			string sum_direct = directory + "\\" + path[i];
			wstring w_path = wstring(sum_direct.begin(), sum_direct.end());

			D3DX11CreateShaderResourceViewFromFile(dev, w_path.c_str(), 0, 0, &temp_texture, 0);
			if (!temp_texture)
			{
				MessageBox(hwnd, L"Effect Lost", w_path.c_str(), MB_OK);
				return false;
			}

			m_texture.push_back(temp_texture);
		}
	}
	

	result = InitializeBuffer(dev);
	if (!result)
	{
		return false;
	}

	return true;
}


bool EffectImageClass::InitializeBuffer(ID3D11Device* dev)
{
	HRESULT hr;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices;

	hr = dev->CreateBuffer(&vbd, &initData, &VertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	initData.pSysMem = &indices;

	hr = dev->CreateBuffer(&ibd, &initData, &IndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}


	return true;
}


bool EffectImageClass::Render(ID3D11DeviceContext* deviceContext, float PlayTime)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	if (PlayTime >= m_texture.size())
	{
		deviceContext->PSSetShaderResources(0, 1, &m_texture[m_texture.size()-1]);
	}
	else
	{
		deviceContext->PSSetShaderResources(0, 1, &m_texture[PlayTime]);
	}
	
	deviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	return true;
}

void EffectImageClass::Close()
{
	VertexBuffer->Release();
	IndexBuffer->Release();

	m_texture.clear();
	vector<ID3D11ShaderResourceView*>().swap(m_texture);

	return;
}


XMMATRIX EffectImageClass::GetPosMatrix()
{
	return XMMatrixScaling(size, size, size) * XMMatrixIdentity();
}