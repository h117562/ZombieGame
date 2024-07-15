#include "Loader.h"

Loader::Loader()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_D3D11device = 0;
	m_size = 0.0f;
	beforeflag = 0;
	directory = "";
}


Loader::~Loader()
{
}

bool Loader::Load(ID3D11Device* device, ID3D11DeviceContext* devcon, string direct, string filename, float size, float speed, bool repeat)
{
	Assimp::Importer importer;

	string tmp = "";
	tmp = direct + '\\' + filename;

	const aiScene* pScene = importer.ReadFile(tmp,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (pScene == NULL)
		return false;

	directory = tmp.substr(0, tmp.find_last_of('\\'));
	m_D3D11device = device;
	m_size = size;

	processNode(pScene->mRootNode, pScene);
	
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = pScene->mMeshes[i];

		processSkin(aiMesh, i);
	}

	if (pScene->HasAnimations()) {
		processAni(pScene, speed, repeat);
	}
	
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Done();
	}

	return true;
}

bool Loader::Node(aiNode* node, const aiScene* scene)
{


	return true;
}

Mesh Loader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<UINT> indices;
	Texture textures;


	// Walk through each of the mesh's vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex verti;

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

	//인덱스 정보 가져오기
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


		//LPCWSTR cstr = w_path.c_str(); 생략
		D3DX11CreateShaderResourceViewFromFile(m_D3D11device, w_path.c_str(), 0, 0, &textures.data, 0);
	}
	
	return Mesh(m_D3D11device, vertices, indices, textures);
}


bool Loader::processNode(aiNode* node, const aiScene* scene, NodeInfo* parent)
{
	
	string temp_str = node->mName.C_Str();
	XMMATRIX tm = XMMatrixTranspose(XMMATRIX(node->mTransformation[0]));
	NodeInfo* nd = new NodeInfo(parent, temp_str, tm);
	nodes.emplace_back(nd);

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, nd);
	}

	return true;
}


bool Loader::processAni(const aiScene* scene, float speed, bool repeat) {
	aiNodeAnim* aiAniNode = 0;
	aiAnimation* aiAni = 0;
	Animation aniInfo;

	//애니메이션의 갯수 만큼...
	for (UINT i = 0; i < scene->mNumAnimations; i++) {

		aiAni = scene->mAnimations[i];

		float lastTime = 0.f;
		
		aniInfo.SetSpeed(speed);
		aniInfo.SetDuration(aiAni->mDuration);
		aniInfo.SetTick((float)aiAni->mTicksPerSecond);
		aniInfo.SetName((string)aiAni->mName.C_Str());

		//연결된 노드 만큼...
		for (UINT j = 0; j < aiAni->mNumChannels; j++) {
			aiAniNode = aiAni->mChannels[j];

			AniNode aniNodeInfo;

			aniNodeInfo.name = aiAniNode->mNodeName.C_Str();

			UINT keyCnt = max(aiAniNode->mNumPositionKeys, aiAniNode->mNumRotationKeys);
			keyCnt = max(keyCnt, aiAniNode->mNumScalingKeys);


			//키 프레임 정보를 구성한다.
			XMFLOAT3 translation = XMFLOAT3(0.f, 0.f, 0.f);
			XMFLOAT3 scale = XMFLOAT3(0.f, 0.f, 0.f);
			XMFLOAT4 rotation = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
			float	 time = 0.f;

			//키 값은 있을 수도 있으며, 없을수도 있다.
			//없는 경우 이전 값으로 채운다.
			for (UINT k = 0; k < keyCnt; k++) {
				if (aiAniNode->mNumPositionKeys > k) {
					auto posKey = aiAniNode->mPositionKeys[k];
					memcpy_s(&translation, sizeof(translation), &posKey.mValue, sizeof(posKey.mValue));
					time = (float)aiAniNode->mPositionKeys[k].mTime;
				}

				if (aiAniNode->mNumRotationKeys > k) {
					auto rotKey = aiAniNode->mRotationKeys[k];
					rotation = XMFLOAT4(rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z, rotKey.mValue.w);
					time = (float)aiAniNode->mRotationKeys[k].mTime;
				}

				if (aiAniNode->mNumScalingKeys > k) {
					auto scaleKey = aiAniNode->mScalingKeys[k];
					memcpy_s(&scale, sizeof(scale), &scaleKey.mValue, sizeof(scaleKey.mValue));
					time = (float)aiAniNode->mScalingKeys[k].mTime;
				}

				aniNodeInfo.keyFrame.emplace_back(KeyFrame{ time, translation, rotation, scale });
			}

			lastTime = max(aniNodeInfo.keyFrame.back().timePos, lastTime);
			aniInfo.GetAniNodeList().emplace_back(aniNodeInfo);
		}

		aniInfo.SetRepeat(repeat);
		aniInfo.SetLast(lastTime);
		animes.push_back(aniInfo);
		animes.back().Initialize();
	}

	return true;
}

