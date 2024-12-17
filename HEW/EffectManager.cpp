#include "EffectManager.h"
#include "Main.h"

CEffectManager::CEffectManager(const char* efc)
	:m_nLimit(0)
{
	m_Effect = LibEffekseer::Create(efc);
	m_Manager = LibEffekseer::GetManager();
}

CEffectManager::~CEffectManager()
{

}

void CEffectManager::Update()
{
	auto itr2 = m_nPlayTime.begin();
	for (auto itr = m_Handle.begin(); itr != m_Handle.end();)
	{
		if (m_Manager->Exists(*itr))
		{
			(*itr2)++;
			if (*itr2 >= m_nLimit)
			{
				m_Manager->StopEffect(*itr);

			}

		}
		std::advance(itr, 1);
		std::advance(itr2, 1);
	}
}

void CEffectManager::Draw()
{
	LibEffekseer::SetViewPosition(GetCameraPos());			// カメラ座標のセット
	LibEffekseer::SetCameraMatrix(GetView(false), GetProj(false));	// view座標,projection座標のセット 
}

void CEffectManager::Play(DirectX::XMFLOAT3 pos,int PlayTime)
{
	m_Handle.push_back(m_Manager->Play(m_Effect, pos.x, pos.y, pos.z));
	m_nPlayTime.push_back(0);
	m_nLimit = PlayTime;
}

Effekseer::ManagerRef CEffectManager::GetManager()
{
	return m_Manager;
}
