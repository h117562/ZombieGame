#include <vector>
#include <d3dx11.h>
#include <DirectXMath.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Animation.h"
#include "Mesh.h"

using namespace std;
using namespace DirectX;


class Loader
{
public:
	Loader();
	~Loader();

	bool Load(ID3D11Device*, ID3D11DeviceContext*,string directory, string filename, float modelsize , float animationSpeed, bool repeat);
	void Render(ID3D11DeviceContext*, UINT, float, float&);
	void Render(ID3D11DeviceContext*);
	bool AddAni(HWND, string filename, float speed, bool repeat);
	
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);


	XMMATRIX GetPosition();
	vector<XMMATRIX> GetBoneList(vector<XMMATRIX> , int);
	void Close();
	vector<Mesh> meshes;
private:
	
	vector<Animation> animes;
	vector<vector<BoneInfo>> bones;
	vector<NodeInfo*> nodes;

	string directory;
	bool processNode(aiNode*, const aiScene*, NodeInfo* = NULL);
	Mesh processMesh(aiMesh*, const aiScene*);
	bool processSkin(aiMesh*, int);
	bool processAni(const aiScene*, float, bool);
	bool Node(aiNode*, const aiScene*);
	void UpdateNodeTm(UINT aniflag, float, float&);

	void FindNode(UINT, int);
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	float m_size;
	UINT beforeflag;
	ID3D11Device* m_D3D11device;
};
