#include "CameraEvent.h"

CCameraEvent::CCameraEvent()
	: m_start{}, m_end{}
	, m_time(0.0f), m_totalTime(0.0f)
	, m_eKind(InEasing)
{
	m_pos.x = cosf(0.0f) * sinf(0.0f) * -90.0f + m_look.x;
	m_pos.y = sinf(0.0f) * -90.0f + m_look.y;
	m_pos.z = cosf(0.0f) * cosf(0.0f) * -90.0f + m_look.z;
}

CCameraEvent::~CCameraEvent()
{
}

void CCameraEvent::Update()
{
	if (!IsEvent()) { return; }

	float param = m_time / m_totalTime;

	switch (m_eKind)
	{
	case CCameraEvent::InEasing:
		break;
	case CCameraEvent::easeOutQuint:
		param = 1 - powf(1 - param, 5);
		break;
	case CCameraEvent::easeOutQuad:
		break;
	case CCameraEvent::EASE_MAX:
		break;
	default:
		break;
	}

	m_look.x = m_start.x * (1 - param) + m_end.x * param;
	m_look.y = m_start.y * (1 - param) + m_end.y * param;
	m_look.z = m_start.z * (1 - param) + m_end.z * param;

	m_pos.x = m_look.x + 0.0f;
	m_pos.y = m_look.y + 0.0f;
	m_pos.z = m_look.z + -90.0f;

	m_time += 1.0f / 60.0f;
}

void CCameraEvent::SetEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time, EASE_KIND kind)
{
	m_eKind = kind;
	m_start = startPos;
	m_end = endPos;
	m_time = 0.0f;
	m_totalTime = time;
}

bool CCameraEvent::IsEvent()
{
	return m_time < m_totalTime;
}