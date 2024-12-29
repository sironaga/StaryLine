#include "Scene.h"

CScene::CScene()
	: m_pFade(nullptr), m_next(-1) 
{

}

CScene::~CScene()
{
	m_pFade = nullptr;
}

void CScene::RootUpdate()
{
	Update();
	if (m_pFade) m_pFade->Update();
}

void CScene::RootDraw()
{
	Draw();
	if (m_pFade) m_pFade->Draw();
}

bool CScene::ChangeScene()
{
	if (m_pFade)
		return m_pFade->IsFinish() && m_pFade->IsFadeOut();
	return false;
}

int CScene::NextScene()
{
	return m_next;
}

void CScene::SetNext(int next)
{
	m_next = next;
	// 切り替え先が発生した際にフェードも実行 
	if (m_pFade)
		m_pFade->Start(false);
}

void CScene::SetFade(CFade* fade)
{
	m_pFade = fade;
}