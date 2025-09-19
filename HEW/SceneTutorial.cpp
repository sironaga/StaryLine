#include "SceneTutorial.h"
#include "InputEx.h"
#include "Main.h"

CSceneTutorial::CSceneTutorial()
	: m_bEnd(false)
{
}

CSceneTutorial::~CSceneTutorial()
{

}

void CSceneTutorial::Update()
{
	if (IsKeyTrigger(VK_ESCAPE) && !m_bEnd)
	{
		m_bEnd = true;
		SetNext(STAGE_SELECT);
	}
}

void CSceneTutorial::Draw()
{
}
