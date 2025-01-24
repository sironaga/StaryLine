#pragma once

#include "Camera.h"

class CCameraEvent : public Camera
{
public:
	CCameraEvent();
	virtual ~CCameraEvent();
	virtual void Update()override;
	enum EASE_KIND
	{
		InEasing = 0,

		EASE_MAX
	};
	void SetEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time, EASE_KIND kind);
	bool IsEvent();
private:
	DirectX::XMFLOAT3 m_start;
	DirectX::XMFLOAT3 m_end;
	float m_time;
	float m_totalTime;
};