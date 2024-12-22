#include "EffectManager.h"
#include "Main.h"
#include "time.h"

CEffectManager::CEffectManager(const char* efc)
	:m_nEffectNum(0), m_pEffectAddrres(nullptr), m_nTime(0),m_Handle(NULL)
{
	m_Effect = LibEffekseer::Create(efc);
	m_Manager = LibEffekseer::GetManager();
	m_effects.clear();
}

CEffectManager::CEffectManager(CEffectManager* InAddrres)
	:m_nTime(0)
{
	m_pEffectAddrres = InAddrres;
	m_nEffectNum = InAddrres->GetEffectNum();
	m_Effect = InAddrres->GetEffect();
	m_Manager = InAddrres->GetManager();
	m_Handle = InAddrres->GetHandle();
}

CEffectManager::CEffectManager(CEffectManager* InAddrres, DirectX::XMFLOAT3 pos, int PlayTime)
{
	m_pEffectAddrres = InAddrres;
	m_nEffectNum = InAddrres->GetEffectNum();
	m_Effect = InAddrres->GetEffect();


	if (m_pEffectAddrres)m_Handle = m_pEffectAddrres->GetManager()->Play(m_pEffectAddrres->GetEffect(), pos.x, pos.y, pos.z);
	m_nTime = PlayTime;
}

CEffectManager::~CEffectManager()
{

}

void CEffectManager::Uninit()
{
	m_Manager->StopAllEffects();
}

void CEffectManager::Update()
{
	//auto itr = m_Handle.begin();
	//auto itr2 = m_nPlayTime.begin();
	//for (int i = 0; i < m_Handle.size(); i++)
	//{
	//	if (m_Manager->Exists(*itr))
	//	{
	//		(*itr2)++;
	//		if (*itr2 >= m_nLimit)
	//		{
	//			m_Manager->StopEffect(*itr);
	//			(*itr) = NULL;
	//			(*itr2) = NULL;
	//		}

	//	}
	//	std::advance(itr, 1);
	//	std::advance(itr2, 1);
	//}

	for (auto itr = m_effects.begin(); itr != m_effects.end();)
	{
		int i = m_effects.size();
		itr->startTime++;
		if (m_Manager->Exists(itr->m_Handle))
		{
			if (itr->startTime >= itr->duration)
			{
				m_Manager->StopEffect(itr->m_Handle);
				itr = m_effects.erase(itr);

				continue;
			}
		}
		++itr;
	}


	//static int time = 0;
	//if (m_pEffectAddrres)
	//{
	//	if(m_Manager->Exists(m_Handle))
	//	{
	//		time++;
	//		if (time >= m_nTime)
	//		{
	//			time = 0;
	//			m_Manager->StopEffect(m_Handle);
	//			
	//			delete m_pEffectAddrres;
	//			m_pEffectAddrres = nullptr;
	//		}
	//	}
	//}
	//else
	//{
	//	if (m_Manager->Exists(m_Handle))
	//	{
	//		time++;
	//		if (time >= m_nTime)
	//		{
	//			time = 0;
	//			m_Manager->StopEffect(m_Handle);
	//		}
	//	}
	//}
}

void CEffectManager::Draw()
{
	LibEffekseer::SetViewPosition(GetCameraPos());			// カメラ座標のセット
	LibEffekseer::SetCameraMatrix(GetView(false), GetProj(false));	// view座標,projection座標のセット 
}

void CEffectManager::Play(DirectX::XMFLOAT3 pos, int PlayTime)
{
	Effect efc;
	efc.m_Handle = { m_Manager->Play(m_Effect, pos.x, pos.y, pos.z) };
	efc.startTime = 0;
	efc.duration = PlayTime;
	m_effects.push_back(efc);
	
	//if (m_pEffectAddrres)m_Handle  = m_pEffectAddrres->GetManager()->Play(m_pEffectAddrres->GetEffect(), pos.x, pos.y, pos.z);
	//else m_Handle = m_Manager->Play(m_Effect, pos.x, pos.y, pos.z);

	//m_nTime = PlayTime;
}

void CEffectManager::AllStop()
{
	m_Manager->StopAllEffects();
	m_effects.clear();
}

void CEffectManager::SetPos(DirectX::XMFLOAT3 pos)
{
	auto itr = m_effects.begin();
	Effekseer::Vector3D effectpos = Effekseer::Vector3D(pos.x,pos.y,pos.z);
	m_Manager->AddLocation(m_effects[0].m_Handle, effectpos);
}

int CEffectManager::GetEffectNum()
{
	return m_nEffectNum;
}

Effekseer::ManagerRef CEffectManager::GetManager()
{
	return m_Manager;
}

Effekseer::EffectRef CEffectManager::GetEffect()
{
	return m_Effect;
}

Effekseer::Handle CEffectManager::GetHandle()
{
	return m_Handle;
}

