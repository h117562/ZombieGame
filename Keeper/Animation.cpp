#include "Animation.h"

using namespace std;

Animation::Animation() {
	m_name = L"none";			//이름
	tickPerSecond = 1.0f;		//tick 정보
	speed = 1.0f;				//애니메이션 속도
	duration = 0.0f;			//애니메이션 길이
	playTime = 0.0f;			//현재 프레임
	lastTime = 0.0f;
	repeat = true;
}

Animation::~Animation() {

}

bool Animation::Initialize()
{
	//애니메이션 키프레임 보간하여 0.1초 단위마다 배열에 저장

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

						//두 프레임 사이 정보 보간
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
	//재생시간 업데이트

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
	//애니메이션 노드 리스트 반환
	return m_aniNode;
}

wstring	Animation::GetName() {
	//애니메이션 이름 반환
	return m_name;
}

void Animation::SetRepeat(bool s) 
{
	repeat = s;

	return;
}

void Animation::close() 
{
	//배열 메모리 반환
	m_aniNode.clear();
	vector<AniNode>().swap(m_aniNode);
}

void Animation::SetPlayTime(float t)
{
	//애니메이션 현재 재생시간 초기화
	playTime = t;
}

void Animation::SetDuration(float t) 
{
	//애니메이션 길이 초기화
	duration = t;
}

void Animation::SetSpeed(float t)
{
	//애니메이션 속도 초기화
	speed = t;
}

void Animation::SetTick(float t)
{
	//애니메이션 프레임 초기화
	tickPerSecond = t;
}

void Animation::SetLast(float t)
{
	//애니메이션 마지막 프레임 초기화
	lastTime = t;
}

void Animation::SetName(string s) 
{
	//애니메이션 이름 초기화
	m_name = wstring(s.begin(), s.end());
}



void Animation::GetAniTM(string nodeName, XMMATRIX& tm)
{
	int cut = playTime * 10;

	//현재 재생 시간에 따라 노드 정보 반환 
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
