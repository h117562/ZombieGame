#include "ObjectLoad.h"

ObjectLoad::ObjectLoad()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = 0.0f;
	m_size = 1.0f;
	m_D3D11device = 0;
	directory = "";
	skeleton_render = false;
}


ObjectLoad::~ObjectLoad()
{
}

bool ObjectLoad::Load(ID3D11Device* device, ID3D11DeviceContext* devcon, string direct, string filename, float size)
{
	Assimp::Importer importer;

	string tmp = "";
	tmp = direct + '\\' + filename;

	const aiScene* pScene = importer.ReadFile(tmp,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
		);

	if (pScene == NULL)
		return false;

	directory = tmp.substr(0, tmp.find_last_of('\\'));
	m_D3D11device = device;
	m_size = size;

	processNode(pScene->mRootNode, pScene);
	
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = pScene->mMeshes[i];
	}

	return true;
}

ObjectMesh ObjectLoad::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<VertexTest> vertices;
	vector<UINT> indices;
	TextureTest textures;


	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VertexTest verti;

		verti.position.x = mesh->mVertices[i].x;
		verti.position.y = mesh->mVertices[i].y;
		verti.position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			verti.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			verti.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.emplace_back(verti);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.emplace_back(face.mIndices[j]);
	}


	if (mesh->mMaterialIndex >= 0)
	{
		aiString path;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL);
		
		string str = path.C_Str();
		str = str.substr(str.find_last_of("\\") + 1, str.length());
		str = directory + "\\" + str;
		wstring w_path = wstring(str.begin(), str.end());
		textures.name = w_path;

		//LPCWSTR cstr = w_path.c_str(); »ý·«
		D3DX11CreateShaderResourceViewFromFile(m_D3D11device, w_path.c_str(), 0, 0, &textures.data, 0);
	}

	
	
	return ObjectMesh(m_D3D11device, vertices, indices, textures);
}


bool ObjectLoad::processNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

	return true;
}

void ObjectLoad::Close()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Close();
	}

	meshes.clear();
	vector<ObjectMesh>().swap(meshes);

	return;
}

void ObjectLoad::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	return;
}

void ObjectLoad::SetRotation(float r)
{
	m_rotation = r;

	return;
}

void ObjectLoad::SetSkeletonRender(bool b)
{
	skeleton_render = b;

	return;
}

XMMATRIX ObjectLoad::GetPosition()
{
	return XMMatrixRotationX(m_rotation)*XMMatrixScaling(m_size, m_size, m_size)*XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}

void ObjectLoad::Render(ID3D11DeviceContext* deviceContext)
{
	if (skeleton_render)
	{
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	else
	{
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Render(deviceContext);
	}
}

bool ObjectLoad::ObjectIntersect(XMFLOAT3& hitted_pos, XMVECTOR origin, XMVECTOR direction, float size)
{
	bool result;

	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j< meshes[i].m_indices.size(); j++)
		{
			XMVECTOR temp_vert[3];
			float dist;
			temp_vert[0] = XMVectorSet(
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.x,
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.y,
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.z,
				0.0f
				)* size;
			j++;
			temp_vert[1] = XMVectorSet(
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.x,
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.y,
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.z,
				0.0f
				) * size;
			j++;
			temp_vert[2] = XMVectorSet(
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.x,
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.y,
				meshes[i].m_vertices[meshes[i].m_indices[j]].position.z,
				0.0f
				) * size;

			result = TriangleTests::Intersects(origin, direction, temp_vert[0], temp_vert[1], temp_vert[2], dist);

			if (result)
			{
				XMVECTOR sum = (temp_vert[0] + temp_vert[1] + temp_vert[2]) / 3;
				XMStoreFloat3(&hitted_pos, sum);
				
				return true;
			}

		}
	}

	return false;
}
