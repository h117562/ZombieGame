#include "ButtonClass.h"

ButtonClass::ButtonClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_touch = 0;
}

ButtonClass::~ButtonClass()
{

}

bool ButtonClass::Initialize(ID3D11Device* device, float x, float y, float w, float h, XMFLOAT4 color)
{
	bool result;


	// Load the rendering buffers with the terrain data.
	result = InitializeBuffers(device, x, y, w, h, color);
	if (!result)
	{
		return false;
	}

	m_touch = new TouchClass;
	if (!m_touch)
	{
		return false;
	}

	result = m_touch->Initialize(x, y, w, h);
	if (!result)
	{
		return false;
	}

	return true;
}



void ButtonClass::Shutdown()
{
	// Release the rendering buffers.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (m_touch)
	{
		delete m_touch;
		m_touch = 0;
	}

	return;
}


bool ButtonClass::Render(ID3D11DeviceContext* deviceContext, ShaderManagerClass* ShaderManager,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	RenderBuffers(deviceContext);
	
	result = ShaderManager->RenderColorShader(deviceContext, m_indexCount, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	
	

	return true;
}

bool ButtonClass::TouchEvent(XMVECTOR ray_o, XMVECTOR ray_d)
{
	return m_touch->Intersect_11(ray_o, ray_d);
}


int ButtonClass::GetIndexCount()
{
	return m_indexCount;
}


bool ButtonClass::InitializeBuffers(ID3D11Device* device, float x, float y, float w, float h, XMFLOAT4 color)
{
	Vertex* vertices;
	UINT* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	float positionX, positionZ, width, height;

	m_vertexCount = 8;
	m_indexCount = 8;

	vertices = new Vertex[8];
	if (!vertices)
	{
		return false;
	}

	indices = new UINT[8];
	if (!indices)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3((x + w / 2), (y + h / 2), 0.0f);
	vertices[1].position = XMFLOAT3((x + w / 2), (y - h / 2), 0.0f);

	vertices[2].position = XMFLOAT3((x + w / 2), (y - h / 2), 0.0f);
	vertices[3].position = XMFLOAT3((x - w / 2), (y - h / 2), 0.0f);

	vertices[4].position = XMFLOAT3((x - w / 2), (y - h / 2), 0.0f);
	vertices[5].position = XMFLOAT3((x - w / 2), (y + h / 2), 0.0f);

	vertices[6].position = XMFLOAT3((x - w / 2), (y + h / 2), 0.0f);
	vertices[7].position = XMFLOAT3((x + w / 2), (y + h / 2), 0.0f);




	for (int i = 0; i < 8; i++)
	{
		vertices[i].color = color;
	}

	for (int i = 0; i < 8; i++)
	{
		indices[i] = i;
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;


	return true;
}





void ButtonClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case lines.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	return;
}