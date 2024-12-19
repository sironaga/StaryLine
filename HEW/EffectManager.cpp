#include "EffectManager.h"
#include "Main.h"

CEffectManager::CEffectManager(const char* efc)
	:m_nLimit(0)
{
	m_Effect = LibEffekseer::Create(efc);
	m_Manager = LibEffekseer::GetManager();
	m_nPlayTime.clear();
}

CEffectManager::~CEffectManager()
{
	m_Effect->Release();
	m_Manager->Release();
}

void CEffectManager::Update()
{
	//auto itr2 = m_nPlayTime.begin();
	//for (auto itr = m_Handle.begin(); itr != m_Handle.end();)
	//{
	//	if (m_Manager->Exists(*itr))
	//	{
	//		(*itr2)++;
	//		if (*itr2 >= m_nLimit)
	//		{
	//			m_Manager->StopEffect(*itr);
	//			m_Handle.erase(itr);
	//			m_nPlayTime.erase(itr2);
	//		}
	//		else
	//		{
	//			std::advance(itr, 1);
	//			std::advance(itr2, 1);
	//		}

	//	}

	//}
	if (m_Manager->GetDrawTime() >= m_nLimit * 6) m_Manager->StopEffect(m_Handle);
}

void CEffectManager::Draw()
{
	SetRender3D();
	LibEffekseer::SetViewPosition(GetCameraPos());			// カメラ座標のセット
	LibEffekseer::SetCameraMatrix(GetView(false), GetProj(false));	// view座標,projection座標のセット 
}

void CEffectManager::Play(DirectX::XMFLOAT3 pos,int PlayTime)
{
	m_nLimit = PlayTime;
	m_Handle = m_Manager->Play(m_Effect, pos.x, pos.y, pos.z);
	m_Manager->SetTimeScaleByHandle(m_Handle, 1.0f);		//スピード
	
}

Effekseer::ManagerRef CEffectManager::GetManager()
{
	return m_Manager;
}
