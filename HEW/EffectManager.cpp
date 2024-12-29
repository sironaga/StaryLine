#include "EffectManager.h"
#include "Main.h"
#include "time.h"

CEffectManager::CEffectManager(const char* efc)
	:m_nEffectNum(0), m_pEffectAddrres(nullptr), m_nTime(0)
	,m_efc{},m_Handle()
{
	m_effects.clear();
	m_Effect = LibEffekseer::Create(efc);
	m_pEffect = &m_Effect;
	m_Manager = LibEffekseer::GetManager();
	m_pManager = &m_Manager;
}



CEffectManager::~CEffectManager()
{
	m_pManager->Get()->StopAllEffects();
	m_effects.clear();
}

void CEffectManager::Uninit()
{

}

void CEffectManager::Update()
{
	for (auto itr = m_effects.begin(); itr != m_effects.end();)
	{
		int i = m_effects.size();
		itr->startTime++;
		if (m_pManager->Get()->Exists(itr->m_Handle))
		{
			if (itr->startTime >= itr->duration)
			{
				m_pManager->Get()->StopEffect(itr->m_Handle);
				(itr)->m_Handle = NULL;
				itr = m_effects.erase(itr);

				continue;
			}
		}
		++itr;
	}
}

void CEffectManager::Draw()
{
	LibEffekseer::SetViewPosition(GetCameraPos());			// カメラ座標のセット
	LibEffekseer::SetCameraMatrix(GetView(false), GetProj(false));	// view座標,projection座標のセット 
}

void CEffectManager::Play(DirectX::XMFLOAT3 pos, int PlayTime)
{
	m_efc.m_Handle = { m_pManager->Get()->Play(*m_pEffect, pos.x, pos.y, pos.z) };
	m_efc.startTime = 0;
	m_efc.duration = PlayTime;
	m_effects.push_back(m_efc);
}

void CEffectManager::Play(DirectX::XMFLOAT3 pos)
{
	m_Handle = m_Manager->Play(m_Effect, pos.x, pos.y, pos.z);
}

void CEffectManager::Stop()
{
	m_Manager->StopEffect(m_Handle);
	m_Handle = 0;
}

void CEffectManager::AllStop()
{
	m_Manager->StopAllEffects();
	m_effects.clear();
	m_nEffectNum = 0;
}

void CEffectManager::SetPos(DirectX::XMFLOAT3 pos)
{
	Effekseer::Matrix43 mat;
	Effekseer::Matrix43 baseMat;
	baseMat.Translation(0.0f,0.0f,0.0f);
	mat.Translation(pos.x, pos.y, pos.z);
	m_Manager->SetBaseMatrix(m_Handle,baseMat);
	m_Manager->SetMatrix(m_Handle, mat);
}

int CEffectManager::GetEffectNum()
{
	return m_nEffectNum;
}