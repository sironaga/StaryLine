#include "SceneResult.h"

ResultGameInfo CSceneResult::ResultGameData;

CSceneResult::CSceneResult()
{
	// --- Default
	m_pHitPoint			= new SpriteEx("Assets/Texture/Result/Result_Helth.png");
	m_pSummonData		= new SpriteEx("Assets/Texture/Result/Result_Summon.png");
	m_pSelectStage		= new SpriteEx("Assets/Texture/Result/Result_Stageselect.png");

	if (ResultGameData.bWin)
	{
		m_pClearTime	= new SpriteEx("Assets/Texture/Result/Result_Win_Cleartime.png");
		m_pNextStage	= new SpriteEx("Assets/Texture/Result/Result_Nextstage.png");
		m_pText			= new SpriteEx("Assets/Texture/Result/Result_Win_Text_top.png");
		m_pCharacter	= new SpriteEx("Assets/Texture/Result/Result_Win_Linie.png");
		m_pLighting		= new SpriteEx("Assets/Texture/Result/Result_Win_Light.png");
	}
	else
	{
		m_pClearTime	= new SpriteEx("Assets/Texture/Result/Result_Lose_Cleartime.png");
		m_pNextStage	= new SpriteEx("Assets/Texture/Result/Result_Retry.png");
		m_pText			= new SpriteEx("Assets/Texture/Result/Result_Lose_Text_top.png");
		m_pCharacter	= new SpriteEx("Assets/Texture/Result/Result_Lose_Linie.png");
		m_pLighting		= new SpriteEx("Assets/Texture/Result/Result_Lose_Light.png");
	}
}

CSceneResult::~CSceneResult()
{
}

void CSceneResult::Update()
{
}

void CSceneResult::Draw()
{
}

void CSceneResult::InResultData(ResultGameInfo InData)
{
	ResultGameData = InData;
}
