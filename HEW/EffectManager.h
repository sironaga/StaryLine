#pragma once

#include "LibEffekseer.h"
#include <vector>

#define MAX_EFFECT 20

class CEffectManager
{
public:
	CEffectManager(const char* efc);

	~CEffectManager();
	void Uninit();
	void Update();
	void Draw();
	void Play(DirectX::XMFLOAT3 pos, int PlayTime);
	void Play(DirectX::XMFLOAT3 pos);
	void Stop();
	void AllStop();

	void SetPos(DirectX::XMFLOAT3 pos);
	int GetEffectNum();
private:
	Effekseer::ManagerRef m_Manager;
	Effekseer::EffectRef m_Effect;
	Effekseer::ManagerRef* m_pManager;
	Effekseer::EffectRef* m_pEffect;
	CEffectManager* m_pEffectAddrres;
	Effekseer::Handle m_Handle;
	struct Effect 
	{
		Effekseer::Handle m_Handle;
		int startTime;
		int duration; // ƒŠƒ~ƒbƒgŽžŠÔ
	}m_efc;
	std::vector<Effect> m_effects;
	int m_nEffectNum;
	int m_nTime;
};