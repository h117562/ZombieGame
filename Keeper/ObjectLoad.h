#include <vector>
#include <d3dx11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "ObjectMesh.h"

using namespace std;
using namespace DirectX;

class ObjectLoad
{
public:
	ObjectLoad();
	~ObjectLoad();

	bool Load(ID3D11Device*, ID3D11DeviceContext*, string directory, string filename, float modelsize);
	void Render(ID3D11DeviceContext*);

	void SetPosition(float x, float y, float z);
	void SetRotation(float);
	void SetSkeletonRender(bool);
	bool ObjectIntersect(XMFLOAT3& hitted_pos, XMVECTOR origin, XMVECTOR direction, float size);
	XMMATRIX GetPosition();
	
	void Close();
	
private:
	vector<ObjectMesh> meshes;
	string directory;
	bool processNode(aiNode*, const aiScene*);
	ObjectMesh processMesh(aiMesh*, const aiScene*);

private:
	XMFLOAT3 m_position;
	float m_rotation;
	ID3D11Device* m_D3D11device;
	float m_size;
	bool skeleton_render;
};
