#include "Animation.h"

using namespace std;

Animation::Animation() {
	m_name = L"none";			//�̸�
	tickPerSecond = 1.0f;		//tick ����
	speed = 1.0f;				//�ִϸ��̼� �ӵ�
	duration = 0.0f;			//�ִϸ��̼� ����
	playTime = 0.0f;			//���� ������
	lastTime = 0.0f;
	repeat = true;
}

Animation::~Animation() {

}

bool Animation::Initialize()
{
	//�ִϸ��̼� Ű������ �����Ͽ� 0.1�� �������� �迭�� ����

	XMVECTOR Z = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	
	float t = 0.0f;
	while (t <= lastTime)
	{

		for (AniNode& node : m_aniNode) {
			vector<KeyFrame> key = node.keyFrame;

			if (node.keyFrame.back().timePos <= t) {
				XMVECTOR T = XMLoadFloat3(&node.keyFrame.back().trans);
				XMVECTOR S = XMLoadFloat3(&node.keyFrame.back().scale);
				XMVECTOR R = XMLoadFloat4(&node.keyFrame.back().rotation);

				node.aniTEST.push_back(XMMatrixAffineTransformation(S, Z, R, T));
			}
			else {
				for (UINT i = 0; i < node.keyFrame.size() - 1; i++) {
					

					if (t >= key[i].timePos && t <= key[i + 1].timePos) {
						float lerpRate = (t - key[i].timePos) / (key[i + 1].timePos - key[i].timePos);

						XMVECTOR s1 = XMLoadFloat3(&key[i].scale);
						XMVECTOR s2 = XMLoadFloat3(&key[i + 1].scale);

						XMVECTOR t1 = XMLoadFloat3(&key[i].trans);
						XMVECTOR t2 = XMLoadFloat3(&key[i + 1].trans);

						XMVECTOR r1 = XMLoadFloat4(&key[i].rotation);
						XMVECTOR r2 = XMLoadFloat4(&key[i + 1].rotation);

						//�� ������ ���� ���� ����
						XMVECTOR T = XMVectorLerp(t1, t2, lerpRate);
						XMVECTOR S = XMVectorLerp(s1, s2, lerpRate);
						XMVECTOR R = XMQuaternionSlerp(r1, r2, lerpRate);

						node.aniTEST.push_back(XMMatrixAffineTransformation(S, Z, R, T));
						
						break;
					}

				}
			}
		}
		t += 0.1f;
	}

	return true;
}

bool Animation::UpdateAnimation(float frametime, float& entitytime)
{
	//����ð� ������Ʈ

	playTime = entitytime;

	entitytime += fmodf(frametime * tickPerSecond, duration) * speed;
	
	if (repeat)
	{
		if (entitytime > lastTime)
		{
			entitytime = 0.0f;
		}
	}
	
	
	return true;
}

vector<AniNode>& Animation::GetAniNodeList()
{
	//�ִϸ��̼� ��� ����Ʈ ��ȯ
	return m_aniNode;
}

wstring	Animation::GetName() {
	//�ִϸ��̼� �̸� ��ȯ
	return m_name;
}

void Animation::SetRepeat(bool s) 
{
	repeat = s;

	return;
}

void Animation::close() 
{
	//�迭 �޸� ��ȯ
	m_aniNode.clear();
	vector<AniNode>().swap(m_aniNode);
}

void Animation::SetPlayTime(float t)
{
	//�ִϸ��̼� ���� ����ð� �ʱ�ȭ
	playTime = t;
}

void Animation::SetDuration(float t) 
{
	//�ִϸ��̼� ���� �ʱ�ȭ
	duration = t;
}

void Animation::SetSpeed(float t)
{
	//�ִϸ��̼� �ӵ� �ʱ�ȭ
	speed = t;
}

void Animation::SetTick(float t)
{
	//�ִϸ��̼� ������ �ʱ�ȭ
	tickPerSecond = t;
}

void Animation::SetLast(float t)
{
	//�ִϸ��̼� ������ ������ �ʱ�ȭ
	lastTime = t;
}

void Animation::SetName(string s) 
{
	//�ִϸ��̼� �̸� �ʱ�ȭ
	m_name = wstring(s.begin(), s.end());
}



void Animation::GetAniTM(string nodeName, XMMATRIX& tm)
{
	int cut = playTime * 10;

	//���� ��� �ð��� ���� ��� ���� ��ȯ 
	for (AniNode& node : m_aniNode)
	{
		if (node.name == nodeName) {

			if (node.aniTEST.size() <= cut)
			{
				tm = node.aniTEST.back();
			}
			else
			{
				tm = node.aniTEST[cut];
			}
		
			break;
		}
	}

}