bool Loader::processSkin(aiMesh* mesh, int num)
{

	if (mesh->HasBones())
	{
		vector<BoneInfo> bonelist;

		for (int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];

			BoneInfo vone;
			vone.matOffset = XMMatrixTranspose(XMMATRIX(bone->mOffsetMatrix[0]));

			//본정보와 노드정보를 일치시킨다.
			for (int m = 0; m < nodes.size(); m++)
			{
				if (nodes[m]->name == bone->mName.C_Str()) {
					vone.linkNode = nodes[m];
				}
			}

			bonelist.push_back(vone);

			for (int j = 0; j < bone->mNumWeights; j++)
			{
				UINT VertexId = bone->mWeights[j].mVertexId;
				float Weight = bone->mWeights[j].mWeight;


				if (meshes[num].vertices[VertexId].weight.x == 0) {
					meshes[num].vertices[VertexId].boneid.x = i;
					meshes[num].vertices[VertexId].weight.x = Weight;
				}
				else if (meshes[num].vertices[VertexId].weight.y == 0) {
					meshes[num].vertices[VertexId].boneid.y = i;
					meshes[num].vertices[VertexId].weight.y = Weight;
				}
				else if (meshes[num].vertices[VertexId].weight.z == 0) {
					meshes[num].vertices[VertexId].boneid.z = i;
					meshes[num].vertices[VertexId].weight.z = Weight;
				}
				else if (meshes[num].vertices[VertexId].weight.w == 0) {
					meshes[num].vertices[VertexId].boneid.w = i;
					meshes[num].vertices[VertexId].weight.w = Weight;
				}

			}
		}
		bones.push_back(bonelist);
	}

	return true;
}

bool Loader::AddAni(HWND hwnd, string filename, float speed, bool repeat)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(directory + '\\' +filename,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (pScene == NULL) 
	{
		wstring m_error = wstring(filename.begin(), filename.end());
		MessageBox(hwnd, m_error.c_str(), L"Adding Animation Failed", MB_OK);
		return false;
	}
		

	if (pScene->HasAnimations()) {
		processAni(pScene, speed, repeat);
	}



	return true;
}

void Loader::Close()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Close();
	}
	meshes.clear();
	vector<Mesh>().swap(meshes);

	nodes.clear();
	vector<NodeInfo*>().swap(nodes);

	bones.clear();
	vector<vector<BoneInfo>>().swap(bones);

	for (int i = 0; i < animes.size(); i++)
	{
		animes[i].close();
	}
	

}


void Loader::UpdateNodeTm(UINT flag, float frame, float &time)
{
	
	if (animes.size() > flag)
	{
		if (!(beforeflag == flag)) {//애니메이션 전환시 재생시간 초기화
			animes[flag].SetPlayTime(0.0f);
		}

		animes[flag].UpdateAnimation(frame, time);

		beforeflag = flag;
	}
	


	for (NodeInfo* node : nodes)
	{
		XMMATRIX tm = node->localTM;

		animes[flag].GetAniTM(node->name, tm);

		node->worldTM = tm;
		if (node->parent)
		{
			node->worldTM = tm * node->parent->worldTM;
		}
	}
}

vector<XMMATRIX> Loader::GetBoneList(vector<XMMATRIX> bonelist, int num)
{

	for (int i = 0; i < bones[num].size(); i++)
	{
		bonelist.push_back(XMMatrixTranspose(bones[num][i].matOffset * bones[num][i].linkNode->worldTM));
	}

	return bonelist;
}

void Loader::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	return;
}

void Loader::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;

	return;
}

XMMATRIX Loader::GetPosition()
{
	return XMMatrixRotationY(((m_rotation.y - 180) / 180) * 3.14) * XMMatrixScaling(m_size, m_size, m_size) * XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
}

void Loader::Render(ID3D11DeviceContext* deviceContext, UINT flag, float frame, float& starttime)
{

	UpdateNodeTm(flag,frame, starttime);
	
	for (int i = 0; i < meshes.size(); i++)
	{
		vector<XMMATRIX> bonelist;
		meshes[i].SetShaderParameters(deviceContext, GetBoneList(bonelist, i));
		meshes[i].Draw(deviceContext);
	}
}


void Loader::Render(ID3D11DeviceContext* deviceContext)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(deviceContext);
	}
}
