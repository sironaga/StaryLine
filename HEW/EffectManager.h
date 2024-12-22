#pragma once

#include "LibEffekseer.h"
#include <vector>

#define MAX_EFFECT 20

class CEffectManager
{
public:
	CEffectManager(const char* efc);
	CEffectManager(CEffectManager* InAddrres);
	CEffectManager(CEffectManager* InAddrres, DirectX::XMFLOAT3 pos, int PlayTime);
	~CEffectManager();
	void Uninit();
	void Update();
	void Draw();
	void Play(DirectX::XMFLOAT3 pos, int PlayTime);
	void AllStop();

	void SetPos(DirectX::XMFLOAT3 pos);
	int GetEffectNum();
	Effekseer::ManagerRef GetManager();
	Effekseer::EffectRef GetEffect();
	Effekseer::Handle GetHandle();
private:
	Effekseer::ManagerRef m_Manager;
	Effekseer::EffectRef m_Effect;
	CEffectManager* m_pEffectAddrres;
	Effekseer::Handle m_Handle;
	struct Effect 
	{
		Effekseer::Handle m_Handle;
		int startTime;
		int duration; // ƒŠƒ~ƒbƒgŽžŠÔ
	};
	std::vector<Effect> m_effects;
	int m_nEffectNum;
	int m_nTime;
};