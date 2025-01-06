#include "SceneResult.h"

ResultGameInfo CSceneResult::ResultGameData;



CSceneResult::CSceneResult()
{
	// --- テクスチャの読み込み
	
	// --- Default
	m_pHitPoint			= new SpriteEx("Assets/Texture/Result/Result_Helth.png");
	m_pStageSelect[0]	= new SpriteEx("Assets/Texture/Result/Result_Stageselect.png");
	m_pStageSelect[1]	= new SpriteEx("Assets/Texture/Result/Result_Stageselect_Push.png");
	m_pShadow			= new SpriteEx("Assets/Texture/Result/Result_Shadow.png");
	m_pSelect[0]		= new SpriteEx("Assets/Texture/Result/Result_Stageselect_Frame.png");
	m_pSelect[1]		= new SpriteEx("Assets/Texture/Result/Result_Nextstage_Frame.png");
	m_pSummonData		= new SpriteEx("Assets/Texture/Result/Result_Summon.png");
	m_pUnderBar			= new SpriteEx("Assets/Texture/Result/Result_Underbar.png");

	if (ResultGameData.bWin)
	{
		// --- WinData
		m_pText			= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_top.png");
		m_pCharacter	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Linie.png");
		m_pLighting		= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Light.png");
		m_pTextShadow	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Text_under.png");
		m_pClearTime	= new SpriteEx("Assets/Texture/Result/Win/Result_Win_Cleartime.png");
		m_pNextUI[0]	= new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
		m_pNextUI[1]	= new SpriteEx("Assets/Texture/Result/Win/Result_Nextstage.png");
	}
	else
	{
		// ---- Lose
		m_pText			= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Text_top.png");
		m_pCharacter	= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Linie.png");
		m_pLighting		= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Light.png");
		m_pTextShadow	= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Text_under.png");
		m_pClearTime	= new SpriteEx("Assets/Texture/Result/Lose/Result_Lose_Cleartime.png");
		m_pNextUI[0]	= new SpriteEx("Assets/Texture/Result/Lose/Result_Retry.png");
		m_pNextUI[1]	= new SpriteEx("Assets/Texture/Result/Lose/Result_Retry_Push.png");

	}

	// =====================================================================

	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pStageSelect[nLoop]->SetProjection(Get2DProj());
		m_pStageSelect[nLoop]->SetView(Get2DView());
		m_pSelect[nLoop]->SetProjection(Get2DProj());
		m_pSelect[nLoop]->SetView(Get2DView());
		m_pNextUI[nLoop]->SetProjection(Get2DProj());
		m_pNextUI[nLoop]->SetView(Get2DView());
	}
	m_pHitPoint		->SetProjection(Get2DProj());
	m_pHitPoint		->SetView(Get2DView());
	m_pShadow		->SetProjection(Get2DProj());
	m_pShadow		->SetView(Get2DView());
	m_pSummonData	->SetProjection(Get2DProj());
	m_pSummonData	->SetView(Get2DView());
	m_pUnderBar		->SetProjection(Get2DProj());
	m_pUnderBar		->SetView(Get2DView());
	m_pText			->SetProjection(Get2DProj());
	m_pText			->SetView(Get2DView());
	m_pCharacter	->SetProjection(Get2DProj());
	m_pCharacter	->SetView(Get2DView());
	m_pLighting		->SetProjection(Get2DProj());
	m_pLighting		->SetView(Get2DView());
	m_pLighting		->SetProjection(Get2DProj());
	m_pLighting		->SetView(Get2DView());
	m_pTextShadow	->SetProjection(Get2DProj());
	m_pTextShadow	->SetView(Get2DView());
	m_pClearTime	->SetProjection(Get2DProj());
	m_pClearTime	->SetView(Get2DView());

	nSlect = 0;

}

CSceneResult::~CSceneResult()
{
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		if (m_pStageSelect[nLoop])
		{
			delete m_pStageSelect[nLoop];
			m_pStageSelect[nLoop] = nullptr;
		}
		if (m_pSelect[nLoop])
		{
			delete m_pSelect[nLoop];
			m_pSelect[nLoop] = nullptr;
		}
		if (m_pNextUI[nLoop])
		{
			delete m_pNextUI[nLoop];
			m_pNextUI[nLoop] = nullptr;
		}
	}

	if (m_pHitPoint)
	{
		delete m_pHitPoint;
		m_pHitPoint = nullptr;
	}
	if (m_pShadow)
	{
		delete m_pShadow;
		m_pShadow = nullptr;
	}
	if (m_pSummonData)
	{
		delete m_pSummonData;
		m_pSummonData = nullptr;
	}
	if (m_pUnderBar)
	{
		delete m_pUnderBar;
		m_pUnderBar = nullptr;
	}
	if (m_pText)
	{
		delete m_pText;
		m_pText = nullptr;
	}
	if (m_pCharacter)
	{
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}
	if (m_pLighting)
	{
		delete m_pLighting;
		m_pLighting = nullptr;
	}
	if (m_pTextShadow)
	{
		delete m_pTextShadow;
		m_pTextShadow = nullptr;
	}
	if (m_pClearTime)
	{
		delete m_pClearTime;
		m_pClearTime = nullptr;
	}
}

void CSceneResult::Update()
{
	// --- 更新処理
	for (int nLoop = 0; nLoop < 2; nLoop++)
	{
		m_pStageSelect[nLoop]->SetProjection(Get2DProj());
		m_pStageSelect[nLoop]->SetView(Get2DView());
		m_pSelect[nLoop]->SetProjection(Get2DProj());
		m_pSelect[nLoop]->SetView(Get2DView());
		m_pNextUI[nLoop]->SetProjection(Get2DProj());
		m_pNextUI[nLoop]->SetView(Get2DView());
	}
	m_pHitPoint->SetProjection(Get2DProj());
	m_pHitPoint->SetView(Get2DView());
	m_pShadow->SetProjection(Get2DProj());
	m_pShadow->SetView(Get2DView());
	m_pSummonData->SetProjection(Get2DProj());
	m_pSummonData->SetView(Get2DView());
	m_pUnderBar->SetProjection(Get2DProj());
	m_pUnderBar->SetView(Get2DView());
	m_pText->SetProjection(Get2DProj());
	m_pText->SetView(Get2DView());
	m_pCharacter->SetProjection(Get2DProj());
	m_pCharacter->SetView(Get2DView());
	m_pLighting->SetProjection(Get2DProj());
	m_pLighting->SetView(Get2DView());
	m_pLighting->SetProjection(Get2DProj());
	m_pLighting->SetView(Get2DView());
	m_pTextShadow->SetProjection(Get2DProj());
	m_pTextShadow->SetView(Get2DView());
	m_pClearTime->SetProjection(Get2DProj());
	m_pClearTime->SetView(Get2DView());




}

void CSceneResult::Draw()
{


}

void CSceneResult::InResultData(ResultGameInfo InData)
{
	ResultGameData = InData;
}
