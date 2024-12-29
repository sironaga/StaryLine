#include "Fade.h"

CFade::CFade()
	: m_isFadeIn(false)
	,m_time(0.0f), m_maxTime(0.0f)
{

}

CFade::~CFade()
{
}

void CFade::Update()
{
	if (m_time > 0.0f) {  // 毎フレーム判定 
		m_time -= 1.0f / 60.0f;
		if (m_time < 0.0f)  // フェードが終了すると呼び出されなくなるので、少しだけ効率がよい 
			m_time = 0.0f;
	}
}
void CFade::Draw()
{
	if (IsFadeIn()) DrawFadeIn(GetAlpha());
	else DrawFadeOut(GetAlpha());
}

void CFade::Start(bool isIn)
{
	m_isFadeIn = isIn;
	m_time = m_maxTime;
}

void CFade::SetFade(float time, bool isIn)
{
	m_isFadeIn = isIn;
	m_time = time;
	m_maxTime = time;
}

float CFade::GetRate()
{
	if (m_maxTime == 0.0f) return 0.0f; // 0除算回避 
	return m_time / m_maxTime;
}

float CFade::GetAlpha()
{
	float alpha = GetRate();
	if (IsFadeOut()) alpha = 1.0f - alpha;
	return alpha;
}

void CFade::DrawFadeIn(float alpha)
{
}

void CFade::DrawFadeOut(float alpha)
{
}
