#include "Mesh.h"

struct KeyFrame {
	float		timePos = 0.f;
	XMFLOAT3	trans = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT4	rotation = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	XMFLOAT3	scale = XMFLOAT3(0.f, 0.f, 0.f);
};

//본과 열결된 애니메이션 정보
struct AniNode {
	vector<KeyFrame>	keyFrame;				//키프레임 리스트
	string				name = "none";			//연결된 본 이름

	XMMATRIX aniTM = XMMatrixIdentity();						//애니메이션 행렬
	vector<XMMATRIX> aniTEST;
};

class Animation {
public:

	Animation();
	~Animation();

	bool Initialize();
	bool UpdateAnimation(float, float& frame);
	void SetRepeat(bool);
	vector<AniNode>& GetAniNodeList();
	wstring	GetName();
	void GetAniTM(string , XMMATRIX&);

	void SetPlayTime(float t);
	void SetDuration(float);
	void SetSpeed(float);
	void SetTick(float);
	void SetLast(float);
	void SetName(string);

	void close();

private:

	vector<AniNode>	 m_aniNode;
	wstring	m_name;			//이름
	float tickPerSecond ;	//tick 정보
	float speed;			//속도
	float duration;			//프레임 길이
	float playTime ;		//현재 프레임
	float lastTime ;		//마지막 프레임
	bool repeat;
};