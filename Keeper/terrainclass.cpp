////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"


TerrainClass::TerrainClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


TerrainClass::TerrainClass(const TerrainClass& other)
{
}


TerrainClass::~TerrainClass()
{
}


bool TerrainClass::Initialize(ID3D11Device* device)
{
	bool result;


	// Load the rendering buffers with the terrain data.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}


void TerrainClass::Shutdown()
{
	// Release the rendering buffers.
	ShutdownBuffers();

	return;
}


bool TerrainClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}


int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}


bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i, j, terrainWidth, terrainHeight, index;
	XMFLOAT4 color;
	float positionX, positionZ;


	// Set the height and width of the terrain grid.
	terrainHeight = 256;
	terrainWidth = 256;

	// Set the color of the terrain grid.
	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Calculate the number of vertices in the terrain.
	m_vertexCount = (terrainWidth - 1) * (terrainHeight - 1) * 8;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize the index into the vertex and index arrays.
	index = 0;

	// Load the vertex array and index array with data.
	for (j = 0; j < (terrainHeight - 1); j++)
	{
		for (i = 0; i < (terrainWidth - 1); i++)
		{
			// Line 1 - Upper left.
			positionX = (float)i;
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 1 - Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 2 - Upper right
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 2 - Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 3 - Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 3 - Bottom left.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 4 - Bottom left.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;

			// Line 4 - Upper left.
			positionX = (float)i;
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = color;
			indices[index] = index;
			index++;
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
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


void TerrainClass::ShutdownBuffers()
{
	// Release the index buffer.
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

	return;
}


void TerrainClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case lines.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}