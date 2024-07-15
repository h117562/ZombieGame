#include "Mesh.h"

struct KeyFrame {
	float		timePos = 0.f;
	XMFLOAT3	trans = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT4	rotation = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	XMFLOAT3	scale = XMFLOAT3(0.f, 0.f, 0.f);
};

//���� ����� �ִϸ��̼� ����
struct AniNode {
	vector<KeyFrame>	keyFrame;				//Ű������ ����Ʈ
	string				name = "none";			//����� �� �̸�

	XMMATRIX aniTM = XMMatrixIdentity();						//�ִϸ��̼� ���
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
	wstring	m_name;			//�̸�
	float tickPerSecond ;	//tick ����
	float speed;			//�ӵ�
	float duration;			//������ ����
	float playTime ;		//���� ������
	float lastTime ;		//������ ������
	bool repeat;
};