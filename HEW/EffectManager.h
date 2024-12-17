#pragma once

#include "LibEffekseer.h"
#include <vector>

class CEffectManager
{
public:
	CEffectManager(const char* efc);
	~CEffectManager();
	void Update();
	void Draw();
	void Play(DirectX::XMFLOAT3 pos,int PlayTime);
	Effekseer::ManagerRef GetManager();
private:
	Effekseer::ManagerRef m_Manager;
	Effekseer::EffectRef m_Effect;
	int m_nLimit;
	std::vector<int> m_nPlayTime;
	std::vector<Effekseer::Handle> m_Handle;
};