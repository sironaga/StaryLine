#include "CameraEvent.h"

CCameraEvent::CCameraEvent()
	: m_start{}, m_end{}
	, m_time(0.0f), m_totalTime(0.0f)
{
}

CCameraEvent::~CCameraEvent()
{
}

void CCameraEvent::Update()
{
	if (!IsEvent()) { return; }

	// ó·)èÌÇ…éËëO 
	// m_pos.x = m_look.x + 0.0f; 
	// m_pos.y = m_look.y + 0.0f; 
	// m_pos.z = m_look.z - 1.0f; 
	m_time += 1.0f / 60.0f;
}

void CCameraEvent::SetEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time, EASE_KIND kind)
{
	switch (kind)
	{
	case CCameraEvent::EASE_MAX:
		break;
	default:
		break;
	}

	m_start = startPos;
	m_end = endPos;
	m_time = 0.0f;
	m_totalTime = time;
}

bool CCameraEvent::IsEvent()
{
	return m_time < m_totalTime;
}